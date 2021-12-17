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

#include "at_network.h"

#include <signal.h>

#include "hril_notification.h"
#include "vendor_report.h"
#include "vendor_util.h"

static struct ReportInfo g_reportInfoForOperListToUse;
static pthread_mutex_t g_networkSearchInformationMutex = PTHREAD_MUTEX_INITIALIZER;

struct ResponseAck {
    ResponseInfo *responseInfo;
    uint8_t *respDataPointer;
    int respDataLen;
};

static int GetResponseErrorCode(ResponseInfo *pResponseInfo)
{
    char *pLine = NULL;
    int ret = HRIL_ERR_GENERIC_FAILURE;
    if (pResponseInfo && pResponseInfo->result) {
        pLine = pResponseInfo->result;
        SkipATPrefix(&pLine);
        NextInt(&pLine, &ret);
    }

    if (ret == -1) {
        ret = HRIL_ERR_INVALID_RESPONSE;
    }
    return ret;
}

static int ResponseNetworkReport(int slotId, const ReqDataInfo *requestInfo, int err, struct ResponseAck *respDataAck)
{
    if (requestInfo == NULL) {
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (respDataAck == NULL) {
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo;
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 1);
    OnNetworkReport(slotId, reportInfo, (const uint8_t *)(respDataAck->respDataPointer), respDataAck->respDataLen);
    if (respDataAck->responseInfo != NULL) {
        FreeResponseInfo(respDataAck->responseInfo);
    }
    return err;
}

static int FindCommaCharNum(const char *srcStr)
{
    char *str = (char *)srcStr;
    if (str == NULL) {
        TELEPHONY_LOGE("srcStr parameter is null.");
        return -1;
    }
    if (*str == '\0') {
        return -1;
    }
    int charNum = 0;
    while (*str != '\0') {
        if (*str == ',') {
            charNum++;
        }
        str++;
    }
    return charNum;
}

static int FillRegArray(const char *s, const int *infoArray, int arraySize)
{
    if ((s == NULL) || (infoArray == NULL) || (arraySize < MAX_REG_INFO_ITEM)) {
        return HRIL_ERR_INVALID_RESPONSE;
    } else {
        char *str = (char *)s;
        int *info = (int *)infoArray;
        const int REG_STAT_POS = 1;
        const int REG_LAC_POS = 2;
        const int REG_CELL_ID_POS = 3;
        const int REG_ACT_POS = 4;
        if (NextInt(&str, &(info[REG_STAT_POS])) != 0) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        if (NextIntFromHex(&str, &(info[REG_LAC_POS])) != 0) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        if (NextIntFromHex(&str, &(info[REG_CELL_ID_POS])) != 0) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        if (NextInt(&str, &(info[REG_ACT_POS])) != 0) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        return HRIL_ERR_SUCCESS;
    }
}

static int ExtractRegStatus(const char *s, const HRilRegStatusInfo *hrilRegStateInfo, int commaNum)
{
    char *str = (char *)s;
    HRilRegStatusInfo *regStateInfo = (HRilRegStatusInfo *)hrilRegStateInfo;
    const int INIT_N_TYPE_VALUE = 2;
    const int REG_N_POS = 0;
    const int REG_STAT_POS = 1;
    const int REG_LAC_POS = 2;
    const int REG_CELL_ID_POS = 3;
    const int REG_ACT_POS = 4;
    const int REG_RESPONSE_COMMA_NUM = 4;
    const int REG_CELL_NOTIFY_COMMA_NUM = 3;
    int info[MAX_REG_INFO_ITEM] = {0, 0, 0, 0, 0};
    if (commaNum == REG_RESPONSE_COMMA_NUM) {
        if (NextInt(&str, &(info[REG_N_POS])) != 0) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        if (FillRegArray(str, info, MAX_REG_INFO_ITEM) != HRIL_ERR_SUCCESS) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
    } else if (commaNum == REG_CELL_NOTIFY_COMMA_NUM) {
        info[REG_N_POS] = INIT_N_TYPE_VALUE;
        if (FillRegArray(str, info, MAX_REG_INFO_ITEM) != HRIL_ERR_SUCCESS) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
    } else if (commaNum == 0) {
        info[REG_N_POS] = INIT_N_TYPE_VALUE;
        if (NextInt(&str, &(info[REG_STAT_POS])) != 0) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        if ((info[REG_STAT_POS] != NO_REG_MT_NO_SEARCH) && (info[REG_STAT_POS] != NO_REG_MT_SEARCHING) &&
            (info[REG_STAT_POS] != REG_MT_REJECTED) && (info[REG_STAT_POS] != REG_MT_UNKNOWN)) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    regStateInfo->notifyType = info[REG_N_POS];
    regStateInfo->regState = info[REG_STAT_POS];
    regStateInfo->lacCode = info[REG_LAC_POS];
    regStateInfo->cellId = info[REG_CELL_ID_POS];
    regStateInfo->actType = info[REG_ACT_POS];
    return HRIL_ERR_SUCCESS;
}

int ProcessRegStatus(const char *s, const HRilRegStatusInfo *hrilRegStateInfo)
{
    char *str = (char *)s;
    HRilRegStatusInfo *regStateInfo = (HRilRegStatusInfo *)hrilRegStateInfo;
    if ((str == NULL) || (regStateInfo == NULL)) {
        TELEPHONY_LOGE("ProcessRegStatus s or regStateInfo param is null");
        return HRIL_ERR_NULL_POINT;
    } else {
        memset_s(regStateInfo, sizeof(HRilRegStatusInfo), 0, sizeof(HRilRegStatusInfo));
        const int INIT_N_TYPE_VALUE = 2;
        regStateInfo->notifyType = INIT_N_TYPE_VALUE;
        int err = SkipATPrefix(&str);
        if (err < 0) {
            TELEPHONY_LOGE("skip failed: [%{public}s]", str);
            return HRIL_ERR_INVALID_RESPONSE;
        }
        int commaNum = FindCommaCharNum(str);
        if ((commaNum < 0) || (commaNum >= MAX_REG_INFO_ITEM)) {
            TELEPHONY_LOGE("ExtractRegStatus check failed commaNum: [%{public}d]", commaNum);
            return HRIL_ERR_INVALID_RESPONSE;
        }
        return ExtractRegStatus(str, regStateInfo, commaNum);
    }
}

static void ParseGetGsmSignalStrength(const char *line, HRilRssi *hrilRssi)
{
    char *lineStr = (char *)line;
    if (lineStr == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&lineStr, &hrilRssi->gsmRssi.rxlev);
    NextInt(&lineStr, &hrilRssi->gsmRssi.ber);
}

static void ParseGetLteSignalStrength(const char *line, HRilRssi *hrilRssi)
{
    char *lineStr = (char *)line;
    if (lineStr == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&lineStr, &hrilRssi->lteRssi.rxlev);
    NextInt(&lineStr, &hrilRssi->lteRssi.rsrq);
    NextInt(&lineStr, &hrilRssi->lteRssi.rsrp);
    NextInt(&lineStr, &hrilRssi->lteRssi.snr);
}

static void ParseGetWcdmaSignalStrength(const char *line, HRilRssi *hrilRssi)
{
    char *lineStr = (char *)line;
    if (lineStr == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&lineStr, &hrilRssi->wcdmaRssi.rxlev);
    NextInt(&lineStr, &hrilRssi->wcdmaRssi.ecio);
    NextInt(&lineStr, &hrilRssi->wcdmaRssi.rscp);
    NextInt(&lineStr, &hrilRssi->wcdmaRssi.ber);
}

int ProcessParamSignalStrength(const char *result, HRilRssi *hrilRssi)
{
    char *resultStr = (char *)result;
    char *c = NULL;
    int tmp = 0;
    int err = SkipATPrefix(&resultStr);
    if (err < 0) {
        TELEPHONY_LOGE("skip failed: [%{public}s]", resultStr);
        return err;
    }
    err = NextInt(&resultStr, &tmp);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }
    err = NextInt(&resultStr, &tmp);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }
    err = NextStr(&resultStr, &c);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }
    if (!strcmp(c, "GSM")) {
        ParseGetGsmSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "LTE")) {
        ParseGetLteSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "WCDMA")) {
        ParseGetWcdmaSignalStrength(resultStr, hrilRssi);
    }
    return 0;
}

