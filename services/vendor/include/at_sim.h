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

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>

#include "securec.h"

#include "hril_vendor_sim_defs.h"

#define OHOS_AT_SIM_H

void ReqGetSimStatus(const ReqDataInfo *requestInfo);
void ReqGetSimIO(const ReqDataInfo *requestInfo, const HRilSimIO *data, size_t dataLen);
void ReqGetSimImsi(const ReqDataInfo *requestInfo);
void ReqGetSimLockStatus(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
void ReqSetSimLock(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
void ReqChangeSimPassword(const ReqDataInfo *requestInfo, const HRilSimPassword *data, size_t dataLen);
void ReqUnlockPin(const ReqDataInfo *requestInfo, const char *pin);
void ReqUnlockPuk(const ReqDataInfo *requestInfo, const char *puk, const char *pin);
void ReqGetSimPinInputTimes(const ReqDataInfo *requestInfo);
void ReqGetSimPinInputTimesRemain(const ReqDataInfo *requestInfo, HRilPinInputTimes *pinInputTimes);
void ReqUnlockPin2(const ReqDataInfo *requestInfo, const char *pin2);
void ReqUnlockPuk2(const ReqDataInfo *requestInfo, const char *puk2, const char *pin2);
void ReqGetSimPin2InputTimes(const ReqDataInfo *requestInfo);
void ReqSetActiveSim(const ReqDataInfo *requestInfo, int32_t index, int32_t enable);
void ReqSimStkSendTerminalResponse(const ReqDataInfo *requestInfo, const char *strCmd);
void ReqSimStkSendEnvelope(const ReqDataInfo *requestInfo, const char *strCmd);
void ReqSimStkSendCallSetupRequestResult(const ReqDataInfo *requestInfo, int32_t accept);
void ReqSimStkIsReady(const ReqDataInfo *requestInfo);
void ReqSetRadioProtocol(const ReqDataInfo *requestInfo, const HRilSimProtocolRequest *data, size_t dataLen);
void ReqSimOpenLogicalChannel(const ReqDataInfo *requestInfo, const char *appID, int32_t p2);
void ReqSimCloseLogicalChannel(const ReqDataInfo *requestInfo, int32_t channelId);
void ReqSimTransmitApduLogicalChannel(const ReqDataInfo *requestInfo, HRilApduSimIO *data, size_t dataLen);
void ReqUnlockSimLock(const ReqDataInfo *requestInfo, int32_t lockType, const char *password);
#endif // OHOS_AT_SIM_H
