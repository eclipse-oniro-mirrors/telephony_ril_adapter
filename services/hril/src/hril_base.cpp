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
    if (dest == nullptr) {
        TELEPHONY_LOGE("ConvertToString dest is null");
        return false;
    }
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

void HRilBase::CopyToCharPoint(char **dest, const std::string &src)
{
    size_t size = src.size();
    if (size <= 0) {
        TELEPHONY_LOGE("CopyToCharPoint src is null");
        return;
    }
    *dest = (char *)malloc((size + 1) * sizeof(char));
    if (*dest == nullptr) {
        TELEPHONY_LOGE("CopyToCharPoint malloc content fail!");
        return;
    }
    if (memset_s(*dest, size + 1, 0, size + 1) != EOK) {
        TELEPHONY_LOGE("CopyToCharPoint memset_s failed");
        SafeFrees(*dest);
        return;
    }
    if (strcpy_s(*dest, size + 1, src.c_str()) != EOK) {
        TELEPHONY_LOGE("CopyToCharPoint strcpy_s error");
        SafeFrees(*dest);
    }
}

ReqDataInfo *HRilBase::CreateHRilRequest(int32_t serial, int32_t request)
{
    return hrilReporter_.CreateHRilRequest(serial, slotId_, request);
}

HDI::Ril::V1_0::RilRadioResponseInfo HRilBase::BuildIHRilRadioResponseInfo(const HRilRadioResponseInfo &responseInfo)
{
    HDI::Ril::V1_0::RilRadioResponseInfo iResponseInfo = { 0 };
    iResponseInfo.slotId = GetSlotId();
    iResponseInfo.flag = responseInfo.flag;
    iResponseInfo.serial = responseInfo.serial;
    iResponseInfo.error = (HDI::Ril::V1_0::RilErrType)responseInfo.error;
    iResponseInfo.type = (HDI::Ril::V1_0::RilResponseTypes)responseInfo.type;
    return iResponseInfo;
}

void HRilBase::SetRilCallback(const sptr<HDI::Ril::V1_0::IRilCallback> &callback)
{
    callback_ = callback;
}
} // namespace Telephony
} // namespace OHOS