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

#include <stdlib.h>

#include "at_support.h"
#include "hril_notification.h"
#include "securec.h"
#include "vendor_report.h"

static int32_t ConvertPdpFailReason(int32_t errorCode)
{
    int32_t modemErrorCode = errorCode;
    int32_t errNo;
    switch (modemErrorCode) {
        case PDP_CME_ERR_USER_VERIFICATION:
        case PDP_CME_ERR_INCORRECT_PASSWORD:
            errNo = HRIL_PDP_ERR_USER_VERIFICATION;
            break;
        case PDP_CME_ERR_INCORRECT_PARAMETERS:
        case PDP_CME_ERR_OPERATOR_DETERMINED_BARRING:
            errNo = HRIL_PDP_ERR_OPERATOR_DETERMINED_BARRING;
            break;
        case PDP_CME_ERR_SHORTAGE_RESOURCES:
            errNo = HRIL_PDP_ERR_SHORTAGE_RESOURCES;
            break;
        case PDP_CME_ERR_MISSING_OR_UNKNOWN_APN:
            errNo = HRIL_PDP_ERR_MISSING_OR_UNKNOWN_APN;
            break;
        case PDP_CME_ERR_UNKNOWN_PDP_ADDR_OR_TYPE:
            errNo = HRIL_PDP_ERR_UNKNOWN_PDP_ADDR_OR_TYPE;
            break;
        case PDP_CME_ERR_ACTIVATION_REJECTED_GGSN:
            errNo = HRIL_PDP_ERR_ACTIVATION_REJECTED_GGSN;
            break;
        case PDP_CME_ERR_SERVICE_ACTIVATION_REJECTED_UNSPECIFIED:
            errNo = HRIL_PDP_ERR_ACTIVATION_REJECTED_UNSPECIFIED;
            break;
        case PDP_CME_ERR_SERVICE_OPTION_NOT_SUPPORTED:
            errNo = HRIL_PDP_ERR_SERVICE_OPTION_NOT_SUPPORTED;
            break;
        case PDP_CME_ERR_SERVICE_OPTION_NOT_SUBSCRIBED:
            errNo = HRIL_PDP_ERR_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED;
            break;
        case PDP_CME_ERR_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER:
            errNo = HRIL_PDP_ERR_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER;
            break;
        case PDP_CME_ERR_NSAPI_ALREADY_USED:
            errNo = HRIL_PDP_ERR_NSAPI_ALREADY_USED;
            break;
        case PDP_CME_ERR_PROTOCOL_ERRORS:
            errNo = HRIL_PDP_ERR_PROTOCOL_ERRORS;
            break;
        default:
            errNo = HRIL_PDP_ERR_UNKNOWN;
            break;
    }
    return errNo;
}

static int32_t OnDataReportErrorMessages(const ReqDataInfo *requestInfo, int32_t err, ResponseInfo *pResponse)
{
    int32_t errorNo = HRIL_ERR_SUCCESS;

    int32_t slotId = GetSlotId(requestInfo);
    ModemReportErrorInfo errInfo = GetReportErrorInfo(pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        if (err == AT_ERR_CHANNEL_CLOSED) {
            err = HRIL_ERR_MODEM_DEVICE_CLOSE;
        } else {
            err = HRIL_ERR_CMD_SEND_FAILURE;
        }
    }
    errorNo = (err != HRIL_ERR_SUCCESS) ? err : errInfo.errorNo;
    FreeResponseInfo(pResponse);
    if (requestInfo != NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, errorNo, HRIL_RESPONSE, 0);
        reportInfo.modemErrInfo = errInfo;
        OnDataReport(slotId, reportInfo, NULL, 0);
    } else {
        struct ReportInfo reportInfo =
            CreateReportInfo(requestInfo, errorNo, HRIL_NOTIFICATION, HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED);
        reportInfo.modemErrInfo = errInfo;
        OnDataReport(GetSlotId(NULL), reportInfo, NULL, 0);
    }
    return errorNo;
}

