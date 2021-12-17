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
    ERR_INVALID_RESPONSE_VALUE = 3,
    INVALID_RESPONSE_VALUE = 11,
};

HRilNetwork::HRilNetwork()
{
    AddHandlerToMap();
}

void HRilNetwork::AddHandlerToMap()
{
    // indication
    notiMemberFuncMap_[HNOTI_NETWORK_CS_REG_STATUS_UPDATED] = &HRilNetwork::NetworkRegStatusUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED] = &HRilNetwork::SignalStrengthUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_TIME_UPDATED] = &HRilNetwork::NetworkTimeUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_TIME_ZONE_UPDATED] = &HRilNetwork::NetworkTimeZoneUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_IMS_REG_STATUS_UPDATED] = &HRilNetwork::NetworkImsRegStatusUpdated;

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
    reqMemberFuncMap_[HREQ_NETWORK_GET_IMEI] = &HRilNetwork::GetImei;
    reqMemberFuncMap_[HREQ_NETWORK_SET_PS_ATTACH_STATUS] = &HRilNetwork::SetPsAttachStatus;
    reqMemberFuncMap_[HREQ_NETWORK_GET_PS_ATTACH_STATUS] = &HRilNetwork::GetPsAttachStatus;
    reqMemberFuncMap_[HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST] = &HRilNetwork::GetCellInfoList;
    reqMemberFuncMap_[HREQ_NETWORK_GET_CURRENT_CELL_INFO] = &HRilNetwork::GetCurrentCellInfo;

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
    respMemberFuncMap_[HREQ_NETWORK_GET_IMEI] = &HRilNetwork::GetImeiResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_PS_ATTACH_STATUS] = &HRilNetwork::SetPsAttachStatusResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_PS_ATTACH_STATUS] = &HRilNetwork::GetPsAttachStatusResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST] = &HRilNetwork::GetNeighboringCellInfoListResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_CURRENT_CELL_INFO] = &HRilNetwork::GetCurrentCellInfoResponse;
}

void HRilNetwork::ProcessNetworkResponse(
    int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    auto itFunc = respMemberFuncMap_.find(code);
    if (itFunc != respMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, code, responseInfo, response, responseLen);
        }
    }
}

void HRilNetwork::ProcessNetworkRequest(int32_t slotId, int32_t code, struct HdfSBuf *data)
{
    auto itFunc = reqMemberFuncMap_.find(code);
    if (itFunc != reqMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, data);
        }
    }
}

void HRilNetwork::ProcessNetworkNotify(int32_t slotId, int32_t notifyType, const struct ReportInfo *reportInfo,
    const void *response, size_t responseLen)
{
    int code;
    HRilErrno e;
    code = reportInfo->notifyId;
    e = (HRilErrno)reportInfo->error;
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, notifyType, e, response, responseLen);
        }
    }
}

void HRilNetwork::GetImsRegStatus(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_IMS_REG_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }
    networkFuncs_->GetImsRegStatus(requestInfo);
    SafeFrees(requestInfo);
}

void HRilNetwork::GetSignalStrength(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_SIGNAL_STRENGTH);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }
    networkFuncs_->GetSignalStrength(requestInfo);
    SafeFrees(requestInfo);
}

void HRilNetwork::ExchangeRilRssiToHdf(const void *response, size_t responseLen, Rssi &rssi)
{
    HRilRssi *rilRssi = (HRilRssi *)response;

    rssi.gw.rxlev = rilRssi->gsmRssi.rxlev;
    rssi.gw.ber = rilRssi->gsmRssi.ber;

    rssi.wcdma.rxlev = rilRssi->wcdmaRssi.rxlev;
    rssi.wcdma.ber = rilRssi->wcdmaRssi.ber;
    rssi.wcdma.ecio = rilRssi->wcdmaRssi.ecio;
    rssi.wcdma.rscp = rilRssi->wcdmaRssi.rscp;

    rssi.lte.rxlev = rilRssi->lteRssi.rxlev;
    rssi.lte.rsrp = rilRssi->lteRssi.rsrp;
    rssi.lte.rsrq = rilRssi->lteRssi.rsrq;
    rssi.lte.snr = rilRssi->lteRssi.snr;
}

