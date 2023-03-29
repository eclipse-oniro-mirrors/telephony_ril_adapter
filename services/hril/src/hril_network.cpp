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

#include "hril_network.h"

#include "hril_notification.h"
#include "hril_request.h"

namespace OHOS {
namespace Telephony {
enum class NetworkParameter : int32_t {
    RESPONSE_VALUE = 3,
    INVALID_RESPONSE_VALUE = 11,
};

HRilNetwork::HRilNetwork(int32_t slotId, IHRilReporter &hrilReporter) : HRilBase(slotId, hrilReporter)
{
    AddHandlerToMap();
}

void HRilNetwork::AddHandlerToMap()
{
    // indication
    notiMemberFuncMap_[HNOTI_NETWORK_CS_REG_STATUS_UPDATED] = &HRilNetwork::NetworkCsRegStatusUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED] = &HRilNetwork::SignalStrengthUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_TIME_UPDATED] = &HRilNetwork::NetworkTimeUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_TIME_ZONE_UPDATED] = &HRilNetwork::NetworkTimeZoneUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_PS_REG_STATUS_UPDATED] = &HRilNetwork::NetworkPsRegStatusUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_PHY_CHNL_CFG_UPDATED] = &HRilNetwork::NetworkPhyChnlCfgUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_CURRENT_CELL_UPDATED] = &HRilNetwork::NetworkCurrentCellUpdated;

    // Response
    respMemberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &HRilNetwork::GetSignalStrengthResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &HRilNetwork::GetCsRegStatusResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &HRilNetwork::GetPsRegStatusResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &HRilNetwork::GetOperatorInfoResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION] = &HRilNetwork::GetNetworkSearchInformationResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_NETWORK_SELECTION_MODE] = &HRilNetwork::GetNetworkSelectionModeResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_NETWORK_SELECTION_MODE] = &HRilNetwork::SetNetworkSelectionModeResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_PREFERRED_NETWORK] = &HRilNetwork::SetPreferredNetworkResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_PREFERRED_NETWORK] = &HRilNetwork::GetPreferredNetworkResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST] = &HRilNetwork::GetNeighboringCellInfoListResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_CURRENT_CELL_INFO] = &HRilNetwork::GetCurrentCellInfoResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG] = &HRilNetwork::GetPhysicalChannelConfigResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_LOCATE_UPDATES] = &HRilNetwork::SetLocateUpdatesResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_NOTIFICATION_FILTER] = &HRilNetwork::SetNotificationFilterResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_DEVICE_STATE] = &HRilNetwork::SetDeviceStateResponse;
}

int32_t HRilNetwork::GetSignalStrength(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_NETWORK_GET_SIGNAL_STRENGTH, networkFuncs_, &HRilNetworkReq::GetSignalStrength);
}

int32_t HRilNetwork::GetCsRegStatus(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_NETWORK_GET_CS_REG_STATUS, networkFuncs_, &HRilNetworkReq::GetCsRegStatus);
}

int32_t HRilNetwork::GetPsRegStatus(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_NETWORK_GET_PS_REG_STATUS, networkFuncs_, &HRilNetworkReq::GetPsRegStatus);
}

int32_t HRilNetwork::GetOperatorInfo(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_NETWORK_GET_OPERATOR_INFO, networkFuncs_, &HRilNetworkReq::GetOperatorInfo);
}

int32_t HRilNetwork::GetNeighboringCellInfoList(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST, networkFuncs_,
        &HRilNetworkReq::GetNeighboringCellInfoList);
}

int32_t HRilNetwork::GetCurrentCellInfo(int32_t serialId)
{
    return RequestVendor(
        serialId, HREQ_NETWORK_GET_CURRENT_CELL_INFO, networkFuncs_, &HRilNetworkReq::GetCurrentCellInfo);
}

int32_t HRilNetwork::GetNetworkSearchInformation(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION, networkFuncs_,
        &HRilNetworkReq::GetNetworkSearchInformation);
}

int32_t HRilNetwork::GetNetworkSelectionMode(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_NETWORK_GET_NETWORK_SELECTION_MODE, networkFuncs_,
        &HRilNetworkReq::GetNetworkSelectionMode);
}

int32_t HRilNetwork::SetNetworkSelectionMode(
    int32_t serialId, const HDI::Ril::V1_0::SetNetworkModeInfo &networkModeInfo)
{
    HRilSetNetworkModeInfo setNetworkModeInfo = {};
    setNetworkModeInfo.selectMode = networkModeInfo.selectMode;
    ConvertToString(&setNetworkModeInfo.oper, networkModeInfo.oper);
    TELEPHONY_LOGI("HRilNetwork::SetNetworkSelectionMode selectMode = %{public}d", setNetworkModeInfo.selectMode);
    return RequestVendor(serialId, HREQ_NETWORK_SET_NETWORK_SELECTION_MODE, networkFuncs_,
        &HRilNetworkReq::SetNetworkSelectionMode, &setNetworkModeInfo);
}

int32_t HRilNetwork::SetPreferredNetwork(int32_t serialId, int32_t preferredNetworkType)
{
    return RequestVendor(serialId, HREQ_NETWORK_SET_PREFERRED_NETWORK, networkFuncs_,
        &HRilNetworkReq::SetPreferredNetwork, &preferredNetworkType);
}

int32_t HRilNetwork::GetPreferredNetwork(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_NETWORK_GET_PREFERRED_NETWORK, networkFuncs_,
        &HRilNetworkReq::GetPreferredNetwork);
}

int32_t HRilNetwork::GetPhysicalChannelConfig(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG, networkFuncs_,
        &HRilNetworkReq::GetPhysicalChannelConfig);
}

