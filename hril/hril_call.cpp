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

#include "hril_call.h"
#include "hril_call_parcel.h"

namespace OHOS {
namespace Telephony {
HRilCall::HRilCall()
{
    AddHandlerToMap();
}

HRilCall::~HRilCall()
{
    respMemberFuncMap_.clear();
    notiMemberFuncMap_.clear();
    reqMemberFuncMap_.clear();
    callFuncs_ = nullptr;
}

void HRilCall::ProcessCallResponse(
    int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    auto itFunc = respMemberFuncMap_.find(code);
    if (itFunc != respMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, code, responseInfo, response, responseLen);
        }
    } else {
        TELEPHONY_LOGE("not fund resqfunc: %{public}d", code);
    }
}

void HRilCall::ProcessCallRequest(int32_t slotId, int32_t code, struct HdfSBuf *data)
{
    auto itFunc = reqMemberFuncMap_.find(code);
    if (itFunc != reqMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, data);
        }
    } else {
        TELEPHONY_LOGE("Can not find CallRequest code in reqMemberFuncMap_!");
    }
}

bool HRilCall::IsCallResponse(uint32_t code)
{
    return ((code >= HREQ_CALL_BASE) && (code < HREQ_SMS_BASE));
}

bool HRilCall::IsCallNotification(uint32_t code)
{
    return ((code >= HNOTI_CALL_BASE) && (code < HNOTI_SMS_BASE));
}

bool HRilCall::IsCallRespOrNotify(uint32_t code)
{
    return IsCallResponse(code) || IsCallNotification(code);
}

void HRilCall::ProcessCallNotify(
    int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen)
{
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            int32_t slotId = 0;
            (this->*memberFunc)(slotId, notifyType, e, response, responseLen);
        }
    } else {
        TELEPHONY_LOGE("not fund notifunc: %{public}d", code);
    }
}