int32_t HRilNetwork::GetImsRegStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    const int IMS_EXT_INFO_NOTIFY_TYPE = 2;
    ImsRegStatusInfo imsRegResponse;
    imsRegResponse.notifyType = IMS_EXT_INFO_NOTIFY_TYPE;
    imsRegResponse.regInfo = 0;
    imsRegResponse.extInfo = 0;
    if (response == nullptr) {
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

int32_t HRilNetwork::GetSignalStrengthResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    Rssi event = {};
    event.slotId = slotId;
    if (response == nullptr || responseLen != sizeof(HRilRssi)) {
        TELEPHONY_LOGE("GetSignalStrengthResponse: Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        ExchangeRilRssiToHdf(response, responseLen, event);
    }
    return ResponseBuffer(requestNum, &responseInfo, sizeof(responseInfo), &event, sizeof(event));
}

void HRilNetwork::GetCsRegStatus(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_CS_REG_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }
    networkFuncs_->GetCsRegStatus(requestInfo);
    SafeFrees(requestInfo);
}

int32_t HRilNetwork::GetCsRegStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    CsRegStatusInfo csRegResponse;
    const int INIT_N_TYPE_VALUE = 2;
    csRegResponse.notifyType = INIT_N_TYPE_VALUE;
    csRegResponse.regStatus = 0;
    csRegResponse.lacCode = 0;
    csRegResponse.cellId = 0;
    csRegResponse.radioTechnology = 0;
    if (response == nullptr) {
        TELEPHONY_LOGE("GetCsRegStatusResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
        csRegResponse.notifyType = hrilRegStatusInfo->notifyType;
        csRegResponse.regStatus = hrilRegStatusInfo->regState;
        csRegResponse.lacCode = hrilRegStatusInfo->lacCode;
        csRegResponse.cellId = hrilRegStatusInfo->cellId;
        csRegResponse.radioTechnology = hrilRegStatusInfo->actType;
    }
    return ResponseMessageParcel(responseInfo, csRegResponse, requestNum);
}

void HRilNetwork::GetOperatorInfo(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_OPERATOR_INFO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }
    networkFuncs_->GetOperatorInfo(requestInfo);
    SafeFrees(requestInfo);
}

void HRilNetwork::GetNetworkSearchInformation(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }
    networkFuncs_->GetNetworkSearchInformation(requestInfo);
}

void HRilNetwork::GetNetworkSelectionMode(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetNetworkSelectionMode::miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_NETWORK_SELECTION_MODE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetNetworkSelectionMode::Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }
    networkFuncs_->GetNetworkSelectionMode(requestInfo);
    SafeFrees(requestInfo);
}

void HRilNetwork::SetNetworkSelectionMode(int32_t slotId, struct HdfSBuf *data)
{
    const int32_t POINTER_NUM = 1;
    struct SetNetworkModeInfo setNetworkModeInfo = {};
    MessageParcel *parcel = nullptr;

    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (!setNetworkModeInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }

    ReqDataInfo *requestInfo =
        CreateHRilRequest(setNetworkModeInfo.serial, slotId, HREQ_NETWORK_SET_NETWORK_SELECTION_MODE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("SetNetworkSelectionMode::Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }
    HRilSetNetworkModeInfo setModeInfo = {};
    setModeInfo.selectMode = setNetworkModeInfo.selectMode;
    if (!ConvertToString(&setModeInfo.oper, setNetworkModeInfo.oper, requestInfo)) {
        TELEPHONY_LOGE("SetNetworkSelectionMode::ConvertToString failed");
        SafeFrees(requestInfo);
        return;
    }
    networkFuncs_->SetNetworkSelectionMode(requestInfo, &setModeInfo);
    FreeStrings(POINTER_NUM, setModeInfo.oper);
    SafeFrees(requestInfo);
}

void HRilNetwork::GetPreferredNetwork(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_PREFERRED_NETWORK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetPreferredNetwork::Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetPreferredNetwork::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }
    networkFuncs_->GetPreferredNetwork(requestInfo);
    SafeFrees(requestInfo);
}

void HRilNetwork::SetPreferredNetwork(int32_t slotId, struct HdfSBuf *data)
{
    int32_t netType = 0;
    int32_t serial = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &netType)) {
        TELEPHONY_LOGE("miss netType parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_SET_PREFERRED_NETWORK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("SetPreferredNetwork::Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }

    networkFuncs_->SetPreferredNetwork(requestInfo, &netType);
    SafeFrees(requestInfo);
}

