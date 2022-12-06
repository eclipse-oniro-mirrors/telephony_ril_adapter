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

#include "at_call.h"

#include "hril_notification.h"
#include "securec.h"
#include "vendor_report.h"
#include "vendor_util.h"

#undef DEFAULT_TIMEOUT
#define DEFAULT_TIMEOUT 5000
#define DEFAULT_TIMEOUT_CLCK 50000

CallNotify g_callNotifyTab[] = {
    {"^CCALLSTATE:", ReportCallStateUpdated},
    {"+CUSD:", ReportCallUssdNotice},
    {"+CIREPH:", ReportSrvccStatusUpdate},
    {"^CSCHANNELINFO:", ReportCsChannelInfo},
    {"^XLEMA:", ReportEmergencyNumberList},
};

static int32_t lastCcCause = HRIL_ERR_CALL_CAUSE;

static void OnCallReportErrorMessages(const ReqDataInfo *requestInfo, uint32_t err, ResponseInfo *pResponse)
{
    uint32_t errorNo = HRIL_ERR_SUCCESS;
    ModemReportErrorInfo errInfo = GetReportErrorInfo(pResponse);
    errorNo = (err != HRIL_ERR_SUCCESS) ? err : errInfo.errorNo;
    TELEPHONY_LOGW("Report error! ret:%{public}d", errorNo);
    FreeResponseInfo(pResponse);
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, errorNo, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}

