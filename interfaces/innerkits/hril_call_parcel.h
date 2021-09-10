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

#ifndef OHOS_RIL_CALL_PARCEL_H
#define OHOS_RIL_CALL_PARCEL_H

#include "parcel.h"
#include "hril_types.h"

namespace OHOS {
namespace Telephony {
/* From 3GPP TS 27.007 V4.3.0 (2001-12) 7.7, AT + ATD */
struct DialInfo : public Parcelable {
    int32_t serial;
    std::string address;
    int32_t clir; /* Calling Line Identification Restriction. From TS 27.007 V3.4.0 (2000-03) */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<DialInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct UusData : public Parcelable {
    int32_t uusDcs; /* Cell Broadcast Data Coding Scheme(default 0). */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<UusData> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct GetClipResult : public Parcelable {
    int32_t result; /* query results */
    int32_t action; /* parameter sets/shows the result code presentation status in the TA */
    int32_t clipStat; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<GetClipResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct GetClirResult : public Parcelable {
    int32_t result; /* query results */
    int32_t action; /* parameter sets/shows the result code presentation status in the TA */
    int32_t clirStat; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<GetClirResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallWaitResult : public Parcelable {
    int32_t result; /* query results */
    int32_t status; /* parameter sets/shows the result code presentation status in the TA */
    int32_t classCw; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallWaitResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallRestrictionResult : public Parcelable {
    int32_t result; /* query results */
    int32_t status; /* parameter sets/shows the result code presentation status in the TA */
    int32_t classCw; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallRestrictionResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallInfo : public Parcelable {
    int32_t index;
    int32_t dir;
    int32_t state;
    int32_t mode;
    int32_t mpty;
    std::string number;
    int32_t type;
    std::string alpha;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallInfoList : public Parcelable {
    int32_t callSize;
    std::vector<CallInfo> calls;
    int32_t flag;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallInfoList> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallForwardSetInfo : public Parcelable {
    int32_t serial;
    int32_t reason;
    int32_t mode;
    std::string number;
    int32_t classx;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallForwardSetInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};
struct CallForwardQueryResult : public Parcelable {
    int32_t serial;
    int32_t result; /* query results */
    int32_t status;
    int32_t classx;
    std::string number;
    int32_t type;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallForwardQueryResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_CALL_PARCEL_H