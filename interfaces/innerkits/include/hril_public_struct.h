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

#ifndef OHOS_RIL_PUBLIC_STRUCT_H
#define OHOS_RIL_PUBLIC_STRUCT_H

#include <stdint.h>

#include "hril_enum.h"

typedef struct ReqDataInfo {
    int32_t serial;
    int32_t request;
    HRilSimSlotId slotId;
} ReqDataInfo;

typedef struct ModemReportErrorInfo {
    int32_t errorNo;
    ReportErrorType errType;
} ModemReportErrorInfo;

typedef struct ReportInfo {
    ReqDataInfo *requestInfo;
    int32_t notifyId;
    ReportType type;
    HRilErrNumber error;
    ModemReportErrorInfo modemErrInfo;
    HRilAckTypes ack;
} ReportInfo;
#endif // OHOS_RIL_PUBLIC_STRUCT_H
