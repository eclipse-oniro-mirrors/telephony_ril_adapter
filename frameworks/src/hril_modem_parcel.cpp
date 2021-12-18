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
} // namespace Telephony
} // namespace OHOS