int ProcessParamSignalStrengthNotify(const char *result, HRilRssi *hrilRssi)
{
    char *resultStr = (char *)result;
    char *c = NULL;
    int err = SkipATPrefix(&resultStr);
    if (err < 0) {
        TELEPHONY_LOGE("skip failed: %{public}s", resultStr);
        return err;
    }

    err = NextStr(&resultStr, &c);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }

    if (!strcmp(c, "GSM")) {
        ParseGetGsmSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "LTE")) {
        ParseGetLteSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "WCDMA")) {
        ParseGetWcdmaSignalStrength(resultStr, hrilRssi);
    }
    return 0;
}

static int CheckImsStatusValid(
    const int *imsStatusArray, HRilImsRegStatusInfo *imsRegStatusInfo, int realInfoNum, int expectInfoNum)
{
    if ((imsStatusArray == NULL) || (imsRegStatusInfo == NULL)) {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    const int IMS_NOTIFY_NUM = 2;
    const int IMS_EXT_INFO_NOTIFY_TYPE = 2;
    const int REG_INFO_MAX_VALUE = 1;
    const int IMS_STATUS_INDEX = 2;
    if (expectInfoNum == IMS_NOTIFY_NUM) {
        if ((realInfoNum == IMS_NOTIFY_NUM) && (imsStatusArray[0] <= REG_INFO_MAX_VALUE)) {
            imsRegStatusInfo->notifyType = IMS_EXT_INFO_NOTIFY_TYPE;
            imsRegStatusInfo->regInfo = imsStatusArray[0];
            imsRegStatusInfo->extInfo = imsStatusArray[1];
        } else if ((realInfoNum == 1) && (imsStatusArray[0] == 0)) {
            imsRegStatusInfo->notifyType = IMS_EXT_INFO_NOTIFY_TYPE;
            imsRegStatusInfo->regInfo = 0;
            imsRegStatusInfo->extInfo = 0;
        } else {
            return HRIL_ERR_INVALID_RESPONSE;
        }
    } else if (expectInfoNum == MAX_IMS_INFO_ITEM) {
        if (realInfoNum == IMS_NOTIFY_NUM) {
            if ((imsStatusArray[0] != IMS_EXT_INFO_NOTIFY_TYPE) || (imsStatusArray[1] != 0)) {
                return HRIL_ERR_INVALID_RESPONSE;
            } else {
                imsRegStatusInfo->notifyType = IMS_EXT_INFO_NOTIFY_TYPE;
                imsRegStatusInfo->regInfo = 0;
                imsRegStatusInfo->extInfo = 0;
            }
        } else {
            imsRegStatusInfo->notifyType = imsStatusArray[0];
            imsRegStatusInfo->regInfo = imsStatusArray[1];
            imsRegStatusInfo->extInfo = imsStatusArray[IMS_STATUS_INDEX];
        }
    }
    if (imsRegStatusInfo->notifyType != IMS_EXT_INFO_NOTIFY_TYPE) {
        TELEPHONY_LOGE("Ims Status n Value is invalid");
        return HRIL_ERR_INVALID_RESPONSE;
    }
    if ((imsRegStatusInfo->regInfo < 0) || (imsRegStatusInfo->regInfo > REG_INFO_MAX_VALUE)) {
        TELEPHONY_LOGE("Ims Status regInfo Value is invalid");
        return HRIL_ERR_INVALID_RESPONSE;
    }
    return HRIL_ERR_SUCCESS;
}

int ProcessImsRegStatus(const char *s, const HRilImsRegStatusInfo *imsRegStatusInfo, int expectInfoNum)
{
    char *str = (char *)s;
    if ((str == NULL) || (imsRegStatusInfo == NULL)) {
        TELEPHONY_LOGE("ProcessIMSRegStatus s or imsRegStatusInfo param is null");
        return HRIL_ERR_NULL_POINT;
    } else {
        int err = SkipATPrefix(&str);
        if (err < 0) {
            TELEPHONY_LOGE("skip failed: [%{public}s]", str);
            return HRIL_ERR_INVALID_RESPONSE;
        }
        int commaNum = FindCommaCharNum(str);
        if ((commaNum < 0) || (commaNum >= MAX_IMS_INFO_ITEM)) {
            TELEPHONY_LOGE("check failed commaNum: [%{public}d]", commaNum);
            return HRIL_ERR_INVALID_RESPONSE;
        }
        int info[MAX_IMS_INFO_ITEM] = {0, 0, 0};
        for (int i = 0; i <= commaNum; i++) {
            err = NextInt(&str, &(info[i]));
            if (err == -1) {
                info[i] = -1;
                return HRIL_ERR_INVALID_RESPONSE;
            }
        }
        if (CheckImsStatusValid(info, (HRilImsRegStatusInfo *)imsRegStatusInfo, commaNum + 1,
            expectInfoNum) != HRIL_ERR_SUCCESS) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        return HRIL_ERR_SUCCESS;
    }
}

void ReqGetImsRegStatus(const ReqDataInfo *requestInfo)
{
    int err = HRIL_ERR_SUCCESS;
    char *result = NULL;
    struct ReportInfo reportInfo;
    const long TIME_OUT = DEFAULT_TIMEOUT;
    ResponseInfo *responseInfo = NULL;

    int ret = SendCommandLock("AT+CIREG?", "+CIREG:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        return;
    }

    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("AT+CIREG send failed");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    HRilImsRegStatusInfo imsRegStatusInfo;
    ret = ProcessImsRegStatus(result, &imsRegStatusInfo, MAX_IMS_INFO_ITEM);
    if (ret != 0) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&imsRegStatusInfo, sizeof(HRilImsRegStatusInfo));
    FreeResponseInfo(responseInfo);
}

