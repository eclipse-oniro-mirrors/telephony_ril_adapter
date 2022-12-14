/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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
    notiMemberFuncMap_[HNOTI_CALL_USSD_REPORT] = &HRilCall::CallUssdNotice;
    notiMemberFuncMap_[HNOTI_CALL_SRVCC_STATUS_REPORT] = &HRilCall::CallSrvccStatusNotice;
    notiMemberFuncMap_[HNOTI_CALL_RINGBACK_VOICE_REPORT] = &HRilCall::CallRingbackVoiceNotice;
    notiMemberFuncMap_[HNOTI_CALL_EMERGENCY_NUMBER_REPORT] = &HRilCall::CallEmergencyNotice;
    notiMemberFuncMap_[HNOTI_CALL_SS_REPORT] = &HRilCall::CallSsNotice;
    notiMemberFuncMap_[HNOTI_CALL_RSRVCC_STATUS_REPORT] = &HRilCall::CallRsrvccStatusNotify;
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
    respMemberFuncMap_[HREQ_CALL_GET_CALL_PREFERENCE] = &HRilCall::GetCallPreferenceModeResponse;
    respMemberFuncMap_[HREQ_CALL_SET_CALL_PREFERENCE] = &HRilCall::SetCallPreferenceModeResponse;
    respMemberFuncMap_[HREQ_CALL_SET_USSD] = &HRilCall::SetUssdResponse;
    respMemberFuncMap_[HREQ_CALL_GET_USSD] = &HRilCall::GetUssdResponse;
    respMemberFuncMap_[HREQ_CALL_SET_MUTE] = &HRilCall::SetMuteResponse;
    respMemberFuncMap_[HREQ_CALL_GET_MUTE] = &HRilCall::GetMuteResponse;
    respMemberFuncMap_[HREQ_CALL_GET_EMERGENCY_LIST] = &HRilCall::GetEmergencyCallListResponse;
    respMemberFuncMap_[HREQ_CALL_SET_EMERGENCY_LIST] = &HRilCall::SetEmergencyCallListResponse;
    respMemberFuncMap_[HREQ_CALL_GET_FAIL_REASON] = &HRilCall::GetCallFailReasonResponse;
    respMemberFuncMap_[HREQ_CALL_SET_BARRING_PASSWORD] = &HRilCall::SetBarringPasswordResponse;
}

int32_t HRilCall::GetCallList(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_GET_CALL_LIST, callFuncs_, &HRilCallReq::GetCallList);
}

int32_t HRilCall::Dial(int32_t serialId, const OHOS::HDI::Ril::V1_0::DialInfo &dialInfo)
{
    HRilDial dial = {};
    dial.address = StringToCString(dialInfo.address);
    dial.clir = dialInfo.clir;
    return RequestVendor(serialId, HREQ_CALL_DIAL, callFuncs_, &HRilCallReq::Dial, &dial, sizeof(HRilDial));
}

int32_t HRilCall::Hangup(int32_t serialId, int32_t gsmIndex)
{
    uint32_t data = gsmIndex;
    return RequestVendor(serialId, HREQ_CALL_HANGUP, callFuncs_, &HRilCallReq::Hangup, &data, sizeof(uint32_t));
}

int32_t HRilCall::Reject(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_REJECT, callFuncs_, &HRilCallReq::Reject);
}

int32_t HRilCall::Answer(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_ANSWER, callFuncs_, &HRilCallReq::Answer);
}

int32_t HRilCall::HoldCall(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_HOLD_CALL, callFuncs_, &HRilCallReq::HoldCall);
}

int32_t HRilCall::UnHoldCall(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_UNHOLD_CALL, callFuncs_, &HRilCallReq::UnHoldCall);
}

int32_t HRilCall::SwitchCall(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_SWITCH_CALL, callFuncs_, &HRilCallReq::SwitchCall);
}

int32_t HRilCall::CombineConference(int32_t serialId, int32_t callType)
{
    return RequestVendor(
        serialId, HREQ_CALL_COMBINE_CONFERENCE, callFuncs_, &HRilCallReq::CombineConference, callType);
}

int32_t HRilCall::SeparateConference(int32_t serialId, int32_t callIndex, int32_t callType)
{
    return RequestVendor(
        serialId, HREQ_CALL_SEPARATE_CONFERENCE, callFuncs_, &HRilCallReq::SeparateConference, callIndex, callType);
}

