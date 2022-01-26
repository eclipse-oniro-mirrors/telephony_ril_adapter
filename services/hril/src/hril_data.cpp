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

#include "hril_notification.h"
#include "hril_request.h"

namespace OHOS {
namespace Telephony {
HRilData::HRilData(int32_t slotId, IHRilReporter &hrilReporter) : HRilBase(slotId, hrilReporter)
{
    AddHandlerToMap();
}

HRilData::~HRilData()
{
    dataFuncs_ = nullptr;
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
    // response
    respMemberFuncMap_[HREQ_DATA_SET_INIT_APN_INFO] = &HRilData::SetInitApnInfoResponse;
    respMemberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &HRilData::ActivatePdpContextResponse;
    respMemberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &HRilData::DeactivatePdpContextResponse;
    respMemberFuncMap_[HREQ_DATA_GET_PDP_CONTEXT_LIST] = &HRilData::GetPdpContextListResponse;
    respMemberFuncMap_[HREQ_DATA_GET_LINK_BANDWIDTH_INFO] = &HRilData::GetLinkBandwidthInfoResponse;
    respMemberFuncMap_[HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE] = &HRilData::SetLinkBandwidthReportingRuleResponse;

    // ReqFunc
    reqMemberFuncMap_[HREQ_DATA_SET_INIT_APN_INFO] = &HRilData::SetInitApnInfo;
    reqMemberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &HRilData::ActivatePdpContext;
    reqMemberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &HRilData::DeactivatePdpContext;
    reqMemberFuncMap_[HREQ_DATA_GET_PDP_CONTEXT_LIST] = &HRilData::GetPdpContextList;
    reqMemberFuncMap_[HREQ_DATA_GET_LINK_BANDWIDTH_INFO] = &HRilData::GetLinkBandwidthInfo;
    reqMemberFuncMap_[HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE] = &HRilData::SetLinkBandwidthReportingRule;
}

void HRilData::SwitchRilDataToHal(const HRilDataCallResponse *response, SetupDataCallResultInfo &result)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("SwitchRilDataToHal response is null!!!");
        return;
    }
    result.active = response->active;
    result.reason = response->reason;
    result.retryTime = response->retryTime;
    result.cid = response->cid;
    result.pduSessionId = response->pduSessionId;
    result.maxTransferUnit = response->maxTransferUnit;
    result.address = (response->address == nullptr) ? "" : response->address;
    result.type = (response->type == nullptr) ? "" : response->type;
    result.dns = (response->dns == nullptr) ? "" : response->dns;
    result.dnsSec = (response->dnsSec == nullptr) ? "" : response->dnsSec;
    result.netPortName = (response->netPortName == nullptr) ? "" : response->netPortName;
    result.gateway = (response->gateway == nullptr) ? "" : response->gateway;
    result.pCscfPrimAddr = (response->pCscfPrimAddr == nullptr) ? "" : response->pCscfPrimAddr;
    result.pCscfSecAddr = (response->pCscfSecAddr == nullptr) ? "" : response->pCscfSecAddr;
}

void HRilData::SwitchHRilDataListToHal(
    const void *response, size_t responseLen, std::vector<SetupDataCallResultInfo> &dcResultList)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("SwitchHRilDataListToHal response is null!!!");
        return;
    }
    int32_t dataNum = responseLen / sizeof(HRilDataCallResponse);
    const HRilDataCallResponse *dataCallResponse = (const HRilDataCallResponse *)response;
    dcResultList.resize(dataNum);

    int32_t i = 0;
    while (i < dataNum) {
        SwitchRilDataToHal(&dataCallResponse[i], dcResultList[i]);
        i++;
    }
}

