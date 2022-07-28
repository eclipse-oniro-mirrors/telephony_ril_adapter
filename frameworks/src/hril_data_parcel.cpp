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

#include "hril_data_parcel.h"

#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
bool SetupDataCallResultInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, flag)) {
        return false;
    }
    if (!Read(parcel, reason)) {
        return false;
    }
    if (!Read(parcel, retryTime)) {
        return false;
    }
    if (!Read(parcel, cid)) {
        return false;
    }
    if (!Read(parcel, active)) {
        return false;
    }
    if (!Read(parcel, type)) {
        return false;
    }
    if (!Read(parcel, netPortName)) {
        return false;
    }
    if (!Read(parcel, address)) {
        return false;
    }
    if (!Read(parcel, dns)) {
        return false;
    }
    if (!Read(parcel, dnsSec)) {
        return false;
    }
    if (!Read(parcel, gateway)) {
        return false;
    }
    if (!Read(parcel, maxTransferUnit)) {
        return false;
    }
    if (!Read(parcel, pCscfPrimAddr)) {
        return false;
    }
    if (!Read(parcel, pCscfSecAddr)) {
        return false;
    }
    if (!Read(parcel, pduSessionId)) {
        return false;
    }
    return true;
}

bool SetupDataCallResultInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, flag)) {
        return false;
    }
    if (!Write(parcel, reason)) {
        return false;
    }
    if (!Write(parcel, retryTime)) {
        return false;
    }
    if (!Write(parcel, cid)) {
        return false;
    }
    if (!Write(parcel, active)) {
        return false;
    }
    if (!Write(parcel, type)) {
        return false;
    }
    if (!Write(parcel, netPortName)) {
        return false;
    }
    if (!Write(parcel, address)) {
        return false;
    }
    if (!Write(parcel, dns)) {
        return false;
    }
    if (!Write(parcel, dnsSec)) {
        return false;
    }
    if (!Write(parcel, gateway)) {
        return false;
    }
    if (!Write(parcel, maxTransferUnit)) {
        return false;
    }
    if (!Write(parcel, pCscfPrimAddr)) {
        return false;
    }
    if (!Write(parcel, pCscfSecAddr)) {
        return false;
    }
    if (!Write(parcel, pduSessionId)) {
        return false;
    }
    return true;
}

std::shared_ptr<SetupDataCallResultInfo> SetupDataCallResultInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<SetupDataCallResultInfo> param = std::make_shared<SetupDataCallResultInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataCallResultList::ReadFromParcel(Parcel &parcel)
{
    if (size > TELEPHONY_PARCEL_MAX_COUNT) {
        TELEPHONY_LOGE("DataCallResultList, ReadFromParcel size:%{public}d is invalid", (int)size);
        return false;
    }
    if (!Read(parcel, size)) {
        return false;
    }
    dcList.resize(size);
    for (int32_t i = 0; i < (int32_t)size; i++) {
        dcList[i].ReadFromParcel(parcel);
    }
    return true;
}

bool DataCallResultList::Marshalling(Parcel &parcel) const
{
    if (size > TELEPHONY_PARCEL_MAX_COUNT) {
        TELEPHONY_LOGE("DataCallResultList, Marshalling size:%{public}d is invalid", (int)size);
        return false;
    }
    if (!Write(parcel, size)) {
        return false;
    }
    for (int32_t i = 0; i < (int32_t)size; i++) {
        dcList[i].Marshalling(parcel);
    }
    return true;
}

std::shared_ptr<DataCallResultList> DataCallResultList::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataCallResultList> param = std::make_shared<DataCallResultList>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataProfileDataInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, serial)) {
        return false;
    }
    if (!Read(parcel, profileId)) {
        return false;
    }
    if (!Read(parcel, apn)) {
        return false;
    }
    if (!Read(parcel, protocol)) {
        return false;
    }
    if (!Read(parcel, roamingProtocol)) {
        return false;
    }
    if (!Read(parcel, verType)) {
        return false;
    }
    if (!Read(parcel, userName)) {
        return false;
    }
    if (!Read(parcel, password)) {
        return false;
    }
    return true;
}

