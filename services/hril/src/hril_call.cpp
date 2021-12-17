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
#include "hril_notification.h"
#include "hril_request.h"

namespace OHOS {
namespace Telephony {
HRilCall::HRilCall()
{
    AddCallNotificationToMap();
    AddCallResponseToMap();
    AddCallRequestToMap();
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
        TELEPHONY_LOGE("Can not find CallRequest code in respMemberFuncMap_: %{public}d!", code);
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

void HRilCall::ProcessCallNotify(int32_t slotId, int32_t notifyType, const struct ReportInfo *reportInfo,
    const void *response, size_t responseLen)
{
    int code;
    HRilErrno e;
    code = reportInfo->notifyId;
    e = (HRilErrno)reportInfo->error;
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, notifyType, e, response, responseLen);
        }
    } else {
        TELEPHONY_LOGE("Can not find CallRequest code in notiMemberFuncMap_: %{public}d!", code);
    }
}

void HRilCall::AddCallNotificationToMap()
{
    // Notification
    notiMemberFuncMap_[HNOTI_CALL_STATE_UPDATED] = &HRilCall::CallStateUpdated;
    notiMemberFuncMap_[HNOTI_CALL_CRING_REPORT] = &HRilCall::CallCringNotice;
    notiMemberFuncMap_[HNOTI_CALL_WAITING_REPORT] = &HRilCall::CallWaitingNotice;
    notiMemberFuncMap_[HNOTI_CALL_CONNECT_REPORT] = &HRilCall::CallConnectNotice;
    notiMemberFuncMap_[HNOTI_CALL_END_REPORT] = &HRilCall::CallEndNotice;
    notiMemberFuncMap_[HNOTI_CALL_STATUS_INFO_REPORT] = &HRilCall::CallStatusInfoNotice;
    notiMemberFuncMap_[HNOTI_CALL_IMS_SERVICE_STATUS_REPORT] = &HRilCall::CallImsServiceStatusNotice;
}

void HRilCall::AddCallResponseToMap()
{
    // Response
    respMemberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &HRilCall::GetCallListResponse;
    respMemberFuncMap_[HREQ_CALL_DIAL] = &HRilCall::DialResponse;
    respMemberFuncMap_[HREQ_CALL_HANGUP] = &HRilCall::HangupResponse;
    respMemberFuncMap_[HREQ_CALL_REJECT] = &HRilCall::RejectResponse;
    respMemberFuncMap_[HREQ_CALL_ANSWER] = &HRilCall::AnswerResponse;
    respMemberFuncMap_[HREQ_CALL_HOLD_CALL] = &HRilCall::HoldCallResponse;
    respMemberFuncMap_[HREQ_CALL_UNHOLD_CALL] = &HRilCall::UnHoldCallResponse;
    respMemberFuncMap_[HREQ_CALL_SWITCH_CALL] = &HRilCall::SwitchCallResponse;
    respMemberFuncMap_[HREQ_CALL_GET_CLIP] = &HRilCall::GetClipResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CLIP] = &HRilCall::SetClipResponse;
    respMemberFuncMap_[HREQ_CALL_COMBINE_CONFERENCE] = &HRilCall::CombineConferenceResponse;
    respMemberFuncMap_[HREQ_CALL_SEPARATE_CONFERENCE] = &HRilCall::SeparateConferenceResponse;
    respMemberFuncMap_[HREQ_CALL_CALL_SUPPLEMENT] = &HRilCall::CallSupplementResponse;
    respMemberFuncMap_[HREQ_CALL_GET_CALL_WAITING] = &HRilCall::GetCallWaitingResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CALL_WAITING] = &HRilCall::SetCallWaitingResponse;
    respMemberFuncMap_[HREQ_CALL_GET_CALL_TRANSFER_INFO] = &HRilCall::GetCallTransferInfoResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CALL_TRANSFER_INFO] = &HRilCall::SetCallTransferInfoResponse;
    respMemberFuncMap_[HREQ_CALL_GET_CALL_RESTRICTION] = &HRilCall::GetCallRestrictionResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CALL_RESTRICTION] = &HRilCall::SetCallRestrictionResponse;
    respMemberFuncMap_[HREQ_CALL_GET_CLIR] = &HRilCall::GetClirResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CLIR] = &HRilCall::SetClirResponse;
    respMemberFuncMap_[HREQ_CALL_START_DTMF] = &HRilCall::StartDtmfResponse;
    respMemberFuncMap_[HREQ_CALL_SEND_DTMF] = &HRilCall::SendDtmfResponse;
    respMemberFuncMap_[HREQ_CALL_STOP_DTMF] = &HRilCall::StopDtmfResponse;
    respMemberFuncMap_[HREQ_CALL_GET_IMS_CALL_LIST] = &HRilCall::GetImsCallListResponse;
    respMemberFuncMap_[HREQ_CALL_GET_CALL_PREFERENCE] = &HRilCall::GetCallPreferenceModeResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CALL_PREFERENCE] = &HRilCall::SetCallPreferenceModeResponse;
    respMemberFuncMap_[HREQ_CALL_GET_LTEIMSSWITCH_STATUS] = &HRilCall::GetLteImsSwitchStatusResponse;
    respMemberFuncMap_[HREQ_CALL_SET_LTEIMSSWITCH_STATUS] = &HRilCall::SetLteImsSwitchStatusResponse;
}

