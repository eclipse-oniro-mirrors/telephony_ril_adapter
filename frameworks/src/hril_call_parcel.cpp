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
    if (!Read(parcel, serial)) {
        return false;
    }
    if (!Read(parcel, address)) {
        return false;
    }
    if (!Read(parcel, clir)) {
        return false;
    }
    return true;
}

bool DialInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, serial)) {
        return false;
    }
    if (!Write(parcel, address)) {
        return false;
    }
    if (!Write(parcel, clir)) {
        return false;
    }
    return true;
}

bool CallInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, index)) {
        return false;
    }
    if (!Read(parcel, dir)) {
        return false;
    }
    if (!Read(parcel, state)) {
        return false;
    }
    if (!Read(parcel, mode)) {
        return false;
    }
    if (!Read(parcel, mpty)) {
        return false;
    }
    if (!Read(parcel, voiceDomain)) {
        return false;
    }
    if (!Read(parcel, callType)) {
        return false;
    }
    if (!Read(parcel, number)) {
        return false;
    }
    if (!Read(parcel, type)) {
        return false;
    }
    if (!Read(parcel, alpha)) {
        return false;
    }
    return true;
}

bool CallInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, index)) {
        return false;
    }
    if (!Write(parcel, dir)) {
        return false;
    }
    if (!Write(parcel, state)) {
        return false;
    }
    if (!Write(parcel, mode)) {
        return false;
    }
    if (!Write(parcel, mpty)) {
        return false;
    }
    if (!Write(parcel, voiceDomain)) {
        return false;
    }
    if (!Write(parcel, callType)) {
        return false;
    }
    if (!Write(parcel, number)) {
        return false;
    }
    if (!Write(parcel, type)) {
        return false;
    }
    if (!Write(parcel, alpha)) {
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
    if (!Read(parcel, callSize)) {
        return false;
    }
    if (!Read(parcel, flag)) {
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
    if (!Write(parcel, callSize)) {
        return false;
    }
    if (!Write(parcel, flag)) {
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

bool EmergencyInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, index)) {
        return false;
    }
    if (!Read(parcel, total)) {
        return false;
    }
    if (!Read(parcel, eccNum)) {
        return false;
    }
    if (!Read(parcel, category)) {
        return false;
    }
    if (!Read(parcel, simpresent)) {
        return false;
    }
    if (!Read(parcel, mcc)) {
        return false;
    }
    if (!Read(parcel, abnormalService)) {
        return false;
    }
    return true;
}

bool EmergencyInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, index)) {
        return false;
    }
    if (!Write(parcel, total)) {
        return false;
    }
    if (!Write(parcel, eccNum)) {
        return false;
    }
    if (!Write(parcel, category)) {
        return false;
    }
    if (!Write(parcel, simpresent)) {
        return false;
    }
    if (!Write(parcel, mcc)) {
        return false;
    }
    if (!Write(parcel, abnormalService)) {
        return false;
    }
    return true;
}

std::shared_ptr<EmergencyInfo> EmergencyInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<EmergencyInfo> param = std::make_shared<EmergencyInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void EmergencyInfo::Dump(std::string, int32_t) {}

bool EmergencyInfoList::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, callSize)) {
        return false;
    }
    if (!Read(parcel, flag)) {
        return false;
    }
    calls.resize(callSize);
    for (int32_t i = 0; i < callSize; i++) {
        calls[i].ReadFromParcel(parcel);
    }
    return true;
}

bool EmergencyInfoList::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, callSize)) {
        return false;
    }
    if (!Write(parcel, flag)) {
        return false;
    }
    for (int32_t i = 0; i < callSize; i++) {
        calls[i].Marshalling(parcel);
    }
    return true;
}

std::shared_ptr<EmergencyInfoList> EmergencyInfoList::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<EmergencyInfoList> param = std::make_shared<EmergencyInfoList>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        return nullptr;
    }
    return param;
}

void EmergencyInfoList::Dump(std::string, int32_t) {}

