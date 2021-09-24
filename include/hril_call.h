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
    HRilCall();
    virtual ~HRilCall();

    void RegisterCallResponseCallback(HdfRemoteService *serviceCallback);
    void RegisterCallNotifyCallback(HdfRemoteService *serviceCallbackInd);
    void ProcessCallResponse(int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    void ProcessCallRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);
    void ProcessCallNotify(int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);

    bool IsCallResponse(uint32_t code);

    bool IsCallNotification(uint32_t code);

    bool IsCallRespOrNotify(uint32_t code);

    void BuildCallList(
        CallInfoList &callInfoList, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    void RegisterCallFuncs(const HRilCallReq *callFuncs);

private:
    void AddHandlerToMap();
    void Dial(int32_t slotId, struct HdfSBuf *data);
    void GetClip(int32_t slotId, struct HdfSBuf *data);
    void SetClip(int32_t slotId, struct HdfSBuf *data);
    void GetCallList(int32_t slotId, struct HdfSBuf *data);
    void Hangup(int32_t slotId, struct HdfSBuf *data);
    void Reject(int32_t slotId, struct HdfSBuf *data);
    void Hold(int32_t slotId, struct HdfSBuf *data);
    void Active(int32_t slotId, struct HdfSBuf *data);
    void Swap(int32_t slotId, struct HdfSBuf *data);
    void Answer(int32_t slotId, struct HdfSBuf *data);
    void Join(int32_t slotId, struct HdfSBuf *data);
    void Split(int32_t slotId, struct HdfSBuf *data);
    void CallSupplement(int32_t slotId, struct HdfSBuf *data);
    void GetCallWait(int32_t slotId, struct HdfSBuf *data);
    void SetCallWait(int32_t slotId, struct HdfSBuf *data);
    void GetCallForward(int32_t slotId, struct HdfSBuf *data);
    void SetCallForward(int32_t slotId, struct HdfSBuf *data);
    void GetCallRestriction(int32_t slotId, struct HdfSBuf *data);
    void SetCallRestriction(int32_t slotId, struct HdfSBuf *data);
    void GetClir(int32_t slotId, struct HdfSBuf *data);
    void SetClir(int32_t slotId, struct HdfSBuf *data);
    void StartDtmf(int32_t slotId, struct HdfSBuf *data);
    void SendDtmf(int32_t slotId, struct HdfSBuf *data);
    void ProcessSendDtmf(int32_t serial, int32_t slotId, CallDTMFInfo &info);
    void StopDtmf(int32_t slotId, struct HdfSBuf *data);

    int32_t GetCallListResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t DialResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t HangupResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t RejectResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t HoldResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t ActiveResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SwapResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetLastCallErrorCodeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t AnswerResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetClipResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetClipResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t JoinResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SplitResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t CallSupplementResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCallWaitResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetCallWaitResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCallForwardResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetCallForwardResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
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

    int32_t CallStateUpdated(
        int32_t slotId, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    template<typename T>
    int32_t ResponseMessageParcel(HRilRadioResponseInfo &responseInfo, T &data, int32_t requestNum)
    {
        struct HdfSBuf *dataSbuf;
        if (serviceCallback_ == nullptr) {
            return HDF_FAILURE;
        }

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

        int32_t ret = serviceCallback_->dispatcher->Dispatch(serviceCallback_, requestNum, dataSbuf, nullptr);
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
        int32_t slotId, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    using ReqFunc = void (HRilCall::*)(int32_t slotId, struct HdfSBuf *data);
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    std::map<uint32_t, ReqFunc> reqMemberFuncMap_;
    const HRilCallReq *callFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_CALL_H
