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

#ifndef OHOS_HRIL_EVENT_H
#define OHOS_HRIL_EVENT_H

#include <functional>
#include <mutex>
#include <list>
#include <sys/time.h>
#include <vector>

namespace OHOS {
namespace Telephony {
typedef std::function<void(int32_t, int16_t, std::shared_ptr<void>)> HRilEventCallback;
struct HRilEventMessage {
    int32_t fd;
    int32_t index;
    bool isHolding;
    struct timeval timeout;
    HRilEventCallback func;
    std::shared_ptr<void> param;
};

class HRilEvent {
public:
    HRilEvent() {};
    virtual ~HRilEvent() = default;
    const int32_t IVNALID_FD = -1;

    void TimerEventInit();
    void AddTimerEvent(HRilEventMessage &eventMsg, const struct timeval &tv);
    void SetTimerEvent(
        HRilEventMessage &eventMsg, int32_t fd, bool isHolding, HRilEventCallback func, std::shared_ptr<void> param);
    void AddEventMessage(const HRilEventMessage &eventMsg);
    void RemoveEventMessage(HRilEventMessage &eventMsg);
    void EventMessageLoop();
    void SetNormalDestory(bool isDestory);
    bool IsNormalDestory();

private:
    void GetNowTime(struct timeval &tv);
    bool GetNextTimeOut(struct timeval &tv);
    void ProcessTimerList();
    void ProcessPendingList();
    void EraseListenEvent(HRilEventMessage &eventMsg, int32_t index);
    void ProcessEvents(fd_set *rfds, int32_t number);

    std::mutex listLock_;
    std::list<HRilEventMessage> timerList_;
    std::list<HRilEventMessage> pendingList_;
    std::vector<HRilEventMessage *> listenEventTable_;
    fd_set readFds_;
    int32_t nfds_;
    bool isNormalDestory = false;
    const int32_t DEFAULT_INDEX = -1;
    const int32_t LISTEN_FD_EVENTS_MAX = 8;
    const int32_t TIME_UNIT = 1000;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_EVENT_H