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

#ifndef OHOS_AT_SIM_H
#define OHOS_AT_SIM_H

#include "hril.h"

void ReqGetSimStatus(const ReqDataInfo *requestInfo);
void ReqGetSimIO(const ReqDataInfo *requestInfo, const HRilSimIO *data, size_t dataLen);
void ReqGetSimImsi(const ReqDataInfo *requestInfo);
void ReqGetSimIccID(const ReqDataInfo *requestInfo);
void ReqGetSimLockStatus(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
void ReqSetSimLock(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
void ReqChangeSimPassword(const ReqDataInfo *requestInfo, const HRilSimPassword *data, size_t dataLen);
void ReqEnterSimPin(const ReqDataInfo *requestInfo, const char *pin);
void ReqUnlockSimPin(const ReqDataInfo *requestInfo, const char *puk, const char *pin);
void ReqGetSimPinInputTimes(const ReqDataInfo *requestInfo);

#endif // OHOS_AT_SIM_H