int32_t HRilCall::CallSupplement(int32_t serialId, int32_t type)
{
    return RequestVendor(serialId, HREQ_CALL_CALL_SUPPLEMENT, callFuncs_, &HRilCallReq::CallSupplement, type);
}

int32_t HRilCall::GetClip(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_GET_CLIP, callFuncs_, &HRilCallReq::GetClip);
}

int32_t HRilCall::SetClip(int32_t serialId, int32_t action)
{
    return RequestVendor(serialId, HREQ_CALL_SET_CLIP, callFuncs_, &HRilCallReq::SetClip, action);
}

int32_t HRilCall::GetClir(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_GET_CLIR, callFuncs_, &HRilCallReq::GetClir);
}

int32_t HRilCall::SetClir(int32_t serialId, int32_t action)
{
    return RequestVendor(serialId, HREQ_CALL_SET_CLIR, callFuncs_, &HRilCallReq::SetClir, action);
}

int32_t HRilCall::GetCallRestriction(int32_t serialId, const std::string &fac)
{
    return RequestVendor(
        serialId, HREQ_CALL_GET_CALL_RESTRICTION, callFuncs_, &HRilCallReq::GetCallRestriction, StringToCString(fac));
}

int32_t HRilCall::SetCallRestriction(
    int32_t serialId, const OHOS::HDI::Ril::V1_0::CallRestrictionInfo &callRestrictionInfo)
{
    CallRestrictionInfo info = {};
    info.fac = StringToCString(callRestrictionInfo.fac);
    info.mode = callRestrictionInfo.mode;
    info.password = StringToCString(callRestrictionInfo.password);
    return RequestVendor(serialId, HREQ_CALL_SET_CALL_RESTRICTION, callFuncs_, &HRilCallReq::SetCallRestriction, info);
}

int32_t HRilCall::GetCallWaiting(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_GET_CALL_WAITING, callFuncs_, &HRilCallReq::GetCallWaiting);
}

int32_t HRilCall::SetCallWaiting(int32_t serialId, int32_t activate)
{
    return RequestVendor(serialId, HREQ_CALL_SET_CALL_WAITING, callFuncs_, &HRilCallReq::SetCallWaiting, activate);
}

int32_t HRilCall::GetCallTransferInfo(int32_t serialId, int32_t reason)
{
    return RequestVendor(
        serialId, HREQ_CALL_GET_CALL_TRANSFER_INFO, callFuncs_, &HRilCallReq::GetCallTransferInfo, reason);
}

int32_t HRilCall::SetCallTransferInfo(
    int32_t serialId, const OHOS::HDI::Ril::V1_0::CallForwardSetInfo &callForwardSetInfo)
{
    HRilCFInfo cFInfo = {};
    cFInfo.number = StringToCString(callForwardSetInfo.number);
    cFInfo.reason = callForwardSetInfo.reason;
    cFInfo.mode = callForwardSetInfo.mode;
    cFInfo.classx = callForwardSetInfo.classx;
    return RequestVendor(
        serialId, HREQ_CALL_SET_CALL_TRANSFER_INFO, callFuncs_, &HRilCallReq::SetCallTransferInfo, cFInfo);
}

int32_t HRilCall::GetCallPreferenceMode(int32_t serialId)
{
    return RequestVendor(
        serialId, HREQ_CALL_GET_CALL_PREFERENCE, callFuncs_, &HRilCallReq::GetCallPreferenceMode);
}

int32_t HRilCall::SetCallPreferenceMode(int32_t serialId, int32_t mode)
{
    return RequestVendor(
        serialId, HREQ_CALL_SET_CALL_PREFERENCE, callFuncs_, &HRilCallReq::SetCallPreferenceMode, mode);
}

int32_t HRilCall::SetUssd(int32_t serialId, const std::string &str)
{
    return RequestVendor(
        serialId, HREQ_CALL_SET_USSD, callFuncs_, &HRilCallReq::SetUssd, StringToCString(str));
}

int32_t HRilCall::GetUssd(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_GET_USSD, callFuncs_, &HRilCallReq::GetUssd);
}

int32_t HRilCall::SetMute(int32_t serialId, int32_t mute)
{
    return RequestVendor(serialId, HREQ_CALL_SET_MUTE, callFuncs_, &HRilCallReq::SetMute, mute);
}

int32_t HRilCall::GetMute(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_GET_MUTE, callFuncs_, &HRilCallReq::GetMute);
}

