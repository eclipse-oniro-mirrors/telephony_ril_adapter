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

#include "hril_vendor_sim_defs.h"
#include "securec.h"

#define OHOS_AT_SIM_H

#define BYTE_NUM_0 0x00FF
#define BYTE_NUM_1 0xFF00
#define BYTE_NUM_2 0xFF
#define BYTE_NUM_3 0x80
#define BYTE_NUM_4 0x0F
#define CMD_GET_RESPONSE 192
#define DECIMAL_MAX 10
#define EF_TYPE_TRANSPARENT 0
#define EF_TYPE_LINEAR_FIXED 1
#define EF_TYPE_CYCLIC 3
#define FCP_TEMP_T 0x62
#define FIVE_LEN 5
#define GET_RESPONSE_EF_SIZE_BYTES 15
#define HALF_BYTE_LEN 4
#define HALF_LEN 2
#define HEX_DIGITS_LEN 16
#define RESPONSE_DATA_FILE_SIZE_1 2
#define RESPONSE_DATA_FILE_SIZE_2 3
#define RESPONSE_DATA_FILE_ID_1 4
#define RESPONSE_DATA_FILE_ID_2 5
#define RESPONSE_DATA_FILE_TYPE 6
#define RESPONSE_DATA_FILE_STATUS 11
#define RESPONSE_DATA_LENGTH 12
#define RESPONSE_DATA_STRUCTURE 13
#define RESPONSE_DATA_RECORD_LENGTH 14
#define THIRD_INDEX 3
#define TYPE_DF 2
#define TYPE_EF 4
#define VALID_FILE_STATUS 0x01

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
void ReqGetRadioProtocol(const ReqDataInfo *requestInfo);
void ReqSetRadioProtocol(const ReqDataInfo *requestInfo, const HRilRadioProtocol *data);
void ReqSimOpenLogicalChannel(const ReqDataInfo *requestInfo, const char *appID, int32_t p2);
void ReqSimCloseLogicalChannel(const ReqDataInfo *requestInfo, int32_t channelId);
void ReqSimTransmitApduLogicalChannel(const ReqDataInfo *requestInfo, HRilApduSimIO *data, size_t dataLen);
void ReqSimAuthentication(const ReqDataInfo *requestInfo, HRilSimAuthenticationRequestInfo *data, size_t dataLen);
void ReqUnlockSimLock(const ReqDataInfo *requestInfo, int32_t lockType, const char *password);
void ReqSendSimMatchedOperatorInfo(const ReqDataInfo *requestInfo, HRilNcfgOperatorInfo *data, size_t dataLen);
#endif // OHOS_AT_SIM_H
