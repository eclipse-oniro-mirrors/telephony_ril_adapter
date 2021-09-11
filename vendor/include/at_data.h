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
#include "at_support.h"
#include "vendor_util.h"

#define PPP_TTY_PATH "eth0"

// Default MTU value
#define DEFAULT_MTU 0
#define INT_DEFAULT_VALUE (-1)
#define STR_DEFAULT_VALUE ""

void ReqActivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
void ReqDeactivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
void PdpContextListUpdated(const ReqDataInfo *requestInfo);

#endif // OHOS_AT_DATA_H
