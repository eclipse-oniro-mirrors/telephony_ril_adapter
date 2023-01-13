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

#include "hril_sms.h"

#include "hril_notification.h"
#include "hril_request.h"

namespace OHOS {
namespace Telephony {
namespace {
const size_t HEX_WIDTH = 2;
const size_t MAX_PDU_LEN = 255;
const size_t MAX_LEN = 100;
const size_t MAX_CHN_LEN = 50000;
const int32_t MSG_DEFAULT_INDEX = -1;
} // namespace

HRilSms::HRilSms(int32_t slotId, IHRilReporter &hrilReporter) : HRilBase(slotId, hrilReporter)
{
    AddHandlerToMap();
}

bool HRilSms::IsSmsRespOrNotify(uint32_t code)
{
    return IsSmsResponse(code) || IsSmsNotification(code);
}

void HRilSms::RegisterSmsFuncs(const HRilSmsReq *smsFuncs)
{
    smsFuncs_ = smsFuncs;
}

void HRilSms::AddHandlerToMap()
{
    // Response
    respMemberFuncMap_[HREQ_SMS_SEND_GSM_SMS] = &HRilSms::SendGsmSmsResponse;
    respMemberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &HRilSms::SendSmsMoreModeResponse;
    respMemberFuncMap_[HREQ_SMS_SEND_SMS_ACK] = &HRilSms::SendSmsAckResponse;
    respMemberFuncMap_[HREQ_SMS_ADD_SIM_MESSAGE] = &HRilSms::AddSimMessageResponse;
    respMemberFuncMap_[HREQ_SMS_DEL_SIM_MESSAGE] = &HRilSms::DelSimMessageResponse;
    respMemberFuncMap_[HREQ_SMS_UPDATE_SIM_MESSAGE] = &HRilSms::UpdateSimMessageResponse;
    respMemberFuncMap_[HREQ_SMS_SET_SMSC_ADDR] = &HRilSms::SetSmscAddrResponse;
    respMemberFuncMap_[HREQ_SMS_GET_SMSC_ADDR] = &HRilSms::GetSmscAddrResponse;
    respMemberFuncMap_[HREQ_SMS_SET_CB_CONFIG] = &HRilSms::SetCBConfigResponse;
    respMemberFuncMap_[HREQ_SMS_GET_CB_CONFIG] = &HRilSms::GetCBConfigResponse;
    respMemberFuncMap_[HREQ_SMS_GET_CDMA_CB_CONFIG] = &HRilSms::GetCdmaCBConfigResponse;
    respMemberFuncMap_[HREQ_SMS_SET_CDMA_CB_CONFIG] = &HRilSms::SetCdmaCBConfigResponse;
    respMemberFuncMap_[HREQ_SMS_SEND_CDMA_SMS] = &HRilSms::SendCdmaSmsResponse;
    respMemberFuncMap_[HREQ_SMS_ADD_CDMA_SIM_MESSAGE] = &HRilSms::AddCdmaSimMessageResponse;
    respMemberFuncMap_[HREQ_SMS_DEL_CDMA_SIM_MESSAGE] = &HRilSms::DelCdmaSimMessageResponse;
    respMemberFuncMap_[HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE] = &HRilSms::UpdateCdmaSimMessageResponse;

    // Notification
    notiMemberFuncMap_[HNOTI_SMS_NEW_SMS] = &HRilSms::NewSmsNotify;
    notiMemberFuncMap_[HNOTI_SMS_NEW_CDMA_SMS] = &HRilSms::NewCdmaSmsNotify;
    notiMemberFuncMap_[HNOTI_SMS_STATUS_REPORT] = &HRilSms::SmsStatusReportNotify;
    notiMemberFuncMap_[HNOTI_SMS_NEW_SMS_STORED_ON_SIM] = &HRilSms::NewSmsStoredOnSimNotify;
    notiMemberFuncMap_[HNOTI_CB_CONFIG_REPORT] = &HRilSms::CBConfigNotify;
}

int32_t HRilSms::SendGsmSms(int32_t serialId, const OHOS::HDI::Ril::V1_0::GsmSmsMessageInfo &gsmSmsMessageInfo)
{
    const int32_t COUNT_STRINGS_VALUE = 2;
    return RequestWithStrings(serialId, HREQ_SMS_SEND_GSM_SMS, COUNT_STRINGS_VALUE, gsmSmsMessageInfo.smscPdu.c_str(),
        gsmSmsMessageInfo.pdu.c_str());
}

int32_t HRilSms::SendCdmaSms(int32_t serialId, const OHOS::HDI::Ril::V1_0::SendCdmaSmsMessageInfo &cdmaSmsMessageInfo)
{
    return RequestVendor(serialId, HREQ_SMS_SEND_CDMA_SMS, smsFuncs_, &HRilSmsReq::SendCdmaSms,
        cdmaSmsMessageInfo.smscPdu.c_str(), cdmaSmsMessageInfo.smscPdu.size());
}

int32_t HRilSms::AddSimMessage(int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &message)
{
    HRilSmsWriteSms msg = {};
    msg.state = message.state;
    msg.index = MSG_DEFAULT_INDEX;
    size_t pduLen = message.pdu.length();
    if (pduLen > MAX_PDU_LEN) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CopyToCharPoint(&msg.pdu, message.pdu);
    size_t smscPduLen = message.smscPdu.length() + 1;
    if (smscPduLen > MAX_PDU_LEN) {
        SafeFrees(msg.pdu);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CopyToCharPoint(&msg.smsc, message.smscPdu);
    int32_t result = RequestVendor(
        serialId, HREQ_SMS_ADD_SIM_MESSAGE, smsFuncs_, &HRilSmsReq::AddSimMessage, &msg, sizeof(HRilSmsWriteSms));
    TELEPHONY_LOGI("AddSimMessage result is: %{public}d", result);
    SafeFrees(msg.pdu, msg.smsc);
    return result;
}

int32_t HRilSms::DelSimMessage(int32_t serialId, int32_t index)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->DelSimMessage == nullptr)) {
        TELEPHONY_LOGE("smsFuncs_ or smsFuncs_->DelSimMessage is null");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serialId, HREQ_SMS_DEL_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t *pBuff = nullptr;
    RequestWithInts(&pBuff, requestInfo, 1, index);
    smsFuncs_->DelSimMessage(requestInfo, pBuff, 0);
    if (pBuff != nullptr) {
        SafeFrees(pBuff);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::UpdateSimMessage(int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &message)
{
    HRilSmsWriteSms msg = {};
    size_t len = 0;
    len = message.pdu.size() + 1;
    msg.state = message.state;
    if (len > MAX_PDU_LEN) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CopyToCharPoint(&msg.pdu, message.pdu);
    msg.index = message.index;
    int32_t result = RequestVendor(
        serialId, HREQ_SMS_UPDATE_SIM_MESSAGE, smsFuncs_, &HRilSmsReq::UpdateSimMessage, &msg, sizeof(HRilSmsWriteSms));
    TELEPHONY_LOGI("UpdateSimMessage result is: %{public}d", result);
    SafeFrees(msg.pdu);
    return result;
}

int32_t HRilSms::SetSmscAddr(int32_t serialId, const OHOS::HDI::Ril::V1_0::ServiceCenterAddress &serCenterAddress)
{
    HRilServiceCenterAddress address;
    size_t len = 0;
    len = strlen(serCenterAddress.address.c_str()) + 1;
    address.tosca = serCenterAddress.tosca;
    if (len > MAX_LEN) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CopyToCharPoint(&address.address, serCenterAddress.address);
    int32_t result = RequestVendor(serialId, HREQ_SMS_SET_SMSC_ADDR, smsFuncs_, &HRilSmsReq::SetSmscAddr, &address,
        sizeof(HRilServiceCenterAddress));
    TELEPHONY_LOGI("SetSmscAddr result is: %{public}d", result);
    SafeFrees(address.address);
    return result;
}

int32_t HRilSms::GetSmscAddr(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_SMS_GET_SMSC_ADDR, smsFuncs_, &HRilSmsReq::GetSmscAddr);
}

int32_t HRilSms::SetCBConfig(int32_t serialId, const OHOS::HDI::Ril::V1_0::CBConfigInfo &broadcastInfo)
{
    HRilCBConfigInfo cellBroadcastInfo;
    cellBroadcastInfo.mode = broadcastInfo.mode;
    size_t midsLen = broadcastInfo.mids.size() + 1;
    if (midsLen > MAX_CHN_LEN) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CopyToCharPoint(&cellBroadcastInfo.mids, broadcastInfo.mids);
    size_t dcssLen = broadcastInfo.dcss.size() + 1;
    if (dcssLen <= 0) {
        SafeFrees(cellBroadcastInfo.mids);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CopyToCharPoint(&cellBroadcastInfo.dcss, broadcastInfo.dcss);
    int32_t result = RequestVendor(serialId, HREQ_SMS_SET_CB_CONFIG, smsFuncs_, &HRilSmsReq::SetCBConfig,
        &cellBroadcastInfo, sizeof(HRilServiceCenterAddress));
    TELEPHONY_LOGI("SetCBConfig result is: %{public}d", result);
    SafeFrees(cellBroadcastInfo.mids, cellBroadcastInfo.dcss);
    return result;
}

int32_t HRilSms::GetCBConfig(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_SMS_GET_CB_CONFIG, smsFuncs_, &HRilSmsReq::GetCBConfig);
}

int32_t HRilSms::SetCdmaCBConfig(
    int32_t serialId, const OHOS::HDI::Ril::V1_0::CdmaCBConfigInfoList &cellBroadcastInfoList)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->SetCdmaCBConfig == nullptr)) {
        TELEPHONY_LOGE("(smsFuncs_ or msFuncs_->SetCdmaCBConfig is null");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serialId, HREQ_SMS_SET_CDMA_CB_CONFIG);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    auto size = cellBroadcastInfoList.size;
    std::unique_ptr<HRilCdmaCBConfigInfo[]> list = std::make_unique<HRilCdmaCBConfigInfo[]>(size);
    CopyToHRilCdmaCBConfigInfo(list.get(), cellBroadcastInfoList);
    smsFuncs_->SetCdmaCBConfig(requestInfo, list.get(), cellBroadcastInfoList.size * sizeof(HRilCdmaCBConfigInfo));
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::GetCdmaCBConfig(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_SMS_GET_CDMA_CB_CONFIG, smsFuncs_, &HRilSmsReq::GetCdmaCBConfig);
}