bool DataProfileDataInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, serial)) {
        return false;
    }
    if (!Write(parcel, profileId)) {
        return false;
    }
    if (!Write(parcel, apn)) {
        return false;
    }
    if (!Write(parcel, protocol)) {
        return false;
    }
    if (!Write(parcel, roamingProtocol)) {
        return false;
    }
    if (!Write(parcel, verType)) {
        return false;
    }
    if (!Write(parcel, userName)) {
        return false;
    }
    if (!Write(parcel, password)) {
        return false;
    }
    return true;
}

std::shared_ptr<DataProfileDataInfo> DataProfileDataInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataProfileDataInfo> param = std::make_shared<DataProfileDataInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataProfilesInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, serial)) {
        return false;
    }
    if (!Read(parcel, profilesSize)) {
        return false;
    }
    profiles.resize(profilesSize);
    for (int32_t i = 0; i < profilesSize; i++) {
        profiles[i].ReadFromParcel(parcel);
    }
    if (!Read(parcel, isRoaming)) {
        return false;
    }
    return true;
}

bool DataProfilesInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, serial)) {
        return false;
    }
    if (!Write(parcel, profilesSize)) {
        return false;
    }
    for (int32_t i = 0; i < profilesSize; i++) {
        profiles[i].Marshalling(parcel);
    }
    if (!Write(parcel, isRoaming)) {
        return false;
    }
    return true;
}

std::shared_ptr<DataProfilesInfo> DataProfilesInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataProfilesInfo> param = std::make_shared<DataProfilesInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataCallInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, serial)) {
        return false;
    }
    if (!Read(parcel, radioTechnology)) {
        return false;
    }
    dataProfileInfo.ReadFromParcel(parcel);
    if (!Read(parcel, modemCognitive)) {
        return false;
    }
    if (!Read(parcel, roamingAllowed)) {
        return false;
    }
    if (!Read(parcel, isRoaming)) {
        return false;
    }
    return true;
}

bool DataCallInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, serial)) {
        return false;
    }
    if (!Write(parcel, radioTechnology)) {
        return false;
    }
    dataProfileInfo.Marshalling(parcel);
    if (!Write(parcel, modemCognitive)) {
        return false;
    }
    if (!Write(parcel, roamingAllowed)) {
        return false;
    }
    if (!Write(parcel, isRoaming)) {
        return false;
    }
    return true;
}

std::shared_ptr<DataCallInfo> DataCallInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataCallInfo> param = std::make_shared<DataCallInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataLinkBandwidthInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, serial)) {
        return false;
    }
    if (!Read(parcel, cid)) {
        return false;
    }
    if (!Read(parcel, qi)) {
        return false;
    }
    if (!Read(parcel, dlGfbr)) {
        return false;
    }
    if (!Read(parcel, ulGfbr)) {
        return false;
    }
    if (!Read(parcel, dlMfbr)) {
        return false;
    }
    if (!Read(parcel, ulMfbr)) {
        return false;
    }
    if (!Read(parcel, ulSambr)) {
        return false;
    }
    if (!Read(parcel, dlSambr)) {
        return false;
    }
    if (!Read(parcel, averagingWindow)) {
        return false;
    }
    return true;
}

bool DataLinkBandwidthInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, serial)) {
        return false;
    }
    if (!Write(parcel, cid)) {
        return false;
    }
    if (!Write(parcel, qi)) {
        return false;
    }
    if (!Write(parcel, dlGfbr)) {
        return false;
    }
    if (!Write(parcel, ulGfbr)) {
        return false;
    }
    if (!Write(parcel, dlMfbr)) {
        return false;
    }
    if (!Write(parcel, ulMfbr)) {
        return false;
    }
    if (!Write(parcel, ulSambr)) {
        return false;
    }
    if (!Write(parcel, dlSambr)) {
        return false;
    }
    if (!Write(parcel, averagingWindow)) {
        return false;
    }
    return true;
}

