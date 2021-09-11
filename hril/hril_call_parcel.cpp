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

#include "hril_call_parcel.h"
#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
std::shared_ptr<DialInfo> DialInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DialInfo> param = std::make_shared<DialInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DialInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, address);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, clir);
    return true;
}

bool DialInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, address);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, clir);
    return true;
}

bool CallInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, index);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, dir);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, state);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mpty);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, number);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, type);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, alpha);
    return true;
}

bool CallInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, index);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, dir);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, state);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mpty);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, number);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, type);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, alpha);
    return true;
}

std::shared_ptr<CallInfo> CallInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallInfo> param = std::make_shared<CallInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool CallInfoList::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, callSize);
    calls.resize(callSize);
    for (int32_t i = 0; i < callSize; i++) {
        calls[i].ReadFromParcel(parcel);
    }
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, flag);
    return true;
}

bool CallInfoList::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, callSize);
    for (int32_t i = 0; i < callSize; i++) {
        calls[i].Marshalling(parcel);
    }
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, flag);
    return true;
}

std::shared_ptr<CallInfoList> CallInfoList::UnMarshalling(Parcel &parcel)
{
    return nullptr;
}

bool UusData::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, uusDcs);
    return true;
}

bool UusData::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, uusDcs);
    return true;
}

std::shared_ptr<UusData> UusData::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<UusData> param = std::make_shared<UusData>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void UusData::Dump(std::string, int32_t) {}

bool CallForwardSetInfo::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reason);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, number);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classx);
    return true;
}

bool CallForwardSetInfo::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, reason);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, mode);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, number);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classx);
    return true;
}

std::shared_ptr<CallForwardSetInfo> CallForwardSetInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallForwardSetInfo> param = std::make_shared<CallForwardSetInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallForwardSetInfo::Dump(std::string, int32_t) {}

bool CallForwardQueryResult::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classx);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, number);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, type);
    return true;
}

bool CallForwardQueryResult::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, serial);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classx);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(String, parcel, number);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, type);
    return true;
}

std::shared_ptr<CallForwardQueryResult> CallForwardQueryResult::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallForwardQueryResult> param = std::make_shared<CallForwardQueryResult>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallForwardQueryResult::Dump(std::string, int32_t) {}

bool GetClipResult::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, action);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, clipStat);
    return true;
}

bool GetClipResult::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, action);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, clipStat);
    return true;
}

std::shared_ptr<GetClipResult> GetClipResult::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<GetClipResult> param = std::make_shared<GetClipResult>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void GetClipResult::Dump(std::string, int32_t) {}

bool GetClirResult::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, action);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, clirStat);
    return true;
}

bool GetClirResult::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, action);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, clirStat);
    return true;
}

std::shared_ptr<GetClirResult> GetClirResult::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<GetClirResult> param = std::make_shared<GetClirResult>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void GetClirResult::Dump(std::string, int32_t) {}

bool CallWaitResult::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classCw);
    return true;
}

bool CallWaitResult::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classCw);
    return true;
}

std::shared_ptr<CallWaitResult> CallWaitResult::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallWaitResult> param = std::make_shared<CallWaitResult>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallWaitResult::Dump(std::string, int32_t) {}

bool CallRestrictionResult::ReadFromParcel(Parcel &parcel)
{
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    READ_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classCw);
    return true;
}

bool CallRestrictionResult::Marshalling(Parcel &parcel) const
{
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, result);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, status);
    WRITE_PARCEL_AND_RETURN_FALSE_IF_FAIL(Int32, parcel, classCw);
    return true;
}

std::shared_ptr<CallRestrictionResult> CallRestrictionResult::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallRestrictionResult> param = std::make_shared<CallRestrictionResult>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallRestrictionResult::Dump(std::string, int32_t) {}
} // namespace Telephony
} // namespace OHOS
