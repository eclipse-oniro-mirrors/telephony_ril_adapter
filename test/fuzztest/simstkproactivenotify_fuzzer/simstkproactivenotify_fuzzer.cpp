/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "simstkproactivenotify_fuzzer.h"

#include <cstddef>
#include <cstdint>

#include "hril_manager.h"
#include "hril_notification.h"
#include "hril_sim.h"
#include "system_ability_definition.h"

using namespace OHOS::Telephony;
namespace OHOS {
constexpr int32_t SLOT_NUM = 3;
constexpr const char *NUMBER = "123";

void DoSomethingInterestingWithMyAPI(const uint8_t *data, size_t size)
{
    if (data == nullptr || size == 0) {
        return;
    }

    int32_t slotId = static_cast<HRilErrNumber>(size) % SLOT_NUM;
    struct ReportInfo reportInfo;
    reportInfo.error = static_cast<HRilErrNumber>(size);
    reportInfo.notifyId = HNOTI_SIM_STK_PROACTIVE_NOTIFY;
    char *response = const_cast<char *>(NUMBER);

    HRilManager hrilManager;
    std::shared_ptr<HRilSim> hrilSim = std::make_shared<HRilSim>(slotId, hrilManager);
    hrilSim->ProcessNotify<HRilSim>(HRIL_RESPONSE_NOTICE, &reportInfo, &response, sizeof(char));
    return;
}
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}
