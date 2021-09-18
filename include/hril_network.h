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

#ifndef OHOS_HRIL_NETWORK_H
#define OHOS_HRIL_NETWORK_H

#include "hril_base.h"
#include "hril_network_parcel.h"

namespace OHOS {
namespace Telephony {
class HRilNetwork : public HRilBase {
public:
    HRilNetwork();
    ~HRilNetwork() = default;

    void ProcessNetworkResponse(int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    void ProcessNetworkRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);
    void ProcessNetworkNotify(
        int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);

    /* Requests to get current signal intensity relevant information. */
    void GetSignalStrength(int32_t slotId, struct HdfSBuf *data);
    /* Requests to get current voice registration status information. */
    void GetCsRegStatus(int32_t slotId, struct HdfSBuf *data);
    /* Requests to get current data registration status information. */
    void GetPsRegStatus(int32_t slotId, struct HdfSBuf *data);
    /* Requests to get current operator ONS or EONS */
    void GetOperatorInfo(int32_t slotId, struct HdfSBuf *data);

    void GetNetworkSearchInformation(int32_t slotId, struct HdfSBuf *data);

    void GetNetworkSelectionMode(int32_t slotId, struct HdfSBuf *data);

    void SetNetworkSelectionMode(int32_t slotId, struct HdfSBuf *data);
    void SetNetworkLocationUpdate(int32_t slotId, struct HdfSBuf *data);

    int32_t GetSignalStrengthResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCsRegStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetPsRegStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetOperatorInfoResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetNetworkSearchInformationResponse(int32_t slotId, int32_t requestNum,
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetNetworkSelectionModeResponse(int32_t slotId, int32_t requestNum,
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetNetworkSelectionModeResponse(int32_t slotId, int32_t requestNum,
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetNetworkLocationUpdateResponse(int32_t slotId, int32_t requestNum,
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t NetworkRegStatusUpdated(
        int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    int32_t SignalStrengthUpdated(
        int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    void BuildOperatorList(AvailableNetworkList &availableNetworkList, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    bool IsNetworkResponse(uint32_t code);

    bool IsNetworkNotification(uint32_t code);

    bool IsNetworkRespOrNotify(uint32_t code);
    void RegisterNetworkFuncs(const HRilNetworkReq *networkFuncs);

    void ExchangeRilRssiToHdf(const void *response, size_t responseLen, Rssi &rssi);

private:
    void AddHandlerToMap();

    using RespFunc = int32_t (HRilNetwork::*)(int32_t slotId, int32_t requestNum,
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    using NotiFunc = int32_t (HRilNetwork::*)(
        int32_t slotId, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    using ReqFunc = void (HRilNetwork::*)(int32_t slotId, struct HdfSBuf *data);

    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    std::map<uint32_t, ReqFunc> reqMemberFuncMap_;
    const HRilNetworkReq *networkFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_NETWORK_H
