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

HRilSms::HRilSms(IHRilReporter &hrilReporter)
    : HRilBase(hrilReporter)
{
    AddHandlerToMap();
}

void HRilSms::ProcessSmsRequest(int32_t slotId, int32_t code, struct HdfSBuf *data)
{
    auto itFunc = reqMemberFuncMap_.find(code);
    if (itFunc != reqMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, data);
        }
    }
}

void HRilSms::ProcessSmsResponse(
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

void HRilSms::ProcessSmsNotify(int32_t slotId, const struct ReportInfo *reportInfo,
    const void *response, size_t responseLen)
{
    int32_t code;
    HRilErrNumber e;

    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("Invalid reportInfo: reportInfo is nullptr");
        return;
    }
    code = reportInfo->notifyId;
    e = (HRilErrNumber)reportInfo->error;
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, (int32_t)reportInfo->type, e, response, responseLen);
        }
    }
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

void HRilSms::SendGsmSms(int32_t slotId, struct HdfSBuf *data)
{
    struct GsmSmsMessageInfo message;
    MessageParcel *parcel = nullptr;
    const int32_t COUNT_STRINGS_VALUE = 2;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel int SendGsmSms is nullptr!");
        return;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    RequestWithStrings(message.serial, slotId, HREQ_SMS_SEND_GSM_SMS, COUNT_STRINGS_VALUE, message.smscPdu.c_str(),
        message.pdu.c_str());
}

void HRilSms::SendCdmaSms(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        return;
    }
    struct SendCdmaSmsMessageInfo message;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel int SendCdmaSms is nullptr!");
        return;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(message.serial, slotId, HREQ_SMS_SEND_CDMA_SMS);
    if (requestInfo == nullptr) {
        return;
    }
    smsFuncs_->SendCdmaSms(requestInfo, message.smscPdu.c_str(), message.smscPdu.size());
}

void HRilSms::AddSimMessage(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        return;
    }
    struct GsmSmsMessageInfo message = {};
    HRilSmsWriteSms msg = {};
    int32_t pduLen = 0;
    int32_t smscPduLen = 0;
    const int POINTER_NUM = 2;
    const int MSG_INDEX = -1;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel parcel=%{public}p", parcel);
        return;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    msg.state = message.state;
    msg.index = MSG_INDEX;
    pduLen = message.pdu.length();
    int32_t len = pduLen + 1;
    if (len <= 0) {
        return;
    }
    msg.pdu = (char *)calloc(len, sizeof(char));
    if (msg.pdu == nullptr) {
        return;
    }
    (void)(int32_t) strcpy_s(msg.pdu, pduLen + 1, message.pdu.c_str());
    smscPduLen = message.smscPdu.length() + 1;
    if (smscPduLen <= 0) {
        return;
    }
    msg.smsc = (char *)calloc(smscPduLen, sizeof(char));
    if (msg.smsc == nullptr) {
        free(msg.pdu);
        return;
    }
    (void)strcpy_s(msg.smsc, smscPduLen, message.smscPdu.c_str());
    ReqDataInfo *requestInfo = CreateHRilRequest(message.serial, slotId, HREQ_SMS_ADD_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        FreeStrings(POINTER_NUM, msg.pdu, msg.smsc);
        return;
    }
    smsFuncs_->AddSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    FreeStrings(POINTER_NUM, msg.pdu, msg.smsc);
}

void HRilSms::DelSimMessage(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        return;
    }
    int32_t *pBuff = nullptr;
    int32_t index = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("DelSimMessage >>> miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("DelSimMessage >> miss index parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SMS_DEL_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return;
    }
    RequestWithInts(&pBuff, requestInfo, 1, index);
    smsFuncs_->DelSimMessage(requestInfo, pBuff, 0);
    if (pBuff != nullptr) {
        free(pBuff);
    }
}

void HRilSms::UpdateSimMessage(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("updateSms failed!");
        return;
    }
    struct SmsMessageIOInfo message = {};
    HRilSmsWriteSms msg = {};
    int32_t len = 0;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in UpdateSimMessage is nullptr!");
        return;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    len = message.pdu.size() + 1;
    msg.state = message.state;
    if (len <= 0) {
        return;
    }
    msg.pdu = (char *)calloc(len, sizeof(char));
    if (msg.pdu == nullptr) {
        TELEPHONY_LOGE("malloc in UpdateSimMessage is failed!");
        return;
    }
    msg.index = message.index;
    (void)strcpy_s(msg.pdu, message.pdu.size() + 1, message.pdu.c_str());
    ReqDataInfo *requestInfo = CreateHRilRequest(message.serial, slotId, HREQ_SMS_ADD_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        free(msg.pdu);
        return;
    }
    smsFuncs_->UpdateSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    free(msg.pdu);
}