int32_t HRilCall::GetCallFailReason(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_GET_FAIL_REASON, callFuncs_, &HRilCallReq::GetCallFailReason);
}

int32_t HRilCall::GetEmergencyCallList(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_CALL_GET_EMERGENCY_LIST, callFuncs_, &HRilCallReq::GetEmergencyCallList);
}

int32_t HRilCall::SetBarringPassword(int32_t serialId, const OHOS::HDI::Ril::V1_0::SetBarringInfo &setBarringInfo)
{
    HRilSetBarringInfo info = {};
    info.fac = StringToCString(setBarringInfo.fac);
    info.oldPassword = StringToCString(setBarringInfo.oldPassword);
    info.newPassword = StringToCString(setBarringInfo.newPassword);
    return RequestVendor(
        serialId, HREQ_CALL_SET_BARRING_PASSWORD, callFuncs_, &HRilCallReq::SetBarringPassword, info);
}

int32_t HRilCall::StartDtmf(int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo)
{
    CallDtmfInfo info = {};
    info.callId = dtmfInfo.callId;
    info.dtmfKey = StringToCString(dtmfInfo.dtmfKey);
    return RequestVendor(serialId, HREQ_CALL_START_DTMF, callFuncs_, &HRilCallReq::StartDtmf, info);
}

int32_t HRilCall::SendDtmf(int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo)
{
    CallDtmfInfo info = {};
    info.callId = dtmfInfo.callId;
    info.dtmfKey = StringToCString(dtmfInfo.dtmfKey);
    info.onLength = dtmfInfo.onLength;
    info.offLength = dtmfInfo.offLength;
    info.stringLength = dtmfInfo.stringLength;
    return RequestVendor(serialId, HREQ_CALL_SEND_DTMF, callFuncs_, &HRilCallReq::SendDtmf, info);
}

int32_t HRilCall::StopDtmf(int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo)
{
    CallDtmfInfo info = {};
    info.callId = dtmfInfo.callId;
    info.dtmfKey = StringToCString(dtmfInfo.dtmfKey);
    return RequestVendor(serialId, HREQ_CALL_STOP_DTMF, callFuncs_, &HRilCallReq::StopDtmf, info);
}

void HRilCall::BuildICallList(
    HDI::Ril::V1_0::CallInfoList &callInfoList, const void *response, size_t responseLen)
{
    size_t num = responseLen / sizeof(HRilCallInfo);
    HDI::Ril::V1_0::CallInfo callInfo;
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
    HDI::Ril::V1_0::CallInfoList callList = {};
    if (response != nullptr) {
        BuildICallList(callList, response, responseLen);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCallListResponse, callList);
}

int32_t HRilCall::DialResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::DialResponse);
}

int32_t HRilCall::HangupResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::HangupResponse);
}

int32_t HRilCall::RejectResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::RejectResponse);
}

int32_t HRilCall::AnswerResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::AnswerResponse);
}

int32_t HRilCall::HoldCallResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::HoldCallResponse);
}

int32_t HRilCall::GetClipResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilGetClipResult)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::GetClipResult getClipResult = {};
    getClipResult.result = static_cast<int32_t>(responseInfo.error);
    if (response != nullptr) {
        const HRilGetClipResult *pGetClip = static_cast<const HRilGetClipResult *>(response);
        getClipResult.action = pGetClip->action;
        getClipResult.clipStat = pGetClip->clipStat;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetClipResponse, getClipResult);
}

int32_t HRilCall::SetClipResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetClipResponse);
}

int32_t HRilCall::UnHoldCallResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::UnHoldCallResponse);
}

int32_t HRilCall::SwitchCallResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SwitchCallResponse);
}

int32_t HRilCall::CombineConferenceResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::CombineConferenceResponse);
}

int32_t HRilCall::SeparateConferenceResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SeparateConferenceResponse);
}

int32_t HRilCall::CallSupplementResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::CallSupplementResponse);
}

int32_t HRilCall::GetCallWaitingResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCallWaitResult)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::CallWaitResult callWaitResult = {};
    callWaitResult.result = static_cast<int32_t>(responseInfo.error);
    if (response != nullptr) {
        const HRilCallWaitResult *result = static_cast<const HRilCallWaitResult *>(response);
        callWaitResult.status = result->status;
        callWaitResult.classCw = result->classCw;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCallWaitingResponse, callWaitResult);
}

