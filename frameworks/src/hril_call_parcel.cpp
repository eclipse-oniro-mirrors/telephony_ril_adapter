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
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseString(parcel, address)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, clir)) {
        return false;
    }
    return true;
}

bool DialInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseString(parcel, address)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, clir)) {
        return false;
    }
    return true;
}

bool CallInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, index)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, dir)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, state)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, mode)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, mpty)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, voiceDomain)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, callType)) {
        return false;
    }
    if (!ReadBaseString(parcel, number)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, type)) {
        return false;
    }
    if (!ReadBaseString(parcel, alpha)) {
        return false;
    }
    return true;
}

bool CallInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, index)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, dir)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, state)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, mode)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, mpty)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, voiceDomain)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, callType)) {
        return false;
    }
    if (!WriteBaseString(parcel, number)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, type)) {
        return false;
    }
    if (!WriteBaseString(parcel, alpha)) {
        return false;
    }
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

void CallInfo::Dump(std::string, int32_t) {}

bool CallInfoList::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, callSize)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, flag)) {
        return false;
    }
    calls.resize(callSize);
    for (int32_t i = 0; i < callSize; i++) {
        calls[i].ReadFromParcel(parcel);
    }
    return true;
}

bool CallInfoList::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, callSize)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, flag)) {
        return false;
    }
    for (int32_t i = 0; i < callSize; i++) {
        calls[i].Marshalling(parcel);
    }
    return true;
}

std::shared_ptr<CallInfoList> CallInfoList::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallInfoList> param = std::make_shared<CallInfoList>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        return nullptr;
    }
    return param;
}

void CallInfoList::Dump(std::string, int32_t) {}

bool UusData::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, uusDcs)) {
        return false;
    }
    return true;
}

bool UusData::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, uusDcs)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, reason)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, mode)) {
        return false;
    }
    if (!ReadBaseString(parcel, number)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, classx)) {
        return false;
    }
    return true;
}

bool CallForwardSetInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, reason)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, mode)) {
        return false;
    }
    if (!WriteBaseString(parcel, number)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, classx)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, serial)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, result)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, status)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, classx)) {
        return false;
    }
    if (!ReadBaseString(parcel, number)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, type)) {
        return false;
    }
    return true;
}

bool CallForwardQueryResult::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, serial)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, result)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, status)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, classx)) {
        return false;
    }
    if (!WriteBaseString(parcel, number)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, type)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, result)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, action)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, clipStat)) {
        return false;
    }
    return true;
}

bool GetClipResult::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, result)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, action)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, clipStat)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, result)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, action)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, clirStat)) {
        return false;
    }
    return true;
}

bool GetClirResult::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, result)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, action)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, clirStat)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, result)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, status)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, classCw)) {
        return false;
    }
    return true;
}

bool CallWaitResult::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, result)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, status)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, classCw)) {
        return false;
    }
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
    if (!ReadBaseInt32(parcel, result)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, status)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, classCw)) {
        return false;
    }
    return true;
}

bool CallRestrictionResult::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, result)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, status)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, classCw)) {
        return false;
    }
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

bool CallCringInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseString(parcel, type)) {
        return false;
    }
    if (!ReadBaseString(parcel, pdpType)) {
        return false;
    }
    if (!ReadBaseString(parcel, pdpAddress)) {
        return false;
    }
    if (!ReadBaseString(parcel, l2p)) {
        return false;
    }
    if (!ReadBaseString(parcel, apn)) {
        return false;
    }
    return true;
}

bool CallCringInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseString(parcel, type)) {
        return false;
    }
    if (!WriteBaseString(parcel, pdpType)) {
        return false;
    }
    if (!WriteBaseString(parcel, pdpAddress)) {
        return false;
    }
    if (!WriteBaseString(parcel, l2p)) {
        return false;
    }
    if (!WriteBaseString(parcel, apn)) {
        return false;
    }
    return true;
}

std::shared_ptr<CallCringInfo> CallCringInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallCringInfo> param = std::make_shared<CallCringInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallCringInfo::Dump(std::string, int32_t) {}

bool CallWaitInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseString(parcel, number)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, type)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, businessClass)) {
        return false;
    }
    return true;
}

bool CallWaitInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseString(parcel, number)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, type)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, businessClass)) {
        return false;
    }
    return true;
}

std::shared_ptr<CallWaitInfo> CallWaitInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallWaitInfo> param = std::make_shared<CallWaitInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallWaitInfo::Dump(std::string, int32_t) {}

bool CallConnectInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, callId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, type)) {
        return false;
    }
    return true;
}

bool CallConnectInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, callId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, type)) {
        return false;
    }
    return true;
}

std::shared_ptr<CallConnectInfo> CallConnectInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallConnectInfo> param = std::make_shared<CallConnectInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallConnectInfo::Dump(std::string, int32_t) {}

bool CallEndInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, callId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, duration)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, noCliCause)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, ccCause)) {
        return false;
    }
    return true;
}

bool CallEndInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, callId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, duration)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, noCliCause)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, ccCause)) {
        return false;
    }
    return true;
}

std::shared_ptr<CallEndInfo> CallEndInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallEndInfo> param = std::make_shared<CallEndInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallEndInfo::Dump(std::string, int32_t) {}

bool CallStatusInfo::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, callId)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, status)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, voiceDomain)) {
        return false;
    }
    return true;
}

bool CallStatusInfo::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, callId)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, status)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, voiceDomain)) {
        return false;
    }
    return true;
}

std::shared_ptr<CallStatusInfo> CallStatusInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallStatusInfo> param = std::make_shared<CallStatusInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallStatusInfo::Dump(std::string, int32_t) {}

bool CallImsServiceStatus::ReadFromParcel(Parcel &parcel)
{
    if (!ReadBaseInt32(parcel, smsSrvStatus)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, smsSrvRat)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, voipSrvStatus)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, voipSrvRat)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, vtSrvStatus)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, vtSrvRat)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, vsSrvStatus)) {
        return false;
    }
    if (!ReadBaseInt32(parcel, vsSrvRat)) {
        return false;
    }
    return true;
}

bool CallImsServiceStatus::Marshalling(Parcel &parcel) const
{
    if (!WriteBaseInt32(parcel, smsSrvStatus)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, smsSrvRat)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, voipSrvStatus)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, voipSrvRat)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, vtSrvStatus)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, vtSrvRat)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, vsSrvStatus)) {
        return false;
    }
    if (!WriteBaseInt32(parcel, vsSrvRat)) {
        return false;
    }
    return true;
}

std::shared_ptr<CallImsServiceStatus> CallImsServiceStatus::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallImsServiceStatus> param = std::make_shared<CallImsServiceStatus>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void CallImsServiceStatus::Dump(std::string, int32_t) {}
} // namespace Telephony
} // namespace OHOS
