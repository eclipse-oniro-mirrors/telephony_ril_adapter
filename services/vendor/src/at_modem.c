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
#include "vendor_adapter.h"
#include "vendor_report.h"
#include "vendor_util.h"

void ReqSetRadioState(const ReqDataInfo *requestInfo, int function, int reset)
{
    struct ReportInfo reportInfo;
    int ret = SetRadioState(function, reset);
    if (ret == HRIL_ERR_SUCCESS) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    } else {
        TELEPHONY_LOGE("ReqSetRadioState failed");
        if (ret == HRIL_ERR_REPEAT_STATUS) {
            reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_REPEAT_STATUS, HRIL_RESPONSE, 0);
        } else {
            reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        }
    }
    OnModemReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
}

static void ErrorHandler(const ReqDataInfo *requestInfo, ResponseInfo *pResponse)
{
    struct ReportInfo reportInfo;
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
    OnModemReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetRadioState(const ReqDataInfo *requestInfo)
{
    const long long timeOut = DEFAULT_TIMEOUT;
    char *pLine = NULL;
    int radioState = -1;
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo;

    int ret = SendCommandLock("AT+CFUN?", "+CFUN", timeOut, &pResponse);
    if (ret != 0 || !pResponse->success) {
        TELEPHONY_LOGE("AT+CFUN send failed");
        return ErrorHandler(requestInfo, pResponse);
    }
    if (pResponse->head != NULL) {
        pLine = pResponse->head->data;
    }

    ret = SkipATPrefix(&pLine);
    if (ret != 0) {
        return ErrorHandler(requestInfo, pResponse);
    }
    ret = NextInt(&pLine, &radioState);
    if (ret != 0) {
        return ErrorHandler(requestInfo, pResponse);
    }

    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnModemReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&radioState, sizeof(int));
    FreeResponseInfo(pResponse);
    return;
}