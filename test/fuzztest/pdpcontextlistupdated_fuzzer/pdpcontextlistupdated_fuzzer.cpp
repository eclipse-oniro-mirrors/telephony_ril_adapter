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

#include "pdpcontextlistupdated_fuzzer.h"

#include <cstddef>
#include <cstdint>

#include "hril_data.h"
#include "hril_manager.h"
#include "hril_notification.h"
#include "system_ability_definition.h"

using namespace OHOS::Telephony;
namespace OHOS {
constexpr int32_t SLOT_NUM = 2;
constexpr int32_t REASON_TYPE = 2;
constexpr int32_t RETRY_TIME = 2;
constexpr int32_t ACTIVE_NUM = 2;
constexpr int32_t KILO_BIT = 1000;
constexpr const char *NUMBER = "123";

void DoSomethingInterestingWithMyAPI(const uint8_t *data, size_t size)
{
    if (data == nullptr || size == 0) {
        return;
    }

    int32_t slotId = static_cast<int32_t>(*data % SLOT_NUM);

    HRilDataCallResponse response;
    int32_t offset = 0;
    response.reason = static_cast<int32_t>(*data + offset) % REASON_TYPE;
    offset += sizeof(int32_t);
    response.retryTime = static_cast<int32_t>(*data + offset) % RETRY_TIME;
    offset += sizeof(int32_t);
    response.cid = static_cast<int32_t>(*data + offset);
    offset += sizeof(int32_t);
    response.active = static_cast<int32_t>(*data + offset) % ACTIVE_NUM;
    offset += sizeof(int32_t);
    response.type = const_cast<char *>(NUMBER);
    response.netPortName = const_cast<char *>(NUMBER);
    response.address = const_cast<char *>(NUMBER);
    response.dns = const_cast<char *>(NUMBER);
    response.dnsSec = const_cast<char *>(NUMBER);
    response.gateway = const_cast<char *>(NUMBER);
    response.maxTransferUnit = static_cast<int32_t>(*data + offset);
    offset += sizeof(int32_t);
    response.pCscfPrimAddr = const_cast<char *>(NUMBER);
    response.pCscfSecAddr = const_cast<char *>(NUMBER);
    response.pduSessionId = static_cast<int32_t>(*data + offset);
    offset += sizeof(int32_t);

    struct ReportInfo report;
    report.error = static_cast<HRilErrNumber>(size);
    report.notifyId = HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED;
    report.type = HRIL_NOTIFICATION;
    HRilManager::GetInstance().OnDataReport(slotId, &report, (const uint8_t *)&response, sizeof(HRilDataCallResponse));

    HRilDataLinkCapability linkCapability;
    linkCapability.primaryDownlinkKbps = static_cast<int32_t>(*data + offset) * KILO_BIT;
    offset += sizeof(int32_t);
    linkCapability.primaryUplinkKbps = static_cast<int32_t>(*data + offset) * KILO_BIT;
    offset += sizeof(int32_t);
    linkCapability.secondaryDownlinkKbps = static_cast<int32_t>(*data + offset) * KILO_BIT;
    offset += sizeof(int32_t);
    linkCapability.secondaryUplinkKbps = static_cast<int32_t>(*data + offset) * KILO_BIT;
    report.notifyId = HNOTI_DATA_LINK_CAPABILITY_UPDATED;
    HRilManager::GetInstance().OnDataReport(
        slotId, &report, (const uint8_t *)&linkCapability, sizeof(HRilDataLinkCapability));
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
