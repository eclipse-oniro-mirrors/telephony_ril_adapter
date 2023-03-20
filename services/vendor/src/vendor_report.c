/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#include "vendor_report.h"

#include "at_call.h"
#include "at_sms.h"
#include "vendor_adapter.h"

#include "hril_notification.h"

static const struct HRilReport *g_reportOps = NULL;
static const size_t ZERO_RESPONSE_LEN = 0;

void OnModemReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnModemReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnCallReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnCallReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnDataReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnDataReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnSmsReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnSmsReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnNetworkReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnNetworkReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnSimReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnSimReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnTimerCallback(HRilCallbackFun func, uint8_t *param, const struct timeval *tv)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnTimerCallback(func, param, tv);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

struct ReportInfo CreateReportInfo(const ReqDataInfo *requestInfo, uint32_t err, uint32_t type, int32_t notifyId)
{
    struct ReportInfo reportInfo = {(ReqDataInfo *)requestInfo, notifyId, type, err, {0, 0}};
    return reportInfo;
}

void SetReportOps(const struct HRilReport *reportOps)
{
    g_reportOps = reportOps;
}

static void ReportCBMOrCSCB(struct ReportInfo *reportInfo)
{
    if (reportInfo == NULL) {
        TELEPHONY_LOGE("reportInfo is null");
        return;
    }
    static int32_t testCount = 0;
    char *tempData = NULL;
    char *testDataStr =
        ("01a41f51101102ea3030a830ea30a230e130fc30eb914d4fe130c630b930c8000d000a305330"
         "8c306f8a669a137528306e30e130c330bb30fc30b8306730593002000d000aff0800320030003"
         "10033002f00310031002f003252ea300037002000310035003a00340034ff09000d000aff0830"
         "a830ea30a25e02ff0900000000000000000000000000000000000000"
         "000000000000000000000000000000000000000000000000000000000022");
    char *testDataTmp =
        ("C0000032401174747A0E4ACF41E8B0BCFD76E741EF39685C66B34162F93B4C1E87E77410BD3CA7836EC2341D440ED3C321");
    if (testCount == 0) {
        tempData = testDataStr;
        testCount++;
    } else {
        tempData = testDataTmp;
        testCount = 0;
    }
    HRilCBConfigReportInfo response = {0};
    reportInfo->notifyId = HNOTI_CB_CONFIG_REPORT;
    int32_t ret = ProcessCellBroadcast("+CBM:100", &response);
    if (ret > 1) {
        response.data = (char *)tempData;
    } else {
        response.pdu = (char *)tempData;
    }
    OnSmsReport(GetSlotId(NULL), *reportInfo, (const uint8_t *)&response, sizeof(HRilCBConfigReportInfo));
}

static void SmsStatus(const char *smsPdu, struct ReportInfo *reportInfo)
{
    if (smsPdu == NULL || reportInfo == NULL) {
        TELEPHONY_LOGE("reportInfo is NULL");
        return;
    }
    int32_t size = (smsPdu != NULL) ? strlen(smsPdu) : 0;
    reportInfo->notifyId = HNOTI_SMS_STATUS_REPORT;
    OnSmsReport(GetSlotId(NULL), *reportInfo, (const uint8_t *)smsPdu, size);
}

static void ReportInfoInit(struct ReportInfo *reportInfo)
{
    if (reportInfo == NULL) {
        TELEPHONY_LOGE("reportInfo is NULL");
        return;
    }
    reportInfo->error = HRIL_ERR_SUCCESS;
    reportInfo->type = HRIL_NOTIFICATION;
}

static int32_t CdmaSmsNotifyMock(const char *s, HRilSmsResponse *smsResponse)
{
    if (s == NULL || smsResponse == NULL) {
        return 0;
    }
    char *testDataStr = ("0101020004081300031008d00106102c2870e1420801c00c01c0");
    char *testDataTmp =
        ("0000021002040602448d159e240601fc081b0003200010010910461c58d8b266a9180306211024102051080100");
    if (ReportStrWith(s, "+CMMS:")) {
        smsResponse->pdu = testDataTmp;
    } else if (ReportStrWith(s, "+CSDH:")) {
        smsResponse->pdu = testDataStr;
    } else {
        return 0;
    }
    return HNOTI_SMS_NEW_CDMA_SMS;
}

