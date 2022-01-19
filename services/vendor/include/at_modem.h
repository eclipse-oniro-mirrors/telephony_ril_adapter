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

#ifndef OHOS_AT_MODEM_H
#define OHOS_AT_MODEM_H

#include "vendor_adapter.h"

void ReqSetRadioState(const ReqDataInfo *requestInfo, int32_t function, int32_t reset);
void ReqGetRadioState(const ReqDataInfo *requestInfo);
void ReqGetImei(const ReqDataInfo *requestInfo);
void ReqGetMeid(const ReqDataInfo *requestInfo);
void ReqGetVoiceRadioTechnology(const ReqDataInfo *requestInfo);
int32_t ProcessVoiceRadioInfo(const char *s, const HRilVoiceRadioInfo *hrilVoiceRadioInfo);
#endif // OHOS_AT_MODEM_H