void HRilCall::AddCallRequestToMap()
{
    // request
    reqMemberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &HRilCall::GetCallList;
    reqMemberFuncMap_[HREQ_CALL_DIAL] = &HRilCall::Dial;
    reqMemberFuncMap_[HREQ_CALL_HANGUP] = &HRilCall::Hangup;
    reqMemberFuncMap_[HREQ_CALL_REJECT] = &HRilCall::Reject;
    reqMemberFuncMap_[HREQ_CALL_ANSWER] = &HRilCall::Answer;
    reqMemberFuncMap_[HREQ_CALL_HOLD_CALL] = &HRilCall::HoldCall;
    reqMemberFuncMap_[HREQ_CALL_UNHOLD_CALL] = &HRilCall::UnHoldCall;
    reqMemberFuncMap_[HREQ_CALL_SWITCH_CALL] = &HRilCall::SwitchCall;
    reqMemberFuncMap_[HREQ_CALL_GET_CLIP] = &HRilCall::GetClip;
    reqMemberFuncMap_[HREQ_CALL_SET_CLIP] = &HRilCall::SetClip;
    reqMemberFuncMap_[HREQ_CALL_COMBINE_CONFERENCE] = &HRilCall::CombineConference;
    reqMemberFuncMap_[HREQ_CALL_SEPARATE_CONFERENCE] = &HRilCall::SeparateConference;
    reqMemberFuncMap_[HREQ_CALL_CALL_SUPPLEMENT] = &HRilCall::CallSupplement;
    reqMemberFuncMap_[HREQ_CALL_GET_CALL_WAITING] = &HRilCall::GetCallWaiting;
    reqMemberFuncMap_[HREQ_CALL_SET_CALL_WAITING] = &HRilCall::SetCallWaiting;
    reqMemberFuncMap_[HREQ_CALL_GET_CALL_TRANSFER_INFO] = &HRilCall::GetCallTransferInfo;
    reqMemberFuncMap_[HREQ_CALL_SET_CALL_TRANSFER_INFO] = &HRilCall::SetCallTransferInfo;
    reqMemberFuncMap_[HREQ_CALL_GET_CALL_RESTRICTION] = &HRilCall::GetCallRestriction;
    reqMemberFuncMap_[HREQ_CALL_SET_CALL_RESTRICTION] = &HRilCall::SetCallRestriction;
    reqMemberFuncMap_[HREQ_CALL_GET_CLIR] = &HRilCall::GetClir;
    reqMemberFuncMap_[HREQ_CALL_SET_CLIR] = &HRilCall::SetClir;
    reqMemberFuncMap_[HREQ_CALL_START_DTMF] = &HRilCall::StartDtmf;
    reqMemberFuncMap_[HREQ_CALL_SEND_DTMF] = &HRilCall::SendDtmf;
    reqMemberFuncMap_[HREQ_CALL_STOP_DTMF] = &HRilCall::StopDtmf;
    reqMemberFuncMap_[HREQ_CALL_GET_IMS_CALL_LIST] = &HRilCall::GetImsCallList;
    reqMemberFuncMap_[HREQ_CALL_GET_CALL_PREFERENCE] = &HRilCall::GetCallPreferenceMode;
    reqMemberFuncMap_[HREQ_CALL_SET_CALL_PREFERENCE] = &HRilCall::SetCallPreferenceMode;
    reqMemberFuncMap_[HREQ_CALL_GET_LTEIMSSWITCH_STATUS] = &HRilCall::GetLteImsSwitchStatus;
    reqMemberFuncMap_[HREQ_CALL_SET_LTEIMSSWITCH_STATUS] = &HRilCall::SetLteImsSwitchStatus;
}

