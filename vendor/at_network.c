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

#include "vendor_adapter.h"
#include "vendor_report.h"
#include "vendor_util.h"

static struct ReportInfo g_reportInfoForOperListToUse;
static pthread_mutex_t g_networkSearchInformationMutex = PTHREAD_MUTEX_INITIALIZER;

int GetResponseErrorCode(ResponseInfo *pResponseInfo)
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
    TELEPHONY_LOGD("networks response error code: %{public}d", ret);
    return ret;
}

int ProcessRegStatus(char *s, char **response, int count)
{
    int ret = HRIL_ERR_SUCCESS;
    if (s == NULL) {
        TELEPHONY_LOGE("ProcessRegStatus result is null");
        return HRIL_ERR_NULL_POINT;
    } else {
        TELEPHONY_LOGD("result: %{public}s", s);
        SkipATPrefix(&s);
        for (int i = 0; i < count; i++) {
            ret = NextStr(&s, &response[i]);
            TELEPHONY_LOGD("result[%{public}d]: %{public}s", i, response[i]);
            if (ret == -1) {
                response[i] = 0;
                break;
            }
        }
        return ret;
    }
}

static void ParseGetGsmSignalStrength(char *line, HRilRssi *hrilRssi)
{
    if (line == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&line, &hrilRssi->gwRssi.rxlev);
    NextInt(&line, &hrilRssi->gwRssi.ber);
}

static void ParseGetLteSignalStrength(char *line, HRilRssi *hrilRssi)
{
    if (line == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&line, &hrilRssi->lte.rxlev);
    NextInt(&line, &hrilRssi->lte.rsrq);
    NextInt(&line, &hrilRssi->lte.rsrp);
    NextInt(&line, &hrilRssi->lte.snr);
}

static void ParseGetWcdmaSignalStrength(char *line, HRilRssi *hrilRssi)
{
    if (line == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&line, &hrilRssi->wcdma.rxlev);
    NextInt(&line, &hrilRssi->wcdma.ecio);
    NextInt(&line, &hrilRssi->wcdma.rscp);
    NextInt(&line, &hrilRssi->wcdma.ber);
}

int ProcessParamSignalStrength(char *result, HRilRssi *hrilRssi)
{
    int err;
    char *c = NULL;
    err = SkipATPrefix(&result);
    int tmp;

    if (err < 0) {
        TELEPHONY_LOGE("skip failed: [%{public}s]", result);
        return err;
    }
    TELEPHONY_LOGD("ProcessParamSignalStrength  enter -->, result %{public}s", result);

    err = NextInt(&result, &tmp);
    TELEPHONY_LOGD("ProcessParamSignalStrength  enter -->, result %{public}s", result);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }
    err = NextInt(&result, &tmp);
    TELEPHONY_LOGD("ProcessParamSignalStrength  enter -->, result %{public}s", result);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }
    err = NextStr(&result, &c);
    TELEPHONY_LOGD("ProcessParamSignalStrength  enter -->, result %{public}s", result);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }

    if (!strcmp(c, "GSM")) {
        TELEPHONY_LOGD("ProcessParamSignalStrength  enter GSM-->, result %{public}s", result);
        ParseGetGsmSignalStrength(result, hrilRssi);
    } else if (!strcmp(c, "LTE")) {
        TELEPHONY_LOGD("ProcessParamSignalStrength  enter LTE-->, result %{public}s", result);
        ParseGetLteSignalStrength(result, hrilRssi);
    } else if (!strcmp(c, "WCDMA")) {
        TELEPHONY_LOGD("ProcessParamSignalStrength  enter WCDMA-->, result %{public}s", result);
        ParseGetWcdmaSignalStrength(result, hrilRssi);
    }
    return 0;
}

