/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_RIL_TEST_UTIL_H
#define OHOS_RIL_TEST_UTIL_H

#include "ril_callback_test.h"
#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_3;
inline const int32_t BANDWIDTH_HYSTERESIS_MS = 3000;
inline const int32_t BANDWIDTH_HYSTERESIS_KBPS = 50;
inline const int32_t MAX_UPLINK_LINK_BANDWIDTH[] = { 100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000 };
inline const int32_t MAX_DOWNLINK_LINK_BANDWIDTH[] = { 100, // VoIP
    500, // Web
    1000, // SD
    5000, // HD
    10000, // file
    20000, // 4K
    50000, // LTE
    100000,
    200000, // 5G
    500000, 1000000 };
inline const int32_t SYSPARA_SIZE = 128;
inline const int32_t SIM_AUTH_EAP_AKA_TYPE = 129;
inline const int32_t NR_OPTION_NSA_ONLY = 1;

class RilTestUtil {
public:
    static void WaitFor(int32_t timeoutSecond);
    static bool GetBoolResult(HdiId hdiId_);
    static int32_t GetSerialId();
    static bool IsValidSlotId(int32_t slotId);
    static bool HasVoiceCapability();
    static bool IsReady(int32_t slotId);
    static RilTestUtil &GetInstance();
    static sptr<OHOS::HDI::Ril::V1_3::IRil> GetRilInterface();
    static sptr<RilCallbackTest> GetCallback();
    void Init();

private:
    void WaitForInner(int32_t timeoutSecond);
    bool GetBoolResultInner(HdiId hdiId_);
    bool IsValidSlotIdInner(int32_t slotId);
    bool HasVoiceCapabilityInner();
    int32_t GetSerialIdInner();
    bool IsReadyInner(int32_t slotId);
    int32_t GetMaxSimCount();

private:
    int slotCount_;
    bool hasVoiceCapable_ = true;
    sptr<OHOS::HDI::Ril::V1_3::IRil> rilInterface_ = nullptr;
    sptr<RilCallbackTest> callback_;
    bool isInit_ = false;
    static RilTestUtil rilTestUtil_;
};

} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_TEST_UTIL_H
