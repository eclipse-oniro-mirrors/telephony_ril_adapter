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

#include "at_data.h"

#include "vendor_report.h"

#define MAX_BUFF_SIZE 500

int ParsePdpCmd(char *str, HRilDataCallResponse *outData)
{
    char *pStr = NULL;
    char *out = NULL;

    if (str == NULL) {
        TELEPHONY_LOGE("param error: str=[%{public}p]", str);
        return HRIL_ERR_NULL_POINT;
    }
    pStr = str;
    if (ReportStrWith(str, "+CGACT:")) {
        if (SkipATPrefix(&pStr) < 0) {
            goto ERR;
        }
        if (NextInt(&pStr, &outData->cid) < 0) {
            goto ERR;
        }
        if (NextInt(&pStr, &outData->active) < 0) {
            goto ERR;
        }
    } else if (ReportStrWith(str, "+CGDCONT:")) {
        if (SkipATPrefix(&pStr) < 0) {
            goto ERR;
        }
        if (NextInt(&pStr, &outData->cid) < 0) {
            goto ERR;
        }
        if (NextStr(&pStr, &outData->type) < 0) {
            goto ERR;
        }
        if (NextStr(&pStr, &out) < 0) { // APN ignored
            goto ERR;
        }
        if (NextStr(&pStr, &outData->address) < 0) {
            goto ERR;
        }
    } else {
        goto ERR;
    }
    return HRIL_ERR_SUCCESS;

ERR:
    TELEPHONY_LOGE("invalid CGACT Or CGDCONT");
    return HRIL_ERR_NULL_POINT;
}

int ParseReportError(char *str)
{
    int ret = INT_DEFAULT_VALUE;
    char *pStr = str;

    if (pStr == NULL) {
        TELEPHONY_LOGE("invalid param");
        return ret;
    }
    TELEPHONY_LOGD("======>enter cmd: %{public}s", pStr);
    if (ReportStrWith(pStr, "+CME ERROR:")) {
        if (!SkipATPrefix(&pStr)) {
            NextInt(&pStr, &ret);
        }
    }
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("report failed: [%{public}p],ret=%{public}d", pStr, ret);
    }
    return ret;
}

ModemReportErrorInfo GetReportErrorInfo(ResponseInfo *response)
{
    int ret = INT_DEFAULT_VALUE;
    ModemReportErrorInfo errInfo;

    errInfo.errType = HRIL_REPORT_ERR_TYPE_GENERIC;
    errInfo.errorNo = HRIL_ERR_GENERIC_FAILURE;
    if ((response != NULL) && !response->success) {
        ret = ParseReportError(response->result);
        if (ret >= 0) {
            errInfo.errType = HRIL_REPORT_ERR_TYPE_CME;
            errInfo.errorNo = ret;
        }
    }
    return errInfo;
}

HRilDataCallResponse *CreatDataCallResponseAndInit(int count)
{
    int size;
    HRilDataCallResponse *newDcr = NULL;

    size = count * sizeof(HRilDataCallResponse);
    newDcr = (HRilDataCallResponse *)malloc(size);
    if (newDcr != NULL) {
        (void)memset_s(newDcr, size, 0, size);
    } else {
        TELEPHONY_LOGE("malloc failed");
    }
    return newDcr;
}

ModemReportErrorInfo SendInquireCGACT(int *outDataNum, HRilDataCallResponse **ppDcr)
{
    int ret;
    int dataCallNum = 0;
    Line *pLine = NULL;
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {
        .errorNo = HRIL_ERR_SUCCESS,
        .errType = HRIL_REPORT_ERR_TYPE_NONE
    };

    ret = SendCommandLock("AT+CGACT?", "+CGACT:", 0, &pResponse);
    if (ret != 0 || !pResponse->success) {
        errInfo = GetReportErrorInfo(pResponse);
        TELEPHONY_LOGE("send AT CMD failed!, ret:%{public}d", errInfo.errorNo);
        FreeResponseInfo(pResponse);
        return errInfo;
    }

    for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next) {
        dataCallNum++;
    }

    TELEPHONY_LOGD("dataCallNum:%{public}d", dataCallNum);
    *ppDcr = CreatDataCallResponseAndInit(dataCallNum);
    HRilDataCallResponse *pDataCall = *ppDcr;
    for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next) {
        ret = ParsePdpCmd(pLine->data, pDataCall);
        if (ret != 0) {
            continue;
        }
        pDataCall++;
    }
    *outDataNum = dataCallNum;
    FreeResponseInfo(pResponse);
    return errInfo;
}

