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

#ifndef OHOS_RIL_BASE_PARCEL_H
#define OHOS_RIL_BASE_PARCEL_H

#include "parcel.h"

#include "hril_types.h"

namespace OHOS {
namespace Telephony {
class HrilBaseParcel : public virtual Parcelable {
protected:
    virtual bool ReadBaseUint8(Parcel &parcel, uint8_t &value);
    virtual bool ReadBaseInt32(Parcel &parcel, int32_t &value);
    virtual bool ReadBaseInt64(Parcel &parcel, int64_t &value);
    virtual bool ReadBaseBool(Parcel &parcel, bool &value);
    virtual bool ReadBaseString(Parcel &parcel, std::string &value);
    virtual bool WriteBaseUint8(Parcel &parcel, uint8_t value) const;
    virtual bool WriteBaseInt32(Parcel &parcel, int32_t value) const;
    virtual bool WriteBaseInt64(Parcel &parcel, int64_t value) const;
    virtual bool WriteBaseBool(Parcel &parcel, bool value) const;
    virtual bool WriteBaseString(Parcel &parcel, std::string value) const;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_BASE_PARCEL_H