void HRilCall::Dial(int32_t slotId, struct HdfSBuf *data)
{
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
        SafeFrees(requestInfo);
        return;
    }
    dial.clir = (int32_t)dialInfo.clir;
    if (callFuncs_ == nullptr) {
        SafeFrees(requestInfo, dial.address);
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        return;
    }
    callFuncs_->Dial(requestInfo, &dial, sizeof(HRilDial));
    SafeFrees(requestInfo, dial.address);
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
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->GetCallList(requestInfo);
    SafeFrees(requestInfo);
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
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->Hangup(requestInfo, &gsmIndex, sizeof(uint32_t));
    SafeFrees(requestInfo);
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
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->Reject(requestInfo);
    SafeFrees(requestInfo);
}

void HRilCall::GetClip(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CLIP);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetClip HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->GetClip(requestInfo);
    SafeFrees(requestInfo);
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
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->SetClip(requestInfo, action);
    SafeFrees(requestInfo);
}

void HRilCall::GetClir(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetClir miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CLIR);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetClir HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->GetClir(requestInfo);
    SafeFrees(requestInfo);
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
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->SetClir(requestInfo, action);
    SafeFrees(requestInfo);
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

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CALL_RESTRICTION);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallRestriction HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->GetCallRestriction(requestInfo, fac);
    SafeFrees(requestInfo);
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
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->SetCallRestriction(requestInfo, info);
    SafeFrees(requestInfo);
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
    CallDtmfInfo info = {0};
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
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->StartDtmf(requestInfo, info);
    SafeFrees(requestInfo);
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
    CallDtmfInfo info = {0};
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

void HRilCall::ProcessSendDtmf(int32_t serial, int32_t slotId, CallDtmfInfo &info)
{
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SEND_DTMF);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("HRilCall::ProcessSendDtmf failed to do Create SendDtmf HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->SendDtmf(requestInfo, info);
    SafeFrees(requestInfo);
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
    CallDtmfInfo info = {0};
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
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->StopDtmf(requestInfo, info);
    SafeFrees(requestInfo);
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
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->Answer(requestInfo);
    SafeFrees(requestInfo);
}

void HRilCall::HoldCall(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call Hold miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_HOLD_CALL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("HRilCall::Hold failed to do Create Hold HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::Hold callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->HoldCall(requestInfo);
    SafeFrees(requestInfo);
}

void HRilCall::UnHoldCall(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call Active miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_UNHOLD_CALL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Active HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::Active callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->UnHoldCall(requestInfo);
    SafeFrees(requestInfo);
}

void HRilCall::SwitchCall(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call Swap miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SWITCH_CALL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Swap Request!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->SwitchCall(requestInfo);
    SafeFrees(requestInfo);
}

void HRilCall::CombineConference(int32_t slotId, struct HdfSBuf *data)
{
    uint32_t serial = 0;
    uint32_t callType = 0;

    if (!HdfSbufReadUint32(data, &serial)) {
        TELEPHONY_LOGE("hril call CombineConference miss serial parameter");
        return;
    }

    if (!HdfSbufReadUint32(data, &callType)) {
        TELEPHONY_LOGE("hril call CombineConference miss callType parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_COMBINE_CONFERENCE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create CombineConference Request!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->CombineConference(requestInfo, callType);
    SafeFrees(requestInfo);
}

void HRilCall::SeparateConference(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    int32_t callIndex = 0;
    int32_t callType = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call SeparateConference miss serial parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &callIndex)) {
        TELEPHONY_LOGE("hril call SeparateConference miss callIndex parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &callType)) {
        TELEPHONY_LOGE("hril call SeparateConference miss callType parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SEPARATE_CONFERENCE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SeparateConference Request!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->SeparateConference(requestInfo, callIndex, callType);
    SafeFrees(requestInfo);
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

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_CALL_SUPPLEMENT);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create CallSupplement Request!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter HRilCall::Dial  callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->CallSupplement(requestInfo, type);
    SafeFrees(requestInfo);
}

void HRilCall::GetCallWaiting(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CALL_WAITING);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallWaiting HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetCallWaiting callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->GetCallWaiting(requestInfo);
    SafeFrees(requestInfo);
}

void HRilCall::SetCallWaiting(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    int32_t operating = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SetCallWaiting >>> miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &operating)) {
        TELEPHONY_LOGE("SetCallWaiting >> miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SET_CALL_WAITING);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetCallWaiting HRilRequest!");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::SetCallWaiting callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->SetCallWaiting(requestInfo, operating);
    SafeFrees(requestInfo);
}

void HRilCall::GetCallTransferInfo(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    int32_t reason = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetCallTransferInfo >>> miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &reason)) {
        TELEPHONY_LOGE("GetCallTransferInfo >> miss reason parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CALL_TRANSFER_INFO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallTransferInfo HRilRequest !");
        return;
    }
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetCallTransferInfo callFuncs_ is nullptr!");
        SafeFrees(requestInfo);
        return;
    }
    callFuncs_->GetCallTransferInfo(requestInfo, reason);
    SafeFrees(requestInfo);
}

