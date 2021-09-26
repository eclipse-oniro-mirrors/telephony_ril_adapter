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
    // Indication
    notiMemberFuncMap_[HNOTI_NETWORK_CS_REG_STATUS_UPDATED] = &HRilNetwork::NetworkRegStatusUpdated;
    notiMemberFuncMap_[HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED] = &HRilNetwork::SignalStrengthUpdated;

    // Request
    reqMemberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &HRilNetwork::GetSignalStrength;
    reqMemberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &HRilNetwork::GetCsRegStatus;
    reqMemberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &HRilNetwork::GetPsRegStatus;
    reqMemberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &HRilNetwork::GetOperatorInfo;
    reqMemberFuncMap_[HREQ_NETWORK_GET_SEARCH_INFORMATION] = &HRilNetwork::GetNetworkSearchInformation;
    reqMemberFuncMap_[HREQ_NETWORK_GET_SELECTION_MODE] = &HRilNetwork::GetNetworkSelectionMode;
    reqMemberFuncMap_[HREQ_NETWORK_SET_SELECTION_MODE] = &HRilNetwork::SetNetworkSelectionMode;
    reqMemberFuncMap_[HREQ_NETWORK_SET_LOCATION_UPDATE] = &HRilNetwork::SetNetworkLocationUpdate;

    // Response
    respMemberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &HRilNetwork::GetSignalStrengthResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &HRilNetwork::GetCsRegStatusResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &HRilNetwork::GetPsRegStatusResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &HRilNetwork::GetOperatorInfoResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_SEARCH_INFORMATION] = &HRilNetwork::GetNetworkSearchInformationResponse;
    respMemberFuncMap_[HREQ_NETWORK_GET_SELECTION_MODE] = &HRilNetwork::GetNetworkSelectionModeResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_SELECTION_MODE] = &HRilNetwork::SetNetworkSelectionModeResponse;
    respMemberFuncMap_[HREQ_NETWORK_SET_LOCATION_UPDATE] = &HRilNetwork::SetNetworkLocationUpdateResponse;
}

void HRilNetwork::ProcessNetworkResponse(
    int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    TELEPHONY_LOGD("HRilNetwork ProcessNetworkResponse code:%{public}d", code);
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
    TELEPHONY_LOGD("HRilNetwork ProcessNetworkRequest code:%{public}d", code);
    auto itFunc = reqMemberFuncMap_.find(code);
    if (itFunc != reqMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, data);
        }
    }
}

void HRilNetwork::ProcessNetworkNotify(
    int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen)
{
    TELEPHONY_LOGD("HRilNetwork ProcessCallNotify code:%{public}d", code);
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(code, notifyType, e, response, responseLen);
        }
    }
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
        free(requestInfo);
        return;
    }
    networkFuncs_->GetSignalStrength(requestInfo);
    free(requestInfo);
}

void HRilNetwork::ExchangeRilRssiToHdf(const void *response, size_t responseLen, Rssi &rssi)
{
    HRilRssi *rilRssi = (HRilRssi *)response;

    rssi.gw.rxlev = rilRssi->gwRssi.rxlev;
    rssi.gw.ber = rilRssi->gwRssi.ber;
    rssi.cdma.absoluteRssi = rilRssi->cdmaRssi.absoluteRssi;
    rssi.cdma.ecno = rilRssi->cdmaRssi.ecno;

    rssi.wcdma.rxlev = rilRssi->wcdma.rxlev;
    rssi.wcdma.ber = rilRssi->wcdma.ber;
    rssi.wcdma.ecio = rilRssi->wcdma.ecio;
    rssi.wcdma.rscp = rilRssi->wcdma.rscp;

    rssi.lte.rxlev = rilRssi->lte.rxlev;
    rssi.lte.rsrp = rilRssi->lte.rsrp;
    rssi.lte.rsrq = rilRssi->lte.rsrq;
    rssi.lte.snr = rilRssi->lte.snr;

    TELEPHONY_LOGD("rxlev:%{public}d, rsrp:%{public}d", rssi.lte.rxlev, rssi.lte.rsrp);
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
        free(requestInfo);
        return;
    }
    networkFuncs_->GetCsRegStatus(requestInfo);
    free(requestInfo);
}

int32_t HRilNetwork::GetCsRegStatusResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    CsRegStatusInfo csRegResponse = {};
    int32_t size = responseLen / sizeof(char *);
    if (response == nullptr) {
        TELEPHONY_LOGE("GetCsRegStatusResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        if (size != HRIL_CS_REG_STATUS_MAX_LEN) {
            TELEPHONY_LOGE("GetCsRegStatusResponse Invalid response: nullptr");
            if (responseInfo.error == HRilErrType::NONE)
                responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        } else {
            char **resp = static_cast<char **>(const_cast<void *>(response));
            csRegResponse.regStatus = AtoiNullHandleDef(resp[HRIL_CS_REG_STATE], HRIL_STAT_UNKNOWN);
            csRegResponse.radioTechnology = AtoiNullHandle(resp[HRIL_CS_REG_RESP_ACT]);
            TELEPHONY_LOGD("GetCsRegStatusResponse regStatus:%{public}d, radioTechnology:%{public}d",
                csRegResponse.regStatus, csRegResponse.radioTechnology);
        }
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
        free(requestInfo);
        return;
    }
    networkFuncs_->GetOperatorInfo(requestInfo);
    free(requestInfo);
}

