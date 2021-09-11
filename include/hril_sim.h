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

#ifndef OHOS_HRIL_SIM_H
#define OHOS_HRIL_SIM_H

#include "hril_base.h"
#include "hril_sim_parcel.h"

namespace OHOS {
namespace Telephony {
class HRilSim : public HRilBase {
public:
    HRilSim();
    ~HRilSim() = default;
    void ProcessSimResponse(int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    void ProcessSimRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);
    void ProcessSimNotify(int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    bool IsSimRespOrNotify(uint32_t code);

    bool IsSimResponse(uint32_t code);

    bool IsSimNotification(uint32_t code);
    void RegisterSimFuncs(const HRilSimReq *simFuncs);

private:
    void AddHandlerToMap();
    IccIoResultInfo ProcessIccIoResponse(
        HRilRadioResponseInfo &responseInfo, int32_t serial, const void *response, size_t responseLen);
    /* Request to get SIM card status */
    void GetSimStatus(int32_t slotId, struct HdfSBuf *data);

    int32_t GetSimStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    /* Requests for ICC I/O operation */
    void RequestSimIO(int32_t slotId, struct HdfSBuf *data);

    int32_t RequestSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    /* Request to get the SIM IMSI. Only effective when radio status is open */
    void GetImsi(int32_t slotId, struct HdfSBuf *data);

    int32_t GetImsiResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void GetIccID(int32_t slotId, struct HdfSBuf *data);

    int32_t GetIccIDResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void GetSimLockStatus(int32_t slotId, struct HdfSBuf *data);

    int32_t GetSimLockStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void SetSimLock(int32_t slotId, struct HdfSBuf *data);

    int32_t SetSimLockResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void ChangeSimPassword(int32_t slotId, struct HdfSBuf *data);

    int32_t ChangeSimPasswordResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void EnterSimPin(int32_t slotId, struct HdfSBuf *data);

    int32_t EnterSimPinResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void UnlockSimPin(int32_t slotId, struct HdfSBuf *data);

    int32_t UnlockSimPinResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void GetSimPinInputTimes(int32_t slotId, struct HdfSBuf *data);

    int32_t GetSimPinInputTimesResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    int32_t SimStateUpdated(
        int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen);

private:
    using RespFunc = int32_t (HRilSim::*)(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    using NotiFunc = int32_t (HRilSim::*)(
        int32_t slotId, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    using ReqFunc = void (HRilSim::*)(int32_t slotId, struct HdfSBuf *data);
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    std::map<uint32_t, ReqFunc> reqMemberFuncMap_;
    const HRilSimReq *simFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_SIM_H