static void BuildDataInfoList(int *validCount, int dataNum, ResponseInfo *response, HRilDataCallResponse **ppDcr)
{
    int ret;
    int i = 0;
    int count = 0;
    int dataCallNum = 0;
    Line *pLine = NULL;
    ResponseInfo *pResponse = response;
    HRilDataCallResponse *pDataCall = *ppDcr;
    HRilDataCallResponse dataCGDCONT = {};

    dataCallNum = dataNum;
    for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next) {
        ret = ParsePdpCmd(pLine->data, &dataCGDCONT);
        if (ret != 0) {
            TELEPHONY_LOGE("parser pdp command failed: [%{public}p],ret=%{public}d", pLine->data, ret);
            continue;
        }
        for (i = 0; i < dataCallNum; i++) {
            if ((*ppDcr)[i].cid == dataCGDCONT.cid) {
                break;
            }
        }
        if (i >= dataCallNum) {
            continue;
        }
        pDataCall->state = 0;
        pDataCall->type = strdup(dataCGDCONT.type);
        pDataCall->address = (strlen(dataCGDCONT.address) != 0) ? strdup(dataCGDCONT.address) : "";
        pDataCall->netPortName = strdup(PPP_TTY_PATH);
        pDataCall->dns = "";
        pDataCall++;
        count++;
    }
    *validCount = count;
}

ModemReportErrorInfo SendInquireCGDCONT(int *validCount, int dataNum, HRilDataCallResponse **ppDcr)
{
    int ret;
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {
        .errorNo = HRIL_ERR_SUCCESS,
        .errType = HRIL_REPORT_ERR_TYPE_NONE
    };
    ret = SendCommandLock("AT+CGDCONT?", "+CGDCONT:", 0, &pResponse);
    if (ret != 0 || !pResponse->success) {
        errInfo = GetReportErrorInfo(pResponse);
        TELEPHONY_LOGE("send AT CMD failed! ret:%{public}d", errInfo.errorNo);
        FreeResponseInfo(pResponse);
        return errInfo;
    }

    if (ppDcr == NULL || *ppDcr == NULL) {
        pResponse->success = 0;
        errInfo = GetReportErrorInfo(pResponse);
        FreeResponseInfo(pResponse);
        TELEPHONY_LOGE("ppDcr is NULL! ret:%{public}d", errInfo.errorNo);
        return errInfo;
    }
    BuildDataInfoList(validCount, dataNum, pResponse, ppDcr);
    FreeResponseInfo(pResponse);
    return errInfo;
}

void FreeDataCallResponse(HRilDataCallResponse *pDcrs, int size)
{
    int i = 0;

    if (pDcrs == NULL) {
        return;
    }
    for (i = 0; i < size; i++) {
        if (pDcrs[i].address != NULL) {
            if (strlen(pDcrs[i].address) != 0)
                free(pDcrs[i].address);
        }
        if (pDcrs[i].netPortName != NULL) {
            free(pDcrs[i].netPortName);
        }
        if (pDcrs[i].type != NULL) {
            free(pDcrs[i].type);
        }
    }
    free(pDcrs);
}

void DataReportMessage(
    const ReqDataInfo *requestInfo, ModemReportErrorInfo errInfo, HRilDataCallResponse *pDataCalls, int validNum)
{
    struct ReportInfo reportInfo = {};
    if (requestInfo != NULL) {
        /* Report results */
        reportInfo = CreateReportInfo((void *)requestInfo, errInfo.errorNo, HRIL_RESPONSE, 0);
        reportInfo.modemErrInfo = errInfo;
        OnDataReport(reportInfo, pDataCalls, validNum * sizeof(HRilDataCallResponse));
    } else {
        reportInfo.error = errInfo.errorNo;
        reportInfo.modemErrInfo = errInfo;
    }

    TELEPHONY_LOGD("Data report HRIL_NOTIFICATION start! validNum:%{public}d", validNum);
    /* Change notice */
    reportInfo.notifyId = HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED;
    reportInfo.type = HRIL_NOTIFICATION;
    OnDataReport(reportInfo, pDataCalls, validNum * sizeof(HRilDataCallResponse));
    FreeDataCallResponse(pDataCalls, validNum);
}

