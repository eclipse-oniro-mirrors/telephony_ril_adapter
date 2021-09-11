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
#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
bool CardStatusInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, index);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, simType);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, simState);

    return true;
}

bool CardStatusInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, index);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, simType);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, simState);

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
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, sw1);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, sw2);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, response);
    return true;
}

bool IccIoResultInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, sw1);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, sw2);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, response);
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
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, command);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, fileId);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, p1);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, p2);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, p3);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, data);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, path);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    return true;
}

bool SimIoRequestInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, command);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, fileId);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, p1);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, p2);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, p3);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, data);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, path);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);

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
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, fac);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, passwd);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classx);

    return true;
}

bool SimLockInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, fac);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, passwd);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classx);

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
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, fac);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, oldPassword);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, newPassword);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, passwordLength);

    return true;
}

bool SimPasswordInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, fac);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, oldPassword);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, newPassword);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, passwordLength);

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
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, code);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, times);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pukTimes);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pinTimes);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, puk2Times);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pin2Times);

    return true;
}

bool SimPinInputTimes::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, code);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, times);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pukTimes);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pinTimes);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, puk2Times);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, pin2Times);

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
} // namespace Telephony
} // namespace OHOS