void ReqGetSignalStrength(const ReqDataInfo *requestInfo)
{
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    const long TIME_OUT = DEFAULT_TIMEOUT;
    const int REPORT_SIZE = 20;
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    int ret = SendCommandLock("AT^HCSQ?", "^HCSQ:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        return;
    }

    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("AT^HCSQ send failed");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    HRilRssi hrilRssi = {0};
    if (strlen(result) > REPORT_SIZE) {
        ret = ProcessParamSignalStrength(result, &hrilRssi);
    } else {
        ret = ProcessParamSignalStrengthNotify(result, &hrilRssi);
    }
    if (ret != 0) {
        err = HRIL_ERR_GENERIC_FAILURE;
        TELEPHONY_LOGE("SignalStrength is null!");
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&hrilRssi, sizeof(HRilRssi));
    FreeResponseInfo(responseInfo);
}

void ReqGetCsRegStatus(const ReqDataInfo *requestInfo)
{
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    const long TIME_OUT = DEFAULT_TIMEOUT;

    int ret = SendCommandLock("AT+CREG?", "+CREG:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    HRilRegStatusInfo regStatusInfo;
    ret = ProcessRegStatus(result, &regStatusInfo);
    if (ret != 0) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&regStatusInfo, sizeof(HRilRegStatusInfo));
    FreeResponseInfo(responseInfo);
}

void ReqGetPsRegStatus(const ReqDataInfo *requestInfo)
{
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    const long TIME_OUT = DEFAULT_TIMEOUT;

    int ret = SendCommandLock("AT+CGREG?", "+CGREG:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    HRilRegStatusInfo regStatusInfo;
    ret = ProcessRegStatus(result, &regStatusInfo);
    if (ret != 0) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&regStatusInfo, sizeof(HRilRegStatusInfo));
    FreeResponseInfo(responseInfo);
}

