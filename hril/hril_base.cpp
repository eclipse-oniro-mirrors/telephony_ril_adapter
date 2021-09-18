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
int32_t HRilBase::AtoiNullHandle(const char *ch)
{
    return ((ch) ? (atoi(ch)) : -1);
}

int32_t HRilBase::AtoiNullHandleDef(const char *ch, int32_t defaultVal)
{
    return ((ch) ? (atoi(ch)) : defaultVal);
}

void HRilBase::SendErrorResponse(ReqDataInfo *requestInfo, HRilErrno err)
{
    if (requestInfo != nullptr) {
        TELEPHONY_LOGE("req: [%{public}d,%{public}d,%{public}d], error response: %{public}d", requestInfo->serial,
            (int32_t)requestInfo->simId, requestInfo->request, err);
    }
}

int32_t HRilBase::ResponseBuffer(
    int32_t requestNum, const void *responseInfo, uint32_t reqLen, const void *event, uint32_t eventLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)responseInfo, reqLen)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)event, eventLen)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallback_->dispatcher->Dispatch(serviceCallback_, requestNum, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

int32_t HRilBase::ResponseRequestInfo(int32_t requestNum, const void *responseInfo, uint32_t reqLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)responseInfo, reqLen)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallback_->dispatcher->Dispatch(serviceCallback_, requestNum, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

int32_t HRilBase::ConvertHexStringToInt(char **response, int32_t index, int32_t length)
{
    const int32_t hexBase = HRIL_INVALID_HEX_CHAR;
    if ((response == nullptr) || (length <= index) || (response[index] == nullptr)) {
        return HDF_FAILURE;
    }
    return strtol(response[index], nullptr, hexBase);
}

HRilNotiType HRilBase::ConvertIntToRadioNoticeType(int32_t indicationType)
{
    return (indicationType == HRIL_NOTIFICATION_TYPE) ? (HRilNotiType::HRIL_NOTIFICATION) :
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
    if (length % SIZE_VALUE != 0 || length / SIZE_VALUE <= 0) {
        TELEPHONY_LOGE("invalid length: %{public}zu", length);
        return nullptr;
    }
    uint8_t *bytes = (uint8_t *)calloc(length / SIZE_VALUE, sizeof(uint8_t));
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

bool HRilBase::ConvertToString(char **dest, const std::string &srcStr, ReqDataInfo *requestInfo)
{
    size_t size = srcStr.size();
    if (size == 0) {
        *dest = nullptr;
        return true;
    }
    size_t len = sizeof(char);
    if (len <= 0) {
        return false;
    }
    *dest = (char *)calloc(size + 1, len);
    if (*dest == nullptr) {
        SendErrorResponse(requestInfo, HRIL_ERR_MEMORY_FULL);
        return false;
    }
    (void)strncpy_s(*dest, size + 1, reinterpret_cast<const char *>(srcStr.c_str()), size);
    return true;
}

void HRilBase::FreeStrings(int32_t argCounts, ...)
{
    va_list list;
    va_start(list, argCounts);
    int32_t i = 0;
    while (i < argCounts) {
        char *ch = va_arg(list, char *);
        if (ch) {
            free(ch);
        }
        i++;
    }
    va_end(list);
}

void HRilBase::RegisterResponseCallback(HdfRemoteService *serviceCallback)
{
    serviceCallback_ = serviceCallback;
}

void HRilBase::RegisterNotifyCallback(HdfRemoteService *serviceCallbackInd)
{
    serviceCallbackNotify_ = serviceCallbackInd;
}
} // namespace Telephony
} // namespace OHOS