int32_t HRilNetwork::SetLocateUpdates(int32_t serialId, const HDI::Ril::V1_0::RilRegNotifyMode mode)
{
    HRilRegNotifyMode regNotifyMode = static_cast<HRilRegNotifyMode>(mode);
    if ((regNotifyMode < REG_NOT_NOTIFY) || (regNotifyMode > REG_NOTIFY_STAT_LAC_CELLID)) {
        TELEPHONY_LOGE("SetLocateUpdates Invalid regNotifyMode parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    return RequestVendor(serialId, HREQ_NETWORK_SET_LOCATE_UPDATES, networkFuncs_,
        &HRilNetworkReq::SetLocateUpdates, static_cast<HRilRegNotifyMode>(mode));
}

int32_t HRilNetwork::SetNotificationFilter(int32_t serialId, int32_t newFilter)
{
    return RequestVendor(serialId, HREQ_NETWORK_SET_NOTIFICATION_FILTER, networkFuncs_,
        &HRilNetworkReq::SetNotificationFilter, &newFilter);
}

int32_t HRilNetwork::SetDeviceState(int32_t serialId, int32_t deviceStateType, int32_t deviceStateOn)
{
    return RequestVendor(serialId, HREQ_NETWORK_SET_DEVICE_STATE, networkFuncs_,
        &HRilNetworkReq::SetDeviceState, &deviceStateType, &deviceStateOn);
}

int32_t HRilNetwork::GetSignalStrengthResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::Rssi rssi = {};
    if (response == nullptr || responseLen != sizeof(HRilRssi)) {
        TELEPHONY_LOGE("GetSignalStrengthResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        ExchangeRilRssiToHdf(response, rssi);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetSignalStrengthResponse, rssi);
}

int32_t HRilNetwork::GetCsRegStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::CsRegStatusInfo csRegStatusInfo = {};
    if (response == nullptr || responseLen != sizeof(HRilRegStatusInfo)) {
        TELEPHONY_LOGE("GetCsRegStatusResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
        csRegStatusInfo.notifyType = hrilRegStatusInfo->notifyMode;
        csRegStatusInfo.regStatus = static_cast<HDI::Ril::V1_0::RilRegStatus>(hrilRegStatusInfo->regStatus);
        csRegStatusInfo.lacCode = hrilRegStatusInfo->lacCode;
        csRegStatusInfo.cellId = hrilRegStatusInfo->cellId;
        csRegStatusInfo.radioTechnology = static_cast<HDI::Ril::V1_0::RilRadioTech>(hrilRegStatusInfo->actType);
        TELEPHONY_LOGI("GetCsRegStatusResponse notifyType:%{public}d, regStatus:%{public}d, "
                       "lacCode:%{private}d, cellId:%{private}d, radioTechnology:%{public}d",
            csRegStatusInfo.notifyType, csRegStatusInfo.regStatus, csRegStatusInfo.lacCode, csRegStatusInfo.cellId,
            csRegStatusInfo.radioTechnology);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCsRegStatusResponse, csRegStatusInfo);
}

int32_t HRilNetwork::GetPsRegStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::PsRegStatusInfo psRegStatusInfo = {};
    if (response == nullptr || responseLen != sizeof(HRilRegStatusInfo)) {
        TELEPHONY_LOGE("GetPsRegStatusResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
        psRegStatusInfo.notifyType = hrilRegStatusInfo->notifyMode;
        psRegStatusInfo.regStatus = static_cast<HDI::Ril::V1_0::RilRegStatus>(hrilRegStatusInfo->regStatus);
        psRegStatusInfo.lacCode = hrilRegStatusInfo->lacCode;
        psRegStatusInfo.cellId = hrilRegStatusInfo->cellId;
        psRegStatusInfo.radioTechnology = static_cast<HDI::Ril::V1_0::RilRadioTech>(hrilRegStatusInfo->actType);
        psRegStatusInfo.isDcNrRestricted = hrilRegStatusInfo->isDcNrRestricted;
        psRegStatusInfo.isNrAvailable = hrilRegStatusInfo->isNrAvailable;
        psRegStatusInfo.isEnDcAvailable = hrilRegStatusInfo->isEnDcAvailable;
        TELEPHONY_LOGI(
            "GetPsRegStatusResponse notifyType:%{public}d, regStatus:%{public}d, lacCode:%{private}d, "
            "cellId:%{private}d, technology:%{public}d, isDcNrRestricted:%{private}d, isNrAvailable:%{private}d, "
            "isEnDcAvailable:%{private}d",
            psRegStatusInfo.notifyType, psRegStatusInfo.regStatus, psRegStatusInfo.lacCode, psRegStatusInfo.cellId,
            psRegStatusInfo.radioTechnology, psRegStatusInfo.isDcNrRestricted, psRegStatusInfo.isNrAvailable,
            psRegStatusInfo.isEnDcAvailable);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetPsRegStatusResponse, psRegStatusInfo);
}

int32_t HRilNetwork::GetOperatorInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::OperatorInfo operatorInfoResult = {};
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("GetOperatorInfoResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        char **resp = static_cast<char **>(const_cast<void *>(response));
        operatorInfoResult.longName = (resp[HRIL_LONE_NAME] == nullptr) ? "" : resp[HRIL_LONE_NAME];
        operatorInfoResult.shortName = (resp[HRIL_SHORT_NAME] == nullptr) ? "" : resp[HRIL_SHORT_NAME];
        operatorInfoResult.numeric = (resp[HRIL_NUMERIC] == nullptr) ? "" : resp[HRIL_NUMERIC];
        TELEPHONY_LOGD("GetOperatorInfoResponse longName:%{public}s, shortName:%{public}s, numeric:%{public}s",
            operatorInfoResult.longName.c_str(), operatorInfoResult.shortName.c_str(),
            operatorInfoResult.numeric.c_str());
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetOperatorInfoResponse, operatorInfoResult);
}

int32_t HRilNetwork::GetNetworkSearchInformationResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::AvailableNetworkList availableNetworkList = {};
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("GetNetworkSearchInformationResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        availableNetworkList.itemNum = 0;
        BuildOperatorList(availableNetworkList, response, responseLen);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetNetworkSearchInformationResponse,
        availableNetworkList);
}

