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

#include "hril_hdf.h"

#include <pthread.h>

#include "dfx_signal_handler.h"
#include "parameter.h"

#include "telephony_log_c.h"

#define RIL_VENDOR_LIB_PATH "persist.sys.radio.vendorlib.path"
#define PARAMETER_SIZE 256

static struct HRilReport g_reportOps = {
    OnCallReport,
    OnDataReport,
    OnModemReport,
    OnNetworkReport,
    OnSimReport,
    OnSmsReport
};

static int GetVendorLibPath(char *path, int length)
{
    int code = GetParameter(RIL_VENDOR_LIB_PATH, "", path, PARAMETER_SIZE);
    if (code <= 0) {
        TELEPHONY_LOGE("Failed to get vendor library path through system properties. err:%{public}d", code);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static void LoadVendor(void)
{
    const char *rilLibPath = NULL;
    char vendorLibPath[PARAMETER_SIZE] = {0};
    // Pointer to ril init function in vendor ril
    const HRilOps *(*rilInitOps)(const struct HRilReport *) = NULL;
    // functions returned by ril init function in vendor ril
    const HRilOps *ops = NULL;

    int length = sizeof(vendorLibPath) / sizeof(vendorLibPath[0]);
    if (GetVendorLibPath(vendorLibPath, length) == HDF_SUCCESS) {
        rilLibPath = vendorLibPath;
    } else {
        rilLibPath = g_modem_list[MODEM_INDEX].path;
    }
    if (rilLibPath == NULL) {
        TELEPHONY_LOGE("dynamic library path is empty");
        return;
    }

    g_dlHandle = dlopen(rilLibPath, RTLD_NOW);
    if (g_dlHandle == NULL) {
        TELEPHONY_LOGE("dlopen %{public}s is fail. %{public}s", rilLibPath, dlerror());
        return;
    }
    rilInitOps = (const HRilOps *(*)(const struct HRilReport *))dlsym(g_dlHandle, "RilInitOps");
    if (rilInitOps == NULL) {
        dlclose(g_dlHandle);
        TELEPHONY_LOGE("RilInit not defined or exported");
        return;
    }
    ops = rilInitOps(&g_reportOps);
    HRilRegOps(ops);
}

static int32_t RilAdapterDispatch(
    struct HdfDeviceIoClient *client, int32_t cmd, struct HdfSBuf *data, struct HdfSBuf *reply)
{
    int32_t ret;
    static pthread_mutex_t dispatchMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&dispatchMutex);
    ret = DispatchRequest(SLOTID, cmd, data);
    pthread_mutex_unlock(&dispatchMutex);
    return ret;
}

static struct IDeviceIoService g_rilAdapterService = {
    .Dispatch = RilAdapterDispatch,
    .Open = NULL,
    .Release = NULL,
};

static int32_t RilAdapterBind(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        return HDF_ERR_INVALID_OBJECT;
    }
    device->service = &g_rilAdapterService;
    return HDF_SUCCESS;
}

static int32_t RilAdapterInit(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        return HDF_ERR_INVALID_OBJECT;
    }
    DFX_InstallSignalHandler();
    struct HdfSBuf *sbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (sbuf == NULL) {
        TELEPHONY_LOGE("HdfSampleDriverBind, failed to obtain ipc sbuf");
        return HDF_ERR_INVALID_OBJECT;
    }
    if (!HdfSbufWriteString(sbuf, "string")) {
        TELEPHONY_LOGE("HdfSampleDriverBind, failed to write string to ipc sbuf");
        HdfSBufRecycle(sbuf);
        return HDF_FAILURE;
    }
    if (sbuf != NULL) {
        HdfSBufRecycle(sbuf);
    }
    TELEPHONY_LOGI("sbuf IPC obtain success!");
    if (!IsLoadedVendorLib()) {
        LoadVendor();
    } else {
        TELEPHONY_LOGI("The vendor library has been loaded!");
    }
    return HDF_SUCCESS;
}

static void RilAdapterRelease(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        return;
    }
    dlclose(g_dlHandle);
}

struct HdfDriverEntry g_rilAdapterDevEntry = {
    .moduleVersion = 1,
    .moduleName = MODULE_NAME,
    .Bind = RilAdapterBind,
    .Init = RilAdapterInit,
    .Release = RilAdapterRelease,
};
HDF_INIT(g_rilAdapterDevEntry);