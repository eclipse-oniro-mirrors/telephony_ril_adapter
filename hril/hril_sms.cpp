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

namespace OHOS {
namespace Telephony {
HRilSms::HRilSms()
{
    AddHandlerToMap();
}

SendSmsResultInfo HRilSms::MakeSendSmsResult(
    HRilRadioResponseInfo &responseInfo, int32_t serial, const void *response, const size_t responseLen)
{
    SendSmsResultInfo result = {};
    if (response == nullptr || responseLen != sizeof(HRilSmsResponse)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
        result.pdu = std::string("");
    } else {
        HRilSmsResponse *smsResponse = (HRilSmsResponse *)response;
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

CellBroadcastReportInfo HRilSms::MakeCellBroadcastResult(const void *response, const size_t responseLen)
{
    CellBroadcastReportInfo result = {};
    if (response == nullptr || responseLen != sizeof(HRilCellBroadcastReportInfo *)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        result.data = std::string("");
        result.pdu = std::string("");
        result.dcs = std::string("");
    } else {
        HRilCellBroadcastReportInfo *cellBroadcastReportInfo = (HRilCellBroadcastReportInfo *)response;
        result.mid = cellBroadcastReportInfo->mid;
        result.length = cellBroadcastReportInfo->length;
        result.page = cellBroadcastReportInfo->page;
        result.pages = cellBroadcastReportInfo->pages;
        result.sn = cellBroadcastReportInfo->sn;
        if (cellBroadcastReportInfo->data == nullptr) {
            result.data = std::string("");
            TELEPHONY_LOGE("result.data is nullptr");
        } else {
            result.data = std::string(cellBroadcastReportInfo->data);
        }
        if (cellBroadcastReportInfo->pdu == nullptr) {
            result.pdu = std::string("");
            TELEPHONY_LOGE("result.pdu is nullptr");
        } else {
            result.pdu = std::string(cellBroadcastReportInfo->pdu);
        }
        if (cellBroadcastReportInfo->dcs == nullptr) {
            result.dcs = std::string("");
            TELEPHONY_LOGE("result.dcs is nullptr");
        } else {
            result.dcs = std::string(cellBroadcastReportInfo->dcs);
        }
    }
    return result;
}

void HRilSms::SendSms(int32_t slotId, struct HdfSBuf *data)
{
    struct GsmSmsMessageInfo message = {};
    MessageParcel *parcel = nullptr;
    const int32_t COUNT_STRINGS_VALUE = 2;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel int32_t SendSms is nullptr!");
        return;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    RequestWithStrings(message.serial, slotId, HREQ_SMS_SEND_SMS, COUNT_STRINGS_VALUE, message.smscPdu.c_str(),
        message.pdu.c_str());
}

void HRilSms::StorageSms(int32_t slotId, struct HdfSBuf *data)
{
    struct GsmSmsMessageInfo message = {};
    HRilSmsWriteSms msg = {};
    size_t pduLen = 0;
    size_t smscPduLen = 0;
    const size_t LEN = 1;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel parcel=%{public}p", parcel);
        return;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(message.serial, slotId, HREQ_SMS_STORAGE_SMS);
    if (requestInfo == nullptr) {
        return;
    }
    msg.state = message.state;
    pduLen = message.pdu.length();
    if (pduLen + LEN == 0 || MALLOC_MAX_SIZE < pduLen + LEN) {
        free(requestInfo);
        return;
    }
    msg.pdu = (char *)calloc(pduLen + LEN, sizeof(char));
    if (msg.pdu == nullptr) {
        TELEPHONY_LOGE("calloc in StorageSms is failed!");
        free(requestInfo);
        return;
    }
    int32_t ret = (int)strcpy_s(msg.pdu, pduLen + LEN, message.pdu.c_str());
    if (ret != 0) {
        TELEPHONY_LOGE("RilAdapter Failed to copy string error!");
    }
    smscPduLen = message.smscPdu.length();
    if (smscPduLen + LEN == 0 || MALLOC_MAX_SIZE < smscPduLen + LEN) {
        free(requestInfo);
        free(msg.pdu);
        return;
    }
    msg.smsc = (char *)calloc(smscPduLen + LEN, sizeof(char));
    if (msg.smsc == nullptr) {
        TELEPHONY_LOGE("calloc in StorageSms is failed!");
        free(requestInfo);
        free(msg.pdu);
        return;
    }
    int32_t tmp = (int)strcpy_s(msg.smsc, smscPduLen + LEN, message.smscPdu.c_str());
    if (tmp != 0 || smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        free(requestInfo);
        free(msg.pdu);
        free(msg.smsc);
        return;
    }
    smsFuncs_->StorageSms(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    free(requestInfo);
    free(msg.pdu);
    free(msg.smsc);
}

void HRilSms::DeleteSms(int32_t slotId, struct HdfSBuf *data)
{
    int32_t *pBuff = nullptr;
    int32_t index = 0;
    int32_t serial = 0;
    const int32_t ARG_COUNT = 1;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("DeleteSms >>> miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("DeleteSms >> miss index parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SMS_DELETE_SMS);
    if (requestInfo == nullptr) {
        return;
    }
    RequestWithInts(&pBuff, requestInfo, ARG_COUNT, index);
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        if (pBuff != nullptr) {
            free(pBuff);
        }
        free(requestInfo);
        return;
    }
    smsFuncs_->DeleteSms(requestInfo, pBuff, sizeof(char *));
    if (pBuff != nullptr) {
        free(pBuff);
    }
    free(requestInfo);
}

void HRilSms::UpdateSms(int32_t slotId, struct HdfSBuf *data)
{
    struct SmsMessageIOInfo message = {};
    HRilSmsWriteSms msg = {};
    uint32_t len = 0;
    const uint32_t LEN = 1;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in UpdateSms is nullptr!");
        return;
    }
    if (!message.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(message.serial, slotId, HREQ_SMS_STORAGE_SMS);
    if (requestInfo == nullptr) {
        return;
    }
    len = message.pdu.size();
    msg.state = message.state;
    if (len + LEN == 0 || len + LEN > MALLOC_MAX_SIZE) {
        free(requestInfo);
        return;
    }
    msg.pdu = (char *)calloc(len + LEN, sizeof(char));
    if (msg.pdu == nullptr) {
        TELEPHONY_LOGE("calloc in UpdateSms is failed!");
        free(requestInfo);
        return;
    }
    msg.index = message.index;
    int32_t ret = (int)strcpy_s(msg.pdu, message.pdu.size() + LEN, message.pdu.c_str());
    if (ret != 0 || smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs failed!");
        free(requestInfo);
        free(msg.pdu);
        return;
    }
    smsFuncs_->UpdateSms(requestInfo, &msg, sizeof(HRilSmsWriteSms));
    free(msg.pdu);
    free(requestInfo);
}

void HRilSms::GetSmsCenterAddress(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetSmsCenterAddress >>> miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SMS_GET_CENTER_ADDRESS);
    if (requestInfo == nullptr) {
        return;
    }
    if (smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        free(requestInfo);
        return;
    }
    smsFuncs_->GetSmsCenterAddress(requestInfo, nullptr, 0);
    free(requestInfo);
}

int32_t HRilSms::GetSmsCenterAddressResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    ServiceCenterAddress result = {};

    if (response == nullptr || responseLen != sizeof(HRilServiceCenterAddress *)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
        result.address = std::string("");
    } else {
        HRilServiceCenterAddress *address = (HRilServiceCenterAddress *)response;
        result.tosca = address->tosca;
        if (address->address == nullptr) {
            result.address = std::string("");
            TELEPHONY_LOGE("address is nullptr");
        } else {
            result.address = std::string(address->address);
        }
    }
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

void HRilSms::SetSmsCenterAddress(int32_t slotId, struct HdfSBuf *data)
{
    struct ServiceCenterAddress serCenterAddress = {};
    HRilServiceCenterAddress address = {};
    uint32_t len = 0;
    const uint32_t LEN = 1;
    MessageParcel *parcel = nullptr;
    TELEPHONY_LOGD("SetSmsCenterAddress");
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SetSmsCenterAddress is nullptr!");
        return;
    }
    if (!serCenterAddress.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serCenterAddress.serial, slotId, HREQ_SMS_SET_CENTER_ADDRESS);
    if (requestInfo == nullptr) {
        return;
    }
    len = strlen(serCenterAddress.address.c_str());
    address.tosca = serCenterAddress.tosca;
    if (len + LEN == 0 || len + LEN > MALLOC_MAX_SIZE) {
        free(requestInfo);
        return;
    }
    address.address = (char *)calloc(len + LEN, sizeof(char));
    if (address.address == nullptr) {
        TELEPHONY_LOGE("calloc in SetSmsCenterAddress is failed!");
        free(requestInfo);
        return;
    }
    int32_t ret =
        (int)strcpy_s(address.address, serCenterAddress.address.size() + LEN, serCenterAddress.address.c_str());
    if (ret != 0) {
        TELEPHONY_LOGE("RilAdapter Failed to copy string error!");
        free(requestInfo);
        free(address.address);
        return;
    }
    smsFuncs_->SetSmsCenterAddress(requestInfo, &address, sizeof(HRilServiceCenterAddress));
    free(address.address);
    free(requestInfo);
}

int32_t HRilSms::SetSmsCenterAddressResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

void HRilSms::SetCellBroadcast(int32_t slotId, struct HdfSBuf *data)
{
    struct CellBroadcastInfo broadcastInfo = {};
    HRilCellBroadcastInfo cellBroadcastInfo = {};
    size_t midsLen = 0;
    size_t dcssLen = 0;
    const uint32_t LEN = 1;
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel parcel=%{public}p", parcel);
        return;
    }
    if (!broadcastInfo.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(broadcastInfo.serial, slotId, HREQ_SMS_SET_CELL_BROADCAST);
    if (requestInfo == nullptr) {
        return;
    }
    cellBroadcastInfo.mode = broadcastInfo.mode;
    midsLen = broadcastInfo.mids.size();
    if (midsLen + LEN == 0 || midsLen + LEN > MALLOC_MAX_SIZE) {
        free(requestInfo);
        return;
    }
    cellBroadcastInfo.mids = (char *)calloc(midsLen + LEN, sizeof(char));
    if (cellBroadcastInfo.mids == nullptr) {
        TELEPHONY_LOGE("calloc in SetCellBroadcast is failed!");
        free(requestInfo);
        return;
    }
    int32_t ret =
        (int)strcpy_s(cellBroadcastInfo.mids, broadcastInfo.mids.size() + LEN, broadcastInfo.mids.c_str());
    if (ret != 0) {
        TELEPHONY_LOGE("RilAdapter Failed to copy string error!");
        free(requestInfo);
        free(cellBroadcastInfo.mids);
        return;
    }
    dcssLen = broadcastInfo.dcss.size();
    if (dcssLen + LEN == 0 || dcssLen + LEN > MALLOC_MAX_SIZE) {
        free(requestInfo);
        return;
    }
    cellBroadcastInfo.dcss = (char *)calloc(dcssLen + LEN, sizeof(char));
    if (cellBroadcastInfo.dcss == nullptr) {
        TELEPHONY_LOGE("calloc in SetCellBroadcast is failed!");
        free(cellBroadcastInfo.mids);
        free(requestInfo);
        return;
    }
    int32_t tmp =
        (int)strcpy_s(cellBroadcastInfo.dcss, broadcastInfo.dcss.size() + LEN, broadcastInfo.dcss.c_str());
    if (tmp != 0 || smsFuncs_ == nullptr) {
        TELEPHONY_LOGE("smsFuncs_ is nullptr!");
        free(cellBroadcastInfo.mids);
        free(cellBroadcastInfo.dcss);
        free(requestInfo);
        return;
    }
    smsFuncs_->SetCellBroadcast(requestInfo, &cellBroadcastInfo, sizeof(HRilServiceCenterAddress));
    free(cellBroadcastInfo.mids);
    free(cellBroadcastInfo.dcss);
    free(requestInfo);
}

int32_t HRilSms::SetCellBroadcastResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

bool HRilSms::RequestWithStrings(int32_t serial, int32_t slotId, int32_t request, int32_t count, ...)
{
    if (smsFuncs_ == nullptr || count <= 0 || count > MALLOC_MAX_SIZE) {
        return false;
    }
    const int32_t ARG_COUNTS = 1;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, request);
    if (requestInfo == nullptr) {
        return false;
    }
    char **pBuff = nullptr;
    pBuff = (char **)calloc(count, sizeof(char *));
    if (pBuff == nullptr) {
        SendErrorResponse(requestInfo, HRIL_ERR_MEMORY_FULL);
        free(requestInfo);
        return false;
    }
    va_list list = {};
    va_start(list, count);
    int32_t i = 0;
    while (i < count) {
        const char *str = va_arg(list, const char *);
        TELEPHONY_LOGD("param：str %{public}s", str);
        if (!ConvertToString(&pBuff[i], str, requestInfo)) {
            TELEPHONY_LOGE("ConvertToString in RequestWithStrings is failed!");
            va_end(list);
            for (int32_t j = 0; j < i; j++) {
                FreeStrings(ARG_COUNTS, pBuff[j]);
            }
            free(pBuff);
            free(requestInfo);
            return false;
        }
        i++;
    }
    va_end(list);
    smsFuncs_->SendSms(requestInfo, pBuff, count * sizeof(char *));
    if (pBuff != nullptr) {
        i = 0;
        while (i < count) {
            FreeStrings(ARG_COUNTS, pBuff[i]);
            i++;
        }
        free(pBuff);
    }
    free(requestInfo);
    return true;
}

int32_t HRilSms::SendSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    SendSmsResultInfo result = MakeSendSmsResult(responseInfo, responseInfo.serial, response, responseLen);

    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSms::StorageSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::DeleteSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

int32_t HRilSms::UpdateSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

void HRilSms::SendSmsMoreMode(int32_t slotId, struct HdfSBuf *data)
{
    struct GsmSmsMessageInfo message = {};
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

bool HRilSms::RequestWithInts(int32_t **pBuff, ReqDataInfo *requestInfo, int32_t argCount, ...)
{
    size_t len = sizeof(int32_t);
    if (len <= 0 || argCount <= 0 || argCount > MALLOC_MAX_SIZE) {
        return false;
    }
    *pBuff = static_cast<int32_t *>(calloc(argCount, len));
    if (*pBuff == nullptr) {
        SendErrorResponse(requestInfo, HRIL_ERR_MEMORY_FULL);
        return false;
    }
    va_list list = {};
    va_start(list, argCount);
    int32_t i = 0;
    while (i < argCount) {
        (*pBuff)[i] = va_arg(list, int32_t);
        i++;
    }
    va_end(list);
    return true;
}

void HRilSms::SendSmsAck(int32_t slotId, struct HdfSBuf *data)
{
    int32_t *pBuff = nullptr;
    struct ModeData mode = {};
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
    if (serviceCallback_ == nullptr) {
        TELEPHONY_LOGE("serviceCallback_ is nullptr!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallback_->dispatcher->Dispatch(serviceCallback_, requestNum, dataSbuf, nullptr);
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

int32_t HRilSms::DataSbuf(HdfSBuf *dataSbuf, uint8_t *bytes, int32_t indType)
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
    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter serviceCallbackNotify_ is null");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret =
        serviceCallbackNotify_->dispatcher->Dispatch(serviceCallbackNotify_, HNOTI_SMS_NEW_SMS, dataSbuf, nullptr);
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
        TELEPHONY_LOGE("SmsStatusReportNotify: invalid response");
        return HDF_SUCCESS;
    }
    uint8_t *bytes = ConvertHexStringToBytes(response, responseLen);
    if (bytes == nullptr) {
        TELEPHONY_LOGE("ConvertHexStringToBytes in SmsStatusReportNotify is failed!");
        return HDF_FAILURE;
    }
    SmsMessageInfo smsMessageInfo = {};
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
    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter serviceCallbackNotify_ is null");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallbackNotify_->dispatcher->Dispatch(
        serviceCallbackNotify_, HNOTI_SMS_STATUS_REPORT, dataSbuf, nullptr);
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
        TELEPHONY_LOGE("NewSmsStoredOnSimNotify: invalid response");
        return HDF_SUCCESS;
    }
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
    int32_t recordNumber = ((int32_t *)response)[0];
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
    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter serviceCallbackNotify_ is null");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallbackNotify_->dispatcher->Dispatch(
        serviceCallbackNotify_, HNOTI_SMS_NEW_SMS_STORED_ON_SIM, dataSbuf, nullptr);
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
    SmsMessageInfo smsMessageInfo = {};
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
    if (DataSbuf(dataSbuf, bytes, indType) == HDF_FAILURE) {
        TELEPHONY_LOGE("DataSbuf in NewSmsNotify is failed!");
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

int32_t HRilSms::CellBroadcastNotify(
    int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responseLen)
{
    CellBroadcastReportInfo result = MakeCellBroadcastResult(response, responseLen);
    result.indicationType = indType;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in CellBroadcastNotify is nullptr!");
        return HDF_FAILURE;
    }
    struct HdfSBuf *dataSbuf = nullptr;
    result.Marshalling(*parcel.get());
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("ParcelToSbuf in CellBroadcastNotify is failed!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in CellBroadcastNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter serviceCallbackNotify_ is null");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallbackNotify_->dispatcher->Dispatch(
        serviceCallbackNotify_, HNOTI_CELL_BROADCAST_REPORT, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in CellBroadcastNotify is not equal to HDF_SUCCESS!");
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
    respMemberFuncMap_[HREQ_SMS_SEND_SMS] = &HRilSms::SendSmsResponse;
    respMemberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &HRilSms::SendSmsMoreModeResponse;
    respMemberFuncMap_[HREQ_SMS_SEND_SMS_ACK] = &HRilSms::SendSmsAckResponse;
    respMemberFuncMap_[HREQ_SMS_STORAGE_SMS] = &HRilSms::StorageSmsResponse;
    respMemberFuncMap_[HREQ_SMS_DELETE_SMS] = &HRilSms::DeleteSmsResponse;
    respMemberFuncMap_[HREQ_SMS_UPDATE_SMS] = &HRilSms::UpdateSmsResponse;
    respMemberFuncMap_[HREQ_SMS_SET_CENTER_ADDRESS] = &HRilSms::SetSmsCenterAddressResponse;
    respMemberFuncMap_[HREQ_SMS_GET_CENTER_ADDRESS] = &HRilSms::GetSmsCenterAddressResponse;
    respMemberFuncMap_[HREQ_SMS_SET_CELL_BROADCAST] = &HRilSms::SetCellBroadcastResponse;

    // Notification
    notiMemberFuncMap_[HNOTI_SMS_NEW_SMS] = &HRilSms::NewSmsNotify;
    notiMemberFuncMap_[HNOTI_SMS_STATUS_REPORT] = &HRilSms::SmsStatusReportNotify;
    notiMemberFuncMap_[HNOTI_SMS_NEW_SMS_STORED_ON_SIM] = &HRilSms::NewSmsStoredOnSimNotify;
    notiMemberFuncMap_[HNOTI_CELL_BROADCAST_REPORT] = &HRilSms::CellBroadcastNotify;

    // Request
    reqMemberFuncMap_[HREQ_SMS_SEND_SMS] = &HRilSms::SendSms;
    reqMemberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &HRilSms::SendSmsMoreMode;
    reqMemberFuncMap_[HREQ_SMS_SEND_SMS_ACK] = &HRilSms::SendSmsAck;
    reqMemberFuncMap_[HREQ_SMS_STORAGE_SMS] = &HRilSms::StorageSms;
    reqMemberFuncMap_[HREQ_SMS_DELETE_SMS] = &HRilSms::DeleteSms;
    reqMemberFuncMap_[HREQ_SMS_UPDATE_SMS] = &HRilSms::UpdateSms;
    reqMemberFuncMap_[HREQ_SMS_SET_CENTER_ADDRESS] = &HRilSms::SetSmsCenterAddress;
    reqMemberFuncMap_[HREQ_SMS_GET_CENTER_ADDRESS] = &HRilSms::GetSmsCenterAddress;
    reqMemberFuncMap_[HREQ_SMS_SET_CELL_BROADCAST] = &HRilSms::SetCellBroadcast;
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

void HRilSms::ProcessSmsNotify(
    int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen)
{
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            int32_t slotId = 0;
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