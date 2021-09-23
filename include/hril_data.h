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

#ifndef OHOS_HRIL_DATA_H
#define OHOS_HRIL_DATA_H

#include "hril_base.h"
#include "hril_data_parcel.h"
#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
class HRilData : public HRilBase {
public:
    HRilData();
    virtual ~HRilData();

    void DeactivatePdpContext(int32_t slotId, struct HdfSBuf *data);
    void ActivatePdpContext(int32_t slotId, struct HdfSBuf *data);
    int32_t ActivatePdpContextResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t DeactivatePdpContextResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t PdpContextListUpdated(
        int32_t slotId, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    void RegisterDataResponseCallback(HdfRemoteService *serviceCallback);
    void RegisterDataNotifyCallback(HdfRemoteService *serviceCallbackInd);
    void ProcessDataResponse(int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    void ProcessDataRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);
    void ProcessDataNotify(int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    bool IsDataRespOrNotify(uint32_t code);

    bool IsDataResponse(uint32_t code);

    bool IsDataNotification(uint32_t code);
    void AddHandlerToMap();
    void RegisterDataFuncs(const HRilDataReq *dataFuncs);

private:
    static constexpr uint32_t HRIL_ERROR_UNSPECIFIED_RSN = 0xffff;
    void SwitchHRilDataListToHal(
        const void *response, size_t responseLen, std::vector<SetupDataCallResultInfo> &dcResultList);
    void SwitchRilDataToHal(HRilDataCallResponse *response, SetupDataCallResultInfo &result);
    static void RilDataCallCharToString(const char *src, std::string dst);
    using RespFunc = int32_t (HRilData::*)(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    using NotiFunc = int32_t (HRilData::*)(
        int32_t slotId, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    using ReqFunc = void (HRilData::*)(int32_t slotId, struct HdfSBuf *data);
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    std::map<uint32_t, ReqFunc> reqMemberFuncMap_;
    const HRilDataReq *dataFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_DATA_H