static int32_t ParseDiffPart(int32_t isAllInfo, char **pLine, HRilCallInfo *outCall)
{
    if (outCall == NULL) {
        return HRIL_ERR_NULL_POINT;
    }
    int32_t tmp = 0;
    if (isAllInfo) {
        if (NextInt(pLine, &outCall->voiceDomain) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
        if (NextInt(pLine, &outCall->callType) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
        NextInt(pLine, &tmp); // ignore
    } else {
        outCall->voiceDomain = INT_DEFAULT_VALUE;
        outCall->callType = INT_DEFAULT_VALUE;
    }
    return 0;
}

static int32_t CallCmdCLCC(const char *lineCmd, HRilCallInfo *outCall)
{
    char *pLine = (char *)lineCmd;
    int32_t state;
    int32_t mode;

    if (pLine == NULL || outCall == NULL) {
        TELEPHONY_LOGE("src or desc pointer is null.");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t isAllInfo = ReportStrWith(pLine, "^CLCC:");
    if (SkipATPrefix(&pLine) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->index) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->dir) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &state) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    outCall->state = (HRilCallState)state;
    if (NextInt(&pLine, &mode) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    outCall->mode = (HRilCallMode)mode;
    if (NextInt(&pLine, &outCall->mpty) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (ParseDiffPart(isAllInfo, &pLine, outCall)) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextStr(&pLine, &outCall->number) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->type) < 0) {
        return HRIL_ERR_NULL_POINT;
    }

    if (pLine != NULL) { // The data returned by some modules does not have this alpha data.
        if (NextStr(&pLine, &outCall->alpha) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
    }
    return HRIL_ERR_SUCCESS;
}

void ReportCallStateUpdated(const char *str)
{
    int32_t err = HRIL_ERR_SUCCESS;
    char *pStr = (char *)str;
    int callId = 0;
    int voiceDomain = 0;
    int state = 0;
    ReqDataInfo requestInfo = {0};
    ModemReportErrorInfo errInfo = InitModemReportErrorInfo();

    if (SkipATPrefix(&pStr) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextInt(&pStr, &callId) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextInt(&pStr, &state) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextInt(&pStr, &voiceDomain) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }

    struct ReportInfo reportInfo = CreateReportInfo(&requestInfo, err, HRIL_NOTIFICATION, HNOTI_CALL_STATE_UPDATED);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(NULL), reportInfo, NULL, 0);
}

void ReportSrvccStatusUpdate(const char *str)
{
    int32_t err = HRIL_ERR_SUCCESS;
    char *pStr = (char *)str;
    HRilCallSrvccStatus srvccStatus = {0};
    ReqDataInfo requestInfo = {0};
    ModemReportErrorInfo errInfo = InitModemReportErrorInfo();

    if (SkipATPrefix(&pStr) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (err == HRIL_ERR_SUCCESS && NextInt(&pStr, &srvccStatus.status) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }

    struct ReportInfo reportInfo =
        CreateReportInfo(&requestInfo, err, HRIL_NOTIFICATION, HNOTI_CALL_SRVCC_STATUS_REPORT);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&srvccStatus, sizeof(HRilCallSrvccStatus));
}

void ReportCsChannelInfo(const char *str)
{
    int32_t err = HRIL_ERR_SUCCESS;
    char *pStr = (char *)str;
    HRilCallCsChannelInfo csChannelInfo = {0};
    ReqDataInfo requestInfo = {0};
    ModemReportErrorInfo errInfo = InitModemReportErrorInfo();
    /* 0 network alerting; 1 local alerting */
    int32_t ringbackVoiceFlag = 0;

    if (SkipATPrefix(&pStr) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (err == HRIL_ERR_SUCCESS && NextInt(&pStr, &csChannelInfo.status) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (err == HRIL_ERR_SUCCESS && NextInt(&pStr, &csChannelInfo.voiceDomain) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }

    ringbackVoiceFlag = !csChannelInfo.status;
    struct ReportInfo reportInfo =
        CreateReportInfo(&requestInfo, err, HRIL_NOTIFICATION, HNOTI_CALL_RINGBACK_VOICE_REPORT);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&ringbackVoiceFlag, sizeof(int32_t));
}

int32_t IsCallNoticeCmd(const char *str)
{
    int32_t tabSize = sizeof(g_callNotifyTab) / sizeof(CallNotify);
    for (int32_t i = 0; i < tabSize; i++) {
        if (ReportStrWith(str, g_callNotifyTab[i].cmd)) {
            return 1;
        }
    }
    return 0;
}

void CallReportInfoProcess(const char *str)
{
    int32_t tabSize = sizeof(g_callNotifyTab) / sizeof(CallNotify);
    for (int32_t i = 0; i < tabSize; i++) {
        if (ReportStrWith(str, g_callNotifyTab[i].cmd)) {
            g_callNotifyTab[i].function(str);
            break;
        }
    }
}

void ReportEmergencyNumberList(const char *str)
{
    int32_t err = HRIL_ERR_SUCCESS;
    char *pStr = (char *)str;
    HRilEmergencyInfo pEmergencyInfo = {0};
    ReqDataInfo requestInfo = {0};
    ModemReportErrorInfo errInfo = InitModemReportErrorInfo();

    if (SkipATPrefix(&pStr) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextInt(&pStr, &pEmergencyInfo.index) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextInt(&pStr, &pEmergencyInfo.total) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextStr(&pStr, &pEmergencyInfo.eccNum) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextInt(&pStr, &pEmergencyInfo.category) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextInt(&pStr, &pEmergencyInfo.simpresent) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextStr(&pStr, &pEmergencyInfo.mcc) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextInt(&pStr, &pEmergencyInfo.abnormalService) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }

    struct ReportInfo reportInfo =
        CreateReportInfo(&requestInfo, err, HRIL_NOTIFICATION, HNOTI_CALL_EMERGENCY_NUMBER_REPORT);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&pEmergencyInfo, sizeof(pEmergencyInfo));
}

void ReportCallUssdNotice(const char *str)
{
    int32_t err = HRIL_ERR_SUCCESS;
    char *pStr = (char *)str;
    HRilUssdNoticeInfo ussdNoticeInfo = {0};
    ReqDataInfo requestInfo = {0};
    ModemReportErrorInfo errInfo = InitModemReportErrorInfo();

    if (SkipATPrefix(&pStr) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextInt(&pStr, &ussdNoticeInfo.m) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    if (NextStr(&pStr, &ussdNoticeInfo.str) < 0) {
        err = HRIL_ERR_INVALID_MODEM_PARAMETER;
    }
    struct ReportInfo reportInfo = CreateReportInfo(&requestInfo, err, HRIL_NOTIFICATION, HNOTI_CALL_USSD_REPORT);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&ussdNoticeInfo, sizeof(HRilUssdNoticeInfo));
}

static int32_t InitCallListCmdBuffer(const ResponseInfo *pResponse, int32_t *callNum, HRilCallInfo **pCalls)
{
    int32_t ret;
    int32_t callNumTmp = 0;
    Line *pLine = NULL;
    HRilCallInfo *pCallsTmp = NULL;

    if (pResponse == NULL || pCalls == NULL || callNum == NULL) {
        TELEPHONY_LOGE("params: %{public}p,%{public}p,%{public}p", pResponse, callNum, pCalls);
        return HRIL_ERR_NULL_POINT;
    }

    *callNum = 0;
    *pCalls = NULL;

    for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next) {
        callNumTmp++;
    }
    if (!callNumTmp) {
        callNumTmp++; // Malloc size cannot be 0.
    }
    pCallsTmp = (HRilCallInfo *)malloc(callNumTmp * sizeof(HRilCallInfo));
    if (pCallsTmp == NULL) {
        TELEPHONY_LOGE("ReqGetCallList malloc pCalls failure");
        return HRIL_ERR_MEMORY_FULL;
    }
    ret = memset_s(pCallsTmp, callNumTmp * sizeof(HRilCallInfo), 0, callNumTmp * sizeof(HRilCallInfo));
    if (ret != EOK) {
        TELEPHONY_LOGE("memset_s is failed!");
        free(pCallsTmp);
        pCallsTmp = NULL;
        return ret;
    }

    *pCalls = pCallsTmp;
    *callNum = callNumTmp;

    return HRIL_ERR_SUCCESS;
}