void HRilNetwork::GetNetworkSearchInformation(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_SEARCH_INFORMATION);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        free(requestInfo);
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
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_SELECTION_MODE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetNetworkSelectionMode::Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        free(requestInfo);
        return;
    }
    networkFuncs_->GetNetworkSelectionMode(requestInfo);
    free(requestInfo);
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
        CreateHRilRequest(setNetworkModeInfo.serial, slotId, HREQ_NETWORK_SET_SELECTION_MODE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("SetNetworkSelectionMode::Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetNetworkSearchInformation::networkFuncs_ is nullptr");
        free(requestInfo);
        return;
    }
    HRiSetNetworkModeInfo setModeInfo = {};
    setModeInfo.selectMode = setNetworkModeInfo.selectMode;
    TELEPHONY_LOGD("setModeInfo = %{public}d", setModeInfo.selectMode);
    if (!ConvertToString(&setModeInfo.oper, setNetworkModeInfo.oper, requestInfo)) {
        TELEPHONY_LOGE("SetNetworkSelectionMode::ConvertToString failed");
        free(requestInfo);
        return;
    }
    TELEPHONY_LOGD("SetNetworkSelectionMode selectMode:%{public}s", setModeInfo.oper);
    networkFuncs_->SetNetworkSelectionMode(requestInfo, &setModeInfo);
    FreeStrings(POINTER_NUM, setModeInfo.oper);
    free(requestInfo);
}

void HRilNetwork::SetNetworkLocationUpdate(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_SET_LOCATION_UPDATE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("SetNetworkLocationUpdate::Create Request is fail");
        return;
    }
    networkFuncs_->GetCsRegStatus(requestInfo);
    free(requestInfo);
}

int32_t HRilNetwork::GetOperatorInfoResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    struct OperatorInfoResult operatorInfoResult = {};
    int32_t numStrings = responseLen / sizeof(char *);
    if (response == nullptr || numStrings != static_cast<int32_t>(NetworkParameter::ERR_INVALID_RESPONSE_VALUE)) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        char **resp = static_cast<char **>(const_cast<void *>(response));
        operatorInfoResult.longName = resp[HRIL_LONE_NAME];
        operatorInfoResult.shortName = resp[HRIL_SHORT_NAME];
        operatorInfoResult.numeric = resp[HRIL_NUMERIC];
        TELEPHONY_LOGD("GetOperatorInfoResponse longName:%{public}s, shortName:%{public}s, numeric:%{public}s",
            operatorInfoResult.longName.c_str(), operatorInfoResult.shortName.c_str(),
            operatorInfoResult.numeric.c_str());
    }
    return ResponseMessageParcel(responseInfo, operatorInfoResult, requestNum);
}

int32_t HRilNetwork::NetworkRegStatusUpdated(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter  serviceCallbackNotify_ is null");
        return HDF_FAILURE;
    }
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
    CsRegStatusInfo csRegResponse = {};
    int32_t numStrings = responseLen / sizeof(char *);
    if (response == nullptr || numStrings != HRIL_CS_REG_STATUS_MAX_LEN) {
        TELEPHONY_LOGE("NetworkRegStatusUpdated: Invalid response");
        return HDF_FAILURE;
    } else {
        char **resp = static_cast<char **>(const_cast<void *>(response));
        csRegResponse.regStatus = AtoiNullHandleDef(resp[HRIL_CS_REG_STATE], HRIL_STAT_UNKNOWN);
        csRegResponse.radioTechnology = AtoiNullHandle(resp[HRIL_CS_REG_RESP_ACT]);
        TELEPHONY_LOGD("NetworkRegStatusUpdated regStatus:%{public}d, radioTechnology:%{public}d",
            csRegResponse.regStatus, csRegResponse.radioTechnology);
    }
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&csRegResponse, sizeof(CsRegStatusInfo))) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallbackNotify_->dispatcher->Dispatch(
        serviceCallbackNotify_, HNOTI_NETWORK_CS_REG_STATUS_UPDATED, dataSbuf, nullptr);
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
    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter serviceCallbackNotify_ is null");
        return HDF_FAILURE;
    }
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
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
    int32_t ret = serviceCallbackNotify_->dispatcher->Dispatch(
        serviceCallbackNotify_, HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    TELEPHONY_LOGD("serviceCallbackNotify_->dispatcher->Dispatch ret %{public}d", ret);
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
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_NETWORK_GET_PS_REG_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("Create Request is fail");
        return;
    }
    if (networkFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetPsRegStatus::networkFuncs_ is nullptr");
        free(requestInfo);
        return;
    }
    networkFuncs_->GetPsRegStatus(requestInfo);
    free(requestInfo);
}