void HRilNetwork::GetImei(int32_t slotId, struct HdfSBuf *data)
{
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetImei::networkFuncs_ is nullptr");
        return;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetImei::miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_IMEI);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetImei::Create Request is fail");
        return;
    }
    networkFuncs_->GetImei(requestInfo);
    SafeFrees(requestInfo);
}

void HRilNetwork::SetPsAttachStatus(int32_t slotId, struct HdfSBuf *data)
{
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("SetPsAttachStatus::networkFuncs_ is nullptr");
        return;
    }
    int32_t psAttachStatus = 0;
    int32_t serial = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &psAttachStatus)) {
        TELEPHONY_LOGE("miss psAttachStatus parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_SET_PS_ATTACH_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("SetPsAttachStatus::Create Request is fail");
        return;
    }

    networkFuncs_->SetPsAttachStatus(requestInfo, &psAttachStatus);
    SafeFrees(requestInfo);
}

void HRilNetwork::GetPsAttachStatus(int32_t slotId, struct HdfSBuf *data)
{
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetPsAttachStatus::networkFuncs_ is nullptr");
        return;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetPsAttachStatus::miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_PS_ATTACH_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetPsAttachStatus::Create Request is fail");
        return;
    }
    networkFuncs_->GetPsAttachStatus(requestInfo);
    SafeFrees(requestInfo);
}

int32_t HRilNetwork::GetOperatorInfoResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct OperatorInfoResult operatorInfoResult = {};
    int32_t numStrings = responseLen;
    if (response == nullptr || numStrings != static_cast<int32_t>(NetworkParameter::ERR_INVALID_RESPONSE_VALUE)) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        char **resp = static_cast<char **>(const_cast<void *>(response));
        operatorInfoResult.longName = resp[HRIL_LONE_NAME];
        operatorInfoResult.shortName = resp[HRIL_SHORT_NAME];
        operatorInfoResult.numeric = resp[HRIL_NUMERIC];
    }
    return ResponseMessageParcel(responseInfo, operatorInfoResult, requestNum);
}

int32_t HRilNetwork::NetworkRegStatusUpdated(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
    if (hrilRegStatusInfo == nullptr) {
        return HDF_FAILURE;
    }
    CsRegStatusInfo regStatusInfoNotify;
    regStatusInfoNotify.notifyType = hrilRegStatusInfo->notifyType;
    regStatusInfoNotify.regStatus = hrilRegStatusInfo->regState;
    regStatusInfoNotify.lacCode = hrilRegStatusInfo->lacCode;
    regStatusInfoNotify.cellId = hrilRegStatusInfo->cellId;
    regStatusInfoNotify.radioTechnology = hrilRegStatusInfo->actType;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        return HDF_FAILURE;
    }
    if (!regStatusInfoNotify.Marshalling(*parcel.get())) {
        return HDF_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_CS_REG_STATUS_UPDATED, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

int32_t HRilNetwork::NetworkImsRegStatusUpdated(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    const HRilImsRegStatusInfo *hrilImsRegStatusInfo = static_cast<const HRilImsRegStatusInfo *>(response);
    if (hrilImsRegStatusInfo == nullptr) {
        return HDF_FAILURE;
    }
    ImsRegStatusInfo imsRegStatusInfoNotify;
    imsRegStatusInfoNotify.notifyType = hrilImsRegStatusInfo->notifyType;
    imsRegStatusInfoNotify.regInfo = hrilImsRegStatusInfo->regInfo;
    imsRegStatusInfoNotify.extInfo = hrilImsRegStatusInfo->extInfo;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        return HDF_FAILURE;
    }
    if (!imsRegStatusInfoNotify.Marshalling(*parcel.get())) {
        return HDF_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_IMS_REG_STATUS_UPDATED, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

int32_t HRilNetwork::SignalStrengthUpdated(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    Rssi rssi = {0};

    if (response == nullptr || responseLen != sizeof(HRilRssi)) {
        TELEPHONY_LOGE("SignalStrengthUpdated: Invalid response");
    } else {
        ExchangeRilRssiToHdf(response, responseLen, rssi);
    }
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&rssi, sizeof(rssi))) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

int32_t HRilNetwork::NetworkTimeUpdated(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));

    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_TIME_UPDATED, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

