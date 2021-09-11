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
#include "at_network.h"
#include "hril.h"
#include "vendor_util.h"

struct ReportInfo CreateReportInfo(
    const ReqDataInfo *requestInfo, unsigned int err, unsigned int type, int notifyId);
void OnModemReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
void OnCallReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
void OnDataReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
void OnSimReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
void OnSmsReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
void OnNetworkReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
void OnNotifyOps(const char *s, const char *smsPdu);
#endif // OHOS_VENDOR_REPORT_H
