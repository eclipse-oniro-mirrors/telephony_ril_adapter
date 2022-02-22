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

#ifndef OHOS_VENDOR_REPORT_H
#define OHOS_VENDOR_REPORT_H

#include <stddef.h>
#include <string.h>

#include "at_data.h"
#include "at_modem.h"
#include "at_network.h"
#include "vendor_util.h"

struct ReportInfo CreateReportInfo(const ReqDataInfo *requestInfo, uint32_t err, uint32_t type, int32_t notifyId);
void SetReportOps(const struct HRilReport *reportOps);
void OnModemReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnCallReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnDataReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnSimReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnSmsReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnNetworkReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnTimerCallback(HRilCallbackFun func, uint8_t *param, const struct timeval *tv);
void OnNotifyOps(const char *s, const char *smsPdu);
void OnNotifyNetWorksOps(const char *s, const char *infoStr);
bool OnNotifyStkOps(const char *s, const char *infoStr);
#endif // OHOS_VENDOR_REPORT_H
