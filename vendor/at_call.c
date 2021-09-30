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

#include "at_call.h"
#include "hril.h"
#include "vendor_util.h"
#include "vendor_report.h"
#include "at_support.h"

#undef DEFAULT_TIMEOUT
#define DEFAULT_TIMEOUT 5000
#define MAX_BUFF_SIZE 500

static int CallCmdCLCC(const char *lineCmd, HRilCallInfo *outCall)
{
    char *pLine = (char *)lineCmd;
    int state;
    int mode;
    if (pLine == NULL) {
        return HRIL_ERR_NULL_POINT;
    }
    if (SkipATPrefix(&pLine) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (outCall == NULL) {
        TELEPHONY_LOGE("outCall is null!!!");
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
    if (NextStr(&pLine, &outCall->number) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->type) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (pLine != NULL) {
        if (NextStr(&pLine, &outCall->alpha) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
    }
    return HRIL_ERR_SUCCESS;
};

void ReqGetCallList(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int ret = -1;
    struct ReportInfo reportInfo = {};
    Line *pLine = NULL;
    ResponseInfo *pResponse = NULL;
    HRilCallInfo *pCalls = NULL;
    HRilCallInfo **ppCalls = NULL;
    int callNum = 0;
    int validCallNum = 0;
    int i = 0;
    int err = HRIL_ERR_SUCCESS;

    ret = SendCommandLock("AT+CLCC", "+CLCC:", DEFAULT_TIMEOUT, &pResponse);
    if (ret != VENDOR_SUCCESS) {
        TELEPHONY_LOGE("AT+CLCC send failed, data=%{public}p", data);
        err = HRIL_ERR_CMD_SEND_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, NULL, 0);
        return;
    }

    if (pResponse->success == 0) {
        TELEPHONY_LOGE("AT+CLCC send return ERROR");
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next) {
        callNum++;
    }
    ppCalls = (HRilCallInfo **)malloc(callNum * sizeof(HRilCallInfo *));
    pCalls = (HRilCallInfo *)malloc(callNum * sizeof(HRilCallInfo));

    for (i = 0; i < callNum; i++) {
        ppCalls[i] = &(pCalls[i]);
    }

    for (pLine = pResponse->head; pLine != NULL && validCallNum < callNum; pLine = pLine->next) {
        ret = CallCmdCLCC(pLine->data, pCalls + validCallNum);
        if (ret != 0) {
            TELEPHONY_LOGE("AT+CLCC send return ERROR:%{public}d", ret);
            continue;
        }
        validCallNum++;
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, ppCalls, sizeof(HRilCallInfo *) * validCallNum);
    FreeResponseInfo(pResponse);
    free(ppCalls);
    free(pCalls);
}

void ReqDial(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    HRilDial *pDial = NULL;
    char cmd[MAX_BUFF_SIZE] = {0};
    const char *clir = NULL;
    int ret;
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo = {};
    ResponseInfo *pResponse = NULL;

    if (data == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        err = HRIL_ERR_INVALID_PARAMETER;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, NULL, 0);
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
        case CALL_CLIR_SUBSCRIPTION_DEFUALT:
        default:
            clir = "";
            break; /* subscription default */
    }

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "ATD%s%s;", pDial->address, clir);
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret != 0) {
        err = HRIL_ERR_CMD_SEND_FAILURE;
        TELEPHONY_LOGE("ATD send failed");
    } else {
        if (pResponse != NULL && pResponse->success == 0) {
            TELEPHONY_LOGE("ReqDial return ERROR");
            err = HRIL_ERR_CMD_NO_CARRIER;
        }
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqHangup(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    const int *pLine = NULL;
    int ret;
    int err = HRIL_ERR_SUCCESS;
    char cmd[MAX_BUFF_SIZE] = {0};
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo = {};

    if (data == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        err = HRIL_ERR_INVALID_PARAMETER;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, NULL, 0);
        return;
    }
    pLine = (const int *)data;
    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CHLD=1%d", pLine[0]);
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && pResponse->success == 0)) {
        TELEPHONY_LOGE("AT+CHLD send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqReject(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    struct ReportInfo reportInfo = {};
    ResponseInfo *pResponse = NULL;
    int ret;
    int err = HRIL_ERR_SUCCESS;

    ret = SendCommandLock("ATH", NULL, 0, &pResponse);
    if (ret != 0 || pResponse->success == 0) {
        TELEPHONY_LOGE("ATH send failed: %{public}p", data);
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqAnswer(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int ret;
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo = {};
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("ATA", NULL, 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && pResponse->success == 0)) {
        TELEPHONY_LOGE("ATA send failed, data=%{public}p", data);
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

// Calling line identification presentation
void ReqGetClip(ReqDataInfo *requestInfo)
{
    struct ReportInfo reportInfo = {};
    HRilGetClipResult result = {};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    err = SendCommandLock("AT+CLIP?", "+CLIP", 0, &pResponse);
    if (err == 0 && pResponse != NULL) {
        if (pResponse->success == 0) {
            TELEPHONY_LOGE("ReqGetClip return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        } else {
            if (pResponse->head) {
                char *line = pResponse->head->data;
                SkipATPrefix(&line);
                NextInt(&line, &result.action);
                NextInt(&line, &result.clipStat);
                TELEPHONY_LOGD(
                    "ReqGetClip pResponse action: %{public}d, line: %{public}d", result.action, result.clipStat);
            } else {
                TELEPHONY_LOGE("ERROR: ReqGetClip pResponse->head is null");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, &result, sizeof(result));
        FreeResponseInfo(pResponse);
    } else {
        TELEPHONY_LOGE("ReqGetClip send failed");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_CMD_SEND_FAILURE, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, &result, 0);
    }
}

void ReqSetClip(ReqDataInfo *requestInfo, int action)
{
    struct ReportInfo reportInfo = {};
    char cmd[MAX_BUFF_SIZE] = {0};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CLIP=%d", action);
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err == 0 && pResponse != NULL) {
        if (pResponse->success == 0) {
            TELEPHONY_LOGE("ReqSetClip return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        }
        FreeResponseInfo(pResponse);
    } else {
        TELEPHONY_LOGE("ReqSetClip send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
}

void ReqGetClir(ReqDataInfo *requestInfo)
{
    struct ReportInfo reportInfo = {};
    HRilGetCallClirResult result = {};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    err = SendCommandLock("AT+CLIR?", "+CLIR", 0, &pResponse);
    if (err == HRIL_ERR_SUCCESS && pResponse != NULL) {
        if (pResponse->success == 0) {
            TELEPHONY_LOGE("CLIR return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        } else {
            if (pResponse->head) {
                char *line = pResponse->head->data;
                SkipATPrefix(&line);
                NextInt(&line, &result.action);
                NextInt(&line, &result.clirStat);
                TELEPHONY_LOGD(
                    "ReqGetClir pResponse action: %{public}d line: %{public}d", result.action, result.clirStat);
            } else {
                TELEPHONY_LOGE("ERROR: ReqGetClir pResponse->head is null");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, &result, sizeof(result));
        FreeResponseInfo(pResponse);
    } else {
        TELEPHONY_LOGE("ReqGetClir send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, &result, 0);
    }
}

void ReqSetClir(ReqDataInfo *requestInfo, int action)
{
    struct ReportInfo reportInfo = {};
    char cmd[MAX_BUFF_SIZE] = {0};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CLIR=%d", action);
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err != HRIL_ERR_SUCCESS || (pResponse != NULL && pResponse->success == 0)) {
        TELEPHONY_LOGE("ReqSetClir send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    free(cmd);
    FreeResponseInfo(pResponse);
}

void ReqStartDtmf(ReqDataInfo *requestInfo, CallDTMFInfo info)
{
    struct ReportInfo reportInfo = {};
    char cmd[MAX_BUFF_SIZE] = {0};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT^DTMF=%d,%c,1,0", info.callId, info.dtmfKey[0]);
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err != HRIL_ERR_SUCCESS || (pResponse != NULL && pResponse->success == 0)) {
        TELEPHONY_LOGE("ReqStartDtmf send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSendDtmf(ReqDataInfo *requestInfo, CallDTMFInfo info)
{
    struct ReportInfo reportInfo = {};
    char cmd[MAX_BUFF_SIZE] = {0};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    int stringLength = 0;

    if (info.dtmfKey == NULL) {
        err = HRIL_ERR_NULL_POINT;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, NULL, 0);
        return;
    }

    for (stringLength = 0; stringLength < info.stringLength; stringLength++) {
        (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT^DTMF=%d,%c,%d,%d", info.callId, info.dtmfKey[stringLength],
            info.onLength, info.offLength);
        err = SendCommandLock(cmd, NULL, 0, &pResponse);
        if (err != HRIL_ERR_SUCCESS || (pResponse != NULL && pResponse->success == 0)) {
            TELEPHONY_LOGE("ReqSendDtmf send failed");
            err = HRIL_ERR_GENERIC_FAILURE;
            break;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqStopDtmf(ReqDataInfo *requestInfo, CallDTMFInfo info)
{
    struct ReportInfo reportInfo = {};
    char cmd[MAX_BUFF_SIZE] = {0};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT^DTMF=%d,%c,0,0", info.callId, info.dtmfKey[0]);
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err != HRIL_ERR_SUCCESS || (pResponse != NULL && pResponse->success == 0)) {
        TELEPHONY_LOGE("ReqStopDtmf send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

static void HoldAndActiveAtSend(ReqDataInfo *requestInfo)
{
    struct ReportInfo reportInfo = {};
    int ret;
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("AT+CHLD=2", NULL, 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && pResponse->success == 0)) {
        TELEPHONY_LOGE("ATA send failed");
        err = HRIL_ERR_GENERIC_FAILURE;
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqHold(ReqDataInfo *requestInfo)
{
    HoldAndActiveAtSend(requestInfo);
}

void ReqActive(ReqDataInfo *requestInfo)
{
    HoldAndActiveAtSend(requestInfo);
}

void ReqSwap(ReqDataInfo *requestInfo)
{
    HoldAndActiveAtSend(requestInfo);
}

void ReqJoin(ReqDataInfo *requestInfo, int callType)
{
    char cmd[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    int ret = 0;
    int count = 3;
    int err = HRIL_ERR_SUCCESS;

    /* call type
     * 0: Voice call
     * 1: Video call: send one-way video, two-way voice
     * 2: Video call: one-way receiving video, two-way voice
     * 3: Video call: two-way video, two-way voice
     */
    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CHLD=3");
    // "Adds a held call to the conversation"
    if (callType >= 0 && callType <= count) {
        long timeout = 5000;
        ret = SendCommandLock(cmd, NULL, timeout, NULL);
        if (ret != 0) {
            TELEPHONY_LOGE("ATA send failed");
            err = HRIL_ERR_CMD_SEND_FAILURE;
        }
    } else {
        TELEPHONY_LOGE("onRequest HREQ_CALL_JOIN args error!!! \n");
        err = HRIL_ERR_INVALID_PARAMETER;
    }
    TELEPHONY_LOGD("ReqJoin FreeResponseInfo(pResponse)");
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
}

void ReqSplit(ReqDataInfo *requestInfo, int nThCall, int callType)
{
    char cmd[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    int ret = 0;
    int count = 3;
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    // Make sure that party is in a valid range.
    // (Note: The Telephony middle layer imposes a range of 1 to 7.
    // It's sufficient for us to just make sure it's single digit.)
    if ((nThCall > 0) && (callType >= 0 && callType <= count)) {
        (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CHLD=2%d", nThCall);
        ret = SendCommandLock(cmd, NULL, 0, &pResponse);
        if (ret != 0) {
            TELEPHONY_LOGE("ATA send failed");
            err = HRIL_ERR_CMD_SEND_FAILURE;
        } else {
            if (pResponse != NULL && pResponse->success == 0) {
                TELEPHONY_LOGE("ATA send failed");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
    } else {
        TELEPHONY_LOGE("onRequest req split args error!!! \n");
        err = HRIL_ERR_INVALID_PARAMETER;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqCallSupplement(ReqDataInfo *requestInfo, int type)
{
    char cmd[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    int ret = 0;
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    switch (type) {
        case TYPE_HANG_UP_HOLD_WAIT: {
            TELEPHONY_LOGD("ReqCallSupplement hang up all holding or waiting call");
            (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CHLD=0");
            break;
        }
        case TYPE_HANG_UP_ACTIVE: {
            TELEPHONY_LOGD("ReqCallSupplement hang up all active call, and active holding or waiting call");
            (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CHLD=1");
            break;
        }
        default: {
            TELEPHONY_LOGW("ReqCallSupplement warring, type is invalid");
            reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
            OnCallReport(reportInfo, NULL, 0);
            FreeResponseInfo(pResponse);
            return;
        }
    }

    long timeout = 3000;
    ret = SendCommandLock(cmd, NULL, timeout, &pResponse);
    if (ret != 0) {
        TELEPHONY_LOGE("ReqCallSupplement cmd send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else {
        if (pResponse != NULL && pResponse->success == 0) {
            TELEPHONY_LOGE("cmd send return error");
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
    free(cmd);
}

void ReqGetCallWait(ReqDataInfo *requestInfo)
{
    struct ReportInfo reportInfo = {};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    HRilCallWaitResult hrilCallWaitResult = {};
    char *line = NULL;
    const long timeout = 80000;

    err = SendCommandLock("AT+CCWA=1,2,1", "+CCWA:", timeout, &pResponse);
    if (err != 0) {
        TELEPHONY_LOGE("ReqGetCallWait return, CCWA send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, &hrilCallWaitResult, sizeof(HRilCallWaitResult));
        return;
    }
    if (pResponse != NULL) {
        if (pResponse->success == 0) {
            TELEPHONY_LOGE("Get CCWA return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        } else {
            if (pResponse->head) {
                line = pResponse->head->data;
                err = SkipATPrefix(&line);
                err = NextInt(&line, &hrilCallWaitResult.status);
                err = NextInt(&line, &hrilCallWaitResult.classCw);
                TELEPHONY_LOGD("ReqGetCallWait pResponse status: %{public}d class: %{public}d",
                    hrilCallWaitResult.status, hrilCallWaitResult.classCw);
            } else {
                TELEPHONY_LOGE("ERROR: ReqGetCallWait pResponse->head is null");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
    } else {
        TELEPHONY_LOGE("ERROR: ReqGetCallWait pResponse is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, &hrilCallWaitResult, sizeof(HRilCallWaitResult));
    FreeResponseInfo(pResponse);
}

void ReqSetCallWait(ReqDataInfo *requestInfo, int active)
{
    struct ReportInfo reportInfo = {};
    char cmd[MAX_BUFF_SIZE] = {0};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    const long timeout = 80000;

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CCWA=1,%d,1", active);
    err = SendCommandLock(cmd, NULL, timeout, &pResponse);
    if (err != 0) {
        TELEPHONY_LOGE("ReqSetCallWait return, CCWA send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else {
        if (pResponse != NULL && pResponse->success == 0) {
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSetCallForwarding(ReqDataInfo *requestInfo, HRilCFInfo info)
{
    int numType;
    const int NUM_F = 145;
    const int NUM_S = 129;
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo = {};
    char cmd[MAX_BUFF_SIZE] = {0};

    if (info.reason > CALL_FORWARD_REASON_ALL_CCF && info.mode > CALL_FORWARD_MODE_ERASURE) {
        TELEPHONY_LOGE("ReqSetCallForwarding call forwarding parameter err!!");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, NULL, 0);
        return;
    }

    if (info.number != NULL && info.number[0] == '+') {
        numType = NUM_F;
    } else {
        numType = NUM_S;
    }

    TELEPHONY_LOGD("ReqSetCallForwarding cmd");
    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CCFC=%d,%d,\"%s\",%d,%d", info.reason, info.mode, info.number, numType,
        info.classx);
    err = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (err != 0) {
        TELEPHONY_LOGE("CCFC send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else {
        if (pResponse != NULL && pResponse->success == 0) {
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetCallForwarding(ReqDataInfo *requestInfo, int reason)
{
    struct ReportInfo reportInfo = {};
    char cmd[MAX_BUFF_SIZE] = {0};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    HRilCFQueryInfo queryInfo = {0};
    char *line = NULL;

    if (reason > CALL_FORWARD_REASON_ALL_CCF) {
        TELEPHONY_LOGE("call forwarding parameter err!!");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnCallReport(reportInfo, NULL, 0);
        return;
    }
    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CCFC=%d,2", reason);
    err = SendCommandLock(cmd, "+CCFC", 0, &pResponse);
    if (err != 0) {
        TELEPHONY_LOGE("CCFC send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else if (pResponse != NULL) {
        if (pResponse->success == 0) {
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
                TELEPHONY_LOGD(
                    "pResponse status: %{public}d, class: %{public}d, type: %{public}d ",
                    queryInfo.status, queryInfo.classx, queryInfo.type);
            } else {
                TELEPHONY_LOGE("ERROR: pResponse->head is null");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
    } else {
        TELEPHONY_LOGE("ERROR: pResponse is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, &queryInfo, sizeof(queryInfo));
    FreeResponseInfo(pResponse);
}

void ReqGetCallRestriction(ReqDataInfo *requestInfo, const char *fac)
{
    long long timeOut = 5000;
    struct ReportInfo reportInfo = {};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;
    HRilCallRestrictionResult result = {};
    char *line = NULL;
    char cmd[MAX_BUFF_SIZE] = {0};

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CLCK=\"%s\",2", fac);
    err = SendCommandLock(cmd, "+CLCK:", timeOut, &pResponse);
    if (err != 0) {
        TELEPHONY_LOGE("CLCK send failed err = %{public}d", err);
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else if (pResponse != NULL) {
        if (pResponse->success == 0) {
            TELEPHONY_LOGE("ERROR: ReqGetCallRestriction return ERROR");
            err = HRIL_ERR_GENERIC_FAILURE;
        } else {
            if (pResponse->head) {
                line = pResponse->head->data;
                err = SkipATPrefix(&line);
                err = NextInt(&line, &result.status);
                err = NextInt(&line, &result.classCw);
                TELEPHONY_LOGD("ReqGetCallRestriction pResponse status: %{public}d class: %{public}d",
                    result.status, result.classCw);
            } else {
                TELEPHONY_LOGE("ERROR: ReqGetCallRestriction pResponse->head is null");
                err = HRIL_ERR_GENERIC_FAILURE;
            }
        }
    } else {
        TELEPHONY_LOGE("ERROR: ReqGetCallRestriction pResponse is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, &result, sizeof(HRilCallRestrictionResult));
    FreeResponseInfo(pResponse);
}

void ReqSetCallRestriction(ReqDataInfo *requestInfo, CallRestrictionInfo info)
{
    long long timeOut = 5000;
    struct ReportInfo reportInfo = {};
    char cmd[MAX_BUFF_SIZE] = {0};
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *pResponse = NULL;

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CLCK=\"%s\",%d,\"%s\"", info.fac, info.mode, info.password);
    err = SendCommandLock(cmd, NULL, timeOut, &pResponse);
    if (err != 0) {
        TELEPHONY_LOGE("CLCK send failed");
        err = HRIL_ERR_CMD_SEND_FAILURE;
    } else {
        if (pResponse != NULL && pResponse->success == 0) {
            err = HRIL_ERR_GENERIC_FAILURE;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnCallReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}