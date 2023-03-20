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

#include "hril_event.h"

#include <cerrno>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

#include "securec.h"

#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
void HRilEvent::GetNowTime(struct timeval &tv)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    tv.tv_sec = ts.tv_sec;
    tv.tv_usec = ts.tv_nsec / TIME_UNIT;
}

bool HRilEvent::GetNextTimeOut(struct timeval &tv)
{
    std::list<HRilEventMessage>::iterator eventIt = timerList_.begin();
    if (eventIt == timerList_.end() || timerList_.empty()) {
        return false;
    }

    struct timeval now;
    GetNowTime(now);
    TELEPHONY_LOGD("now = %{public}ds + %{public}dus", (int32_t)now.tv_sec, (int32_t)now.tv_usec);
    TELEPHONY_LOGD(
        "next = %{public}ds + %{public}dus", (int32_t)eventIt->timeout.tv_sec, (int32_t)eventIt->timeout.tv_usec);
    if (timercmp(&eventIt->timeout, &now, >)) {
        timersub(&eventIt->timeout, &now, &tv);
    } else {
        // timer already expired.
        tv.tv_sec = tv.tv_usec = 0;
    }
    return true;
}

void HRilEvent::ProcessTimerList()
{
    struct timeval now;
    std::lock_guard<std::mutex> mutexLock(listLock_);
    std::list<HRilEventMessage>::iterator eventIt = timerList_.begin();
    GetNowTime(now);

    TELEPHONY_LOGD("finding for timers <= %{public}ds + %{public}dus", (int32_t)now.tv_sec, (int32_t)now.tv_usec);
    while ((eventIt != timerList_.end()) && (timercmp(&now, &eventIt->timeout, >))) {
        HRilEventMessage evMsg = {};
        evMsg.fd = eventIt->fd;
        evMsg.func = eventIt->func;
        evMsg.index = eventIt->index;
        evMsg.param = eventIt->param;
        evMsg.timeout = eventIt->timeout;
        pendingList_.push_back(evMsg);
        eventIt = timerList_.erase(eventIt);
    }
}

void HRilEvent::ProcessPendingList()
{
    std::list<HRilEventMessage>::iterator eventIt = pendingList_.begin();
    while (eventIt != pendingList_.end()) {
        if (eventIt->func != nullptr) {
            eventIt->func(eventIt->fd, 0, eventIt->param);
        }
        eventIt = pendingList_.erase(eventIt);
    }
}

void HRilEvent::EraseListenEvent(HRilEventMessage &eventMsg, int32_t index)
{
    listenEventTable_[index] = nullptr;
    eventMsg.index = DEFAULT_INDEX;

    FD_CLR(eventMsg.fd, &readFds_);

    if (eventMsg.fd + 1 == nfds_) {
        int32_t n = 0;
        for (auto msg : listenEventTable_) {
            if ((msg != nullptr) && (msg->fd > n)) {
                n = msg->fd;
            }
        }
        nfds_ = n + 1;
        TELEPHONY_LOGD("Updated nfds = %{public}d", nfds_);
    }
}

void HRilEvent::ProcessEvents(fd_set *rfds, int32_t number)
{
    auto it = listenEventTable_.begin();
    for (; (it != listenEventTable_.end()) && (number > 0); ++it) {
        if (*it != nullptr && FD_ISSET((*it)->fd, rfds)) {
            pendingList_.push_back(*(*it));
            if ((*it)->isHolding == false) {
                EraseListenEvent(*(*it), (*it)->index);
            }
            number--;
        }
    }
}

void HRilEvent::TimerEventInit()
{
    FD_ZERO(&readFds_);
    timerList_.clear();
    pendingList_.clear();
    listenEventTable_.clear();
    for (int32_t i = 0; i < LISTEN_FD_EVENTS_MAX; i++) {
        listenEventTable_.push_back(nullptr);
    }
}

void HRilEvent::AddTimerEvent(HRilEventMessage &eventMsg, const struct timeval &tv)
{
    std::lock_guard<std::mutex> mutexLock(listLock_);
    struct timeval now;
    eventMsg.fd = IVNALID_FD; // make sure fd is invalid

    GetNowTime(now);
    timeradd(&now, &tv, &eventMsg.timeout);

    std::list<HRilEventMessage>::iterator it = timerList_.begin();
    for (; it != timerList_.end(); ++it) {
        if (timercmp(&it->timeout, &eventMsg.timeout, >)) {
            timerList_.insert(it, eventMsg);
            return;
        }
    }
    if (it == timerList_.end()) {
        timerList_.push_back(eventMsg);
    }
}

void HRilEvent::SetTimerEvent(
    HRilEventMessage &eventMsg, int32_t fd, bool isHolding, HRilEventCallback func, std::shared_ptr<void> param)
{
    (void)memset_s(&eventMsg, sizeof(HRilEventMessage), 0, sizeof(HRilEventMessage));
    eventMsg.fd = fd;
    eventMsg.index = DEFAULT_INDEX;
    eventMsg.func = func;
    eventMsg.param = param;
    eventMsg.isHolding = isHolding;
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

void HRilEvent::AddEventMessage(const HRilEventMessage &eventMsg)
{
    std::lock_guard<std::mutex> mutexLock(listLock_);
    for (int32_t i = 0; i < LISTEN_FD_EVENTS_MAX; i++) {
        if (listenEventTable_[i] == nullptr) {
            listenEventTable_[i] = const_cast<HRilEventMessage *>(&eventMsg);
            listenEventTable_[i]->index = i;
            FD_SET(eventMsg.fd, &readFds_);
            if (eventMsg.fd >= nfds_) {
                nfds_ = eventMsg.fd + 1;
            }
            break;
        }
    }
}

void HRilEvent::RemoveEventMessage(HRilEventMessage &eventMsg)
{
    std::lock_guard<std::mutex> mutexLock(listLock_);
    if (eventMsg.index < 0 || eventMsg.index >= LISTEN_FD_EVENTS_MAX) {
        TELEPHONY_LOGE("Invalid event message! index:%{pubulic}d", eventMsg.index);
        return;
    }
    EraseListenEvent(eventMsg, eventMsg.index);
}

void HRilEvent::EventMessageLoop()
{
    int32_t ret;
    fd_set rfds;
    struct timeval timeout;
    struct timeval *pTimeOut;

    TELEPHONY_LOGD("****** EventMessageLoop start ******");
    while (1) {
        (void)memcpy_s(&rfds, sizeof(fd_set), &readFds_, sizeof(fd_set));
        if (!GetNextTimeOut(timeout)) {
            // Enter blocking wait without setting a timer.
            TELEPHONY_LOGD("Enter blocking wait without setting a timer.");
            pTimeOut = nullptr;
        } else {
            TELEPHONY_LOGD(
                "Setting timeout for %{public}ds + %{public}dus", (int32_t)timeout.tv_sec, (int32_t)timeout.tv_usec);
            pTimeOut = &timeout;
        }
        ret = select(nfds_, &rfds, nullptr, nullptr, pTimeOut);
        TELEPHONY_LOGD("There are %{public}d events fired, isNormalDestory: %{public}d.", ret, isNormalDestory);
        if (isNormalDestory) {
            return;
        }
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            }
            TELEPHONY_LOGE("select error (%{public}d)", errno);
            return;
        }
        ProcessTimerList();
        ProcessEvents(&rfds, ret);
        ProcessPendingList();
    }
}

void HRilEvent::SetNormalDestory(bool isDestory)
{
    isNormalDestory = isDestory;
}
bool HRilEvent::IsNormalDestory()
{
    return isNormalDestory;
}

} // namespace Telephony
} // namespace OHOS