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

#include "at_modem.h"
#include "vendor_util.h"
#include "vendor_report.h"
#include "vendor_adapter.h"

void ReqSetRadioStatus(const ReqDataInfo *requestInfo, int function, int reset)
{
    int ret = -1;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));

    ret = SetRadioState(function, reset);
    if (ret != 0) {
        TELEPHONY_LOGE("ReqSetRadioStatus failed");
        goto ERR;
    }

    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnModemReport(reportInfo, NULL, 0);
    return;
ERR:
    if (ret == HRIL_ERR_REPEAT_STATUS) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_REPEAT_STATUS, HRIL_RESPONSE, 0);
        OnModemReport(reportInfo, NULL, 0);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
    OnModemReport(reportInfo, NULL, 0);
}

void ReqGetRadioStatus(const ReqDataInfo *requestInfo)
{
    const long long timeOut = DEFAULT_TIMEOUT;
    int ret = -1;
    char *pLine = NULL;
    int radioStatus = -1;
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));

    ret = SendCommandLock("AT+CFUN?", "+CFUN", timeOut, &pResponse);
    if (ret != 0 || !pResponse->success) {
        TELEPHONY_LOGE("AT+CFUN send failed");
        goto ERR;
    }
    if (pResponse->head != NULL) {
        pLine = pResponse->head->data;
    }

    ret = SkipATPrefix(&pLine);
    if (ret != 0) {
        goto ERR;
    }
    ret = NextInt(&pLine, &radioStatus);
    if (ret != 0) {
        goto ERR;
    }

    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnModemReport(reportInfo, &radioStatus, sizeof(int));
    FreeResponseInfo(pResponse);
    return;
ERR:
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
    OnModemReport(reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}