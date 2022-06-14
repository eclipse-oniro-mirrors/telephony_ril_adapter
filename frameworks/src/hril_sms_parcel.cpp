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

#include "hril_sms_parcel.h"

#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
bool GsmSmsMessageInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseString(parcel, smscPdu)) {
        return false;
    }
    if (!ReadBaseString(parcel, pdu)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, state)) {
        return false;
    };
    return true;
}

bool GsmSmsMessageInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseString(parcel, smscPdu)) {
        return false;
    }
    if (!WriteBaseString(parcel, pdu)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, state)) {
        return false;
    };
    return true;
}

std::shared_ptr<GsmSmsMessageInfo> GsmSmsMessageInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<GsmSmsMessageInfo> param = std::make_shared<GsmSmsMessageInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SendCdmaSmsMessageInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseString(parcel, smscPdu)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, state)) {
        return false;
    };
    return true;
}

bool SendCdmaSmsMessageInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseString(parcel, smscPdu)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, state)) {
        return false;
    };
    return true;
}

std::shared_ptr<SendCdmaSmsMessageInfo> SendCdmaSmsMessageInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SendCdmaSmsMessageInfo> param = std::make_shared<SendCdmaSmsMessageInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SmsMessageIOInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseString(parcel, smscPdu)) {
        return false;
    }
    if (!ReadBaseString(parcel, pdu)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, state)) {
        return false;
    };
    if (!ReadBaseInt32(parcel, index)) {
        return false;
    };
    return true;
}

bool SmsMessageIOInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseString(parcel, smscPdu)) {
        return false;
    }
    if (!WriteBaseString(parcel, pdu)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, state)) {
        return false;
    };
    if (!WriteBaseInt32(parcel, index)) {
        return false;
    };
    return true;
}

std::shared_ptr<SmsMessageIOInfo> SmsMessageIOInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SmsMessageIOInfo> param = std::make_shared<SmsMessageIOInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool ServiceCenterAddress::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, tosca)) {
        return false;
    }
    if (!ReadBaseString(parcel, address)) {
        return false;
    }
    return true;
}

bool ServiceCenterAddress::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, tosca)) {
        return false;
    }
    if (!WriteBaseString(parcel, address)) {
        return false;
    }
    return true;
}

std::shared_ptr<ServiceCenterAddress> ServiceCenterAddress::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<ServiceCenterAddress> param = std::make_shared<ServiceCenterAddress>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CBConfigInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, mode)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, indicationType)) {
        return false;
    }
    if (!ReadBaseString(parcel, mids)) {
        return false;
    }
    if (!ReadBaseString(parcel, dcss)) {
        return false;
    }
    return true;
}

bool CBConfigInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, mode)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, indicationType)) {
        return false;
    }
    if (!WriteBaseString(parcel, mids)) {
        return false;
    }
    if (!WriteBaseString(parcel, dcss)) {
        return false;
    }
    return true;
}

std::shared_ptr<CBConfigInfo> CBConfigInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CBConfigInfo> param = std::make_shared<CBConfigInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CdmaCBConfigInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, service)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, language)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, checked)) {
        return false;
    }
    return true;
}

bool CdmaCBConfigInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, service)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, language)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, checked)) {
        return false;
    }
    return true;
}

std::shared_ptr<CdmaCBConfigInfo> CdmaCBConfigInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CdmaCBConfigInfo> param = std::make_shared<CdmaCBConfigInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CdmaCBConfigInfoList::ReadFromParcel(Parcel &parcel)
{
    if (size > TELEPHONY_PARCEL_MAX_COUNT) {
        TELEPHONY_LOGE("CdmaCBConfigInfoList, ReadFromParcel size:%{public}d is invalid", size);
        return false;
    }
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, size)) {
        return false;
    }
    list.resize(size);
    for (int32_t i = 0; i < size; i++) {
        list[i].ReadFromParcel(parcel);
    }
    return true;
}

bool CdmaCBConfigInfoList::Marshalling(Parcel &parcel) const
{
    if (size > TELEPHONY_PARCEL_MAX_COUNT) {
        TELEPHONY_LOGE("CdmaCBConfigInfoList, Marshalling size:%{public}d is invalid", size);
        return false;
    }
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, size)) {
        return false;
    }
    for (int32_t i = 0; i < size; i++) {
        list[i].Marshalling(parcel);
    }
    return true;
}

