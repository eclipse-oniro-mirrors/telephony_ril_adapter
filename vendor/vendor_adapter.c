/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "vendor_adapter.h"

#include <fcntl.h>
#include <termios.h>

#include "at_call.h"
#include "at_data.h"
#include "at_modem.h"
#include "at_network.h"
#include "at_sim.h"
#include "at_sms.h"
#include "at_support.h"
#include "vendor_report.h"

#define DEVICE_PATH "/dev/ttyUSB1"
#define DEVICE_PATH_DEFAULT "/dev/ttyUSB"
#define MAX_BUFF_SIZE 500

static HRilRadioState g_radioState = HRIL_RADIO_POWER_STATE_UNAVAILABLE;
static pthread_mutex_t g_statusMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_statusCond = PTHREAD_COND_INITIALIZER;
static pthread_t g_eventListeners = 0;
static int g_fd = -1;
static int g_atStatus = 0;
const struct HRilReport *g_reportOps = NULL;

static const HRilCallReq g_callReqOps = {
    .GetCallList = ReqGetCallList,
    .Dial = ReqDial,
    .Hangup = ReqHangup,
    .Reject = ReqReject,
    .Answer = ReqAnswer,
    .GetClip = ReqGetClip,
    .SetClip = ReqSetClip,
    .Hold = ReqHold,
    .Active = ReqActive,
    .Swap = ReqSwap,
    .Join = ReqJoin,
    .Split = ReqSplit,
    .CallSupplement = ReqCallSupplement,
    .GetCallWait = ReqGetCallWait,
    .SetCallWait = ReqSetCallWait,
    .GetCallForwarding = ReqGetCallForwarding,
    .SetCallForwarding = ReqSetCallForwarding,
    .GetCallRestriction = ReqGetCallRestriction,
    .SetCallRestriction = ReqSetCallRestriction,
    .GetClir = ReqGetClir,
    .SetClir = ReqSetClir,
    .StartDtmf = ReqStartDtmf,
    .SendDtmf = ReqSendDtmf,
    .StopDtmf = ReqStopDtmf,
};

static const HRilSimReq g_simReqOps = {
    .GetSimStatus = ReqGetSimStatus,
    .GetSimImsi = ReqGetSimImsi,
    .GetSimIO = ReqGetSimIO,
    .GetSimIccID = ReqGetSimIccID,
    .GetSimLockStatus = ReqGetSimLockStatus,
    .SetSimLock = ReqSetSimLock,
    .ChangeSimPassword = ReqChangeSimPassword,
    .EnterSimPin = ReqEnterSimPin,
    .UnlockSimPin = ReqUnlockSimPin,
    .GetSimPinInputTimes = ReqGetSimPinInputTimes,
};

static const HRilSmsReq g_smsReqOps = {
    .SendSms = ReqSendSms,
    .SendSmsAck = ReqSendSmsAck,
    .StorageSms = ReqStorageSms,
    .DeleteSms = ReqDeleteSms,
    .UpdateSms = ReqUpdateSms,
    .SetSmsCenterAddress = ReqSetSmsCenterAddress,
    .GetSmsCenterAddress = ReqGetSmsCenterAddress,
    .SetCellBroadcast = ReqSetCellBroadcast,
};

static const HRilNetworkReq g_networkReqOps = {
    .GetSignalStrength = ReqGetSignalStrength,
    .GetCsRegStatus = ReqGetCsRegStatus,
    .GetPsRegStatus = ReqGetPsRegStatus,
    .GetOperatorInfo = ReqGetOperatorInfo,
    .GetNetworkSearchInformation = RequestGetNetworkSearchInformation,
    .GetNetworkSelectionMode = RequestQueryNetworkSelectionMode,
    .SetNetworkSelectionMode = RequestSetAutomaticModeForNetworks,
};

static const HRilDataReq g_dataReqOps = {
    .ActivatePdpContext = ReqActivatePdpContext,
    .DeactivatePdpContext = ReqDeactivatePdpContext,
};