bool UusData::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, uusDcs)) {
        return false;
    }
    return true;
}

bool UusData::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, uusDcs)) {
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
    if (!Read(parcel, serial)) {
        return false;
    }
    if (!Read(parcel, reason)) {
        return false;
    }
    if (!Read(parcel, mode)) {
        return false;
    }
    if (!Read(parcel, number)) {
        return false;
    }
    if (!Read(parcel, classx)) {
        return false;
    }
    return true;
}

bool CallForwardSetInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, serial)) {
        return false;
    }
    if (!Write(parcel, reason)) {
        return false;
    }
    if (!Write(parcel, mode)) {
        return false;
    }
    if (!Write(parcel, number)) {
        return false;
    }
    if (!Write(parcel, classx)) {
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
    if (!Read(parcel, serial)) {
        return false;
    }
    if (!Read(parcel, result)) {
        return false;
    }
    if (!Read(parcel, status)) {
        return false;
    }
    if (!Read(parcel, classx)) {
        return false;
    }
    if (!Read(parcel, number)) {
        return false;
    }
    if (!Read(parcel, type)) {
        return false;
    }
    if (!Read(parcel, reason)) {
        return false;
    }
    if (!Read(parcel, time)) {
        return false;
    }
    return true;
}

bool CallForwardQueryResult::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, serial)) {
        return false;
    }
    if (!Write(parcel, result)) {
        return false;
    }
    if (!Write(parcel, status)) {
        return false;
    }
    if (!Write(parcel, classx)) {
        return false;
    }
    if (!Write(parcel, number)) {
        return false;
    }
    if (!Write(parcel, type)) {
        return false;
    }
    if (!Write(parcel, reason)) {
        return false;
    }
    if (!Write(parcel, time)) {
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

bool CallForwardQueryInfoList::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, callSize)) {
        return false;
    }
    if (!Read(parcel, flag)) {
        return false;
    }
    calls.resize(callSize);
    for (int32_t i = 0; i < callSize; i++) {
        calls[i].ReadFromParcel(parcel);
    }
    return true;
}

bool CallForwardQueryInfoList::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, callSize)) {
        return false;
    }
    if (!Write(parcel, flag)) {
        return false;
    }
    for (int32_t i = 0; i < callSize; i++) {
        calls[i].Marshalling(parcel);
    }
    return true;
}

std::shared_ptr<CallForwardQueryInfoList> CallForwardQueryInfoList::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<CallForwardQueryInfoList> param = std::make_shared<CallForwardQueryInfoList>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        return nullptr;
    }
    return param;
}

void CallForwardQueryInfoList::Dump(std::string, int32_t) {}

bool GetClipResult::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, result)) {
        return false;
    }
    if (!Read(parcel, action)) {
        return false;
    }
    if (!Read(parcel, clipStat)) {
        return false;
    }
    return true;
}

bool GetClipResult::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, result)) {
        return false;
    }
    if (!Write(parcel, action)) {
        return false;
    }
    if (!Write(parcel, clipStat)) {
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
    if (!Read(parcel, result)) {
        return false;
    }
    if (!Read(parcel, action)) {
        return false;
    }
    if (!Read(parcel, clirStat)) {
        return false;
    }
    return true;
}

bool GetClirResult::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, result)) {
        return false;
    }
    if (!Write(parcel, action)) {
        return false;
    }
    if (!Write(parcel, clirStat)) {
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
    if (!Read(parcel, result)) {
        return false;
    }
    if (!Read(parcel, status)) {
        return false;
    }
    if (!Read(parcel, classCw)) {
        return false;
    }
    return true;
}

bool CallWaitResult::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, result)) {
        return false;
    }
    if (!Write(parcel, status)) {
        return false;
    }
    if (!Write(parcel, classCw)) {
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
    if (!Read(parcel, result)) {
        return false;
    }
    if (!Read(parcel, status)) {
        return false;
    }
    if (!Read(parcel, classCw)) {
        return false;
    }
    return true;
}

