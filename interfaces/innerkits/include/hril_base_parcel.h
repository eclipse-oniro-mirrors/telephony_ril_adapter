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

#include <cassert>

#include "desensitize_string_stream.h"
#include "parcel.h"

#include "hril_types.h"
#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
// Serialize the underlying set of handler functions.
namespace BaseParcel {
// Read specific data from the parcel object.
bool Read(Parcel &parcel, int8_t &value);
bool Read(Parcel &parcel, uint8_t &value);
bool Read(Parcel &parcel, int32_t &value);
bool Read(Parcel &parcel, uint32_t &value);
bool Read(Parcel &parcel, int64_t &value);
bool Read(Parcel &parcel, uint64_t &value);
bool Read(Parcel &parcel, bool &value);
bool Read(Parcel &parcel, std::string &value);
// Reads only 32 bits of data. Such as: enum.
template<typename E>
bool Read(Parcel &parcel, E &e)
{
    static_assert(sizeof(E) == sizeof(int32_t) && !std::is_pointer<E>::value,
        "This interface is an enum-specific interface,"
        " please use the enumeration type(type size == sizeof(int32_t)).");
    return Read(parcel, *((int32_t *)&e));
}
// Write the data to the parcel object.
bool Write(Parcel &parcel, const int8_t &value);
bool Write(Parcel &parcel, const uint8_t &value);
bool Write(Parcel &parcel, const int32_t &value);
bool Write(Parcel &parcel, const uint32_t &value);
bool Write(Parcel &parcel, const int64_t &value);
bool Write(Parcel &parcel, const uint64_t &value);
bool Write(Parcel &parcel, const bool &value);
bool Write(Parcel &parcel, const std::string &value);
// Writes only 32 bits of data. Such as: enum.
template<typename E>
bool Write(Parcel &parcel, E &&e)
{
    static_assert(sizeof(E) == sizeof(int32_t) && !std::is_pointer<E>::value,
        "This interface is an enum-specific interface,"
        " please use the enumeration type(type size == sizeof(int32_t)).");
    return Write(parcel, *((const int32_t *)&e));
}

/**
 * @brief Write the data set from Parcel.
 * @tparam ValueTypes ValueTypes Support data types: uint8_t, int32_t, int64_t, bool, std::string.
 * @param parcel Serialized data structure.
 * @param vals Output data set.
 *      1st param: slotId
 *      2nd param: serialId
 * @return true success
 * @return false failed
 */
template<typename... ValueTypes>
bool WriteVals(Parcel &parcel, ValueTypes &&...vals)
{
    // Write data with the return value "and".
    return (Write(parcel, std::forward<ValueTypes>(vals)) && ...);
}

/**
 * @brief Read the data set from Parcel
 * @tparam ValueTypes Support data types: uint8_t, int32_t, int64_t, bool, std::string
 * @param parcel Serialized data structure
 * @param vals Outgoing data reference
 * @return true success
 * @return false failed
 */
template<typename... ValueTypes>
bool ReadVals(Parcel &parcel, ValueTypes &&...vals)
{
    // Read data with the return value "and".
    return (Read(parcel, std::forward<ValueTypes>(vals)) && ...);
}
} // namespace BaseParcel

class HrilBaseParcel : public virtual Parcelable {
public:
    // Formatted output for serialized structures.
    virtual const char *ToString() const;

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

    template<typename... ValueTypes>
    bool Read(Parcel &parcel, ValueTypes &&...vals)
    {
        return BaseParcel::ReadVals(parcel, std::forward<ValueTypes>(vals)...);
    }

    template<typename... ValueTypes>
    bool Write(Parcel &parcel, ValueTypes &&...vals) const
    {
        return BaseParcel::WriteVals(parcel, std::forward<ValueTypes>(vals)...);
    }

    // String streams: thread variables. Used to optimize execution efficiency.
    std::stringstream &StringStream(void) const;
    // String: Thread variable. Used to optimize execution efficiency.
    std::string &String(void) const;
};

// Empty serialization class.
struct HrilEmptyParcel : public HrilBaseParcel {
    bool ReadFromParcel(Parcel &parcel)
    {
        return true;
    }
    virtual bool Marshalling(Parcel &parcel) const override
    {
        return true;
    }
    std::shared_ptr<HrilEmptyParcel> UnMarshalling(Parcel &parcel)
    {
        return std::shared_ptr<HrilEmptyParcel>(nullptr);
    }
    void Dump(std::string, int32_t) {}
};

// Primitive type serialization class.
template<typename T>
struct HRilCommonParcel : public HrilBaseParcel {
    T data;

    HRilCommonParcel() {}
    // copy constructor
    HRilCommonParcel(const T &d) : data(d) {}
    HRilCommonParcel(const T &&d) : data(d) {}
    // Only the std::string template type is supported.
    HRilCommonParcel(const char *s)
    {
        if (s != nullptr) {
            data = s;
        }
    }
    // The std::string template type is not supported.
    HRilCommonParcel(const uint8_t *buf, size_t bufLen)
    {
        static_assert(std::is_class<T>::value == 0,
            "This constructor does not support the std::string type,"
            " please use the HRilStringParcel(const std::string &) constructor.");
        assert((bufLen % sizeof(T)) == 0);

        if (buf != nullptr) {
            data = *(T *)buf;
        } else {
            data = 0;
        }
    }
    HRilCommonParcel &operator=(const T &t)
    {
        data = t;
        return *this;
    }
    bool ReadFromParcel(Parcel &parcel)
    {
        return Read(parcel, data);
    }
    virtual bool Marshalling(Parcel &parcel) const override
    {
        return Write(parcel, data);
    }
    std::shared_ptr<HRilCommonParcel<T>> UnMarshalling(Parcel &parcel)
    {
        return std::make_shared<HRilCommonParcel<T>>();
    }
    void Dump(std::string, int32_t) {}
    virtual const char *ToString() const override
    {
        DesensitizeStringStream dss(String(), StringStream());
        dss << DSS::Dese << data;
        return *dss;
    }
};

// Basic type serialization type extension.
using HRilUint8Parcel = struct HRilCommonParcel<uint8_t>;
using HRilInt32Parcel = struct HRilCommonParcel<int32_t>;
using HRilInt64Parcel = struct HRilCommonParcel<int64_t>;
using HRilBoolParcel = struct HRilCommonParcel<bool>;
using HRilStringParcel = struct HRilCommonParcel<std::string>;

static constexpr int32_t TELEPHONY_PARCEL_MAX_COUNT = 1024;
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_BASE_PARCEL_H