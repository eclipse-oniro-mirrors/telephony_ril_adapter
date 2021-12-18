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
    CALL_CLIR_SUBSCRIPTION_DEFAULT, /* subscription default */
    CALL_CLIR_INVOCATION, /* invocation */
    CALL_CLIR_SUPPRESSION, /* suppression */
} CallClirType;

typedef struct {
    char *cmd;
    void (*function)(const char *str);
} CallNotify;

void ReqGetCallList(const ReqDataInfo *requestInfo);
void ReqDial(const ReqDataInfo *requestInfo, const HRilDial *data, size_t dataLen);
void ReqHangup(const ReqDataInfo *requestInfo, const uint32_t *data, size_t dataLen);
void ReqReject(const ReqDataInfo *requestInfo);
void ReqAnswer(const ReqDataInfo *requestInfo);
void ReqHoldCall(const ReqDataInfo *requestInfo);
void ReqUnHoldCall(const ReqDataInfo *requestInfo);
void ReqSwitchCall(const ReqDataInfo *requestInfo);
void ReqCombineConference(const ReqDataInfo *requestInfo, int callType);
void ReqSeparateConference(const ReqDataInfo *requestInfo, int callIndex, int callType);
void ReqCallSupplement(const ReqDataInfo *requestInfo, int type);
void ReqGetCallWaiting(const ReqDataInfo *requestInfo);
void ReqSetCallWaiting(const ReqDataInfo *requestInfo, int active);
void ReqSetCallTransferInfo(const ReqDataInfo *requestInfo, HRilCFInfo info);
void ReqGetCallTransferInfo(const ReqDataInfo *requestInfo, int reason);
void ReqGetCallRestriction(const ReqDataInfo *requestInfo, const char *fac);
void ReqSetCallRestriction(const ReqDataInfo *requestInfo, CallRestrictionInfo info);
void ReqGetClip(const ReqDataInfo *requestInfo);
void ReqSetClip(const ReqDataInfo *requestInfo, int action);
void ReqGetClir(const ReqDataInfo *requestInfo);
void ReqSetClir(const ReqDataInfo *requestInfo, int action);
void ReqStartDtmf(const ReqDataInfo *requestInfo, CallDtmfInfo info);
void ReqSendDtmf(const ReqDataInfo *requestInfo, CallDtmfInfo info);
void ReqStopDtmf(const ReqDataInfo *requestInfo, CallDtmfInfo info);
void ReqGetImsCallList(const ReqDataInfo *requestInfo);
void ReqGetCallPreferenceMode(const ReqDataInfo *requestInfo);
void ReqSetCallPreferenceMode(const ReqDataInfo *requestInfo, int mode);
void ReqGetLteImsSwitchStatus(const ReqDataInfo *requestInfo);
void ReqSetLteImsSwitchStatus(const ReqDataInfo *requestInfo, int active);

void ReportImsServiceStatusInfo(const char *str);
void ReportCallStatusInfo(const char *str);
void ReportCallEndInfo(const char *str);
void ReportCallConnectInfo(const char *str);
void ReportCallCringInfo(const char *str);
void ReportCallWaitingInfo(const char *str);
void ReportCallStatusUpdate(const char *str);
void CallReportInfoProcess(const char *str);
int IsCallNoticeCmd(const char *str);
#endif // OHOS_AT_CALL_H
