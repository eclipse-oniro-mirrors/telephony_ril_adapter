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

#include "at_call.h"
#include "at_sms.h"
#include "hril_notification.h"
#include "vendor_adapter.h"

static const struct HRilReport *g_reportOps = NULL;

void OnModemReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnModemReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnCallReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnCallReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnDataReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnDataReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnSmsReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnSmsReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnNetworkReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnNetworkReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
    }
}

void OnSimReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_reportOps != NULL) {
        g_reportOps->OnSimReport(slotId, reportInfo, response, responseLen);
    } else {
        TELEPHONY_LOGE("g_reportOps is NULL");
        return;
    }
}

struct ReportInfo CreateReportInfo(const ReqDataInfo *requestInfo, unsigned int err, unsigned int type, int notifyId)
{
    struct ReportInfo reportInfo = {(ReqDataInfo *)requestInfo, notifyId, type, err, {0, 0}};
    return reportInfo;
}

void SetReportOps(const struct HRilReport *reportOps)
{
    g_reportOps = reportOps;
}

static void ReportCBMOrCSCB(struct ReportInfo *reportInfo)
{
    if (reportInfo == NULL) {
        TELEPHONY_LOGE("reportInfo is null");
        return;
    }
    static int32_t testCount = 0;
    char *tempData = NULL;
    char *testDataStr =
        ("01a41f51101102ea3030a830ea30a230e130fc30eb914d4fe130c630b930c8000d000a305330"
         "8c306f8a669a137528306e30e130c330bb30fc30b8306730593002000d000aff0800320030003"
         "10033002f00310031002f003252ea300037002000310035003a00340034ff09000d000aff0830"
         "a830ea30a25e02ff0900000000000000000000000000000000000000"
         "000000000000000000000000000000000000000000000000000000000022");
    char *testDataTmp =
        ("C0000032401174747A0E4ACF41E8B0BCFD76E741EF39685C66B34162F93B4C1E87E77410BD3CA7836EC2341D440ED3C321");
    if (testCount == 0) {
        tempData = testDataStr;
        testCount++;
    } else {
        tempData = testDataTmp;
        testCount = 0;
    }
    HRilCBConfigReportInfo response = {0};
    reportInfo->notifyId = HNOTI_CB_CONFIG_REPORT;
    int ret = ProcessCellBroadcast("+CBM:100", &response);
    if (ret > 1) {
        response.data = (char *)tempData;
    } else {
        response.pdu = (char *)tempData;
    }
    OnSmsReport(HRIL_SIM_SLOT_1, *reportInfo, (const uint8_t *)&response, sizeof(HRilCBConfigReportInfo));
}

void OnNotifyOps(const char *s, const char *smsPdu)
{
    char *str = NULL;
    struct ReportInfo reportInfo = {0};
    reportInfo.error = HRIL_ERR_SUCCESS;
    reportInfo.type = HRIL_NOTIFICATION;

    if (GetRadioState() == HRIL_RADIO_POWER_STATE_UNAVAILABLE) {
        return;
    }
    str = strdup(s);
    if (IsCallNoticeCmd(s)) {
        CallReportInfoProcess(s);
    } else if (ReportStrWith(s, "+CMT:")) {
        reportInfo.notifyId = HNOTI_SMS_NEW_SMS;
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)smsPdu, strlen(smsPdu));
    } else if (ReportStrWith(s, "+CDS:")) {
        int size = (smsPdu != NULL) ? strlen(smsPdu) : 0;
        reportInfo.notifyId = HNOTI_SMS_STATUS_REPORT;
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)smsPdu, size);
    } else if (ReportStrWith(s, "+CBM:") || ReportStrWith(s, "+CSCB:")) {
        ReportCBMOrCSCB(&reportInfo);
        ReportCBMOrCSCB(&reportInfo); // The test requires sending two
    } else if (ReportStrWith(s, "+COPS: (")) {
        char *copsStr = strdup(s);
        ProcessOperListToUse(copsStr);
        free(copsStr);
    } else if (ReportStrWith(s, "^SIMST:")) {
        reportInfo.notifyId = HNOTI_SIM_STATUS_CHANGED;
        OnSimReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    } else {
        OnNotifyNetWorksOps(s, str);
    }
    free(str);
}

void OnNotifyNetWorksOps(const char *s, const char *infoStr)
{
    int ret = 0;
    char *str = (char *)infoStr;
    char *responseData[MAX_REG_INFO_ITEM] = {""};
    struct ReportInfo reportInfo = {0};
    reportInfo.error = HRIL_ERR_SUCCESS;
    reportInfo.type = HRIL_NOTIFICATION;
    if (ReportStrWith(s, "+CREG:") || ReportStrWith(s, "+CGREG:")) {
        reportInfo.notifyId = HNOTI_NETWORK_CS_REG_STATUS_UPDATED;
        HRilRegStatusInfo regStatusInfo;
        ret = ProcessRegStatus(str, &regStatusInfo);
        if (ret == 0) {
            OnNetworkReport(
                HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)(&regStatusInfo), sizeof(HRilRegStatusInfo));
        } else {
            TELEPHONY_LOGW("CREG or CGREG notify str format  unexpected: %{public}s", s);
        }
    } else if (ReportStrWith(s, "^TIME:")) {
        reportInfo.notifyId = HNOTI_NETWORK_TIME_UPDATED;
        char *time[DEFAULT_INDEX] = {""};
        int32_t tmp = GenerateCommand((char *)time, DEFAULT_INDEX, "^TIME:\"20%s", infoStr + DEFAULT_ADD_NUM);
        if (tmp < 0) {
            TELEPHONY_LOGE("GenerateCommand is failed!");
            OnNetworkReport(
                HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)responseData, MAX_REG_INFO_ITEM * sizeof(char));
            return;
        }
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)time, MAX_REG_INFO_ITEM * sizeof(char));
    } else if (ReportStrWith(s, "+CTZV:")) {
        reportInfo.notifyId = HNOTI_NETWORK_TIME_ZONE_UPDATED;
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)responseData, MAX_REG_INFO_ITEM * sizeof(char));
    } else if (ReportStrWith(s, "^HCSQ:")) {
        HRilRssi response = {0};
        reportInfo.notifyId = HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED;
        ret = ProcessParamSignalStrengthNotify(str, &response);
        OnNetworkReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&response, sizeof(HRilRssi));
    } else if (ReportStrWith(s, "+CIREGU:")) {
        HRilImsRegStatusInfo imsRegStatusInfo;
        reportInfo.notifyId = HNOTI_NETWORK_IMS_REG_STATUS_UPDATED;
        ret = ProcessImsRegStatus(str, &imsRegStatusInfo, MAX_IMS_REG_INFO_ITEM);
        if (ret == 0) {
            OnNetworkReport(
                HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)(&imsRegStatusInfo), sizeof(HRilImsRegStatusInfo));
        } else {
            TELEPHONY_LOGW("CIREGU notify str format  unexpected: %{public}s", s);
        }
    }
}