std::shared_ptr<CdmaCBConfigInfoList> CdmaCBConfigInfoList::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CdmaCBConfigInfoList> param = std::make_shared<CdmaCBConfigInfoList>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CBConfigReportInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, indicationType)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, sn)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, mid)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, page)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, pages)) {
        return false;
    }
    if (!ReadBaseString(parcel, dcs)) {
        return false;
    }
    if (!ReadBaseString(parcel, data)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, length)) {
        return false;
    }
    if (!ReadBaseString(parcel, pdu)) {
        return false;
    }
    return true;
}

bool CBConfigReportInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, indicationType)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, sn)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, mid)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, page)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, pages)) {
        return false;
    }
    if (!WriteBaseString(parcel, dcs)) {
        return false;
    }
    if (!WriteBaseString(parcel, data)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, length)) {
        return false;
    }
    if (!WriteBaseString(parcel, pdu)) {
        return false;
    }
    return true;
}

std::shared_ptr<CBConfigReportInfo> CBConfigReportInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CBConfigReportInfo> param = std::make_shared<CBConfigReportInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SendSmsResultInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, msgRef)) {
        return false;
    }
    if (!ReadBaseString(parcel, pdu)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, errCode)) {
        return false;
    }
    if (!ReadBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

bool SendSmsResultInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, msgRef)) {
        return false;
    }
    if (!WriteBaseString(parcel, pdu)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, errCode)) {
        return false;
    }
    if (!WriteBaseInt64(parcel, flag)) {
        return false;
    }
    return true;
}

std::shared_ptr<SendSmsResultInfo> SendSmsResultInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SendSmsResultInfo> param = std::make_shared<SendSmsResultInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool ModeData::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseBool(parcel, result)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, mode)) {
        return false;
    }
    if (!ReadBaseString(parcel, pdu)) {
        return false;
    }
    return true;
}

bool ModeData::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseBool(parcel, result)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, mode)) {
        return false;
    }
    if (!WriteBaseString(parcel, pdu)) {
        return false;
    }
    return true;
}

std::shared_ptr<ModeData> ModeData::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<ModeData> param = std::make_shared<ModeData>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SmsMessageInfo::ReadFromParcel(Parcel &parcel)
{
    if (size > TELEPHONY_PARCEL_MAX_COUNT) {
        TELEPHONY_LOGE("SmsMessageInfo, ReadFromParcel size:%{public}d is invalid", size);
        return false;
    }
    if (!ReadBaseInt32(parcel, indicationType)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, size)) {
        return false;
    }
    pdu.resize(size);
    for (int32_t i = 0; i < size; i++) {
        if (!ReadBaseUint8(parcel, pdu[i])) {
            return false;
        }
    }
    return true;
}

bool SmsMessageInfo::Marshalling(Parcel &parcel) const
{
    if (size > TELEPHONY_PARCEL_MAX_COUNT) {
        TELEPHONY_LOGE("SmsMessageInfo, Marshalling size:%{public}d is invalid", size);
        return false;
    }
    if (!WriteBaseInt32(parcel, indicationType)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, size)) {
        return false;
    }
    for (int32_t i = 0; i < size; i++) {
        if (!WriteBaseUint8(parcel, pdu[i])) {
            return false;
        }
    }
    return true;
}

std::shared_ptr<SmsMessageInfo> SmsMessageInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SmsMessageInfo> param = std::make_shared<SmsMessageInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CdmaSmsInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, indicationType)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, msgRef)) {
        return false;
    }
    if (!ReadBaseString(parcel, pdu)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, errCode)) {
        return false;
    }
    return true;
}

bool CdmaSmsInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, indicationType)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, msgRef)) {
        return false;
    }
    if (!WriteBaseString(parcel, pdu)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, errCode)) {
        return false;
    }
    return true;
}

std::shared_ptr<CdmaSmsInfo> CdmaSmsInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CdmaSmsInfo> param = std::make_shared<CdmaSmsInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}
} // namespace Telephony
} // namespace OHOS