int32_t HRilSms::SendSmsMoreMode(int32_t serialId, const OHOS::HDI::Ril::V1_0::GsmSmsMessageInfo &gsmSmsMessageInfo)
{
    const int32_t COUNT_STRINGS_VALUE = 2;
    return RequestWithStrings(serialId, HREQ_SMS_SEND_SMS_MORE_MODE, COUNT_STRINGS_VALUE,
        gsmSmsMessageInfo.smscPdu.c_str(), gsmSmsMessageInfo.pdu.c_str());
}

int32_t HRilSms::SendSmsAck(int32_t serialId, const OHOS::HDI::Ril::V1_0::ModeData &modeData)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->SendSmsAck == nullptr)) {
        TELEPHONY_LOGE("smsFuncs_ or smsFuncs_->SendSmsAck is null");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serialId, HREQ_SMS_SEND_SMS_ACK);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t *pBuff = nullptr;
    const int32_t COUNT_INTS_VALUE = 2;
    RequestWithInts(&pBuff, requestInfo, COUNT_INTS_VALUE, static_cast<int32_t>(modeData.result), modeData.mode);
    smsFuncs_->SendSmsAck(requestInfo, pBuff, sizeof(int32_t));
    if (pBuff != nullptr) {
        SafeFrees(pBuff);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::AddCdmaSimMessage(int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &message)
{
    HRilSmsWriteSms msg = {};
    int32_t pduLen = 0;
    const int32_t MSG_INDEX = -1;
    msg.state = message.state;
    msg.index = MSG_INDEX;
    pduLen = message.pdu.length();
    int32_t len = pduLen + 1;
    if (len <= 0) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CopyToCharPoint(&msg.pdu, message.pdu);
    int32_t result = RequestVendor(serialId, HREQ_SMS_ADD_CDMA_SIM_MESSAGE, smsFuncs_, &HRilSmsReq::AddCdmaSimMessage,
        &msg, sizeof(HRilSmsWriteSms));
    TELEPHONY_LOGI("AddCdmaSimMessage result is: %{public}d", result);
    SafeFrees(msg.pdu);
    return result;
}

int32_t HRilSms::DelCdmaSimMessage(int32_t serialId, int32_t index)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->DelCdmaSimMessage == nullptr)) {
        TELEPHONY_LOGE("smsFuncs_ or smsFuncs_->DelCdmaSimMessage is null");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serialId, HREQ_SMS_DEL_CDMA_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t *pBuff = nullptr;
    RequestWithInts(&pBuff, requestInfo, 1, index);
    smsFuncs_->DelCdmaSimMessage(requestInfo, pBuff, 0);
    if (pBuff != nullptr) {
        SafeFrees(pBuff);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::UpdateCdmaSimMessage(int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &message)
{
    HRilSmsWriteSms msg = {};
    size_t len = 0;
    len = message.pdu.size() + 1;
    msg.state = message.state;
    if (len > MAX_CHN_LEN) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    CopyToCharPoint(&msg.pdu, message.pdu);
    msg.index = message.index;
    RequestVendor(serialId, HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE, smsFuncs_, &HRilSmsReq::UpdateCdmaSimMessage, &msg,
        sizeof(HRilSmsWriteSms));
    SafeFrees(msg.pdu);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::SendGsmSmsResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::SendSmsResultInfo result =
        MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SendGsmSmsResponse, result);
}

int32_t HRilSms::SendCdmaSmsResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::SendSmsResultInfo result =
        MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SendCdmaSmsResponse, result);
}