int32_t HRilNetwork::GetNetworkSelectionModeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::SetNetworkModeInfo selectModeInfo = {};
    if (response == nullptr || responseLen != sizeof(int32_t)) {
        TELEPHONY_LOGE("GetNetworkSelectionModeResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t *resp = static_cast<int32_t *>(const_cast<void *>(response));
        selectModeInfo.selectMode = *resp;
        TELEPHONY_LOGI("GetNetworkSelectionModeResponse selectMode: %{public}d", selectModeInfo.selectMode);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetNetworkSelectionModeResponse, selectModeInfo);
}

int32_t HRilNetwork::SetNetworkSelectionModeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetNetworkSelectionModeResponse);
}

int32_t HRilNetwork::SetPreferredNetworkResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetPreferredNetworkResponse);
}

int32_t HRilNetwork::GetPreferredNetworkResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::PreferredNetworkTypeInfo preferredNetworkTypeInfo = {};
    if (response == nullptr || responseLen != sizeof(int32_t)) {
        TELEPHONY_LOGE("GetPreferredNetworkResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t *resp = static_cast<int32_t *>(const_cast<void *>(response));
        preferredNetworkTypeInfo.preferredNetworkType = *resp;
        TELEPHONY_LOGI("GetPreferredNetworkResponse preferredNetworkType: %{public}d", *resp);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetPreferredNetworkResponse, preferredNetworkTypeInfo);
}

int32_t HRilNetwork::GetNeighboringCellInfoListResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::CellListNearbyInfo cellInfoList;
    if (response == nullptr || responseLen != sizeof(CellInfoList)) {
        TELEPHONY_LOGE("GetNeighboringCellInfoListResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        cellInfoList.itemNum = 0;
        cellInfoList.cellNearbyInfo.clear();
        if (BuildNeighboringCellList(cellInfoList, response, responseLen) != 0) {
            TELEPHONY_LOGE("GetNeighboringCellInfoListResponse BuildNeighboringCellList failed");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetNeighboringCellInfoListResponse, cellInfoList);
}

int32_t HRilNetwork::GetCurrentCellInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::CellListCurrentInfo cellList;
    if (response == nullptr || responseLen != sizeof(CurrentCellInfoList)) {
        TELEPHONY_LOGE("GetCurrentCellInfoResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        cellList.itemNum = 0;
        cellList.cellCurrentInfo.clear();
        if (BuildCurrentCellList(cellList, response, responseLen) != 0) {
            TELEPHONY_LOGE("GetCurrentCellInfoResponse BuildCurrentCellList failed");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCurrentCellInfoResponse, cellList);
}

int32_t HRilNetwork::GetPhysicalChannelConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::ChannelConfigInfoList phyChnlCfgList = {};
    if (response == nullptr || responseLen != sizeof(HRilChannelConfigList)) {
        TELEPHONY_LOGE("GetPhysicalChannelConfigResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        phyChnlCfgList.itemNum = 0;
        phyChnlCfgList.channelConfigInfos.clear();
        const HRilChannelConfigList *hrilChannelConfigList = static_cast<const HRilChannelConfigList *>(response);
        phyChnlCfgList.itemNum = hrilChannelConfigList->itemNum;
        for (int32_t i = 0; i < phyChnlCfgList.itemNum; i++) {
            HDI::Ril::V1_0::PhysicalChannelConfig phyChnlCfg;
            phyChnlCfg.cellConnStatus = static_cast<HDI::Ril::V1_0::RilCellConnectionStatus>(
                hrilChannelConfigList->channelConfigs[i].cellConnStatus);
            phyChnlCfg.cellBandwidthDownlinkKhz = hrilChannelConfigList->channelConfigs[i].cellBandwidthDownlinkKhz;
            phyChnlCfg.cellBandwidthUplinkKhz = hrilChannelConfigList->channelConfigs[i].cellBandwidthUplinkKhz;
            phyChnlCfg.ratType =
                static_cast<HDI::Ril::V1_0::RilRadioTech>(hrilChannelConfigList->channelConfigs[i].ratType);
            phyChnlCfg.freqRange = hrilChannelConfigList->channelConfigs[i].freqRange;
            phyChnlCfg.downlinkChannelNum = hrilChannelConfigList->channelConfigs[i].downlinkChannelNum;
            phyChnlCfg.uplinkChannelNum = hrilChannelConfigList->channelConfigs[i].uplinkChannelNum;
            phyChnlCfg.physicalCellId = hrilChannelConfigList->channelConfigs[i].physicalCellId;
            phyChnlCfg.contextIdNum = hrilChannelConfigList->channelConfigs[i].contextIdNum;
            TELEPHONY_LOGI(
                "GetPhysicalChannelConfigResponse cellConnStatus:%{private}d, "
                "cellBandwidthDownlinkKhz:%{private}d, cellBandwidthUplinkKhz:%{private}d, physicalCellId:%{private}d, "
                "ratType:%{private}d, freqRange:%{private}d, downlinkChannelNum:%{private}d, "
                "uplinkChannelNum:%{private}d, contextIdNum:%{private}d",
                phyChnlCfg.cellConnStatus, phyChnlCfg.cellBandwidthDownlinkKhz, phyChnlCfg.cellBandwidthUplinkKhz,
                phyChnlCfg.ratType, phyChnlCfg.freqRange, phyChnlCfg.downlinkChannelNum, phyChnlCfg.uplinkChannelNum,
                phyChnlCfg.physicalCellId, phyChnlCfg.contextIdNum);
            for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
                phyChnlCfg.contextIds.push_back(hrilChannelConfigList->channelConfigs[i].contextIds[j]);
                TELEPHONY_LOGI("contextIds:%{public}d---contextId:%{private}d", j, phyChnlCfg.contextIds[j]);
            }
            phyChnlCfgList.channelConfigInfos.push_back(phyChnlCfg);
        }
        TELEPHONY_LOGI("GetPhysicalChannelConfigResponse itemNum:%{public}d", phyChnlCfgList.itemNum);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetPhysicalChannelConfigResponse, phyChnlCfgList);
}

int32_t HRilNetwork::SetLocateUpdatesResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetLocateUpdatesResponse);
}

