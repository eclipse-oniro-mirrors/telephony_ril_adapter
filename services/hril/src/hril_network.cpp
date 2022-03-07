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
    notiMemberFuncMap_[HNOTI_NETWORK_IMS_REG_STATUS_UPDATED] = &HRilNetwork::NetworkImsRegStatusUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_PS_REG_STATUS_UPDATED] = &HRilNetwork::NetworkPsRegStatusUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_PHY_CHNL_CFG_UPDATED] = &HRilNetwork::NetworkPhyChnlCfgUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_CURRENT_CELL_UPDATED] = &HRilNetwork::NetworkCurrentCellUpdated;

    // request
    reqMemberFuncMap_[HREQ_NETWORK_GET_IMS_REG_STATUS] = &HRilNetwork::GetImsRegStatus;
    reqMemberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &HRilNetwork::GetSignalStrength;
    reqMemberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &HRilNetwork::GetCsRegStatus;
    reqMemberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &HRilNetwork::GetPsRegStatus;
    reqMemberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &HRilNetwork::GetOperatorInfo;
    reqMemberFuncMap_[HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION] = &HRilNetwork::GetNetworkSearchInformation;
    reqMemberFuncMap_[HREQ_NETWORK_GET_NETWORK_SELECTION_MODE] = &HRilNetwork::GetNetworkSelectionMode;
    reqMemberFuncMap_[HREQ_NETWORK_SET_NETWORK_SELECTION_MODE] = &HRilNetwork::SetNetworkSelectionMode;
    reqMemberFuncMap_[HREQ_NETWORK_SET_PREFERRED_NETWORK] = &HRilNetwork::SetPreferredNetwork;
    reqMemberFuncMap_[HREQ_NETWORK_GET_PREFERRED_NETWORK] = &HRilNetwork::GetPreferredNetwork;
    reqMemberFuncMap_[HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST] = &HRilNetwork::GetNeighboringCellInfoList;
    reqMemberFuncMap_[HREQ_NETWORK_GET_CURRENT_CELL_INFO] = &HRilNetwork::GetCurrentCellInfo;
    reqMemberFuncMap_[HREQ_NETWORK_GET_RADIO_CAPABILITY] = &HRilNetwork::GetRadioCapability;
    reqMemberFuncMap_[HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG] = &HRilNetwork::GetPhysicalChannelConfig;
    reqMemberFuncMap_[HREQ_NETWORK_SET_LOCATE_UPDATES] = &HRilNetwork::SetLocateUpdates;

    // Response
    respMemberFuncMap_[HREQ_NETWORK_GET_IMS_REG_STATUS] = &HRilNetwork::GetImsRegStatusResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &HRilNetwork::GetSignalStrengthResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &HRilNetwork::GetCsRegStatusResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &HRilNetwork::GetPsRegStatusResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &HRilNetwork::GetOperatorInfoResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION] =
        &HRilNetwork::GetNetworkSearchInformationResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_NETWORK_SELECTION_MODE] = &HRilNetwork::GetNetworkSelectionModeResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_NETWORK_SELECTION_MODE] = &HRilNetwork::SetNetworkSelectionModeResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_PREFERRED_NETWORK] = &HRilNetwork::SetPreferredNetworkResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_PREFERRED_NETWORK] = &HRilNetwork::GetPreferredNetworkResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST] = &HRilNetwork::GetNeighboringCellInfoListResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_CURRENT_CELL_INFO] = &HRilNetwork::GetCurrentCellInfoResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_RADIO_CAPABILITY] = &HRilNetwork::GetRadioCapabilityResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG] = &HRilNetwork::GetPhysicalChannelConfigResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_LOCATE_UPDATES] = &HRilNetwork::SetLocateUpdatesResponse;
}

int32_t HRilNetwork::GetImsRegStatus(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetImsRegStatus == nullptr)) {
        TELEPHONY_LOGE("GetImsRegStatus::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_IMS_REG_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetImsRegStatus(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetSignalStrength(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetSignalStrength == nullptr)) {
        TELEPHONY_LOGE("GetSignalStrength::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_SIGNAL_STRENGTH);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetSignalStrength(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetRadioCapability(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetRadioCapability == nullptr)) {
        TELEPHONY_LOGE("GetRadioCapability::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_RADIO_CAPABILITY);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetRadioCapability(requestInfo);
    return HRIL_ERR_SUCCESS;
}

void HRilNetwork::ExchangeRilRssiToHdf(const void *response, size_t responseLen, Rssi &rssi)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("response is nullptr");
        return;
    }
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

    TELEPHONY_LOGI("rxlev:%{public}d, rsrp:%{public}d", rssi.lte.rxlev, rssi.lte.rsrp);
}

int32_t HRilNetwork::GetImsRegStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    const int32_t IMS_EXT_INFO_NOTIFY_TYPE = 2;
    ImsRegStatusInfo imsRegResponse = {};
    imsRegResponse.notifyType = IMS_EXT_INFO_NOTIFY_TYPE;
    imsRegResponse.regInfo = 0;
    imsRegResponse.extInfo = 0;
    TELEPHONY_LOGI("GetImsRegStatusResponse ---> response %{public}p", response);
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(HRilImsRegStatusInfo)))) {
        TELEPHONY_LOGE("GetImsRegStatusResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGE("response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilImsRegStatusInfo *hrilImsRegStatusInfo = static_cast<const HRilImsRegStatusInfo *>(response);
        imsRegResponse.notifyType = hrilImsRegStatusInfo->notifyType;
        imsRegResponse.regInfo = hrilImsRegStatusInfo->regInfo;
        imsRegResponse.extInfo = hrilImsRegStatusInfo->extInfo;
    }
    return ResponseMessageParcel(responseInfo, imsRegResponse, requestNum);
}

