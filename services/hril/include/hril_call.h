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

#ifndef OHOS_HRIL_CALL_H
#define OHOS_HRIL_CALL_H

#include "hril_base.h"
#include "hril_call_parcel.h"

namespace OHOS {
namespace Telephony {
class HRilCall : public HRilBase {
public:
    HRilCall(int32_t slotId, IHRilReporter &hrilReporter);
    virtual ~HRilCall();

    bool IsCallRespOrNotify(uint32_t code);

    void RegisterCallFuncs(const HRilCallReq *callFuncs);

    int32_t GetCallList(int32_t serialId);
    int32_t Dial(int32_t serialId, const OHOS::HDI::Ril::V1_0::DialInfo &dialInfo);
    int32_t Reject(int32_t serialId);
    int32_t Hangup(int32_t serialId, int32_t gsmIndex);
    int32_t Answer(int32_t serialId);
    int32_t HoldCall(int32_t serialId);
    int32_t UnHoldCall(int32_t serialId);
    int32_t SwitchCall(int32_t serialId);
    int32_t CombineConference(int32_t serialId, int32_t callType);
    int32_t SeparateConference(int32_t serialId, int32_t callIndex, int32_t callType);
    int32_t GetCallWaiting(int32_t serialId);
    int32_t SetCallWaiting(int32_t serialId, int32_t activate);
    int32_t GetCallTransferInfo(int32_t serialId, int32_t reason);
    int32_t SetCallTransferInfo(int32_t serialId, const OHOS::HDI::Ril::V1_0::CallForwardSetInfo &callForwardSetInfo);
    int32_t GetCallRestriction(int32_t serialId, const std::string &fac);
    int32_t SetCallRestriction(int32_t serialId, const OHOS::HDI::Ril::V1_0::CallRestrictionInfo &callRestrictionInfo);
    int32_t GetClip(int32_t serialId);
    int32_t SetClip(int32_t serialId, int32_t action);
    int32_t GetClir(int32_t serialId);
    int32_t SetClir(int32_t serialId, int32_t action);
    int32_t GetCallPreferenceMode(int32_t serialId);
    int32_t SetCallPreferenceMode(int32_t serialId, int32_t mode);
    int32_t SetUssd(int32_t serialId, const std::string &str);
    int32_t GetUssd(int32_t serialId);
    int32_t SetMute(int32_t serialId, int32_t mute);
    int32_t GetMute(int32_t serialId);
    int32_t GetCallFailReason(int32_t serialId);
    int32_t CallSupplement(int32_t serialId, int32_t type);
    int32_t SetBarringPassword(int32_t serialId, const OHOS::HDI::Ril::V1_0::SetBarringInfo &setBarringInfo);
    int32_t StartDtmf(int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo);
    int32_t SendDtmf(int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo);
    int32_t StopDtmf(int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo);
    int32_t GetEmergencyCallList(int32_t serialId);
    int32_t SetEmergencyCallList(int32_t serialId, const OHOS::HDI::Ril::V1_0::EmergencyInfoList &emergencyInfoList);
    bool IsCallResponse(uint32_t code);
    bool IsCallNotification(uint32_t code);

    int32_t GetCallListResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t DialResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t HangupResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t RejectResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t HoldCallResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t UnHoldCallResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SwitchCallResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetLastCallErrorCodeResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t AnswerResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetClipResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetClipResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t CombineConferenceResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SeparateConferenceResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t CallSupplementResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCallWaitingResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetCallWaitingResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCallTransferInfoResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetCallTransferInfoResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCallRestrictionResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetCallRestrictionResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetClirResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetClirResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t StartDtmfResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SendDtmfResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t StopDtmfResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCallPreferenceModeResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetCallPreferenceModeResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetUssdResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetUssdResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetMuteResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetMuteResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetEmergencyCallListResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCallFailReasonResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetEmergencyCallListResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetBarringPasswordResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);

    int32_t CallStateUpdated(int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t CallUssdNotice(int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t CallRingbackVoiceNotice(int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t CallSrvccStatusNotice(int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t CallEmergencyNotice(int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t CallSsNotice(int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t CallRsrvccStatusNotify(int32_t notifyType, HRilErrNumber error, const void *response, size_t responseLen);
    void BuildICallList(
        HDI::Ril::V1_0::CallInfoList &callInfoList, const void *response, size_t responseLen);
    void BuildIEmergencyCallList(
        HDI::Ril::V1_0::EmergencyInfoList &emergencyCallInfoList, const void *response, size_t responseLen);
    void BuildICallForwardQueryInfoList(HDI::Ril::V1_0::CallForwardQueryInfoList &cFQueryList,
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);

private:
    void CopyToHRilEmergencyInfoArray(
        HRilEmergencyInfo *emergencyInfoCalls, std::vector<HDI::Ril::V1_0::EmergencyCall> calls);
    void AddCallNotificationToMap();
    void AddCallResponseToMap();

private:
    const HRilCallReq *callFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_CALL_H