void HRilSms::SetSmscAddr(int32_t slotId, struct HdfSBuf *data)
{
    struct ServiceCenterAddress serCenterAddress;
    HRilServiceCenterAddress address;
    int32_t len = 0;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SetSmscAddr is nullptr!");
        return;
    }
    if (!serCenterAddress.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    len = strlen(serCenterAddress.address.c_str()) + 1;
    address.tosca = serCenterAddress.tosca;
    if (len <= 0) {
        return;
    }
    address.address = (char *)calloc(len, sizeof(char));
    if (address.address == nullptr) {
        TELEPHONY_LOGE("malloc in SetSmscAddr is failed!");
        return;
    }
    int32_t ret =
        (int32_t)strcpy_s(address.address, serCenterAddress.address.size() + 1, serCenterAddress.address.c_str());
    if (ret != 0) {
        TELEPHONY_LOGE("RilAdapter Failed to copy string error!");
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serCenterAddress.serial, slotId, HREQ_SMS_SET_SMSC_ADDR);
    if (requestInfo == nullptr) {
        free(address.address);
        return;
    }
    smsFuncs_->SetSmscAddr(requestInfo, &address, sizeof(HRilServiceCenterAddress));
    free(address.address);
}

void HRilSms::GetSmscAddr(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SMS_GET_SMSC_ADDR);
    if (requestInfo == nullptr) {
        return;
    }
    smsFuncs_->GetSmscAddr(requestInfo);
}

void HRilSms::SetCBConfig(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        return;
    }
    struct CBConfigInfo broadcastInfo;
    HRilCBConfigInfo cellBroadcastInfo;
    int32_t midsLen = 0;
    int32_t dcssLen = 0;
    const int32_t POINTER_NUM = 2;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel parcel=%{public}p", parcel);
        return;
    }
    if (!broadcastInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    cellBroadcastInfo.mode = broadcastInfo.mode;
    midsLen = broadcastInfo.mids.size() + 1;
    if (midsLen <= 0) {
        return;
    }
    cellBroadcastInfo.mids = (char *)calloc(midsLen, sizeof(char));
    if (cellBroadcastInfo.mids == nullptr) {
        return;
    }
    (void)strcpy_s(cellBroadcastInfo.mids, broadcastInfo.mids.size() + 1, broadcastInfo.mids.c_str());
    dcssLen = broadcastInfo.dcss.size() + 1;
    if (dcssLen <= 0) {
        free(cellBroadcastInfo.mids);
        return;
    }
    cellBroadcastInfo.dcss = (char *)calloc(dcssLen, sizeof(char));
    if (cellBroadcastInfo.dcss == nullptr) {
        free(cellBroadcastInfo.mids);
        return;
    }
    (void)strcpy_s(cellBroadcastInfo.dcss, broadcastInfo.dcss.size() + 1, broadcastInfo.dcss.c_str());
    ReqDataInfo *requestInfo = CreateHRilRequest(broadcastInfo.serial, slotId, HREQ_SMS_SET_CB_CONFIG);
    if (requestInfo == nullptr) {
        FreeStrings(POINTER_NUM, cellBroadcastInfo.mids, cellBroadcastInfo.dcss);
        return;
    }
    smsFuncs_->SetCBConfig(requestInfo, &cellBroadcastInfo, sizeof(HRilServiceCenterAddress));
    FreeStrings(POINTER_NUM, cellBroadcastInfo.mids, cellBroadcastInfo.dcss);
}

void HRilSms::GetCBConfig(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SMS_GET_CB_CONFIG);
    if (requestInfo == nullptr) {
        return;
    }
    smsFuncs_->GetCBConfig(requestInfo);
}

