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
/**
 * @brief Defines the common information.
 */
struct UniInfo {
    /** Request serial number. */
    int32_t serial = 0;

    /** Index of Global System for Mobile Communications. */
    int32_t gsmIndex = 0;

    /** Common information ID. */
    bool flag = false;

    /** Parameter 1. */
    int32_t arg1 = 0;

    /** Parameter 2. */
    int32_t arg2 = 0;

    /** Temporary string. */
    std::string strTmp = "";
};

/**
 * @brief Defines the radio access technology of the CS domain.
 */
struct VoiceRadioTechnology {
    /** System service status. */
    HRilSrvStatus srvStatus = HRilSrvStatus::HRIL_NO_SRV_SERVICE;

    /** System service domain. */
    HRilSrvDomain srvDomain = HRilSrvDomain::HRIL_NO_DOMAIN_SERVICE;

    /** Roaming status. */
    HRilRoamStatus roamStatus = HRilRoamStatus::HRIL_ROAM_UNKNOWN;

    /** Sim card status. */
    HRilSimStatus simStatus = HRilSimStatus::HRIL_USIM_INVALID;

    /** Sim card lock status. */
    HRilSimLockStatus lockStatus = HRilSimLockStatus::HRIL_SIM_CARD_UNLOCK;

    /** System mode. */
    HRilSysMode sysMode = HRilSysMode::HRIL_NO_SYSMODE_SERVICE;

    /** String corresponding to the system mode. */
    std::string sysModeName = "";

    /** Radio access technology type. For details, see {@link RilRadioTech}. */
    HRilRadioTech actType = HRilRadioTech::RADIO_TECHNOLOGY_UNKNOWN;

    /** String corresponding to the radio access technology type. */
    std::string actName = "";

    /** Radio access technology ID. */
    int64_t flag = 0;

    VoiceRadioTechnology() = default;
    VoiceRadioTechnology(const HRilVoiceRadioInfo &hrilVoiceRadioInfo);
    VoiceRadioTechnology &operator=(const HRilVoiceRadioInfo &hrilVoiceRadioInfo);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_COMMON_PARCEL_H