void HRilCall::AddHandlerToMap()
{
    // Notification
    notiMemberFuncMap_[HNOTI_CALL_STATE_UPDATED] = &HRilCall::CallStateUpdated;

    // Response
    respMemberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &HRilCall::GetCallListResponse;
    respMemberFuncMap_[HREQ_CALL_DIAL] = &HRilCall::DialResponse;
    respMemberFuncMap_[HREQ_CALL_HANGUP] = &HRilCall::HangupResponse;
    respMemberFuncMap_[HREQ_CALL_REJECT] = &HRilCall::RejectResponse;
    respMemberFuncMap_[HREQ_CALL_ANSWER] = &HRilCall::AnswerResponse;
    respMemberFuncMap_[HREQ_CALL_HOLD] = &HRilCall::HoldResponse;
    respMemberFuncMap_[HREQ_CALL_ACTIVE] = &HRilCall::ActiveResponse;
    respMemberFuncMap_[HREQ_CALL_SWAP] = &HRilCall::SwapResponse;
    respMemberFuncMap_[HREQ_CALL_DEAL_CLIP] = &HRilCall::GetClipResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CLIP] = &HRilCall::SetClipResponse;
    respMemberFuncMap_[HREQ_CALL_JOIN] = &HRilCall::JoinResponse;
    respMemberFuncMap_[HREQ_CALL_SPLIT] = &HRilCall::SplitResponse;
    respMemberFuncMap_[HREQ_CALL_SUPPLEMENT] = &HRilCall::CallSupplementResponse;
    respMemberFuncMap_[HREQ_CALL_GET_CALL_WAIT] = &HRilCall::GetCallWaitResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CALL_WAIT] = &HRilCall::SetCallWaitResponse;
    respMemberFuncMap_[HREQ_CALL_GET_CALL_FORWARDING] = &HRilCall::GetCallForwardResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CALL_FORWARDING] = &HRilCall::SetCallForwardResponse;
    respMemberFuncMap_[HREQ_CALL_GET_CALL_RESTRICTION] = &HRilCall::GetCallRestrictionResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CALL_RESTRICTION] = &HRilCall::SetCallRestrictionResponse;
    respMemberFuncMap_[HREQ_CALL_DEAL_CLIR] = &HRilCall::GetClirResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CLIR] = &HRilCall::SetClirResponse;
    respMemberFuncMap_[HREQ_CALL_START_DTMF] = &HRilCall::StartDtmfResponse;
    respMemberFuncMap_[HREQ_CALL_SEND_DTMF] = &HRilCall::SendDtmfResponse;
    respMemberFuncMap_[HREQ_CALL_STOP_DTMF] = &HRilCall::StopDtmfResponse;

    // request
    reqMemberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &HRilCall::GetCallList;
    reqMemberFuncMap_[HREQ_CALL_DIAL] = &HRilCall::Dial;
    reqMemberFuncMap_[HREQ_CALL_HANGUP] = &HRilCall::Hangup;
    reqMemberFuncMap_[HREQ_CALL_REJECT] = &HRilCall::Reject;
    reqMemberFuncMap_[HREQ_CALL_ANSWER] = &HRilCall::Answer;
    reqMemberFuncMap_[HREQ_CALL_HOLD] = &HRilCall::Hold;
    reqMemberFuncMap_[HREQ_CALL_ACTIVE] = &HRilCall::Active;
    reqMemberFuncMap_[HREQ_CALL_SWAP] = &HRilCall::Swap;
    reqMemberFuncMap_[HREQ_CALL_DEAL_CLIP] = &HRilCall::GetClip;
    reqMemberFuncMap_[HREQ_CALL_SET_CLIP] = &HRilCall::SetClip;
    reqMemberFuncMap_[HREQ_CALL_JOIN] = &HRilCall::Join;
    reqMemberFuncMap_[HREQ_CALL_SPLIT] = &HRilCall::Split;
    reqMemberFuncMap_[HREQ_CALL_SUPPLEMENT] = &HRilCall::CallSupplement;
    reqMemberFuncMap_[HREQ_CALL_GET_CALL_WAIT] = &HRilCall::GetCallWait;
    reqMemberFuncMap_[HREQ_CALL_SET_CALL_WAIT] = &HRilCall::SetCallWait;
    reqMemberFuncMap_[HREQ_CALL_GET_CALL_FORWARDING] = &HRilCall::GetCallForward;
    reqMemberFuncMap_[HREQ_CALL_SET_CALL_FORWARDING] = &HRilCall::SetCallForward;
    reqMemberFuncMap_[HREQ_CALL_GET_CALL_RESTRICTION] = &HRilCall::GetCallRestriction;
    reqMemberFuncMap_[HREQ_CALL_SET_CALL_RESTRICTION] = &HRilCall::SetCallRestriction;
    reqMemberFuncMap_[HREQ_CALL_DEAL_CLIR] = &HRilCall::GetClir;
    reqMemberFuncMap_[HREQ_CALL_SET_CLIR] = &HRilCall::SetClir;
    reqMemberFuncMap_[HREQ_CALL_START_DTMF] = &HRilCall::StartDtmf;
    reqMemberFuncMap_[HREQ_CALL_SEND_DTMF] = &HRilCall::SendDtmf;
    reqMemberFuncMap_[HREQ_CALL_STOP_DTMF] = &HRilCall::StopDtmf;
}

