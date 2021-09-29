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

#ifndef OHOS_HRIL_UTILS_H
#define OHOS_HRIL_UTILS_H

#include <securec.h>
#include <map>

#include "hdf_remote_service.h"
#include "hdf_sbuf_ipc.h"

#include "hril_types.h"
#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
#define HRIL_NOTIFICATION_TYPE 1

// Max malloc memory size
#define MALLOC_MAX_SIZE (1024 * 1024)

using RespCmdInfo = struct RespCmdInfo;
using namespace std;
extern "C" const char *GetRequestStringInfo(int32_t request);

typedef struct RespCmdInfo {
    int32_t request;
    int32_t (*respFunc)(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
} RespCmdInfo;

typedef struct {
    int32_t request;
    void (*requestFunction)(int32_t slotId, struct HdfSBuf *data, struct HdfSBuf *reply);
} ReqCmdInfo;

ReqDataInfo *CreateHRilRequest(int32_t serial, int32_t slotId, int32_t request);

class HRilBase {
public:
    int32_t AtoiNullHandle(const char *x);
    int32_t AtoiNullHandleDef(const char *x, int32_t defaultVal);

    void RegisterResponseCallback(HdfRemoteService *serviceCallback);
    void RegisterNotifyCallback(HdfRemoteService *serviceCallbackInd);

    void SendErrorResponse(ReqDataInfo *requestInfo, HRilErrno err);

    int32_t ResponseBuffer(
        int32_t requestNum, const void *responseInfo, uint32_t reqLen, const void *event, uint32_t eventLen);
    int32_t ResponseRequestInfo(int32_t requestNum, const void *responseInfo, uint32_t reqLen);

    HRilNotiType ConvertIntToRadioNoticeType(int32_t indicationType);
    uint8_t ConvertHexCharToInt(uint8_t c);
    uint8_t *ConvertHexStringToBytes(const void *response, size_t responseLen);
    bool ConvertToString(char **dest, const std::string &src, ReqDataInfo *requestInfo);
    void FreeStrings(int32_t argCounts, ...);

    template<typename T>
    int32_t ResponseMessageParcel(HRilRadioResponseInfo &responseInfo, T &data, int32_t requestNum)
    {
        struct HdfSBuf *dataSbuf;
        std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
        if (parcel == nullptr) {
            return HDF_FAILURE;
        }
        data.Marshalling(*parcel.get());
        dataSbuf = ParcelToSbuf(parcel.get());
        if (dataSbuf == nullptr) {
            return HDF_FAILURE;
        }
        if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
            HdfSBufRecycle(dataSbuf);
            return HDF_FAILURE;
        }
        if (serviceCallback_ == nullptr) {
            HdfSBufRecycle(dataSbuf);
            return HDF_FAILURE;
        }
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

    int32_t ConvertHexStringToInt(char **response, int32_t index, int32_t length);
    struct HdfRemoteService *serviceCallback_ = nullptr;
    struct HdfRemoteService *serviceCallbackNotify_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_UTILS_H