static int32_t WapPushNotifyMock(const char *s, HRilSmsResponse *smsResponse)
{
    if (s == NULL || smsResponse == NULL) {
        return 0;
    }
    char *testDataStr =
        ("0891683108200105f04408a0015608860104216092902512236e0605040b8423f0120601ae020"
         "56a0045c60c033231312e3133362e3130372e37382f646f776e2e7068703f703d413063303026"
         "733d383500080103e68e8ce68fa1e882a1e5b882e58588e69cbaefbc8ce6aca2e8bf8ee4bdbfe"
         "794a8e6898be69cbae8af81e588b8e38082000101");
    char *testDataTmp =
        ("0041000B915121551532F400042E0B05040B84C0020003F001010A060403B0"
         "81EA02066A008509036D6F62696C65746964696E67732E636F6D2F0001");
    if (ReportStrWith(s, "+CMGF:")) {
        smsResponse->pdu = testDataStr;
    } else if (ReportStrWith(s, "+CSMP:")) {
        smsResponse->pdu = testDataTmp;
    } else {
        return 0;
    }
    return HNOTI_SMS_NEW_SMS;
}

void OnNotifyOps(const char *s, const char *smsPdu)
{
    if (s == NULL) {
        return;
    }
    char *str = NULL;
    struct ReportInfo reportInfo = {0};
    ReportInfoInit(&reportInfo);
    if (GetRadioState() == HRIL_RADIO_POWER_STATE_UNAVAILABLE) {
        return;
    }
    str = strdup(s);
    if (str == NULL) {
        return;
    }
    if (IsCallNoticeCmd(s)) {
        CallReportInfoProcess(s);
    } else if (ReportStrWith(s, "+CMT:")) {
        HRilSmsResponse smsResponse = {};
        smsResponse.pdu = (char *)smsPdu;
        reportInfo.notifyId = HNOTI_SMS_NEW_SMS;
        OnSmsReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&smsResponse, strlen(smsResponse.pdu));
    } else if (ReportStrWith(s, "+CDS:")) {
        SmsStatus(smsPdu, &reportInfo);
    } else if (ReportStrWith(s, "+CBM:") || ReportStrWith(s, "+CSCB:")) {
        ReportCBMOrCSCB(&reportInfo);
        ReportCBMOrCSCB(&reportInfo); // The test requires send twice
    } else if (ReportStrWith(s, "+COPS: (")) {
        char *copsStr = strdup(s);
        ProcessOperListToUse(copsStr);
        free(copsStr);
    } else if (ReportStrWith(s, "^SIMST:")) {
        reportInfo.notifyId = HNOTI_SIM_STATUS_CHANGED;
        OnSimReport(GetSlotId(NULL), reportInfo, NULL, 0);
    } else if (ReportStrWith(s, "^MONSC:")) {
        ProcessCurrentCellList(reportInfo, str);
    } else if (ReportStrWith(s, "^DATACONNECT") || ReportStrWith(s, "^DATADISCONN")) {
        PdpContextListUpdate();
    } else if (OnNotifyStkOps(s, str)) {
        TELEPHONY_LOGI("STK notify completed.");
    } else {
        HRilSmsResponse response = {0};
        if ((reportInfo.notifyId = CdmaSmsNotifyMock(s, &response)) > 0) {
            OnSmsReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&response, sizeof(HRilSmsResponse));
        }
        if ((reportInfo.notifyId = WapPushNotifyMock(s, &response)) > 0) {
            OnSmsReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&response, strlen(response.pdu));
        }
        OnNotifyNetWorksOps(s, str);
    }
    free(str);
}

