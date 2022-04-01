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
    if (!Read(parcel, serial)) {
        return false;
    }
    if (!Read(parcel, gsmIndex)) {
        return false;
    }
    if (!Read(parcel, flag)) {
        return false;
    }
    if (!Read(parcel, arg1)) {
        return false;
    }
    if (!Read(parcel, arg2)) {
        return false;
    }
    if (!Read(parcel, strTmp)) {
        return false;
    }
    return true;
}

bool UniInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, serial)) {
        return false;
    }
    if (!Write(parcel, gsmIndex)) {
        return false;
    }
    if (!Write(parcel, flag)) {
        return false;
    }
    if (!Write(parcel, arg1)) {
        return false;
    }
    if (!Write(parcel, arg2)) {
        return false;
    }
    if (!Write(parcel, strTmp)) {
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

VoiceRadioTechnology::VoiceRadioTechnology(const HRilVoiceRadioInfo &hrilVoiceRadioInfo)
{
    *this = hrilVoiceRadioInfo;
}

VoiceRadioTechnology &VoiceRadioTechnology::operator=(const HRilVoiceRadioInfo &hrilVoiceRadioInfo)
{
    srvStatus = hrilVoiceRadioInfo.srvStatus;
    srvDomain = hrilVoiceRadioInfo.srvDomain;
    roamStatus = hrilVoiceRadioInfo.roamStatus;
    simStatus = hrilVoiceRadioInfo.simStatus;
    lockStatus = hrilVoiceRadioInfo.lockStatus;
    sysMode = hrilVoiceRadioInfo.sysMode;
    sysModeName = hrilVoiceRadioInfo.sysModeName != nullptr ? hrilVoiceRadioInfo.sysModeName : "";
    actType = hrilVoiceRadioInfo.actType;
    actName = hrilVoiceRadioInfo.actName != nullptr ? hrilVoiceRadioInfo.actName : "";
    return *this;
}

bool VoiceRadioTechnology::ReadFromParcel(Parcel &parcel)
{
    return Read(parcel, srvStatus, srvDomain, roamStatus, simStatus,
        lockStatus, sysMode, sysModeName, actType, actName);
}

bool VoiceRadioTechnology::Marshalling(Parcel &parcel) const
{
    return Write(parcel, srvStatus, srvDomain, roamStatus, simStatus,
        lockStatus, sysMode, sysModeName, actType, actName);
}

std::shared_ptr<VoiceRadioTechnology> VoiceRadioTechnology::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<VoiceRadioTechnology> param = std::make_shared<VoiceRadioTechnology>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

const char *VoiceRadioTechnology::ToString() const
{
    DesensitizeStringStream dss(String(), StringStream());
    dss << srvStatus << srvDomain << roamStatus << simStatus << lockStatus << sysMode << sysModeName << actType
        << actName;
    return *dss;
}
} // namespace Telephony
} // namespace OHOS