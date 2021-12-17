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
HRilSms::HRilSms()
{
    AddHandlerToMap();
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
    const struct CdmaSmsMessageInfo *message = nullptr;
    HRilCdmaSmsMessageInfo msg = {};
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    const size_t readSpSize = sizeof(struct CdmaSmsMessageInfo);
    const uint8_t *spBuffer = parcel->ReadUnpadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("read spBuffer failed");
        return;
    }
    message = reinterpret_cast<const struct CdmaSmsMessageInfo *>(spBuffer);
    if (message == nullptr) {
        TELEPHONY_LOGE("ERROR : radioResponseInfo == nullptr !!!");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(message->serial, slotId, HREQ_SMS_SEND_CDMA_SMS);
    if (requestInfo == nullptr) {
        return;
    }
    MakeCdmaSmsInfo(msg, *message);
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    smsFuncs_->SendCdmaSms(requestInfo, &msg, sizeof(HRilCdmaSmsMessageInfo));
    free(requestInfo);
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
        return;
    }
    smsFuncs_->AddSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    free(requestInfo);
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
    free(requestInfo);
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
    ReqDataInfo *requestInfo = CreateHRilRequest(message.serial, slotId, HREQ_SMS_ADD_SIM_MESSAGE);
    if (requestInfo == nullptr) {
        return;
    }
    len = message.pdu.size() + 1;
    msg.state = message.state;
    if (len <= 0) {
        free(requestInfo);
        return;
    }
    msg.pdu = (char *)calloc(len, sizeof(char));
    if (msg.pdu == nullptr) {
        TELEPHONY_LOGE("calloc in UpdateSimMessage is failed!");
        free(requestInfo);
        return;
    }
    msg.index = message.index;
    (void)strcpy_s(msg.pdu, message.pdu.size() + 1, message.pdu.c_str());
    smsFuncs_->UpdateSimMessage(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    free(msg.pdu);
    free(requestInfo);
}

void HRilSms::GetSmscAddr(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SMS_GET_SMSC_ADDR);
    if (requestInfo == nullptr) {
        return;
    }
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    smsFuncs_->GetSmscAddr(requestInfo);
    free(requestInfo);
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
    ReqDataInfo *requestInfo = CreateHRilRequest(serCenterAddress.serial, slotId, HREQ_SMS_SET_SMSC_ADDR);
    if (requestInfo == nullptr) {
        return;
    }
    len = strlen(serCenterAddress.address.c_str()) + 1;
    address.tosca = serCenterAddress.tosca;
    if (len <= 0) {
        free(requestInfo);
        return;
    }
    address.address = (char *)calloc(len, sizeof(char));
    if (address.address == nullptr) {
        TELEPHONY_LOGE("calloc in SetSmscAddr is failed!");
        free(requestInfo);
        return;
    }
    int32_t ret =
        (int32_t)strcpy_s(address.address, serCenterAddress.address.size() + 1, serCenterAddress.address.c_str());
    if (ret != 0) {
        TELEPHONY_LOGE("RilAdapter Failed to copy string error!");
    }
    smsFuncs_->SetSmscAddr(requestInfo, &address, sizeof(HRilServiceCenterAddress));
    free(address.address);
    free(requestInfo);
}

int32_t HRilSms::SetSmscAddrResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
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
    free(requestInfo);
}

int32_t HRilSms::SetCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
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
    free(requestInfo);
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
    free(requestInfo);
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
    ReqDataInfo *requestInfo = CreateHRilRequest(cellBroadcastInfoList.serial, slotId, HREQ_SMS_SET_CDMA_CB_CONFIG);
    if (requestInfo == nullptr) {
        return;
    }
    HRilCdmaCBConfigInfo list[cellBroadcastInfoList.size];
    for (int32_t i = 0; i < cellBroadcastInfoList.size; i++) {
        list[i].service = cellBroadcastInfoList.list[i].service;
        list[i].language = cellBroadcastInfoList.list[i].language;
        list[i].checked = cellBroadcastInfoList.list[i].checked;
    }

    if (smsFuncs_->SetCdmaCBConfig != nullptr) {
        smsFuncs_->SetCdmaCBConfig(requestInfo, list, cellBroadcastInfoList.size * sizeof(HRilCdmaCBConfigInfo));
    }
    free(requestInfo);
}

int32_t HRilSms::SetCdmaCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

bool HRilSms::RequestWithStrings(int32_t serial, int32_t slotId, int32_t request, int32_t count, ...)
{
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, request);
    if (requestInfo == nullptr || smsFuncs_ == nullptr) {
        return false;
    }
    char **pBuff = nullptr;
    if (count <= 0) {
        free(requestInfo);
        return false;
    }
    pBuff = (char **)calloc(count, sizeof(int32_t));
    if (pBuff == nullptr) {
        SendErrorResponse(requestInfo, HRIL_ERR_MEMORY_FULL);
        free(requestInfo);
        return false;
    }
    va_list list;
    va_start(list, count);
    int32_t i = 0;
    while (i < count) {
        const char *str = va_arg(list, const char *);
        if (!ConvertToString(&pBuff[i], str, requestInfo)) {
            TELEPHONY_LOGE("ConvertToString in RequestWithStrings is failed!");
            va_end(list);
            for (int32_t j = 0; j < i; j++) {
                FreeStrings(1, pBuff[j]);
            }
            free(pBuff);
            free(requestInfo);
            return false;
        }
        i++;
    }
    va_end(list);
    smsFuncs_->SendGsmSms(requestInfo, pBuff, 0);
    if (pBuff != nullptr) {
        i = 0;
        while (i < count) {
            FreeStrings(1, pBuff[i]);
            i++;
        }
        free(pBuff);
    }
    free(requestInfo);
    return true;
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