int32_t BuildCallInfoList(const ReqDataInfo *requestInfo, ResponseInfo *response)
{
    int32_t ret = 0;
    int32_t callNum = 0;
    int32_t validCallNum = 0;
    int32_t err = HRIL_ERR_SUCCESS;
    Line *pLine = NULL;
    ResponseInfo *pResponse = response;
    HRilCallInfo *pCalls = NULL;

    if (pResponse == NULL || requestInfo == NULL) {
        TELEPHONY_LOGE("response or requestInfo is null.");
        return HRIL_ERR_NULL_POINT;
    }
    if (pResponse->success == 0) {
        TELEPHONY_LOGE("send cmd return ERROR");
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    ret = InitCallListCmdBuffer(pResponse, &callNum, &pCalls);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("init command failed: %{public}d", ret);
        return ret;
    }

    for (pLine = pResponse->head; pLine != NULL && validCallNum < callNum; pLine = pLine->next) {
        ret = CallCmdCLCC(pLine->data, pCalls + validCallNum);
        if (ret != 0) {
            TELEPHONY_LOGE("Parse CLCC data is fail. ret:%{public}d", ret);
            continue;
        }
        validCallNum++;
    }

    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)pCalls, sizeof(HRilCallInfo) * validCallNum);
    FreeResponseInfo(pResponse);
    free(pCalls);
    return HRIL_ERR_SUCCESS;
}

void ReqGetCallList(const ReqDataInfo *requestInfo)
{
    int32_t ret;
    ResponseInfo *pResponse = NULL;
    int32_t err = HRIL_ERR_SUCCESS;
    long timeOut = DEFAULT_TIMEOUT;

    ret = SendCommandLock("AT+CLCC", "+CLCC:", timeOut, &pResponse);
    if (ret || (pResponse != NULL && !pResponse->success)) {
        err = ret ? ret : err;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
        if (err < HRIL_ERR_SUCCESS) {
            err = HRIL_ERR_GENERIC_FAILURE;
        }
        OnCallReportErrorMessages(requestInfo, err, pResponse);
        return;
    }
    err = BuildCallInfoList(requestInfo, pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        if (err < HRIL_ERR_SUCCESS) {
            err = HRIL_ERR_GENERIC_FAILURE;
        }
        TELEPHONY_LOGE("Build Call Info List is failed.");
        OnCallReportErrorMessages(requestInfo, err, pResponse);
    }
}