void ReqGetOperatorInfo(const ReqDataInfo *requestInfo)
{
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    const int numCount = 3;
    char *response[numCount] = {"", "", ""};
    const long TIME_OUT = DEFAULT_TIMEOUT;

    int ret =
        SendCommandLock("AT+COPS=3,2;+COPS?;+COPS=3,1;+COPS?;+COPS=3,0;+COPS?", "+COPS:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    Line *pLine = responseInfo->head;
    for (int i = 0; pLine != NULL; i++, pLine = pLine->next) {
        int skip = 0;
        result = pLine->data;
        SkipATPrefix(&result);
        ret = NextInt(&result, &skip);
        if (ret == -1) {
            TELEPHONY_LOGE("read failed");
            break;
        }
        ret = NextInt(&result, &skip);
        if (ret == -1) {
            TELEPHONY_LOGE("read failed");
            break;
        }
        ret = NextStr(&result, &response[i]);
        if (ret == -1) {
            response[i] = "";
            TELEPHONY_LOGE("read failed");
            break;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)response, numCount);
    FreeResponseInfo(responseInfo);
}

static int MoveLeftBracket(char **pStr)
{
    if (*pStr == NULL) {
        return -1;
    }
    *pStr = strchr(*pStr, '(');
    if (*pStr == NULL) {
        return -1;
    }
    (*pStr)++;
    return 0;
}

static int MoveRightBracket(char **pStr)
{
    if (*pStr == NULL) {
        return -1;
    }
    *pStr = strchr(*pStr, ')');
    if (*pStr == NULL) {
        return -1;
    }
    (*pStr)++;
    return 0;
}

void GetNetworkSearchInformationPause(void)
{
    pthread_mutex_lock(&g_networkSearchInformationMutex);
    g_reportInfoForOperListToUse.error = HRIL_ERR_NETWORK_SEARCHING_INTERRUPTED;
    OnNetworkReport(HRIL_SIM_SLOT_1, g_reportInfoForOperListToUse, NULL, 0);
    SetAtPauseFlag(false);
    if (g_reportInfoForOperListToUse.requestInfo != NULL) {
        free(g_reportInfoForOperListToUse.requestInfo);
        g_reportInfoForOperListToUse.requestInfo = NULL;
    }
    pthread_mutex_unlock(&g_networkSearchInformationMutex);
}

void PerformTimeOut(int sigFlag)
{
    if (SIGALRM == sigFlag) {
        pthread_mutex_lock(&g_networkSearchInformationMutex);
        bool sendFlag = GetAtPauseFlag();
        if (sendFlag) {
            g_reportInfoForOperListToUse.error = HRIL_ERR_NETWORK_SEARCH_TIMEOUT;
            OnNetworkReport(HRIL_SIM_SLOT_1, g_reportInfoForOperListToUse, NULL, 0);
            SetAtPauseFlag(false);
            if (g_reportInfoForOperListToUse.requestInfo != NULL) {
                free(g_reportInfoForOperListToUse.requestInfo);
                g_reportInfoForOperListToUse.requestInfo = NULL;
            }
        }
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
    }
    return;
}

void ReqGetNetworkSearchInformation(const ReqDataInfo *requestInfo)
{
    const long TIME_OUT = 1000;
    ResponseInfo *responseInfo = NULL;
    const int SECOND = 120;
    pthread_mutex_lock(&g_networkSearchInformationMutex);
    alarm(0);
    if (g_reportInfoForOperListToUse.requestInfo != NULL) {
        g_reportInfoForOperListToUse.error = HRIL_ERR_NETWORK_SEARCHING;
        OnNetworkReport(HRIL_SIM_SLOT_1, g_reportInfoForOperListToUse, NULL, 0);
        free(g_reportInfoForOperListToUse.requestInfo);
        g_reportInfoForOperListToUse.requestInfo = NULL;
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
        return;
    }
    if (signal(SIGALRM, PerformTimeOut) == SIG_ERR) {
        TELEPHONY_LOGE("ReqGetNetworkSearchInformation signal PerformTimeOut is SIG_ERR!");
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
        return;
    }
    int ret = SendCommandNetWorksLock("AT+COPS=?", "+COPS:", TIME_OUT, &responseInfo);

    g_reportInfoForOperListToUse = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    if ((ret != 0 && ret != AT_ERR_WAITING) || (responseInfo != NULL && !responseInfo->success)) {
        TELEPHONY_LOGE("send AT CMD failed!");
        SetAtPauseFlag(false);
        g_reportInfoForOperListToUse.error = GetResponseErrorCode(responseInfo);
        OnNetworkReport(HRIL_SIM_SLOT_1, g_reportInfoForOperListToUse, NULL, 1);
        if (responseInfo != NULL) {
            FreeResponseInfo(responseInfo);
        }
        if (g_reportInfoForOperListToUse.requestInfo != NULL) {
            free(g_reportInfoForOperListToUse.requestInfo);
            g_reportInfoForOperListToUse.requestInfo = NULL;
        }
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
        return;
    }
    alarm(SECOND);
    if (responseInfo != NULL) {
        FreeResponseInfo(responseInfo);
    }
    pthread_mutex_unlock(&g_networkSearchInformationMutex);
    SetWatchFunction(GetNetworkSearchInformationPause);
}

static int ParseOperInfoItem(
    char **lineInfo, char **const longNameInfo, char **const shortNameInfo, char **const numericInfo)
{
    char *data = *lineInfo;
    char **longNameInfoStr = (char **)longNameInfo;
    char **shortNameInfoStr = (char **)shortNameInfo;
    char **numericInfoStr = (char **)numericInfo;
    int ret = NextStr(&data, longNameInfoStr);
    if (ret < 0) {
        return -1;
    }

    ret = NextStr(&data, shortNameInfoStr);
    if (ret < 0) {
        return -1;
    }

    ret = NextStr(&data, numericInfoStr);
    if (ret < 0) {
        return -1;
    }
    *lineInfo = data;
    return 0;
}

int ParseOperListInfo(const char *lineInfo, int count, AvailableOperInfo *pOperInfo, AvailableOperInfo **ppOperInfo)
{
    int state = 0;
    int rat = 0;
    int operCount = 0;
    char *line = (char *)lineInfo;
    int item = count;
    for (int i = 0; i < item && operCount < item; i++) {
        int ret = MoveLeftBracket(&line);
        if (ret < 0) {
            break;
        }
        ret = NextInt(&line, &state);
        if (ret < 0) {
            break;
        }
        char *longName = NULL;
        char *shortName = NULL;
        char *numeric = NULL;
        ret = ParseOperInfoItem(&line, &longName, &shortName, &numeric);
        if (ret < 0) {
            break;
        }
        ret = NextInt(&line, &rat);
        if (ret < 0) {
            break;
        }
        ret = MoveRightBracket(&line);
        if (ret < 0) {
            break;
        }
        if (state > 0) {
            pOperInfo[operCount].status = state;
            pOperInfo[operCount].longName = longName;
            pOperInfo[operCount].numeric = numeric;
            pOperInfo[operCount].shortName = shortName;
            pOperInfo[operCount].rat = rat;
            operCount++;
        }
    }
    return operCount;
}

int ParseListItemNum(const char *list)
{
    int item = 0;
    if (list == NULL) {
    } else {
        char *line = (char *)list;
        while (*line != '\0') {
            if (*line == ')') {
                item++;
            }
            line++;
        }
    }
    return item;
}

static void DealNetworkSearchInformation(int operCount, AvailableOperInfo **ppOperInfo, AvailableOperInfo *pOperInfo)
{
    if (operCount == 0) {
        pthread_mutex_lock(&g_networkSearchInformationMutex);
        SetAtPauseFlag(false);
        alarm(0);
        if (g_reportInfoForOperListToUse.requestInfo != NULL) {
            g_reportInfoForOperListToUse.error = HRIL_ERR_INVALID_RESPONSE;
            OnNetworkReport(HRIL_SIM_SLOT_1, g_reportInfoForOperListToUse, NULL, 0);
            free(g_reportInfoForOperListToUse.requestInfo);
            g_reportInfoForOperListToUse.requestInfo = NULL;
        }
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
    } else {
        pthread_mutex_lock(&g_networkSearchInformationMutex);
        SetAtPauseFlag(false);
        alarm(0);
        if (g_reportInfoForOperListToUse.requestInfo != NULL) {
            OnNetworkReport(HRIL_SIM_SLOT_1, g_reportInfoForOperListToUse, (const uint8_t *)ppOperInfo, operCount);
            free(g_reportInfoForOperListToUse.requestInfo);
            g_reportInfoForOperListToUse.requestInfo = NULL;
        }
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
    }
    if (ppOperInfo == NULL) {
        free(ppOperInfo);
    }
    if (pOperInfo == NULL) {
        free(pOperInfo);
    }
}

static int ErrorHandling(void)
{
    TELEPHONY_LOGE("This command resolution is not supported.");
    return HRIL_ERR_GENERIC_FAILURE;
}

static int ParseCellInfoGsm(const char *str, CellInfo *ci)
{
    char *pStr = (char *)str;
    char *pat = NULL;
    memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    int ret = SkipATPrefix(&pStr);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextStr(&pStr, &pat);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.gsm.band);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.gsm.arfcn);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.gsm.bsic);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextIntFromHex(&pStr, &ci->ServiceCellParas.gsm.cellId);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextIntFromHex(&pStr, &ci->ServiceCellParas.gsm.lac);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.gsm.rxlev);
    if (ret < 0) {
        return ErrorHandling();
    }

    ci->ratType = NETWORK_TYPE_GSM;
    return HRIL_ERR_SUCCESS;
}

