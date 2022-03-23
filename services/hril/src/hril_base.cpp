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

#include "hril_base.h"

namespace OHOS {
namespace Telephony {
bool HRilBase::ReadFromHdfBuf(struct HdfSBuf *data, int32_t &val)
{
    return HdfSbufReadInt32(data, &val);
}

bool HRilBase::ReadFromHdfBuf(struct HdfSBuf *data, const char *&val)
{
    val = HdfSbufReadString(data);
    if (val == nullptr) {
        TELEPHONY_LOGE("read failed from hdf buf!!!");
        return false;
    }
    return true;
}

int32_t HRilBase::ResponseHeader(
    const HRilRadioResponseInfo &responseInfo, std::shared_ptr<struct HdfSBuf> &dataSbuf, MessageParcel &parcel)
{
    int32_t ret = ReportHeader(dataSbuf, parcel);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("write fail for report header!!!");
        return ret;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf.get(), (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("write failed for buffer");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilBase::ReportHeader(std::shared_ptr<struct HdfSBuf> &dataSbuf, MessageParcel &parcel)
{
    if (!parcel.WriteInterfaceToken(HRIL_INTERFACE_TOKEN)) {
        TELEPHONY_LOGE("write interface token failed.");
        return HDF_FAILURE;
    }
    dataSbuf.reset(ParcelToSbuf(&parcel), [](struct HdfSBuf *d) { HdfSbufRecycle(d); });
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is null!!!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf.get(), GetSlotId())) {
        TELEPHONY_LOGE("write failed for slot id");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilBase::ResponseBuffer(
    int32_t requestNum, const void *responseInfo, uint32_t reqLen, const void *event, uint32_t eventLen)
{
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
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, slotId_)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)responseInfo, reqLen)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)event, eventLen)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilBase::ResponseInt32(int32_t requestNum, const void *responseInfo, uint32_t reqLen, uint32_t value)
{
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
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, slotId_)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)responseInfo, reqLen)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, value)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilBase::ResponseRequestInfo(int32_t requestNum, const void *responseInfo, uint32_t reqLen)
{
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
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, slotId_)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)responseInfo, reqLen)) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t HRilBase::ConvertHexStringToInt(char **response, int32_t index, int32_t length)
{
    const int32_t hexBase = HRIL_INVALID_HEX_CHAR;
    if ((response == nullptr) || (length <= index) || (response[index] == nullptr)) {
        return HRIL_ERR_GENERIC_FAILURE;
    }
    return strtol(response[index], nullptr, hexBase);
}

HRilNotiType HRilBase::ConvertIntToRadioNoticeType(int32_t indicationType)
{
    return (indicationType == (int32_t)ReportType::HRIL_NOTIFICATION) ? (HRilNotiType::HRIL_NOTIFICATION) :
                                                                        (HRilNotiType::HRIL_NO_DEFINE);
}

uint8_t HRilBase::ConvertHexCharToInt(uint8_t ch)
{
    if ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) {
        return ((ch - 'A') % HRIL_UPPER_CASE_LETTERS_OFFSET + HRIL_DEC);
    } else if (ch >= '0' && ch <= '9') {
        return (ch - '0');
    } else {
        return HRIL_INVALID_HEX_CHAR;
    }
}

uint8_t *HRilBase::ConvertHexStringToBytes(const void *response, size_t length)
{
    const int32_t SIZE_VALUE = 2;
    const int32_t BIT_NUM = 4;

    if (response == nullptr) {
        TELEPHONY_LOGE("response is null!!!");
        return nullptr;
    }

    size_t bytesLen = length / SIZE_VALUE;
    if (length % SIZE_VALUE != 0 || bytesLen <= 0) {
        TELEPHONY_LOGE("invalid length: %{public}zu", length);
        return nullptr;
    }
    uint8_t *bytes = (uint8_t *)calloc(bytesLen, sizeof(uint8_t));
    if (bytes == nullptr) {
        TELEPHONY_LOGE("ConvertHexStringToBytes: cannot allocate memory for bytes string");
        return nullptr;
    }

    uint8_t *hexStr = (uint8_t *)response;
    size_t i = 0;
    while (i < length) {
        uint8_t hexCh1 = ConvertHexCharToInt(hexStr[i]);
        uint8_t hexCh2 = ConvertHexCharToInt(hexStr[i + 1]);
        if (hexCh1 == HRIL_INVALID_HEX_CHAR || hexCh2 == HRIL_INVALID_HEX_CHAR) {
            free(bytes);
            return nullptr;
        }
        bytes[i / SIZE_VALUE] = ((hexCh1 << BIT_NUM) | hexCh2);
        i += SIZE_VALUE;
    }
    return bytes;
}

bool HRilBase::ConvertToString(char **dest, const std::string &srcStr)
{
    size_t size = srcStr.size();
    if (size == 0) {
        *dest = nullptr;
        return true;
    }
    size_t len = size + 1;
    if (len <= 0) {
        return false;
    }

    *dest = (char *)calloc(len, sizeof(char));
    if (*dest == nullptr) {
        TELEPHONY_LOGE("ConvertToString malloc fail");
        return false;
    }
    if (strncpy_s(*dest, len, reinterpret_cast<const char *>(srcStr.c_str()), size) != EOK) {
        return false;
    }
    return true;
}

int32_t HRilBase::ServiceDispatcher(int32_t requestNum, const HdfSBuf *dataSbuf)
{
    return hrilReporter_.ReportToParent(requestNum, dataSbuf);
}

int32_t HRilBase::ServiceNotifyDispatcher(int32_t requestNum, const HdfSBuf *dataSbuf)
{
    return hrilReporter_.NotifyToParent(requestNum, dataSbuf);
}

ReqDataInfo *HRilBase::CreateHRilRequest(int32_t serial, int32_t request)
{
    return hrilReporter_.CreateHRilRequest(serial, slotId_, request);
}
} // namespace Telephony
} // namespace OHOS