void HRilSms::SetCdmaCBConfig(int32_t slotId, struct HdfSBuf *data)
{
    CdmaCBConfigInfoList cellBroadcastInfoList = {};
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SetCdmaCBConfig is nullptr!");
        return;
    }
    if (!cellBroadcastInfoList.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    HRilCdmaCBConfigInfo list[cellBroadcastInfoList.size];
    for (int32_t i = 0; i < cellBroadcastInfoList.size; i++) {
        list[i].service = cellBroadcastInfoList.list[i].service;
        list[i].language = cellBroadcastInfoList.list[i].language;
        list[i].checked = cellBroadcastInfoList.list[i].checked;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(
        cellBroadcastInfoList.serial, slotId, HREQ_SMS_SET_CDMA_CB_CONFIG);
    if (requestInfo == nullptr) {
        return;
    }
    if (smsFuncs_->SetCdmaCBConfig != nullptr) {
        smsFuncs_->SetCdmaCBConfig(requestInfo, list, cellBroadcastInfoList.size * sizeof(HRilCdmaCBConfigInfo));
    }
}

void HRilSms::GetCdmaCBConfig(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SMS_GET_CDMA_CB_CONFIG);
    if (requestInfo == nullptr) {
        return;
    }
    if (smsFuncs_->GetCdmaCBConfig != nullptr) {
        smsFuncs_->GetCdmaCBConfig(requestInfo);
    }
}

void HRilSms::SendSmsMoreMode(int32_t slotId, struct HdfSBuf *data)
{
    struct GsmSmsMessageInfo message;
    MessageParcel *parcel = nullptr;
    const int32_t COUNT_STRINGS_VALUE = 2;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SendSmsMoreMode is nullptr!");
        return;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    RequestWithStrings(message.serial, slotId, HREQ_SMS_SEND_SMS_MORE_MODE, COUNT_STRINGS_VALUE,
        message.smscPdu.c_str(), message.pdu.c_str());
}

void HRilSms::SendSmsAck(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        return;
    }
    int32_t *pBuff = nullptr;
    struct ModeData mode;
    MessageParcel *parcel = nullptr;
    const int32_t COUNT_INTS_VALUE = 2;

    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to SbufToParcel!");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SendSmsAck is nullptr!");
        return;
    }
    if (!mode.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(mode.serial, slotId, HREQ_SMS_SEND_SMS_ACK);
    if (requestInfo == nullptr) {
        return;
    }
    RequestWithInts(&pBuff, requestInfo, COUNT_INTS_VALUE, static_cast<int32_t>(mode.result), mode.mode);
    smsFuncs_->SendSmsAck(requestInfo, pBuff, sizeof(int32_t));
    if (pBuff != nullptr) {
        free(pBuff);
    }
}

void HRilSms::AddCdmaSimMessage(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        return;
    }
    struct SmsMessageIOInfo mSmsMessageIOInfo = {};
    HRilSmsWriteSms msg = {};
    int32_t pduLen = 0;
    const int MSG_INDEX = -1;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel parcel=%{public}p", parcel);
        return;
    }
    if (!mSmsMessageIOInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    msg.state = mSmsMessageIOInfo.state;
    msg.index = MSG_INDEX;
    pduLen = mSmsMessageIOInfo.pdu.length();
    int32_t len = pduLen + 1;
    if (len <= 0) {
        return;
    }
    msg.pdu = (char *)calloc(len, sizeof(char));
    if (msg.pdu == nullptr) {
        return;
    }
    (void)(int32_t) strcpy_s(msg.pdu, pduLen + 1, mSmsMessageIOInfo.pdu.c_str());
    ReqDataInfo *requestInfo =
        CreateHRilRequest(mSmsMessageIOInfo.serial, slotId, HREQ_SMS_ADD_CDMA_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return;
    }
    smsFuncs_->AddCdmaSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    FreeStrings(1, msg.pdu);
}

void HRilSms::DelCdmaSimMessage(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        return;
    }
    int32_t *pBuff = nullptr;
    int32_t index = 0;
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("HdfSbufReadInt32 >>> miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("HdfSbufReadInt32 >> miss index parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SMS_DEL_CDMA_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return;
    }
    RequestWithInts(&pBuff, requestInfo, 1, index);
    smsFuncs_->DelCdmaSimMessage(requestInfo, pBuff, 0);
    if (pBuff != nullptr) {
        free(pBuff);
    }
}

