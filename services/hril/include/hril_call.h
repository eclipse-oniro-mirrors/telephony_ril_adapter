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

    int32_t Dial(struct HdfSBuf *data);
    int32_t GetClip(struct HdfSBuf *data);
    int32_t SetClip(struct HdfSBuf *data);
    int32_t GetCallList(struct HdfSBuf *data);
    int32_t Hangup(struct HdfSBuf *data);
    int32_t Reject(struct HdfSBuf *data);
    int32_t HoldCall(struct HdfSBuf *data);
    int32_t UnHoldCall(struct HdfSBuf *data);
    int32_t SwitchCall(struct HdfSBuf *data);
    int32_t Answer(struct HdfSBuf *data);
    int32_t CombineConference(struct HdfSBuf *data);
    int32_t SeparateConference(struct HdfSBuf *data);
    int32_t CallSupplement(struct HdfSBuf *data);
    int32_t GetCallWaiting(struct HdfSBuf *data);
    int32_t SetCallWaiting(struct HdfSBuf *data);
    int32_t GetCallTransferInfo(struct HdfSBuf *data);
    int32_t SetCallTransferInfo(struct HdfSBuf *data);
    int32_t GetCallRestriction(struct HdfSBuf *data);
    int32_t SetCallRestriction(struct HdfSBuf *data);
    int32_t GetClir(struct HdfSBuf *data);
    int32_t SetClir(struct HdfSBuf *data);
    int32_t StartDtmf(struct HdfSBuf *data);
    int32_t SendDtmf(struct HdfSBuf *data);
    int32_t ProcessSendDtmf(int32_t serial, CallDtmfInfo &info);
    int32_t StopDtmf(struct HdfSBuf *data);
    bool IsCallResponse(uint32_t code);
    bool IsCallNotification(uint32_t code);
    int32_t GetCallPreferenceMode(struct HdfSBuf *data);
    int32_t SetCallPreferenceMode(struct HdfSBuf *data);
    int32_t SetUssd(struct HdfSBuf *data);
    int32_t GetUssd(struct HdfSBuf *data);
    int32_t SetMute(struct HdfSBuf *data);
    int32_t GetMute(struct HdfSBuf *data);
    int32_t GetEmergencyCallList(int32_t serialId);
    int32_t GetCallFailReason(struct HdfSBuf *data);
    int32_t SetEmergencyCallList(int32_t serialId, const OHOS::HDI::Ril::V1_0::IEmergencyInfoList &emergencyInfoList);
    int32_t SetBarringPassword(struct HdfSBuf *data);

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

    int32_t CallStateUpdated(int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallUssdNotice(int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallRingbackVoiceNotice(int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallSrvccStatusNotice(int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallEmergencyNotice(int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallSsNotice(int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);

    void BuildCallList(
        CallInfoList &callInfoList, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    void BuildIEmergencyCallList(
        HDI::Ril::V1_0::IEmergencyInfoList &emergencyCallInfoList, const void *response, size_t responseLen);
    void BuildCallForwardQueryInfoList(CallForwardQueryInfoList &callForwardQueryInfoList,
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetBarringPasswordResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);

private:
    void CopyToHRilEmergencyInfoArray(
        HRilEmergencyInfo *emergencyInfoCalls, std::vector<OHOS::HDI::Ril::V1_0::IEmergencyCall> calls);
    void AddCallNotificationToMap();
    void AddCallResponseToMap();
    void AddCallRequestToMap();

private:
    const HRilCallReq *callFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_CALL_H