int32_t HRilNetwork::GetPsRegStatusResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    PsRegStatusResultInfo psRegResponse = {};
    if (response == nullptr) {
        TELEPHONY_LOGE("GetPsRegStatusResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        int32_t numStrings = responseLen / sizeof(char *);
        if ((numStrings != HRIL_PS_REG_STATUS_MAX_LEN) &&
            (numStrings != static_cast<int32_t>(NetworkParameter::INVALID_RESPONSE_VALUE))) {
            TELEPHONY_LOGE("GetPsRegStatusResponse Invalid response: nullptr");
            if (responseInfo.error == HRilErrType::NONE) {
                responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
            }
        } else {
            char **resp = static_cast<char **>(const_cast<void *>(response));

            psRegResponse.regStatus =
                AtoiNullHandleDef(resp[HRIL_PS_REG_STATE], HRilCircuitModeRegState::HRIL_STAT_UNKNOWN);
            psRegResponse.radioTechnology = AtoiNullHandle(resp[HRIL_PS_RADIO_TECHNOLOGY]);
            TELEPHONY_LOGD("GetPsRegStatusResponse regStatus:%{public}d, radioTechnology:%{public}d",
                psRegResponse.regStatus, psRegResponse.radioTechnology);
        }
    }
    return ResponseMessageParcel(responseInfo, psRegResponse, requestNum);
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
        TELEPHONY_LOGD("selectModeResultInfo.selectMode: %{public}d", *resp);
        selectModeResultInfo.selectMode = *resp;
    }
    return ResponseMessageParcel(responseInfo, selectModeResultInfo, requestNum);
}

void HRilNetwork::BuildOperatorList(AvailableNetworkList &availableNetworkList,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    int32_t numStrings = responseLen / sizeof(AvailableOperInfo *);
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(AvailableOperInfo *)) != 0) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        AvailableNetworkInfo operInfo;
        availableNetworkList.itemNum = numStrings;
        TELEPHONY_LOGD("availableNetworkList.itemNum: %{public}d", numStrings);
        for (int32_t i = 0; i < numStrings; i++) {
            AvailableOperInfo *curPtr = ((AvailableOperInfo **)response)[i];
            if (curPtr != nullptr) {
                operInfo.status = curPtr->status;
                TELEPHONY_LOGD("operInfo.status:%{public}d", curPtr->status);
                operInfo.longName = curPtr->longName;
                TELEPHONY_LOGD("operInfo.longName:%{public}s", curPtr->longName);
                operInfo.numeric = curPtr->numeric;
                TELEPHONY_LOGD("operInfo.numeric:%{public}s", curPtr->numeric);
                operInfo.shortName = curPtr->shortName;
                TELEPHONY_LOGD("operInfo.shortName:%{public}s", curPtr->shortName);
                operInfo.rat = curPtr->rat;
                TELEPHONY_LOGD("operInfo.rat:%{public}d", curPtr->rat);
            }
            availableNetworkList.availableNetworkInfo.push_back(operInfo);
        }
    }
}

int32_t HRilNetwork::GetNetworkSearchInformationResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    TELEPHONY_LOGD("GetNetworkSearchInformationResponse ---> response %{public}p", response);
    AvailableNetworkList availableNetworkList;
    availableNetworkList.itemNum = 0;
    BuildOperatorList(availableNetworkList, responseInfo, response, responseLen);

    return ResponseMessageParcel(responseInfo, availableNetworkList, requestNum);
}

int32_t HRilNetwork::SetNetworkLocationUpdateResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    LocationUpdateResultInfo locationUpdateResultInfo;
    int32_t numStrings = responseLen / sizeof(char *);
    if (response == nullptr || numStrings != static_cast<int32_t>(NetworkParameter::ERR_INVALID_RESPONSE_VALUE) ||
        (numStrings != HRIL_PS_REG_STATUS_MAX_LEN)) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        char **resp = static_cast<char **>(const_cast<void *>(response));
        locationUpdateResultInfo.regState = AtoiNullHandle(resp[HRIL_LOCATION_REG_STATE]);
        locationUpdateResultInfo.lac = resp[HRIL_LOCATION_LOC];
        locationUpdateResultInfo.ci = resp[HRIL_LOCATION_CI];
    }
    return ResponseMessageParcel(responseInfo, locationUpdateResultInfo, requestNum);
}

int32_t HRilNetwork::SetNetworkSelectionModeResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
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