int32_t HRilNetwork::SetNotificationFilterResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetNotificationFilterResponse);
}

int32_t HRilNetwork::SetDeviceStateResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetDeviceStateResponse);
}

int32_t HRilNetwork::NetworkCsRegStatusUpdated(
    int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(HRilRegStatusInfo)) {
        TELEPHONY_LOGE("NetworkCsRegStatusUpdated response is invalid");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::CsRegStatusInfo regStatusInfoNotify = {};
    const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
    regStatusInfoNotify.notifyType = hrilRegStatusInfo->notifyMode;
    regStatusInfoNotify.regStatus = static_cast<HDI::Ril::V1_0::RilRegStatus>(hrilRegStatusInfo->regStatus);
    regStatusInfoNotify.lacCode = hrilRegStatusInfo->lacCode;
    regStatusInfoNotify.cellId = hrilRegStatusInfo->cellId;
    regStatusInfoNotify.radioTechnology = static_cast<HDI::Ril::V1_0::RilRadioTech>(hrilRegStatusInfo->actType);
    TELEPHONY_LOGI("NetworkCsRegStatusUpdated notifyType:%{public}d, regStatus:%{public}d, "
                   "lacCode:%{private}d, cellId:%{private}d, radioTechnology:%{public}d",
        regStatusInfoNotify.notifyType, regStatusInfoNotify.regStatus, regStatusInfoNotify.lacCode,
        regStatusInfoNotify.cellId, regStatusInfoNotify.radioTechnology);
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::NetworkCsRegStatusUpdated, regStatusInfoNotify);
}

int32_t HRilNetwork::NetworkPsRegStatusUpdated(
    int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(HRilRegStatusInfo)) {
        TELEPHONY_LOGE("NetworkPsRegStatusUpdated response is invalid");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::PsRegStatusInfo regStatusInfoNotify = {};
    const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
    regStatusInfoNotify.notifyType = hrilRegStatusInfo->notifyMode;
    regStatusInfoNotify.regStatus = static_cast<HDI::Ril::V1_0::RilRegStatus>(hrilRegStatusInfo->regStatus);
    regStatusInfoNotify.lacCode = hrilRegStatusInfo->lacCode;
    regStatusInfoNotify.cellId = hrilRegStatusInfo->cellId;
    regStatusInfoNotify.radioTechnology = static_cast<HDI::Ril::V1_0::RilRadioTech>(hrilRegStatusInfo->actType);
    regStatusInfoNotify.isDcNrRestricted = hrilRegStatusInfo->isDcNrRestricted;
    regStatusInfoNotify.isNrAvailable = hrilRegStatusInfo->isNrAvailable;
    regStatusInfoNotify.isEnDcAvailable = hrilRegStatusInfo->isEnDcAvailable;
    TELEPHONY_LOGI(
        "GetPsRegStatusResponse notifyType:%{public}d, regStatus:%{public}d, lacCode:%{private}d, cellId:%{private}d, "
        "radioTechnology:%{public}d, isDcNrRestricted:%{private}d, isNrAvailable:%{private}d, "
        "isEnDcAvailable:%{private}d",
        regStatusInfoNotify.notifyType, regStatusInfoNotify.regStatus, regStatusInfoNotify.lacCode,
        regStatusInfoNotify.cellId, regStatusInfoNotify.radioTechnology, regStatusInfoNotify.isDcNrRestricted,
        regStatusInfoNotify.isNrAvailable, regStatusInfoNotify.isEnDcAvailable);
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::NetworkPsRegStatusUpdated, regStatusInfoNotify);
}

int32_t HRilNetwork::SignalStrengthUpdated(
    int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(HRilRssi)) {
        TELEPHONY_LOGE("SignalStrengthUpdated response is invalid");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::Rssi rssi = {};
    ExchangeRilRssiToHdf(response, rssi);
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::SignalStrengthUpdated, rssi);
}

int32_t HRilNetwork::NetworkTimeUpdated(
    int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("NetworkTimeUpdated response is invalid");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::NetworkTimeUpdated, (const char *)response);
}

int32_t HRilNetwork::NetworkTimeZoneUpdated(
    int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("NetworkTimeZoneUpdated response is invalid");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::NetworkTimeZoneUpdated, (const char *)response);
}