int32_t HRilNetwork::GetSignalStrengthResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    Rssi event = {};
    event.slotId = GetSlotId();
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(HRilRssi)))) {
        TELEPHONY_LOGE("GetSignalStrengthResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if (response == nullptr || responseLen != sizeof(HRilRssi)) {
        TELEPHONY_LOGE("GetSignalStrengthResponse: Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        ExchangeRilRssiToHdf(response, responseLen, event);
    }
    return ResponseBuffer(requestNum, &responseInfo, sizeof(responseInfo), &event, sizeof(event));
}

int32_t HRilNetwork::GetCsRegStatus(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetCsRegStatus == nullptr)) {
        TELEPHONY_LOGE("GetCsRegStatus::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_CS_REG_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetCsRegStatus(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetCsRegStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    CsRegStatusInfo csRegResponse = {};
    csRegResponse.notifyType = REG_NOTIFY_STAT_LAC_CELLID;
    csRegResponse.regStatus = NO_REG_MT_NO_SEARCH;
    csRegResponse.lacCode = 0;
    csRegResponse.cellId = 0;
    csRegResponse.radioTechnology = RADIO_TECHNOLOGY_UNKNOWN;
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(HRilRegStatusInfo)))) {
        TELEPHONY_LOGE("GetCsRegStatusResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGE("GetCsRegStatusResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
        csRegResponse.notifyType = hrilRegStatusInfo->notifyMode;
        csRegResponse.regStatus = hrilRegStatusInfo->regStatus;
        csRegResponse.lacCode = hrilRegStatusInfo->lacCode;
        csRegResponse.cellId = hrilRegStatusInfo->cellId;
        csRegResponse.radioTechnology = hrilRegStatusInfo->actType;
        TELEPHONY_LOGI(
            "GetCsRegStatusResponse n:%{public}d, regStatus:%{public}d, "
            "lacCode:%{public}d, cellId:%{public}d, actType:%{public}d",
            csRegResponse.notifyType, csRegResponse.regStatus, csRegResponse.lacCode, csRegResponse.cellId,
            csRegResponse.radioTechnology);
    }
    return ResponseMessageParcel(responseInfo, csRegResponse, requestNum);
}