static int32_t ParseStkResponseStr(const char *s, char **cmdResponseInfo)
{
    char *str = (char *)s;
    if (str == NULL) {
        TELEPHONY_LOGE("ProcessStkNotify, s or cmdResponse param is null");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t err = SkipATPrefix(&str);
    if (err != VENDOR_SUCCESS) {
        TELEPHONY_LOGE("ProcessStkNotify, invalid response");
        return HRIL_ERR_INVALID_RESPONSE;
    }
    err = NextStr(&str, cmdResponseInfo);
    TELEPHONY_LOGD("ParseStkResponseStr, cmdResponse: %{public}s", *cmdResponseInfo);
    if (err != 0) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    return VENDOR_SUCCESS;
}

bool OnNotifyStkOps(const char *s, const char *strInfo)
{
    bool isStkNotify = true;
    struct ReportInfo reportInfo = {0};
    reportInfo.error = HRIL_ERR_SUCCESS;
    reportInfo.type = HRIL_NOTIFICATION;
    if (ReportStrWith(s, "SoftwareVersion:")) {
        reportInfo.notifyId = HNOTI_SIM_STK_SESSION_END_NOTIFY;
        OnSimReport(GetSlotId(NULL), reportInfo, NULL, ZERO_RESPONSE_LEN);
    } else if (ReportStrWith(s, "HardwareVersion:")) {
        reportInfo.notifyId = HNOTI_SIM_STK_PROACTIVE_NOTIFY;
        char *cmdResponse = (char *)strInfo;
        int32_t ret = ParseStkResponseStr(s, &cmdResponse);
        if (ret != VENDOR_SUCCESS) {
            reportInfo.error = ret;
        }
        if (cmdResponse == NULL) {
            TELEPHONY_LOGE("cmdResponse is NULL");
            OnSimReport(GetSlotId(NULL), reportInfo, NULL, ZERO_RESPONSE_LEN);
        } else {
            TELEPHONY_LOGD("OnNotifyStkOps, cmdResponse: %{public}s", cmdResponse);
            OnSimReport(GetSlotId(NULL), reportInfo, (const uint8_t *)cmdResponse, sizeof(char));
        }
    } else if (ReportStrWith(s, "+CGMM:")) {
        reportInfo.notifyId = HNOTI_SIM_STK_ALPHA_NOTIFY;
        OnSimReport(GetSlotId(NULL), reportInfo, NULL, ZERO_RESPONSE_LEN);
    } else {
        isStkNotify = false;
    }
    return isStkNotify;
}

static void OnCsRegStatusNotify(struct ReportInfo reportInfo, int32_t ret, char *str, const char *s)
{
    reportInfo.notifyId = HNOTI_NETWORK_CS_REG_STATUS_UPDATED;
    HRilRegStatusInfo regStatusInfo;
    ret = ProcessRegStatus(str, &regStatusInfo);
    if (ret == 0) {
        OnNetworkReport(GetSlotId(NULL), reportInfo, (const uint8_t *)(&regStatusInfo), sizeof(HRilRegStatusInfo));
    } else {
        TELEPHONY_LOGW("CREG notify str format  unexpected: %{public}s", s);
    }
}

static void RadioTurnNotify(struct ReportInfo reportInfo, char *str)
{
    HRilRadioState radioState = HRIL_RADIO_POWER_STATE_UNAVAILABLE;
    if (ReportStrWith(str, "^RADIO: 1")) {
        radioState = HRIL_RADIO_POWER_STATE_ON;
    } else if (ReportStrWith(str, "^RADIO: 0")) {
        radioState = HRIL_RADIO_POWER_STATE_OFF;
    } else {
        TELEPHONY_LOGW("^RADIO notify str format  unexpected: %{public}s", str);
    }
    if (radioState != HRIL_RADIO_POWER_STATE_UNAVAILABLE) {
        reportInfo.error = HRIL_ERR_SUCCESS;
        reportInfo.type = HRIL_NOTIFICATION;
        reportInfo.notifyId = HNOTI_MODEM_RADIO_STATE_UPDATED;
        OnModemReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&radioState, sizeof(HRilRadioState));
    }
}

static void OnPsRegStatusNotify(struct ReportInfo reportInfo, int32_t ret, char *str, const char *s)
{
    reportInfo.notifyId = HNOTI_NETWORK_PS_REG_STATUS_UPDATED;
    HRilRegStatusInfo regStatusInfo;
    ret = ProcessRegStatus(str, &regStatusInfo);
    if (ret == 0) {
        OnNetworkReport(GetSlotId(NULL), reportInfo, (const uint8_t *)(&regStatusInfo), sizeof(HRilRegStatusInfo));
    } else {
        TELEPHONY_LOGW("CGREG notify str format  unexpected: %{public}s", s);
    }
}