int32_t HRilNetwork::NetworkPhyChnlCfgUpdated(
    int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(HRilRegStatusInfo)) {
        TELEPHONY_LOGE("NetworkPhyChnlCfgUpdated response is invalid");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::ChannelConfigInfoList phyChnlCfgList = {};
    phyChnlCfgList.itemNum = 0;
    phyChnlCfgList.channelConfigInfos.clear();
    const HRilChannelConfigList *hrilChannelConfigList = static_cast<const HRilChannelConfigList *>(response);
    phyChnlCfgList.itemNum = hrilChannelConfigList->itemNum;
    for (int32_t i = 0; i < phyChnlCfgList.itemNum; i++) {
        HDI::Ril::V1_0::PhysicalChannelConfig phyChnlCfg;
        phyChnlCfg.cellConnStatus = static_cast<HDI::Ril::V1_0::RilCellConnectionStatus>(
            hrilChannelConfigList->channelConfigs[i].cellConnStatus);
        phyChnlCfg.cellBandwidthDownlinkKhz = hrilChannelConfigList->channelConfigs[i].cellBandwidthDownlinkKhz;
        phyChnlCfg.cellBandwidthUplinkKhz = hrilChannelConfigList->channelConfigs[i].cellBandwidthUplinkKhz;
        phyChnlCfg.ratType =
            static_cast<HDI::Ril::V1_0::RilRadioTech>(hrilChannelConfigList->channelConfigs[i].ratType);
        phyChnlCfg.freqRange = hrilChannelConfigList->channelConfigs[i].freqRange;
        phyChnlCfg.downlinkChannelNum = hrilChannelConfigList->channelConfigs[i].downlinkChannelNum;
        phyChnlCfg.uplinkChannelNum = hrilChannelConfigList->channelConfigs[i].uplinkChannelNum;
        phyChnlCfg.physicalCellId = hrilChannelConfigList->channelConfigs[i].physicalCellId;
        phyChnlCfg.contextIdNum = hrilChannelConfigList->channelConfigs[i].contextIdNum;
        for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
            phyChnlCfg.contextIds.push_back(hrilChannelConfigList->channelConfigs[i].contextIds[j]);
        }
        phyChnlCfgList.channelConfigInfos.push_back(phyChnlCfg);
    }
    TELEPHONY_LOGI("NetworkPhyChnlCfgUpdated itemNum:%{public}d", phyChnlCfgList.itemNum);
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::NetworkPhyChnlCfgUpdated, phyChnlCfgList);
}

int32_t HRilNetwork::NetworkCurrentCellUpdated(
    int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(HRilRegStatusInfo)) {
        TELEPHONY_LOGE("NetworkCurrentCellUpdated response is invalid");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::CellListCurrentInfo cellList;
    cellList.itemNum = 0;
    cellList.cellCurrentInfo.clear();
    if (BuildCurrentCellList(cellList, response, responseLen) != 0) {
        TELEPHONY_LOGE("NetworkCurrentCellUpdated BuildCurrentCellList failed");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::NetworkCurrentCellUpdated, cellList);
}

void HRilNetwork::ExchangeRilRssiToHdf(const void *response, HDI::Ril::V1_0::Rssi &rssi)
{
    HRilRssi *rilRssi = (HRilRssi *)response;
    rssi.gw.rxlev = rilRssi->gsmRssi.rxlev;
    rssi.gw.ber = rilRssi->gsmRssi.ber;
    rssi.cdma.absoluteRssi = rilRssi->cdmaRssi.absoluteRssi;
    rssi.cdma.ecno = rilRssi->cdmaRssi.ecno;
    rssi.wcdma.rxlev = rilRssi->wcdmaRssi.rxlev;
    rssi.wcdma.ber = rilRssi->wcdmaRssi.ber;
    rssi.wcdma.ecio = rilRssi->wcdmaRssi.ecio;
    rssi.wcdma.rscp = rilRssi->wcdmaRssi.rscp;
    rssi.lte.rxlev = rilRssi->lteRssi.rxlev;
    rssi.lte.rsrp = rilRssi->lteRssi.rsrp;
    rssi.lte.rsrq = rilRssi->lteRssi.rsrq;
    rssi.lte.snr = rilRssi->lteRssi.snr;
    rssi.tdScdma.rscp = rilRssi->tdScdmaRssi.rscp;
    rssi.nr.rsrp = rilRssi->nrRssi.rsrp;
    rssi.nr.rsrq = rilRssi->nrRssi.rsrq;
    rssi.nr.sinr = rilRssi->nrRssi.sinr;
    TELEPHONY_LOGD("ExchangeRilRssiToHdf rxlev:%{public}d, rsrp:%{public}d", rssi.lte.rxlev, rssi.lte.rsrp);
}

void HRilNetwork::BuildOperatorList(
    HDI::Ril::V1_0::AvailableNetworkList &availableNetworkList, const void *response, size_t responseLen)
{
    size_t numStrings = responseLen / sizeof(AvailableOperInfo *);
    HDI::Ril::V1_0::AvailableNetworkInfo operInfo = {};
    availableNetworkList.itemNum = numStrings;
    TELEPHONY_LOGI(
        "BuildOperatorList availableNetworkList.itemNum: %{public}lu", static_cast<unsigned long>(numStrings));
    for (size_t i = 0; i < numStrings; i++) {
        AvailableOperInfo *curPtr = ((AvailableOperInfo **)response)[i];
        if (curPtr != nullptr) {
            operInfo.status = curPtr->status;
            TELEPHONY_LOGI("operInfo.status:%{public}d", curPtr->status);
            operInfo.longName = (curPtr->longName == nullptr) ? "" : curPtr->longName;
            TELEPHONY_LOGI("operInfo.longName:%{public}s", curPtr->longName);
            operInfo.numeric = (curPtr->numeric == nullptr) ? "" : curPtr->numeric;
            TELEPHONY_LOGI("operInfo.numeric:%{public}s", curPtr->numeric);
            operInfo.shortName = (curPtr->shortName == nullptr) ? "" : curPtr->shortName;
            TELEPHONY_LOGI("operInfo.shortName:%{public}s", curPtr->shortName);
            operInfo.rat = curPtr->rat;
            TELEPHONY_LOGI("operInfo.rat:%{public}d", curPtr->rat);
            availableNetworkList.availableNetworkInfo.push_back(operInfo);
        }
    }
}

