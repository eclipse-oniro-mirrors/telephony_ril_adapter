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
    HRilNetwork(IHRilReporter &hrilReporter);
    virtual ~HRilNetwork() = default;

    void ProcessNetworkResponse(
        int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    void ProcessNetworkRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);
    void ProcessNetworkNotify(int32_t slotId, const struct ReportInfo *reportInfo,
        const void *response, size_t responseLen);

    void GetImsRegStatus(int32_t slotId, struct HdfSBuf *data);
    /* Requests to get current signal intensity relevant information. */
    void GetSignalStrength(int32_t slotId, struct HdfSBuf *data);
    /* Requests to get current voice registration status information. */
    void GetCsRegStatus(int32_t slotId, struct HdfSBuf *data);
    /* Requests to get current data registration status information. */
    void GetPsRegStatus(int32_t slotId, struct HdfSBuf *data);
    /* Requests to get current operator ONS or EONS */
    void GetOperatorInfo(int32_t slotId, struct HdfSBuf *data);

    void GetCellInfoList(int32_t slotId, struct HdfSBuf *data);

    void GetCurrentCellInfo(int32_t slotId, struct HdfSBuf *data);

    void GetNetworkSearchInformation(int32_t slotId, struct HdfSBuf *data);

    void GetNetworkSelectionMode(int32_t slotId, struct HdfSBuf *data);

    void SetNetworkSelectionMode(int32_t slotId, struct HdfSBuf *data);

    void SetPreferredNetwork(int32_t slotId, struct HdfSBuf *data);
    void GetPreferredNetwork(int32_t slotId, struct HdfSBuf *data);
    void SetPsAttachStatus(int32_t slotId, struct HdfSBuf *data);
    void GetPsAttachStatus(int32_t slotId, struct HdfSBuf *data);
    void GetRadioCapability(int32_t slotId, struct HdfSBuf *data);
    void SetRadioCapability(int32_t slotId, struct HdfSBuf *data);
    void GetPhysicalChannelConfig(int32_t slotId, struct HdfSBuf *data);
    void SetLocateUpdates(int32_t slotId, struct HdfSBuf *data);

    int32_t GetImsRegStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
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
    int32_t GetNetworkSelectionModeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetNetworkSelectionModeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetPreferredNetworkResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetPreferredNetworkResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetPsAttachStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetPsAttachStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetRadioCapabilityResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetRadioCapabilityResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetNeighboringCellInfoListResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCurrentCellInfoResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetPhysicalChannelConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetLocateUpdatesResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t NetworkCsRegStatusUpdated(
        int32_t slotId, int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NetworkPsRegStatusUpdated(
        int32_t slotId, int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t SignalStrengthUpdated(
        int32_t slotId, int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    void BuildOperatorList(AvailableNetworkList &availableNetworkList, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t NetworkTimeUpdated(
        int32_t slotId, int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NetworkTimeZoneUpdated(
        int32_t slotId, int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NetworkImsRegStatusUpdated(
        int32_t slotId, int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NetworkPhyChnlCfgUpdated(
        int32_t slotId, int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen);
    void RegisterNetworkFuncs(const HRilNetworkReq *networkFuncs);
    bool IsNetworkRespOrNotify(uint32_t code);

private:
    void AddHandlerToMap();
    void ExchangeRilRssiToHdf(const void *response, size_t responseLen, Rssi &rssi);
    void BuildCellInfo(
        CurrentCellInfo &cellInfo, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    void BuildCellList(CellListNearbyInfo &cellInfoList, HRilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen);
    void FillCellInfo(CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *cellInfoVendor);
    void FillCellInfoType(CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *hrilCellInfoVendor);
    void FillCellNearbyInfo(CellNearbyInfo &cellInfo, const CellInfo *cellPtr);
    void FillCellNearbyInfoTdscdma(CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr);
    void FillCellNearbyInfoCdma(CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr);
    using RespFunc = int32_t (HRilNetwork::*)(int32_t slotId, int32_t requestNum,
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    using NotiFunc = int32_t (HRilNetwork::*)(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);
    using ReqFunc = void (HRilNetwork::*)(int32_t slotId, struct HdfSBuf *data);
    bool IsNetworkResponse(uint32_t code);
    bool IsNetworkNotification(uint32_t code);

    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    std::map<uint32_t, ReqFunc> reqMemberFuncMap_;
    const HRilNetworkReq *networkFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_NETWORK_H
