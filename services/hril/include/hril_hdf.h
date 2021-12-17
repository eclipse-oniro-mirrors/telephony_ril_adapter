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

#ifndef OHOS_HRIL_HDF_H
#define OHOS_HRIL_HDF_H

#include <dlfcn.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>

#include "hdf_base.h"
#include "hdf_device_desc.h"
#include "hdf_load_hdi.h"
#include "hdf_remote_service.h"

#include "hril.h"

#define MODEM_INDEX 0

static struct {
    const char *name;
    const char *type;
    const char *path;
    const int slotIdNum;
} g_modem_list[] = {
    {"meig_slm790", "atmodem", "/system/lib/libril_vendor.z.so", 1},
    {"unisoc_xx", "atmodem", "/system/lib/libril_vendor_unisoc_xx.z.so", 2}
};

static void *g_dlHandle = NULL;

extern int32_t DispatchRequest(int32_t slotId, int32_t cmd, struct HdfSBuf *data);
extern void HRilRegOps(const HRilOps *g_hrilOps);

extern void OnCallReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
extern void OnDataReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
extern void OnModemReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
extern void OnNetworkReport(
    int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
extern void OnSimReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
extern void OnSmsReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
extern int32_t IsLoadedVendorLib(void);
#endif // OHOS_RIL_ADAPTER_RIL_INIT_H