void HRilSms::UpdateCdmaSimMessage(int32_t slotId, struct HdfSBuf *data)
{
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ failed!");
        return;
    }
    struct SmsMessageIOInfo mSmsMessageIOInfo = {};
    HRilSmsWriteSms msg = {};
    int32_t len = 0;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in UpdateCdmaSimMessage is nullptr!");
        return;
    }
    if (!mSmsMessageIOInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(mSmsMessageIOInfo.serial, slotId,
        HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return;
    }
    len = mSmsMessageIOInfo.pdu.size() + 1;
    msg.state = mSmsMessageIOInfo.state;
    if (len <= 0) {
        free(requestInfo);
        return;
    }
    msg.pdu = (char *)calloc(len, sizeof(char));
    if (msg.pdu == nullptr) {
        TELEPHONY_LOGE("malloc in UpdateCdmaSimMessage is failed!");
        free(requestInfo);
        return;
    }
    msg.index = mSmsMessageIOInfo.index;
    (void)strcpy_s(msg.pdu, mSmsMessageIOInfo.pdu.size() + 1, mSmsMessageIOInfo.pdu.c_str());
    smsFuncs_->UpdateCdmaSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    free(msg.pdu);
}


int32_t HRilSms::SendGsmSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    SendSmsResultInfo result = MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSms::SendCdmaSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    SendSmsResultInfo result = MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSms::AddSimMessageResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::DelSimMessageResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::UpdateSimMessageResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::SetSmscAddrResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::GetSmscAddrResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
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

int32_t HRilSms::SetCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::GetCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
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

int32_t HRilSms::SetCdmaCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::GetCdmaCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
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

