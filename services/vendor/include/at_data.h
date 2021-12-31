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

#ifndef OHOS_AT_DATA_H
#define OHOS_AT_DATA_H

#include <stddef.h>

#include "hril.h"

#define NET_NODE "usb0"

// Default MTU value
#define DEFAULT_MTU 0

#define DEFAULT_MEM_NMEMB 1

#define MAX_CID 11 // GU is 1~16 (currently only 11 is supported, and it can be expanded to 16 in the future).
#define MIN_CID 1 // GU is 1~16 (currently only 11 is supported, and it can be expanded to 16 in the future).
#define MAX_PDP_NUM 11

#define ACTIVATE 1
#define DEACTIVATE 0

#define ADDR_MASK 0xFF
#define ADDR_OFFSET_0BIT 0
#define ADDR_OFFSET_8BIT 8
#define ADDR_OFFSET_16BIT 16
#define ADDR_OFFSET_24BIT 24
#define ADDR_OFFSET_32BIT 32
#define IVP4_INDEX_MAX 4

#define DELAY_WAIT_MS (1000 * 1000)

#define DEFAULT_CID (-1)

#define QUERY_MAX_COUNT 6
#define QUERY_DELAY_MS 500
#define DELAY_US_OFFSET 1000

typedef struct {
    int cid;
    char *ipType;
    char *apn;
} PDNInfo;

void ReqActivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
void ReqDeactivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
void ReqGetPdpContextList(const ReqDataInfo *requestInfo);

#endif // OHOS_AT_DATA_H