void HRilNetwork::FillCellNearbyInfo(HDI::Ril::V1_0::CellNearbyInfo &cellInfo, const CellInfo *cellPtr)
{
    cellInfo.ratType = cellPtr->ratType;
    switch (cellPtr->ratType) {
        case NETWORK_TYPE_GSM:
            cellInfo.serviceCells.gsm.band = cellPtr->ServiceCellParas.gsm.band;
            cellInfo.serviceCells.gsm.arfcn = cellPtr->ServiceCellParas.gsm.arfcn;
            cellInfo.serviceCells.gsm.bsic = cellPtr->ServiceCellParas.gsm.bsic;
            cellInfo.serviceCells.gsm.cellId = cellPtr->ServiceCellParas.gsm.cellId;
            cellInfo.serviceCells.gsm.lac = cellPtr->ServiceCellParas.gsm.lac;
            cellInfo.serviceCells.gsm.rxlev = cellPtr->ServiceCellParas.gsm.rxlev;
            break;
        case NETWORK_TYPE_LTE:
            cellInfo.serviceCells.lte.arfcn = cellPtr->ServiceCellParas.lte.arfcn;
            cellInfo.serviceCells.lte.pci = cellPtr->ServiceCellParas.lte.pci;
            cellInfo.serviceCells.lte.rsrp = cellPtr->ServiceCellParas.lte.rsrp;
            cellInfo.serviceCells.lte.rsrq = cellPtr->ServiceCellParas.lte.rsrq;
            cellInfo.serviceCells.lte.rxlev = cellPtr->ServiceCellParas.lte.rxlev;
            break;
        case NETWORK_TYPE_WCDMA:
            cellInfo.serviceCells.wcdma.arfcn = cellPtr->ServiceCellParas.wcdma.arfcn;
            cellInfo.serviceCells.wcdma.psc = cellPtr->ServiceCellParas.wcdma.psc;
            cellInfo.serviceCells.wcdma.rscp = cellPtr->ServiceCellParas.wcdma.rscp;
            cellInfo.serviceCells.wcdma.ecno = cellPtr->ServiceCellParas.wcdma.ecno;
            break;
        case NETWORK_TYPE_CDMA:
            FillCellNearbyInfoCdma(cellInfo, cellPtr);
            break;
        case NETWORK_TYPE_TDSCDMA:
            FillCellNearbyInfoTdscdma(cellInfo, cellPtr);
            break;
        case NETWORK_TYPE_NR:
            cellInfo.serviceCells.nr.nrArfcn = cellPtr->ServiceCellParas.nr.nrArfcn;
            cellInfo.serviceCells.nr.pci = cellPtr->ServiceCellParas.nr.pci;
            cellInfo.serviceCells.nr.tac = cellPtr->ServiceCellParas.nr.tac;
            cellInfo.serviceCells.nr.nci = cellPtr->ServiceCellParas.nr.nci;
            break;
        default:
            cellInfo.serviceCells.gsm.band = 0;
            cellInfo.serviceCells.gsm.arfcn = 0;
            cellInfo.serviceCells.gsm.bsic = 0;
            cellInfo.serviceCells.gsm.cellId = 0;
            cellInfo.serviceCells.gsm.lac = 0;
            cellInfo.serviceCells.gsm.rxlev = 0;
            break;
    }
}

void HRilNetwork::FillCellNearbyInfoTdscdma(HDI::Ril::V1_0::CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr)
{
    cellInfo.serviceCells.tdscdma.arfcn = hrilCellPtr->ServiceCellParas.tdscdma.arfcn;
    cellInfo.serviceCells.tdscdma.syncId = hrilCellPtr->ServiceCellParas.tdscdma.syncId;
    cellInfo.serviceCells.tdscdma.sc = hrilCellPtr->ServiceCellParas.tdscdma.sc;
    cellInfo.serviceCells.tdscdma.cellId = hrilCellPtr->ServiceCellParas.tdscdma.cellId;
    cellInfo.serviceCells.tdscdma.lac = hrilCellPtr->ServiceCellParas.tdscdma.lac;
    cellInfo.serviceCells.tdscdma.rscp = hrilCellPtr->ServiceCellParas.tdscdma.rscp;
    cellInfo.serviceCells.tdscdma.drx = hrilCellPtr->ServiceCellParas.tdscdma.drx;
    cellInfo.serviceCells.tdscdma.rac = hrilCellPtr->ServiceCellParas.tdscdma.rac;
    cellInfo.serviceCells.tdscdma.cpid = hrilCellPtr->ServiceCellParas.tdscdma.cpid;
}

