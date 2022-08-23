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

#ifndef OHOS_HRIL_BASE_H
#define OHOS_HRIL_BASE_H

#include <any>
#include <cstdlib>
#include <map>
#include <securec.h>

#include "hdf_remote_service.h"
#include "hdf_sbuf_ipc.h"
#include "hril_types.h"
#include "telephony_log_wrapper.h"
#include "v1_0/iril_callback.h"

namespace OHOS {
namespace Telephony {
class IHRilReporter {
public:
    virtual int32_t ReportToParent(int32_t requestNum, const HdfSBuf *dataSbuf) = 0;
    virtual int32_t NotifyToParent(int32_t requestNum, const HdfSBuf *dataSbuf) = 0;
    virtual ReqDataInfo *CreateHRilRequest(int32_t serial, int32_t slotId, int32_t request) = 0;
    virtual void ReleaseHRilRequest(int32_t request, ReqDataInfo *requestInfo) = 0;
};

class HRilBase {
public:
    // The "reply" event processing entry.
    template<typename T>
    int32_t ProcessResponse(
        int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    // The "Active reporting" event processing entry.
    template<typename T>
    int32_t ProcessNotify(
        int32_t notifyType, const struct ReportInfo *reportInfo, const void *response, size_t responseLen);
    // The "request" event processing entry.
    template<typename T>
    int32_t ProcessRequest(int32_t code, struct HdfSBuf *data);
    void SetRilCallback(const sptr<HDI::Ril::V1_0::IRilCallback> &callback);

protected:
    HRilBase(int32_t slotId, IHRilReporter &hrilReporter) : hrilReporter_(hrilReporter), slotId_(slotId) {}
    virtual ~HRilBase() {}

    int32_t ResponseBuffer(int32_t requestNum, const HRilRadioResponseInfo *responseInfo, uint32_t reqLen,
        const void *event, uint32_t eventLen);
    int32_t ResponseRequestInfo(int32_t requestNum, const HRilRadioResponseInfo *responseInfo, uint32_t reqLen);
    int32_t ResponseInt32(
        int32_t requestNum, const HRilRadioResponseInfo *responseInfo, uint32_t reqLen, uint32_t value);

    HRilNotiType ConvertIntToRadioNoticeType(int32_t indicationType);
    uint8_t ConvertHexCharToInt(uint8_t c);
    uint8_t *ConvertHexStringToBytes(const void *response, size_t responseLen);
    bool ConvertToString(char **dest, const std::string &src);
    HDI::Ril::V1_0::IHRilRadioResponseInfo BuildIHRilRadioResponseInfo(const HRilRadioResponseInfo &responseInfo);
    inline void SafeFrees() {}
    template<typename M, typename... Ms>
    inline void SafeFrees(M &m, Ms &...ms)
    {
        if (m != nullptr) {
            free(m);
            m = nullptr;
        }
        SafeFrees(ms...);
    }

    template<typename T>
    int32_t Response(const HRilRadioResponseInfo &responseInfo, T &&data, int32_t requestNum);
    template<typename SourceType, typename ToType>
    int32_t Notify(int32_t indType, const SourceType *notifyData, size_t notifyDataLen, int32_t notifyId);
    template<typename T>
    int32_t ResponseMessageParcel(const HRilRadioResponseInfo &responseInfo, const T &data, int32_t requestNum);
    template<typename T>
    int32_t NotifyMessageParcel(int32_t notifyType, T &data, int32_t requestNum);

    int32_t ConvertHexStringToInt(char **response, int32_t index, int32_t length);
    inline char *StringToCString(const std::string &src)
    {
        return static_cast<char *>(const_cast<char *>(src.c_str()));
    }

    // get slotid
    int32_t GetSlotId() const
    {
        return slotId_;
    }
    // normal dispatcher
    int32_t ServiceDispatcher(int32_t requestNum, const HdfSBuf *dataSbuf);
    // notify dispatcher
    int32_t ServiceNotifyDispatcher(int32_t requestNum, const HdfSBuf *dataSbuf);
    // create request event
    ReqDataInfo *CreateHRilRequest(int32_t serial, int32_t request);
    // Obtain request information from HDF and request it to the vendor layer.
    template<typename ReqFuncSet, typename FuncPointer, typename... ValueTypes>
    int32_t RequestVendor(
        struct HdfSBuf *data, int32_t requestId, ReqFuncSet reqFuncSet, FuncPointer func, ValueTypes &&...vals);
    template<typename ReqFuncSet, typename FuncPointer, typename... ValueTypes>
    int32_t RequestVendor(
        int32_t serial, int32_t requestId, ReqFuncSet reqFuncSet, FuncPointer func, ValueTypes &&... vals);

protected:
    std::map<uint32_t, std::any> respMemberFuncMap_;
    std::map<uint32_t, std::any> notiMemberFuncMap_;
    std::map<uint32_t, std::any> reqMemberFuncMap_;
    sptr<HDI::Ril::V1_0::IRilCallback> callback_ = nullptr;

private:
    // Read data from HDF buffer
    bool ReadFromHdfBuf(struct HdfSBuf *data, int32_t &val);
    bool ReadFromHdfBuf(struct HdfSBuf *data, const char *&val);
    template<typename... ValueTypes>
    bool ReadFromHdf(struct HdfSBuf *data, ValueTypes &&...vals);
    // Get the function pointer of the event handler.
    template<typename F>
    F GetFunc(std::map<uint32_t, std::any> &funcs, uint32_t code);
    // Write the report message header.
    int32_t ReportHeader(int32_t responseType, std::shared_ptr<struct HdfSBuf> &dataSbuf, MessageParcel &parcel);
    // Write reply message header
    int32_t ResponseHeader(
        const HRilRadioResponseInfo &responseInfo, std::shared_ptr<struct HdfSBuf> &dataSbuf, MessageParcel &parcel);

private:
    IHRilReporter &hrilReporter_;
    int32_t slotId_;
};

template<typename... ValueTypes>
bool HRilBase::ReadFromHdf(struct HdfSBuf *data, ValueTypes &&...__vals)
{
    return (ReadFromHdfBuf(data, std::forward<ValueTypes>(__vals)) && ...);
}

template<typename ReqFuncSet, typename FuncPointer, typename... ValueTypes>
int32_t HRilBase::RequestVendor(
    struct HdfSBuf *data, int32_t requestId, ReqFuncSet reqFuncSet, FuncPointer func, ValueTypes &&...vals)
{
    int32_t serial = -1;