void ReqDial(const ReqDataInfo *requestInfo, const HRilDial *data, size_t dataLen)
{
    HRilDial *pDial = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    const char *clir = NULL;
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    if (data == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        return;
    }

    pDial = (HRilDial *)data;
    switch (pDial->clir) {
        case CALL_CLIR_INVOCATION:
            clir = "I";
            break; /* invocation */
        case CALL_CLIR_SUPPRESSION:
            clir = "i";
            break; /* suppression */
        case CALL_CLIR_SUBSCRIPTION_DEFAULT:
        default:
            clir = "";
            break; /* subscription default */
    }

    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "ATD%s%s;", pDial->address, clir);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret != 0) {
        err = HRIL_ERR_CMD_SEND_FAILURE;
        TELEPHONY_LOGE("ATD send failed");
    } else {
        if (!pResponse->success) {
            TELEPHONY_LOGE("ReqDial return ERROR");
            err = HRIL_ERR_CMD_NO_CARRIER;
        }
    }

    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqHangup(const ReqDataInfo *requestInfo, const uint32_t *data, size_t dataLen)
{
    const int32_t *pLine = NULL;
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;

    if (data == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        return;
    }
    pLine = (const int32_t *)data;
    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CHLD=1%d", pLine[0]);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret != HRIL_ERR_SUCCESS || !pResponse->success) {
        TELEPHONY_LOGE("AT+CHLD send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqReject(const ReqDataInfo *requestInfo)
{
    ResponseInfo *pResponse = NULL;
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;

    ret = SendCommandLock("ATH", NULL, 0, &pResponse);
    if (ret != HRIL_ERR_SUCCESS || !pResponse->success) {
        TELEPHONY_LOGE("ATH send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqAnswer(const ReqDataInfo *requestInfo)
{
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("ATA", NULL, 0, &pResponse);
    if (ret != HRIL_ERR_SUCCESS || !pResponse->success) {
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

// Calling line identification presentation
void ReqGetClip(const ReqDataInfo *requestInfo)
{
    HRilGetClipResult result = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    err = SendCommandLock("AT+CLIP?", "+CLIP", 0, &pResponse);
    if (err == HRIL_ERR_SUCCESS) {
        if (!pResponse->success) {
            TELEPHONY_LOGE("CLIP return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        } else {
            if (pResponse->head != NULL) {
                char *line = pResponse->head->data;
                SkipATPrefix(&line);
                NextInt(&line, &result.action);
                NextInt(&line, &result.clipStat);
            } else {
                TELEPHONY_LOGE("ERROR: ReqGetClip pResponse->head is null");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&result, sizeof(result));
        FreeResponseInfo(pResponse);
    } else {
        TELEPHONY_LOGE("ReqGetClip send failed");
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_CMD_SEND_FAILURE, HRIL_RESPONSE, 0);
        OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&result, 0);
    }
}

void ReqSetClip(const ReqDataInfo *requestInfo, int32_t action)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CLIP=%d", action);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err == HRIL_ERR_SUCCESS) {
        if (!pResponse->success) {
            TELEPHONY_LOGE("ReqSetClip return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        }

        FreeResponseInfo(pResponse);
    } else {
        TELEPHONY_LOGE("ReqSetClip send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}

void ReqGetClir(const ReqDataInfo *requestInfo)
{
    HRilGetCallClirResult result = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    err = SendCommandLock("AT+CLIR?", "+CLIR", 0, &pResponse);
    if (err == HRIL_ERR_SUCCESS) {
        if (!pResponse->success) {
            TELEPHONY_LOGE("CLIR return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        } else {
            if (pResponse->head != NULL) {
                char *line = pResponse->head->data;
                SkipATPrefix(&line);
                NextInt(&line, &result.action);
                NextInt(&line, &result.clirStat);
            } else {
                TELEPHONY_LOGE("ERROR: ReqGetClir pResponse->head is null");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&result, sizeof(result));
        FreeResponseInfo(pResponse);
    } else {
        TELEPHONY_LOGE("ReqGetClir send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
        OnCallReportErrorMessages(requestInfo, err, pResponse);
    }
}

void ReqSetClir(const ReqDataInfo *requestInfo, int32_t action)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CLIR=%d", action);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err != HRIL_ERR_SUCCESS || !pResponse->success) {
        TELEPHONY_LOGE("ReqSetClir send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqStartDtmf(const ReqDataInfo *requestInfo, CallDtmfInfo info)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^DTMF=%d,%c,1,0", info.callId, info.dtmfKey[0]);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err != HRIL_ERR_SUCCESS || !pResponse->success) {
        TELEPHONY_LOGE("ReqStartDtmf send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSendDtmf(const ReqDataInfo *requestInfo, CallDtmfInfo info)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    int32_t stringLength = 0;
    int32_t ret;

    if (info.dtmfKey == NULL) {
        err = HRIL_ERR_NULL_POINT;
        OnCallReportErrorMessages(requestInfo, err, NULL);
        return;
    }

    for (stringLength = 0; stringLength < info.stringLength; stringLength++) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^DTMF=%d,%c,%d,%d", info.callId, info.dtmfKey[stringLength],
            info.onLength, info.offLength);
        if (ret < 0) {
            TELEPHONY_LOGE("GenerateCommand is failed!");
            continue;
        }
        err = SendCommandLock(cmd, NULL, 0, &pResponse);
        if (err != HRIL_ERR_SUCCESS || !pResponse->success) {
            TELEPHONY_LOGE("ReqSendDtmf send failed");
            err = HRIL_ERR_GENERIC_FAILURE;
            break;
        }
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqStopDtmf(const ReqDataInfo *requestInfo, CallDtmfInfo info)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^DTMF=%d,%c,0,0", info.callId, info.dtmfKey[0]);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err != HRIL_ERR_SUCCESS || !pResponse->success) {
        TELEPHONY_LOGE("ReqStopDtmf send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

static void HoldCallAndUnHoldCallAtSend(const ReqDataInfo *requestInfo)
{
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("AT+CHLD=2", NULL, 0, &pResponse);
    if (ret != HRIL_ERR_SUCCESS || !pResponse->success) {
        TELEPHONY_LOGE("ATA send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqHoldCall(const ReqDataInfo *requestInfo)
{
    HoldCallAndUnHoldCallAtSend(requestInfo);
}

void ReqUnHoldCall(const ReqDataInfo *requestInfo)
{
    HoldCallAndUnHoldCallAtSend(requestInfo);
}

void ReqSwitchCall(const ReqDataInfo *requestInfo)
{
    HoldCallAndUnHoldCallAtSend(requestInfo);
}

void ReqCombineConference(const ReqDataInfo *requestInfo, int32_t callType)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t ret = 0;
    int32_t count = 3;
    int32_t err = HRIL_ERR_SUCCESS;

    /* call type
     * 0: Voice call
     * 1: Video call: send one-way video, two-way voice
     * 2: Video call: one-way receiving video, two-way voice
     * 3: Video call: two-way video, two-way voice
     */
    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CHLD=3");
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    // "Adds a held call to the conversation"
    if (callType >= 0 && callType <= count) {
        long timeout = DEFAULT_TIMEOUT;
        ret = SendCommandLock(cmd, NULL, timeout, NULL);
        if (ret != HRIL_ERR_SUCCESS) {
            TELEPHONY_LOGE("ATA send failed");
            err = HRIL_ERR_CMD_SEND_FAILURE;
        }
    } else {
        TELEPHONY_LOGE("onRequest HREQ_CALL_COMBINE_CONFERENCE args error!!! \n");
        err = HRIL_ERR_INVALID_PARAMETER;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}

void ReqSeparateConference(const ReqDataInfo *requestInfo, int32_t callIndex, int32_t callType)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t ret = 0;
    int32_t count = 3;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    // Make sure that party is in a valid range.
    // (Note: The Telephony middle layer imposes a range of 1 to 7.
    // It's sufficient for us to just make sure it's single digit.)
    if ((callIndex > 0) && (callType >= 0 && callType <= count)) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CHLD=2%d", callIndex);
        if (ret < 0) {
            TELEPHONY_LOGE("GenerateCommand is failed!");
            OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
            return;
        }
        ret = SendCommandLock(cmd, NULL, 0, &pResponse);
        if (ret != HRIL_ERR_SUCCESS) {
            TELEPHONY_LOGE("ATA send failed");
            err = HRIL_ERR_CMD_SEND_FAILURE;
        } else {
            if (!pResponse->success) {
                TELEPHONY_LOGE("ATA send failed");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
    } else {
        TELEPHONY_LOGE("onRequest req split args error!!!");
        err = HRIL_ERR_INVALID_PARAMETER;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqCallSupplement(const ReqDataInfo *requestInfo, int32_t type)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t ret = 0;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    switch (type) {
        case TYPE_HANG_UP_HOLD_WAIT: {
            ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CHLD=0");
            if (ret < 0) {
                TELEPHONY_LOGE("GenerateCommand is failed!");
                OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
                return;
            }
            break;
        }
        case TYPE_HANG_UP_ACTIVE: {
            ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CHLD=1");
            if (ret < 0) {
                TELEPHONY_LOGE("GenerateCommand is failed!");
                OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
                return;
            }
            break;
        }
        default: {
            TELEPHONY_LOGW("ReqCallSupplement warring, type is invalid");
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
            OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
            FreeResponseInfo(pResponse);
            return;
        }
    }

    const long timeout = 3000;
    ret = SendCommandLock(cmd, NULL, timeout, &pResponse);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ReqCallSupplement cmd send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else {
        if (!pResponse->success) {
            TELEPHONY_LOGE("cmd send return error");
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    }

    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetCallWaiting(const ReqDataInfo *requestInfo)
{
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    HRilCallWaitResult hrilCallWaitResult = {0};
    char *line = NULL;
    const long timeout = 80000;
    err = SendCommandLock("AT+CCWA=1,2,1", "+CCWA:", timeout, &pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ReqGetCallWaiting return, CCWA send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(
            GetSlotId(requestInfo), reportInfo, (const uint8_t *)&hrilCallWaitResult, sizeof(HRilCallWaitResult));
        return;
    }
    if (!pResponse->success) {
        TELEPHONY_LOGE("Get CCWA return ERROR");
        err = HRIL_ERR_GENERIC_FAILURE;
    } else {
        if (pResponse->head != NULL) {
            line = pResponse->head->data;
            SkipATPrefix(&line);
            NextInt(&line, &hrilCallWaitResult.status);
            NextInt(&line, &hrilCallWaitResult.classCw);
        } else {
            TELEPHONY_LOGE("ERROR: ReqGetCallWaiting pResponse->head is null");
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&hrilCallWaitResult, sizeof(HRilCallWaitResult));
    FreeResponseInfo(pResponse);
}

void ReqSetCallWaiting(const ReqDataInfo *requestInfo, int32_t active)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    const long timeout = 500;

    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CCWA=1,%d,1", active);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    err = SendCommandLock(cmd, NULL, timeout, &pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ReqSetCallWaiting return, CCWA send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else {
        if (!pResponse->success) {
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSetCallTransferInfo(const ReqDataInfo *requestInfo, HRilCFInfo info)
{
    int32_t numType;
    const int32_t NUM_F = 145;
    const int32_t NUM_S = 129;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};

    if (info.reason > CALL_FORWARD_REASON_ALL_CCF || info.mode > CALL_FORWARD_MODE_ERASURE) {
        TELEPHONY_LOGE("ReqSetCallTransferInfo call forwarding parameter err!!");
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        return;
    }

    if (info.number != NULL && info.number[0] == '+') {
        numType = NUM_F;
    } else {
        numType = NUM_S;
    }

    int32_t ret = GenerateCommand(
        cmd, MAX_CMD_LENGTH, "AT+CCFC=%d,%d,\"%s\",%d,%d", info.reason, info.mode, info.number, numType, info.classx);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("CCFC send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else {
        if (!pResponse->success) {
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetCallTransferInfo(const ReqDataInfo *requestInfo, int32_t reason)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    HRilCFQueryInfo queryInfo = {0};
    char *line = NULL;

    if (reason > CALL_FORWARD_REASON_ALL_CCF) {
        TELEPHONY_LOGE("ReqGetCallTransferInfo call forwarding parameter err!!");
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        return;
    }
    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CCFC=%d,2", reason);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    err = SendCommandLock(cmd, "+CCFC", 0, &pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("CCFC send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else if (pResponse != NULL) {
        if (!pResponse->success) {
            TELEPHONY_LOGE("CCFC send and return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        } else {
            if (pResponse->head) {
                line = pResponse->head->data;
                SkipATPrefix(&line);
                NextInt(&line, &queryInfo.status);
                NextInt(&line, &queryInfo.classx);
                NextStr(&line, &queryInfo.number);
                NextInt(&line, &queryInfo.type);
            } else {
                TELEPHONY_LOGE("ERROR: ReqGetCallTransferInfo pResponse->head is null");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
    } else {
        TELEPHONY_LOGE("ERROR: ReqGetCallTransferInfo pResponse is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&queryInfo, sizeof(queryInfo));
    FreeResponseInfo(pResponse);
}

void ReqGetCallRestriction(const ReqDataInfo *requestInfo, const char *fac)
{
    long long timeOut = DEFAULT_TIMEOUT_CLCK;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    HRilCallRestrictionResult result = {0};
    char *line = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};

    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",2", fac);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    err = SendCommandLock(cmd, "+CLCK:", timeOut, &pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("CLCK send failed err = %{public}d", err);
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else if (pResponse != NULL) {
        if (!pResponse->success) {
            TELEPHONY_LOGE("ERROR: ReqGetCallRestriction return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        } else {
            if (pResponse->head) {
                line = pResponse->head->data;
                SkipATPrefix(&line);
                NextInt(&line, &result.status);
                NextInt(&line, &result.classCw);
            } else {
                TELEPHONY_LOGE("ERROR: ReqGetCallRestriction pResponse->head is null");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
    } else {
        TELEPHONY_LOGE("ERROR: ReqGetCallRestriction pResponse is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&result, sizeof(HRilCallRestrictionResult));
    FreeResponseInfo(pResponse);
}

void ReqSetCallRestriction(const ReqDataInfo *requestInfo, CallRestrictionInfo info)
{
    long long timeOut = DEFAULT_TIMEOUT;
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d,\"%s\"", info.fac, info.mode, info.password);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    err = SendCommandLock(cmd, NULL, timeOut, &pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("CLCK send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else {
        if (!pResponse->success) {
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSetBarringPassword(const ReqDataInfo *requestInfo, HRilSetBarringInfo info)
{
    long long timeOut = DEFAULT_TIMEOUT;
    char cmd[MAX_CMD_LENGTH] = { 0 };
    ResponseInfo *pResponse = NULL;

    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CPWD=\"%s\",\"%s\",\"%s\"", info.fac,
        info.oldPassword, info.newPassword);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    int32_t err = SendCommandLock(cmd, NULL, timeOut, &pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("CPWD send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else if (pResponse != NULL) {
        if (!pResponse->success) {
            TELEPHONY_LOGE("ERROR: ReqSetBarringPassword return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    } else {
        TELEPHONY_LOGE("ERROR: ReqSetBarringPassword pResponse is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetCallPreferenceMode(const ReqDataInfo *requestInfo)
{
    char *line = NULL;
    int32_t mode = VENDOR_FAIL;
    int32_t ret = VENDOR_FAIL;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    ModemReportErrorInfo errInfo = InitModemReportErrorInfo();
    ret = SendCommandLock("AT+CEVDP?", "+CEVDP:", 0, &pResponse);
    if (ret || pResponse == NULL || !pResponse->success) {
        err = (ret != HRIL_ERR_SUCCESS) ? HRIL_ERR_CMD_SEND_FAILURE : err;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
        OnCallReportErrorMessages(requestInfo, err, pResponse);
        return;
    }
    if (pResponse->head) {
        line = pResponse->head->data;
        err = SkipATPrefix(&line);
        if (err == 0) {
            err = NextInt(&line, &mode);
            TELEPHONY_LOGI("mode:%{public}d", mode);
        } else {
            TELEPHONY_LOGE("response error");
        }
    } else {
        TELEPHONY_LOGE("ERROR: pResponse->head is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&mode, sizeof(mode));
    FreeResponseInfo(pResponse);
}

void ReqSetCallPreferenceMode(const ReqDataInfo *requestInfo, int32_t mode)
{
    int32_t ret = VENDOR_FAIL;
    int32_t err = HRIL_ERR_SUCCESS;
    int32_t value = HRIL_CALL_MODE_CS_1ST_PS_2ND;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;

    ModemReportErrorInfo errInfo = InitModemReportErrorInfo();
    value = mode;
    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CEVDP=%d", value);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret || (pResponse != NULL && !pResponse->success)) {
        errInfo = GetReportErrorInfo(pResponse);
        err = errInfo.errorNo;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", ret ? ret : err);
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSetUssd(const ReqDataInfo *requestInfo, const char *str)
{
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {};

    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CUSD=1, \"% s\" , 15", str);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret || (pResponse != NULL && !pResponse->success)) {
        errInfo = GetReportErrorInfo(pResponse);
        err = errInfo.errorNo;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", ret ? ret : err);
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetUssd(const ReqDataInfo *requestInfo)
{
    int32_t ret;
    char *line = NULL;
    int32_t cusd = 0;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {};

    ret = SendCommandLock("AT+CUSD?", "+CUSD:", 0, &pResponse);
    if (ret || pResponse == NULL || !pResponse->success) {
        err = ret ? ret : err;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
        OnCallReportErrorMessages(requestInfo, err, pResponse);
        return;
    }
    if (pResponse->head) {
        line = pResponse->head->data;
        err = SkipATPrefix(&line);
        if (err == 0) {
            err = NextInt(&line, &cusd);
            TELEPHONY_LOGI("+CUSD:%{public}d", cusd);
        } else {
            TELEPHONY_LOGE("response error");
        }
    } else {
        TELEPHONY_LOGE("ERROR: pResponse->head is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&cusd, sizeof(cusd));
    FreeResponseInfo(pResponse);
}

void ReqSetMute(const ReqDataInfo *requestInfo, int32_t mute)
{
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {};

    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMUT=%d ", mute);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret || (pResponse != NULL && !pResponse->success)) {
        errInfo = GetReportErrorInfo(pResponse);
        err = errInfo.errorNo;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", ret ? ret : err);
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetMute(const ReqDataInfo *requestInfo)
{
    int32_t ret;
    char *line = NULL;
    int32_t mute = 0;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {};

    ret = SendCommandLock("AT+CMUT?", "+CMUT:", 0, &pResponse);
    if (ret || pResponse == NULL || !pResponse->success) {
        err = ret ? ret : err;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
        OnCallReportErrorMessages(requestInfo, err, pResponse);
        return;
    }
    if (pResponse->head) {
        line = pResponse->head->data;
        err = SkipATPrefix(&line);
        if (err == 0) {
            err = NextInt(&line, &mute);
            TELEPHONY_LOGI("+CMUT:%{public}d", mute);
        } else {
            TELEPHONY_LOGE("response error");
        }
    } else {
        TELEPHONY_LOGE("ERROR: pResponse->head is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&mute, sizeof(mute));
    FreeResponseInfo(pResponse);
}

static int32_t CallCmdXLEMA(const char *lineCmd, HRilEmergencyInfo *outCall)
{
    char *pLine = (char *)lineCmd;

    if (pLine == NULL || outCall == NULL) {
        TELEPHONY_LOGE("src or desc pointer is null.");
        return HRIL_ERR_NULL_POINT;
    }

    if (SkipATPrefix(&pLine) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->index) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->total) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextStr(&pLine, &outCall->eccNum) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->category) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->simpresent) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextStr(&pLine, &outCall->mcc) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->abnormalService) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    return HRIL_ERR_SUCCESS;
}

static int32_t InitGetEmergencyCallList(const ResponseInfo *pResponse, int32_t *callNum,
    HRilEmergencyInfo **pEmergencyCalls)
{
    int32_t ret;
    int32_t callNumTmp = 0;
    Line *pLine = NULL;
    HRilEmergencyInfo *pEmergencyCallsTmp = NULL;

    if (pResponse == NULL || pEmergencyCalls == NULL || callNum == NULL) {
        TELEPHONY_LOGE("params: %{public}p,%{public}p,%{public}p", pResponse, callNum, pEmergencyCalls);
        return HRIL_ERR_NULL_POINT;
    }

    *callNum = 0;
    *pEmergencyCalls = NULL;

    for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next) {
        callNumTmp++;
    }
    if (!callNumTmp) {
        callNumTmp++; // Malloc size cannot be 0.
    }
    pEmergencyCallsTmp = (HRilEmergencyInfo *)malloc(callNumTmp * sizeof(HRilEmergencyInfo));
    if (pEmergencyCallsTmp == NULL) {
        TELEPHONY_LOGE("ReqGetCallList malloc pCalls failure");
        return HRIL_ERR_MEMORY_FULL;
    }
    ret = memset_s(
        pEmergencyCallsTmp, callNumTmp * sizeof(HRilEmergencyInfo), 0, callNumTmp * sizeof(HRilEmergencyInfo));
    if (ret != EOK) {
        TELEPHONY_LOGE("memset_s is failed!");
        free(pEmergencyCallsTmp);
        pEmergencyCallsTmp = NULL;
        return ret;
    }

    *pEmergencyCalls = pEmergencyCallsTmp;
    *callNum = callNumTmp;

    return HRIL_ERR_SUCCESS;
}

int32_t BuildGetEmergencyCallList(const ReqDataInfo *requestInfo, ResponseInfo *response)
{
    int32_t ret = 0;
    int32_t callNum = 0;
    int32_t validCallNum = 0;
    int32_t err = HRIL_ERR_SUCCESS;
    Line *pLine = NULL;
    ResponseInfo *pResponse = response;
    HRilEmergencyInfo *pEmergencyCalls = NULL;

    if (pResponse == NULL || requestInfo == NULL) {
        TELEPHONY_LOGE("response or requestInfo is null.");
        return HRIL_ERR_NULL_POINT;
    }
    if (pResponse->success == 0) {
        TELEPHONY_LOGE("send cmd return ERROR");
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    ret = InitGetEmergencyCallList(pResponse, &callNum, &pEmergencyCalls);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("init command failed: %{public}d", ret);
        return ret;
    }

    for (pLine = pResponse->head; pLine != NULL && validCallNum < callNum; pLine = pLine->next) {
        ret = CallCmdXLEMA(pLine->data, pEmergencyCalls + validCallNum);
        if (ret != 0) {
            TELEPHONY_LOGE("Parse CLCC data is fail. ret:%{public}d", ret);
            continue;
        }
        validCallNum++;
    }

    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(
        GetSlotId(requestInfo), reportInfo, (const uint8_t *)pEmergencyCalls, sizeof(HRilEmergencyInfo) * validCallNum);
    FreeResponseInfo(pResponse);
    free(pEmergencyCalls);
    return HRIL_ERR_SUCCESS;
}

void ReqGetEmergencyCallList(const ReqDataInfo *requestInfo)
{
    int32_t ret;
    ResponseInfo *pResponse = NULL;
    int32_t err = HRIL_ERR_SUCCESS;
    long timeOut = DEFAULT_TIMEOUT;

    ret = SendCommandLock("AT^XLEMA?", "^XLEMA:", timeOut, &pResponse);
    if (ret || (pResponse != NULL && !pResponse->success)) {
        err = (ret != HRIL_ERR_SUCCESS) ? HRIL_ERR_CMD_SEND_FAILURE : err;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
        OnCallReportErrorMessages(requestInfo, err, pResponse);
        return;
    }
    err = BuildGetEmergencyCallList(requestInfo, pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("Build Call Info List is failed.");
        OnCallReportErrorMessages(requestInfo, err, pResponse);
    }
}

void ReqSetEmergencyCallList(const ReqDataInfo *requestInfo, HRilEmergencyInfo *emergencyInfo, const int len)
{
    TELEPHONY_LOGI("ReqSetEmergencyCallList start");
    int32_t ret = 0;
    int32_t err = HRIL_ERR_SUCCESS;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {};
    for (int i = 0; i  < len; i++) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^NVM=%d,%d,\"%s\",%d,%d,%s,%d", emergencyInfo[i].index,
            emergencyInfo[i].total, emergencyInfo[i].eccNum, emergencyInfo[i].category, emergencyInfo[i].simpresent,
            emergencyInfo[i].mcc, emergencyInfo[i].abnormalService);
        if (ret < 0) {
            TELEPHONY_LOGE("GenerateCommand is failed!");
            OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
            return;
        }
        ret = SendCommandLock(cmd, NULL, 0, &pResponse);
        if (ret || (pResponse != NULL && !pResponse->success)) {
            errInfo = GetReportErrorInfo(pResponse);
            err = errInfo.errorNo;
            TELEPHONY_LOGE("cmd send failed, err:%{public}d", ret ? ret : err);
        }
    }
    TELEPHONY_LOGI("ReqSetEmergencyCallList end");
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetCallFailReason(const ReqDataInfo *requestInfo)
{
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {};

    if (lastCcCause == HRIL_ERR_CALL_CAUSE) {
        TELEPHONY_LOGE("lastCcCause is none!");
        OnCallReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }

    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnCallReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lastCcCause, sizeof(lastCcCause));
    FreeResponseInfo(pResponse);
}
