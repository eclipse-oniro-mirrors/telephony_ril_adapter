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

#ifndef OHOS_MODE_ADAPTER_H
#define OHOS_MODE_ADAPTER_H

typedef struct {
    int32_t idVendor;
    int32_t idProduct;
    char *libPath; // vendorlib path.
} UsbDeviceInfo;

#define MEIG_VENDOR_ID 0x2DEE
#define MEIG_PRODUCT_ID_SLM790 0x4D20

#define FIBOCOM_VENDOR_ID 0x1508
#define FIBOCOM_PRODUCT_ID_NL668 0x1001

/* The USB modem automatically recognizes the configuration. */
#define DEFAULT_MODE_INDEX 0
UsbDeviceInfo g_usbModemVendorInfo[] = {
    // MEIG modem
    {.idVendor = MEIG_VENDOR_ID, .idProduct = MEIG_PRODUCT_ID_SLM790, .libPath = "libril_vendor.z.so"},
    // Fibocom modem
    {.idVendor = FIBOCOM_VENDOR_ID, .idProduct = FIBOCOM_PRODUCT_ID_NL668, .libPath = "libfibocom_ril.z.so"},
};

#endif // OHOS_MODE_ADAPTER_H