    if (reqFuncSet == nullptr || (reqFuncSet->*func) == nullptr || data == nullptr) {
        TELEPHONY_LOGE("it is null: func set=%{public}p, data=%{public}p", reqFuncSet, data);
        return HRIL_ERR_NULL_POINT;
    }

    if (!ReadFromHdf(data, serial, std::forward<ValueTypes>(vals)...)) {
        TELEPHONY_LOGE("read fail from hdf, data=%{public}p, serial=%{public}d", data, serial);
        return HRIL_ERR_GENERIC_FAILURE;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, requestId);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("requestInfo == nullptr: serial=%{public}d, request=%{public}d", serial, requestId);
        return HRIL_ERR_MEMORY_FULL;
    }

    (reqFuncSet->*func)(requestInfo, std::forward<ValueTypes>(vals)...);
    return HRIL_ERR_SUCCESS;
}

template<typename ReqFuncSet, typename FuncPointer, typename... ValueTypes>
int32_t HRilBase::RequestVendor(
    int32_t serial, int32_t requestId, ReqFuncSet reqFuncSet, FuncPointer func, ValueTypes &&... vals)
{
    if (reqFuncSet == nullptr || (reqFuncSet->*func) == nullptr) {
        TELEPHONY_LOGE("it is null: func set=%{public}p", reqFuncSet);
        return HRIL_ERR_NULL_POINT;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, requestId);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("requestInfo == nullptr: serial=%{public}d, request=%{public}d", serial, requestId);
        return HRIL_ERR_MEMORY_FULL;
    }

