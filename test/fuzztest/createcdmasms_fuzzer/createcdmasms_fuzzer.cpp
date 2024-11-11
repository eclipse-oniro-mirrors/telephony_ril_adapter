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

#include "createcdmasms_fuzzer.h"

#include <cstddef>
#include <cstdint>
#include <fuzzer/FuzzedDataProvider.h>

#include "hril_manager.h"
#include "hril_sms.h"
#include "system_ability_definition.h"

using namespace OHOS::Telephony;
namespace OHOS {

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
    int32_t maxCnt = HRilManager::GetInstance().GetMaxSimSlotCount();
    int32_t slotId = GetRandomInt(0, maxCnt, data, size);
    OHOS::HDI::Ril::V1_1::SmsMessageIOInfo message;
    std::string pdu(reinterpret_cast<const char *>(data), size);
    message.pdu = pdu;
    HRilManager::GetInstance().AddCdmaSimMessage(slotId, size, message);
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