void HRilCall::Dial(int32_t slotId, struct HdfSBuf *data)
{
    const int32_t NUM_POINTERS = 1;
    DialInfo dialInfo = DialInfo();
    MessageParcel *parcel = nullptr;
    if (data == nullptr) {
        TELEPHONY_LOGE("RilAdapter Dial data is null!");
        return;
    }
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (!dialInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(dialInfo.serial, slotId, HREQ_CALL_DIAL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Dial HRilRequest!");
        return;
    }
    HRilDial dial = {};

    if (!ConvertToString(&dial.address, dialInfo.address, requestInfo)) {
        TELEPHONY_LOGE("RilAdapter failed to do ConvertToString!");
        free(requestInfo);
        return;
    }
    dial.clir = (int32_t)dialInfo.clir;
    if (callFuncs_ == nullptr) {
        FreeStrings(NUM_POINTERS, dial.address);
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->Dial(requestInfo, &dial, sizeof(HRilDial));
    FreeStrings(NUM_POINTERS, dial.address);
    free(requestInfo);
}

void HRilCall::GetCallList(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CALL_LIST);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallList HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->GetCallList(requestInfo, nullptr, 0);
    free(requestInfo);
}

void HRilCall::Hangup(int32_t slotId, struct HdfSBuf *data)
{
    uint32_t serial = 0;
    uint32_t gsmIndex = 0;
    if (!HdfSbufReadUint32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    if (!HdfSbufReadUint32(data, &gsmIndex)) {
        TELEPHONY_LOGE("miss gsmIndex parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_HANGUP);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Hangup HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->Hangup(requestInfo, &gsmIndex, sizeof(uint32_t));
    free(requestInfo);
}

void HRilCall::Reject(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_REJECT);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Reject HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->Reject(requestInfo, nullptr, 0);
    free(requestInfo);
}

void HRilCall::GetClip(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_DEAL_CLIP);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetClip HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->GetClip(requestInfo);
    free(requestInfo);
}

void HRilCall::SetClip(int32_t slotId, struct HdfSBuf *data)
{
    int32_t action = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &action)) {
        TELEPHONY_LOGE("miss action parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SET_CLIP);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetClip HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->SetClip(requestInfo, action);
    free(requestInfo);
}

void HRilCall::GetClir(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetClir miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_DEAL_CLIR);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetClir HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->GetClir(requestInfo);
    free(requestInfo);
}

void HRilCall::SetClir(int32_t slotId, struct HdfSBuf *data)
{
    int32_t action = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SetClir miss serial parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &action)) {
        TELEPHONY_LOGE("SetClir miss action parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SET_CLIR);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetClir HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->SetClir(requestInfo, action);
    free(requestInfo);
}

void HRilCall::GetCallRestriction(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetCallRestriction miss serial parameter");
        return;
    }

    const char *fac = NULL;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("GetCallRestriction failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetCallRestriction parcel is nullptr");
        return;
    }
    fac = parcel->ReadCString();
    TELEPHONY_LOGD("HRilCall::GetCallRestriction fac %{public}p", fac);

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CALL_RESTRICTION);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallRestriction HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->GetCallRestriction(requestInfo, fac);
    free(requestInfo);
}

void HRilCall::SetCallRestriction(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SetCallRestriction miss serial parameter");
        return;
    }

    int32_t mode = 0;
    if (!HdfSbufReadInt32(data, &mode)) {
        TELEPHONY_LOGE("SetCallRestriction miss mode parameter");
        return;
    }

    const char *fac = NULL;
    const char *code = nullptr;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("SetCallRestriction failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("HRilCall::SetCallRestriction parcel is null!");
        return;
    }
    fac = parcel->ReadCString();
    code = parcel->ReadCString();

    CallRestrictionInfo info = {.fac = fac, .mode = mode, .password = code};
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SET_CALL_RESTRICTION);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetCallRestriction HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->SetCallRestriction(requestInfo, info);
    free(requestInfo);
}