int32_t HRilSms::SendSmsMoreModeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    SendSmsResultInfo result = MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);

    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSms::SendSmsAckResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf in SendSmsAckResponse is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(HRilRadioResponseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in SendSmsAckResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in SendSmsAckResponse is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::AddCdmaSimMessageResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::DelCdmaSimMessageResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::UpdateCdmaSimMessageResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::SmsStatusReportNotify(
    int32_t slotId, int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    const int32_t MESSAGE_SIZE = responseLen / HEX_WIDTH;
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("invalid response");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    const HRilSmsResponse *smsResponse = static_cast<const HRilSmsResponse *>(response);
    uint8_t *bytes = ConvertHexStringToBytes(smsResponse->pdu, responseLen);
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
        TELEPHONY_LOGE("parcel in  SmsStatusReportNotify is nullptr!");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    smsMessageInfo.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("ParcelToSbuf in SmsStatusReportNotify is failed!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in SmsStatusReportNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_STATUS_REPORT, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in SmsStatusReportNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::NewSmsStoredOnSimNotify(
    int32_t slotId, int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(int32_t)) {
        TELEPHONY_LOGE("invalid response");
        return HRIL_ERR_SUCCESS;
    }
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
    int32_t recordNumber = *(static_cast<const int32_t*>(response));
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("HdfSBufTypedObtain in NewSmsStoredOnSimNotify is failed!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, recordNumber)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NewSmsStoredOnSimNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NewSmsStoredOnSimNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_NEW_SMS_STORED_ON_SIM, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in NewSmsStoredOnSimNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        TELEPHONY_LOGE("dataSbuf in NewSmsStoredOnSimNotify is nullptr!");
        HdfSBufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::NewSmsNotify(
    int32_t slotId, int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    HRilSmsResponse *smsResponse = nullptr;
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("NewSmsNotify: invalid response");
        return HDF_FAILURE;
    } else {
        smsResponse = (HRilSmsResponse *)response;
    }
    const int32_t NEW_SMS_SIZE = strlen(smsResponse->pdu) / HEX_WIDTH;
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    uint8_t *bytes = ConvertHexStringToBytes(smsResponse->pdu, strlen(smsResponse->pdu));
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
    struct HdfSBuf *dataSbuf = nullptr;
    smsMessageInfo.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (DataSbuf(dataSbuf, indType) == HRIL_ERR_GENERIC_FAILURE) {
        TELEPHONY_LOGE("DataSbuf in NewSmsNotify is failed!");
        return HRIL_ERR_GENERIC_FAILURE;
    }

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::NewCdmaSmsNotify(
    int32_t slotId, int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
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
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    messageInfo.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_NEW_CDMA_SMS, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSms::CBConfigNotify(
    int32_t slotId, int32_t indType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    CBConfigReportInfo result = MakeCBConfigResult(response, responseLen);
    result.indicationType = indType;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in CBConfigNotify is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    result.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("ParcelToSbuf in CBConfigNotify is failed!");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in CBConfigNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_CB_CONFIG_REPORT, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in CBConfigNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
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
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_NEW_SMS, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in DataSbuf is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}

bool HRilSms::RequestWithInts(int **p, ReqDataInfo *requestInfo, int32_t argCount, ...)
{
    size_t len = sizeof(int32_t);
    if (len <= 0 || argCount <= 0) {
        return false;
    }
    *p = static_cast<int32_t *>(calloc(argCount, len));
    if (*p == nullptr) {
        TELEPHONY_LOGE("req: [%{public}d,%{public}d,%{public}d], alloc fail", requestInfo->serial,
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

bool HRilSms::RequestWithStrings(int32_t serial, int32_t slotId, int32_t request, int32_t count, ...)
{
    if (smsFuncs_ == nullptr) {
        return false;
    }
    char **pBuff = nullptr;
    if (count <= 0) {
        return false;
    }
    pBuff = (char **)calloc(count, sizeof(int32_t));
    if (pBuff == nullptr) {
        TELEPHONY_LOGE("req: [%{public}d,%{public}d,%{public}d], malloc fail", serial, slotId, request);
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
                FreeStrings(1, pBuff[j]);
            }
            free(pBuff);
            return false;
        }
        i++;
    }
    va_end(list);
    bool ret = false;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, request);
    if (requestInfo != nullptr) {
        smsFuncs_->SendGsmSms(requestInfo, pBuff, 0);
        ret = true;
    }
    if (pBuff != nullptr) {
        i = 0;
        while (i < count) {
            FreeStrings(1, pBuff[i]);
            i++;
        }
        free(pBuff);
    }
    return ret;
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

void HRilSms::MakeCdmaSmsInfo(HRilCdmaSmsMessageInfo &msg, const CdmaSmsMessageInfo &message)
{
    msg.size = message.size;
    (void)memcpy_s(msg.bytes, HRIL_MAX_CDMA_MESSAGE_LEN, message.bytes, HRIL_MAX_CDMA_MESSAGE_LEN);
    msg.isExist = message.isExist;
    msg.serviceId = message.serviceId;
    msg.type = message.type;

    msg.address.digitMode = message.address.digitMode;
    msg.address.mode = message.address.mode;
    msg.address.type = message.address.type;
    msg.address.plan = message.address.plan;
    msg.address.number = message.address.number;
    (void)memcpy_s(msg.bytes, HRIL_MAX_CDMA_ADDRESS_LEN, message.address.bytes, HRIL_MAX_CDMA_ADDRESS_LEN);

    msg.subAddress.type = message.subAddress.type;
    msg.subAddress.odd = message.subAddress.odd;
    msg.subAddress.number = message.subAddress.number;
    (void)memcpy_s(msg.bytes, HRIL_MAX_CDMA_ADDRESS_LEN, message.subAddress.bytes, HRIL_MAX_CDMA_ADDRESS_LEN);
}

void HRilSms::MakeCdmaSmsInfo(CdmaSmsMessageInfo &msg, const HRilCdmaSmsMessageInfo &message)
{
    msg.size = message.size;
    (void)memcpy_s(msg.bytes, HRIL_MAX_CDMA_MESSAGE_LEN, message.bytes, HRIL_MAX_CDMA_MESSAGE_LEN);
    msg.isExist = message.isExist;
    msg.serviceId = message.serviceId;
    msg.type = message.type;

    msg.address.digitMode = message.address.digitMode;
    msg.address.mode = message.address.mode;
    msg.address.type = message.address.type;
    msg.address.plan = message.address.plan;
    msg.address.number = message.address.number;
    (void)memcpy_s(msg.bytes, HRIL_MAX_CDMA_ADDRESS_LEN, message.address.bytes, HRIL_MAX_CDMA_ADDRESS_LEN);

    msg.subAddress.type = message.subAddress.type;
    msg.subAddress.odd = message.subAddress.odd;
    msg.subAddress.number = message.subAddress.number;
    (void)memcpy_s(msg.bytes, HRIL_MAX_CDMA_ADDRESS_LEN, message.subAddress.bytes, HRIL_MAX_CDMA_ADDRESS_LEN);
}
} // namespace Telephony
} // namespace OHOS