void ReqGetSignalStrength(const ReqDataInfo *requestInfo)
{
    int ret;
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    const long TIME_OUT = DEFAULT_TIMEOUT;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    TELEPHONY_LOGD("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    ret = SendCommandLock("AT^HCSQ?", "^HCSQ:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(reportInfo, NULL, 0);
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
    ret = ProcessParamSignalStrength(result, &hrilRssi);
    if (ret != 0) {
        err = HRIL_ERR_GENERIC_FAILURE;
        TELEPHONY_LOGE("SignalStrength is null!");
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(reportInfo, (void *)&hrilRssi, sizeof(HRilRssi));
    FreeResponseInfo(responseInfo);
}

void ReqGetCsRegStatus(const ReqDataInfo *requestInfo)
{
    int ret;
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    int numCount = MAX_REG_INFO_ITEM;
    char *response[MAX_REG_INFO_ITEM] = {""};
    const long TIME_OUT = DEFAULT_TIMEOUT;

    ret = SendCommandLock("AT+CREG?", "+CREG:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    if (result == NULL) {
        TELEPHONY_LOGE("ReqGetCsRegStatus result is null");
    } else {
        SkipATPrefix(&result);
        for (int i = 0; i < numCount; i++) {
            ret = NextStr(&result, &response[i]);
            if (ret == -1) {
                response[i] = "";
                TELEPHONY_LOGE("read failed: %{public}d", ret);
                break;
            }
            TELEPHONY_LOGD("result[%{public}d]: %{public}s", i, response[i]);
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(reportInfo, (void *)response, numCount * sizeof(char *));
    FreeResponseInfo(responseInfo);
}

void ReqGetPsRegStatus(const ReqDataInfo *requestInfo)
{
    int ret;
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    int numCount = MAX_REG_INFO_ITEM;
    char *response[MAX_REG_INFO_ITEM] = {""};
    const long TIME_OUT = DEFAULT_TIMEOUT;

    ret = SendCommandLock("AT+CGREG?", "+CGREG:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    if (result == NULL) {
        TELEPHONY_LOGE("ReqGetCsRegStatus result is null");
    } else {
        SkipATPrefix(&result);
        for (int i = 0; i < numCount; i++) {
            ret = NextStr(&result, &response[i]);
            if (ret == -1) {
                response[i] = "";
                break;
            }
            TELEPHONY_LOGD("result[%{public}d]: %{public}s", i, response[i]);
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(reportInfo, (void *)response, numCount * sizeof(char *));
    FreeResponseInfo(responseInfo);
}

void ReqGetOperatorInfo(const ReqDataInfo *requestInfo)
{
    int ret;
    int err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    const int numCount = 3;
    char *response[numCount] = {"", "", ""};
    Line *pLine = NULL;
    int i = 0;
    const long TIME_OUT = DEFAULT_TIMEOUT;

    ret = SendCommandLock("AT+COPS=3,2;+COPS?;+COPS=3,1;+COPS?;+COPS=3,0;+COPS?", "+COPS:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    for (i = 0, pLine = responseInfo->head; pLine != NULL; i++, pLine = pLine->next) {
        int skip;
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
        TELEPHONY_LOGD("result[%{public}d]: %{public}s", i, response[i]);
        if (ret == -1) {
            response[i] = "";
            TELEPHONY_LOGE("read failed");
            break;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(reportInfo, (void *)response, numCount * sizeof(char *));
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

void GetNetworkSearchInformationPause()
{
    TELEPHONY_LOGD("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    pthread_mutex_lock(&g_networkSearchInformationMutex);
    g_reportInfoForOperListToUse.error = HRIL_ERR_GENERIC_FAILURE;
    OnNetworkReport(g_reportInfoForOperListToUse, NULL, 0);
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
        TELEPHONY_LOGD("enter to [%{public}s]:%{public}d", __func__, __LINE__);
        if (sendFlag) {
            g_reportInfoForOperListToUse.error = HRIL_ERR_GENERIC_FAILURE;
            OnNetworkReport(g_reportInfoForOperListToUse, NULL, 0);
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

void RequestGetNetworkSearchInformation(const ReqDataInfo *requestInfo)
{
    int ret = 0;
    const long TIME_OUT = 1000;
    ResponseInfo *responseInfo = NULL;
    const int MINUTE = 60;
    TELEPHONY_LOGD("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    pthread_mutex_lock(&g_networkSearchInformationMutex);
    alarm(0);
    if (g_reportInfoForOperListToUse.requestInfo != NULL) {
        free(g_reportInfoForOperListToUse.requestInfo);
        g_reportInfoForOperListToUse.requestInfo = NULL;
    }
    if (signal(SIGALRM, PerformTimeOut) == SIG_ERR) {
        TELEPHONY_LOGE("RequestGetNetworkSearchInformation signal PerformTimeOut is SIG_ERR!");
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
        return;
    }
    ret = SendCommandNetWorksLock("AT+COPS=?", "+COPS:", TIME_OUT, &responseInfo);

    g_reportInfoForOperListToUse = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    if ((ret != 0 && ret != AT_ERR_WAITING) || (responseInfo != NULL && !responseInfo->success)) {
        TELEPHONY_LOGE("send AT CMD failed!");
        SetAtPauseFlag(false);
        g_reportInfoForOperListToUse.error = GetResponseErrorCode(responseInfo);
        OnNetworkReport(g_reportInfoForOperListToUse, NULL, 0);
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
    alarm(MINUTE);
    if (responseInfo != NULL) {
        FreeResponseInfo(responseInfo);
    }
    pthread_mutex_unlock(&g_networkSearchInformationMutex);
    SetWatchFunction(GetNetworkSearchInformationPause);
}

int ParseOperListInfo(char *lineinfo, int count, AvailableOperInfo *pOperInfo, AvailableOperInfo **ppOperInfo)
{
    int ret = 0;
    int state = 0;
    int rat = 0;
    int operCount = 0;
    char *line = lineinfo;
    int item = count;
    for (int i = 0; i < item && operCount < item; i++) {
        ret = MoveLeftBracket(&line);
        if (ret < 0) {
            break;
        }
        ret = NextInt(&line, &state);
        if (ret < 0) {
            break;
        }
        char *longName = NULL;
        ret = NextStr(&line, &longName);
        if (ret < 0) {
            break;
        }
        char *shortName = NULL;
        ret = NextStr(&line, &shortName);
        if (ret < 0) {
            break;
        }
        char *numeric = NULL;
        ret = NextStr(&line, &numeric);
        if (ret < 0) {
            break;
        }
        ret = NextInt(&line, &rat);
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

static void DealNetworkSearchInformation(int operCount, AvailableOperInfo **ppOperInfo, AvailableOperInfo *pOperInfo)
{
    if (operCount == 0) {
        pthread_mutex_lock(&g_networkSearchInformationMutex);
        SetAtPauseFlag(false);
        alarm(0);
        if (g_reportInfoForOperListToUse.requestInfo != NULL) {
            g_reportInfoForOperListToUse.error = HRIL_ERR_INVALID_RESPONSE;
            OnNetworkReport(g_reportInfoForOperListToUse, NULL, 0);
            free(g_reportInfoForOperListToUse.requestInfo);
            g_reportInfoForOperListToUse.requestInfo = NULL;
        }
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
    } else {
        pthread_mutex_lock(&g_networkSearchInformationMutex);
        SetAtPauseFlag(false);
        alarm(0);
        if (g_reportInfoForOperListToUse.requestInfo != NULL) {
            OnNetworkReport(g_reportInfoForOperListToUse, (void *)ppOperInfo, operCount * sizeof(AvailableOperInfo *));
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

int ProcessOperListToUse(char *list)
{
    int item = 0;
    int operCount = 0;
    const int UNUSED_ITEM_COUNT = 2;
    AvailableOperInfo *pOperInfo = NULL;
    AvailableOperInfo **ppOperInfo = NULL;
    if (list == NULL) {
        TELEPHONY_LOGD("ProcessOperListToUse result is null");
        goto ERROR;
    }
    char *line = list;
    while (*line != '\0') {
        if (*line == ')') {
            item++;
        }
        line++;
    }
    if (item <= UNUSED_ITEM_COUNT) {
        goto ERROR;
    }
    line = list;
    int ret = SkipATPrefix(&line);
    if (ret < 0) {
        goto ERROR;
    }
    item = item - UNUSED_ITEM_COUNT;
    ppOperInfo = (AvailableOperInfo **)malloc(item * sizeof(AvailableOperInfo *));
    if (!ppOperInfo) {
        goto ERROR;
    }
    pOperInfo = (AvailableOperInfo *)malloc(item * sizeof(AvailableOperInfo));
    if (!pOperInfo) {
        goto ERROR;
    }
    (void)memset_s(pOperInfo, item * sizeof(AvailableOperInfo), 0, item * sizeof(AvailableOperInfo));
    for (int j = 0; j < item; j++) {
        ppOperInfo[j] = &(pOperInfo[j]);
    }
    operCount = ParseOperListInfo(line, item, pOperInfo, ppOperInfo);
    if (operCount != 0) {
        DealNetworkSearchInformation(operCount, ppOperInfo, pOperInfo);
    } else {
        goto ERROR;
    }
    return HRIL_ERR_SUCCESS;
ERROR:
    TELEPHONY_LOGE("RequestGetNetworkSearchInformation Failed");
    DealNetworkSearchInformation(operCount, ppOperInfo, pOperInfo);
    return HRIL_ERR_GENERIC_FAILURE;
}

static bool PrepareSetNetworkSelectionMode(char **cmd, const HRiSetNetworkModeInfo *setModeInfo)
{
    bool ret = true;
    TELEPHONY_LOGD("setModeInfo, serial123 = %{public}d", setModeInfo->selectMode);
    if (setModeInfo->selectMode == 0) {
        asprintf(cmd, "%s", "AT+COPS=0");
    } else if (setModeInfo->selectMode == 1) {
        if (setModeInfo->oper == NULL) {
            ret = false;
        } else {
            asprintf(cmd, "AT+COPS=1,2,%s", setModeInfo->oper);
        }
    } else {
        ret = false;
    }
    return ret;
}

void RequestSetAutomaticModeForNetworks(const ReqDataInfo *requestInfo, const HRiSetNetworkModeInfo *data)
{
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    char *cmd = NULL;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    HRiSetNetworkModeInfo *setModeInfo = (HRiSetNetworkModeInfo *)data;
    if (setModeInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode HRIL_ERR_NULL_POINT");
        return;
    }
    if (!PrepareSetNetworkSelectionMode(&cmd, setModeInfo)) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnNetworkReport(reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode HRIL_ERR_INVALID_PARAMETER");
        return;
    }
    TELEPHONY_LOGD("requestSetAutomaticModeForNetworks, cmd = %{public}s", cmd);
    err = SendCommandLock(cmd, NULL, 0, &responseInfo);
    if (cmd != NULL) {
        free(cmd);
        cmd = NULL;
    }
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode responseInfo == NULL");
        return;
    }
    TELEPHONY_LOGD("SetAutomaticModeForNetworks, responseInfo->success = %{public}d", responseInfo->success);
    if (err != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnNetworkReport(reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode HRIL_ERR_GENERIC_FAILURE");
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 1);
    OnNetworkReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void RequestQueryNetworkSelectionMode(const ReqDataInfo *requestInfo)
{
    int ret = 0;
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    int state = 0;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));

    ret = SendCommandLock("AT+COPS?", "+COPS:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnNetworkReport(reportInfo, NULL, 0);
        return;
    }
    if (responseInfo == NULL) {
        err = HRIL_ERR_NULL_POINT;
        goto ERROR;
        return;
    }
    if (ret != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        goto ERROR;
    }
    if (responseInfo->head == NULL) {
        err = HRIL_ERR_INVALID_RESPONSE;
        goto ERROR;
    }
    Line *pLine = responseInfo->head;
    char *line = pLine->data;
    ret = SkipATPrefix(&line);
    if (ret < 0) {
        err = HRIL_ERR_INVALID_RESPONSE;
        goto ERROR;
    }
    ret = NextInt(&line, &state);
    if (ret < 0) {
        err = HRIL_ERR_INVALID_RESPONSE;
        goto ERROR;
    }

    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 1);
    OnNetworkReport(reportInfo, &state, sizeof(int));
    FreeResponseInfo(responseInfo);
    return;
ERROR:
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 1);
    OnNetworkReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}