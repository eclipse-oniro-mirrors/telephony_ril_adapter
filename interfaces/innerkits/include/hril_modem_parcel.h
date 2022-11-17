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
    int32_t serial;
    int32_t gsmIndex; /* Index of Global System for Mobile Communications */
    bool flag;
    int32_t arg1;
    int32_t arg2;
    std::string strTmp;
};

struct VoiceRadioTechnology {
    HRilSrvStatus srvStatus;
    HRilSrvDomain srvDomain;
    HRilRoamStatus roamStatus;
    HRilSimStatus simStatus;
    HRilSimLockStatus lockStatus;
    HRilSysMode sysMode;
    std::string sysModeName;
    HRilRadioTech actType;
    std::string actName;
    int64_t flag;

    VoiceRadioTechnology() = default;
    VoiceRadioTechnology(const HRilVoiceRadioInfo &hrilVoiceRadioInfo);
    VoiceRadioTechnology &operator=(const HRilVoiceRadioInfo &hrilVoiceRadioInfo);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_COMMON_PARCEL_H
