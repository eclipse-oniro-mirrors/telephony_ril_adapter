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
    HRilNetwork(int32_t slotId, IHRilReporter &hrilReporter);
    virtual ~HRilNetwork() = default;

    int32_t GetImsRegStatus(struct HdfSBuf *data);
    /* Requests to get current signal intensity relevant information. */
    int32_t GetSignalStrength(struct HdfSBuf *data);
    /* Requests to get current voice registration status information. */
    int32_t GetCsRegStatus(struct HdfSBuf *data);
    /* Requests to get current data registration status information. */
    int32_t GetPsRegStatus(struct HdfSBuf *data);
    /* Requests to get current operator ONS or EONS */
    int32_t GetOperatorInfo(struct HdfSBuf *data);
    int32_t GetNeighboringCellInfoList(struct HdfSBuf *data);
    int32_t GetCurrentCellInfo(struct HdfSBuf *data);
    int32_t GetNetworkSearchInformation(struct HdfSBuf *data);
    int32_t GetNetworkSelectionMode(struct HdfSBuf *data);
    int32_t SetNetworkSelectionMode(struct HdfSBuf *data);
    int32_t SetPreferredNetwork(struct HdfSBuf *data);
    int32_t GetPreferredNetwork(struct HdfSBuf *data);
    int32_t GetRadioCapability(struct HdfSBuf *data);
    int32_t GetPhysicalChannelConfig(struct HdfSBuf *data);
    int32_t SetLocateUpdates(struct HdfSBuf *data);

    int32_t GetImsRegStatusResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetSignalStrengthResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCsRegStatusResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetPsRegStatusResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetOperatorInfoResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetNetworkSearchInformationResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetNetworkSelectionModeResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetNetworkSelectionModeResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetPreferredNetworkResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetPreferredNetworkResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetRadioCapabilityResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetNeighboringCellInfoListResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCurrentCellInfoResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetPhysicalChannelConfigResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetLocateUpdatesResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t NetworkCsRegStatusUpdated(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NetworkPsRegStatusUpdated(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t SignalStrengthUpdated(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    void BuildOperatorList(AvailableNetworkList &availableNetworkList, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t NetworkTimeUpdated(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NetworkTimeZoneUpdated(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NetworkImsRegStatusUpdated(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NetworkPhyChnlCfgUpdated(int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NetworkCurrentCellUpdated(
        int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen);
    void RegisterNetworkFuncs(const HRilNetworkReq *networkFuncs);
    bool IsNetworkRespOrNotify(uint32_t code);

private:
    void AddHandlerToMap();
    void ExchangeRilRssiToHdf(const void *response, size_t responseLen, Rssi &rssi);
    void FillCellNearbyInfo(CellNearbyInfo &cellInfo, const CellInfo *cellPtr);
    void FillCellNearbyInfoTdscdma(CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr);
    void FillCellNearbyInfoCdma(CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr);
    int32_t BuildNeighboringCellList(CellListNearbyInfo &cellInfoList, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    void FillCellInfoType(CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *hrilCellInfoVendor);
    void FillCurrentCellInfo(CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *cellInfoVendor);
    int32_t BuildCurrentCellList(CellListCurrentInfo &cellInfoList, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t FixCurrentCellList(CellListCurrentInfo &cellInfoList, const CurrentCellInfoList *hrilCurrentCellList);
    bool IsNetworkResponse(uint32_t code);
    bool IsNetworkNotification(uint32_t code);

    const HRilNetworkReq *networkFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_NETWORK_H
