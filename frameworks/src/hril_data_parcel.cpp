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

#include "hril_data_parcel.h"

#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
bool SetupDataCallResultInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, flag)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, reason)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, retryTime)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, cid)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, active)) {
        return false;
    }
    if (!ReadBaseString(parcel, type)) {
        return false;
    }
    if (!ReadBaseString(parcel, netPortName)) {
        return false;
    }
    if (!ReadBaseString(parcel, address)) {
        return false;
    }
    if (!ReadBaseString(parcel, dns)) {
        return false;
    }
    if (!ReadBaseString(parcel, gateway)) {
        return false;
    }
    if (!ReadBaseString(parcel, pcscfa)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, maxTransferUnit)) {
        return false;
    }
    return true;
}

bool SetupDataCallResultInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, flag)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, reason)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, retryTime)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, cid)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, active)) {
        return false;
    }
    if (!WriteBaseString(parcel, type)) {
        return false;
    }
    if (!WriteBaseString(parcel, netPortName)) {
        return false;
    }
    if (!WriteBaseString(parcel, address)) {
        return false;
    }
    if (!WriteBaseString(parcel, dns)) {
        return false;
    }
    if (!WriteBaseString(parcel, gateway)) {
        return false;
    }
    if (!WriteBaseString(parcel, pcscfa)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, maxTransferUnit)) {
        return false;
    }
    return true;
}

std::shared_ptr<SetupDataCallResultInfo> SetupDataCallResultInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SetupDataCallResultInfo> param = std::make_shared<SetupDataCallResultInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataCallResultList::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, size)) {
        return false;
    }
    dcList.resize(size);
    for (int32_t i = 0; i < size; i++) {
        dcList[i].ReadFromParcel(parcel);
    }
    return true;
}

bool DataCallResultList::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, size)) {
        return false;
    }
    for (int32_t i = 0; i < size; i++) {
        dcList[i].Marshalling(parcel);
    }
    return true;
}

std::shared_ptr<DataCallResultList> DataCallResultList::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataCallResultList> param = std::make_shared<DataCallResultList>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataProfileDataInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, profileId)) {
        return false;
    }
    if (!ReadBaseString(parcel, apn)) {
        return false;
    }
    if (!ReadBaseString(parcel, protocol)) {
        return false;
    }
    if (!ReadBaseString(parcel, roamingProtocol)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, verType)) {
        return false;
    }
    if (!ReadBaseString(parcel, userName)) {
        return false;
    }
    if (!ReadBaseString(parcel, password)) {
        return false;
    }
    return true;
}

bool DataProfileDataInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, profileId)) {
        return false;
    }
    if (!WriteBaseString(parcel, apn)) {
        return false;
    }
    if (!WriteBaseString(parcel, protocol)) {
        return false;
    }
    if (!WriteBaseString(parcel, roamingProtocol)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, verType)) {
        return false;
    }
    if (!WriteBaseString(parcel, userName)) {
        return false;
    }
    if (!WriteBaseString(parcel, password)) {
        return false;
    }
    return true;
}

std::shared_ptr<DataProfileDataInfo> DataProfileDataInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataProfileDataInfo> param = std::make_shared<DataProfileDataInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataProfilesInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, profilesSize)) {
        return false;
    }
    profiles.resize(profilesSize);
    for (int32_t i = 0; i < profilesSize; i++) {
        profiles[i].ReadFromParcel(parcel);
    }
    if (!ReadBaseBool(parcel, isRoaming)) {
        return false;
    }
    return true;
}

bool DataProfilesInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, profilesSize)) {
        return false;
    }
    for (int32_t i = 0; i < profilesSize; i++) {
        profiles[i].Marshalling(parcel);
    }
    if (!WriteBaseBool(parcel, isRoaming)) {
        return false;
    }
    return true;
}

std::shared_ptr<DataProfilesInfo> DataProfilesInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataProfilesInfo> param = std::make_shared<DataProfilesInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataCallInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, radioTechnology)) {
        return false;
    }
    dataProfileInfo.ReadFromParcel(parcel);
    if (!ReadBaseBool(parcel, modemCognitive)) {
        return false;
    }
    if (!ReadBaseBool(parcel, roamingAllowed)) {
        return false;
    }
    if (!ReadBaseBool(parcel, isRoaming)) {
        return false;
    }
    return true;
}

bool DataCallInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, radioTechnology)) {
        return false;
    }
    dataProfileInfo.Marshalling(parcel);
    if (!WriteBaseBool(parcel, modemCognitive)) {
        return false;
    }
    if (!WriteBaseBool(parcel, roamingAllowed)) {
        return false;
    }
    if (!WriteBaseBool(parcel, isRoaming)) {
        return false;
    }
    return true;
}

std::shared_ptr<DataCallInfo> DataCallInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataCallInfo> param = std::make_shared<DataCallInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}
} // namespace Telephony
} // namespace OHOS