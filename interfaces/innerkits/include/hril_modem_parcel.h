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

#ifndef OHOS_RIL_MODEM_PARCEL_H
#define OHOS_RIL_MODEM_PARCEL_H

#include "hril_base_parcel.h"

namespace OHOS {
namespace Telephony {
struct UniInfo {
    int32_t serial = 0;
    int32_t gsmIndex = 0; /* Index of Global System for Mobile Communications */
    bool flag = false;
    int32_t arg1 = 0;
    int32_t arg2 = 0;
    std::string strTmp = "";
};

struct VoiceRadioTechnology {
    HRilSrvStatus srvStatus = HRilSrvStatus::HRIL_NO_SRV_SERVICE;
    HRilSrvDomain srvDomain = HRilSrvDomain::HRIL_NO_DOMAIN_SERVICE;
    HRilRoamStatus roamStatus = HRilRoamStatus::HRIL_ROAM_UNKNOWN;
    HRilSimStatus simStatus = HRilSimStatus::HRIL_USIM_INVALID;
    HRilSimLockStatus lockStatus = HRilSimLockStatus::HRIL_SIM_CARD_UNLOCK;
    HRilSysMode sysMode = HRilSysMode::HRIL_NO_SYSMODE_SERVICE;
    std::string sysModeName = "";
    HRilRadioTech actType = HRilRadioTech::RADIO_TECHNOLOGY_UNKNOWN;
    std::string actName = "";
    int64_t flag = 0;

    VoiceRadioTechnology() = default;
    VoiceRadioTechnology(const HRilVoiceRadioInfo &hrilVoiceRadioInfo);
    VoiceRadioTechnology &operator=(const HRilVoiceRadioInfo &hrilVoiceRadioInfo);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_COMMON_PARCEL_H
