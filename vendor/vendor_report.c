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

#include "vendor_report.h"

#include "at_sms.h"
#include "vendor_adapter.h"

extern const struct HRilReport *g_reportOps;
#define MAX_BUFF_SIZE 500

void OnModemReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnModemReport(reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnCallReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnCallReport(reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnDataReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnDataReport(reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnSmsReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnSmsReport(reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnNetworkReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnNetworkReport(reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnSimReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnSimReport(reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
        return;
    }
}

struct ReportInfo CreateReportInfo(
    const ReqDataInfo *requestInfo, unsigned int err, unsigned int type, int notifyId)
{
    struct ReportInfo reportInfo = {};

    reportInfo.requestInfo = (ReqDataInfo *)requestInfo;
    reportInfo.notifyId = notifyId;
    reportInfo.type = type;
    reportInfo.error = err;

    return reportInfo;
}

static void ReportCBMOrCSCB(struct ReportInfo *reportInfo)
{
    if (reportInfo == NULL) {
        TELEPHONY_LOGE("reportInfo is null");
        return;
    }

    const char *tempData =
        ("01a41f51101102ea3030a830ea30a230e130fc30eb914d4fe130c630b930c8000d000a305330"
        "8c306f8a669a137528306e30e130c330bb30fc30b8306730593002000d000aff0800320030003"
        "10033002f00310031002f003252ea300037002000310035003a00340034ff09000d000aff0830"
        "a830ea30a25e02ff0900000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000022"); // test data
    HRilCellBroadcastReportInfo response = {0};
    reportInfo->notifyId = HNOTI_CELL_BROADCAST_REPORT;
    int ret = ProcessCellBroadcast("+CBM:100", &response);
    if (ret > 1) {
        response.data = (char *)tempData;
    } else {
        response.pdu = (char *)tempData;
    }
    OnSmsReport(*reportInfo, (void *)&response, sizeof(HRilCellBroadcastReportInfo *));
}

void OnNotifyOps(const char *s, const char *smsPdu)
{
    int ret = 0;
    struct ReportInfo reportInfo = {0};
    reportInfo.error = HRIL_ERR_SUCCESS;
    reportInfo.type = HRIL_NOTIFICATION;

    if (GetRadioState() == HRIL_RADIO_POWER_STATE_UNAVAILABLE) {
        return;
    }
    TELEPHONY_LOGD("enter to [%{public}s]:%{public}s", s, smsPdu);

    if (ReportStrWith(s, "+CRING:") || ReportStrWith(s, "RING") || ReportStrWith(s, "IRING") ||
        ReportStrWith(s, "NO CARRIER") || ReportStrWith(s, "+CCWA") || ReportStrWith(s, "^CCALLSTATE") ||
        ReportStrWith(s, "^CEND") || ReportStrWith(s, "^CCWA")) {
        reportInfo.notifyId = HNOTI_CALL_STATE_UPDATED;
        OnCallReport(reportInfo, NULL, 0);
    } else if (ReportStrWith(s, "+CMT:")) {
        reportInfo.notifyId = HNOTI_SMS_NEW_SMS;
        OnSmsReport(reportInfo, (void *)smsPdu, strlen(smsPdu));
    } else if (ReportStrWith(s, "+CDS:")) {
        int size = 0;
        if (smsPdu != NULL) {
            size = strlen(smsPdu);
        }
        reportInfo.notifyId = HNOTI_SMS_STATUS_REPORT;
        OnSmsReport(reportInfo, (void *)smsPdu, size);
    } else if (ReportStrWith(s, "+CREG:") || ReportStrWith(s, "+CGREG:")) {
        char *response[MAX_REG_INFO_ITEM] = {""};
        char *str = strdup(s);
        reportInfo.notifyId = HNOTI_NETWORK_CS_REG_STATUS_UPDATED;
        ret = ProcessRegStatus(str, response, MAX_REG_INFO_ITEM);
        OnNetworkReport(reportInfo, (void *)response, MAX_REG_INFO_ITEM * sizeof(char *));
        free(str);
    } else if (ReportStrWith(s, "^HCSQ:")) {
        HRilRssi response = {0};
        char *str = strdup(s);
        reportInfo.notifyId = HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED;
        ret = ProcessParamSignalStrength(str, &response);
        OnNetworkReport(reportInfo, (void *)&response, sizeof(HRilRssi));
        free(str);
    } else if (ReportStrWith(s, "+CBM:") || ReportStrWith(s, "+CSCB:")) {
        ReportCBMOrCSCB(&reportInfo);
    } else if (ReportStrWith(s, "+COPS: (")) {
        char *copsstr = strdup(s);
        ProcessOperListToUse(copsstr);
        free(copsstr);
    } else if (ReportStrWith(s, "^SIMST:")) {
        reportInfo.notifyId = HNOTI_SIM_STATUS_CHANGED;
        OnSimReport(reportInfo, NULL, 0);
    } else {
        TELEPHONY_LOGW("enter to  is unrecognized command: %{public}s", s);
    }
}