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

#ifndef OHOS_HRIL_TIMER_CALLBACK_H
#define OHOS_HRIL_TIMER_CALLBACK_H

#include <thread>

#include "hril_event.h"

namespace OHOS {
namespace Telephony {
typedef void (*HRilCallbackFun)(uint8_t *param);

struct HRilTimerCallbackMessage {
    uint8_t *param;
    HRilCallbackFun func;
};

class HRilTimerCallback {
public:
    HRilTimerCallback() {};
    virtual ~HRilTimerCallback() = default;
    std::unique_ptr<HRilEvent> event_ = nullptr;

    void EventLoop();
    std::shared_ptr<HRilTimerCallbackMessage> HRilSetTimerCallbackInfo(
        HRilCallbackFun func, uint8_t *param, const struct timeval *tv);
    void OnTriggerEvent();

private:
    void FdTriggerCallback(int32_t fd, int16_t events, std::shared_ptr<void> param);
    void TimerCallback(int32_t fd, int16_t events, std::shared_ptr<void> param);

private:
    HRilEventMessage fdTriggerEvent_;
    int32_t triggerReadFd_;
    int32_t triggerWriteFd_;
    const int32_t PIPE_SIZE_MAX = 2;
    const int32_t READ_FD_BUFF_SIZE = 16;
    std::thread::id eventLoopTid_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_TIMER_CALLBACK_H