void HRilCall::SetCallTransferInfo(int32_t slotId, struct HdfSBuf *data)
{
    CallForwardSetInfo callForwardSetInfo = CallForwardSetInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("HRilCall::SetCallTransferInfo parcel is nullptr!");
        return;
    }
    if (!callForwardSetInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(callForwardSetInfo.serial, slotId, HREQ_CALL_SET_CALL_TRANSFER_INFO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetCallTransferInfo HRilRequest!");
        return;
    }

    HRilCFInfo cFInfo = {};
    if (!ConvertToString(&cFInfo.number, callForwardSetInfo.number, requestInfo)) {
        TELEPHONY_LOGE("RilAdapter failed to do ConvertToString!");
        SafeFrees(requestInfo);
        return;
    }
    cFInfo.reason = callForwardSetInfo.reason;
    cFInfo.mode = callForwardSetInfo.mode;
    cFInfo.classx = callForwardSetInfo.classx;
    if (callFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::SetCallTransferInfo callFuncs_ is nullptr!");
        SafeFrees(requestInfo, cFInfo.number);
        return;
    }
    callFuncs_->SetCallTransferInfo(requestInfo, cFInfo);
    SafeFrees(requestInfo, cFInfo.number);
}

void HRilCall::GetImsCallList(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_IMS_CALL_LIST);
    if (requestInfo == nullptr || callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallList HRilRequest!");
        return;
    }
    callFuncs_->GetImsCallList(requestInfo);
}

void HRilCall::GetCallPreferenceMode(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_CALL_PREFERENCE);
    if (requestInfo == nullptr || callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        free(requestInfo);
        return;
    }
    callFuncs_->GetCallPreferenceMode(requestInfo);
    free(requestInfo);
}

void HRilCall::SetCallPreferenceMode(int32_t slotId, struct HdfSBuf *data)
{
    int32_t mode = HRIL_CALL_MODE_CS_1ST_PS_2ND;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &mode)) {
        TELEPHONY_LOGE("miss mode parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SET_CALL_PREFERENCE);
    if (requestInfo == nullptr || callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        free(requestInfo);
        return;
    }
    callFuncs_->SetCallPreferenceMode(requestInfo, mode);
    free(requestInfo);
}

void HRilCall::GetLteImsSwitchStatus(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_GET_LTEIMSSWITCH_STATUS);
    if (requestInfo == nullptr || callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        free(requestInfo);
        return;
    }
    callFuncs_->GetLteImsSwitchStatus(requestInfo);
    free(requestInfo);
}

void HRilCall::SetLteImsSwitchStatus(int32_t slotId, struct HdfSBuf *data)
{
    int32_t active = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &active)) {
        TELEPHONY_LOGE("miss mode parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_CALL_SET_LTEIMSSWITCH_STATUS);
    if (requestInfo == nullptr || callFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        free(requestInfo);
        return;
    }
    callFuncs_->SetLteImsSwitchStatus(requestInfo, active);
    free(requestInfo);
}

void HRilCall::BuildCallList(
    CallInfoList &callInfoList, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCallInfo)) != 0) {
        TELEPHONY_LOGE("BuildCallList: Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t num = responseLen / sizeof(HRilCallInfo);
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
                callInfo.voiceDomain = curPtr->voiceDomain;
                callInfo.callType = curPtr->callType;
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

int32_t HRilCall::HoldCallResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
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
int32_t HRilCall::UnHoldCallResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SwitchCallResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::CombineConferenceResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SeparateConferenceResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::CallSupplementResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetCallWaitingResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("GetCallWaitingResponse response is nullptr.");
        return HDF_FAILURE;
    }

    HRilCallWaitResult *result = (HRilCallWaitResult *)response;
    CallWaitResult callWaitResult;
    callWaitResult.result = static_cast<int32_t>(responseInfo.error);
    callWaitResult.status = result->status;
    callWaitResult.classCw = result->classCw;
    return ResponseMessageParcel(responseInfo, callWaitResult, requestNum);
}