int32_t HRilSms::AddSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::AddSimMessageResponse);
}

int32_t HRilSms::DelSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::DelSimMessageResponse);
}

int32_t HRilSms::UpdateSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::UpdateSimMessageResponse);
}

int32_t HRilSms::SetSmscAddrResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetSmscAddrResponse);
}

int32_t HRilSms::GetSmscAddrResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::ServiceCenterAddress result;
    if (response == nullptr || responseLen != sizeof(HRilServiceCenterAddress)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
        result.address = std::string("");
    } else {
        const HRilServiceCenterAddress *address = static_cast<const HRilServiceCenterAddress *>(response);
        result.tosca = address->tosca;
        if (address->address == nullptr) {
            result.address = std::string("");
            TELEPHONY_LOGE("address is nullptr");
        } else {
            result.address = std::string(address->address);
        }
        TELEPHONY_LOGE("result address:%{public}s, tosca:%{public}d", result.address.c_str(), result.tosca);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetSmscAddrResponse, result);
}

int32_t HRilSms::SetCBConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetCBConfigResponse);
}

int32_t HRilSms::GetCBConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::CBConfigInfo broadcastInfo;
    if (response == nullptr || responseLen != sizeof(HRilCBConfigInfo)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        broadcastInfo.mode = -1;
        broadcastInfo.mids = std::string("");
        broadcastInfo.dcss = std::string("");
    } else {
        HRilCBConfigInfo *cellBroadcastInfo = (HRilCBConfigInfo *)response;
        broadcastInfo.mode = cellBroadcastInfo->mode;
        if (cellBroadcastInfo->mids == nullptr) {
            broadcastInfo.mids = std::string("");
            TELEPHONY_LOGE("result.mids is nullptr");
        } else {
            TELEPHONY_LOGI("result.mids :%{private}s", cellBroadcastInfo->mids);
            broadcastInfo.mids = std::string(cellBroadcastInfo->mids);
        }
        if (cellBroadcastInfo->dcss == nullptr) {
            broadcastInfo.dcss = std::string("");
            TELEPHONY_LOGE("result.dcss is nullptr");
        } else {
            TELEPHONY_LOGI("result.dcss :%{private}s", cellBroadcastInfo->dcss);
            broadcastInfo.dcss = std::string(cellBroadcastInfo->dcss);
        }
        TELEPHONY_LOGD("mode:%{private}d, mids:%{private}s, dcss:%{private}s", cellBroadcastInfo->mode,
            cellBroadcastInfo->mids, cellBroadcastInfo->dcss);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCBConfigResponse, broadcastInfo);
}