void HRilCall::StartDtmf(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("StartDtmf miss serial parameter");
        return;
    }
    int32_t index = 1;
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("StartDtmf miss index parameter");
        return;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("HRilCall::StartDtmf parcel is null!");
        return;
    }
    CallDTMFInfo info = {0};
    info.callId = index;
    info.dtmfKey = parcel->ReadCString();
    if (info.dtmfKey == nullptr) {
        TELEPHONY_LOGE("StartDtmf miss dtmfKey parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_START_DTMF);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create StartDtmf HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->StartDtmf(requestInfo, info);
    free(requestInfo);
}

void HRilCall::SendDtmf(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    int32_t index = 1;
    int32_t onLength = 1;
    int32_t offLength = 0;
    int32_t stringLength = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SendDtmf miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("SendDtmf miss index parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &onLength)) {
        TELEPHONY_LOGE("SendDtmf miss onLength parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &offLength)) {
        TELEPHONY_LOGE("SendDtmf miss offLength parameter");
        return;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("HRilCall::SendDtmf  failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("HRilCall::SendDtmf parcel is null!");
        return;
    }
    if (!HdfSbufReadInt32(data, &stringLength)) {
        TELEPHONY_LOGE("SendDtmf miss stringLength parameter");
        return;
    }
    CallDTMFInfo info = {0};
    info.callId = index;
    info.onLength = onLength;
    info.offLength = offLength;
    info.stringLength = stringLength;
    info.dtmfKey = parcel->ReadCString();
    if (info.dtmfKey == nullptr) {
        TELEPHONY_LOGE("SendDtmf miss dtmfKey parameter");
        return;
    }
    ProcessSendDtmf(serial, slotId, info);
}

void HRilCall::ProcessSendDtmf(int32_t serial, int32_t slotId, CallDTMFInfo &info)
{
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SEND_DTMF);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("HRilCall::ProcessSendDtmf failed to do Create SendDtmf HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->SendDtmf(requestInfo, info);
    free(requestInfo);
}

void HRilCall::StopDtmf(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("StopDtmf miss serial parameter");
        return;
    }
    int32_t index = 1;
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("StopDtmf miss index parameter");
        return;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("HRilCall::StopDtmf parcel is null!");
        return;
    }
    CallDTMFInfo info = {0};
    info.callId = index;
    info.dtmfKey = parcel->ReadCString();
    if (info.dtmfKey == nullptr) {
        TELEPHONY_LOGE("StopDtmf miss dtmfKey parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_STOP_DTMF);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create StopDtmf HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->StopDtmf(requestInfo, info);
    free(requestInfo);
}

void HRilCall::Answer(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_ANSWER);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Answer HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->Answer(requestInfo, nullptr, 0);
    free(requestInfo);
}

void HRilCall::Hold(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call Hold miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_HOLD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("HRilCall::Hold failed to do Create Hold HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::Hold callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->Hold(requestInfo);
    free(requestInfo);
}

void HRilCall::Active(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call Active miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_ACTIVE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Active HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::Active callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->Active(requestInfo);
    free(requestInfo);
}

void HRilCall::Swap(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call Swap miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SWAP);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Swap Request!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->Swap(requestInfo);
    free(requestInfo);
}

void HRilCall::Join(int32_t slotId, struct HdfSBuf *data)
{
    uint32_t serial = 0;
    uint32_t callType = 0;

    if (!HdfSbufReadUint32(data, &serial)) {
        TELEPHONY_LOGE("hril call Join miss serial parameter");
        return;
    }

    if (!HdfSbufReadUint32(data, &callType)) {
        TELEPHONY_LOGE("hril call Join miss callType parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_JOIN);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Join Request!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->Join(requestInfo, callType);
    free(requestInfo);
}

void HRilCall::Split(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    int32_t nThCall = 0;
    int32_t callType = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call Split miss serial parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &nThCall)) {
        TELEPHONY_LOGE("hril call Split miss nThCall parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &callType)) {
        TELEPHONY_LOGE("hril call Split miss callType parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SPLIT);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Split Request!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->Split(requestInfo, nThCall, callType);
    free(requestInfo);
}

void HRilCall::CallSupplement(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    int32_t type = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call CallSupplement miss serial parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &type)) {
        TELEPHONY_LOGE("hril call CallSupplement miss type parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SUPPLEMENT);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create CallSupplement Request!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->CallSupplement(requestInfo, type);
    free(requestInfo);
}

void HRilCall::GetCallWait(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CALL_WAIT);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallWait HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetCallWait callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->GetCallWait(requestInfo);
    free(requestInfo);
}

void HRilCall::SetCallWait(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    int32_t operating = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SetCallWait >>> miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &operating)) {
        TELEPHONY_LOGE("SetCallWait >> miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SET_CALL_WAIT);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetCallWait HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::SetCallWait callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->SetCallWait(requestInfo, operating);
    free(requestInfo);
}

void HRilCall::GetCallForward(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    int32_t reason = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetCallForward >>> miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &reason)) {
        TELEPHONY_LOGE("GetCallForward >> miss reason parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CALL_FORWARDING);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallForward HRilRequest !");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetCallForward callFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    callFuncs_->GetCallForwarding(requestInfo, reason);
    free(requestInfo);
}

void HRilCall::SetCallForward(int32_t slotId, struct HdfSBuf *data)
{
    const int32_t NUM_POINTERS = 1;
    CallForwardSetInfo callForwardSetInfo = CallForwardSetInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("HRilCall::SetCallForward parcel is nullptr!");
        return;
    }
    if (!callForwardSetInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(callForwardSetInfo.serial, slotId, HREQ_CALL_SET_CALL_FORWARDING);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetCallForward HRilRequest!");
        return;
    }

    HRilCFInfo cFInfo = {};
    if (!ConvertToString(&cFInfo.number, callForwardSetInfo.number, requestInfo)) {
        TELEPHONY_LOGE("RilAdapter failed to do ConvertToString!");
        free(requestInfo);
        return;
    }
    cFInfo.reason = callForwardSetInfo.reason;
    cFInfo.mode = callForwardSetInfo.mode;
    cFInfo.classx = callForwardSetInfo.classx;
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::SetCallForward callFuncs_ is nullptr!");
        FreeStrings(NUM_POINTERS, cFInfo.number);
        free(requestInfo);
        return;
    }
    callFuncs_->SetCallForwarding(requestInfo, cFInfo);
    FreeStrings(NUM_POINTERS, cFInfo.number);
    free(requestInfo);
}

void HRilCall::BuildCallList(
    CallInfoList &callInfoList, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCallInfo *)) != 0) {
        TELEPHONY_LOGE("BuildCallList: Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t num = responseLen / sizeof(HRilCallInfo *);
        CallInfo callInfo;
        callInfoList.callSize = num;
        for (int32_t i = 0; i < num; i++) {
            HRilCallInfo *curPtr = ((HRilCallInfo **)response)[i];
            if (curPtr != nullptr) {
                callInfo.index = curPtr->index;
                callInfo.dir = curPtr->dir;
                callInfo.state = curPtr->state;
                callInfo.mode = curPtr->mode;
                callInfo.mpty = curPtr->mpty;
                callInfo.number = (curPtr->number == nullptr) ? "" : curPtr->number;
                callInfo.type = curPtr->type;
                callInfo.alpha = (curPtr->alpha == nullptr) ? "" : curPtr->alpha;
                callInfoList.calls.push_back(callInfo);
            } else {
                TELEPHONY_LOGE("BuildCallList: Invalid curPtr");
                break;
            }
        }
    }
}