void PdpContextListUpdated(const ReqDataInfo *requestInfo)
{
    int err = HRIL_ERR_SUCCESS;
    int validNum = 0;
    int dataCallNum = 0;
    HRilDataCallResponse *pDataCalls = NULL;
    ModemReportErrorInfo errInfo = {
        .errorNo = HRIL_ERR_SUCCESS,
        .errType = HRIL_REPORT_ERR_TYPE_NONE
    };

    errInfo = SendInquireCGACT(&dataCallNum, &pDataCalls);
    if (errInfo.errorNo != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("SendInquireCGACT send failed");
        err = errInfo.errorNo;
        goto REPORT;
    }

    errInfo = SendInquireCGDCONT(&validNum, dataCallNum, &pDataCalls);
    if (errInfo.errorNo != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("SendInquireCGDCONT send failed");
        err = errInfo.errorNo;
        goto REPORT;
    }

    DataReportMessage(requestInfo, errInfo, pDataCalls, validNum);
    return;

REPORT:
    TELEPHONY_LOGE("Data report error!");
    if (pDataCalls == NULL) {
        validNum = 1;
        pDataCalls = CreatDataCallResponseAndInit(validNum);
    }

    DataReportMessage(requestInfo, errInfo, pDataCalls, validNum);
}

void ReqActivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data)
{
    int ret;
    int err = HRIL_ERR_SUCCESS;
    char cmd[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    ResponseInfo *pResponse = NULL;
    const HRilDataInfo *pDataInfo = data;
    ModemReportErrorInfo errInfo = {
        .errorNo = HRIL_ERR_SUCCESS,
        .errType = HRIL_REPORT_ERR_TYPE_NONE
    };

    if (data == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        err = HRIL_ERR_INVALID_PARAMETER;
        goto REPORT_ERR;
    }

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CGDCONT=1,\"%s\",\"%s\"", pDataInfo->type, pDataInfo->apn);
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret != 0 || !pResponse->success) {
        errInfo = GetReportErrorInfo(pResponse);
        err = errInfo.errorNo;
        free(cmd);
        TELEPHONY_LOGE("send AT CMD failed!, ret:%{public}d", errInfo.errorNo);
        goto REPORT_ERR;
    }
    FreeResponseInfo(pResponse);

    ret = SendCommandLock("AT+CGACT=1,1", NULL, 0, &pResponse);
    if (ret != 0 || !pResponse->success) {
        errInfo = GetReportErrorInfo(pResponse);
        err = errInfo.errorNo;
        TELEPHONY_LOGE("send AT CMD failed!, ret:%{public}d", errInfo.errorNo);
        goto REPORT_ERR;
    }
    FreeResponseInfo(pResponse);
    PdpContextListUpdated(requestInfo);
    return;

REPORT_ERR:
    TELEPHONY_LOGE("Data report error.");
    FreeResponseInfo(pResponse);
    reportInfo = CreateReportInfo((void *)requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnDataReport(reportInfo, NULL, 0);
}

void ReqDeactivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data)
{
    int ret = 0;
    int err = HRIL_ERR_SUCCESS;
    char cmd[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    ResponseInfo *pResponse = NULL;
    const HRilDataInfo *pDataInfo = data;
    ModemReportErrorInfo errInfo = {
        .errorNo = HRIL_ERR_SUCCESS,
        .errType = HRIL_REPORT_ERR_TYPE_NONE
    };

    if (data == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        err = HRIL_ERR_INVALID_PARAMETER;
        goto REPORT_ERR;
    }

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CGACT=0,%d", pDataInfo->cid);
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret != 0 || !pResponse->success) {
        errInfo = GetReportErrorInfo(pResponse);
        err = errInfo.errorNo;
        TELEPHONY_LOGE("send AT CMD failed!, ret:%{public}d", errInfo.errorNo);
        goto REPORT_ERR;
    }
    FreeResponseInfo(pResponse);
    PdpContextListUpdated(requestInfo);
    return;

REPORT_ERR:
    TELEPHONY_LOGE("Data report error.");
    FreeResponseInfo(pResponse);
    reportInfo = CreateReportInfo((void *)requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnDataReport(reportInfo, NULL, 0);
}
