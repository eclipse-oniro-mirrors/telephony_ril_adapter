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

#ifndef OHOS_RIL_ADAPTER_RIL_INIT_H
#define OHOS_RIL_ADAPTER_RIL_INIT_H

#include <dlfcn.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>

#include "hdf_base.h"
#include "hdf_device_desc.h"
#include "hdf_load_hdi.h"
#include "hdf_remote_service.h"

#include "hril.h"

static void *g_dlHandle = NULL;
static int32_t g_slotId = 0;

extern int32_t DispatchRequest(int32_t slotId, int32_t cmd, struct HdfSBuf *data);
extern void HRilRegOps(const HRilOps *g_hrilOps);

extern void OnCallReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern void OnDataReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern void OnModemReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern void OnNetworkReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern void OnSimReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern void OnSmsReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);

static struct HRilReport g_reportOps = {
    OnCallReport,
    OnDataReport,
    OnModemReport,
    OnNetworkReport,
    OnSimReport,
    OnSmsReport
};
#endif // OHOS_RIL_ADAPTER_RIL_INIT_H