int32_t HRilNetwork::NetworkTimeZoneUpdated(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));

    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }

    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }

    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_NETWORK_TIME_ZONE_UPDATED, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

void HRilNetwork::GetPsRegStatus(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        HdfSBufRecycle(data);
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_PS_REG_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        SafeFrees(requestInfo);
        return;
    }
    networkFuncs_->GetPsRegStatus(requestInfo);
    SafeFrees(requestInfo);
}

void HRilNetwork::GetCellInfoList(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetCellInfoList miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetCellInfoList::Create Request is fail");
        return;
    }
    networkFuncs_->GetCellInfoList(requestInfo);
}

void HRilNetwork::GetCurrentCellInfo(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetCurrentCellInfo miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_CURRENT_CELL_INFO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetCurrentCellInfo::Create Request is fail");
        return;
    }
    networkFuncs_->GetCurrentCellInfo(requestInfo);
}

int32_t HRilNetwork::GetPsRegStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    PsRegStatusResultInfo psRegResponse;
    const int INIT_N_TYPE_VALUE = 2;
    psRegResponse.notifyType = INIT_N_TYPE_VALUE;
    psRegResponse.regStatus = 0;
    psRegResponse.lacCode = 0;
    psRegResponse.cellId = 0;
    psRegResponse.radioTechnology = 0;
    if (response == nullptr) {
        TELEPHONY_LOGE("GetPsRegStatusResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilRegStatusInfo *hrilRegStatusInfo = static_cast<const HRilRegStatusInfo *>(response);
        psRegResponse.notifyType = hrilRegStatusInfo->notifyType;
        psRegResponse.regStatus = hrilRegStatusInfo->regState;
        psRegResponse.lacCode = hrilRegStatusInfo->lacCode;
        psRegResponse.cellId = hrilRegStatusInfo->cellId;
        psRegResponse.radioTechnology = hrilRegStatusInfo->actType;
    }
    return ResponseMessageParcel(responseInfo, psRegResponse, requestNum);
}

int32_t HRilNetwork::GetNeighboringCellInfoListResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    CellListNearbyInfo cellList;
    cellList.itemNum = 0;
    cellList.cellNearbyInfo.clear();
    if (response == nullptr) {
        TELEPHONY_LOGE("GetNeighboringCellInfoListResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        BuildCellList(cellList, responseInfo, response, responseLen);
    }

    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        return HDF_FAILURE;
    }
    return ResponseMessageParcel(responseInfo, cellList, requestNum);
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

void HRilNetwork::BuildCellList(
    CellListNearbyInfo &cellInfoList, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || ((responseLen % sizeof(CellInfoList)) != 0)) {
        TELEPHONY_LOGE("BuildCellList: Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const CellInfoList *temp = reinterpret_cast<const CellInfoList *>(response);
        cellInfoList.itemNum = temp->itemNum;
        for (int32_t i = 0; i < temp->itemNum; i++) {
            CellNearbyInfo cellInfo;
            CellInfo *cellPtr = temp->cellNearbyInfo + i;
            FillCellNearbyInfo(cellInfo, cellPtr);
            cellInfoList.cellNearbyInfo.push_back(cellInfo);
        }
    }
}