int32_t HRilSms::SendSmsMoreModeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    SendSmsResultInfo result = MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);

    return ResponseMessageParcel(responseInfo, result, requestNum);
}

bool HRilSms::RequestWithInts(int **p, ReqDataInfo *requestInfo, int32_t argCount, ...)
{
    size_t len = sizeof(int32_t);
    if (len <= 0 || argCount <= 0) {
        return false;
    }
    *p = static_cast<int32_t *>(calloc(argCount, len));
    if (*p == nullptr) {
        SendErrorResponse(requestInfo, HRIL_ERR_MEMORY_FULL);
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

void HRilSms::SendSmsAck(int32_t slotId, struct HdfSBuf *data)
{
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
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        if (pBuff != nullptr) {
            free(pBuff);
        }
        free(requestInfo);
        return;
    }
    smsFuncs_->SendSmsAck(requestInfo, pBuff, sizeof(int32_t));
    if (pBuff != nullptr) {
        free(pBuff);
    }
    free(requestInfo);
}

bool HRilSms::IsSmsResponse(uint32_t code)
{
    return ((code >= HREQ_SMS_BASE) && (code < HREQ_SIM_BASE));
}

bool HRilSms::IsSmsNotification(uint32_t code)
{
    return ((code >= HNOTI_SMS_BASE) && (code < HNOTI_SIM_BASE));
}

bool HRilSms::IsSmsRespOrNotify(uint32_t code)
{
    return IsSmsResponse(code) || IsSmsNotification(code);
}

int32_t HRilSms::SendSmsAckResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf in SendSmsAckResponse is nullptr!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(HRilRadioResponseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in SendSmsAckResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in SendSmsAckResponse is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

int32_t HRilSms::DataSbuf(HdfSBuf *dataSbuf, int32_t indType)
{
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in DataSbuf is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_NEW_SMS, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in DataSbuf is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

int32_t HRilSms::SmsStatusReportNotify(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    const int32_t MESSAGE_SIZE = responseLen / 2;
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("invalid response");
        return HDF_SUCCESS;
    }
    uint8_t *bytes = ConvertHexStringToBytes(response, responseLen);
    if (bytes == nullptr) {
        TELEPHONY_LOGE("ConvertHexStringToBytes in SmsStatusReportNotify is failed!");
        return HDF_FAILURE;
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
        return HDF_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    smsMessageInfo.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("ParcelToSbuf in SmsStatusReportNotify is failed!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in SmsStatusReportNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_STATUS_REPORT, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in SmsStatusReportNotify is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

int32_t HRilSms::NewSmsStoredOnSimNotify(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(int32_t)) {
        TELEPHONY_LOGE("invalid response");
        return HDF_SUCCESS;
    }
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
    int32_t recordNumber = *(static_cast<const int32_t *>(response));
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("HdfSBufTypedObtain in NewSmsStoredOnSimNotify is failed!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, recordNumber)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NewSmsStoredOnSimNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in NewSmsStoredOnSimNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_NEW_SMS_STORED_ON_SIM, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in NewSmsStoredOnSimNotify is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        TELEPHONY_LOGE("dataSbuf in NewSmsStoredOnSimNotify is nullptr!");
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

int32_t HRilSms::NewSmsNotify(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    const int32_t NEW_SMS_SIZE = responseLen / 2;

    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("NewSmsNotify: invalid response");
        return HDF_FAILURE;
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));
    uint8_t *bytes = ConvertHexStringToBytes(response, responseLen);
    if (bytes == nullptr) {
        TELEPHONY_LOGE("NewSmsNotify: ConvertHexStringToBytes failed");
        return HDF_FAILURE;
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
        return HDF_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    smsMessageInfo.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (DataSbuf(dataSbuf, indType) == HDF_FAILURE) {
        TELEPHONY_LOGE("DataSbuf in NewSmsNotify is failed!");
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

int32_t HRilSms::NewCdmaSmsNotify(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    CdmaSmsMessageInfo smsMessageInfo = {};
    if (response == nullptr || responseLen == 0) {
        TELEPHONY_LOGE("invalid response");
        return HDF_FAILURE;
    } else {
        const HRilCdmaSmsMessageInfo *message = static_cast<const HRilCdmaSmsMessageInfo *>(response);
        MakeCdmaSmsInfo(smsMessageInfo, *message);
    }
    indType = static_cast<int32_t>(ConvertIntToRadioNoticeType(indType));

    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        return HDF_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    parcel->WriteUnpadBuffer(&smsMessageInfo, sizeof(CdmaSmsMessageInfo));
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SMS_NEW_CDMA_SMS, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

int32_t HRilSms::CBConfigNotify(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    CBConfigReportInfo result = MakeCBConfigResult(response, responseLen);
    result.indicationType = indType;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in CBConfigNotify is nullptr!");
        return HDF_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    result.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("ParcelToSbuf in CBConfigNotify is failed!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in CBConfigNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_CB_CONFIG_REPORT, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in CBConfigNotify is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
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

void HRilSms::ProcessSmsNotify(int32_t slotId, int32_t notifyType, const struct ReportInfo *reportInfo,
    const void *response, size_t responseLen)
{
    int32_t code;
    HRilErrno e;

    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("Invalid reportInfo: reportInfo is nullptr");
        return;
    }
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

void HRilSms::RegisterSmsFuncs(const HRilSmsReq *smsFuncs)
{
    smsFuncs_ = smsFuncs;
}
} // namespace Telephony
} // namespace OHOS