int32_t HRilCall::SetCallWaitingResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetCallWaitingResponse);
}

int32_t HRilCall::GetCallTransferInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCFQueryInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::CallForwardQueryInfoList cFQueryList = {};
    if (response != nullptr) {
        BuildICallForwardQueryInfoList(cFQueryList, responseInfo, response, responseLen);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCallTransferInfoResponse, cFQueryList);
}

void HRilCall::BuildICallForwardQueryInfoList(HDI::Ril::V1_0::CallForwardQueryInfoList &cFQueryList,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    size_t num = responseLen / sizeof(HRilCFQueryInfo);
    HDI::Ril::V1_0::CallForwardQueryResult cFQueryResult;
    cFQueryList.callSize = num;
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
            cFQueryList.calls.push_back(cFQueryResult);
        } else {
            TELEPHONY_LOGE("BuildICallForwardQueryInfoList: Invalid curPtr");
            break;
        }
    }
}

int32_t HRilCall::SetCallTransferInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetCallTransferInfoResponse);
}

int32_t HRilCall::GetClirResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilGetCallClirResult)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::GetClirResult getClirResult = {};
    getClirResult.result = static_cast<int32_t>(responseInfo.error);
    if (response != nullptr) {
        const HRilGetCallClirResult *pGetClir = static_cast<const HRilGetCallClirResult *>(response);
        getClirResult.action = pGetClir->action;
        getClirResult.clirStat = pGetClir->clirStat;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetClirResponse, getClirResult);
}

int32_t HRilCall::SetClirResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetClirResponse);
}

int32_t HRilCall::GetCallRestrictionResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilCallRestrictionResult)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::CallRestrictionResult resultT = {};
    resultT.result = static_cast<int32_t>(responseInfo.error);
    if (response != nullptr) {
        const HRilCallRestrictionResult *result = static_cast<const HRilCallRestrictionResult *>(response);
        resultT.status = result->status;
        resultT.classCw = result->classCw;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCallRestrictionResponse, resultT);
}

int32_t HRilCall::SetCallRestrictionResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetCallRestrictionResponse);
}

int32_t HRilCall::SetBarringPasswordResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetBarringPasswordResponse);
}

int32_t HRilCall::StartDtmfResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::StartDtmfResponse);
}

int32_t HRilCall::SendDtmfResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SendDtmfResponse);
}

int32_t HRilCall::StopDtmfResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::StopDtmfResponse);
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
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCallPreferenceModeResponse, mode);
}

int32_t HRilCall::SetCallPreferenceModeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetCallPreferenceModeResponse);
}

int32_t HRilCall::SetUssdResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetUssdResponse);
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
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetMuteResponse, mute);
}

int32_t HRilCall::SetMuteResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetMuteResponse);
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
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetUssdResponse, cusd);
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
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCallFailReasonResponse, callFail);
}