int32_t HRilCall::SetCallWaitingResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetCallTransferInfoResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("GetCallTransferInfoResponse response is null!!!");
        return HDF_FAILURE;
    }
    HRilCFQueryInfo *pCFQueryInfo = (HRilCFQueryInfo *)response;
    CallForwardQueryResult cFQueryResult;

    cFQueryResult.result = static_cast<int32_t>(responseInfo.error);
    cFQueryResult.serial = responseInfo.serial;
    cFQueryResult.status = pCFQueryInfo->status;
    cFQueryResult.classx = pCFQueryInfo->classx;
    cFQueryResult.type = pCFQueryInfo->type;
    cFQueryResult.number = ((pCFQueryInfo->number == nullptr) ? "" : pCFQueryInfo->number);
    return ResponseMessageParcel(responseInfo, cFQueryResult, requestNum);
}

int32_t HRilCall::SetCallTransferInfoResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
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

int32_t HRilCall::GetCallRestrictionResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetCallRestrictionResponse response is nullptr");
        return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
    }
    HRilCallRestrictionResult *result = (HRilCallRestrictionResult *)response;
    CallRestrictionResult resultT;
    resultT.result = static_cast<int32_t>(responseInfo.error);
    resultT.status = result->status;
    resultT.classCw = result->classCw;
    return ResponseMessageParcel(responseInfo, resultT, requestNum);
}

int32_t HRilCall::SetCallRestrictionResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
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

int32_t HRilCall::GetImsCallListResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    CallInfoList callList;
    if (response == nullptr) {
        TELEPHONY_LOGE("GetImsCallListResponse response is null!!!");
        return HDF_FAILURE;
    }
    BuildCallList(callList, responseInfo, response, responseLen);
    return ResponseMessageParcel(responseInfo, callList, requestNum);
}

int32_t HRilCall::GetCallPreferenceModeResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    int32_t mode = 0;
    if (response != nullptr) {
        mode = *((int32_t *)response);
    }
    return ResponseInt32(requestNum, &responseInfo, sizeof(responseInfo), mode);
}

int32_t HRilCall::SetCallPreferenceModeResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetLteImsSwitchStatusResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    int32_t active = 0;
    if (response != nullptr) {
        active = *((int32_t *)response);
    }
    return ResponseInt32(requestNum, &responseInfo, sizeof(responseInfo), active);
}

