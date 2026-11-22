/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "modemradiostateupdated_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

#include "hril_manager.h"
#include "hril_modem.h"
#include "hril_notification.h"
#include "system_ability_definition.h"

using namespace OHOS::Telephony;
namespace OHOS {
constexpr int32_t MIN_SLOT_ID = -1;
constexpr int32_t MAX_SLOT_ID = 4;

void DoSomethingInterestingWithMyAPI(const uint8_t *data, size_t size)
{
    // A single FuzzedDataProvider is created per input: each semantic field
    // consumes independent bytes instead of re-reading the same tail byte.
    FuzzedDataProvider fdp(data, size);
    int32_t slotId = fdp.ConsumeIntegralInRange<int32_t>(MIN_SLOT_ID, MAX_SLOT_ID);

    int32_t minErrCode = -1;
    int32_t maxErrCode = static_cast<int32_t>(HRIL_ERR_HDF_IPC_FAILURE) + 1;
    struct ReportInfo report = {};
    report.error = static_cast<HRilErrNumber>(
        fdp.ConsumeIntegralInRange<int32_t>(minErrCode, maxErrCode));
    report.notifyId = HNOTI_MODEM_RADIO_STATE_UPDATED;
    report.type = HRIL_NOTIFICATION;

    // All remaining bytes form the report payload. An empty payload yields a
    // null response covering the null branch, 3/5-byte payloads cover the
    // responseLen % sizeof(int32_t) != 0 branch, and 4-byte payloads carry an
    // arbitrary radioState through the normal notification path.
    std::vector<uint8_t> payload = fdp.ConsumeRemainingBytes<uint8_t>();
    const uint8_t *response = payload.empty() ? nullptr : payload.data();
    HRilManager::GetInstance().OnModemReport(slotId, &report, response, payload.size());
}
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    /* Run your code on data */
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}