int32_t HRilData::DeactivatePdpContext(struct HdfSBuf *data)
{
    if (dataFuncs_ == nullptr || dataFuncs_->DeactivatePdpContext == nullptr || data == nullptr) {
        TELEPHONY_LOGE("dataFuncs_:%{public}p or dataFuncs_->DeactivatePdpContext or data:%{public}p is nullptr!",
            dataFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    struct UniInfo uInfo;
    HRilDataInfo dataInfo = {};
    MessageParcel *parcel = nullptr;
    if (data == nullptr) {
        TELEPHONY_LOGE("RilAdapter data is null!");
        return HRIL_ERR_NULL_POINT;
    }
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel: %p", parcel);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!uInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(uInfo.serial, HREQ_DATA_DEACTIVATE_PDP_CONTEXT);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest! requestInfo=%{public}p", requestInfo);
        return HRIL_ERR_NULL_POINT;
    }
    dataInfo.cid = uInfo.gsmIndex;
    dataInfo.reason = uInfo.arg1;
    dataFuncs_->DeactivatePdpContext(requestInfo, &dataInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::ActivatePdpContext(struct HdfSBuf *data)
{
    if (dataFuncs_ == nullptr || dataFuncs_->ActivatePdpContext == nullptr || data == nullptr) {
        TELEPHONY_LOGE("dataFuncs_:%{public}p or dataFuncs_->ActivatePdpContext or data:%{public}p is nullptr!",
            dataFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    struct DataCallInfo dataCallInfo;
    HRilDataInfo dataInfo;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel:");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!dataCallInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    dataInfo.apn = StringToCString(dataCallInfo.dataProfileInfo.apn);
    dataInfo.type = StringToCString(dataCallInfo.dataProfileInfo.protocol);
    dataInfo.roamingType = StringToCString(dataCallInfo.dataProfileInfo.roamingProtocol);
    dataInfo.userName = StringToCString(dataCallInfo.dataProfileInfo.userName);
    dataInfo.password = StringToCString(dataCallInfo.dataProfileInfo.password);
    dataInfo.verType = dataCallInfo.dataProfileInfo.verType;
    dataInfo.rat = dataCallInfo.radioTechnology;
    dataInfo.roamingEnable = dataCallInfo.roamingAllowed ? 1 : 0;
    ReqDataInfo *requestInfo = CreateHRilRequest(dataCallInfo.serial, HREQ_DATA_ACTIVATE_PDP_CONTEXT);
    if (requestInfo == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    dataFuncs_->ActivatePdpContext(requestInfo, &dataInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::GetPdpContextList(struct HdfSBuf *data)
{
    if (dataFuncs_ == nullptr || dataFuncs_->GetPdpContextList == nullptr || data == nullptr) {
        TELEPHONY_LOGE("dataFuncs_:%{public}p or dataFuncs_->GetPdpContextList or data:%{public}p is nullptr!",
            dataFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    struct UniInfo uInfo;
    MessageParcel *parcel = nullptr;

    if (data == nullptr) {
        TELEPHONY_LOGE("RilAdapter data is null!");
        return HRIL_ERR_NULL_POINT;
    }
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!uInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    TELEPHONY_LOGI("serial %{public}d on %{public}d", uInfo.serial, uInfo.flag);
    ReqDataInfo *requestInfo = CreateHRilRequest(uInfo.serial, HREQ_DATA_GET_PDP_CONTEXT_LIST);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    dataFuncs_->GetPdpContextList(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::SetInitApnInfo(struct HdfSBuf *data)
{
    if (dataFuncs_ == nullptr || dataFuncs_->SetInitApnInfo == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "dataFuncs_:%{public}p or dataFuncs_->SetInitApnInfo or data:%{public}p is nullptr!", dataFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    DataProfileDataInfo dataProfileInfo;
    HRilDataInfo dataInfo;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel:");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!dataProfileInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    dataInfo.apn = StringToCString(dataProfileInfo.apn);
    dataInfo.type = StringToCString(dataProfileInfo.protocol);
    dataInfo.roamingType = StringToCString(dataProfileInfo.roamingProtocol);
    dataInfo.userName = StringToCString(dataProfileInfo.userName);
    dataInfo.password = StringToCString(dataProfileInfo.password);
    dataInfo.verType = dataProfileInfo.verType;
    ReqDataInfo *requestInfo = CreateHRilRequest(dataProfileInfo.serial, HREQ_DATA_SET_INIT_APN_INFO);
    if (requestInfo == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    dataFuncs_->SetInitApnInfo(requestInfo, &dataInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::GetLinkBandwidthInfo(struct HdfSBuf *data)
{
    if (dataFuncs_ == nullptr || dataFuncs_->GetLinkBandwidthInfo == nullptr || data == nullptr) {
        TELEPHONY_LOGE("dataFuncs_:%{public}p or dataFuncs_->GetLinkBandwidthInfo or data:%{public}p is nullptr!",
            dataFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    int32_t cid;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &cid)) {
        TELEPHONY_LOGE("miss mode parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_DATA_GET_LINK_BANDWIDTH_INFO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    dataFuncs_->GetLinkBandwidthInfo(requestInfo, cid);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::SetLinkBandwidthReportingRule(struct HdfSBuf *data)
{
    if (dataFuncs_ == nullptr || dataFuncs_->SetLinkBandwidthReportingRule == nullptr || data == nullptr) {
        TELEPHONY_LOGE(
            "dataFuncs_:%{public}p or dataFuncs_->SetLinkBandwidthReportingRule or data:%{public}p is nullptr!",
            dataFuncs_, data);
        return HRIL_ERR_NULL_POINT;
    }
    DataLinkBandwidthReportingRule linkBandwidthRule;
    HRilLinkBandwidthReportingRule hLinkBandwidthRule;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel:");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!linkBandwidthRule.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    hLinkBandwidthRule.rat = (RatType)linkBandwidthRule.rat;
    hLinkBandwidthRule.delayMs = linkBandwidthRule.delayMs;
    hLinkBandwidthRule.delayUplinkKbps = linkBandwidthRule.delayUplinkKbps;
    hLinkBandwidthRule.delayDownlinkKbps = linkBandwidthRule.delayDownlinkKbps;
    hLinkBandwidthRule.maximumUplinkKbpsSize = linkBandwidthRule.maximumUplinkKbpsSize;
    hLinkBandwidthRule.maximumDownlinkKbpsSize = linkBandwidthRule.maximumDownlinkKbpsSize;
    hLinkBandwidthRule.maximumUplinkKbps = new int32_t[linkBandwidthRule.maximumUplinkKbpsSize];
    hLinkBandwidthRule.maximumDownlinkKbps = new int32_t[linkBandwidthRule.maximumDownlinkKbpsSize];

    TELEPHONY_LOGI("maximumUplinkKbpsSize:%{public}d, maximumDownlinkKbpsSize:%{public}d",
        linkBandwidthRule.maximumUplinkKbpsSize, linkBandwidthRule.maximumDownlinkKbpsSize);
    for (int32_t i = 0; i < hLinkBandwidthRule.maximumUplinkKbpsSize; i++) {
        hLinkBandwidthRule.maximumUplinkKbps[i] = linkBandwidthRule.maximumUplinkKbps[i];
    }
    for (int32_t i = 0; i < hLinkBandwidthRule.maximumDownlinkKbpsSize; i++) {
        hLinkBandwidthRule.maximumDownlinkKbps[i] = linkBandwidthRule.maximumDownlinkKbps[i];
    }
    ReqDataInfo *requestInfo =
        CreateHRilRequest(linkBandwidthRule.serial, HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE);
    if (requestInfo == nullptr) {
        SafeFrees(hLinkBandwidthRule.maximumUplinkKbps, hLinkBandwidthRule.maximumDownlinkKbps);
        return HRIL_ERR_NULL_POINT;
    }
    dataFuncs_->SetLinkBandwidthReportingRule(requestInfo, &hLinkBandwidthRule);
    SafeFrees(hLinkBandwidthRule.maximumUplinkKbps, hLinkBandwidthRule.maximumDownlinkKbps);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::DeactivatePdpContextResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilData::ActivatePdpContextResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilDataCallResponse)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    SetupDataCallResultInfo result = {};
    result.reason = HRIL_ERROR_UNSPECIFIED_RSN;
    result.cid = -1;
    if (response != nullptr) {
        SwitchRilDataToHal((HRilDataCallResponse *)response, result);
    }
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilData::GetPdpContextListResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilDataCallResponse)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    DataCallResultList dataCallResultList = {};
    if (response != nullptr) {
        SwitchHRilDataListToHal(response, responseLen, dataCallResultList.dcList);
    }
    return ResponseMessageParcel(responseInfo, dataCallResultList, requestNum);
}

int32_t HRilData::SetInitApnInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilData::SetLinkBandwidthReportingRuleResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilData::PdpContextListUpdated(
    int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    if ((response == nullptr) || (responseLen % sizeof(HRilEmergencyInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    DataCallResultList dataCallResultList = {};
    SwitchHRilDataListToHal(response, responseLen, dataCallResultList.dcList);
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel is null.");
        return HRIL_ERR_NULL_POINT;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is null.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    dataCallResultList.size = dataCallResultList.dcList.size();
    dataCallResultList.Marshalling(*parcel.get());
    int32_t ret = ServiceNotifyDispatcher(HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSbufRecycle(dataSbuf);
        TELEPHONY_LOGE("Call Dispatch is failed. ret:%{public}d", ret);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSbufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilData::GetLinkBandwidthInfoResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilLinkBandwidthInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    DataLinkBandwidthInfo uplinkAndDownlinkBandwidthInfo = {};

    if (response != nullptr) {
        const HRilLinkBandwidthInfo *result = static_cast<const HRilLinkBandwidthInfo *>(response);
        uplinkAndDownlinkBandwidthInfo.cid = result->cid;
        uplinkAndDownlinkBandwidthInfo.qi = result->qi;
        uplinkAndDownlinkBandwidthInfo.dlGfbr = result->dlGfbr;
        uplinkAndDownlinkBandwidthInfo.ulGfbr = result->ulGfbr;
        uplinkAndDownlinkBandwidthInfo.dlMfbr = result->dlMfbr;
        uplinkAndDownlinkBandwidthInfo.ulMfbr = result->ulMfbr;
        uplinkAndDownlinkBandwidthInfo.ulSambr = result->ulSambr;
        uplinkAndDownlinkBandwidthInfo.dlSambr = result->dlSambr;
        uplinkAndDownlinkBandwidthInfo.averagingWindow = result->averagingWindow;
    }
    return ResponseMessageParcel(responseInfo, uplinkAndDownlinkBandwidthInfo, requestNum);
}

void HRilData::RegisterDataFuncs(const HRilDataReq *dataFuncs)
{
    dataFuncs_ = dataFuncs;
}
} // namespace Telephony
} // namespace OHOS
