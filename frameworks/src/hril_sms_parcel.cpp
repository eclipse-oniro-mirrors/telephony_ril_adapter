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
} // namespace Telephony
} // namespace OHOS