int32_t HRilSms::SetCdmaCBConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetCdmaCBConfigResponse);
}

int32_t HRilSms::GetCdmaCBConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::CdmaCBConfigInfo broadcastInfo;
    if (response == nullptr || responseLen != sizeof(HRilCdmaCBConfigInfo)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        broadcastInfo.checked = -1;
        broadcastInfo.language = -1;
        broadcastInfo.service = -1;
    } else {
        const HRilCdmaCBConfigInfo *cellBroadcastInfo = static_cast<const HRilCdmaCBConfigInfo *>(response);
        broadcastInfo.checked = cellBroadcastInfo->checked;
        broadcastInfo.language = cellBroadcastInfo->language;
        broadcastInfo.service = cellBroadcastInfo->service;
        TELEPHONY_LOGE("checked:%{public}c, language:%{public}d, service:%{public}d", cellBroadcastInfo->checked,
            cellBroadcastInfo->language, cellBroadcastInfo->service);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetCdmaCBConfigResponse, broadcastInfo);
}

int32_t HRilSms::SendSmsMoreModeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::SendSmsResultInfo result =
        MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SendSmsMoreModeResponse, result);
}

int32_t HRilSms::SendSmsAckResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SendSmsAckResponse);
}

int32_t HRilSms::AddCdmaSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::AddCdmaSimMessageResponse);
}