std::shared_ptr<DataLinkBandwidthInfo> DataLinkBandwidthInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataLinkBandwidthInfo> param = std::make_shared<DataLinkBandwidthInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataLinkBandwidthReportingRule::ReadFromParcel(Parcel &parcel)
{
    int32_t i;
    if (!Read(parcel, serial)) {
        return false;
    }
    if (!Read(parcel, rat)) {
        return false;
    }
    if (!Read(parcel, delayMs)) {
        return false;
    }
    if (!Read(parcel, delayUplinkKbps)) {
        return false;
    }
    if (!Read(parcel, delayDownlinkKbps)) {
        return false;
    }
    if (!Read(parcel, maximumUplinkKbpsSize)) {
        return false;
    }
    if (!Read(parcel, maximumDownlinkKbpsSize)) {
        return false;
    }
    maximumUplinkKbps.resize(maximumUplinkKbpsSize);
    for (i = 0; i < maximumUplinkKbpsSize; i++) {
        if (!Read(parcel, maximumUplinkKbps[i])) {
            return false;
        }
    }
    maximumDownlinkKbps.resize(maximumDownlinkKbpsSize);
    for (i = 0; i < maximumDownlinkKbpsSize; i++) {
        if (!Read(parcel, maximumDownlinkKbps[i])) {
            return false;
        }
    }
    return true;
}

bool DataLinkBandwidthReportingRule::Marshalling(Parcel &parcel) const
{
    int32_t i;
    if (!Write(parcel, serial)) {
        return false;
    }
    if (!Write(parcel, rat)) {
        return false;
    }
    if (!Write(parcel, delayMs)) {
        return false;
    }
    if (!Write(parcel, delayUplinkKbps)) {
        return false;
    }
    if (!Write(parcel, delayDownlinkKbps)) {
        return false;
    }
    if (!Write(parcel, maximumUplinkKbpsSize)) {
        return false;
    }
    if (!Write(parcel, maximumDownlinkKbpsSize)) {
        return false;
    }
    for (i = 0; i < maximumUplinkKbpsSize; i++) {
        if (!Write(parcel, maximumUplinkKbps[i])) {
            return false;
        }
    }
    for (i = 0; i < maximumDownlinkKbpsSize; i++) {
        if (!Write(parcel, maximumDownlinkKbps[i])) {
            return false;
        }
    }
    return true;
}

std::shared_ptr<DataLinkBandwidthReportingRule> DataLinkBandwidthReportingRule::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataLinkBandwidthReportingRule> param = std::make_shared<DataLinkBandwidthReportingRule>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}

bool DataPerformanceInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, performanceEnable)) {
        return false;
    }
    if (!Read(parcel, enforce)) {
        return false;
    }
    return true;
}

bool DataPerformanceInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, performanceEnable)) {
        return false;
    }
    if (!Write(parcel, enforce)) {
        return false;
    }
    return true;
}

std::shared_ptr<DataPerformanceInfo> DataPerformanceInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataPerformanceInfo> param = std::make_shared<DataPerformanceInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
} 

bool DataSleepInfo::ReadFromParcel(Parcel &parcel)
{
    if (!Read(parcel, sleepEnable)) {
        return false;
    }
    return true;
}

bool DataSleepInfo::Marshalling(Parcel &parcel) const
{
    if (!Write(parcel, sleepEnable)) {
        return false;
    }
    return true;
}

std::shared_ptr<DataSleepInfo> DataSleepInfo::UnMarshalling(Parcel &parcel)
{
    std::shared_ptr<DataSleepInfo> param = std::make_shared<DataSleepInfo>();
    if (param == nullptr || !param->ReadFromParcel(parcel)) {
        param = nullptr;
    }
    return param;
}
} // namespace Telephony
} // namespace OHOS
