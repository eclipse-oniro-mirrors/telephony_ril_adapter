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
const int32_t HEX_WIDTH = 2;

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

    // Request
    reqMemberFuncMap_[HREQ_SMS_SEND_GSM_SMS] = &HRilSms::SendGsmSms;
    reqMemberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &HRilSms::SendSmsMoreMode;
    reqMemberFuncMap_[HREQ_SMS_SEND_SMS_ACK] = &HRilSms::SendSmsAck;
    reqMemberFuncMap_[HREQ_SMS_ADD_SIM_MESSAGE] = &HRilSms::AddSimMessage;
    reqMemberFuncMap_[HREQ_SMS_DEL_SIM_MESSAGE] = &HRilSms::DelSimMessage;
    reqMemberFuncMap_[HREQ_SMS_UPDATE_SIM_MESSAGE] = &HRilSms::UpdateSimMessage;
    reqMemberFuncMap_[HREQ_SMS_SET_SMSC_ADDR] = &HRilSms::SetSmscAddr;
    reqMemberFuncMap_[HREQ_SMS_GET_SMSC_ADDR] = &HRilSms::GetSmscAddr;
    reqMemberFuncMap_[HREQ_SMS_SET_CB_CONFIG] = &HRilSms::SetCBConfig;
    reqMemberFuncMap_[HREQ_SMS_GET_CB_CONFIG] = &HRilSms::GetCBConfig;
    reqMemberFuncMap_[HREQ_SMS_GET_CDMA_CB_CONFIG] = &HRilSms::GetCdmaCBConfig;
    reqMemberFuncMap_[HREQ_SMS_SET_CDMA_CB_CONFIG] = &HRilSms::SetCdmaCBConfig;
    reqMemberFuncMap_[HREQ_SMS_SEND_CDMA_SMS] = &HRilSms::SendCdmaSms;
    reqMemberFuncMap_[HREQ_SMS_ADD_CDMA_SIM_MESSAGE] = &HRilSms::AddCdmaSimMessage;
    reqMemberFuncMap_[HREQ_SMS_DEL_CDMA_SIM_MESSAGE] = &HRilSms::DelCdmaSimMessage;
    reqMemberFuncMap_[HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE] = &HRilSms::UpdateCdmaSimMessage;
}

int32_t HRilSms::SendGsmSms(struct HdfSBuf *data)
{
    struct GsmSmsMessageInfo message;
    MessageParcel *parcel = nullptr;
    const int32_t COUNT_STRINGS_VALUE = 2;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel int32_t SendGsmSms is nullptr!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    return RequestWithStrings(
        message.serial, HREQ_SMS_SEND_GSM_SMS, COUNT_STRINGS_VALUE, message.smscPdu.c_str(), message.pdu.c_str());
}