static void OnNotifyNetWorksOpsJudgeTwo(
    struct ReportInfo reportInfo, const char *infoStr, char *responseData[MAX_REG_INFO_ITEM])
{
    reportInfo.notifyId = HNOTI_NETWORK_TIME_UPDATED;
    char *time[DEFAULT_INDEX] = {""};
    if (GenerateCommand((char *)time, DEFAULT_INDEX, "^TIME:\"20%s", infoStr + DEFAULT_ADD_NUM) < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnNetworkReport(GetSlotId(NULL), reportInfo, (const uint8_t *)responseData, MAX_REG_INFO_ITEM * sizeof(char));
        return;
    }
    TELEPHONY_LOGW("Report TIME: %{public}s", (char *)time);
    OnNetworkReport(GetSlotId(NULL), reportInfo, (const uint8_t *)time, MAX_REG_INFO_ITEM * sizeof(char));
}

static void SignalStrengthNotify(struct ReportInfo reportInfo, int32_t ret, char *str, const char *s)
{
    HRilRssi response = {0};
    reportInfo.notifyId = HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED;
    TELEPHONY_LOGD("start report SignalStrengthNotify ");
    ret = ProcessParamSignalStrengthNotify(str, &response);
    if (ret == 0) {
        OnNetworkReport(GetSlotId(NULL), reportInfo, (const uint8_t *)(&response), sizeof(HRilRssi));
    } else {
        TELEPHONY_LOGW("HCSQ notify str format  unexpected: %{public}s", s);
    }
}

static void VoiceRadioInfoNotify(struct ReportInfo reportInfo, int32_t ret, char *str, const char *s)
{
    HRilVoiceRadioInfo voiceRadioInfo = {0};
    ret = ProcessVoiceRadioInfo(str, &voiceRadioInfo);
    if (ret != 0) {
        TELEPHONY_LOGE("ProcessVoiceRadioInfo format  unexpected: %{public}s", s);
        return;
    }
    reportInfo.notifyId = HNOTI_MODEM_VOICE_TECH_UPDATED;
    OnModemReport(GetSlotId(NULL), reportInfo, (const uint8_t *)(&voiceRadioInfo), sizeof(HRilVoiceRadioInfo));
}

void OnNotifyNetWorksOps(const char *s, const char *infoStr)
{
    int32_t ret = 0;
    char *str = (char *)infoStr;
    char *responseData[MAX_REG_INFO_ITEM] = {""};
    struct ReportInfo reportInfo = {0};
    reportInfo.error = HRIL_ERR_SUCCESS;
    reportInfo.type = HRIL_NOTIFICATION;
    if (ReportStrWith(s, "+CREG:")) {
        OnCsRegStatusNotify(reportInfo, ret, str, s);
    } else if (ReportStrWith(s, "+CGREG:")) {
        OnPsRegStatusNotify(reportInfo, ret, str, s);
    } else if (ReportStrWith(s, "^TIME:")) {
        OnNotifyNetWorksOpsJudgeTwo(reportInfo, infoStr, responseData);
    } else if (ReportStrWith(s, "+CTZV:")) {
        reportInfo.notifyId = HNOTI_NETWORK_TIME_ZONE_UPDATED;
        OnNetworkReport(GetSlotId(NULL), reportInfo, (const uint8_t *)responseData, MAX_REG_INFO_ITEM * sizeof(char));
    } else if (ReportStrWith(s, "^HCSQ:")) {
        SignalStrengthNotify(reportInfo, ret, str, s);
    } else if (ReportStrWith(s, "^RADIO:")) {
        RadioTurnNotify(reportInfo, str);
    } else if (ReportStrWith(s, "^PHYCHLCFG:")) {
        ProcessPhyChnlCfgNotify(reportInfo, str);
    } else if (ReportStrWith(s, "^SYSINFOEX:")) {
        VoiceRadioInfoNotify(reportInfo, ret, str, s);
    } else {
        TELEPHONY_LOGW("enter to  is unrecognized command");
    }
}