void HRilNetwork::FillCellNearbyInfoCdma(HDI::Ril::V1_0::CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr)
{
    cellInfo.serviceCells.cdma.systemId = hrilCellPtr->ServiceCellParas.cdma.systemId;
    cellInfo.serviceCells.cdma.networkId = hrilCellPtr->ServiceCellParas.cdma.networkId;
    cellInfo.serviceCells.cdma.baseId = hrilCellPtr->ServiceCellParas.cdma.baseId;
    cellInfo.serviceCells.cdma.zoneId = hrilCellPtr->ServiceCellParas.cdma.zoneId;
    cellInfo.serviceCells.cdma.pilotPn = hrilCellPtr->ServiceCellParas.cdma.pilotPn;
    cellInfo.serviceCells.cdma.pilotStrength = hrilCellPtr->ServiceCellParas.cdma.pilotStrength;
    cellInfo.serviceCells.cdma.channel = hrilCellPtr->ServiceCellParas.cdma.channel;
    cellInfo.serviceCells.cdma.longitude = hrilCellPtr->ServiceCellParas.cdma.longitude;
    cellInfo.serviceCells.cdma.latitude = hrilCellPtr->ServiceCellParas.cdma.latitude;
}

int32_t HRilNetwork::BuildNeighboringCellList(
    HDI::Ril::V1_0::CellListNearbyInfo &cellInfoList, const void *response, size_t responseLen)
{
    const CellInfoList *temp = reinterpret_cast<const CellInfoList *>(response);
    cellInfoList.itemNum = temp->itemNum;
    TELEPHONY_LOGI("BuildNeighboringCellList cellInfoList.itemNum = %{public}d", cellInfoList.itemNum);
    for (int32_t i = 0; i < temp->itemNum; i++) {
        HDI::Ril::V1_0::CellNearbyInfo cellInfo;
        CellInfo *cell = temp->cellNearbyInfo + i;
        if (cell == nullptr) {
            TELEPHONY_LOGE("BuildNeighboringCellList cell is nullptr");
            return HRIL_ERR_GENERIC_FAILURE;
        }
        FillCellNearbyInfo(cellInfo, cell);
        cellInfoList.cellNearbyInfo.push_back(cellInfo);
    }
    return HRIL_ERR_SUCCESS;
}

void HRilNetwork::FillCellInfoType(
    HDI::Ril::V1_0::CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *hrilCellInfoVendor)
{
    if (hrilCellInfoVendor->ratType == NETWORK_TYPE_WCDMA) {
        cellInfo.serviceCells.wcdma.arfcn = hrilCellInfoVendor->ServiceCellParas.wcdma.arfcn;
        cellInfo.serviceCells.wcdma.cellId = hrilCellInfoVendor->ServiceCellParas.wcdma.cellId;
        cellInfo.serviceCells.wcdma.psc = hrilCellInfoVendor->ServiceCellParas.wcdma.psc;
        cellInfo.serviceCells.wcdma.lac = hrilCellInfoVendor->ServiceCellParas.wcdma.lac;
        cellInfo.serviceCells.wcdma.rxlev = hrilCellInfoVendor->ServiceCellParas.wcdma.rxlev;
        cellInfo.serviceCells.wcdma.rscp = hrilCellInfoVendor->ServiceCellParas.wcdma.rscp;
        cellInfo.serviceCells.wcdma.ecno = hrilCellInfoVendor->ServiceCellParas.wcdma.ecno;
        cellInfo.serviceCells.wcdma.ura = hrilCellInfoVendor->ServiceCellParas.wcdma.ura;
        cellInfo.serviceCells.wcdma.drx = hrilCellInfoVendor->ServiceCellParas.wcdma.drx;
    } else if (hrilCellInfoVendor->ratType == NETWORK_TYPE_CDMA) {
        cellInfo.serviceCells.cdma.systemId = hrilCellInfoVendor->ServiceCellParas.cdma.systemId;
        cellInfo.serviceCells.cdma.networkId = hrilCellInfoVendor->ServiceCellParas.cdma.networkId;
        cellInfo.serviceCells.cdma.baseId = hrilCellInfoVendor->ServiceCellParas.cdma.baseId;
        cellInfo.serviceCells.cdma.zoneId = hrilCellInfoVendor->ServiceCellParas.cdma.zoneId;
        cellInfo.serviceCells.cdma.pilotPn = hrilCellInfoVendor->ServiceCellParas.cdma.pilotPn;
        cellInfo.serviceCells.cdma.pilotStrength = hrilCellInfoVendor->ServiceCellParas.cdma.pilotStrength;
        cellInfo.serviceCells.cdma.channel = hrilCellInfoVendor->ServiceCellParas.cdma.channel;
        cellInfo.serviceCells.cdma.longitude = hrilCellInfoVendor->ServiceCellParas.cdma.longitude;
        cellInfo.serviceCells.cdma.latitude = hrilCellInfoVendor->ServiceCellParas.cdma.latitude;
    } else if (hrilCellInfoVendor->ratType == NETWORK_TYPE_TDSCDMA) {
        cellInfo.serviceCells.tdscdma.arfcn = hrilCellInfoVendor->ServiceCellParas.tdscdma.arfcn;
        cellInfo.serviceCells.tdscdma.syncId = hrilCellInfoVendor->ServiceCellParas.tdscdma.syncId;
        cellInfo.serviceCells.tdscdma.sc = hrilCellInfoVendor->ServiceCellParas.tdscdma.sc;
        cellInfo.serviceCells.tdscdma.cellId = hrilCellInfoVendor->ServiceCellParas.tdscdma.cellId;
        cellInfo.serviceCells.tdscdma.lac = hrilCellInfoVendor->ServiceCellParas.tdscdma.lac;
        cellInfo.serviceCells.tdscdma.rscp = hrilCellInfoVendor->ServiceCellParas.tdscdma.rscp;
        cellInfo.serviceCells.tdscdma.drx = hrilCellInfoVendor->ServiceCellParas.tdscdma.drx;
        cellInfo.serviceCells.tdscdma.rac = hrilCellInfoVendor->ServiceCellParas.tdscdma.rac;
        cellInfo.serviceCells.tdscdma.cpid = hrilCellInfoVendor->ServiceCellParas.tdscdma.cpid;
    }
}

