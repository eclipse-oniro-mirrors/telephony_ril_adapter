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

#ifndef OHOS_AT_CALL_H
#define OHOS_AT_CALL_H

#include "hril.h"

typedef enum {
    GET_CALL_WAITING,
    SET_CALL_WAITING,
} CallWaitingReqType;

typedef enum {
    CALL_CLIR_SUBSCRIPTION_DEFUALT, /* subscription default */
    CALL_CLIR_INVOCATION, /* invocation */
    CALL_CLIR_SUPPRESSION, /* suppression */
} CallClirType;

void ReqGetCallList(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
void ReqDial(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
void ReqHangup(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
void ReqReject(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
void ReqAnswer(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
void ReqHold(ReqDataInfo *requestInfo);
void ReqActive(ReqDataInfo *requestInfo);
void ReqSwap(ReqDataInfo *requestInfo);
void ReqJoin(ReqDataInfo *requestInfo, int callType);
void ReqSplit(ReqDataInfo *requestInfo, int nThCall, int callType);
void ReqCallSupplement(ReqDataInfo *requestInfo, int type);
void ReqGetCallWait(ReqDataInfo *requestInfo);
void ReqSetCallWait(ReqDataInfo *requestInfo, int active);
void ReqSetCallForwarding(ReqDataInfo *requestInfo, HRilCFInfo info);
void ReqGetCallForwarding(ReqDataInfo *requestInfo, int reason);
void ReqGetCallRestriction(ReqDataInfo *requestInfo, const char *fac);
void ReqSetCallRestriction(ReqDataInfo *requestInfo, CallRestrictionInfo info);
void ReqGetClip(ReqDataInfo *requestInfo);
void ReqSetClip(ReqDataInfo *requestInfo, int action);
void ReqGetClir(ReqDataInfo *requestInfo);
void ReqSetClir(ReqDataInfo *requestInfo, int action);
void ReqStartDtmf(ReqDataInfo *requestInfo, CallDTMFInfo info);
void ReqSendDtmf(ReqDataInfo *requestInfo, CallDTMFInfo info);
void ReqStopDtmf(ReqDataInfo *requestInfo, CallDTMFInfo info);

#endif // OHOS_AT_CALL_H