static const HRilModemReq g_modemReqOps = {
    .SetRadioStatus = ReqSetRadioStatus,
    .GetRadioStatus = ReqGetRadioStatus,
};

HRilOps g_hrilOps = {
    .callOps = &g_callReqOps,
    .simOps = &g_simReqOps,
    .smsOps = &g_smsReqOps,
    .networkOps = &g_networkReqOps,
    .dataOps = &g_dataReqOps,
    .modemOps = &g_modemReqOps,
};

HRilRadioState GetRadioState(void)
{
    return g_radioState;
}

int SetRadioState(HRilRadioState newState, int rst)
{
    char cmd[MAX_BUFF_SIZE] = {0};
    int err = 0;
    ResponseInfo *pResponse = NULL;
    HRilRadioState oldState;
    const int timeOut = 10000;
    (void)memset_s(&oldState, sizeof(HRilRadioState), 0, sizeof(HRilRadioState));
    struct ReportInfo reportInfo = {};
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    if (g_atStatus > 0) {
        newState = HRIL_RADIO_POWER_STATE_UNAVAILABLE;
        pthread_cond_signal(&g_statusCond);
        return -1;
    }
    TELEPHONY_LOGD("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    pthread_mutex_lock(&g_statusMutex);
    oldState = g_radioState;
    if (oldState != newState || g_atStatus > 0) {
        g_radioState = newState;
        pthread_cond_broadcast(&g_statusCond);
    }
    pthread_mutex_unlock(&g_statusMutex);

    if (oldState != g_radioState) {
        (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CFUN=%u,%d", newState, rst);
        err = SendCommandLock(cmd, NULL, timeOut, &pResponse);
        if (err != 0 || !pResponse->success) {
            TELEPHONY_LOGE("AT+CFUN send failed");
            FreeResponseInfo(pResponse);
            return -1;
        }
    } else {
        TELEPHONY_LOGD("now then is same state");
        return HRIL_ERR_REPEAT_STATUS;
    }
    FreeResponseInfo(pResponse);
    reportInfo.notifyId = HNOTI_MODEM_RADIO_STATE_UPDATED;
    reportInfo.type = HRIL_NOTIFICATION;
    reportInfo.error = HRIL_ERR_SUCCESS;
    OnModemReport(reportInfo, &g_radioState, sizeof(HRilRadioState));
    return 0;
}

static void AtOnUnusual(void)
{
    ATCloseReadLoop();

    g_atStatus = 1;
    g_fd = -1;

    SetRadioState(HRIL_RADIO_POWER_STATE_OFF, 0);
}

static void WaitAtClose(void)
{
    pthread_mutex_lock(&g_statusMutex);

    while (g_atStatus == 0) {
        pthread_cond_wait(&g_statusCond, &g_statusMutex);
    }

    pthread_mutex_unlock(&g_statusMutex);
}

static int ModemInit(void)
{
    int err = 0;
    ResponseInfo *pResponse = NULL;

    TELEPHONY_LOGD("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    SetRadioState(HRIL_RADIO_POWER_STATE_ON, 0);

    err = SendCommandLock("ATE0Q0V1", NULL, 0, &pResponse);
    if (err != 0 || pResponse->success == 0) {
        TELEPHONY_LOGE("ATE0Q0V1 send failed");
    }
    FreeResponseInfo(pResponse);
    /* Network registration events */
    err = SendCommandLock("AT+CREG=2", NULL, 0, &pResponse);
    if (err != 0 || pResponse->success == 0) {
        SendCommandLock("AT+CREG=1", NULL, 0, &pResponse);
    }
    FreeResponseInfo(pResponse);
    /* Set the SMS service type to Phase 2+ version */
    SendCommandLock("AT+CSMS=1", NULL, 0, NULL);
    /* Set the new SMS reporting method to +CMTI */
    SendCommandLock("AT+CNMI=1,2,0,1,1", NULL, 0, NULL);
    /* Enable active reporting of (U)SIM status */
    SendCommandLock("AT^SIMST=1", NULL, 0, NULL);
    /* Disabled  auto-answer */
    SendCommandLock("ATS0=0", NULL, 0, NULL);
    /* Extended errors */
    SendCommandLock("AT+CMEE=1", NULL, 0, NULL);
    /* GPRS registration events */
    err = SendCommandLock("AT+CGREG=2", NULL, 0, &pResponse);
    if (err != 0 || pResponse->success == 0) {
        SendCommandLock("AT+CGREG=1", NULL, 0, &pResponse);
    }
    FreeResponseInfo(pResponse);
    SendCommandLock("AT^HCSQ=0", NULL, 0, NULL);
    /*  Call Waiting notifications */
    SendCommandLock("AT+CCWA=1", NULL, 0, NULL);
    /* Disabled muted */
    SendCommandLock("AT+CMUT=0", NULL, 0, NULL);
    /* Enabled CSSU unsolicited supp service notifications */
    SendCommandLock("AT+CSSN=0,1", NULL, 0, NULL);
    /* Set SMS PDU mode */
    SendCommandLock("AT+CMGF=0", NULL, 0, NULL);
    /* Set UNICODE character */
    SendCommandLock("AT+CSCS=\"IRA\"", NULL, 0, NULL);
    /* Set sms memory */
    SendCommandLock("AT+CPMS=\"SM\",\"SM\",\"SM\"", NULL, 0, NULL);
    /* Set to open network time reporting */
    SendCommandLock("AT^TIME=1", NULL, 0, NULL);
    /* Set to open network time zone reporting */
    SendCommandLock("AT+CTZR=1", NULL, 0, NULL);

    sleep(SLEEP_TIME);
    TELEPHONY_LOGD("enter to : ModemInit OnModemReport %{public}d", g_radioState);
    struct ReportInfo reportInfo = {};
    reportInfo.notifyId = HNOTI_MODEM_RADIO_STATE_UPDATED;
    reportInfo.type = HRIL_NOTIFICATION;
    reportInfo.error = HRIL_ERR_SUCCESS;
    OnModemReport(reportInfo, &g_radioState, sizeof(HRilRadioState));
    return err;
}

static void *EventListeners(void *param)
{
    const int RETRY_TIMEOUT_SEC = 60;
    int ret = -1;
    const char *devicePath = DEVICE_PATH;
    AtSetOnUnusual(AtOnUnusual);

    for (;;) {
        while (g_fd < 0) {
            if (devicePath != NULL) {
                g_fd = open(devicePath, O_RDWR);
            }
            if (g_fd >= 0 && !memcmp(devicePath, DEVICE_PATH_DEFAULT, sizeof(DEVICE_PATH_DEFAULT) - 1)) {
                struct termios ios;
                tcgetattr(g_fd, &ios);
                ios.c_lflag = 0;
                tcsetattr(g_fd, TCSANOW, &ios);
            }
            if (g_fd < 0) {
                TELEPHONY_LOGE("ril vendorlib,opening AT interface. retrying...");
                sleep(RETRY_TIMEOUT_SEC);
            }
        }
        g_atStatus = 0;
        ret = ATStartReadLoop(g_fd, OnNotifyOps);

        if (ret < 0) {
            TELEPHONY_LOGE("AtRead error %d %{public}p\n", ret, param);
            return 0;
        }
        ModemInit();
        sleep(1);
        WaitAtClose();
        TELEPHONY_LOGD("ReOpening after close");
    }
}

const HRilOps *RilInitOps(const struct HRilReport *reportOps)
{
    int ret = -1;
    pthread_attr_t attr;
    g_reportOps = reportOps;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&g_eventListeners, &attr, EventListeners, NULL);
    if (ret < 0) {
        TELEPHONY_LOGE("EventListeners create failed %d \n", ret);
    }
    if (g_hrilOps.smsOps == NULL) {
        TELEPHONY_LOGE("g_hrilOps.smsOps is null");
    }
    TELEPHONY_LOGD("g_hrilOps.smsOps ：%{public}p", g_hrilOps.smsOps);
    return &g_hrilOps;
}