    (reqFuncSet->*func)(requestInfo, std::forward<ValueTypes>(vals)...);
    return HRIL_ERR_SUCCESS;
}

template<typename F>
F HRilBase::GetFunc(std::map<uint32_t, std::any> &funcs, uint32_t code)
{
    auto itFunc = funcs.find(code);
    if (itFunc != funcs.end()) {
        return std::any_cast<F>(itFunc->second);
    }
    TELEPHONY_LOGE("Can not find Request code in func map: %{public}d!", code);
    return nullptr;
}

template<typename T>
int32_t HRilBase::ProcessResponse(
    int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    using RespFunc = int32_t (T::*)(
        int32_t requestNum, HRilRadioResponseInfo & responseInfo, const void *response, size_t responseLen);
    auto func = GetFunc<RespFunc>(respMemberFuncMap_, code);
    if (func != nullptr) {
        return (static_cast<T *>(this)->*func)(code, responseInfo, response, responseLen);
    }
    return HRIL_ERR_INVALID_PARAMETER;
}

template<typename T>
int32_t HRilBase::ProcessNotify(
    int32_t notifyType, const struct ReportInfo *reportInfo, const void *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    using NotiFunc = int32_t (T::*)(int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t code = reportInfo->notifyId;
    HRilErrNumber e = (HRilErrNumber)reportInfo->error;
    auto func = GetFunc<NotiFunc>(notiMemberFuncMap_, code);
    if (func != nullptr) {
        return (static_cast<T *>(this)->*func)(notifyType, e, response, responseLen);
    }
    return HRIL_ERR_INVALID_PARAMETER;
}

template<typename T>
int32_t HRilBase::ProcessRequest(int32_t code, struct HdfSBuf *data)
{
    using ReqFunc = int32_t (T::*)(struct HdfSBuf * data);
    auto func = GetFunc<ReqFunc>(reqMemberFuncMap_, code);
    if (func != nullptr) {
        // Because of the interaction with HDF, the error code of the HDF subsystem is used.
        return (static_cast<T *>(this)->*func)(data) == HDF_SUCCESS ? HDF_SUCCESS : HDF_FAILURE;
    }
    return HDF_FAILURE;
}

template<typename T>
int32_t HRilBase::Response(const HRilRadioResponseInfo &responseInfo, T &&data, int32_t requestNum)
{
    std::shared_ptr<struct HdfSBuf> dataSbuf;
    MessageParcel parcel = {};
    int32_t ret = ResponseHeader(responseInfo, dataSbuf, parcel);
    if (ret == HRIL_ERR_SUCCESS) {
        data.Marshalling(parcel);
        ret = ServiceDispatcher(requestNum, dataSbuf.get());
        if (ret != HRIL_ERR_SUCCESS) {
            TELEPHONY_LOGW("response to telRil ret: %{public}d", ret);
            return HRIL_ERR_GENERIC_FAILURE;
        }
        return HRIL_ERR_SUCCESS;
    }
    TELEPHONY_LOGE("write failed for response header, event id=%{public}d", requestNum);
    return ret;
}

template<typename SourceType, typename ToType>
int32_t HRilBase::Notify(int32_t indType, const SourceType *notifyData, size_t notifyDataLen, int32_t notifyId)
{
    if (notifyData == nullptr || notifyDataLen == 0 || (notifyDataLen % sizeof(SourceType)) != 0) {
        TELEPHONY_LOGE("notify to tel-ril error: data=%{public}p(len=%{public}zu)", notifyData, notifyDataLen);
        return HRIL_ERR_NULL_POINT;
    }

    std::shared_ptr<struct HdfSBuf> dataSbuf;
    MessageParcel parcel = {};
    int32_t ret = ReportHeader(indType, dataSbuf, parcel);
    if (ret == HRIL_ERR_SUCCESS) {
        ToType data = *notifyData;
        data.Marshalling(parcel);
        ret = ServiceNotifyDispatcher(notifyId, dataSbuf.get());
        if (ret != HRIL_ERR_SUCCESS) {
            TELEPHONY_LOGW("notify to telRil ret: %{public}d", ret);
            return HRIL_ERR_GENERIC_FAILURE;
        }
        return HRIL_ERR_SUCCESS;
    }
    TELEPHONY_LOGE("write failed for report header, event id=%{public}d", notifyId);
    return ret;
}

template<typename T>
int32_t HRilBase::ResponseMessageParcel(const HRilRadioResponseInfo &responseInfo, const T &data, int32_t requestNum)
{
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HRilResponseHeadInfo headInfo = {0};
    headInfo.slotId = slotId_;
    headInfo.type = responseInfo.type;
    if (!parcel->WriteUnpadBuffer(&headInfo, sizeof(HRilResponseHeadInfo))) {
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!parcel->WriteUnpadBuffer(&responseInfo, sizeof(responseInfo))) {
        return HRIL_ERR_GENERIC_FAILURE;
    }
    data.Marshalling(*parcel.get());

    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

template<typename T>
int32_t HRilBase::NotifyMessageParcel(int32_t notifyType, T &data, int32_t requestNum)
{
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    // step1:Write slotId into serialization.
    HRilResponseHeadInfo headInfo = {0};
    headInfo.slotId = slotId_;
    headInfo.type = (HRilResponseTypes)(notifyType);
    if (!parcel->WriteUnpadBuffer(&headInfo, sizeof(HRilResponseHeadInfo))) {
        return HRIL_ERR_GENERIC_FAILURE;
    }
    // step2:Write data to serialization.
    data.Marshalling(*parcel.get());
    // step3:Get the address of dataSbuf.
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    // step4:Dispatch to telRil.
    int32_t ret = ServiceNotifyDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSbufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_UTILS_H