void HRilNetwork::FillCurrentCellInfo(
    HDI::Ril::V1_0::CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *cellInfoVendor)
{
    cellInfo.ratType = cellInfoVendor->ratType;
    cellInfo.mcc = cellInfoVendor->mcc;
    cellInfo.mnc = cellInfoVendor->mnc;
    switch (cellInfoVendor->ratType) {
        case NETWORK_TYPE_GSM:
            cellInfo.serviceCells.gsm.band = cellInfoVendor->ServiceCellParas.gsm.band;
            cellInfo.serviceCells.gsm.arfcn = cellInfoVendor->ServiceCellParas.gsm.arfcn;
            cellInfo.serviceCells.gsm.bsic = cellInfoVendor->ServiceCellParas.gsm.bsic;
            cellInfo.serviceCells.gsm.cellId = cellInfoVendor->ServiceCellParas.gsm.cellId;
            cellInfo.serviceCells.gsm.lac = cellInfoVendor->ServiceCellParas.gsm.lac;
            cellInfo.serviceCells.gsm.rxlev = cellInfoVendor->ServiceCellParas.gsm.rxlev;
            cellInfo.serviceCells.gsm.rxQuality = cellInfoVendor->ServiceCellParas.gsm.rxQuality;
            cellInfo.serviceCells.gsm.ta = cellInfoVendor->ServiceCellParas.gsm.ta;
            break;
        case NETWORK_TYPE_LTE:
            cellInfo.serviceCells.lte.arfcn = cellInfoVendor->ServiceCellParas.lte.arfcn;
            cellInfo.serviceCells.lte.cellId = cellInfoVendor->ServiceCellParas.lte.cellId;
            cellInfo.serviceCells.lte.pci = cellInfoVendor->ServiceCellParas.lte.pci;
            cellInfo.serviceCells.lte.tac = cellInfoVendor->ServiceCellParas.lte.tac;
            cellInfo.serviceCells.lte.rsrp = cellInfoVendor->ServiceCellParas.lte.rsrp;
            cellInfo.serviceCells.lte.rsrq = cellInfoVendor->ServiceCellParas.lte.rsrq;
            cellInfo.serviceCells.lte.rssi = cellInfoVendor->ServiceCellParas.lte.rssi;
            break;
        case NETWORK_TYPE_WCDMA:
        case NETWORK_TYPE_CDMA:
        case NETWORK_TYPE_TDSCDMA:
            FillCellInfoType(cellInfo, cellInfoVendor);
            break;
        case NETWORK_TYPE_NR:
            cellInfo.serviceCells.nr.nrArfcn = cellInfoVendor->ServiceCellParas.nr.nrArfcn;
            cellInfo.serviceCells.nr.pci = cellInfoVendor->ServiceCellParas.nr.pci;
            cellInfo.serviceCells.nr.tac = cellInfoVendor->ServiceCellParas.nr.tac;
            cellInfo.serviceCells.nr.nci = cellInfoVendor->ServiceCellParas.nr.nci;
            break;
        default:
            cellInfo.serviceCells.wcdma.arfcn = 0;
            cellInfo.serviceCells.wcdma.cellId = 0;
            cellInfo.serviceCells.wcdma.psc = 0;
            cellInfo.serviceCells.wcdma.lac = 0;
            cellInfo.serviceCells.wcdma.rxlev = 0;
            cellInfo.serviceCells.wcdma.rscp = 0;
            cellInfo.serviceCells.wcdma.ecno = 0;
            cellInfo.serviceCells.wcdma.drx = 0;
            cellInfo.serviceCells.wcdma.ura = 0;
            break;
    }
}

int32_t HRilNetwork::BuildCurrentCellList(HDI::Ril::V1_0::CellListCurrentInfo &cellInfoList,
    const void *response, size_t responseLen)
{
    const CurrentCellInfoList *temp = reinterpret_cast<const CurrentCellInfoList *>(response);
    cellInfoList.itemNum = temp->itemNum;
    TELEPHONY_LOGI("BuildCurrentCellList cellInfoList.itemNum = %{public}d", cellInfoList.itemNum);
    for (int32_t i = 0; i < temp->itemNum; i++) {
        HDI::Ril::V1_0::CurrentCellInfo cellInfo;
        CurrentCellInfoVendor *cell = temp->currentCellInfo + i;
        if (cell == nullptr) {
            TELEPHONY_LOGE("BuildCurrentCellList cell is nullptr");
            return HRIL_ERR_GENERIC_FAILURE;
        }
        FillCurrentCellInfo(cellInfo, cell);
        cellInfoList.cellCurrentInfo.push_back(cellInfo);
    }
    return HRIL_ERR_SUCCESS;
}

bool HRilNetwork::IsNetworkResponse(uint32_t code)
{
    return ((code >= HREQ_NETWORK_BASE) && (code < HREQ_COMMON_BASE));
}

bool HRilNetwork::IsNetworkNotification(uint32_t code)
{
    return ((code >= HNOTI_NETWORK_BASE) && (code < HNOTI_COMMON_BASE));
}

bool HRilNetwork::IsNetworkRespOrNotify(uint32_t code)
{
    return IsNetworkResponse(code) || IsNetworkNotification(code);
}

void HRilNetwork::RegisterNetworkFuncs(const HRilNetworkReq *networkFuncs)
{
    networkFuncs_ = networkFuncs;
}
} // namespace Telephony
} // namespace OHOS