static int ParseCellInfoLte(const char *str, CellInfo *ci)
{
    char *pStr = (char *)str;
    char *pat = NULL;
    memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    int ret = SkipATPrefix(&pStr);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextStr(&pStr, &pat);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.lte.arfcn);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextIntFromHex(&pStr, &ci->ServiceCellParas.lte.pci);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.lte.rsrp);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.lte.rsrq);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.lte.rxlev);
    if (ret < 0) {
        return ErrorHandling();
    }
    ci->ratType = NETWORK_TYPE_LTE;
    return HRIL_ERR_SUCCESS;
}

static int ParseCellInfoWcdma(const char *str, CellInfo *ci)
{
    char *pStr = (char *)str;
    char *pat = NULL;
    memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    int ret = SkipATPrefix(&pStr);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextStr(&pStr, &pat);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.wcdma.arfcn);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.wcdma.psc);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.wcdma.rscp);
    if (ret < 0) {
        return ErrorHandling();
    }
    ret = NextInt(&pStr, &ci->ServiceCellParas.wcdma.ecno);
    if (ret < 0) {
        return ErrorHandling();
    }
    ci->ratType = NETWORK_TYPE_WCDMA;
    return HRIL_ERR_SUCCESS;
}

static int ParseCellInfos(const char *str, const CellInfo *cellInfo)
{
    int ret = -1;
    CellInfo *ci = (CellInfo *)cellInfo;
    char *pStr = (char *)str;
    if (pStr == NULL) {
        TELEPHONY_LOGE("pStr is null.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (ReportStrWith(pStr, "^MONNC: GSM")) {
        ret = ParseCellInfoGsm(pStr, ci);
        if (ret != 0) {
            TELEPHONY_LOGE("parse gsm command failed!");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    } else if (ReportStrWith(pStr, "^MONNC: WCDMA")) {
        ret = ParseCellInfoWcdma(pStr, ci);
        if (ret != 0) {
            TELEPHONY_LOGE("parse wcdma command failed!");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    } else if (ReportStrWith(pStr, "^MONNC: LTE")) {
        ret = ParseCellInfoLte(pStr, ci);
        if (ret != 0) {
            TELEPHONY_LOGE("parse lte command failed!");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    } else if (ReportStrWith(pStr, "^MONNC: NONE")) {
        memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    } else {
        TELEPHONY_LOGE("%{public}s This command resolution not supported.", pStr);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    return HRIL_ERR_SUCCESS;
}

void ReqGetCellInfoList(const ReqDataInfo *requestInfo)
{
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int err = HRIL_ERR_SUCCESS;
    int countCellInfo = 0;
    int index = 0;
    ResponseInfo *responseInfo = NULL;
    Line *pLine = NULL;
    CellInfo *cellInfo = NULL;
    CellInfoList cellInfoList;
    int ret = SendCommandLock("AT^MONNC", "^MONNC:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    respDataAck.respDataPointer = (uint8_t *)&cellInfoList;
    respDataAck.respDataLen = sizeof(CellInfoList);
    if (ret != 0 || responseInfo->success == 0) {
        TELEPHONY_LOGE("send AT CMD failed!");
        err = (responseInfo->success == 0) ? HRIL_ERR_CMD_SEND_FAILURE : HRIL_ERR_GENERIC_FAILURE;
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }
    for (countCellInfo = 0, pLine = responseInfo->head; pLine != NULL; pLine = pLine->next) {
        countCellInfo++;
    }
    if (countCellInfo == 0) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    cellInfo = (CellInfo *)calloc(countCellInfo, sizeof(CellInfo));
    if (cellInfo == NULL) {
        TELEPHONY_LOGE("cellInfoList alloc failed!");
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_MEMORY_FULL, &respDataAck);
        return;
    }
    for (pLine = responseInfo->head; pLine != NULL; pLine = pLine->next) {
        ret = ParseCellInfos(pLine->data, &cellInfo[index]);
        if (ret != 0) {
            continue;
        }
        index++;
    }

    cellInfoList.itemNum = index;
    cellInfoList.cellNearbyInfo = cellInfo;
    ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
    free(cellInfo);
    return;
}

static void ParseGetGsmCellInfoLine(char *line, CurrentCellInfoVendor *response)
{
    NextInt(&line, &response->ServiceCellParas.gsm.band);
    NextInt(&line, &response->ServiceCellParas.gsm.arfcn);
    NextInt(&line, &response->ServiceCellParas.gsm.bsic);
    NextIntFromHex(&line, &response->ServiceCellParas.gsm.cellId);
    NextIntFromHex(&line, &response->ServiceCellParas.gsm.lac);
    NextInt(&line, &response->ServiceCellParas.gsm.rxlev);
    NextInt(&line, &response->ServiceCellParas.gsm.rxQuality);
    NextInt(&line, &response->ServiceCellParas.gsm.ta);
}

static void ParseGetLteCellInfoLine(char *line, CurrentCellInfoVendor *response)
{
    NextInt(&line, &response->ServiceCellParas.lte.arfcn);
    NextIntFromHex(&line, &response->ServiceCellParas.lte.cellId);
    NextIntFromHex(&line, &response->ServiceCellParas.lte.pci);
    NextIntFromHex(&line, &response->ServiceCellParas.lte.tac);
    NextInt(&line, &response->ServiceCellParas.lte.rsrp);
    NextInt(&line, &response->ServiceCellParas.lte.rsrq);
    NextInt(&line, &response->ServiceCellParas.lte.rssi);
}

static void ParseGetWcdmaCellInfoLine(char *line, CurrentCellInfoVendor *response)
{
    NextInt(&line, &response->ServiceCellParas.wcdma.arfcn);
    NextInt(&line, &response->ServiceCellParas.wcdma.psc);
    NextIntFromHex(&line, &response->ServiceCellParas.wcdma.cellId);
    NextIntFromHex(&line, &response->ServiceCellParas.wcdma.lac);
    NextInt(&line, &response->ServiceCellParas.wcdma.rscp);
    NextInt(&line, &response->ServiceCellParas.wcdma.rxlev);
    NextInt(&line, &response->ServiceCellParas.wcdma.ecno);
    NextInt(&line, &response->ServiceCellParas.wcdma.drx);
    NextInt(&line, &response->ServiceCellParas.wcdma.ura);
}

static int ParseGetCurrentCellInfoResponseLine(char *line, CurrentCellInfoVendor *response)
{
    char *c = NULL;
    int err = SkipATPrefix(&line);
    if (err < 0) {
        return err;
    }
    err = NextStr(&line, &c);
    if (err < 0) {
        return err;
    }
    if (!strcmp(c, "GSM")) {
        response->ratType = NETWORK_TYPE_GSM;
    } else if (!strcmp(c, "LTE")) {
        response->ratType = NETWORK_TYPE_LTE;
    } else if (!strcmp(c, "WCDMA")) {
        response->ratType = NETWORK_TYPE_WCDMA;
    } else if (!strcmp(c, "NONE")) {
        memset_s(response, sizeof(CurrentCellInfoVendor), 0, sizeof(CurrentCellInfoVendor));
        response->ratType = NETWORK_TYPE_UNKNOWN;
        return 0;
    } else {
        return -1;
    }
    err = NextInt(&line, &response->mcc);
    if (err < 0) {
        return err;
    }
    err = NextInt(&line, &response->mnc);
    if (err < 0) {
        return err;
    }
    switch (response->ratType) {
        case NETWORK_TYPE_GSM:
            ParseGetGsmCellInfoLine(line, response);
            break;
        case NETWORK_TYPE_LTE:
            ParseGetLteCellInfoLine(line, response);
            break;
        case NETWORK_TYPE_WCDMA:
            ParseGetWcdmaCellInfoLine(line, response);
            break;
        default:
            TELEPHONY_LOGE("ParseGetCellInfoResponseLine is not support cell, line = %{public}s", line);
            break;
    }
    return 0;
}

void ReqGetCurrentCellInfo(const ReqDataInfo *requestInfo)
{
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int err = HRIL_RESPONSE;
    CurrentCellInfoVendor cciv;
    ResponseInfo *responseInfo = NULL;
    int ret = SendCommandLock("AT^MONSC", "^MONSC:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    respDataAck.respDataPointer = (uint8_t *)&cciv;
    respDataAck.respDataLen = sizeof(CurrentCellInfoVendor);
    if (ret != 0 || responseInfo->success == 0) {
        TELEPHONY_LOGE("send AT CMD failed!");
        err = (responseInfo->success == 0) ? HRIL_ERR_CMD_SEND_FAILURE : HRIL_ERR_GENERIC_FAILURE;
        memset_s(&cciv, sizeof(CurrentCellInfoVendor), 0, sizeof(CurrentCellInfoVendor));
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }

    char *line = responseInfo->head->data;
    if (line == NULL) {
        err = HRIL_ERR_INVALID_RESPONSE;
        memset_s(&cciv, sizeof(CurrentCellInfoVendor), 0, sizeof(CurrentCellInfoVendor));
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }
    if (ParseGetCurrentCellInfoResponseLine(line, &cciv) != 0) {
        err = HRIL_ERR_INVALID_RESPONSE;
        memset_s(&cciv, sizeof(CurrentCellInfoVendor), 0, sizeof(CurrentCellInfoVendor));
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }
    ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
    return;
}

static int OperListErrorHandler(AvailableOperInfo **ppOperInfo, AvailableOperInfo *pOperInfo)
{
    TELEPHONY_LOGE("ReqGetNetworkSearchInformation Failed");
    DealNetworkSearchInformation(0, ppOperInfo, pOperInfo);
    return HRIL_ERR_GENERIC_FAILURE;
}

int ProcessOperListToUse(const char *list)
{
    const int UNUSED_ITEM_COUNT = 2;
    AvailableOperInfo **ppOperInfo = NULL;
    AvailableOperInfo *pOperInfo = NULL;
    SetAtPauseFlag(false);
    alarm(0);
    int item = ParseListItemNum(list);
    if (item <= UNUSED_ITEM_COUNT) {
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }
    char *line = (char *)list;
    int ret = SkipATPrefix(&line);
    if (ret < 0) {
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }

    item = item - UNUSED_ITEM_COUNT;
    ppOperInfo = (AvailableOperInfo **)malloc(item * sizeof(size_t));
    if (!ppOperInfo) {
        TELEPHONY_LOGE("ppOperInfo malloc fail");
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }

    pOperInfo = (AvailableOperInfo *)malloc(item * sizeof(AvailableOperInfo));
    if (!pOperInfo) {
        TELEPHONY_LOGE("pOperInfo malloc fail");
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }
    (void)memset_s(pOperInfo, item * sizeof(AvailableOperInfo), 0, item * sizeof(AvailableOperInfo));
    for (int j = 0; j < item; j++) {
        ppOperInfo[j] = &(pOperInfo[j]);
    }

    int operCount = ParseOperListInfo(line, item, pOperInfo, ppOperInfo);
    if (operCount != 0) {
        DealNetworkSearchInformation(operCount, ppOperInfo, pOperInfo);
    } else {
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }
    return HRIL_ERR_SUCCESS;
}

static bool PrepareSetNetworkSelectionMode(char *cmd, const HRilSetNetworkModeInfo *setModeInfo)
{
    bool ret = true;
    if (setModeInfo->selectMode == 0) {
        (void)sprintf_s(cmd, MAX_CMD_LENGTH, "%s", "AT+COPS=0");
    } else if (setModeInfo->selectMode == 1) {
        if (setModeInfo->oper == NULL) {
            ret = false;
        } else {
            (void)sprintf_s(cmd, MAX_CMD_LENGTH, "AT+COPS=1,2,%s", setModeInfo->oper);
        }
    } else {
        ret = false;
    }
    return ret;
}

void ReqSetNetworkSelectionMode(const ReqDataInfo *requestInfo, const HRilSetNetworkModeInfo *data)
{
    ResponseInfo *responseInfo = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char *cmdBuff = cmd;

    struct ReportInfo reportInfo;
    HRilSetNetworkModeInfo *setModeInfo = (HRilSetNetworkModeInfo *)data;
    if (setModeInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode HRIL_ERR_NULL_POINT");
        return;
    }
    if (!PrepareSetNetworkSelectionMode(cmdBuff, setModeInfo)) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode HRIL_ERR_INVALID_PARAMETER");
        return;
    }
    int err = SendCommandLock(cmd, NULL, 0, &responseInfo);

    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode responseInfo == NULL");
        return;
    }
    if (err != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode HRIL_ERR_GENERIC_FAILURE");
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 1);
    OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqGetNetworkSelectionMode(const ReqDataInfo *requestInfo)
{
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    int state = 0;
    struct ReportInfo reportInfo;
    int ret = SendCommandLock("AT+COPS?", "+COPS:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }
    if (responseInfo->head == NULL) {
        err = HRIL_ERR_NULL_POINT;
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }
    Line *pLine = responseInfo->head;
    char *line = pLine->data;
    ret = SkipATPrefix(&line);
    if (ret < 0) {
        err = HRIL_ERR_INVALID_RESPONSE;
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }
    ret = NextInt(&line, &state);
    if (ret < 0) {
        err = HRIL_ERR_INVALID_RESPONSE;
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }

    respDataAck.respDataPointer = (uint8_t *)&state;
    respDataAck.respDataLen = sizeof(int);
    ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_SUCCESS, &respDataAck);

    NotifyNetWorkTime();
    return;
}

static char *PrepareCommandByNetworkType(HRilPreferredNetworkType setNetTypeInfo)
{
    int32_t ret = 0;
    char *cmd = (char *)calloc(1, MAX_CMD_LENGTH);
    if (cmd != NULL) {
        if (setNetTypeInfo == HRIL_NETWORK_AUTO) {
            ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"00\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
            if (ret < 0) {
                TELEPHONY_LOGI("GenerateCommand is failed!");
            }
        } else if (setNetTypeInfo == HRIL_NETWORK_GSM) {
            ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"01\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
            if (ret < 0) {
                TELEPHONY_LOGI("GenerateCommand is failed!");
            }
        } else if (setNetTypeInfo == HRIL_NETWORK_WCDMA) {
            ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"02\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
            if (ret < 0) {
                TELEPHONY_LOGI("GenerateCommand is failed!");
            }
        } else if (setNetTypeInfo == HRIL_NETWORK_LTE) {
            ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"03\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
            if (ret < 0) {
                TELEPHONY_LOGI("GenerateCommand is failed!");
            }
        } else if (setNetTypeInfo == HRIL_NETWORK_LTE_WCDMA_GSM) {
            ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "%s",
                "AT^SYSCFGEX=\"030201\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
            if (ret < 0) {
                TELEPHONY_LOGI("GenerateCommand is failed!");
            }
        } else if (setNetTypeInfo == HRIL_NETWORK_WCDMA_GSM) {
            ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"0201\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
            if (ret < 0) {
                TELEPHONY_LOGI("GenerateCommand is failed!");
            }
        } else if (setNetTypeInfo == HRIL_NETWORK_LTE_WCDMA) {
            ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"0302\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
            if (ret < 0) {
                TELEPHONY_LOGI("GenerateCommand is failed!");
            }
        } else {
            free(cmd);
            cmd = NULL;
            TELEPHONY_LOGE("PrepareCommand HRIL_ERR_INVALID_PARAMETER  setNetTypeInfo = %{public}d", setNetTypeInfo);
        }
    }
    return cmd;
}

void ReqSetPreferredNetwork(const ReqDataInfo *requestInfo, const int32_t *data)
{
    ResponseInfo *responseInfo = NULL;
    struct ReportInfo reportInfo;
    if (data == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        TELEPHONY_LOGE("RequestReqSetPreferredNetwork  HRIL_ERR_INVALID_PARAMETER data NULL");
        return;
    }
    int setNetTypeInfo = *(int *)data;
    char *cmd = PrepareCommandByNetworkType(setNetTypeInfo);
    if (cmd == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        return;
    }
    int err = SendCommandLock(cmd, NULL, 0, &responseInfo);
    if (cmd != NULL) {
        free(cmd);
        cmd = NULL;
    }
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetPreferredNetwork responseInfo == NULL");
        return;
    }
    if (err != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetPreferredNetwork HRIL_ERR_GENERIC_FAILURE");
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 1);
    OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

static int ParseNetTypeStr(const char *netType)
{
    if (netType == NULL) {
        return -1;
    }
    if (strcmp(netType, AUTO_TYPE) == 0) {
        return HRIL_NETWORK_AUTO;
    } else if (strcmp(netType, GSM_TYPE) == 0) {
        return HRIL_NETWORK_GSM;
    } else if (strcmp(netType, WCDMA_TYPE) == 0) {
        return HRIL_NETWORK_WCDMA;
    } else if (strcmp(netType, LTE_TYPE) == 0) {
        return HRIL_NETWORK_LTE;
    } else if (strcmp(netType, LTE_WCDMA_TYPE) == 0) {
        return HRIL_NETWORK_LTE_WCDMA;
    } else if (strcmp(netType, LTE_WCDMA_GSM_TYPE) == 0) {
        return HRIL_NETWORK_LTE_WCDMA_GSM;
    } else if (strcmp(netType, WCDMA_GSM_TYPE) == 0) {
        return HRIL_NETWORK_WCDMA_GSM;
    } else {
        return -1;
    }
}

void ReqGetPreferredNetwork(const ReqDataInfo *requestInfo)
{
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    char *netTypeStr = "";
    int ret = SendCommandLock("AT^SYSCFGEX?", "^SYSCFGEX:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    if (responseInfo == NULL) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_NULL_POINT, &respDataAck);
        return;
    }
    respDataAck.responseInfo = responseInfo;
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }
    if (responseInfo->head == NULL) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    Line *pLine = responseInfo->head;
    char *line = pLine->data;
    ret = SkipATPrefix(&line);
    if (ret < 0) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    ret = NextStr(&line, &netTypeStr);
    if (ret < 0) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    int netType = ParseNetTypeStr(netTypeStr);
    respDataAck.respDataPointer = (uint8_t *)&netType;
    respDataAck.respDataLen = sizeof(int);
    ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_SUCCESS, &respDataAck);
}

void ReqGetImei(const ReqDataInfo *requestInfo)
{
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    int ret = SendCommandLock("AT+CGSN", NULL, TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    if (responseInfo == NULL) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_NULL_POINT, &respDataAck);
        return;
    }
    respDataAck.responseInfo = responseInfo;
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }
    if (responseInfo->head == NULL) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    char *imeiSn = responseInfo->head->data;
    if ((imeiSn == NULL) || (strlen(imeiSn) == 0)) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    respDataAck.respDataPointer = (uint8_t *)imeiSn;
    respDataAck.respDataLen = strlen(imeiSn);
    ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_SUCCESS, &respDataAck);
}