int32_t HRilSms::DelCdmaSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::DelCdmaSimMessageResponse);
}

int32_t HRilSms::UpdateCdmaSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::UpdateCdmaSimMessageResponse);
}

int32_t HRilSms::SmsStatusReportNotify(
    int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("invalid response");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HDI::Ril::V1_0::SmsMessageInfo smsMessageInfo;
    uint8_t *bytes = ConvertHexStringToBytes(response, responseLen);
    if (bytes == nullptr) {
        TELEPHONY_LOGE("ConvertHexStringToBytes in SmsStatusReportNotify is failed!");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    const size_t MESSAGE_SIZE = responseLen / HEX_WIDTH;
    smsMessageInfo.size = MESSAGE_SIZE;
    smsMessageInfo.indicationType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    uint8_t *temp = bytes;
    for (int32_t i = 0; i < smsMessageInfo.size; i++) {
        smsMessageInfo.pdu.push_back(*temp);
        temp++;
    }
    SafeFrees(bytes);
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::SmsStatusReportNotify, smsMessageInfo);
}

int32_t HRilSms::NewSmsStoredOnSimNotify(
    int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(int32_t)) {
        TELEPHONY_LOGE("invalid response");
        return HRIL_ERR_SUCCESS;
    }
    int32_t recordNumber = *(static_cast<const int32_t *>(response));
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::NewSmsStoredOnSimNotify, recordNumber, indType);
}

int32_t HRilSms::NewSmsNotify(int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    HRilSmsResponse *smsResponse = nullptr;
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("NewSmsNotify: invalid response");
        return HDF_FAILURE;
    } else {
        smsResponse = (HRilSmsResponse *)response;
    }
    uint8_t *bytes = ConvertHexStringToBytes(smsResponse->pdu, responseLen);
    if (bytes == nullptr) {
        TELEPHONY_LOGE("NewSmsNotify: ConvertHexStringToBytes failed");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HDI::Ril::V1_0::SmsMessageInfo smsMessageInfo;
    const size_t NEW_SMS_SIZE = responseLen / HEX_WIDTH;
    smsMessageInfo.size = NEW_SMS_SIZE;
    smsMessageInfo.indicationType = indType;
    uint8_t *temp = bytes;
    for (int32_t i = 0; i < static_cast<int32_t>(smsMessageInfo.size); i++) {
        smsMessageInfo.pdu.push_back(*temp);
        temp++;
    }
    SafeFrees(bytes);
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::NewSmsNotify, smsMessageInfo);
}