int32_t HRilCall::GetCallListResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    CallInfoList callList;
    if (response == nullptr) {
        TELEPHONY_LOGE("GetCallListResponse response is null!!!");
        return HDF_FAILURE;
    }
    BuildCallList(callList, responseInfo, response, responseLen);

    return ResponseMessageParcel(responseInfo, callList, requestNum);
}

int32_t HRilCall::DialResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::HangupResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::RejectResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::AnswerResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::HoldResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetClipResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("GetClipResponse response is null!!!");
        return HDF_FAILURE;
    }
    HRilGetClipResult *pGetClip = (HRilGetClipResult *)response;
    TELEPHONY_LOGD(
        "HRilCall::GetClipResponse action:%{public}d, clipStat:%{public}d.", pGetClip->action, pGetClip->clipStat);
    GetClipResult getClipResult;
    getClipResult.result = static_cast<int32_t>(responseInfo.error);
    getClipResult.action = pGetClip->action;
    getClipResult.clipStat = pGetClip->clipStat;
    return ResponseMessageParcel(responseInfo, getClipResult, requestNum);
}

int32_t HRilCall::SetClipResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}
int32_t HRilCall::ActiveResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SwapResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::JoinResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SplitResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::CallSupplementResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetCallWaitResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("GetCallWaitResponse response is nullptr.");
        return HDF_FAILURE;
    }

    HRilCallWaitResult *result = (HRilCallWaitResult *)response;
    TELEPHONY_LOGD("GetCallWaitResponse, status:%{public}d, classCw:%{public}d.", result->status, result->classCw);
    CallWaitResult callWaitResult;
    callWaitResult.result = static_cast<int32_t>(responseInfo.error);
    callWaitResult.status = result->status;
    callWaitResult.classCw = result->classCw;
    return ResponseMessageParcel(responseInfo, callWaitResult, requestNum);
}