static int32_t OnDataReportPdpErrorMessages(const ReqDataInfo *requestInfo, int32_t err, ResponseInfo *pResponse)
{
    HRilDataCallResponse pDataCall = {};
    ModemReportErrorInfo errInfo = GetReportErrorInfo(pResponse);
    FreeResponseInfo(pResponse);
    if (err != HRIL_ERR_SUCCESS) {
        if (err == AT_ERR_CHANNEL_CLOSED) {
            err = HRIL_ERR_MODEM_DEVICE_CLOSE;
        } else {
            err = HRIL_ERR_CMD_SEND_FAILURE;
        }
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    pDataCall.reason = ConvertPdpFailReason(errInfo.errorNo);
    pDataCall.retryTime = INT_DEFAULT_VALUE;
    pDataCall.cid = INT_DEFAULT_VALUE;
    OnDataReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&pDataCall, sizeof(HRilDataCallResponse));
    return (err != HRIL_ERR_SUCCESS) ? err : errInfo.errorNo;
}

int32_t ParsePdpCmd(char *str, HRilDataCallResponse *outData)
{
    char *pStr = NULL;

    if (str == NULL || outData == NULL) {
        TELEPHONY_LOGE("param error: str=%{public}p,outData=%{public}p", str, outData);
        return HRIL_ERR_NULL_POINT;
    }
    pStr = str;
    if (ReportStrWith(str, "+CGACT:")) {
        if (SkipATPrefix(&pStr) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
        if (NextInt(&pStr, &outData->cid) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
        if (NextInt(&pStr, &outData->active) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
        outData->reason = HRIL_PDP_ERR_RETRY;
    } else if (ReportStrWith(str, "+CGDCONT:")) {
        if (SkipATPrefix(&pStr) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
        if (NextInt(&pStr, &outData->cid) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
        if (NextStr(&pStr, &outData->type) < 0) {
            return HRIL_ERR_NULL_POINT;
        }
    } else {
        return HRIL_ERR_NULL_POINT;
    }
    return HRIL_ERR_SUCCESS;
}

static HRilDataCallResponse *CreatDataCallResponseAndInit(int32_t count)
{
    int32_t size;
    HRilDataCallResponse *newDcr = NULL;

    size = count * sizeof(HRilDataCallResponse);
    if (size <= 0) {
        TELEPHONY_LOGE("param is error, size=%{public}d", size);
        return newDcr;
    }
    newDcr = (HRilDataCallResponse *)malloc(size);
    if (newDcr == NULL) {
        return NULL;
    }
    (void)memset_s(newDcr, size, 0, size);
    return newDcr;
}

static ModemReportErrorInfo SendInquireCGACT(int32_t *outDataNum, HRilDataCallResponse **ppDcr)
{
    int32_t ret;
    int32_t dataCallNum = 0;
    Line *pLine = NULL;
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {};

    ret = SendCommandLock("AT+CGACT?", "+CGACT:", 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success || outDataNum == NULL || ppDcr == NULL) {
        errInfo = GetReportErrorInfo(pResponse);
        errInfo.errorNo = (ret != HRIL_ERR_SUCCESS) ? ret : errInfo.errorNo;
        TELEPHONY_LOGE("send AT CMD failed! ret:%{public}d", errInfo.errorNo);
        FreeResponseInfo(pResponse);
        return errInfo;
    }

    for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next) {
        dataCallNum++;
    }

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

static void BuildDataInfoList(
    int32_t *validCount, int32_t dataNum, ResponseInfo *response, HRilDataCallResponse **ppDcr)
{
    if (validCount == NULL || response == NULL || ppDcr == NULL || *ppDcr == NULL) {
        return;
    }
    int32_t ret;
    int32_t i = 0;
    int32_t count = 0;
    int32_t dataCallNum = 0;
    Line *pLine = NULL;
    ResponseInfo *pResponse = response;
    HRilDataCallResponse *pDataCall = *ppDcr;
    HRilDataCallResponse dataCGDCONT;

    dataCallNum = dataNum;
    for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next) {
        ret = memset_s(&dataCGDCONT, sizeof(HRilDataCallResponse), 0, sizeof(HRilDataCallResponse));
        if (ret != EOK) {
            TELEPHONY_LOGE("memset_s is failed!");
        }
        ret = ParsePdpCmd(pLine->data, &dataCGDCONT);
        if (ret != 0) {
            TELEPHONY_LOGE("parser pdp command failed: [%{public}s],ret=%{public}d", pLine->data, ret);
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
        pDataCall->type = strdup(dataCGDCONT.type);
        pDataCall->netPortName = strdup(NET_NODE);
        pDataCall++;
        count++;
    }
    *validCount = count;
}

static int32_t AddressFormat(uint64_t addr, char *outBuf, size_t bufLen)
{
    int32_t data[IPV4_INDEX_MAX] = { 0 };
    const int32_t index1st = 0;
    const int32_t index2nd = 1;
    const int32_t index3rd = 2;
    const int32_t index4th = 3;

    if ((outBuf == NULL) || !bufLen) {
        TELEPHONY_LOGE("outBuf is Null or bufLen is zero!");
        return -HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = memset_s(outBuf, bufLen, 0, bufLen);
    if (ret != EOK) {
        TELEPHONY_LOGE("memset_s outBuf is failed!");
        return -HRIL_ERR_GENERIC_FAILURE;
    }
    data[index1st] = (int32_t)((addr >> ADDR_OFFSET_0BIT) & ADDR_MASK);
    data[index2nd] = (int32_t)((addr >> ADDR_OFFSET_8BIT) & ADDR_MASK);
    data[index3rd] = (int32_t)((addr >> ADDR_OFFSET_16BIT) & ADDR_MASK);
    data[index4th] = (int32_t)((addr >> ADDR_OFFSET_24BIT) & ADDR_MASK);
    GenerateCommand(
        outBuf, MAX_CMD_LENGTH, "%d.%d.%d.%d", data[index1st], data[index2nd], data[index3rd], data[index4th]);
    return HRIL_ERR_SUCCESS;
}

static int32_t NetMaskFormat(uint64_t addr, char *outBuf, size_t bufLen)
{
    if (addr < IPV4_MASK_MIN || addr > IPV4_MASK_MAX) {
        return HRIL_ERR_NULL_POINT;
    }
    if ((outBuf == NULL) || !bufLen) {
        TELEPHONY_LOGE("outBuf is Null or bufLen is zero!");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = memset_s(outBuf, bufLen, 0, bufLen);
    if (ret != EOK) {
        TELEPHONY_LOGE("memset_s outBuf is failed!");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t prefixLen = IPV4_MASK_LEN_MIN;
    for (int32_t i = ADDR_OFFSET_8BIT; i <= ADDR_OFFSET_24BIT; i += ADDR_OFFSET_8BIT) {
        uint32_t maskValue = (addr >> i) & ADDR_MASK;
        while ((maskValue & IPV4_BIT_MASK) != 0) {
            prefixLen++;
            maskValue = (maskValue << 1);
        }
    }
    GenerateCommand(outBuf, MAX_CMD_LENGTH, "/%d", prefixLen);
    return HRIL_ERR_SUCCESS;
}

static ModemReportErrorInfo GetLinkInformation(int32_t activeIndex, HRilDataCallResponse **ppDcr)
{
    int32_t ret;
    char readBuf[MAX_CMD_LENGTH] = {0};
    char *lineStr = NULL;
    Line *pLine = NULL;
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {};

    ret = SendCommandLock("AT^DHCP?", "^DHCP:", 0, &pResponse);
    if (ret != 0 || pResponse == NULL || pResponse->head == NULL || !pResponse->success) {
        errInfo = GetReportErrorInfo(pResponse);
        errInfo.errorNo = (ret != HRIL_ERR_SUCCESS) ? ret : errInfo.errorNo;
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
    pLine = pResponse->head;
    lineStr = pLine->data;
    uint64_t addr = 0;
    SkipATPrefix(&lineStr);
    NextULongFromHex(&lineStr, &addr); // ip
    AddressFormat(addr, readBuf, MAX_CMD_LENGTH);
    NextULongFromHex(&lineStr, &addr); // subnet mask
    ret = strlen(readBuf);
    NetMaskFormat(addr, &readBuf[ret], MAX_CMD_LENGTH - ret);
    (*ppDcr)[activeIndex].address = strdup(readBuf);
    NextULongFromHex(&lineStr, &addr); // default gateway
    AddressFormat(addr, readBuf, MAX_CMD_LENGTH);
    (*ppDcr)[activeIndex].gateway = strdup(readBuf);
    NextULongFromHex(&lineStr, &addr); // DHCP server
    NextULongFromHex(&lineStr, &addr); // primary DNS server
    AddressFormat(addr, readBuf, MAX_CMD_LENGTH);
    (*ppDcr)[activeIndex].dns = strdup(readBuf);
    NextULongFromHex(&lineStr, &addr); // secondary DNS server
    FreeResponseInfo(pResponse);
    (*ppDcr)[activeIndex].reason = HRIL_PDP_ERR_NONE;
    return errInfo;
}

static ModemReportErrorInfo SendInquireCGDCONT(int32_t *validCount, int32_t dataNum, HRilDataCallResponse **ppDcr)
{
    int32_t ret;
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {};

    ret = SendCommandLock("AT+CGDCONT?", "+CGDCONT:", 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        errInfo = GetReportErrorInfo(pResponse);
        errInfo.errorNo = (ret != HRIL_ERR_SUCCESS) ? ret : errInfo.errorNo;
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

static int32_t QueryAllSupportPDNInfos(PDNInfo *pdnInfo)
{
    if (pdnInfo == NULL) {
        return -1;
    }
    char *pStr = NULL;
    int32_t ret = -1;
    int32_t err = HRIL_ERR_SUCCESS;
    Line *pLine = NULL;
    PDNInfo *pdns = pdnInfo;
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("AT+CGDCONT?", "+CGDCONT:", 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        ModemReportErrorInfo errInfo = GetReportErrorInfo(pResponse);
        errInfo.errorNo = (ret != HRIL_ERR_SUCCESS) ? ret : errInfo.errorNo;
        TELEPHONY_LOGE("send AT CMD failed! ret:%{public}d", err);
        FreeResponseInfo(pResponse);
        return err;
    }

    for (pLine = pResponse->head; pLine != NULL; pLine = pLine->next) {
        pStr = pLine->data;
        ret = SkipATPrefix(&pStr);
        if (ret < 0) {
            pdns->cid = INT_DEFAULT_VALUE;
        }
        ret = NextInt(&pStr, &pdns->cid);
        if (ret < 0) {
            pdns->cid = INT_DEFAULT_VALUE;
        }
        ret = NextStr(&pStr, &pdns->ipType);
        if (ret < 0) {
            pdns->cid = INT_DEFAULT_VALUE;
        }
        ret = NextStr(&pStr, &pdns->apn);
        if (ret < 0) {
            pdns->cid = INT_DEFAULT_VALUE;
        }
        pdns++;
    }
    FreeResponseInfo(pResponse);
    return ret;
}

static int32_t IsStrEmpty(const char *str)
{
    if (str == NULL || strcmp(str, "") == 0) {
        return TRUE;
    }
    return FALSE;
}

static int32_t IsStrEqual(const char *src1, const char *src2)
{
    int32_t ret = FALSE;
    if (IsStrEmpty(src1) && IsStrEmpty(src2)) {
        ret = TRUE;
    } else if (!IsStrEmpty(src1) && !IsStrEmpty(src2)) {
        if (strcasecmp(src1, src2) == 0) {
            ret = TRUE;
        } else {
            TELEPHONY_LOGE("IsStrEqual src1=%{public}s, src2=%{public}s", src1, src2);
        }
    } else {
        TELEPHONY_LOGE("IsStrEqual src1 or src2 is empty!");
    }
    return ret;
}

static int32_t QuerySupportCID(const PDNInfo *pdnInfos, int32_t pdnSize, const char *apn, const char *ipType)
{
    int32_t i;
    int32_t j;
    int32_t isUsedCid = 0;
    int32_t cid = INT_DEFAULT_VALUE;
    if (pdnInfos == NULL) {
        return cid;
    }

    for (j = 0; j < pdnSize; j++) {
        if (IsStrEqual(apn, pdnInfos[j].apn)) {
            cid = pdnInfos[j].cid;
            break;
        }
    }
    if (cid > 0) {
        return cid;
    }
    for (i = MIN_CID; i <= MAX_CID; i++) {
        isUsedCid = 0;
        for (j = 0; j < pdnSize; j++) {
            if (pdnInfos[j].cid == i) {
                isUsedCid = 1;
                break;
            }
        }
        if (isUsedCid == 1) {
            break;
        }
    }
    cid = i;
    return cid;
}

static int32_t GetNeedActivateCid(const char *apn, const char *ipType)
{
    int32_t cid = DEFAULT_CID;
    PDNInfo pdnInfos[MAX_PDP_NUM] = {{DEFAULT_CID, "", ""}, {DEFAULT_CID, "", ""}, {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""}, {DEFAULT_CID, "", ""}, {DEFAULT_CID, "", ""}, {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""}, {DEFAULT_CID, "", ""}, {DEFAULT_CID, "", ""}, {DEFAULT_CID, "", ""}};

    if (!QueryAllSupportPDNInfos(pdnInfos)) {
        cid = QuerySupportCID(pdnInfos, MAX_PDP_NUM, apn, ipType);
    }
    if (cid <= 0) {
        cid = MIN_CID;
    }
    return cid;
}

static void FreeDataCallResponse(HRilDataCallResponse *pDcrs, int32_t size)
{
    int32_t i = 0;

    if (pDcrs == NULL) {
        return;
    }
    for (i = 0; i < size; i++) {
        if (pDcrs[i].address != NULL) {
            free(pDcrs[i].address);
        }
        if (pDcrs[i].netPortName != NULL) {
            free(pDcrs[i].netPortName);
        }
        if (pDcrs[i].type != NULL) {
            free(pDcrs[i].type);
        }
        if (pDcrs[i].dns != NULL) {
            free(pDcrs[i].dns);
        }
        if (pDcrs[i].gateway != NULL) {
            free(pDcrs[i].gateway);
        }
    }
    free(pDcrs);
}

static void DataReportMessage(int32_t cid, const ReqDataInfo *requestInfo, ModemReportErrorInfo errInfo,
    HRilDataCallResponse *pDataCalls, int32_t validNum)
{
    struct ReportInfo reportInfo = {};
    int32_t slotId = GetSlotId(requestInfo);
    if (requestInfo != NULL) {
        /* Report results */
        int32_t index;
        reportInfo = CreateReportInfo(requestInfo, errInfo.errorNo, HRIL_RESPONSE, 0);
        reportInfo.modemErrInfo = errInfo;
        if ((cid == DEFAULT_CID) || (pDataCalls == NULL)) {
            OnDataReport(slotId, reportInfo, (const uint8_t *)pDataCalls, validNum * sizeof(HRilDataCallResponse));
            FreeDataCallResponse(pDataCalls, validNum);
            return;
        }
        for (index = 0; index < validNum; index++) {
            if (pDataCalls[index].cid == cid) {
                break;
            }
        }
        OnDataReport(slotId, reportInfo, (const uint8_t *)&pDataCalls[index], sizeof(HRilDataCallResponse));
    } else {
        /* Change notice */
        reportInfo.requestInfo = NULL;
        reportInfo.error = errInfo.errorNo;
        reportInfo.modemErrInfo = errInfo;
        reportInfo.notifyId = HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED;
        reportInfo.type = HRIL_NOTIFICATION;
        OnDataReport(GetSlotId(NULL), reportInfo, (const uint8_t *)pDataCalls, validNum * sizeof(HRilDataCallResponse));
    }
    FreeDataCallResponse(pDataCalls, validNum);
}

static void InquirePdpContextList(int32_t cid, const ReqDataInfo *requestInfo)
{
    int32_t validNum = 0;
    int32_t queryCount = 0;
    int32_t dataCallNum = 0;
    HRilDataCallResponse *pDataCalls = NULL;
    ModemReportErrorInfo errInfo = {};

    do {
        errInfo = SendInquireCGACT(&dataCallNum, &pDataCalls);
        if (errInfo.errorNo != HRIL_ERR_SUCCESS) {
            TELEPHONY_LOGE("SendInquireCGACT send failed");
            OnDataReportErrorMessages(requestInfo, errInfo.errorNo, NULL);
            FreeDataCallResponse(pDataCalls, validNum);
            return;
        }
        for (int32_t i = 0; i < dataCallNum; i++) {
            if ((pDataCalls[i].cid == cid) && (DEACTIVATE == pDataCalls[i].active)) {
                errInfo.errorNo = HRIL_ERR_GENERIC_FAILURE;
                usleep(QUERY_DELAY_MS * DELAY_US_OFFSET);
                queryCount++;
                break;
            }
        }
    } while ((cid != DEFAULT_CID) && (errInfo.errorNo != HRIL_ERR_SUCCESS) && (queryCount < QUERY_MAX_COUNT));

    errInfo = SendInquireCGDCONT(&validNum, dataCallNum, &pDataCalls);
    if (errInfo.errorNo != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("SendInquireCGDCONT send failed");
        OnDataReportErrorMessages(requestInfo, errInfo.errorNo, NULL);
        FreeDataCallResponse(pDataCalls, validNum);
        return;
    }
    for (int32_t index = 0; index < validNum; index++) {
        if (ACTIVATE == pDataCalls[index].active) {
            errInfo = GetLinkInformation(index, &pDataCalls);
            if (errInfo.errorNo != HRIL_ERR_SUCCESS) {
                TELEPHONY_LOGE("Get link information is failed!");
                OnDataReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
                FreeDataCallResponse(pDataCalls, validNum);
                return;
            }
        }
    }
    DataReportMessage(cid, requestInfo, errInfo, pDataCalls, validNum);
}

static int32_t SendCmdCGDCONT(int32_t cid, const ReqDataInfo *requestInfo, const HRilDataInfo *pDataInfo)
{
    if (pDataInfo == NULL) {
        return -1;
    }
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;
    ret = GenerateCommand(
        cmd, MAX_CMD_LENGTH, "AT+CGDCONT=%d,\"%s\",\"%s\",\"\",0,0", cid, pDataInfo->type, pDataInfo->apn);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnDataReportPdpErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return ret;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        err = (ret != HRIL_ERR_SUCCESS) ? ret : err;
        ret = OnDataReportPdpErrorMessages(requestInfo, err, pResponse);
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", ret);
        return ret;
    }
    FreeResponseInfo(pResponse);
    return HRIL_ERR_SUCCESS;
}

static int32_t SendCmdNDISDUP(int32_t cid, int32_t activate, const ReqDataInfo *requestInfo)
{
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;

    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^NDISDUP=%d,%d", cid, activate);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnDataReportPdpErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return ret;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if ((ret != HRIL_ERR_SUCCESS) || pResponse == NULL || !pResponse->success) {
        err = (ret != HRIL_ERR_SUCCESS) ? ret : err;
        ret = OnDataReportPdpErrorMessages(requestInfo, err, pResponse);
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", ret);
        return ret;
    }
    FreeResponseInfo(pResponse);
    return HRIL_ERR_SUCCESS;
}

static int32_t RouteUp(void)
{
    int32_t ret = system("ifconfig usb0 up");
    if (ret != 0) {
        TELEPHONY_LOGE("exec system is failed! ret:%{public}d, %{public}s", ret, strerror(ret));
        return ret;
    }
    TELEPHONY_LOGI("Open net device is finished!");
    return HRIL_ERR_SUCCESS;
}

static int32_t RouteDown(void)
{
    int32_t ret = system("ifconfig usb0 down");
    if (ret != 0) {
        TELEPHONY_LOGE("exec system is failed! ret:%{public}d, %{public}s", ret, strerror(ret));
        return ret;
    }
    TELEPHONY_LOGI("Close net device is finished!");
    return HRIL_ERR_SUCCESS;
}

void ReqActivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data)
{
    int32_t cid = INT_DEFAULT_VALUE;
    const HRilDataInfo *pDataInfo = data;

    if (pDataInfo == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        OnDataReportPdpErrorMessages(requestInfo, HRIL_ERR_INVALID_PARAMETER, NULL);
        return;
    }
    if (RouteUp() != HRIL_ERR_SUCCESS) {
        OnDataReportPdpErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    cid = GetNeedActivateCid(pDataInfo->apn, pDataInfo->type);
    if (SendCmdCGDCONT(cid, requestInfo, pDataInfo) != HRIL_ERR_SUCCESS) {
        RouteDown();
        return;
    }
    if (SendCmdNDISDUP(cid, ACTIVATE, requestInfo) != HRIL_ERR_SUCCESS) {
        RouteDown();
        return;
    }
    InquirePdpContextList(cid, requestInfo);
}

void ReqDeactivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data)
{
    const HRilDataInfo *pDataInfo = data;
    ModemReportErrorInfo errInfo = {};

    if (pDataInfo == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_INVALID_PARAMETER, NULL);
        return;
    }
    if (SendCmdNDISDUP(pDataInfo->cid, DEACTIVATE, requestInfo) != HRIL_ERR_SUCCESS) {
        RouteDown();
        return;
    }
    if (RouteDown() != HRIL_ERR_SUCCESS) {
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    DataReportMessage(pDataInfo->cid, requestInfo, errInfo, NULL, 0);
}

void ReqGetPdpContextList(const ReqDataInfo *requestInfo)
{
    InquirePdpContextList(DEFAULT_CID, requestInfo);
}

static void PdpContextListCallback(uint8_t *param)
{
    InquirePdpContextList(DEFAULT_CID, NULL);
}

void PdpContextListUpdate(void)
{
    struct timeval tv = {0, CALLBACK_DELAY_MS * DELAY_US_OFFSET};
    OnTimerCallback(PdpContextListCallback, NULL, &tv);
}

static int32_t SetDataProfileInfo(int32_t cid, const ReqDataInfo *requestInfo, const HRilDataInfo *pDataInfo)
{
    if (pDataInfo == NULL) {
        return -1;
    }
    int32_t ret;
    int32_t err = HRIL_ERR_SUCCESS;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;

    ret = GenerateCommand(
        cmd, MAX_CMD_LENGTH, "AT+CGDCONT=%d,\"%s\",\"%s\",\"\",0,0", cid, pDataInfo->type, pDataInfo->apn);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return ret;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        err = (ret != HRIL_ERR_SUCCESS) ? ret : err;
        ret = OnDataReportErrorMessages(requestInfo, err, pResponse);
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
        return ret;
    }
    FreeResponseInfo(pResponse);
    if ((pDataInfo->verType >= VERIFY_TYPE_MIN) && (pDataInfo->verType <= VERIFY_TYPE_MAX)) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^AUTHDATA=%d,%d,\"\",\"%s\",\"%s\"", cid, pDataInfo->verType,
            pDataInfo->password, pDataInfo->userName);
        if (ret < 0) {
            TELEPHONY_LOGE("GenerateCommand is failed!");
            OnDataReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
            return ret;
        }
        ret = SendCommandLock(cmd, NULL, 0, &pResponse);
        if (ret != 0 || pResponse == NULL || !pResponse->success) {
            err = (ret != HRIL_ERR_SUCCESS) ? ret : err;
            ret = OnDataReportErrorMessages(requestInfo, err, pResponse);
            TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
            return ret;
        }
        FreeResponseInfo(pResponse);
    }
    return HRIL_ERR_SUCCESS;
}

void ReqSetInitApnInfo(const ReqDataInfo *requestInfo, const HRilDataInfo *data)
{
    int32_t cid = INT_DEFAULT_VALUE;
    const HRilDataInfo *pDataInfo = data;
    ModemReportErrorInfo errInfo = {};

    if (pDataInfo == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_INVALID_PARAMETER, NULL);
        return;
    }
    cid = GetNeedActivateCid(pDataInfo->apn, pDataInfo->type);
    if (SetDataProfileInfo(cid, requestInfo, pDataInfo) != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("Set data profile info is failed!");
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, errInfo.errorNo, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnDataReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}

void ReqSetLinkBandwidthReportingRule(const ReqDataInfo *requestInfo, const HRilLinkBandwidthReportingRule *data)
{
    const HRilLinkBandwidthReportingRule *linkBandwidthRule = data;
    ModemReportErrorInfo errInfo = {};

    if (linkBandwidthRule == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_INVALID_PARAMETER, NULL);
        return;
    }
    TELEPHONY_LOGI("rat:%{public}d, delayMs:%{public}d", linkBandwidthRule->rat, linkBandwidthRule->delayMs);
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, errInfo.errorNo, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnDataReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}

static int32_t CallCmdC5GQOSRDP(const char *lineCmd, HRilLinkBandwidthInfo *outCall)
{
    char *pLine = (char *)lineCmd;
    if (pLine == NULL || outCall == NULL) {
        TELEPHONY_LOGE("pLine or outCall is null.");
        return HRIL_ERR_NULL_POINT;
    }
    if (SkipATPrefix(&pLine) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->cid) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->qi) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->dlGfbr) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->ulGfbr) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->dlMfbr) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->ulMfbr) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->ulSambr) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->dlSambr) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    if (NextInt(&pLine, &outCall->averagingWindow) < 0) {
        return HRIL_ERR_NULL_POINT;
    }
    return HRIL_ERR_SUCCESS;
}

