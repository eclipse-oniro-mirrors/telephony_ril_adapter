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
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, smscPdu);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, state);
    return true;
}

bool GsmSmsMessageInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, smscPdu);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, state);
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

bool SmsMessageIOInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, smscPdu);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, state);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, index);
    return true;
}

bool SmsMessageIOInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, smscPdu);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, state);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, index);
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
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, tosca);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, address);
    return true;
}

bool ServiceCenterAddress::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, tosca);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, address);
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

bool CellBroadcastInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, indicationType);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, mids);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, dcss);
    return true;
}

bool CellBroadcastInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, indicationType);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, mids);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, dcss);
    return true;
}

std::shared_ptr<CellBroadcastInfo> CellBroadcastInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CellBroadcastInfo> param = std::make_shared<CellBroadcastInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CellBroadcastReportInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, indicationType);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, sn);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mid);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, page);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pages);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, dcs);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, data);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, length);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
    return true;
}

bool CellBroadcastReportInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, indicationType);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, sn);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mid);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, page);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pages);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, dcs);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, data);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, length);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
    return true;
}

std::shared_ptr<CellBroadcastReportInfo> CellBroadcastReportInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CellBroadcastReportInfo> param = std::make_shared<CellBroadcastReportInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool ImsSmsMessageInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, technology);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, retry);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, msgRef);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, gsmMessageSize);
    gsmMessage.resize(gsmMessageSize);
    for (int32_t i = 0; i < gsmMessageSize; i++) {
        gsmMessage[i].ReadFromParcel(parcel);
    }
    return true;
}

bool ImsSmsMessageInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, technology);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, retry);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, msgRef);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, gsmMessageSize);
    for (int32_t i = 0; i < gsmMessageSize; i++) {
        gsmMessage[i].Marshalling(parcel);
    }
    return true;
}

std::shared_ptr<ImsSmsMessageInfo> ImsSmsMessageInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<ImsSmsMessageInfo> param = std::make_shared<ImsSmsMessageInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SendSmsResultInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, msgRef);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, errCode);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, flag);
    return true;
}

bool SendSmsResultInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, msgRef);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, errCode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int64, parcel, flag);
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
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, result);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
    return true;
}

bool ModeData::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Bool, parcel, result);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, pdu);
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
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, indicationType);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, size);
    pdu.resize(size);
    for (int32_t i = 0; i < size; i++) {
        READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint8, parcel, pdu[i]);
    }
    return true;
}

bool SmsMessageInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, indicationType);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, size);
    for (int32_t i = 0; i < size; i++) {
        WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Uint8, parcel, pdu[i]);
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
} // namespace Telephony
} // namespace OHOS