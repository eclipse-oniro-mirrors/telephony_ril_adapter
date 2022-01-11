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

#include <cstdlib>
#include <map>
#include <securec.h>

#include "hdf_remote_service.h"
#include "hdf_sbuf_ipc.h"

#include "hril_types.h"
#include "telephony_log_wrapper.h"

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
    HRilBase(IHRilReporter &hrilReporter) : hrilReporter_(hrilReporter) { }
    virtual ~HRilBase() { }

    int32_t ResponseBuffer(
        int32_t requestNum, const void *responseInfo, uint32_t reqLen, const void *event, uint32_t eventLen);
    int32_t ResponseRequestInfo(int32_t requestNum, const void *responseInfo, uint32_t reqLen);
    int32_t ResponseInt32(int32_t requestNum, const void *responseInfo, uint32_t reqLen, uint32_t value);

    HRilNotiType ConvertIntToRadioNoticeType(int32_t indicationType);
    uint8_t ConvertHexCharToInt(uint8_t c);
    uint8_t *ConvertHexStringToBytes(const void *response, size_t responseLen);
    bool ConvertToString(char **dest, const std::string &src);
    void FreeStrings(int32_t argCounts, ...);

    template<typename T>
    int32_t ResponseMessageParcel(const HRilRadioResponseInfo &responseInfo, const T &data, int32_t requestNum)
    {
        struct HdfSBuf *dataSbuf;
        std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
        if (parcel == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
        data.Marshalling(*parcel.get());
        dataSbuf = ParcelToSbuf(parcel.get());
        if (dataSbuf == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
        if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
            HdfSBufRecycle(dataSbuf);
            return HRIL_ERR_GENERIC_FAILURE;
        }
        int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
        if (ret != HRIL_ERR_SUCCESS) {
            HdfSBufRecycle(dataSbuf);
            return HRIL_ERR_GENERIC_FAILURE;
        }
        if (dataSbuf != nullptr) {
            HdfSBufRecycle(dataSbuf);
        }
        return HRIL_ERR_SUCCESS;
    }

    template<typename T>
    int32_t NotifyMessageParcel(T &data, int32_t requestNum)
    {
        struct HdfSBuf *dataSbuf;
        std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();

        dataSbuf = ParcelToSbuf(parcel.get());
        if (dataSbuf == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
        data.Marshalling(*parcel.get());
        int32_t ret = ServiceNotifyDispatcher(requestNum, dataSbuf);
        if (ret != HRIL_ERR_SUCCESS) {
            HdfSBufRecycle(dataSbuf);
            return HRIL_ERR_GENERIC_FAILURE;
        }
        if (dataSbuf != nullptr) {
            HdfSBufRecycle(dataSbuf);
        }
        return HRIL_ERR_SUCCESS;
    }

    int32_t ConvertHexStringToInt(char **response, int32_t index, int32_t length);

    // Safety release memorys
    inline void SafeFrees() {}
    // Safety release memorys
    template<typename M, typename...Ms>
    inline void SafeFrees(M &m, Ms& ...ms)
    {
        if (m != nullptr) {
            free(m);
            m = nullptr;
        }
        SafeFrees(ms...);
    }

    inline char *StringToCString(const std::string &src)
    {
        return static_cast<char*>(const_cast<char*>(src.c_str()));
    }

protected:
    // normal dispatcher
    int32_t ServiceDispatcher(int32_t requestNum, const HdfSBuf *dataSbuf);
    // notify dispatcher
    int32_t ServiceNotifyDispatcher(int32_t requestNum, const HdfSBuf *dataSbuf);

    // create request event
    ReqDataInfo *CreateHRilRequest(int32_t serial, int32_t slotId, int32_t request);

private:
    IHRilReporter &hrilReporter_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_UTILS_H