void ReqSetPsAttachStatus(const ReqDataInfo *requestInfo, const int32_t *data)
{
    const long TIME_OUT = DEFAULT_TIMEOUT;
    ResponseInfo *responseInfo = NULL;
    struct ReportInfo reportInfo;
    if (data == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        TELEPHONY_LOGE("ReqSetPsAttachStatus  HRIL_ERR_INVALID_PARAMETER data NULL");
        return;
    }
    int setPsAttachStatus = *(int *)data;
    char *cmd = NULL;
    if (setPsAttachStatus == 1) {
        cmd = "AT+CGATT=1";
    } else if (setPsAttachStatus == 0) {
        cmd = "AT+CGATT=0";
    } else {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        return;
    }
    int err = SendCommandLock(cmd, NULL, TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 1);
        TELEPHONY_LOGE("ReqSetPsAttachStatus responseInfo == NULL");
        return;
    }
    if (err != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("ReqSetPsAttachStatus errcode = %{public}d", err);
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

static int ParsePsAttachStatusStr(const Line *pLine)
{
    const char *psAttachedStr = "1";
    const char *psDetachedStr = "0";

    if (pLine == NULL) {
        return -1;
    }
    char *line = pLine->data;
    int ret = SkipATPrefix(&line);
    if (ret < 0) {
        return -1;
    }
    char *psAttachStatusStr = NULL;
    ret = NextStr(&line, &psAttachStatusStr);
    if (ret < 0) {
        return -1;
    }
    if (psAttachStatusStr == NULL) {
        return -1;
    }
    int psAttachStatus = -1;
    if (strcmp(psAttachStatusStr, psAttachedStr) == 0) {
        psAttachStatus = 1;
    } else if (strcmp(psAttachStatusStr, psDetachedStr) == 0) {
        psAttachStatus = 0;
    } else {
    }
    return psAttachStatus;
}

void ReqGetPsAttachStatus(const ReqDataInfo *requestInfo)
{
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    int ret = SendCommandLock("AT+CGATT?", "+CGATT:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    if (responseInfo == NULL) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_NULL_POINT, &respDataAck);
        return;
    }
    respDataAck.responseInfo = responseInfo;
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, err, &respDataAck);
        return;
    }
    int psAttachStatus = ParsePsAttachStatusStr(responseInfo->head);
    if (psAttachStatus < 0) {
        ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    respDataAck.respDataPointer = (uint8_t *)&psAttachStatus;
    respDataAck.respDataLen = sizeof(int);
    ResponseNetworkReport(HRIL_SIM_SLOT_1, requestInfo, HRIL_ERR_SUCCESS, &respDataAck);
}

