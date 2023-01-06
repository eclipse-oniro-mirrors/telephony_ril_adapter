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

#include "hril_timer_callback.h"

#include <cerrno>
#include <fcntl.h>
#include <csignal>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

#include "securec.h"

#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
void HRilTimerCallback::FdTriggerCallback(int32_t fd, int16_t events, std::shared_ptr<void> param)
{
    int32_t ret;
    int8_t buff[READ_FD_BUFF_SIZE];

    do {
        ret = read(triggerReadFd_, &buff, sizeof(buff));
    } while (ret > 0 || (ret < 0 && errno == EINTR));
}

void HRilTimerCallback::TimerCallback(int32_t fd, int16_t events, std::shared_ptr<void> param)
{
    HRilTimerCallbackMessage *pCbMsg = (HRilTimerCallbackMessage *)(param.get());
    if (pCbMsg == nullptr) {
        TELEPHONY_LOGE("Argument conversion failed. pCbMsg is nullptr!");
        return;
    }
    if (pCbMsg->func != nullptr) {
        pCbMsg->func(pCbMsg->param);
    } else {
        TELEPHONY_LOGE("HRilTimerCallbackMessage func is nullptr");
    }
}

void HRilTimerCallback::OnTriggerEvent()
{
    if (std::this_thread::get_id() == eventLoopTid_) {
        // Write operations are prohibited in read threads.
        TELEPHONY_LOGE("Currently in a read thread.");
        return;
    }
    int ret;
    do {
        ret = write(triggerWriteFd_, " ", 1);
    } while (ret < 0 && errno == EINTR);
}

std::shared_ptr<HRilTimerCallbackMessage> HRilTimerCallback::HRilSetTimerCallbackInfo(
    HRilCallbackFun func, uint8_t *param, const struct timeval *tv)
{
    struct timeval timeout;
    std::shared_ptr<HRilTimerCallbackMessage> pCbMsg = std::make_shared<HRilTimerCallbackMessage>();
    if (event_ == nullptr || pCbMsg == nullptr) {
        TELEPHONY_LOGE("HRilSetTimerCallbackInfo event_ or pCbMsg is nullptr");
        return nullptr;
    }
    pCbMsg->func = func;
    pCbMsg->param = param;

    if (tv == NULL) {
        (void)memset_s(&timeout, sizeof(timeout), 0, sizeof(timeout));
    } else {
        (void)memcpy_s(&timeout, sizeof(timeout), tv, sizeof(timeout));
    }

    HRilEventMessage eventMsg = { 0 };
    auto funcCallback = std::bind(
        &HRilTimerCallback::TimerCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    event_->SetTimerEvent(eventMsg, event_->IVNALID_FD, false, funcCallback, pCbMsg);
    event_->AddTimerEvent(eventMsg, timeout);
    OnTriggerEvent();
    return pCbMsg;
}

void HRilTimerCallback::EventLoop()
{
    TELEPHONY_LOGI("EventLoop start");
    eventLoopTid_ = std::this_thread::get_id();
    event_ = std::make_unique<HRilEvent>();
    event_->TimerEventInit();

    int32_t pipedes[PIPE_SIZE_MAX];
    int32_t ret = pipe(pipedes);
    if (ret < 0) {
        TELEPHONY_LOGE("Call pipe() is failed, errno:%{public}d", errno);
        return;
    }

    triggerReadFd_ = pipedes[0];
    triggerWriteFd_ = pipedes[1];

    fcntl(triggerReadFd_, F_SETFL, O_NONBLOCK);
    auto func = std::bind(&HRilTimerCallback::FdTriggerCallback, this, std::placeholders::_1, std::placeholders::_2,
        std::placeholders::_3);
    event_->SetTimerEvent(fdTriggerEvent_, triggerReadFd_, true, func, NULL);
    event_->AddEventMessage(fdTriggerEvent_);
    event_->EventMessageLoop();
    TELEPHONY_LOGE("error in EventMessageLoop errno:%{public}d, isNormalDestory:%{public}d",
        errno, event_->IsNormalDestory());
    if (!event_->IsNormalDestory()) {
        EventLoop();
    }
}
} // namespace Telephony
} // namespace OHOS