void HRilNetwork::FillCellInfo(CurrentCellInfo &cellInfo, const CurrentCellInfoVendor *cellInfoVendor)
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
            cellInfo.ServiceCellParas.wcdma.arfcn = cellInfoVendor->ServiceCellParas.wcdma.arfcn;
            cellInfo.ServiceCellParas.wcdma.cellId = cellInfoVendor->ServiceCellParas.wcdma.cellId;
            cellInfo.ServiceCellParas.wcdma.psc = cellInfoVendor->ServiceCellParas.wcdma.psc;
            cellInfo.ServiceCellParas.wcdma.lac = cellInfoVendor->ServiceCellParas.wcdma.lac;
            cellInfo.ServiceCellParas.wcdma.rxlev = cellInfoVendor->ServiceCellParas.wcdma.rxlev;
            cellInfo.ServiceCellParas.wcdma.rscp = cellInfoVendor->ServiceCellParas.wcdma.rscp;
            cellInfo.ServiceCellParas.wcdma.ecno = cellInfoVendor->ServiceCellParas.wcdma.ecno;
            cellInfo.ServiceCellParas.wcdma.drx = cellInfoVendor->ServiceCellParas.wcdma.drx;
            cellInfo.ServiceCellParas.wcdma.ura = cellInfoVendor->ServiceCellParas.wcdma.ura;
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

void HRilNetwork::BuildCellInfo(
    CurrentCellInfo &cellInfo, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr) && (responseLen != 0)) {
        TELEPHONY_LOGE("BuildCallList: Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const CurrentCellInfoVendor *temp = reinterpret_cast<const CurrentCellInfoVendor *>(response);
        FillCellInfo(cellInfo, temp);
    }
}

int32_t HRilNetwork::GetCurrentCellInfoResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    CurrentCellInfo cellInfo;
    if (response == nullptr) {
        TELEPHONY_LOGE("GetCurrentCellInfoResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    }
    BuildCellInfo(cellInfo, responseInfo, response, responseLen);
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        return HDF_FAILURE;
    }
    return ResponseMessageParcel(responseInfo, cellInfo, requestNum);
}

int32_t HRilNetwork::GetNetworkSelectionModeResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    struct SetNetworkModeInfo selectModeResultInfo = {};
    if (response == nullptr) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t *resp = static_cast<int32_t *>(const_cast<void *>(response));
        selectModeResultInfo.selectMode = *resp;
    }
    return ResponseMessageParcel(responseInfo, selectModeResultInfo, requestNum);
}

void HRilNetwork::BuildOperatorList(AvailableNetworkList &availableNetworkList, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    int32_t numStrings = responseLen;
    if (response == nullptr) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        AvailableNetworkInfo operInfo;
        availableNetworkList.itemNum = numStrings;
        for (int32_t i = 0; i < numStrings; i++) {
            AvailableOperInfo *curPtr = ((AvailableOperInfo **)response)[i];
            if (curPtr != nullptr) {
                operInfo.status = curPtr->status;
                operInfo.longName = curPtr->longName;
                operInfo.numeric = curPtr->numeric;
                operInfo.shortName = curPtr->shortName;
                operInfo.rat = curPtr->rat;
            }
            availableNetworkList.availableNetworkInfo.push_back(operInfo);
        }
    }
}

int32_t HRilNetwork::GetNetworkSearchInformationResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    AvailableNetworkList availableNetworkList;
    availableNetworkList.itemNum = 0;
    BuildOperatorList(availableNetworkList, responseInfo, response, responseLen);

    return ResponseMessageParcel(responseInfo, availableNetworkList, requestNum);
}

int32_t HRilNetwork::GetPreferredNetworkResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    PreferredNetworkTypeInfo preferredNetworkTypeInfo;

    if (response == nullptr) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t *resp = static_cast<int32_t *>(const_cast<void *>(response));
        preferredNetworkTypeInfo.preferredNetworkType = *resp;
    }
    return ResponseMessageParcel(responseInfo, preferredNetworkTypeInfo, requestNum);
}

int32_t HRilNetwork::SetPreferredNetworkResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilNetwork::SetNetworkSelectionModeResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilNetwork::GetImeiResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in GetIccIDResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetIccIDResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);

    return HDF_SUCCESS;
}

int32_t HRilNetwork::SetPsAttachStatusResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilNetwork::GetPsAttachStatusResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    PsAttachStatusInfo psAttachStatusInfo;

    if (response == nullptr) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t *resp = static_cast<int32_t *>(const_cast<void *>(response));
        psAttachStatusInfo.psAttachStatus = *resp;
    }
    return ResponseMessageParcel(responseInfo, psAttachStatusInfo, requestNum);
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
