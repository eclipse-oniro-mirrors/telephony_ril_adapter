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
HRilCall::HRilCall(int32_t slotId, IHRilReporter &hrilReporter) : HRilBase(slotId, hrilReporter)
{
    AddCallNotificationToMap();
    AddCallResponseToMap();
    AddCallRequestToMap();
}

HRilCall::~HRilCall()
{
    callFuncs_ = nullptr;
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

void HRilCall::AddCallNotificationToMap()
{
    // Notification
    notiMemberFuncMap_[HNOTI_CALL_STATE_UPDATED] = &HRilCall::CallStateUpdated;
    notiMemberFuncMap_[HNOTI_CALL_IMS_SERVICE_STATUS_REPORT] = &HRilCall::CallImsServiceStatusNotice;
    notiMemberFuncMap_[HNOTI_CALL_USSD_REPORT] = &HRilCall::CallUssdNotice;
    notiMemberFuncMap_[HNOTI_CALL_SRVCC_STATUS_REPORT] = &HRilCall::CallSrvccStatusNotice;
    notiMemberFuncMap_[HNOTI_CALL_RINGBACK_VOICE_REPORT] = &HRilCall::CallRingbackVoiceNotice;
    notiMemberFuncMap_[HNOTI_CALL_EMERGENCY_NUMBER_REPORT] = &HRilCall::CallEmergencyNotice;
    notiMemberFuncMap_[HNOTI_CALL_SS_REPORT] = &HRilCall::CallSsNotice;
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
    respMemberFuncMap_[HREQ_CALL_SET_USSD] = &HRilCall::SetUssdResponse;
    respMemberFuncMap_[HREQ_CALL_GET_USSD] = &HRilCall::GetUssdResponse;
    respMemberFuncMap_[HREQ_CALL_SET_MUTE] = &HRilCall::SetMuteResponse;
    respMemberFuncMap_[HREQ_CALL_GET_MUTE] = &HRilCall::GetMuteResponse;
    respMemberFuncMap_[HREQ_CALL_GET_EMERGENCY_LIST] = &HRilCall::GetEmergencyCallListResponse;
    respMemberFuncMap_[HREQ_CALL_SET_EMERGENCY_LIST] = &HRilCall::SetEmergencyCallListResponse;
    respMemberFuncMap_[HREQ_CALL_GET_FAIL_REASON] = &HRilCall::GetCallFailReasonResponse;
    respMemberFuncMap_[HREQ_CALL_SET_BARRING_PASSWORD] = &HRilCall::SetBarringPasswordResponse;
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
    reqMemberFuncMap_[HREQ_CALL_SET_USSD] = &HRilCall::SetUssd;
    reqMemberFuncMap_[HREQ_CALL_GET_USSD] = &HRilCall::GetUssd;
    reqMemberFuncMap_[HREQ_CALL_SET_MUTE] = &HRilCall::SetMute;
    reqMemberFuncMap_[HREQ_CALL_GET_MUTE] = &HRilCall::GetMute;
    reqMemberFuncMap_[HREQ_CALL_GET_EMERGENCY_LIST] = &HRilCall::GetEmergencyCallList;
    reqMemberFuncMap_[HREQ_CALL_SET_EMERGENCY_LIST] = &HRilCall::SetEmergencyCallList;
    reqMemberFuncMap_[HREQ_CALL_GET_FAIL_REASON] = &HRilCall::GetCallFailReason;
    reqMemberFuncMap_[HREQ_CALL_SET_BARRING_PASSWORD] = &HRilCall::SetBarringPassword;
}

int32_t HRilCall::Dial(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->Dial == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->Dial or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    DialInfo dialInfo = DialInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!dialInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilDial dial = {};
    dial.address = StringToCString(dialInfo.address);
    dial.clir = (int32_t)dialInfo.clir;
    ReqDataInfo *requestInfo = CreateHRilRequest(dialInfo.serial, HREQ_CALL_DIAL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Dial HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->Dial(requestInfo, &dial, sizeof(HRilDial));
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetCallList(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetCallList == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->GetCallList or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_CALL_LIST);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallList HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetCallList(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::Hangup(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->Hangup == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->Hangup or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    uint32_t serial = 0;
    if (!HdfSbufReadUint32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    uint32_t gsmIndex = 0;
    if (!HdfSbufReadUint32(data, &gsmIndex)) {
        TELEPHONY_LOGE("miss gsmIndex parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_HANGUP);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Hangup HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->Hangup(requestInfo, &gsmIndex, sizeof(uint32_t));
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::Reject(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->Reject == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->Reject or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_REJECT);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Reject HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->Reject(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetClip(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetClip == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->GetClip or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_CLIP);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetClip HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetClip(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetClip(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetClip == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->SetClip or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t action = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &action)) {
        TELEPHONY_LOGE("miss action parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_CLIP);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetClip HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetClip(requestInfo, action);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetClir(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetClir == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->GetClir or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetClir miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_CLIR);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetClir HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetClir(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetClir(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetClir == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->SetClir or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t action = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SetClir miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &action)) {
        TELEPHONY_LOGE("SetClir miss action parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_CLIR);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetClir HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetClir(requestInfo, action);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetCallRestriction(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetCallRestriction == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->GetCallRestriction or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetCallRestriction miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }

    const char *fac = nullptr;
    fac = HdfSbufReadString(data);
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_CALL_RESTRICTION);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallRestriction HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetCallRestriction(requestInfo, fac);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetCallRestriction(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetCallRestriction == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->SetCallRestriction or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SetCallRestriction miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t mode = 0;
    if (!HdfSbufReadInt32(data, &mode)) {
        TELEPHONY_LOGE("SetCallRestriction miss mode parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    const char *fac = nullptr;
    const char *code = nullptr;
    fac = HdfSbufReadString(data);
    code = HdfSbufReadString(data);
    CallRestrictionInfo info = { .fac = fac, .mode = mode, .password = code };
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_CALL_RESTRICTION);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetCallRestriction HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetCallRestriction(requestInfo, info);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetBarringPassword(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetBarringPassword == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->SetBarringPassword or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SetBarringPassword miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    const char *fac = nullptr;
    const char *oldPassword = nullptr;
    const char *newPassword = nullptr;
    fac = HdfSbufReadString(data);
    if (fac == nullptr) {
        TELEPHONY_LOGE("miss fac parameter");
        return HRIL_ERR_NULL_POINT;
    }
    oldPassword = HdfSbufReadString(data);
    if (oldPassword == nullptr) {
        TELEPHONY_LOGE("miss oldPassword parameter");
        return HRIL_ERR_NULL_POINT;
    }
    newPassword = HdfSbufReadString(data);
    if (newPassword == nullptr) {
        TELEPHONY_LOGE("miss newPassword parameter");
        return HRIL_ERR_NULL_POINT;
    }
    HRilSetBarringInfo info = { .fac = fac, .oldPassword = oldPassword, .newPassword = newPassword };
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_BARRING_PASSWORD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetBarringPassword HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetBarringPassword(requestInfo, info);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::StartDtmf(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->StartDtmf == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->StartDtmf or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("StartDtmf miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t index = 1;
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("StartDtmf miss index parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CallDtmfInfo info = { 0 };
    info.callId = index;
    info.dtmfKey = HdfSbufReadString(data);
    if (info.dtmfKey == nullptr) {
        TELEPHONY_LOGE("StartDtmf miss dtmfKey parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_START_DTMF);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create StartDtmf HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->StartDtmf(requestInfo, info);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SendDtmf(struct HdfSBuf *data)
{
    if (data == nullptr) {
        TELEPHONY_LOGE("data is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    int32_t index = 1;
    int32_t onLength = 1;
    int32_t offLength = 0;
    int32_t stringLength = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SendDtmf miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("SendDtmf miss index parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &onLength)) {
        TELEPHONY_LOGE("SendDtmf miss onLength parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &offLength)) {
        TELEPHONY_LOGE("SendDtmf miss offLength parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &stringLength)) {
        TELEPHONY_LOGE("SendDtmf miss stringLength parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CallDtmfInfo info = { 0 };
    info.callId = index;
    info.onLength = onLength;
    info.offLength = offLength;
    info.stringLength = stringLength;
    info.dtmfKey = HdfSbufReadString(data);
    if (info.dtmfKey == nullptr) {
        TELEPHONY_LOGE("SendDtmf miss dtmfKey parameter");
        return HRIL_ERR_NULL_POINT;
    }
    return ProcessSendDtmf(serial, info);
}

int32_t HRilCall::ProcessSendDtmf(int32_t serial, CallDtmfInfo &info)
{
    if (callFuncs_ == nullptr || callFuncs_->SendDtmf == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->SendDtmf is nullptr!", callFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SEND_DTMF);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("HRilCall::ProcessSendDtmf failed to do Create SendDtmf HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SendDtmf(requestInfo, info);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::StopDtmf(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->StopDtmf == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->StopDtmf or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("StopDtmf miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t index = 1;
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("StopDtmf miss index parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CallDtmfInfo info = { 0 };
    info.callId = index;
    info.dtmfKey = HdfSbufReadString(data);
    if (info.dtmfKey == nullptr) {
        TELEPHONY_LOGE("StopDtmf miss dtmfKey parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_STOP_DTMF);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create StopDtmf HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->StopDtmf(requestInfo, info);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::Answer(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->Answer == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->Answer or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_ANSWER);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create Answer HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->Answer(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::HoldCall(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->HoldCall == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->HoldCall or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call HoldCall miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_HOLD_CALL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("HRilCall::HoldCall failed to do Create HoldCall HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->HoldCall(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::UnHoldCall(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->UnHoldCall == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->UnHoldCall or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call UnHoldCall miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_UNHOLD_CALL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create UnHoldCall HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->UnHoldCall(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SwitchCall(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SwitchCall == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->SwitchCall or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call SwitchCall miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SWITCH_CALL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SwitchCall Request!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SwitchCall(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::CombineConference(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->CombineConference == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->CombineConference or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    uint32_t serial = 0;
    uint32_t callType = 0;
    if (!HdfSbufReadUint32(data, &serial)) {
        TELEPHONY_LOGE("hril call CombineConference miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadUint32(data, &callType)) {
        TELEPHONY_LOGE("hril call CombineConference miss callType parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_COMBINE_CONFERENCE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create CombineConference Request!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->CombineConference(requestInfo, callType);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SeparateConference(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SeparateConference == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->SeparateConference or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    int32_t callIndex = 0;
    int32_t callType = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call SeparateConference miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &callIndex)) {
        TELEPHONY_LOGE("hril call SeparateConference miss callIndex parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &callType)) {
        TELEPHONY_LOGE("hril call SeparateConference miss callType parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SEPARATE_CONFERENCE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SeparateConference Request!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SeparateConference(requestInfo, callIndex, callType);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::CallSupplement(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->CallSupplement == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->CallSupplement or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    int32_t type = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("hril call CallSupplement miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &type)) {
        TELEPHONY_LOGE("hril call CallSupplement miss type parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_CALL_SUPPLEMENT);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create CallSupplement Request!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->CallSupplement(requestInfo, type);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetCallWaiting(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetCallWaiting == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->GetCallWaiting or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_CALL_WAITING);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallWaiting HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetCallWaiting(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetCallWaiting(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetCallWaiting == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->SetCallWaiting or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    int32_t operating = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SetCallWaiting >>> miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &operating)) {
        TELEPHONY_LOGE("SetCallWaiting >> miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_CALL_WAITING);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetCallWaiting HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetCallWaiting(requestInfo, operating);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetCallTransferInfo(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetCallTransferInfo == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->GetCallTransferInfo or data:%{public}p is nullptr!",
            callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    int32_t reason = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetCallTransferInfo >>> miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &reason)) {
        TELEPHONY_LOGE("GetCallTransferInfo >> miss reason parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_CALL_TRANSFER_INFO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallTransferInfo HRilRequest !");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetCallTransferInfo(requestInfo, reason);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetCallTransferInfo(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetCallTransferInfo == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->SetCallTransferInfo or data:%{public}p is nullptr!",
            callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    CallForwardSetInfo callForwardSetInfo = CallForwardSetInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("HRilCall::SetCallTransferInfo parcel is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!callForwardSetInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilCFInfo cFInfo = {};
    cFInfo.number = StringToCString(callForwardSetInfo.number);
    cFInfo.reason = callForwardSetInfo.reason;
    cFInfo.mode = callForwardSetInfo.mode;
    cFInfo.classx = callForwardSetInfo.classx;
    ReqDataInfo *requestInfo = CreateHRilRequest(callForwardSetInfo.serial, HREQ_CALL_SET_CALL_TRANSFER_INFO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetCallTransferInfo HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetCallTransferInfo(requestInfo, cFInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetImsCallList(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetImsCallList == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->GetImsCallList or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_IMS_CALL_LIST);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetCallList HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetImsCallList(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetCallPreferenceMode(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetCallPreferenceMode == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->GetCallPreferenceMode or data:%{public}p is nullptr!",
            callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_CALL_PREFERENCE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetCallPreferenceMode(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetCallPreferenceMode(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetCallPreferenceMode == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->SetCallPreferenceMode or data:%{public}p is nullptr!",
            callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t mode = HRIL_CALL_MODE_CS_1ST_PS_2ND;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &mode)) {
        TELEPHONY_LOGE("miss mode parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_CALL_PREFERENCE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetCallPreferenceMode(requestInfo, mode);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetLteImsSwitchStatus(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetLteImsSwitchStatus == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->GetLteImsSwitchStatus or data:%{public}p is nullptr!",
            callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_LTEIMSSWITCH_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetLteImsSwitchStatus(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetLteImsSwitchStatus(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetLteImsSwitchStatus == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->SetLteImsSwitchStatus or data:%{public}p is nullptr!",
            callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t active = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &active)) {
        TELEPHONY_LOGE("miss mode parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_LTEIMSSWITCH_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetLteImsSwitchStatus(requestInfo, active);
    return HRIL_ERR_SUCCESS;
}

void HRilCall::BuildCallList(
    CallInfoList &callInfoList, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    size_t num = responseLen / sizeof(HRilCallInfo);
    CallInfo callInfo;
    callInfoList.callSize = num;
    for (size_t i = 0; i < num; i++) {
        HRilCallInfo *curPtr = ((HRilCallInfo *)response + i);
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

int32_t HRilCall::GetCallListResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCallInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CallInfoList callList = {};
    if (response != nullptr) {
        BuildCallList(callList, responseInfo, response, responseLen);
    }

    return ResponseMessageParcel(responseInfo, callList, requestNum);
}

int32_t HRilCall::DialResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::HangupResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::RejectResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::AnswerResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::HoldCallResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetClipResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilGetClipResult)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    GetClipResult getClipResult = {};
    getClipResult.result = static_cast<int32_t>(responseInfo.error);
    if (response != nullptr) {
        const HRilGetClipResult *pGetClip = static_cast<const HRilGetClipResult *>(response);
        getClipResult.action = pGetClip->action;
        getClipResult.clipStat = pGetClip->clipStat;
    }
    return ResponseMessageParcel(responseInfo, getClipResult, requestNum);
}

int32_t HRilCall::SetClipResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}
int32_t HRilCall::UnHoldCallResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SwitchCallResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::CombineConferenceResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SeparateConferenceResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::CallSupplementResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetCallWaitingResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCallWaitResult)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CallWaitResult callWaitResult = {};
    callWaitResult.result = static_cast<int32_t>(responseInfo.error);
    if (response != nullptr) {
        const HRilCallWaitResult *result = static_cast<const HRilCallWaitResult *>(response);
        callWaitResult.status = result->status;
        callWaitResult.classCw = result->classCw;
    }
    return ResponseMessageParcel(responseInfo, callWaitResult, requestNum);
}

int32_t HRilCall::SetCallWaitingResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetCallTransferInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCFQueryInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CallForwardQueryInfoList cFQueryList = {};
    if (response != nullptr) {
        BuildCallForwardQueryInfoList(cFQueryList, responseInfo, response, responseLen);
    }

    return ResponseMessageParcel(responseInfo, cFQueryList, requestNum);
}

void HRilCall::BuildCallForwardQueryInfoList(CallForwardQueryInfoList &callForwardQueryInfoList,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    size_t num = responseLen / sizeof(HRilCFQueryInfo);
    CallForwardQueryResult cFQueryResult;
    callForwardQueryInfoList.callSize = static_cast<int32_t>(num);
    for (size_t i = 0; i < num; i++) {
        HRilCFQueryInfo *curPtr = ((HRilCFQueryInfo *)response + i);
        if (curPtr != nullptr) {
            cFQueryResult.result = static_cast<int32_t>(responseInfo.error);
            cFQueryResult.serial = responseInfo.serial;
            cFQueryResult.status = curPtr->status;
            cFQueryResult.classx = curPtr->classx;
            cFQueryResult.type = curPtr->type;
            cFQueryResult.number = ((curPtr->number == nullptr) ? "" : curPtr->number);
            cFQueryResult.reason = curPtr->reason;
            cFQueryResult.time = curPtr->time;
            callForwardQueryInfoList.calls.push_back(cFQueryResult);
        } else {
            TELEPHONY_LOGE("BuildCallForwardQueryInfoList: Invalid curPtr");
            break;
        }
    }
}

int32_t HRilCall::SetCallTransferInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetClirResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilGetCallClirResult)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    GetClirResult getClirResult = {};
    getClirResult.result = static_cast<int32_t>(responseInfo.error);
    if (response != nullptr) {
        const HRilGetCallClirResult *pGetClir = static_cast<const HRilGetCallClirResult *>(response);
        getClirResult.action = pGetClir->action;
        getClirResult.clirStat = pGetClir->clirStat;
    }
    return ResponseMessageParcel(responseInfo, getClirResult, requestNum);
}

int32_t HRilCall::SetClirResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetCallRestrictionResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCallRestrictionResult)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CallRestrictionResult resultT = {};
    resultT.result = static_cast<int32_t>(responseInfo.error);
    if (response != nullptr) {
        const HRilCallRestrictionResult *result = static_cast<const HRilCallRestrictionResult *>(response);
        resultT.status = result->status;
        resultT.classCw = result->classCw;
    }
    return ResponseMessageParcel(responseInfo, resultT, requestNum);
}

int32_t HRilCall::SetCallRestrictionResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SetBarringPasswordResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::StartDtmfResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SendDtmfResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::StopDtmfResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetImsCallListResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCallInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CallInfoList callList = {};
    if (response != nullptr) {
        BuildCallList(callList, responseInfo, response, responseLen);
    }
    return ResponseMessageParcel(responseInfo, callList, requestNum);
}

int32_t HRilCall::GetCallPreferenceModeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(int32_t)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t mode = 0;
    if (response != nullptr) {
        mode = *((int32_t *)response);
    }
    return ResponseInt32(requestNum, &responseInfo, sizeof(responseInfo), mode);
}

int32_t HRilCall::SetCallPreferenceModeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetLteImsSwitchStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(int32_t)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t active = 0;
    if (response != nullptr) {
        active = *((int32_t *)response);
    }
    return ResponseInt32(requestNum, &responseInfo, sizeof(responseInfo), active);
}

int32_t HRilCall::SetLteImsSwitchStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::SetUssd(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetUssd == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->SetUssd or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    const char *str = NULL;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    str = HdfSbufReadString(data);
    if (str == NULL) {
        TELEPHONY_LOGE("miss str parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_USSD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetUssd(requestInfo, str);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetUssd(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetUssd == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->GetUssd or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_USSD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetUssd(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetUssdResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetMute(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetMute == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->GetMute or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_MUTE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetMute(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetMuteResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(int32_t)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t mute = 0;
    if (response != nullptr) {
        mute = *((int32_t *)response);
    }
    return ResponseInt32(requestNum, &responseInfo, sizeof(responseInfo), mute);
}

int32_t HRilCall::SetMute(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetMute == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->SetMute or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t mute = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &mute)) {
        TELEPHONY_LOGE("miss mute parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_MUTE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetMute(requestInfo, mute);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::SetMuteResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::GetUssdResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(int32_t)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t cusd = 0;
    if (response != nullptr) {
        cusd = *((int32_t *)response);
    }
    return ResponseInt32(requestNum, &responseInfo, sizeof(responseInfo), cusd);
}

int32_t HRilCall::GetCallFailReason(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetCallFailReason == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "callFuncs_:%{public}p or callFuncs_->GetCallFailReason or data:%{public}p is nullptr!", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_FAIL_REASON);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        SafeFrees(requestInfo);
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetCallFailReason(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::GetCallFailReasonResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(int32_t)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t callFail = HRIL_ERR_CALL_CAUSE;
    if (response != nullptr) {
        callFail = *((int32_t *)response);
    }
    return ResponseInt32(requestNum, &responseInfo, sizeof(responseInfo), callFail);
}

int32_t HRilCall::GetEmergencyCallList(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->GetEmergencyCallList == nullptr || data == nullptr) {
        TELEPHONY_LOGE("callFuncs_:%{public}p or callFuncs_->GetEmergencyCallList or data:%{public}p is nullptr!",
            callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_GET_EMERGENCY_LIST);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        SafeFrees(requestInfo);
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->GetEmergencyCallList(requestInfo);
    return HRIL_ERR_SUCCESS;
}

void HRilCall::BuildEmergencyCallList(
    EmergencyInfoList &emergencyCallInfoList, const void *response, size_t responseLen)
{
    size_t num = responseLen / sizeof(HRilEmergencyInfo);
    EmergencyInfo callInfo;
    emergencyCallInfoList.callSize = num;
    for (size_t i = 0; i < num; i++) {
        HRilEmergencyInfo *curPtr = ((HRilEmergencyInfo *)response + i);
        if (curPtr != nullptr) {
            callInfo.index = curPtr->index;
            callInfo.total = curPtr->total;
            callInfo.eccNum = curPtr->eccNum;
            callInfo.category = curPtr->category;
            callInfo.simpresent = curPtr->simpresent;
            callInfo.mcc = curPtr->mcc;
            callInfo.abnormalService = curPtr->abnormalService;
            emergencyCallInfoList.calls.push_back(callInfo);
        } else {
            TELEPHONY_LOGE("BuildEmergencyCallList: Invalid curPtr");
            break;
        }
    }
}

int32_t HRilCall::GetEmergencyCallListResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilEmergencyInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    EmergencyInfoList callList = {};
    if (response != nullptr) {
        BuildEmergencyCallList(callList, response, responseLen);
    }

    return ResponseMessageParcel(responseInfo, callList, requestNum);
}

int32_t HRilCall::SetEmergencyCallList(struct HdfSBuf *data)
{
    if (callFuncs_ == nullptr || callFuncs_->SetEmergencyCallList == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "SetEmergencyCallList callFuncs %{public}p or SetFunc or data:%{public}p null", callFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("SetEmergencyCallList RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE(":SetEmergencyCallList parcel in SetEmergencyCallList is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    EmergencyInfoList emergencyInfoList;
    if (!emergencyInfoList.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE(":SetEmergencyCallList RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int size = emergencyInfoList.calls.size();
    if (size <= 0) {
        TELEPHONY_LOGE("SetEmergencyCallList RilAdapter failed to do ReadFromParcel! calls len 0");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilEmergencyInfo emergencyInfoCalls[size];
    int32_t serial = emergencyInfoList.flag;
    CopyToHRilEmergencyInfoArray(emergencyInfoCalls, emergencyInfoList.calls);
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_CALL_SET_EMERGENCY_LIST);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("SetEmergencyCallList RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    callFuncs_->SetEmergencyCallList(requestInfo, emergencyInfoCalls, size);
    return HRIL_ERR_SUCCESS;
}

void HRilCall::CopyToHRilEmergencyInfoArray(HRilEmergencyInfo *emergencyInfoCalls, std::vector<EmergencyInfo> calls)
{
    for (unsigned int i = 0; i < calls.size(); i++) {
        auto call = calls.at(i);
        emergencyInfoCalls[i].index = call.index;
        emergencyInfoCalls[i].total = call.total;
        char *eccNum = new char[call.eccNum.size() + 1];
        if (strcpy_s(eccNum, call.eccNum.size() + 1, call.eccNum.c_str()) == EOK) {
            emergencyInfoCalls[i].eccNum = eccNum;
        } else {
            delete[] eccNum;
        }
        emergencyInfoCalls[i].category = call.category;
        emergencyInfoCalls[i].simpresent = call.simpresent;
        char *mcc = new char[call.mcc.size() + 1];
        if (strcpy_s(mcc, call.mcc.size() + 1, call.mcc.c_str()) == EOK) {
            emergencyInfoCalls[i].mcc = mcc;
        } else {
            delete[] mcc;
        }
        emergencyInfoCalls[i].abnormalService = call.abnormalService;
    }
}

int32_t HRilCall::SetEmergencyCallListResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilCall::CallStateUpdated(int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is null.");
        return HRIL_ERR_GENERIC_FAILURE;
    }

    HRilResponseHeadInfo headInfo = { 0 };
    headInfo.slotId = GetSlotId();
    headInfo.type = (HRilResponseTypes)notifyType;
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&headInfo, sizeof(HRilResponseHeadInfo))) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_CALL_STATE_UPDATED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSbufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilCall::CallImsServiceStatusNotice(
    int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilImsServiceStatus)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CallImsServiceStatus imsServiceStatusInfo = {};
    const HRilImsServiceStatus *hImsServiceStatusInfo = reinterpret_cast<const HRilImsServiceStatus *>(response);
    imsServiceStatusInfo.smsSrvStatus = hImsServiceStatusInfo->smsSrvStatus;
    imsServiceStatusInfo.smsSrvRat = hImsServiceStatusInfo->smsSrvRat;
    imsServiceStatusInfo.voipSrvStatus = hImsServiceStatusInfo->voipSrvStatus;
    imsServiceStatusInfo.voipSrvRat = hImsServiceStatusInfo->voipSrvRat;
    imsServiceStatusInfo.vtSrvStatus = hImsServiceStatusInfo->vtSrvStatus;
    imsServiceStatusInfo.vtSrvRat = hImsServiceStatusInfo->vtSrvRat;
    imsServiceStatusInfo.vsSrvStatus = hImsServiceStatusInfo->vsSrvStatus;
    imsServiceStatusInfo.vsSrvRat = hImsServiceStatusInfo->vsSrvRat;
    return NotifyMessageParcel(notifyType, imsServiceStatusInfo, HNOTI_CALL_IMS_SERVICE_STATUS_REPORT);
}

int32_t HRilCall::CallUssdNotice(int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilUssdNoticeInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    UssdNoticeInfo ussdNoticeInfo = {};
    const HRilUssdNoticeInfo *hUssdNoticeInfo = reinterpret_cast<const HRilUssdNoticeInfo *>(response);
    ussdNoticeInfo.m = hUssdNoticeInfo->m;
    ussdNoticeInfo.str = hUssdNoticeInfo->str == nullptr ? "" : hUssdNoticeInfo->str;
    ussdNoticeInfo.dcs = hUssdNoticeInfo->dcs;
    return NotifyMessageParcel(notifyType, ussdNoticeInfo, HNOTI_CALL_USSD_REPORT);
}

int32_t HRilCall::CallSsNotice(int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilSsNoticeInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    SsNoticeInfo ssNoticeInfo = {};
    const HRilSsNoticeInfo *hSsNoticeInfo = reinterpret_cast<const HRilSsNoticeInfo *>(response);
    ssNoticeInfo.serviceType = hSsNoticeInfo->serviceType;
    ssNoticeInfo.requestType = hSsNoticeInfo->requestType;
    ssNoticeInfo.serviceClass = hSsNoticeInfo->serviceClass;
    ssNoticeInfo.result = hSsNoticeInfo->result;
    return NotifyMessageParcel(notifyType, ssNoticeInfo, HNOTI_CALL_SS_REPORT);
}

int32_t HRilCall::CallSrvccStatusNotice(int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilCallSrvccStatus)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    SrvccStatus srvccStatus = {};
    const HRilCallSrvccStatus *hSrvccStatus = reinterpret_cast<const HRilCallSrvccStatus *>(response);
    srvccStatus.status = hSrvccStatus->status;
    return NotifyMessageParcel(notifyType, srvccStatus, HNOTI_CALL_SRVCC_STATUS_REPORT);
}

int32_t HRilCall::CallRingbackVoiceNotice(int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(int32_t)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    RingbackVoice ringbackVoice = {};
    const int32_t *ringbackVoiceFlag = reinterpret_cast<const int32_t *>(response);
    ringbackVoice.status = *ringbackVoiceFlag;
    return NotifyMessageParcel(notifyType, ringbackVoice, HNOTI_CALL_RINGBACK_VOICE_REPORT);
}

int32_t HRilCall::CallEmergencyNotice(
    int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    TELEPHONY_LOGI("CallEmergencyNotice");
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilEmergencyInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    EmergencyInfoList callList = {};
    if (response != nullptr) {
        BuildEmergencyCallList(callList, response, responseLen);
    }
    return NotifyMessageParcel(notifyType, callList, HNOTI_CALL_EMERGENCY_NUMBER_REPORT);
}

void HRilCall::RegisterCallFuncs(const HRilCallReq *callFuncs)
{
    callFuncs_ = callFuncs;
}
} // namespace Telephony
} // namespace OHOS