int32_t HRilCall::SetCallWaitResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetCallForwardResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("GetCallForwardResponse response is null!!!");
        return HDF_FAILURE;
    }
    HRilCFQueryInfo *pCFQueryInfo = (HRilCFQueryInfo *)response;
    CallForwardQueryResult cFQueryResult;

    cFQueryResult.result = static_cast<int32_t>(responseInfo.error);
    cFQueryResult.serial = responseInfo.serial;
    cFQueryResult.status = pCFQueryInfo->status;
    cFQueryResult.classx = pCFQueryInfo->classx;
    cFQueryResult.type = pCFQueryInfo->type;
    cFQueryResult.number = (pCFQueryInfo->number == nullptr) ? "" : pCFQueryInfo->number;
    return ResponseMessageParcel(responseInfo, cFQueryResult, requestNum);
}

int32_t HRilCall::SetCallForwardResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetClirResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("GetClirResponse response is null!!!");
        return HDF_FAILURE;
    }
    HRilGetCallClirResult *pGetClir = (HRilGetCallClirResult *)response;
    TELEPHONY_LOGD(
        "HRilCall::GetClirResponse action:%{public}d, clirStat:%{public}d.", pGetClir->action, pGetClir->clirStat);
    GetClirResult getClirResult;
    getClirResult.result = static_cast<int32_t>(responseInfo.error);
    getClirResult.action = pGetClir->action;
    getClirResult.clirStat = pGetClir->clirStat;
    return ResponseMessageParcel(responseInfo, getClirResult, requestNum);
}

int32_t HRilCall::SetClirResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetCallRestrictionResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetCallRestrictionResponse response is nullptr");
        return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
    }
    HRilCallRestrictionResult *result = (HRilCallRestrictionResult *)response;
    TELEPHONY_LOGD("HRilCall::GetCallRestrictionResponse status:%{public}d, classCw:%{public}d.", result->status,
        result->classCw);
    CallRestrictionResult resultT;
    resultT.result = static_cast<int32_t>(responseInfo.error);
    resultT.status = result->status;
    resultT.classCw = result->classCw;
    return ResponseMessageParcel(responseInfo, resultT, requestNum);
}

int32_t HRilCall::SetCallRestrictionResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::StartDtmfResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SendDtmfResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::StopDtmfResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::CallStateUpdated(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter serviceCallbackNotify_ is null");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallbackNotify_->dispatcher->Dispatch(
        serviceCallbackNotify_, HNOTI_CALL_STATE_UPDATED, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

void HRilCall::RegisterCallFuncs(const HRilCallReq *callFuncs)
{
    callFuncs_ = callFuncs;
}
} // namespace Telephony
} // namespace OHOS
