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

#include "parameter.h"
#include "ril_test_util.h"

namespace OHOS {
namespace Telephony {
void RilTestUtil::WaitFor(int32_t timeoutSecond)
{
    g_callback.WaitFor(WAIT_TIME_SECOND);
}

bool RilTestUtil::GetBoolResult(HdiId hdiId_)
{
    return g_callback.GetBoolResult(hdiId_);
}

bool RilTestUtil::HasVoiceCapability()
{
    char retValue[VOICECALL_CAP_VAL_LEN + 1] = { "true" };
    int retLen = GetParameter(KEY_VOICECALL_CAP.c_str(), "true", retValue, VOICECALL_CAP_VAL_LEN);
    TELEPHONY_LOGI("HasVoiceCapability retValue %{public}s, retLen %{public}d", retValue, retLen);
    if (strcmp(retValue, "false") == 0) {
        return false;
    }
    return true;
}

int32_t RilTestUtil::GetSerialId()
{
    return g_callback.GetSerialId();
}

bool RilTestUtil::IsValidSlotId(int32_t slotId)
{
    if ((slotId < 0) || (slotId >= g_slotCount_)) {
        TELEPHONY_LOGE("slotId is invalid, slotId = %{public}d", slotId);
        return false;
    }
    TELEPHONY_LOGD("slotId is valid, slotId = %{public}d", slotId);
    return true;
}

bool RilTestUtil::IsReady(int32_t slotId)
{
    if (g_rilInterface == nullptr || !g_hasVoiceCapable || !IsValidSlotId(slotId)) {
        return false;
    }
    return g_callback.IsReady(slotId);
}

int32_t RilTestUtil::GetMaxSimCount()
{
    char simSlotCount[SYSPARA_SIZE] = { 0 };
    GetParameter(TEL_SIM_SLOT_COUNT, DEFAULT_SLOT_COUNT, simSlotCount, SYSPARA_SIZE);
    int32_t slotCount = std::atoi(simSlotCount);
    return slotCount;
}
} // namespace Telephony
} // namespace OHOS