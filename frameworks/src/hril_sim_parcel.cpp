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

#include "hril_sim_parcel.h"

namespace OHOS {
namespace Telephony {
bool CardStatusInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, index)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, simType)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, simState)) {
        return false;
    }
    return true;
}

bool CardStatusInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, index)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, simType)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, simState)) {
        return false;
    }
    return true;
}

std::shared_ptr<CardStatusInfo> CardStatusInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CardStatusInfo> param = std::make_shared<CardStatusInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool IccIoResultInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, sw1)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, sw2)) {
        return false;
    }
    if (!ReadBaseString(parcel, response)) {
        return false;
    }
    return true;
}

bool IccIoResultInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, sw1)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, sw2)) {
        return false;
    }
    if (!WriteBaseString(parcel, response)) {
        return false;
    }
    return true;
}

std::shared_ptr<IccIoResultInfo> IccIoResultInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<IccIoResultInfo> param = std::make_shared<IccIoResultInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SimIoRequestInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, command)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, fileId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, p1)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, p2)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, p3)) {
        return false;
    }
    if (!ReadBaseString(parcel, data)) {
        return false;
    }
    if (!ReadBaseString(parcel, path)) {
        return false;
    }
    if (!ReadBaseString(parcel, pin2)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseString(parcel, aid)) {
        return false;
    };
    return true;
}

bool SimIoRequestInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, command)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, fileId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, p1)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, p2)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, p3)) {
        return false;
    }
    if (!WriteBaseString(parcel, data)) {
        return false;
    }
    if (!WriteBaseString(parcel, path)) {
        return false;
    }
    if (!WriteBaseString(parcel, pin2)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseString(parcel, aid)) {
        return false;
    }
    return true;
}

std::shared_ptr<SimIoRequestInfo> SimIoRequestInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SimIoRequestInfo> param = std::make_shared<SimIoRequestInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SimLockInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseString(parcel, fac)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, mode)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, status)) {
        return false;
    }
    if (!ReadBaseString(parcel, passwd)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, classx)) {
        return false;
    }
    return true;
}

bool SimLockInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseString(parcel, fac)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, mode)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, status)) {
        return false;
    }
    if (!WriteBaseString(parcel, passwd)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, classx)) {
        return false;
    }
    return true;
}

std::shared_ptr<SimLockInfo> SimLockInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SimLockInfo> param = std::make_shared<SimLockInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SimPasswordInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseString(parcel, fac)) {
        return false;
    }
    if (!ReadBaseString(parcel, oldPassword)) {
        return false;
    }
    if (!ReadBaseString(parcel, newPassword)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, passwordLength)) {
        return false;
    }
    return true;
}

bool SimPasswordInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseString(parcel, fac)) {
        return false;
    }
    if (!WriteBaseString(parcel, oldPassword)) {
        return false;
    }
    if (!WriteBaseString(parcel, newPassword)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, passwordLength)) {
        return false;
    }
    return true;
}

std::shared_ptr<SimPasswordInfo> SimPasswordInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SimPasswordInfo> param = std::make_shared<SimPasswordInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SimPinInputTimes::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseString(parcel, code)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, times)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, pukTimes)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, pinTimes)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, puk2Times)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, pin2Times)) {
        return false;
    }
    return true;
}

bool SimPinInputTimes::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseString(parcel, code)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, times)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, pukTimes)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, pinTimes)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, puk2Times)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, pin2Times)) {
        return false;
    }
    return true;
}

std::shared_ptr<SimPinInputTimes> SimPinInputTimes::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SimPinInputTimes> param = std::make_shared<SimPinInputTimes>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SimProtocolRequest::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, phase)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, protocol)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, slotId)) {
        return false;
    }
    return true;
}

bool SimProtocolRequest::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, phase)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, protocol)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, slotId)) {
        return false;
    }
    return true;
}

std::shared_ptr<SimProtocolRequest> SimProtocolRequest::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SimProtocolRequest> param = std::make_shared<SimProtocolRequest>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SimProtocolResponse::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, phase)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, result)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, slotId)) {
        return false;
    }
    return true;
}

bool SimProtocolResponse::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, phase)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, result)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, slotId)) {
        return false;
    }
    return true;
}

std::shared_ptr<SimProtocolResponse> SimProtocolResponse::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SimProtocolResponse> param = std::make_shared<SimProtocolResponse>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool OpenLogicalChannelResponse::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, sw1)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, sw2)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, channelId)) {
        return false;
    }
    if (!ReadBaseString(parcel, response)) {
        return false;
    }
    return true;
}

bool OpenLogicalChannelResponse::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, sw1)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, sw2)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, channelId)) {
        return false;
    }
    if (!WriteBaseString(parcel, response)) {
        return false;
    }
    return true;
}

std::shared_ptr<OpenLogicalChannelResponse> OpenLogicalChannelResponse::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<OpenLogicalChannelResponse> param = std::make_shared<OpenLogicalChannelResponse>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool ApduSimIORequestInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, channelId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, type)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, instruction)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, p1)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, p2)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, p3)) {
        return false;
    }
    if (!ReadBaseString(parcel, data)) {
        return false;
    }
    return true;
}

bool ApduSimIORequestInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, channelId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, type)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, instruction)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, p1)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, p2)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, p3)) {
        return false;
    }
    if (!WriteBaseString(parcel, data)) {
        return false;
    }
    return true;
}

std::shared_ptr<ApduSimIORequestInfo> ApduSimIORequestInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<ApduSimIORequestInfo> param = std::make_shared<ApduSimIORequestInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool SimAuthenticationRequestInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseString(parcel, aid)) {
        return false;
    }
    if (!ReadBaseString(parcel, authData)) {
        return false;
    }
    return true;
}

bool SimAuthenticationRequestInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseString(parcel, aid)) {
        return false;
    }
    if (!WriteBaseString(parcel, authData)) {
        return false;
    }
    return true;
}

std::shared_ptr<SimAuthenticationRequestInfo> SimAuthenticationRequestInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SimAuthenticationRequestInfo> param = std::make_shared<SimAuthenticationRequestInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool LockStatusResp::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, result)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, remain)) {
        return false;
    }
    return true;
}

bool LockStatusResp::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, result)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, remain)) {
        return false;
    }
    return true;
}

std::shared_ptr<LockStatusResp> LockStatusResp::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<LockStatusResp> param = std::make_shared<LockStatusResp>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}
} // namespace Telephony
} // namespace OHOS
