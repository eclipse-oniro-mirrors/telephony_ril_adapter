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

#include "securec.h"

#include "at_support.h"
#include "hril_notification.h"
#include "vendor_report.h"

static void OnDataReportErrorMessages(const ReqDataInfo *requestInfo, uint32_t err, ResponseInfo *pResponse)
{
    uint32_t errorNo = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    ModemReportErrorInfo errInfo = {
        .errorNo = HRIL_ERR_SUCCESS,
        .errType = HRIL_REPORT_ERR_TYPE_NONE
    };
    errInfo = GetReportErrorInfo(pResponse);
    errorNo = (err != HRIL_ERR_SUCCESS) ? err : errInfo.errorNo;
    FreeResponseInfo(pResponse);
    if (requestInfo != NULL) {
        reportInfo = CreateReportInfo(requestInfo, errorNo, HRIL_RESPONSE, 0);
    } else {
        reportInfo = CreateReportInfo(requestInfo, errorNo, HRIL_NOTIFICATION, HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED);
    }
    reportInfo.modemErrInfo = errInfo;
    OnDataReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
}

int ParsePdpCmd(char *str, HRilDataCallResponse *outData)
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

static HRilDataCallResponse *CreatDataCallResponseAndInit(int count)
{
    int size;
    HRilDataCallResponse *newDcr = NULL;

    size = count * sizeof(HRilDataCallResponse);
    newDcr = (HRilDataCallResponse *)calloc(DEFAULT_MEM_NMEMB, size);
    return newDcr;
}

static ModemReportErrorInfo SendInquireCGACT(int *outDataNum, HRilDataCallResponse **ppDcr)
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
        TELEPHONY_LOGE("send AT CMD failed! ret:%{public}d", ret ? ret : errInfo.errorNo);
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