int32_t HRilSms::SendCdmaSms(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->SendCdmaSms == nullptr)) {
        TELEPHONY_LOGE("SendCdmaSms::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    struct SendCdmaSmsMessageInfo message;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel int32_t SendCdmaSms is nullptr!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(message.serial, HREQ_SMS_SEND_CDMA_SMS);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->SendCdmaSms(requestInfo, message.smscPdu.c_str(), message.smscPdu.size());
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::AddSimMessage(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->AddSimMessage == nullptr)) {
        TELEPHONY_LOGE("AddSimMessage::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    struct GsmSmsMessageInfo message = {};
    HRilSmsWriteSms msg = {};
    int32_t pduLen = 0;
    int32_t smscPduLen = 0;
    const int32_t MSG_INDEX = -1;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel parcel=%{public}p", parcel);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    msg.state = message.state;
    msg.index = MSG_INDEX;
    pduLen = message.pdu.length();
    int32_t len = pduLen + 1;
    if (len <= 0) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    msg.pdu = (char *)malloc(len * sizeof(char));
    if (msg.pdu == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    (void)(int32_t) strcpy_s(msg.pdu, pduLen + 1, message.pdu.c_str());
    smscPduLen = message.smscPdu.length() + 1;
    if (smscPduLen <= 0) {
        free(msg.pdu);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    msg.smsc = (char *)malloc(smscPduLen * sizeof(char));
    if (msg.smsc == nullptr) {
        free(msg.pdu);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    (void)strcpy_s(msg.smsc, smscPduLen, message.smscPdu.c_str());
    ReqDataInfo *requestInfo = CreateHRilRequest(message.serial, HREQ_SMS_ADD_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        SafeFrees(msg.pdu, msg.smsc);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->AddSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    SafeFrees(msg.pdu, msg.smsc);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::DelSimMessage(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->DelSimMessage == nullptr)) {
        TELEPHONY_LOGE("DelSimMessage::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t *pBuff = nullptr;
    int32_t index = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("DelSimMessage >>> miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("DelSimMessage >> miss index parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SMS_DEL_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    RequestWithInts(&pBuff, requestInfo, 1, index);
    smsFuncs_->DelSimMessage(requestInfo, pBuff, 0);
    if (pBuff != nullptr) {
        free(pBuff);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::UpdateSimMessage(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->UpdateSimMessage == nullptr)) {
        TELEPHONY_LOGE("UpdateSimMessage::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    struct SmsMessageIOInfo message = {};
    HRilSmsWriteSms msg = {};
    int32_t len = 0;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in UpdateSimMessage is nullptr!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    len = message.pdu.size() + 1;
    msg.state = message.state;
    if (len <= 0) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    msg.pdu = (char *)malloc(len * sizeof(char));
    if (msg.pdu == nullptr) {
        TELEPHONY_LOGE("malloc in UpdateSimMessage is failed!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    msg.index = message.index;
    (void)strcpy_s(msg.pdu, message.pdu.size() + 1, message.pdu.c_str());
    ReqDataInfo *requestInfo = CreateHRilRequest(message.serial, HREQ_SMS_ADD_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        free(msg.pdu);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->UpdateSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    free(msg.pdu);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::SetSmscAddr(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->SetSmscAddr == nullptr)) {
        TELEPHONY_LOGE("SetSmscAddr::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    struct ServiceCenterAddress serCenterAddress;
    HRilServiceCenterAddress address;
    int32_t len = 0;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SetSmscAddr is nullptr!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!serCenterAddress.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    len = strlen(serCenterAddress.address.c_str()) + 1;
    address.tosca = serCenterAddress.tosca;
    if (len <= 0) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    address.address = (char *)malloc(len * sizeof(char));
    if (address.address == nullptr) {
        TELEPHONY_LOGE("malloc in SetSmscAddr is failed!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t ret =
        (int32_t)strcpy_s(address.address, serCenterAddress.address.size() + 1, serCenterAddress.address.c_str());
    if (ret != 0) {
        TELEPHONY_LOGE("RilAdapter Failed to copy string error!");
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serCenterAddress.serial, HREQ_SMS_SET_SMSC_ADDR);
    if (requestInfo == nullptr) {
        free(address.address);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->SetSmscAddr(requestInfo, &address, sizeof(HRilServiceCenterAddress));
    free(address.address);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::GetSmscAddr(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->GetSmscAddr == nullptr)) {
        TELEPHONY_LOGE("GetSmscAddr::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SMS_GET_SMSC_ADDR);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->GetSmscAddr(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::SetCBConfig(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->SetCBConfig == nullptr)) {
        TELEPHONY_LOGE("SetCBConfig::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    struct CBConfigInfo broadcastInfo;
    HRilCBConfigInfo cellBroadcastInfo;
    int32_t midsLen = 0;
    int32_t dcssLen = 0;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel parcel=%{public}p", parcel);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!broadcastInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    cellBroadcastInfo.mode = broadcastInfo.mode;
    midsLen = broadcastInfo.mids.size() + 1;
    if (midsLen <= 0) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    cellBroadcastInfo.mids = (char *)malloc(midsLen * sizeof(char));
    if (cellBroadcastInfo.mids == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    (void)strcpy_s(cellBroadcastInfo.mids, broadcastInfo.mids.size() + 1, broadcastInfo.mids.c_str());
    dcssLen = broadcastInfo.dcss.size() + 1;
    if (dcssLen <= 0) {
        free(cellBroadcastInfo.mids);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    cellBroadcastInfo.dcss = (char *)malloc(dcssLen * sizeof(char));
    if (cellBroadcastInfo.dcss == nullptr) {
        free(cellBroadcastInfo.mids);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    (void)strcpy_s(cellBroadcastInfo.dcss, broadcastInfo.dcss.size() + 1, broadcastInfo.dcss.c_str());
    ReqDataInfo *requestInfo = CreateHRilRequest(broadcastInfo.serial, HREQ_SMS_SET_CB_CONFIG);
    if (requestInfo == nullptr) {
        SafeFrees(cellBroadcastInfo.mids, cellBroadcastInfo.dcss);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->SetCBConfig(requestInfo, &cellBroadcastInfo, sizeof(HRilServiceCenterAddress));
    SafeFrees(cellBroadcastInfo.mids, cellBroadcastInfo.dcss);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::GetCBConfig(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->GetCBConfig == nullptr)) {
        TELEPHONY_LOGE("GetCBConfig::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SMS_GET_CB_CONFIG);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->GetCBConfig(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::SetCdmaCBConfig(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->SetCdmaCBConfig == nullptr)) {
        TELEPHONY_LOGE("SetCdmaCBConfig::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    CdmaCBConfigInfoList cellBroadcastInfoList = {};
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SetCdmaCBConfig is nullptr!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!cellBroadcastInfoList.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilCdmaCBConfigInfo list[cellBroadcastInfoList.size];
    for (int32_t i = 0; i < cellBroadcastInfoList.size; i++) {
        list[i].service = cellBroadcastInfoList.list[i].service;
        list[i].language = cellBroadcastInfoList.list[i].language;
        list[i].checked = cellBroadcastInfoList.list[i].checked;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(cellBroadcastInfoList.serial, HREQ_SMS_SET_CDMA_CB_CONFIG);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->SetCdmaCBConfig(requestInfo, list, cellBroadcastInfoList.size * sizeof(HRilCdmaCBConfigInfo));

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::GetCdmaCBConfig(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->GetCdmaCBConfig == nullptr)) {
        TELEPHONY_LOGE("GetCdmaCBConfig::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SMS_GET_CDMA_CB_CONFIG);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->GetCdmaCBConfig(requestInfo);

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::SendSmsMoreMode(struct HdfSBuf *data)
{
    struct GsmSmsMessageInfo message;
    MessageParcel *parcel = nullptr;
    const int32_t COUNT_STRINGS_VALUE = 2;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SendSmsMoreMode is nullptr!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    return RequestWithStrings(message.serial, HREQ_SMS_SEND_SMS_MORE_MODE, COUNT_STRINGS_VALUE,
        message.smscPdu.c_str(), message.pdu.c_str());
}

int32_t HRilSms::SendSmsAck(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->SendSmsAck == nullptr)) {
        TELEPHONY_LOGE("SendSmsAck::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t *pBuff = nullptr;
    struct ModeData mode;
    MessageParcel *parcel = nullptr;
    const int32_t COUNT_INTS_VALUE = 2;

    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to SbufToParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SendSmsAck is nullptr!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!mode.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(mode.serial, HREQ_SMS_SEND_SMS_ACK);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    RequestWithInts(&pBuff, requestInfo, COUNT_INTS_VALUE, static_cast<int32_t>(mode.result), mode.mode);
    smsFuncs_->SendSmsAck(requestInfo, pBuff, sizeof(int32_t));
    if (pBuff != nullptr) {
        free(pBuff);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::AddCdmaSimMessage(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->AddCdmaSimMessage == nullptr)) {
        TELEPHONY_LOGE("AddCdmaSimMessage::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    struct SmsMessageIOInfo mSmsMessageIOInfo = {};
    HRilSmsWriteSms msg = {};
    int32_t pduLen = 0;
    const int32_t MSG_INDEX = -1;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel parcel=%{public}p", parcel);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!mSmsMessageIOInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    msg.state = mSmsMessageIOInfo.state;
    msg.index = MSG_INDEX;
    pduLen = mSmsMessageIOInfo.pdu.length();
    int32_t len = pduLen + 1;
    if (len <= 0) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    msg.pdu = (char *)malloc(len * sizeof(char));
    if (msg.pdu == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    (void)(int32_t) strcpy_s(msg.pdu, pduLen + 1, mSmsMessageIOInfo.pdu.c_str());
    ReqDataInfo *requestInfo = CreateHRilRequest(mSmsMessageIOInfo.serial, HREQ_SMS_ADD_CDMA_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        free(msg.pdu);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    smsFuncs_->AddCdmaSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    SafeFrees(msg.pdu);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::DelCdmaSimMessage(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->DelCdmaSimMessage == nullptr)) {
        TELEPHONY_LOGE("DelCdmaSimMessage::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t *pBuff = nullptr;
    int32_t index = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("HdfSbufReadInt32 >>> miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("HdfSbufReadInt32 >> miss index parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SMS_DEL_CDMA_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    RequestWithInts(&pBuff, requestInfo, 1, index);
    smsFuncs_->DelCdmaSimMessage(requestInfo, pBuff, 0);
    if (pBuff != nullptr) {
        free(pBuff);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::UpdateCdmaSimMessage(struct HdfSBuf *data)
{
    if ((smsFuncs_ == nullptr) || (smsFuncs_->UpdateCdmaSimMessage == nullptr)) {
        TELEPHONY_LOGE("UpdateCdmaSimMessage::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    struct SmsMessageIOInfo mSmsMessageIOInfo = {};
    HRilSmsWriteSms msg = {};
    int32_t len = 0;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in UpdateCdmaSimMessage is nullptr!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!mSmsMessageIOInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(mSmsMessageIOInfo.serial, HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    len = mSmsMessageIOInfo.pdu.size() + 1;
    msg.state = mSmsMessageIOInfo.state;
    if (len <= 0) {
        free(requestInfo);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    msg.pdu = (char *)malloc(len * sizeof(char));
    if (msg.pdu == nullptr) {
        TELEPHONY_LOGE("malloc in UpdateCdmaSimMessage is failed!");
        free(requestInfo);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    msg.index = mSmsMessageIOInfo.index;
    (void)strcpy_s(msg.pdu, mSmsMessageIOInfo.pdu.size() + 1, mSmsMessageIOInfo.pdu.c_str());
    smsFuncs_->UpdateCdmaSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    free(msg.pdu);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::SendGsmSmsResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    SendSmsResultInfo result = MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSms::SendCdmaSmsResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    SendSmsResultInfo result = MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSms::AddSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::DelSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::UpdateSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::SetSmscAddrResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::GetSmscAddrResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    ServiceCenterAddress result;

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
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSms::SetCBConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::GetCBConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    struct CBConfigInfo broadcastInfo;
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
            TELEPHONY_LOGE("result.mids :%{public}s", cellBroadcastInfo->mids);
            broadcastInfo.mids = std::string(cellBroadcastInfo->mids);
        }
        if (cellBroadcastInfo->dcss == nullptr) {
            broadcastInfo.dcss = std::string("");
            TELEPHONY_LOGE("result.dcss is nullptr");
        } else {
            TELEPHONY_LOGE("result.dcss :%{public}s", cellBroadcastInfo->dcss);
            broadcastInfo.dcss = std::string(cellBroadcastInfo->dcss);
        }
        TELEPHONY_LOGE("mode:%{public}d, mids:%{public}s, dcss:%{public}s", cellBroadcastInfo->mode,
            cellBroadcastInfo->mids, cellBroadcastInfo->dcss);
    }
    return ResponseMessageParcel(responseInfo, broadcastInfo, requestNum);
}

int32_t HRilSms::SetCdmaCBConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::GetCdmaCBConfigResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    struct CdmaCBConfigInfo broadcastInfo;

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
    return ResponseBuffer(
        requestNum, &responseInfo, sizeof(HRilRadioResponseInfo), &broadcastInfo, sizeof(struct CdmaCBConfigInfo));
}

int32_t HRilSms::SendSmsMoreModeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    SendSmsResultInfo result = MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSms::SendSmsAckResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SendSmsAckResponse is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf in SendSmsAckResponse is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(HRilRadioResponseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in SendSmsAckResponse is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in SendSmsAckResponse is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::AddCdmaSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::DelCdmaSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::UpdateCdmaSimMessageResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::SmsStatusReportNotify(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    const int32_t MESSAGE_SIZE = responseLen / HEX_WIDTH;
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("invalid response");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    uint8_t *bytes = ConvertHexStringToBytes(response, responseLen);
    if (bytes == nullptr) {
        TELEPHONY_LOGE("ConvertHexStringToBytes in SmsStatusReportNotify is failed!");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    SmsMessageInfo smsMessageInfo;
    smsMessageInfo.size = MESSAGE_SIZE;
    smsMessageInfo.indicationType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    uint8_t *temp = bytes;
    for (int32_t i = 0; i < smsMessageInfo.size; i++) {
        smsMessageInfo.pdu.push_back(*temp);
        temp++;
    }
    free(bytes);
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SmsStatusReportNotify is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    parcel->WriteInt32(GetSlotId());
    smsMessageInfo.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("ParcelToSbuf in SmsStatusReportNotify is failed!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in SmsStatusReportNotify is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_STATUS_REPORT, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in SmsStatusReportNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSbufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::NewSmsStoredOnSimNotify(
    int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(int32_t)) {
        TELEPHONY_LOGE("invalid response");
        return HRIL_ERR_SUCCESS;
    }
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
    int32_t recordNumber = *(static_cast<const int32_t *>(response));
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in NewSmsStoredOnSimNotify is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("HdfSbufTypedObtain in NewSmsStoredOnSimNotify is failed!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in SmsStatusReportNotify is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, recordNumber)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NewSmsStoredOnSimNotify is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NewSmsStoredOnSimNotify is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_NEW_SMS_STORED_ON_SIM, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in NewSmsStoredOnSimNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        TELEPHONY_LOGE("dataSbuf in NewSmsStoredOnSimNotify is nullptr!");
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::NewSmsNotify(int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    const int32_t NEW_SMS_SIZE = responseLen / HEX_WIDTH;
    HRilSmsResponse *smsResponse = nullptr;
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("NewSmsNotify: invalid response");
        return HDF_FAILURE;
    } else {
        smsResponse = (HRilSmsResponse *)response;
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    uint8_t *bytes = ConvertHexStringToBytes(smsResponse->pdu, responseLen);
    if (bytes == nullptr) {
        TELEPHONY_LOGE("NewSmsNotify: ConvertHexStringToBytes failed");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    SmsMessageInfo smsMessageInfo;
    smsMessageInfo.size = NEW_SMS_SIZE;
    smsMessageInfo.indicationType = indType;
    uint8_t *temp = bytes;
    for (int32_t i = 0; i < static_cast<int32_t>(smsMessageInfo.size); i++) {
        smsMessageInfo.pdu.push_back(*temp);
        temp++;
    }
    free(bytes);
    bytes = nullptr;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in NewSmsNotify is nullptr!");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    parcel->WriteInt32(GetSlotId());
    smsMessageInfo.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (DataSbuf(dataSbuf, indType) == HRIL_ERR_GENERIC_FAILURE) {
        TELEPHONY_LOGE("DataSbuf in NewSmsNotify is failed!");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::NewCdmaSmsNotify(int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    HRilSmsResponse *message = nullptr;
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("invalid response");
        return HRIL_ERR_GENERIC_FAILURE;
    } else {
        message = (HRilSmsResponse *)response;
    }

    SmsMessageInfo messageInfo;
    messageInfo.indicationType = indType;
    if (message->pdu == nullptr) {
        std::move(messageInfo.pdu);
    } else {
        messageInfo.size = strlen(message->pdu) / HEX_WIDTH;
        uint8_t *bytes = ConvertHexStringToBytes(message->pdu, strlen(message->pdu));
        uint8_t *temp = bytes;
        for (int32_t i = 0; i < messageInfo.size; i++) {
            messageInfo.pdu.push_back(*temp);
            temp++;
        }
        free(bytes);
    }

    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in NewCdmaSmsNotify is nullptr");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    parcel->WriteInt32(GetSlotId());
    messageInfo.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_NEW_CDMA_SMS, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::CBConfigNotify(int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    CBConfigReportInfo result = MakeCBConfigResult(response, responseLen);
    result.indicationType = indType;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in CBConfigNotify is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!parcel->WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    parcel->WriteInt32(GetSlotId());
    result.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("ParcelToSbuf in CBConfigNotify is failed!");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in CBConfigNotify is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_CB_CONFIG_REPORT, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in CBConfigNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

bool HRilSms::IsSmsResponse(uint32_t code)
{
    return ((code >= HREQ_SMS_BASE) && (code < HREQ_SIM_BASE));
}

bool HRilSms::IsSmsNotification(uint32_t code)
{
    return ((code >= HNOTI_SMS_BASE) && (code < HNOTI_SIM_BASE));
}

int32_t HRilSms::DataSbuf(HdfSBuf *dataSbuf, int32_t indType)
{
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in DataSbuf is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_NEW_SMS, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in DataSbuf is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSbufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
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
            (int32_t)requestInfo->slotId, requestInfo->request);
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
        TELEPHONY_LOGE("SendGsmSms::smsFuncs_:%{public}p", smsFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    char **pBuff = nullptr;
    if (count <= 0) {
        return HRIL_ERR_NULL_POINT;
    }
    pBuff = (char **)malloc(count * sizeof(int32_t));
    if (pBuff == nullptr) {
        TELEPHONY_LOGE("req: [%{public}d,%{public}d], malloc fail", serial, request);
        return HRIL_ERR_NULL_POINT;
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
            free(pBuff);
            return HRIL_ERR_NULL_POINT;
        }
        i++;
    }
    va_end(list);
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, request);
    if (requestInfo != nullptr) {
        smsFuncs_->SendGsmSms(requestInfo, pBuff, 0);
        return HRIL_ERR_NULL_POINT;
    }
    if (pBuff != nullptr) {
        i = 0;
        while (i < count) {
            SafeFrees(pBuff[i]);
            i++;
        }
        free(pBuff);
    }
    return HRIL_ERR_SUCCESS;
}

CBConfigReportInfo HRilSms::MakeCBConfigResult(const void *response, const size_t responseLen)
{
    CBConfigReportInfo result;
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
            TELEPHONY_LOGE("result.data :%{public}s", cellBroadcastReportInfo->data);
            result.data = std::string(cellBroadcastReportInfo->data);
        }
        if (cellBroadcastReportInfo->pdu == nullptr) {
            result.pdu = std::string("");
            TELEPHONY_LOGE("result.pdu is nullptr");
        } else {
            TELEPHONY_LOGE("result.pdu :%{public}s", cellBroadcastReportInfo->pdu);
            result.pdu = std::string(cellBroadcastReportInfo->pdu);
        }
        if (cellBroadcastReportInfo->dcs == nullptr) {
            result.dcs = std::string("");
            TELEPHONY_LOGE("result.dcs is nullptr");
        } else {
            TELEPHONY_LOGE("result.dcs :%{public}s", cellBroadcastReportInfo->dcs);
            result.dcs = std::string(cellBroadcastReportInfo->dcs);
        }
    }
    return result;
}

SendSmsResultInfo HRilSms::MakeSendSmsResult(
    HRilRadioResponseInfo &responseInfo, int32_t serial, const void *response, const size_t responseLen)
{
    SendSmsResultInfo result;
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
} // namespace Telephony
} // namespace OHOS