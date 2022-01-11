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

#ifndef OHOS_HRIL_CALL_H
#define OHOS_HRIL_CALL_H

#include "hril_base.h"
#include "hril_call_parcel.h"

namespace OHOS {
namespace Telephony {
class HRilCall : public HRilBase {
public:
    HRilCall(IHRilReporter &hrilReporter);
    virtual ~HRilCall();

    void RegisterCallResponseCallback(const HdfRemoteService *serviceCallback);
    void RegisterCallNotifyCallback(const HdfRemoteService *serviceCallbackInd);
    void ProcessCallResponse(
        int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    void ProcessCallRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);
    void ProcessCallNotify(int32_t slotId, const struct ReportInfo *reportInfo,
        const void *response, size_t responseLen);

    bool IsCallRespOrNotify(uint32_t code);

    void BuildCallList(
        CallInfoList &callInfoList, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    void RegisterCallFuncs(const HRilCallReq *callFuncs);

private:
    void AddCallNotificationToMap();
    void AddCallResponseToMap();
    void AddCallRequestToMap();
    void Dial(int32_t slotId, struct HdfSBuf *data);
    void GetClip(int32_t slotId, struct HdfSBuf *data);
    void SetClip(int32_t slotId, struct HdfSBuf *data);
    void GetCallList(int32_t slotId, struct HdfSBuf *data);
    void Hangup(int32_t slotId, struct HdfSBuf *data);
    void Reject(int32_t slotId, struct HdfSBuf *data);
    void HoldCall(int32_t slotId, struct HdfSBuf *data);
    void UnHoldCall(int32_t slotId, struct HdfSBuf *data);
    void SwitchCall(int32_t slotId, struct HdfSBuf *data);
    void Answer(int32_t slotId, struct HdfSBuf *data);
    void CombineConference(int32_t slotId, struct HdfSBuf *data);
    void SeparateConference(int32_t slotId, struct HdfSBuf *data);
    void CallSupplement(int32_t slotId, struct HdfSBuf *data);
    void GetCallWaiting(int32_t slotId, struct HdfSBuf *data);
    void SetCallWaiting(int32_t slotId, struct HdfSBuf *data);
    void GetCallTransferInfo(int32_t slotId, struct HdfSBuf *data);
    void SetCallTransferInfo(int32_t slotId, struct HdfSBuf *data);
    void GetCallRestriction(int32_t slotId, struct HdfSBuf *data);
    void SetCallRestriction(int32_t slotId, struct HdfSBuf *data);
    void GetClir(int32_t slotId, struct HdfSBuf *data);
    void SetClir(int32_t slotId, struct HdfSBuf *data);
    void StartDtmf(int32_t slotId, struct HdfSBuf *data);
    void SendDtmf(int32_t slotId, struct HdfSBuf *data);
    void ProcessSendDtmf(int32_t serial, int32_t slotId, CallDtmfInfo &info);
    void StopDtmf(int32_t slotId, struct HdfSBuf *data);
    bool IsCallResponse(uint32_t code);
    bool IsCallNotification(uint32_t code);
    void GetImsCallList(int32_t slotId, struct HdfSBuf *data);
    void GetCallPreferenceMode(int32_t slotId, struct HdfSBuf *data);
    void SetCallPreferenceMode(int32_t slotId, struct HdfSBuf *data);
    void GetLteImsSwitchStatus(int32_t slotId, struct HdfSBuf *data);
    void SetLteImsSwitchStatus(int32_t slotId, struct HdfSBuf *data);
    void SetUssdCusd(int32_t slotId, struct HdfSBuf *data);
    void GetUssdCusd(int32_t slotId, struct HdfSBuf *data);
    void SetMute(int32_t slotId, struct HdfSBuf *data);
    void GetMute(int32_t slotId, struct HdfSBuf *data);
    void GetEmergencyCallList(int32_t slotId, struct HdfSBuf *data);
    void GetCallFailReason(int32_t slotId, struct HdfSBuf *data);

    int32_t GetCallListResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t DialResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t HangupResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t RejectResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t HoldCallResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t UnHoldCallResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SwitchCallResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetLastCallErrorCodeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t AnswerResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetClipResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetClipResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t CombineConferenceResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SeparateConferenceResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t CallSupplementResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCallWaitingResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetCallWaitingResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCallTransferInfoResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetCallTransferInfoResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCallRestrictionResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetCallRestrictionResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetClirResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetClirResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t StartDtmfResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SendDtmfResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t StopDtmfResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetImsCallListResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCallPreferenceModeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetCallPreferenceModeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetLteImsSwitchStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetLteImsSwitchStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetUssdCusdResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetUssdCusdResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetMuteResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetMuteResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetEmergencyCallListResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCallFailReasonResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    int32_t CallStateUpdated(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallCringNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallWaitingNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallConnectNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallEndNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallStatusInfoNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallImsServiceStatusNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallUssdCusdNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallRingbackVoiceNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallSrvccStatusNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CallEmergencyNotice(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);

    void BuildEmergencyCallList(
        EmergencyInfoList &list, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);

    template<typename T>
    int32_t ResponseMessageParcel(const HRilRadioResponseInfo &responseInfo, const T &data, int32_t requestNum)
    {
        struct HdfSBuf *dataSbuf;
        std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
        if (parcel == nullptr) {
            return HDF_FAILURE;
        }
        dataSbuf = ParcelToSbuf(parcel.get());
        if (dataSbuf == nullptr) {
            return HDF_FAILURE;
        }

        if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
            HdfSBufRecycle(dataSbuf);
            return HDF_FAILURE;
        }

        data.Marshalling(*parcel.get());

        int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
        if (ret != HDF_SUCCESS) {
            HdfSBufRecycle(dataSbuf);
            return HDF_FAILURE;
        }
        if (dataSbuf != nullptr) {
            HdfSBufRecycle(dataSbuf);
        }
        return HDF_SUCCESS;
    }

private:
    using RespFunc = int32_t (HRilCall::*)(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    using NotiFunc = int32_t (HRilCall::*)(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    using ReqFunc = void (HRilCall::*)(int32_t slotId, struct HdfSBuf *data);
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    std::map<uint32_t, ReqFunc> reqMemberFuncMap_;
    const HRilCallReq *callFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_CALL_H