int32_t HRilNetwork::GetOperatorInfo(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetOperatorInfo == nullptr)) {
        TELEPHONY_LOGE("GetOperatorInfo::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_OPERATOR_INFO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetOperatorInfo(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetNetworkSearchInformation(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetNetworkSearchInformation == nullptr)) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetNetworkSearchInformation(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetNetworkSelectionMode(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetNetworkSelectionMode == nullptr)) {
        TELEPHONY_LOGE("GetNetworkSelectionMode::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetNetworkSelectionMode::miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_NETWORK_SELECTION_MODE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetNetworkSelectionMode::Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetNetworkSelectionMode(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::SetNetworkSelectionMode(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->SetNetworkSelectionMode == nullptr)) {
        TELEPHONY_LOGE("SetNetworkSelectionMode::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    struct SetNetworkModeInfo setNetworkModeInfo = {};
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!setNetworkModeInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilSetNetworkModeInfo setModeInfo = {};
    setModeInfo.selectMode = setNetworkModeInfo.selectMode;
    TELEPHONY_LOGI("setModeInfo = %{public}d", setModeInfo.selectMode);
    if (!ConvertToString(&setModeInfo.oper, setNetworkModeInfo.oper)) {
        TELEPHONY_LOGE("SetNetworkSelectionMode::ConvertToString failed");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(setNetworkModeInfo.serial, HREQ_NETWORK_SET_NETWORK_SELECTION_MODE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("SetNetworkSelectionMode::Create Request is fail");
        SafeFrees(setModeInfo.oper);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    TELEPHONY_LOGI("SetNetworkSelectionMode selectMode:%{public}s", setModeInfo.oper);
    networkFuncs_->SetNetworkSelectionMode(requestInfo, &setModeInfo);
    SafeFrees(setModeInfo.oper);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetPreferredNetwork(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetPreferredNetwork == nullptr)) {
        TELEPHONY_LOGE("GetPreferredNetwork::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    TELEPHONY_LOGI("serial = %{public}d", serial);
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_PREFERRED_NETWORK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetPreferredNetwork::Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetPreferredNetwork(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::SetPreferredNetwork(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->SetPreferredNetwork == nullptr)) {
        TELEPHONY_LOGE("SetPreferredNetwork::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t netType = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &netType)) {
        TELEPHONY_LOGE("miss netType parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_SET_PREFERRED_NETWORK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("SetPreferredNetwork::Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    TELEPHONY_LOGI("SetPreferredNetwork netType:%{public}d", netType);
    networkFuncs_->SetPreferredNetwork(requestInfo, &netType);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetPhysicalChannelConfig(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetPhysicalChannelConfig == nullptr)) {
        TELEPHONY_LOGE("GetPhysicalChannelConfig::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetPhysicalChannelConfig::miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetPhysicalChannelConfig::Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetPhysicalChannelConfig(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::SetLocateUpdates(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->SetLocateUpdates == nullptr)) {
        TELEPHONY_LOGE("SetLocateUpdates::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("SetLocateUpdates::miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t regNotifyMode = 0;
    if (!HdfSbufReadInt32(data, &regNotifyMode)) {
        TELEPHONY_LOGE("miss regNotifyMode parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilRegNotifyMode mode = REG_NOTIFY_STAT_LAC_CELLID;
    if ((regNotifyMode >= REG_NOT_NOTIFY) && (regNotifyMode <= REG_NOTIFY_STAT_LAC_CELLID)) {
        mode = static_cast<HRilRegNotifyMode>(regNotifyMode);
    } else {
        TELEPHONY_LOGE("Invalid regNotifyMode parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_SET_LOCATE_UPDATES);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("SetLocateUpdates::Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->SetLocateUpdates(requestInfo, mode);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetOperatorInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    struct OperatorInfoResult operatorInfoResult = {};
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen == 0))) {
        TELEPHONY_LOGE("GetOperatorInfoResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGE("Create Request is fail");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        char **resp = static_cast<char **>(const_cast<void *>(response));
        operatorInfoResult.longName = resp[HRIL_LONE_NAME];
        operatorInfoResult.shortName = resp[HRIL_SHORT_NAME];
        operatorInfoResult.numeric = resp[HRIL_NUMERIC];
        TELEPHONY_LOGI("GetOperatorInfoResponse longName:%{public}s, shortName:%{public}s, numeric:%{public}s",
            operatorInfoResult.longName.c_str(), operatorInfoResult.shortName.c_str(),
            operatorInfoResult.numeric.c_str());
    }
    return ResponseMessageParcel(responseInfo, operatorInfoResult, requestNum);
}

int32_t HRilNetwork::NetworkCsRegStatusUpdated(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
    if (hrilRegStatusInfo == nullptr) {
        TELEPHONY_LOGE("hrilRegStatusInfo is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    CsRegStatusInfo regStatusInfoNotify = {};
    regStatusInfoNotify.notifyType = hrilRegStatusInfo->notifyMode;
    regStatusInfoNotify.regStatus = hrilRegStatusInfo->regStatus;
    regStatusInfoNotify.lacCode = hrilRegStatusInfo->lacCode;
    regStatusInfoNotify.cellId = hrilRegStatusInfo->cellId;
    regStatusInfoNotify.radioTechnology = hrilRegStatusInfo->actType;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!regStatusInfoNotify.Marshalling(*parcel.get())) {
        TELEPHONY_LOGE("Marshalling is fail");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NetworkCsRegStatusUpdated is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_CS_REG_STATUS_UPDATED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::NetworkPhyChnlCfgUpdated(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    const HRilChannelConfigList *hrilChannelConfigList = static_cast<const HRilChannelConfigList *>(response);
    if (hrilChannelConfigList == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    ChannelConfigInfoList phyChnlCfgList = {};
    phyChnlCfgList.itemNum = hrilChannelConfigList->itemNum;
    for (int32_t i = 0; i < phyChnlCfgList.itemNum; i++) {
        PhysicalChannelConfig cfg;
        cfg.cellConnStatus = hrilChannelConfigList->channelConfigs[i].cellConnStatus;
        cfg.cellBandwidth = hrilChannelConfigList->channelConfigs[i].cellBandwidth;
        cfg.ratType = hrilChannelConfigList->channelConfigs[i].ratType;
        cfg.freqRange = hrilChannelConfigList->channelConfigs[i].freqRange;
        cfg.channelNum = hrilChannelConfigList->channelConfigs[i].channelNum;
        cfg.physicalCellId = hrilChannelConfigList->channelConfigs[i].physicalCellId;
        cfg.contextIdNum = hrilChannelConfigList->channelConfigs[i].contextIdNum;
        for (int32_t j = 0; j < cfg.contextIdNum; j++) {
            cfg.contextIds.push_back(hrilChannelConfigList->channelConfigs[i].contextIds[j]);
        }
        phyChnlCfgList.channelConfigInfos.push_back(cfg);
    }
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!phyChnlCfgList.Marshalling(*parcel.get())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    ServiceNotifyDispatcher(HNOTI_NETWORK_PHY_CHNL_CFG_UPDATED, dataSbuf);
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::FixCurrentCellList(
    CellListCurrentInfo &cellInfoList, const CurrentCellInfoList *hrilCurrentCellList)
{
    if (hrilCurrentCellList == nullptr) {
        TELEPHONY_LOGE("BuildCurrentCellList: hrilCurrentCellList nullptr!");
        return HRIL_ERR_GENERIC_FAILURE;
    } else {
        cellInfoList.itemNum = hrilCurrentCellList->itemNum;
        TELEPHONY_LOGI("BuildCurrentCellList:  cellInfoList.itemNum = %{public}d", cellInfoList.itemNum);
        for (int32_t i = 0; i < cellInfoList.itemNum; i++) {
            CurrentCellInfo cellInfo;
            CurrentCellInfoVendor *cellPtr = hrilCurrentCellList->currentCellInfo + i;
            if (cellPtr == nullptr) {
                TELEPHONY_LOGE("BuildNeighboringCellList: response item error!");
                return HRIL_ERR_GENERIC_FAILURE;
            }
            FillCurrentCellInfo(cellInfo, cellPtr);
            cellInfoList.cellCurrentInfo.push_back(cellInfo);
        }
        return HRIL_ERR_SUCCESS;
    }
}

int32_t HRilNetwork::NetworkCurrentCellUpdated(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    const CurrentCellInfoList *hrilCurrentCellList = static_cast<const CurrentCellInfoList *>(response);
    if (hrilCurrentCellList == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    CellListCurrentInfo cellListCurrentInfo;
    cellListCurrentInfo.itemNum = 0;
    cellListCurrentInfo.cellCurrentInfo.clear();
    if (FixCurrentCellList(cellListCurrentInfo, hrilCurrentCellList) != 0) {
        return HRIL_ERR_GENERIC_FAILURE;
    }
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!cellListCurrentInfo.Marshalling(*parcel.get())) {
        return HRIL_ERR_GENERIC_FAILURE;
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSbufRecycle(dataSbuf);
        TELEPHONY_LOGE("Marshalling is fail");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    ServiceNotifyDispatcher(HNOTI_NETWORK_CURRENT_CELL_UPDATED, dataSbuf);
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::NetworkPsRegStatusUpdated(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
    if (hrilRegStatusInfo == nullptr) {
        TELEPHONY_LOGE("hrilRegStatusInfo is nullptr");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    CsRegStatusInfo regStatusInfoNotify = {};
    regStatusInfoNotify.notifyType = hrilRegStatusInfo->notifyMode;
    regStatusInfoNotify.regStatus = hrilRegStatusInfo->regStatus;
    regStatusInfoNotify.lacCode = hrilRegStatusInfo->lacCode;
    regStatusInfoNotify.cellId = hrilRegStatusInfo->cellId;
    regStatusInfoNotify.radioTechnology = hrilRegStatusInfo->actType;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!regStatusInfoNotify.Marshalling(*parcel.get())) {
        HdfSbufRecycle(dataSbuf);
        TELEPHONY_LOGE("Marshalling is fail");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NetworkPsRegStatusUpdated is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_PS_REG_STATUS_UPDATED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::NetworkImsRegStatusUpdated(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    const HRilImsRegStatusInfo *hrilImsRegStatusInfo = static_cast<const HRilImsRegStatusInfo *>(response);
    if (hrilImsRegStatusInfo == nullptr) {
        TELEPHONY_LOGE("hrilRegStatusInfo is nullptr");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    ImsRegStatusInfo imsRegStatusInfoNotify = {};
    imsRegStatusInfoNotify.notifyType = hrilImsRegStatusInfo->notifyType;
    imsRegStatusInfoNotify.regInfo = hrilImsRegStatusInfo->regInfo;
    imsRegStatusInfoNotify.extInfo = hrilImsRegStatusInfo->extInfo;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!imsRegStatusInfoNotify.Marshalling(*parcel.get())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NetworkImsRegStatusUpdated is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_IMS_REG_STATUS_UPDATED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::SignalStrengthUpdated(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    Rssi rssi = {0};

    if (response == nullptr || responseLen != sizeof(HRilRssi)) {
        TELEPHONY_LOGE("SignalStrengthUpdated: Invalid response");
    } else {
        ExchangeRilRssiToHdf(response, responseLen, rssi);
    }
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&rssi, sizeof(rssi))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in SignalStrengthUpdated is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::NetworkTimeUpdated(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));

    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in NetworkTimeUpdated is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NetworkTimeUpdated is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_TIME_UPDATED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::NetworkTimeZoneUpdated(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));

    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in NetworkTimeZoneUpdated is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }

    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NetworkTimeZoneUpdated is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_TIME_ZONE_UPDATED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    TELEPHONY_LOGI("serviceCallbackNotify_->dispatcher->Dispatch ret %{public}d", ret);
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetPsRegStatus(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetPsRegStatus == nullptr)) {
        TELEPHONY_LOGE("GetPsRegStatus::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        HdfSbufRecycle(data);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_PS_REG_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetPsRegStatus(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetNeighboringCellInfoList(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetNeighboringCellInfoList == nullptr)) {
        TELEPHONY_LOGE("GetNeighboringCellInfoList::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetNeighboringCellInfoList miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetNeighboringCellInfoList::Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetNeighboringCellInfoList(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetCurrentCellInfo(struct HdfSBuf *data)
{
    if ((networkFuncs_ == nullptr) || (networkFuncs_->GetCurrentCellInfo == nullptr)) {
        TELEPHONY_LOGE("GetCurrentCellInfo::networkFuncs_:%{public}p", networkFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetCurrentCellInfo miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_NETWORK_GET_CURRENT_CELL_INFO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetCurrentCellInfo::Create Request is fail");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    networkFuncs_->GetCurrentCellInfo(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilNetwork::GetPsRegStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    PsRegStatusResultInfo psRegResponse = {};
    psRegResponse.notifyType = REG_NOTIFY_STAT_LAC_CELLID;
    psRegResponse.regStatus = REG_MT_UNKNOWN;
    psRegResponse.lacCode = 0;
    psRegResponse.cellId = 0;
    psRegResponse.radioTechnology = RADIO_TECHNOLOGY_UNKNOWN;
    psRegResponse.isDcNrRestricted = false;
    psRegResponse.isNrAvailable = false;
    psRegResponse.isEnDcAvailable = false;
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(HRilRegStatusInfo)))) {
        TELEPHONY_LOGE("GetPsRegStatusResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGE("GetPsRegStatusResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
        psRegResponse.notifyType = hrilRegStatusInfo->notifyMode;
        psRegResponse.regStatus = hrilRegStatusInfo->regStatus;
        psRegResponse.lacCode = hrilRegStatusInfo->lacCode;
        psRegResponse.cellId = hrilRegStatusInfo->cellId;
        psRegResponse.radioTechnology = hrilRegStatusInfo->actType;
        psRegResponse.isDcNrRestricted = (hrilRegStatusInfo->isDcNrRestricted == 1) ? true : false;
        psRegResponse.isNrAvailable = (hrilRegStatusInfo->isNrAvailable == 1) ? true : false;
        psRegResponse.isEnDcAvailable = (hrilRegStatusInfo->isEnDcAvailable == 1) ? true : false;
        TELEPHONY_LOGI(
            "GetPsRegStatusResponse n:%{public}d, regStatus:%{public}d, "
            "lacCode:%{public}d, cellId:%{public}d, actType:%{public}d",
            psRegResponse.notifyType, psRegResponse.regStatus, psRegResponse.lacCode, psRegResponse.cellId,
            psRegResponse.radioTechnology);
    }
    return ResponseMessageParcel(responseInfo, psRegResponse, requestNum);
}

void HRilNetwork::FillCellNearbyInfoTdscdma(CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr)
{
    CellInfo *cellPtr = (CellInfo *)hrilCellPtr;
    if (cellPtr == NULL) {
        TELEPHONY_LOGE("FillCellNearbyInfoTdscdma cellPtr param is null");
        return;
    }
    cellInfo.ServiceCellParas.tdscdma.arfcn = cellPtr->ServiceCellParas.tdscdma.arfcn;
    cellInfo.ServiceCellParas.tdscdma.syncId = cellPtr->ServiceCellParas.tdscdma.syncId;
    cellInfo.ServiceCellParas.tdscdma.sc = cellPtr->ServiceCellParas.tdscdma.sc;
    cellInfo.ServiceCellParas.tdscdma.cellId = cellPtr->ServiceCellParas.tdscdma.cellId;
    cellInfo.ServiceCellParas.tdscdma.lac = cellPtr->ServiceCellParas.tdscdma.lac;
    cellInfo.ServiceCellParas.tdscdma.rscp = cellPtr->ServiceCellParas.tdscdma.rscp;
    cellInfo.ServiceCellParas.tdscdma.drx = cellPtr->ServiceCellParas.tdscdma.drx;
    cellInfo.ServiceCellParas.tdscdma.rac = cellPtr->ServiceCellParas.tdscdma.rac;
    cellInfo.ServiceCellParas.tdscdma.cpid = cellPtr->ServiceCellParas.tdscdma.cpid;
}

void HRilNetwork::FillCellNearbyInfoCdma(CellNearbyInfo &cellInfo, const CellInfo *hrilCellPtr)
{
    CellInfo *cellPtr = (CellInfo *)hrilCellPtr;
    if (cellPtr == NULL) {
        TELEPHONY_LOGE("FillCellNearbyInfoTdscdma cellPtr param is null");
        return;
    }
    cellInfo.ServiceCellParas.cdma.systemId = cellPtr->ServiceCellParas.cdma.systemId;
    cellInfo.ServiceCellParas.cdma.networkId = cellPtr->ServiceCellParas.cdma.networkId;
    cellInfo.ServiceCellParas.cdma.baseId = cellPtr->ServiceCellParas.cdma.baseId;
    cellInfo.ServiceCellParas.cdma.zoneId = cellPtr->ServiceCellParas.cdma.zoneId;
    cellInfo.ServiceCellParas.cdma.pilotPn = cellPtr->ServiceCellParas.cdma.pilotPn;
    cellInfo.ServiceCellParas.cdma.pilotStrength = cellPtr->ServiceCellParas.cdma.pilotStrength;
    cellInfo.ServiceCellParas.cdma.channel = cellPtr->ServiceCellParas.cdma.channel;
    cellInfo.ServiceCellParas.cdma.longitude = cellPtr->ServiceCellParas.cdma.longitude;
    cellInfo.ServiceCellParas.cdma.latitude = cellPtr->ServiceCellParas.cdma.latitude;
}

void HRilNetwork::FillCellNearbyInfo(CellNearbyInfo &cellInfo, const CellInfo *cellPtr)
{
    cellInfo.ratType = cellPtr->ratType;
    switch (cellPtr->ratType) {
        case NETWORK_TYPE_GSM:
            cellInfo.ServiceCellParas.gsm.band = cellPtr->ServiceCellParas.gsm.band;
            cellInfo.ServiceCellParas.gsm.arfcn = cellPtr->ServiceCellParas.gsm.arfcn;
            cellInfo.ServiceCellParas.gsm.bsic = cellPtr->ServiceCellParas.gsm.bsic;
            cellInfo.ServiceCellParas.gsm.cellId = cellPtr->ServiceCellParas.gsm.cellId;
            cellInfo.ServiceCellParas.gsm.lac = cellPtr->ServiceCellParas.gsm.lac;
            cellInfo.ServiceCellParas.gsm.rxlev = cellPtr->ServiceCellParas.gsm.rxlev;
            break;
        case NETWORK_TYPE_LTE:
            cellInfo.ServiceCellParas.lte.arfcn = cellPtr->ServiceCellParas.lte.arfcn;
            cellInfo.ServiceCellParas.lte.pci = cellPtr->ServiceCellParas.lte.pci;
            cellInfo.ServiceCellParas.lte.rsrp = cellPtr->ServiceCellParas.lte.rsrp;
            cellInfo.ServiceCellParas.lte.rsrq = cellPtr->ServiceCellParas.lte.rsrq;
            cellInfo.ServiceCellParas.lte.rxlev = cellPtr->ServiceCellParas.lte.rxlev;
            break;
        case NETWORK_TYPE_WCDMA:
            cellInfo.ServiceCellParas.wcdma.arfcn = cellPtr->ServiceCellParas.wcdma.arfcn;
            cellInfo.ServiceCellParas.wcdma.psc = cellPtr->ServiceCellParas.wcdma.psc;
            cellInfo.ServiceCellParas.wcdma.rscp = cellPtr->ServiceCellParas.wcdma.rscp;
            cellInfo.ServiceCellParas.wcdma.ecno = cellPtr->ServiceCellParas.wcdma.ecno;
            break;
        case NETWORK_TYPE_CDMA:
            FillCellNearbyInfoCdma(cellInfo, cellPtr);
            break;
        case NETWORK_TYPE_TDSCDMA:
            FillCellNearbyInfoTdscdma(cellInfo, cellPtr);
            break;
        case NETWORK_TYPE_NR:
            cellInfo.ServiceCellParas.nr.nrArfcn = cellPtr->ServiceCellParas.nr.nrArfcn;
            cellInfo.ServiceCellParas.nr.pci = cellPtr->ServiceCellParas.nr.pci;
            cellInfo.ServiceCellParas.nr.tac = cellPtr->ServiceCellParas.nr.tac;
            cellInfo.ServiceCellParas.nr.nci = cellPtr->ServiceCellParas.nr.nci;
            break;
        default:
            cellInfo.ServiceCellParas.gsm.band = 0;
            cellInfo.ServiceCellParas.gsm.arfcn = 0;
            cellInfo.ServiceCellParas.gsm.bsic = 0;
            cellInfo.ServiceCellParas.gsm.cellId = 0;
            cellInfo.ServiceCellParas.gsm.lac = 0;
            cellInfo.ServiceCellParas.gsm.rxlev = 0;
            break;
    }
}

int32_t HRilNetwork::BuildNeighboringCellList(
    CellListNearbyInfo &cellInfoList, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("BuildCellList: Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
        return HRIL_ERR_NULL_POINT;
    } else {
        const CellInfoList *temp = reinterpret_cast<const CellInfoList *>(response);
        cellInfoList.itemNum = temp->itemNum;
        TELEPHONY_LOGI("BuildNeighboringCellList:  cellInfoList.itemNum = %{public}d", cellInfoList.itemNum);
        for (int32_t i = 0; i < temp->itemNum; i++) {
            CellNearbyInfo cellInfo;
            CellInfo *cellPtr = temp->cellNearbyInfo + i;
            if (cellPtr == nullptr) {
                TELEPHONY_LOGE("BuildNeighboringCellList: response item error!");
                return HRIL_ERR_GENERIC_FAILURE;
            }
            FillCellNearbyInfo(cellInfo, cellPtr);
            cellInfoList.cellNearbyInfo.push_back(cellInfo);
        }
        return HRIL_ERR_SUCCESS;
    }
}

int32_t HRilNetwork::GetNeighboringCellInfoListResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    CellListNearbyInfo cellList;
    cellList.itemNum = 0;
    cellList.cellNearbyInfo.clear();
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(CellInfoList)))) {
        TELEPHONY_LOGE("GetNeighboringCellInfoListResponse  check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGI("GetNeighboringCellInfoListResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        if (BuildNeighboringCellList(cellList, responseInfo, response, responseLen) != 0) {
            return HRIL_ERR_GENERIC_FAILURE;
        }
    }
    return ResponseMessageParcel(responseInfo, cellList, requestNum);
}

void HRilNetwork::FillCellInfoType(CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *hrilCellInfoVendor)
{
    CurrentCellInfoVendor *cellInfoVendor = (CurrentCellInfoVendor *)hrilCellInfoVendor;
    if (cellInfoVendor == NULL) {
        TELEPHONY_LOGE("FillCellInfoType cellInfoVendor param is null");
        return;
    }
    if (cellInfoVendor->ratType == NETWORK_TYPE_WCDMA) {
        cellInfo.ServiceCellParas.wcdma.arfcn = cellInfoVendor->ServiceCellParas.wcdma.arfcn;
        cellInfo.ServiceCellParas.wcdma.cellId = cellInfoVendor->ServiceCellParas.wcdma.cellId;
        cellInfo.ServiceCellParas.wcdma.psc = cellInfoVendor->ServiceCellParas.wcdma.psc;
        cellInfo.ServiceCellParas.wcdma.lac = cellInfoVendor->ServiceCellParas.wcdma.lac;
        cellInfo.ServiceCellParas.wcdma.rxlev = cellInfoVendor->ServiceCellParas.wcdma.rxlev;
        cellInfo.ServiceCellParas.wcdma.rscp = cellInfoVendor->ServiceCellParas.wcdma.rscp;
        cellInfo.ServiceCellParas.wcdma.ecno = cellInfoVendor->ServiceCellParas.wcdma.ecno;
        cellInfo.ServiceCellParas.wcdma.ura = cellInfoVendor->ServiceCellParas.wcdma.ura;
        cellInfo.ServiceCellParas.wcdma.drx = cellInfoVendor->ServiceCellParas.wcdma.drx;
    } else if (cellInfoVendor->ratType == NETWORK_TYPE_CDMA) {
        cellInfo.ServiceCellParas.cdma.systemId = cellInfoVendor->ServiceCellParas.cdma.systemId;
        cellInfo.ServiceCellParas.cdma.networkId = cellInfoVendor->ServiceCellParas.cdma.networkId;
        cellInfo.ServiceCellParas.cdma.baseId = cellInfoVendor->ServiceCellParas.cdma.baseId;
        cellInfo.ServiceCellParas.cdma.zoneId = cellInfoVendor->ServiceCellParas.cdma.zoneId;
        cellInfo.ServiceCellParas.cdma.pilotPn = cellInfoVendor->ServiceCellParas.cdma.pilotPn;
        cellInfo.ServiceCellParas.cdma.pilotStrength = cellInfoVendor->ServiceCellParas.cdma.pilotStrength;
        cellInfo.ServiceCellParas.cdma.channel = cellInfoVendor->ServiceCellParas.cdma.channel;
        cellInfo.ServiceCellParas.cdma.longitude = cellInfoVendor->ServiceCellParas.cdma.longitude;
        cellInfo.ServiceCellParas.cdma.latitude = cellInfoVendor->ServiceCellParas.cdma.latitude;
    } else if (cellInfoVendor->ratType == NETWORK_TYPE_TDSCDMA) {
        cellInfo.ServiceCellParas.tdscdma.arfcn = cellInfoVendor->ServiceCellParas.tdscdma.arfcn;
        cellInfo.ServiceCellParas.tdscdma.syncId = cellInfoVendor->ServiceCellParas.tdscdma.syncId;
        cellInfo.ServiceCellParas.tdscdma.sc = cellInfoVendor->ServiceCellParas.tdscdma.sc;
        cellInfo.ServiceCellParas.tdscdma.cellId = cellInfoVendor->ServiceCellParas.tdscdma.cellId;
        cellInfo.ServiceCellParas.tdscdma.lac = cellInfoVendor->ServiceCellParas.tdscdma.lac;
        cellInfo.ServiceCellParas.tdscdma.rscp = cellInfoVendor->ServiceCellParas.tdscdma.rscp;
        cellInfo.ServiceCellParas.tdscdma.drx = cellInfoVendor->ServiceCellParas.tdscdma.drx;
        cellInfo.ServiceCellParas.tdscdma.rac = cellInfoVendor->ServiceCellParas.tdscdma.rac;
        cellInfo.ServiceCellParas.tdscdma.cpid = cellInfoVendor->ServiceCellParas.tdscdma.cpid;
    }
}

void HRilNetwork::FillCurrentCellInfo(CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *cellInfoVendor)
{
    cellInfo.ratType = cellInfoVendor->ratType;
    cellInfo.mcc = cellInfoVendor->mcc;
    cellInfo.mnc = cellInfoVendor->mnc;
    switch (cellInfoVendor->ratType) {
        case NETWORK_TYPE_GSM:
            cellInfo.ServiceCellParas.gsm.band = cellInfoVendor->ServiceCellParas.gsm.band;
            cellInfo.ServiceCellParas.gsm.arfcn = cellInfoVendor->ServiceCellParas.gsm.arfcn;
            cellInfo.ServiceCellParas.gsm.bsic = cellInfoVendor->ServiceCellParas.gsm.bsic;
            cellInfo.ServiceCellParas.gsm.cellId = cellInfoVendor->ServiceCellParas.gsm.cellId;
            cellInfo.ServiceCellParas.gsm.lac = cellInfoVendor->ServiceCellParas.gsm.lac;
            cellInfo.ServiceCellParas.gsm.rxlev = cellInfoVendor->ServiceCellParas.gsm.rxlev;
            cellInfo.ServiceCellParas.gsm.rxQuality = cellInfoVendor->ServiceCellParas.gsm.rxQuality;
            cellInfo.ServiceCellParas.gsm.ta = cellInfoVendor->ServiceCellParas.gsm.ta;
            break;
        case NETWORK_TYPE_LTE:
            cellInfo.ServiceCellParas.lte.arfcn = cellInfoVendor->ServiceCellParas.lte.arfcn;
            cellInfo.ServiceCellParas.lte.cellId = cellInfoVendor->ServiceCellParas.lte.cellId;
            cellInfo.ServiceCellParas.lte.pci = cellInfoVendor->ServiceCellParas.lte.pci;
            cellInfo.ServiceCellParas.lte.tac = cellInfoVendor->ServiceCellParas.lte.tac;
            cellInfo.ServiceCellParas.lte.rsrp = cellInfoVendor->ServiceCellParas.lte.rsrp;
            cellInfo.ServiceCellParas.lte.rsrq = cellInfoVendor->ServiceCellParas.lte.rsrq;
            cellInfo.ServiceCellParas.lte.rssi = cellInfoVendor->ServiceCellParas.lte.rssi;
            break;
        case NETWORK_TYPE_WCDMA:
        case NETWORK_TYPE_CDMA:
        case NETWORK_TYPE_TDSCDMA:
            FillCellInfoType(cellInfo, cellInfoVendor);
            break;
        case NETWORK_TYPE_NR:
            cellInfo.ServiceCellParas.nr.nrArfcn = cellInfoVendor->ServiceCellParas.nr.nrArfcn;
            cellInfo.ServiceCellParas.nr.pci = cellInfoVendor->ServiceCellParas.nr.pci;
            cellInfo.ServiceCellParas.nr.tac = cellInfoVendor->ServiceCellParas.nr.tac;
            cellInfo.ServiceCellParas.nr.nci = cellInfoVendor->ServiceCellParas.nr.nci;
            break;
        default:
            cellInfo.ServiceCellParas.wcdma.arfcn = 0;
            cellInfo.ServiceCellParas.wcdma.cellId = 0;
            cellInfo.ServiceCellParas.wcdma.psc = 0;
            cellInfo.ServiceCellParas.wcdma.lac = 0;
            cellInfo.ServiceCellParas.wcdma.rxlev = 0;
            cellInfo.ServiceCellParas.wcdma.rscp = 0;
            cellInfo.ServiceCellParas.wcdma.ecno = 0;
            cellInfo.ServiceCellParas.wcdma.drx = 0;
            cellInfo.ServiceCellParas.wcdma.ura = 0;
            break;
    }
}

int32_t HRilNetwork::BuildCurrentCellList(
    CellListCurrentInfo &cellInfoList, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("BuildCurrentCellList: Invalid response");
        return HRIL_ERR_NULL_POINT;
    } else {
        const CurrentCellInfoList *temp = reinterpret_cast<const CurrentCellInfoList *>(response);
        cellInfoList.itemNum = temp->itemNum;
        TELEPHONY_LOGI("BuildCurrentCellList:  cellInfoList.itemNum = %{public}d", cellInfoList.itemNum);
        for (int32_t i = 0; i < temp->itemNum; i++) {
            CurrentCellInfo cellInfo;
            CurrentCellInfoVendor *cellPtr = temp->currentCellInfo + i;
            if (cellPtr == nullptr) {
                TELEPHONY_LOGE("BuildCurrentCellList: response item error!");
                return HRIL_ERR_GENERIC_FAILURE;
            }
            FillCurrentCellInfo(cellInfo, cellPtr);
            cellInfoList.cellCurrentInfo.push_back(cellInfo);
        }
        return HRIL_ERR_SUCCESS;
    }
}

int32_t HRilNetwork::GetCurrentCellInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    CellListCurrentInfo cellList;
    cellList.itemNum = 0;
    cellList.cellCurrentInfo.clear();
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(CurrentCellInfoList)))) {
        TELEPHONY_LOGE("GetCurrentCellInfoResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGI("GetCurrentCellInfoResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        if (BuildCurrentCellList(cellList, responseInfo, response, responseLen) != 0) {
            return HRIL_ERR_GENERIC_FAILURE;
        }
    }
    return ResponseMessageParcel(responseInfo, cellList, requestNum);
}

int32_t HRilNetwork::GetNetworkSelectionModeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    struct SetNetworkModeInfo selectModeResultInfo = {};
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(int32_t)))) {
        TELEPHONY_LOGE("GetNetworkSelectionModeResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGE("response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t *resp = static_cast<int32_t *>(const_cast<void *>(response));
        TELEPHONY_LOGI("selectModeResultInfo.selectMode: %{public}d", *resp);
        selectModeResultInfo.selectMode = *resp;
    }
    return ResponseMessageParcel(responseInfo, selectModeResultInfo, requestNum);
}

void HRilNetwork::BuildOperatorList(AvailableNetworkList &availableNetworkList, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    int32_t numStrings = responseLen / sizeof(AvailableOperInfo *);
    if (response == nullptr) {
        TELEPHONY_LOGE("response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        AvailableNetworkInfo operInfo = {};
        availableNetworkList.itemNum = numStrings;
        TELEPHONY_LOGI("availableNetworkList.itemNum: %{public}d", numStrings);
        for (int32_t i = 0; i < numStrings; i++) {
            AvailableOperInfo *curPtr = ((AvailableOperInfo **)response)[i];
            if (curPtr != nullptr) {
                operInfo.status = curPtr->status;
                TELEPHONY_LOGI("operInfo.status:%{public}d", curPtr->status);
                operInfo.longName = curPtr->longName;
                TELEPHONY_LOGI("operInfo.longName:%{public}s", curPtr->longName);
                operInfo.numeric = curPtr->numeric;
                TELEPHONY_LOGI("operInfo.numeric:%{public}s", curPtr->numeric);
                operInfo.shortName = curPtr->shortName;
                TELEPHONY_LOGI("operInfo.shortName:%{public}s", curPtr->shortName);
                operInfo.rat = curPtr->rat;
                TELEPHONY_LOGI("operInfo.rat:%{public}d", curPtr->rat);
            }
            availableNetworkList.availableNetworkInfo.push_back(operInfo);
        }
    }
}

int32_t HRilNetwork::GetNetworkSearchInformationResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    TELEPHONY_LOGI("GetNetworkSearchInformationResponse ---> response %{public}p", response);
    AvailableNetworkList availableNetworkList = {};
    availableNetworkList.itemNum = 0;
    BuildOperatorList(availableNetworkList, responseInfo, response, responseLen);

    return ResponseMessageParcel(responseInfo, availableNetworkList, requestNum);
}

int32_t HRilNetwork::GetPreferredNetworkResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    PreferredNetworkTypeInfo preferredNetworkTypeInfo = {};

    TELEPHONY_LOGI("GetPreferredNetworkResponse ---> response %{public}p", response);
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(int32_t)))) {
        TELEPHONY_LOGE("GetPreferredNetworkResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGE("response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t *resp = static_cast<int32_t *>(const_cast<void *>(response));
        preferredNetworkTypeInfo.preferredNetworkType = *resp;
        TELEPHONY_LOGI("preferredNetworkTypeInfo.preferredNetworkType: %{public}d", *resp);
    }
    return ResponseMessageParcel(responseInfo, preferredNetworkTypeInfo, requestNum);
}

int32_t HRilNetwork::SetPreferredNetworkResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilNetwork::SetNetworkSelectionModeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilNetwork::GetRadioCapabilityResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    RadioCapabilityInfo radioCapabilityInfo = {};
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(HRilRadioCapability)))) {
        TELEPHONY_LOGE("GetRadioCapabilityResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGE("response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        TELEPHONY_LOGE("GetRadioCapabilityResponse is success");
        const HRilRadioCapability *hRilRadioCapability = static_cast<const HRilRadioCapability *>(response);
        radioCapabilityInfo.ratFamily = hRilRadioCapability->ratFamily;
        radioCapabilityInfo.modemId = hRilRadioCapability->modemId;
    }
    return ResponseMessageParcel(responseInfo, radioCapabilityInfo, requestNum);
}

int32_t HRilNetwork::GetPhysicalChannelConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    ChannelConfigInfoList phyChnlCfgList = {};
    phyChnlCfgList.itemNum = 0;
    phyChnlCfgList.channelConfigInfos.clear();
    if (((response == nullptr) && (responseLen != 0)) ||
        ((response != nullptr) && (responseLen != sizeof(HRilChannelConfigList)))) {
        TELEPHONY_LOGE("GetPhysicalChannelConfigResponse check data: error");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if ((response == nullptr) && (responseLen == 0)) {
        TELEPHONY_LOGE("GetPhysicalChannelConfigResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilChannelConfigList *hrilChannelConfigList = static_cast<const HRilChannelConfigList *>(response);
        phyChnlCfgList.itemNum = hrilChannelConfigList->itemNum;
        for (int32_t i = 0; i < phyChnlCfgList.itemNum; i++) {
            PhysicalChannelConfig phyChnlCfg;
            phyChnlCfg.cellConnStatus = hrilChannelConfigList->channelConfigs[i].cellConnStatus;
            phyChnlCfg.cellBandwidth = hrilChannelConfigList->channelConfigs[i].cellBandwidth;
            phyChnlCfg.ratType = hrilChannelConfigList->channelConfigs[i].ratType;
            phyChnlCfg.freqRange = hrilChannelConfigList->channelConfigs[i].freqRange;
            phyChnlCfg.channelNum = hrilChannelConfigList->channelConfigs[i].channelNum;
            phyChnlCfg.physicalCellId = hrilChannelConfigList->channelConfigs[i].physicalCellId;
            phyChnlCfg.contextIdNum = hrilChannelConfigList->channelConfigs[i].contextIdNum;
            TELEPHONY_LOGI(
                "cellConnStatus:%{public}d,cellBandwidth:%{public}d,physicalCellId:%{public}d,"
                "ratType:%{public}d,freqRange:%{public}d,channelNum:%{public}d,contextIdNum:%{public}d",
                phyChnlCfg.cellConnStatus, phyChnlCfg.cellBandwidth, phyChnlCfg.ratType, phyChnlCfg.freqRange,
                phyChnlCfg.channelNum, phyChnlCfg.physicalCellId, phyChnlCfg.contextIdNum);
            for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
                phyChnlCfg.contextIds.push_back(hrilChannelConfigList->channelConfigs[i].contextIds[j]);
                TELEPHONY_LOGI("contextIds:%{public}d---contextId:%{public}d", j, phyChnlCfg.contextIds[j]);
            }
            phyChnlCfgList.channelConfigInfos.push_back(phyChnlCfg);
        }
        TELEPHONY_LOGI("GetPhysicalChannelConfigResponse itemNum:%{public}d", phyChnlCfgList.itemNum);
    }
    return ResponseMessageParcel(responseInfo, phyChnlCfgList, requestNum);
}

int32_t HRilNetwork::SetLocateUpdatesResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
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
