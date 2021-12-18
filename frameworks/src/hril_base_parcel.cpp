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

#include "hril_base_parcel.h"

namespace OHOS {
namespace Telephony {
bool HrilBaseParcel::ReadBaseUint8(Parcel &parcel, uint8_t &value)
{
    return parcel.ReadUint8(value);
}

bool HrilBaseParcel::ReadBaseInt32(Parcel &parcel, int32_t &value)
{
    return parcel.ReadInt32(value);
}

bool HrilBaseParcel::ReadBaseInt64(Parcel &parcel, int64_t &value)
{
    return parcel.ReadInt64(value);
}

bool HrilBaseParcel::ReadBaseBool(Parcel &parcel, bool &value)
{
    return parcel.ReadBool(value);
}

bool HrilBaseParcel::ReadBaseString(Parcel &parcel, std::string &value)
{
    return parcel.ReadString(value);
}

bool HrilBaseParcel::WriteBaseUint8(Parcel &parcel, uint8_t value) const
{
    return parcel.WriteUint8(value);
}

bool HrilBaseParcel::WriteBaseInt32(Parcel &parcel, int32_t value) const
{
    return parcel.WriteInt32(value);
}

bool HrilBaseParcel::WriteBaseInt64(Parcel &parcel, int64_t value) const
{
    return parcel.WriteInt64(value);
}

bool HrilBaseParcel::WriteBaseBool(Parcel &parcel, bool value) const
{
    return parcel.WriteBool(value);
}

bool HrilBaseParcel::WriteBaseString(Parcel &parcel, std::string value) const
{
    return parcel.WriteString(value);
}
} // namespace Telephony
} // namespace OHOS