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

#include "hril_data.h"

namespace OHOS {
namespace Telephony {
HRilData::HRilData()
{
    AddHandlerToMap();
}

HRilData::~HRilData()
{
    respMemberFuncMap_.clear();
    notiMemberFuncMap_.clear();
    reqMemberFuncMap_.clear();
    dataFuncs_ = nullptr;
}

void HRilData::DeactivatePdpContext(int32_t slotId, struct HdfSBuf *data)
{
    struct UniInfo uInfo;
    HRilDataInfo dataInfo = {};
    MessageParcel *parcel = nullptr;

    if (data == nullptr) {
        TELEPHONY_LOGE("RilAdapter DeactivatePdpContext data is null!");
        return;
    }
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (!uInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(uInfo.serial, slotId, HREQ_DATA_DEACTIVATE_PDP_CONTEXT);
    if (requestInfo == nullptr || dataFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest! requestInfo=%{public}p", requestInfo);
        if (requestInfo != nullptr) {
            free(requestInfo);
        }
        return;
    }
    (void)memset_s(&dataInfo, sizeof(HRilDataInfo), 0, sizeof(HRilDataInfo));
    dataInfo.cid = uInfo.gsmIndex;
    dataFuncs_->DeactivatePdpContext(requestInfo, &dataInfo);
    free(requestInfo);
}

void HRilData::ActivatePdpContext(int32_t slotId, struct HdfSBuf *data)
{
    struct DataCallInfo dataCallInfo;
    HRilDataInfo dataInfo = {};
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel:");
        return;
    }
    if (!dataCallInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    const std::string &protocol = (dataCallInfo.isRoaming ? dataCallInfo.dataProfileInfo.roamingProtocol :
                                                            dataCallInfo.dataProfileInfo.protocol);

    ReqDataInfo *requestInfo = CreateHRilRequest(dataCallInfo.serial, slotId, HREQ_DATA_ACTIVATE_PDP_CONTEXT);
    if (requestInfo == nullptr || dataFuncs_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest! requestInfo=%{public}p", requestInfo);
        if (requestInfo != nullptr) {
            free(requestInfo);
        }
        return;
    }
    if (!ConvertToString(&dataInfo.apn, dataCallInfo.dataProfileInfo.apn, requestInfo)) {
        TELEPHONY_LOGE("RilAdapter failed to do ConvertToString!");
        free(requestInfo);
        return;
    }
    if (!ConvertToString(&dataInfo.type, protocol, requestInfo)) {
        TELEPHONY_LOGE("RilAdapter failed to do ConvertToString!");
        free(requestInfo);
        return;
    }

    dataFuncs_->ActivatePdpContext(requestInfo, &dataInfo);
    free(dataInfo.apn);
    free(dataInfo.type);
    free(requestInfo);
}

void HRilData::SwitchRilDataToHal(HRilDataCallResponse *response, SetupDataCallResultInfo &result)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("SwitchRilDataToHal response is null!!!");
        return;
    }
    result.active = response->active;
    result.state = response->state;
    result.retryTime = response->retryTime;
    result.cid = response->cid;
    if (response->address == nullptr) {
        result.address = std::string("");
    } else {
        result.address = std::string(response->address);
    }
    if (response->type == nullptr) {
        result.type = std::string("");
    } else {
        result.type = std::string(response->type);
    }
    if (response->dns == nullptr) {
        result.dns = std::string("");
    } else {
        result.dns = std::string(response->dns);
    }
    if (response->netPortName == nullptr) {
        result.netPortName = std::string("");
    } else {
        result.netPortName = std::string(response->netPortName);
    }
    if (response->gateway == nullptr) {
        result.gateway = std::string("");
    } else {
        result.gateway = std::string(response->gateway);
    }
    result.mtu = response->maxTransferUnit;
}

int32_t HRilData::DeactivatePdpContextResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    SetupDataCallResultInfo result = {};
    if (response == nullptr || (responseLen % sizeof(HRilDataCallResponse)) != 0) {
        if (response != nullptr) {
            TELEPHONY_LOGE("Invalid response");
            if (responseInfo.error == HRilErrType::NONE) {
                responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
            }
        }
        result.state = HRIL_ERROR_UNSPECIFIED_RSN;
        result.type = std::string("");
        result.netPortName = std::string("");
        result.address = std::string("");
        result.dns = std::string("");
        result.gateway = std::string("");
    } else {
        TELEPHONY_LOGD("SwitchRilDataToHal ===>");
        SwitchRilDataToHal((HRilDataCallResponse *)response, result);
    }
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilData::ActivatePdpContextResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    SetupDataCallResultInfo result = {};