bool CallRestrictionResult::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, result)) {
        return false;
    }
    if (!Write(parcel, status)) {
        return false;
    }
    if (!Write(parcel, classCw)) {
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

bool CallImsServiceStatus::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, smsSrvStatus)) {
        return false;
    }
    if (!Read(parcel, smsSrvRat)) {
        return false;
    }
    if (!Read(parcel, voipSrvStatus)) {
        return false;
    }
    if (!Read(parcel, voipSrvRat)) {
        return false;
    }
    if (!Read(parcel, vtSrvStatus)) {
        return false;
    }
    if (!Read(parcel, vtSrvRat)) {
        return false;
    }
    if (!Read(parcel, vsSrvStatus)) {
        return false;
    }
    if (!Read(parcel, vsSrvRat)) {
        return false;
    }
    return true;
}

bool CallImsServiceStatus::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, smsSrvStatus)) {
        return false;
    }
    if (!Write(parcel, smsSrvRat)) {
        return false;
    }
    if (!Write(parcel, voipSrvStatus)) {
        return false;
    }
    if (!Write(parcel, voipSrvRat)) {
        return false;
    }
    if (!Write(parcel, vtSrvStatus)) {
        return false;
    }
    if (!Write(parcel, vtSrvRat)) {
        return false;
    }
    if (!Write(parcel, vsSrvStatus)) {
        return false;
    }
    if (!Write(parcel, vsSrvRat)) {
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

bool UssdInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, n)) {
        return false;
    }
    if (!Read(parcel, str)) {
        return false;
    }
    if (!Read(parcel, dcs)) {
        return false;
    }
    return true;
}

bool UssdInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, n)) {
        return false;
    }
    if (!Write(parcel, str)) {
        return false;
    }
    if (!Write(parcel, dcs)) {
        return false;
    }
    return true;
}

std::shared_ptr<UssdInfo> UssdInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<UssdInfo> param = std::make_shared<UssdInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void UssdInfo::Dump(std::string, int32_t) {}

bool UssdNoticeInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, m)) {
        return false;
    }
    if (!Read(parcel, str)) {
        return false;
    }
    return true;
}

bool UssdNoticeInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, m)) {
        return false;
    }
    if (!Write(parcel, str)) {
        return false;
    }
    return true;
}

std::shared_ptr<UssdNoticeInfo> UssdNoticeInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<UssdNoticeInfo> param = std::make_shared<UssdNoticeInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void UssdNoticeInfo::Dump(std::string, int32_t) {}

bool SsNoticeInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, serviceType)) {
        return false;
    }
    if (!Read(parcel, requestType)) {
        return false;
    }
    if (!Read(parcel, serviceClass)) {
        return false;
    }
    if (!Read(parcel, result)) {
        return false;
    }
    return true;
}

bool SsNoticeInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, serviceType)) {
        return false;
    }
    if (!Write(parcel, requestType)) {
        return false;
    }
    if (!Write(parcel, serviceClass)) {
        return false;
    }
    if (!Write(parcel, result)) {
        return false;
    }
    return true;
}

std::shared_ptr<SsNoticeInfo> SsNoticeInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SsNoticeInfo> param = std::make_shared<SsNoticeInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void SsNoticeInfo::Dump(std::string, int32_t) {}

bool SrvccStatus::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, status)) {
        return false;
    }
    return true;
}

bool SrvccStatus::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, status)) {
        return false;
    }
    return true;
}

std::shared_ptr<SrvccStatus> SrvccStatus::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SrvccStatus> param = std::make_shared<SrvccStatus>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void SrvccStatus::Dump(std::string, int32_t) {}

bool RingbackVoice::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, status)) {
        return false;
    }
    return true;
}

bool RingbackVoice::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, status)) {
        return false;
    }
    return true;
}

std::shared_ptr<RingbackVoice> RingbackVoice::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<RingbackVoice> param = std::make_shared<RingbackVoice>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

void RingbackVoice::Dump(std::string, int32_t) {}
} // namespace Telephony
} // namespace OHOS
