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

#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
bool UniInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, gsmIndex)) {
        return false;
    }
    if (!ReadBaseBool(parcel, flag)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, arg1)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, arg2)) {
        return false;
    }
    if (!ReadBaseString(parcel, strTmp)) {
        return false;
    }
    return true;
}

bool UniInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, gsmIndex)) {
        return false;
    }
    if (!WriteBaseBool(parcel, flag)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, arg1)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, arg2)) {
        return false;
    }
    if (!WriteBaseString(parcel, strTmp)) {
        return false;
    }
    return true;
}

std::shared_ptr<UniInfo> UniInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<UniInfo> param = std::make_shared<UniInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool VoiceRadioTechnology::ReadFromParcel(Parcel &parcel)
{
    int32_t srv = 0;
    int32_t domain = 0;
    int32_t roam = 0;
    int32_t sim = 0;
    int32_t lock = 0;
    int32_t sys = 0;
    int32_t act = 0;
    if (!ReadBaseInt32(parcel, srv)) {
        return false;
    }
    srvStatus = static_cast<HRilSrvStatus>(srv);
    if (!ReadBaseInt32(parcel, domain)) {
        return false;
    }
    srvDomain = static_cast<HRilSrvDomain>(domain);
    if (!ReadBaseInt32(parcel, roam)) {
        return false;
    }
    roam = static_cast<HRilRoamStatus>(roam);
    if (!ReadBaseInt32(parcel, sim)) {
        return false;
    }
    simStatus = static_cast<HRilSimStatus>(sim);
    if (!ReadBaseInt32(parcel, lock)) {
        return false;
    }
    lockStatus = static_cast<HRilSimLockStatus>(lock);
    if (!ReadBaseInt32(parcel, sys)) {
        return false;
    }
    sysMode = static_cast<HRilSysMode>(sys);
    if (!ReadBaseString(parcel, sysModeName)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, act)) {
        return false;
    }
    actType = static_cast<HRilRadioTech>(act);
    if (!ReadBaseString(parcel, actName)) {
        return false;
    }
    return true;
}

bool VoiceRadioTechnology::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, static_cast<int32_t>(srvStatus))) {
        return false;
    }
    if (!WriteBaseInt32(parcel, static_cast<int32_t>(srvDomain))) {
        return false;
    }
    if (!WriteBaseInt32(parcel, static_cast<int32_t>(roamStatus))) {
        return false;
    }
    if (!WriteBaseInt32(parcel, static_cast<int32_t>(simStatus))) {
        return false;
    }
    if (!WriteBaseInt32(parcel, static_cast<int32_t>(lockStatus))) {
        return false;
    }
    if (!WriteBaseInt32(parcel, static_cast<int32_t>(sysMode))) {
        return false;
    }
    if (!WriteBaseString(parcel, sysModeName)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, static_cast<int32_t>(actType))) {
        return false;
    }
    if (!WriteBaseString(parcel, actName)) {
        return false;
    }
    return true;
}

std::shared_ptr<VoiceRadioTechnology> VoiceRadioTechnology::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<VoiceRadioTechnology> param = std::make_shared<VoiceRadioTechnology>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}
} // namespace Telephony
} // namespace OHOS