    if (response == nullptr || (responseLen % sizeof(HRilDataCallResponse)) != 0) {
        if (response != nullptr) {
            TELEPHONY_LOGE("Invalid response");
            if (responseInfo.error == HRilErrType::NONE) {
                responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
            }
        }
        result.state = HRIL_ERROR_UNSPECIFIED_RSN;
        result.type = std::string("");
        result.netPortName = std::string("");
        result.address = std::string("");
        result.dns = std::string("");
        result.gateway = std::string("");
    } else {
        TELEPHONY_LOGD("SwitchRilDataToHal ===>");
        SwitchRilDataToHal((HRilDataCallResponse *)response, result);
    }
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

void HRilData::SwitchHRilDataListToHal(
    const void *response, size_t responseLen, std::vector<SetupDataCallResultInfo> &dcResultList)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("SwitchHRilDataListToHal response is null!!!");
        return;
    }
    int32_t dataNum = responseLen / sizeof(HRilDataCallResponse);
    HRilDataCallResponse *dataCallResponse = (HRilDataCallResponse *)response;
    dcResultList.resize(dataNum);

    int32_t i = 0;
    while (i < dataNum) {
        SwitchRilDataToHal(&dataCallResponse[i], dcResultList[i]);
        i++;
    }
}

int32_t HRilData::PdpContextListUpdated(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    TELEPHONY_LOGD("======>enter");
    if (response == nullptr) {
        TELEPHONY_LOGE("response is null.");
        return HDF_FAILURE;
    }
    if ((response == nullptr && responseLen != 0) || responseLen % sizeof(HRilDataCallResponse) != 0) {
        TELEPHONY_LOGE("invalid response");
        return HDF_SUCCESS;
    }
    DataCallResultList dataCallResultList;
    SwitchHRilDataListToHal(response, responseLen, dataCallResultList.dcList);
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is null.");
        return HDF_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    dataCallResultList.size = dataCallResultList.dcList.size();
    dataCallResultList.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is null.");
        return HDF_FAILURE;
    }
    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter serviceCallbackNotify_ is null");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallbackNotify_->dispatcher->Dispatch(
        serviceCallbackNotify_, HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("Call Dispatch is failed. ret:%{public}d", ret);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

void HRilData::ProcessDataResponse(
    int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    TELEPHONY_LOGD("HRilData ProcessCallResponse code:%{public}d", code);
    auto itFunc = respMemberFuncMap_.find(code);
    if (itFunc != respMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, code, responseInfo, response, responseLen);
        }
    }
}

void HRilData::ProcessDataRequest(int32_t slotId, int32_t code, struct HdfSBuf *data)
{
    TELEPHONY_LOGD("HRilData ProcessCallRequest code:%{public}d", code);
    auto itFunc = reqMemberFuncMap_.find(code);
    if (itFunc != reqMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, data);
        }
    }
}

void HRilData::ProcessDataNotify(
    int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen)
{
    TELEPHONY_LOGD("HRilData ProcessCallNotify code:%{public}d", code);
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            int32_t slotId = 0;
            (this->*memberFunc)(slotId, notifyType, e, response, responseLen);
        }
    }
}

bool HRilData::IsDataResponse(uint32_t code)
{
    return ((code >= HREQ_DATA_BASE) && (code < HREQ_NETWORK_BASE));
}

bool HRilData::IsDataNotification(uint32_t code)
{
    return ((code >= HNOTI_DATA_BASE) && (code < HNOTI_NETWORK_BASE));
}

bool HRilData::IsDataRespOrNotify(uint32_t code)
{
    return IsDataResponse(code) || IsDataNotification(code);
}

void HRilData::AddHandlerToMap()
{
    // Notification
    notiMemberFuncMap_[HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED] = &HRilData::PdpContextListUpdated;
    // Response
    respMemberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &HRilData::ActivatePdpContextResponse;
    respMemberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &HRilData::DeactivatePdpContextResponse;
    // ReqFunc
    reqMemberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &HRilData::ActivatePdpContext;
    reqMemberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &HRilData::DeactivatePdpContext;
}

void HRilData::RegisterDataFuncs(const HRilDataReq *dataFuncs)
{
    dataFuncs_ = dataFuncs;
}
} // namespace Telephony
} // namespace OHOS
