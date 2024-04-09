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
namespace {
const std::string KEY_VOICECALL_CAP = "const.telephony.voice.capable";
const char *TEL_SIM_SLOT_COUNT = "const.telephony.slotCount";
const char *DEFAULT_SLOT_COUNT = "1";
const int32_t VOICECALL_CAP_VAL_LEN = 6;
}

RilTestUtil RilTestUtil::rilTestUtil_;

void RilTestUtil::WaitFor(int32_t timeoutSecond)
{
    rilTestUtil_.WaitForInner(WAIT_TIME_SECOND);
}

bool RilTestUtil::GetBoolResult(HdiId hdiId_)
{
    return rilTestUtil_.GetBoolResultInner(hdiId_);
}

bool RilTestUtil::HasVoiceCapability()
{
    return rilTestUtil_.HasVoiceCapabilityInner();
}

int32_t RilTestUtil::GetSerialId()
{
    return rilTestUtil_.GetSerialIdInner();
}

bool RilTestUtil::IsValidSlotId(int32_t slotId)
{
    return rilTestUtil_.IsValidSlotIdInner(slotId);
}

bool RilTestUtil::IsReady(int32_t slotId)
{
    return rilTestUtil_.IsReadyInner(slotId);
}

int32_t RilTestUtil::GetMaxSimCount()
{
    char simSlotCount[SYSPARA_SIZE] = { 0 };
    GetParameter(TEL_SIM_SLOT_COUNT, DEFAULT_SLOT_COUNT, simSlotCount, SYSPARA_SIZE);
    int32_t slotCount = std::atoi(simSlotCount);
    return slotCount;
}

void RilTestUtil::Init()
{
    if (isInit_) {
        TELEPHONY_LOGE("RilTestUtil has init");
        return;
    }
    rilInterface_ = OHOS::HDI::Ril::V1_3::IRil::Get();
    hasVoiceCapable_ = HasVoiceCapability();
    slotCount_ = GetMaxSimCount();
    if (rilInterface_ == nullptr) {
        TELEPHONY_LOGE("RilTestUtil has init");
        return;
    }
    callback_ = new RilCallbackTest();
    rilInterface_->SetCallback1_3(callback_);
    rilInterface_->GetSimStatus(SLOTID_1, RilTestUtil::GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    rilInterface_->GetSimStatus(SLOTID_2, RilTestUtil::GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    isInit_ = true;
}

void RilTestUtil::WaitForInner(int32_t timeoutSecond)
{
    if (callback_ == nullptr) {
        TELEPHONY_LOGE("callback_ is null");
        return;
    }
    callback_->WaitFor(WAIT_TIME_SECOND);
}

bool RilTestUtil::GetBoolResultInner(HdiId hdiId_)
{
    if (callback_ == nullptr) {
        TELEPHONY_LOGE("callback_ is null");
        return false;
    }
    return callback_->GetBoolResult(hdiId_);
}

int32_t RilTestUtil::GetSerialIdInner()
{
    if (callback_ == nullptr) {
        TELEPHONY_LOGE("callback_ is null");
        return 0;
    }
    return callback_->GetSerialId();
}

bool RilTestUtil::HasVoiceCapabilityInner()
{
    char retValue[VOICECALL_CAP_VAL_LEN + 1] = { "true" };
    int retLen = GetParameter(KEY_VOICECALL_CAP.c_str(), "true", retValue, VOICECALL_CAP_VAL_LEN);
    TELEPHONY_LOGI("HasVoiceCapability retValue %{public}s, retLen %{public}d", retValue, retLen);
    if (strcmp(retValue, "false") == 0) {
        return false;
    }
    return true;
}

bool RilTestUtil::IsValidSlotIdInner(int32_t slotId)
{
    if ((slotId < 0) || (slotId >= slotCount_)) {
        TELEPHONY_LOGE("slotId is invalid, slotId = %{public}d", slotId);
        return false;
    }
    TELEPHONY_LOGD("slotId is valid, slotId = %{public}d", slotId);
    return true;
}

bool RilTestUtil::IsReadyInner(int32_t slotId)
{
    if (rilInterface_ == nullptr || !hasVoiceCapable_ || !IsValidSlotId(slotId)) {
        return false;
    }
    if (callback_ == nullptr) {
        TELEPHONY_LOGE("callback_ is null");
        return false;
    }
    return callback_->IsReady(slotId);
}

sptr<OHOS::HDI::Ril::V1_3::IRil> RilTestUtil::GetRilInterface()
{
    return rilTestUtil_.rilInterface_;
}

sptr<RilCallbackTest> RilTestUtil::GetCallback()
{
    return rilTestUtil_.callback_;
}

RilTestUtil &RilTestUtil::GetInstance()
{
    return rilTestUtil_;
}

} // namespace Telephony
} // namespace OHOS