int32_t HRilSms::NewCdmaSmsNotify(int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    HRilSmsResponse *message = nullptr;
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("invalid response");
        return HRIL_ERR_GENERIC_FAILURE;
    } else {
        message = (HRilSmsResponse *)response;
    }
    HDI::Ril::V1_0::SmsMessageInfo messageInfo;
    messageInfo.indicationType = indType;
    if (message->pdu != nullptr) {
        messageInfo.size = strlen(message->pdu) / HEX_WIDTH;
        uint8_t *bytes = ConvertHexStringToBytes(message->pdu, strlen(message->pdu));
        if (bytes == nullptr) {
            TELEPHONY_LOGE("bytes is nullptr");
            return HRIL_ERR_GENERIC_FAILURE;
        }
        uint8_t *temp = bytes;
        for (int32_t i = 0; i < messageInfo.size; i++) {
            messageInfo.pdu.push_back(*temp);
            temp++;
        }
        SafeFrees(bytes);
    }
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::NewCdmaSmsNotify, messageInfo);
}

int32_t HRilSms::CBConfigNotify(int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::CBConfigReportInfo result = MakeCBConfigResult(response, responseLen);
    result.indicationType = indType;
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::CBConfigNotify, result);
}

bool HRilSms::IsSmsResponse(uint32_t code)
{
    return ((code >= HREQ_SMS_BASE) && (code < HREQ_SIM_BASE));
}

bool HRilSms::IsSmsNotification(uint32_t code)
{
    return ((code >= HNOTI_SMS_BASE) && (code < HNOTI_SIM_BASE));
}

int32_t HRilSms::RequestWithInts(int32_t **p, ReqDataInfo *requestInfo, int32_t argCount, ...)
{
    size_t len = sizeof(int32_t);
    if (len <= 0 || argCount <= 0) {
        return false;
    }
    *p = static_cast<int32_t *>(malloc(argCount * len));
    if (*p == nullptr) {
        TELEPHONY_LOGE("req: [%{public}d,%{public}d,%{public}d], malloc fail", requestInfo->serial,
            static_cast<int32_t>(requestInfo->slotId), requestInfo->request);
        return false;
    }
    if (memset_s(*p, argCount * len, 0, argCount * len) != EOK) {
        TELEPHONY_LOGE("RequestWithInts memset_s failed");
        SafeFrees(*p);
        return false;
    }
    va_list list;
    va_start(list, argCount);
    int32_t i = 0;
    while (i < argCount) {
        (*p)[i] = va_arg(list, int32_t);
        i++;
    }
    va_end(list);
    return true;
}

int32_t HRilSms::RequestWithStrings(int32_t serial, int32_t request, int32_t count, ...)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->SendGsmSms == nullptr)) {
        TELEPHONY_LOGE("smsFuncs_ or smsFuncs_->SendGsmSms is null");
        return HRIL_ERR_NULL_POINT;
    }
    char **pBuff = nullptr;
    if (count <= 0) {
        return HRIL_ERR_NULL_POINT;
    }
    int32_t len = count * sizeof(char *);
    pBuff = (char **)malloc(len);
    if (pBuff == nullptr) {
        TELEPHONY_LOGE("req: [%{public}d,%{public}d], malloc fail", serial, request);
        return HRIL_ERR_NULL_POINT;
    }
    if (memset_s(pBuff, len, 0, len) != EOK) {
        TELEPHONY_LOGE("RequestWithInts memset_s failed");
        SafeFrees(pBuff);
        return false;
    }
    va_list list;
    va_start(list, count);
    int32_t i = 0;
    while (i < count) {
        const char *str = va_arg(list, const char *);
        if (!ConvertToString(&pBuff[i], str)) {
            TELEPHONY_LOGE("ConvertToString in RequestWithStrings is failed!");
            va_end(list);
            for (int32_t j = 0; j < i; j++) {
                SafeFrees(pBuff[j]);
            }
            SafeFrees(pBuff);
            return HRIL_ERR_NULL_POINT;
        }
        i++;
    }
    va_end(list);
    int32_t result = RequestVendor(serial, request, smsFuncs_, &HRilSmsReq::SendGsmSms, pBuff, 0);
    if (pBuff != nullptr) {
        i = 0;
        while (i < count) {
            SafeFrees(pBuff[i]);
            i++;
        }
        SafeFrees(pBuff);
    }
    return result;
}

