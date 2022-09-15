/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

    int32_t GetSignalStrength(int32_t serialId);
    int32_t GetCsRegStatus(int32_t serialId);
    int32_t GetPsRegStatus(int32_t serialId);
    int32_t GetOperatorInfo(int32_t serialId);
    int32_t GetNeighboringCellInfoList(int32_t serialId);
    int32_t GetCurrentCellInfo(int32_t serialId);
    int32_t GetNetworkSearchInformation(int32_t serialId);
    int32_t GetNetworkSelectionMode(int32_t serialId);
    int32_t SetNetworkSelectionMode(int32_t serialId, const HDI::Ril::V1_0::SetNetworkModeInfo &networkModeInfo);
    int32_t SetPreferredNetwork(int32_t serialId, int32_t preferredNetworkType);
    int32_t GetPreferredNetwork(int32_t serialId);
    int32_t GetPhysicalChannelConfig(int32_t serialId);
    int32_t SetLocateUpdates(int32_t serialId, const HDI::Ril::V1_0::RilRegNotifyMode mode);
    int32_t SetNotificationFilter(int32_t serialId, int32_t newFilter);
    int32_t SetDeviceState(int32_t serialId, int32_t deviceStateType, int32_t deviceStateOn);

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
    int32_t GetNeighboringCellInfoListResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCurrentCellInfoResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetPhysicalChannelConfigResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetLocateUpdatesResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetNotificationFilterResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetDeviceStateResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);

    int32_t NetworkCsRegStatusUpdated(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NetworkPsRegStatusUpdated(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t SignalStrengthUpdated(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NetworkTimeUpdated(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NetworkTimeZoneUpdated(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NetworkPhyChnlCfgUpdated(
        int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NetworkCurrentCellUpdated(
        int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen);

    void RegisterNetworkFuncs(const HRilNetworkReq *networkFuncs);
    bool IsNetworkRespOrNotify(uint32_t code);

private:
    void AddHandlerToMap();
    void ExchangeRilRssiToHdf(const void *response, HDI::Ril::V1_0::Rssi &rssi);
    void BuildOperatorList(
        HDI::Ril::V1_0::AvailableNetworkList &availableNetworkList, const void *response, size_t responseLen);
    void FillCellNearbyInfo(HDI::Ril::V1_0::CellNearbyInfo &cellInfo, const CellInfo *cellPtr);
    void FillCellNearbyInfoTdscdma(HDI::Ril::V1_0::CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr);
    void FillCellNearbyInfoCdma(HDI::Ril::V1_0::CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr);
    int32_t BuildNeighboringCellList(HDI::Ril::V1_0::CellListNearbyInfo &cellInfoList,
        const void *response, size_t responseLen);
    void FillCellInfoType(HDI::Ril::V1_0::CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *hrilCellInfoVendor);
    void FillCurrentCellInfo(HDI::Ril::V1_0::CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *cellInfoVendor);
    int32_t BuildCurrentCellList(HDI::Ril::V1_0::CellListCurrentInfo &cellInfoList,
        const void *response, size_t responseLen);
    bool IsNetworkResponse(uint32_t code);
    bool IsNetworkNotification(uint32_t code);

private:
    const HRilNetworkReq *networkFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_NETWORK_H