void HRilCall::BuildIEmergencyCallList(
    HDI::Ril::V1_0::EmergencyInfoList &emergencyCallInfoList, const void *response, size_t responseLen)
{
    size_t num = responseLen / sizeof(HRilEmergencyInfo);
    HDI::Ril::V1_0::EmergencyCall callInfo;
    emergencyCallInfoList.callSize = num;
    for (size_t i = 0; i < num; i++) {
        HRilEmergencyInfo *curPtr = ((HRilEmergencyInfo *)response + i);
        if (curPtr != nullptr) {
            callInfo.index = curPtr->index;
            callInfo.total = curPtr->total;
            callInfo.eccNum = (curPtr->eccNum == nullptr) ? "" : curPtr->eccNum;
            callInfo.eccType = static_cast<OHOS::HDI::Ril::V1_0::EccType>(curPtr->category);
            callInfo.simpresent = static_cast<OHOS::HDI::Ril::V1_0::SimpresentType>(curPtr->simpresent);
            callInfo.mcc = (curPtr->mcc == nullptr) ? "" : curPtr->mcc;
            callInfo.abnormalService = static_cast<OHOS::HDI::Ril::V1_0::AbnormalServiceType>(curPtr->abnormalService);
            emergencyCallInfoList.calls.push_back(callInfo);
        } else {
            TELEPHONY_LOGE("BuildIEmergencyCallList: Invalid curPtr");
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
    HDI::Ril::V1_0::EmergencyInfoList callList = {};
    if (response != nullptr) {
        BuildIEmergencyCallList(callList, response, responseLen);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetEmergencyCallListResponse, callList);
}

int32_t HRilCall::SetEmergencyCallList(
    int32_t serialId, const OHOS::HDI::Ril::V1_0::EmergencyInfoList &emergencyInfoList)
{
    auto size = emergencyInfoList.calls.size();
    std::unique_ptr<HRilEmergencyInfo[]> emergencyInfoCalls = std::make_unique<HRilEmergencyInfo[]>(size);
    CopyToHRilEmergencyInfoArray(emergencyInfoCalls.get(), emergencyInfoList.calls);
    return RequestVendor(serialId, HREQ_CALL_SET_EMERGENCY_LIST, callFuncs_, &HRilCallReq::SetEmergencyCallList,
        emergencyInfoCalls.get(), size);
}

void HRilCall::CopyToHRilEmergencyInfoArray(
    HRilEmergencyInfo *emergencyInfoCalls, std::vector<HDI::Ril::V1_0::EmergencyCall> calls)
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
            eccNum = nullptr;
        }
        emergencyInfoCalls[i].category = static_cast<int32_t>(call.eccType);
        emergencyInfoCalls[i].simpresent = call.simpresent;
        char *mcc = new char[call.mcc.size() + 1];
        if (strcpy_s(mcc, call.mcc.size() + 1, call.mcc.c_str()) == EOK) {
            emergencyInfoCalls[i].mcc = mcc;
        } else {
            delete[] mcc;
            mcc = nullptr;
        }
        emergencyInfoCalls[i].abnormalService = call.abnormalService;
    }
}

int32_t HRilCall::SetEmergencyCallListResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetEmergencyCallListResponse);
}

int32_t HRilCall::CallStateUpdated(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::CallStateUpdated);
}

int32_t HRilCall::CallUssdNotice(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilUssdNoticeInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::UssdNoticeInfo ussdNoticeInfo = {};
    const HRilUssdNoticeInfo *hUssdNoticeInfo = reinterpret_cast<const HRilUssdNoticeInfo *>(response);
    ussdNoticeInfo.type = hUssdNoticeInfo->m;
    ussdNoticeInfo.message = hUssdNoticeInfo->str == nullptr ? "" : hUssdNoticeInfo->str;
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::CallUssdNotice, ussdNoticeInfo);
}

int32_t HRilCall::CallSsNotice(int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilSsNoticeInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::SsNoticeInfo ssNoticeInfo = {};
    const HRilSsNoticeInfo *hSsNoticeInfo = reinterpret_cast<const HRilSsNoticeInfo *>(response);
    ssNoticeInfo.serviceType = hSsNoticeInfo->serviceType;
    ssNoticeInfo.requestType = hSsNoticeInfo->requestType;
    ssNoticeInfo.serviceClass = hSsNoticeInfo->serviceClass;
    ssNoticeInfo.result = hSsNoticeInfo->result;
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::CallSsNotice, ssNoticeInfo);
}

int32_t HRilCall::CallSrvccStatusNotice(
    int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilCallSrvccStatus)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::SrvccStatus srvccStatus = {};
    const HRilCallSrvccStatus *hSrvccStatus = reinterpret_cast<const HRilCallSrvccStatus *>(response);
    srvccStatus.status = hSrvccStatus->status;
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::CallSrvccStatusNotice, srvccStatus);
}

int32_t HRilCall::CallRingbackVoiceNotice(
    int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(int32_t)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::RingbackVoice ringbackVoice = {};
    const int32_t *ringbackVoiceFlag = reinterpret_cast<const int32_t *>(response);
    ringbackVoice.status = *ringbackVoiceFlag;
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::CallRingbackVoiceNotice, ringbackVoice);
}

int32_t HRilCall::CallEmergencyNotice(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen == 0 || (responseLen % sizeof(HRilEmergencyInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::EmergencyInfoList callList = {};
    BuildIEmergencyCallList(callList, response, responseLen);
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::CallEmergencyNotice, callList);
}

int32_t HRilCall::CallRsrvccStatusNotify(
    int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen)
{
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::CallRsrvccStatusNotify);
}

void HRilCall::RegisterCallFuncs(const HRilCallReq *callFuncs)
{
    callFuncs_ = callFuncs;
}
} // namespace Telephony
} // namespace OHOS