HDI::Ril::V1_0::CBConfigReportInfo HRilSms::MakeCBConfigResult(const void *response, const size_t responseLen)
{
    HDI::Ril::V1_0::CBConfigReportInfo result;
    if (response == nullptr || responseLen != sizeof(HRilCBConfigReportInfo)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        result.data = std::string("");
        result.pdu = std::string("");
        result.dcs = std::string("");
    } else {
        const HRilCBConfigReportInfo *cellBroadcastReportInfo = static_cast<const HRilCBConfigReportInfo *>(response);
        result.mid = cellBroadcastReportInfo->mid;
        result.length = cellBroadcastReportInfo->length;
        result.page = cellBroadcastReportInfo->page;
        result.pages = cellBroadcastReportInfo->pages;
        result.sn = cellBroadcastReportInfo->sn;
        if (cellBroadcastReportInfo->data == nullptr) {
            result.data = std::string("");
            TELEPHONY_LOGE("result.data is nullptr");
        } else {
            TELEPHONY_LOGI("result.data :%{private}s", cellBroadcastReportInfo->data);
            result.data = std::string(cellBroadcastReportInfo->data);
        }
        if (cellBroadcastReportInfo->pdu == nullptr) {
            result.pdu = std::string("");
            TELEPHONY_LOGE("result.pdu is nullptr");
        } else {
            TELEPHONY_LOGI("result.pdu :%{private}s", cellBroadcastReportInfo->pdu);
            result.pdu = std::string(cellBroadcastReportInfo->pdu);
        }
        if (cellBroadcastReportInfo->dcs == nullptr) {
            result.dcs = std::string("");
            TELEPHONY_LOGE("result.dcs is nullptr");
        } else {
            TELEPHONY_LOGI("result.dcs :%{private}s", cellBroadcastReportInfo->dcs);
            result.dcs = std::string(cellBroadcastReportInfo->dcs);
        }
    }
    return result;
}

HDI::Ril::V1_0::SendSmsResultInfo HRilSms::MakeSendSmsResult(
    HRilRadioResponseInfo &responseInfo, int32_t serial, const void *response, const size_t responseLen)
{
    HDI::Ril::V1_0::SendSmsResultInfo result;
    if (response == nullptr || responseLen != sizeof(HRilSmsResponse)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
        result.pdu = std::string("");
    } else {
        const HRilSmsResponse *smsResponse = static_cast<const HRilSmsResponse *>(response);
        result.msgRef = smsResponse->msgRef;
        if (smsResponse->pdu == nullptr) {
            result.pdu = std::string("");
            TELEPHONY_LOGE("result.pdu is nullptr");
        } else {
            result.pdu = std::string(smsResponse->pdu);
        }
        result.errCode = smsResponse->errCode;
    }
    return result;
}

void HRilSms::CopyToHRilCdmaCBConfigInfo(
    HRilCdmaCBConfigInfo *list, OHOS::HDI::Ril::V1_0::CdmaCBConfigInfoList cellBroadcastInfoList)
{
    for (int32_t i = 0; i < cellBroadcastInfoList.size; i++) {
        list[i].service = cellBroadcastInfoList.list[i].service;
        list[i].language = cellBroadcastInfoList.list[i].language;
        list[i].checked = cellBroadcastInfoList.list[i].checked;
    }
}
} // namespace Telephony
} // namespace OHOS