void ReqGetLinkBandwidthInfo(const ReqDataInfo *requestInfo, const int32_t cid)
{
    int32_t ret;
    char *line = NULL;
    int32_t err = HRIL_ERR_SUCCESS;
    HRilLinkBandwidthInfo uplinkAndDownlinkBandwidth = {0};
    ResponseInfo *pResponse = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};

    ModemReportErrorInfo errInfo = InitModemReportErrorInfo();
    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+C5GQOSRDP=%d", cid);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return;
    }
    ret = SendCommandLock(cmd, "+C5GQOSRDP:", 0, &pResponse);
    if (ret || pResponse == NULL || !pResponse->success) {
        err = ret ? ret : err;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
        OnDataReportErrorMessages(requestInfo, err, pResponse);
        return;
    }
    if (pResponse->head) {
        line = pResponse->head->data;
        ret = CallCmdC5GQOSRDP(line, &uplinkAndDownlinkBandwidth);
        if (ret != 0) {
            TELEPHONY_LOGE("Parse C5GQOSRDP data is fail. ret:%{public}d", ret);
            return;
        }
        TELEPHONY_LOGI(
            "+C5GQOSRDP:%{public}d, %{public}d", uplinkAndDownlinkBandwidth.cid, uplinkAndDownlinkBandwidth.qi);
    } else {
        TELEPHONY_LOGE("ERROR: pResponse->head is null");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnDataReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&uplinkAndDownlinkBandwidth,
        sizeof(HRilLinkBandwidthInfo));
    FreeResponseInfo(pResponse);
}

void ReqSetDataPermitted(const ReqDataInfo *requestInfo, const int32_t dataPermitted)
{
    TELEPHONY_LOGI("dataPermitted:%{public}d", dataPermitted);
    ModemReportErrorInfo errInfo = {};
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, errInfo.errorNo, HRIL_RESPONSE, 0);
    reportInfo.modemErrInfo = errInfo;
    OnDataReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}