void NotifyNetWorkTime(void)
{
    const long TIME_OUT = DEFAULT_TIMEOUT;
    ResponseInfo *responseInfo = NULL;
    const int TIME_VALUE_LEN = 30;
    const size_t TIME_VALUE_OFFSET = 2;
    char timeStr[TIME_VALUE_LEN] = {0};
    struct ReportInfo reportInfo = {0};

    int ret = SendCommandLock("AT+CCLK?", "+CCLK:", TIME_OUT, &responseInfo);
    if (ret != 0 || responseInfo->success == 0) {
        TELEPHONY_LOGE("send AT CMD failed!");
        return;
    }
    if (responseInfo->head == NULL) {
        TELEPHONY_LOGE("no data!");
        return;
    }
    Line *pLine = responseInfo->head;
    if (pLine->data == NULL) {
        TELEPHONY_LOGE("no data!");
        return;
    }
    char *line = pLine->data;
    ret = SkipATPrefix(&line);
    if (ret < 0) {
        TELEPHONY_LOGE("format error!");
        return;
    }
    size_t len = strlen(line) - 1;
    if (len <= TIME_VALUE_OFFSET) {
        TELEPHONY_LOGE("invalid len!");
        return;
    }
    for (size_t i = TIME_VALUE_OFFSET; i < len; i++) {
        timeStr[i - TIME_VALUE_OFFSET] = *(line + i);
    }
    reportInfo.notifyId = HNOTI_NETWORK_TIME_UPDATED;
    reportInfo.type = HRIL_NOTIFICATION;
    reportInfo.error = HRIL_ERR_SUCCESS;
    OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)timeStr, TIME_VALUE_LEN);
    FreeResponseInfo(responseInfo);
}