int32_t HRilCall::SetLteImsSwitchStatusResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::CallStateUpdated(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    int32_t ret = ServiceNotifyDispatcher(HNOTI_CALL_STATE_UPDATED, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

int32_t HRilCall::CallCringNotice(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    CallCringInfo cringInfo;

    if (response == nullptr || (responseLen % sizeof(HRilCallCringInfo))) {
        TELEPHONY_LOGE("This is an invalid notification.");
        return HDF_FAILURE;
    }
    const HRilCallCringInfo *hCringInfo = reinterpret_cast<const HRilCallCringInfo *>(response);
    cringInfo.type = hCringInfo->type == nullptr ? "" : hCringInfo->type;
    cringInfo.pdpType = hCringInfo->pdpType == nullptr ? "" : hCringInfo->pdpType;
    cringInfo.pdpAddress = hCringInfo->pdpAddress == nullptr ? "" : hCringInfo->pdpAddress;
    cringInfo.l2p = hCringInfo->l2p == nullptr ? "" : hCringInfo->l2p;
    cringInfo.apn = hCringInfo->apn == nullptr ? "" : hCringInfo->apn;
    return NotifyMessageParcel(cringInfo, HNOTI_CALL_CRING_REPORT);
}

int32_t HRilCall::CallWaitingNotice(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    CallWaitInfo waitInfo;

    if (response == nullptr || (responseLen % sizeof(HRilCallWaitInfo))) {
        TELEPHONY_LOGE("This is an invalid notification.");
        return HDF_FAILURE;
    }
    const HRilCallWaitInfo *hWaitInfo = reinterpret_cast<const HRilCallWaitInfo *>(response);
    waitInfo.number = hWaitInfo->number == nullptr ? "" : hWaitInfo->number;
    waitInfo.type = hWaitInfo->type;
    waitInfo.businessClass = hWaitInfo->businessClass;
    return NotifyMessageParcel(waitInfo, HNOTI_CALL_WAITING_REPORT);
}

int32_t HRilCall::CallConnectNotice(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    CallConnectInfo connectInfo;

    if (response == nullptr || (responseLen % sizeof(HRilCallConnectInfo))) {
        TELEPHONY_LOGE("This is an invalid notification.");
        return HDF_FAILURE;
    }
    const HRilCallConnectInfo *hConnectInfo = reinterpret_cast<const HRilCallConnectInfo *>(response);
    connectInfo.type = hConnectInfo->type;
    connectInfo.callId = hConnectInfo->callId;
    return NotifyMessageParcel(connectInfo, HNOTI_CALL_CONNECT_REPORT);
}

int32_t HRilCall::CallEndNotice(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    CallEndInfo endInfo;

    if (response == nullptr || (responseLen % sizeof(HRilCallEndInfo))) {
        TELEPHONY_LOGE("This is an invalid notification.");
        return HDF_FAILURE;
    }
    const HRilCallEndInfo *hEndInfo = reinterpret_cast<const HRilCallEndInfo *>(response);
    endInfo.callId = hEndInfo->callId;
    endInfo.duration = hEndInfo->duration;
    endInfo.noCliCause = hEndInfo->noCliCause;
    endInfo.ccCause = hEndInfo->ccCause;
    return NotifyMessageParcel(endInfo, HNOTI_CALL_END_REPORT);
}

int32_t HRilCall::CallStatusInfoNotice(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    CallStatusInfo statusInfo;

    if (response == nullptr || (responseLen % sizeof(HRilCallStatusInfo))) {
        TELEPHONY_LOGE("This is an invalid notification.");
        return HDF_FAILURE;
    }
    const HRilCallStatusInfo *hStatusInfo = reinterpret_cast<const HRilCallStatusInfo *>(response);
    statusInfo.callId = hStatusInfo->callId;
    statusInfo.status = hStatusInfo->status;
    statusInfo.voiceDomain = hStatusInfo->voiceDomain;
    return NotifyMessageParcel(statusInfo, HNOTI_CALL_STATUS_INFO_REPORT);
}

int32_t HRilCall::CallImsServiceStatusNotice(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    CallImsServiceStatus imsServiceStatusInfo;

    if (response == nullptr || (responseLen % sizeof(HRilImsServiceStatus))) {
        TELEPHONY_LOGE("This is an invalid notification.");
        return HDF_FAILURE;
    }
    const HRilImsServiceStatus *hImsServiceStatusInfo = reinterpret_cast<const HRilImsServiceStatus *>(response);
    imsServiceStatusInfo.smsSrvStatus = hImsServiceStatusInfo->smsSrvStatus;
    imsServiceStatusInfo.smsSrvRat = hImsServiceStatusInfo->smsSrvRat;
    imsServiceStatusInfo.voipSrvStatus = hImsServiceStatusInfo->voipSrvStatus;
    imsServiceStatusInfo.voipSrvRat = hImsServiceStatusInfo->voipSrvRat;
    imsServiceStatusInfo.vtSrvStatus = hImsServiceStatusInfo->vtSrvStatus;
    imsServiceStatusInfo.vtSrvRat = hImsServiceStatusInfo->vtSrvRat;
    imsServiceStatusInfo.vsSrvStatus = hImsServiceStatusInfo->vsSrvStatus;
    imsServiceStatusInfo.vsSrvRat = hImsServiceStatusInfo->vsSrvRat;
    return NotifyMessageParcel(imsServiceStatusInfo, HNOTI_CALL_IMS_SERVICE_STATUS_REPORT);
}

void HRilCall::RegisterCallFuncs(const HRilCallReq *callFuncs)
{
    callFuncs_ = callFuncs;
}
} // namespace Telephony
} // namespace OHOS
