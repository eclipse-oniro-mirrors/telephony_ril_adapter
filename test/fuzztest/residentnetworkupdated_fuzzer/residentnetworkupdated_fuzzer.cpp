/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "residentnetworkupdated_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <fuzzer/FuzzedDataProvider.h>

#include "hril_network.h"
#include "hril_manager.h"
#include "hril_notification.h"
#include "system_ability_definition.h"

using namespace OHOS::Telephony;
namespace OHOS {
constexpr int32_t MIN_SLOT_ID = -1;
constexpr int32_t MAX_SLOT_ID = 4;

int32_t GetRandomInt(int min, int max, const uint8_t *data, size_t size)
{
    FuzzedDataProvider fdp(data, size);
    return fdp.ConsumeIntegralInRange<int32_t>(min, max);
}

void DoSomethingInterestingWithMyAPI(const uint8_t *data, size_t size)
{
    if (data == nullptr || size == 0) {
        return;
    }
    int32_t slotId = GetRandomInt(MIN_SLOT_ID, MAX_SLOT_ID, data, size);
    const char *plmn = reinterpret_cast<const char *>(data);
    
    int32_t minErrCode = -1;
    int32_t maxErrCode = static_cast<int32_t>(HRIL_ERR_HDF_IPC_FAILURE) + 1;
    HRilErrNumber error = static_cast<HRilErrNumber>(GetRandomInt(minErrCode, maxErrCode, data, size));
    struct ReportInfo report;
    report.error = error;
    report.notifyId = HNOTI_NETWORK_RESIDENT_NETWORK_UPDATED;
    report.type = HRIL_NOTIFICATION;
    HRilManager::GetInstance().OnNetworkReport(slotId, &report, (const uint8_t *)plmn, size);
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