static void BuildDataInfoList(int *validCount, int dataNum, ResponseInfo *response, HRilDataCallResponse **ppDcr)
{
    int ret;
    int i = 0;
    int count = 0;
    int dataCallNum = 0;
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

static int AddressFormat(uint64_t addr, char *outBuf, size_t bufLen)
{
    int data[IVP4_INDEX_MAX] = {0};
    const int index1st = 0;
    const int index2nd = 1;
    const int index3rd = 2;
    const int index4th = 3;

    if ((outBuf == NULL) || !bufLen) {
        TELEPHONY_LOGE("outBuf is Null or bufLen is zero!");
        return -HRIL_ERR_GENERIC_FAILURE;
    }
    int ret = memset_s(outBuf, bufLen, 0, bufLen);
    if (ret != EOK) {
        TELEPHONY_LOGE("memset_s outBuf is failed!");
        return -HRIL_ERR_GENERIC_FAILURE;
    }
    data[index1st] = (int)((addr >> ADDR_OFFSET_0BIT) & ADDR_MASK);
    data[index2nd] = (int)((addr >> ADDR_OFFSET_8BIT) & ADDR_MASK);
    data[index3rd] = (int)((addr >> ADDR_OFFSET_16BIT) & ADDR_MASK);
    data[index4th] = (int)((addr >> ADDR_OFFSET_24BIT) & ADDR_MASK);
    GenerateCommand(outBuf, MAX_CMD_LENGTH, "%d.%d.%d.%d",
        data[index1st], data[index2nd], data[index3rd], data[index4th]);
    return HRIL_ERR_SUCCESS;
}

static ModemReportErrorInfo GetLinkInformation(int activeIndex, HRilDataCallResponse **ppDcr)
{
    int ret;
    char readBuf[MAX_CMD_LENGTH] = {0};
    char *lineStr = NULL;
    Line *pLine = NULL;
    ResponseInfo *pResponse = NULL;
    ModemReportErrorInfo errInfo = {
        .errorNo = HRIL_ERR_SUCCESS,
        .errType = HRIL_REPORT_ERR_TYPE_NONE
    };

    ret = SendCommandLock("AT^DHCP?", "^DHCP:", 0, &pResponse);
    if (ret != 0 || !pResponse->success) {
        errInfo = GetReportErrorInfo(pResponse);
        TELEPHONY_LOGE("send AT CMD failed! ret:%{public}d", errInfo.errorNo);
        FreeResponseInfo(pResponse);
        return errInfo;
    }
    pLine = pResponse->head;
    lineStr = pLine->data;
    uint64_t addr = 0;
    SkipATPrefix(&lineStr);
    NextULongFromHex(&lineStr, &addr); // ip
    AddressFormat(addr, readBuf, MAX_CMD_LENGTH);
    ret = strlen(readBuf);
    readBuf[ret] = '.';
    NextULongFromHex(&lineStr, &addr); // subnet mask
    ret = strlen(readBuf);
    AddressFormat(addr, &readBuf[ret], MAX_CMD_LENGTH - ret);
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
    return errInfo;
}

static ModemReportErrorInfo SendInquireCGDCONT(int *validCount, int dataNum, HRilDataCallResponse **ppDcr)
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

static int QueryAllSupportPDNInfos(PDNInfo *pdnInfo)
{
    char *pStr = NULL;
    int ret = -1;
    Line *pLine = NULL;
    PDNInfo *pdns = pdnInfo;
    ResponseInfo *pResponse = NULL;
    ret = SendCommandLock("AT+CGDCONT?", "+CGDCONT:", 0, &pResponse);
    if (ret != 0 || !pResponse->success) {
        if (!pResponse->success) {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        TELEPHONY_LOGE("send AT CMD failed! ret:%{public}d", ret);
        FreeResponseInfo(pResponse);
        return ret;
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

static int IsStrEmpty(const char *str)
{
    if (str == NULL || strcmp(str, "") == 0) {
        return TRUE;
    }
    return FALSE;
}

static int IsStrEqual(const char *src1, const char *src2)
{
    int ret = FALSE;
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

static int QuerySupportCID(const PDNInfo *pdnInfos, int32_t pdnSize, const char *apn, const char *ipType)
{
    int32_t i;
    int cid = INT_DEFAULT_VALUE;
    if (pdnInfos == NULL) {
        return cid;
    }

    for (i = 0; i < pdnSize; i++) {
        if (IsStrEqual(apn, pdnInfos[i].apn)) {
            cid = pdnInfos[i].cid;
            break;
        }
    }
    if (cid < 0) {
        for (i = MIN_CID; i < MAX_CID; i++) {
            if (pdnInfos[i].cid != i) {
                break;
            }
        }
        cid = i;
    }
    return cid;
}

static int GetNeedActivateCid(const char *apn, const char *ipType)
{
    int cid = DEFAULT_CID;
    PDNInfo pdnInfos[MAX_PDP_NUM] = {
        {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""},
        {DEFAULT_CID, "", ""}
    };

    if (!QueryAllSupportPDNInfos(pdnInfos)) {
        cid = QuerySupportCID(pdnInfos, MAX_PDP_NUM, apn, ipType);
    }
    if (cid < 0) {
        cid = 0;
    }
    return cid;
}

static void FreeDataCallResponse(HRilDataCallResponse *pDcrs, int size)
{
    int i = 0;

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

static void DataReportMessage(int cid, const ReqDataInfo *requestInfo, ModemReportErrorInfo errInfo,
    HRilDataCallResponse *pDataCalls, int validNum)
{
    struct ReportInfo reportInfo;
    if (requestInfo != NULL) {
        /* Report results */
        int index;
        reportInfo = CreateReportInfo(requestInfo, errInfo.errorNo, HRIL_RESPONSE, 0);
        reportInfo.modemErrInfo = errInfo;
        if ((cid == DEFAULT_CID) || (pDataCalls == NULL)) {
            OnDataReport(
                HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)pDataCalls, validNum * sizeof(HRilDataCallResponse));
            FreeDataCallResponse(pDataCalls, validNum);
            return;
        }
        for (index = 0; index < validNum; index++) {
            if (pDataCalls[index].cid == cid) {
                break;
            }
        }
        OnDataReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&pDataCalls[index], sizeof(HRilDataCallResponse));
    } else {
        /* Change notice */
        reportInfo.requestInfo = NULL;
        reportInfo.error = errInfo.errorNo;
        reportInfo.modemErrInfo = errInfo;
        reportInfo.notifyId = HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED;
        reportInfo.type = HRIL_NOTIFICATION;
        OnDataReport(
            HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)pDataCalls, validNum * sizeof(HRilDataCallResponse));
    }
    FreeDataCallResponse(pDataCalls, validNum);
}

static void InquirePdpContextList(int cid, const ReqDataInfo *requestInfo)
{
    int validNum = 0;
    int queryCount = 0;
    int dataCallNum = 0;
    HRilDataCallResponse *pDataCalls = NULL;
    ModemReportErrorInfo errInfo = {
        .errorNo = HRIL_ERR_SUCCESS,
        .errType = HRIL_REPORT_ERR_TYPE_NONE
    };

    do {
        errInfo = SendInquireCGACT(&dataCallNum, &pDataCalls);
        if (errInfo.errorNo != HRIL_ERR_SUCCESS) {
            TELEPHONY_LOGE("SendInquireCGACT send failed");
            OnDataReportErrorMessages(requestInfo, errInfo.errorNo, NULL);
            FreeDataCallResponse(pDataCalls, validNum);
            return;
        }
        for (int i = 0; i < dataCallNum; i++) {
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
    for (int index = 0; index < validNum; index++) {
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

static int SendCmdCGDCONT(int cid, const ReqDataInfo *requestInfo, const HRilDataInfo *pDataInfo)
{
    int ret;
    int err = HRIL_ERR_SUCCESS;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;

    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CGDCONT=%d,\"%s\",\"%s\"", cid, pDataInfo->type, pDataInfo->apn);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return ret;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if (ret != 0 || !pResponse->success) {
        err = (ret != HRIL_ERR_SUCCESS) ? ret : err;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
        OnDataReportErrorMessages(requestInfo, err, pResponse);
        return ret;
    }
    FreeResponseInfo(pResponse);
    return HRIL_ERR_SUCCESS;
}

static int SendCmdNDISDUP(int cid, int activate, const ReqDataInfo *requestInfo)
{
    int ret;
    int err = HRIL_ERR_SUCCESS;
    char cmd[MAX_CMD_LENGTH] = {0};
    ResponseInfo *pResponse = NULL;

    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^NDISDUP=%d,%d", cid, activate);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
        return ret;
    }
    ret = SendCommandLock(cmd, NULL, 0, &pResponse);
    if ((ret != HRIL_ERR_SUCCESS) || !pResponse->success) {
        err = (ret != HRIL_ERR_SUCCESS) ? ret : err;
        TELEPHONY_LOGE("cmd send failed, err:%{public}d", err);
        OnDataReportErrorMessages(requestInfo, err, pResponse);
        return ret;
    }
    FreeResponseInfo(pResponse);
    return HRIL_ERR_SUCCESS;
}

static int RouteUp(void)
{
    int ret;
    char cmd[MAX_CMD_LENGTH] = {0};

    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "ifconfig %s up", NET_NODE);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        return ret;
    }
    ret = system(cmd);
    if (ret != 0) {
        TELEPHONY_LOGE("exec system is failed! ret:%{public}d, %{public}s", ret, strerror(ret));
        return ret;
    }
    return HRIL_ERR_SUCCESS;
}

static int RouteDown(void)
{
    int ret;
    char cmd[MAX_CMD_LENGTH] = {0};

    ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "ifconfig %s down", NET_NODE);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand is failed!");
        return ret;
    }
    ret = system(cmd);
    if (ret != 0) {
        TELEPHONY_LOGE("exec system is failed! ret:%{public}d, %{public}s", ret, strerror(ret));
        return ret;
    }
    return HRIL_ERR_SUCCESS;
}

void ReqActivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data)
{
    int cid = INT_DEFAULT_VALUE;
    const HRilDataInfo *pDataInfo = data;

    if (pDataInfo == NULL) {
        TELEPHONY_LOGE("data is null!!!");
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_INVALID_PARAMETER, NULL);
        return;
    }
    if (RouteUp() != HRIL_ERR_SUCCESS) {
        OnDataReportErrorMessages(requestInfo, HRIL_ERR_GENERIC_FAILURE, NULL);
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
    ModemReportErrorInfo errInfo = {
        .errorNo = HRIL_ERR_SUCCESS,
        .errType = HRIL_REPORT_ERR_TYPE_NONE
    };

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