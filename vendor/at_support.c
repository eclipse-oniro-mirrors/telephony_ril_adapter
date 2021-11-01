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

#include "at_support.h"
#include "vendor_util.h"
#include "channel.h"

static int g_atFd = -1;
static OnNotify g_onNotifyFunc = NULL;

static volatile ResponseInfo *g_response = NULL;
static volatile const char *g_smsPdu = NULL;
static volatile bool g_isNeedATPause = false;
static volatile const char *g_prefix = NULL;
static pthread_mutex_t g_commandmutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_commandcond = PTHREAD_COND_INITIALIZER;
static volatile int g_readerClosed = 0;
static pthread_t g_reader;
static void (*g_onTimeout)(void) = NULL;
static void (*g_atnUnusual)(void) = NULL;
static void (*g_atWatch)(void) = NULL;

// reader close
static void OnReaderClosed(void);
// process response
static void ProcessResponse(const char *responseLine, const char *pdu);
// add item to list
static void AddLinkListNode(const char *responseLine);
// thread function: readLoop
static void *ReaderLoop(void *s);
// clear command memory
static void ClearCurCommand(void);

void AtSetOnUnusual(void (*OnAtUnusual)(void))
{
    g_atnUnusual = OnAtUnusual;
}

int ATStartReadLoop(int fd, OnNotify func)
{
    int ret = 0;
    g_atFd = fd;
    g_onNotifyFunc = func;
    pthread_attr_t t;
    pthread_attr_init(&t);
    pthread_attr_setdetachstate(&t, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&g_reader, &t, ReaderLoop, &t);
    if (ret < 0) {
        TELEPHONY_LOGE("create pthread error code： %{public}d", ret);
        return VENDOR_ERR_PROCESS;
    }
    return VENDOR_SUCCESS;
}

void OnReaderClosed(void)
{
    pthread_mutex_lock(&g_commandmutex);
    if (g_atnUnusual != NULL && g_readerClosed == 0) {
        g_readerClosed = 1;
        pthread_cond_signal(&g_commandcond);
        pthread_mutex_unlock(&g_commandmutex);
        g_atnUnusual();
        return;
    }
    pthread_mutex_unlock(&g_commandmutex);
}

void ATCloseReadLoop(void)
{
    if (g_atFd >= 0) {
        close(g_atFd);
    }
    g_atFd = -1;
    pthread_mutex_lock(&g_commandmutex);
    g_readerClosed = 1;
    pthread_cond_signal(&g_commandcond);
    pthread_mutex_unlock(&g_commandmutex);
}

void FreeResponseInfo(ResponseInfo *resp)
{
    Line *p = NULL;
    if (resp == NULL) {
        TELEPHONY_LOGD("%{public}s enter resp is null", __func__);
        return;
    }
    p = resp->head;
    if (p == NULL && resp->result != NULL) {
        free(resp->result);
        resp->result = NULL;
        return;
    }
    while (p != NULL) {
        Line *t = NULL;
        t = p;
        p = p->next;
        if (t->data != NULL) {
            free(t->data);
            t->data = NULL;
        }
        free(t);
        t = NULL;
    }
    if (resp->result != NULL) {
        free(resp->result);
        resp->result = NULL;
    }
    free(resp);
    resp = NULL;
}

void *ReaderLoop(void *s)
{
    while (1) {
        const char *str = NULL;
        const char *pdu = NULL;
        str = ReadResponse(g_atFd);
        if (str == NULL) {
            TELEPHONY_LOGI("%{public}s enter str is null", __func__);
            break;
        }
        if (IsSmsNotify(str)) {
            pdu = ReadResponse(g_atFd);
            TELEPHONY_LOGI("new sms notify :%{public}s, pdu :%{public}s", str, pdu);
        }
        ProcessResponse(str, pdu);
    }
    OnReaderClosed();
    return NULL;
}

void ProcessResponse(const char *responseLine, const char *pdu)
{
    if (responseLine == NULL) {
        TELEPHONY_LOGE("%{public}s enter s is null", __func__);
        return;
    }
    TELEPHONY_LOGD("processLine line = %{public}s", responseLine);

    int isPrefix = ReportStrWith(responseLine, (const char*)g_prefix);
    pthread_mutex_lock(&g_commandmutex);
    if (g_response == NULL) {
        if (g_onNotifyFunc != NULL) {
            pthread_mutex_unlock(&g_commandmutex);

            g_onNotifyFunc(responseLine, pdu);
            return;
        }
    } else if (IsResponseError(responseLine)) {
        if (g_response != NULL) {
            g_response->success = 0;
            g_response->result = strdup(responseLine);
        }
        pthread_cond_signal(&g_commandcond);
    } else if (IsResponseSuccess(responseLine)) {
        if (g_response != NULL) {
            g_response->success = 1;
            g_response->result = strdup(responseLine);
        }
        pthread_cond_signal(&g_commandcond);
    } else if (IsSms(responseLine) && g_smsPdu != NULL) {
        if (g_response != NULL) {
            g_response->result = strdup(responseLine);
        }
        WriteATCommand((const char *)g_smsPdu, 1, g_atFd);
    } else {
        if (((isdigit(responseLine[0]) || isPrefix) && g_smsPdu == NULL) || (g_smsPdu != NULL && isPrefix)) {
            AddLinkListNode(responseLine);
        } else {
            if (g_onNotifyFunc != NULL) {
                pthread_mutex_unlock(&g_commandmutex);

                g_onNotifyFunc(responseLine, pdu);
                return;
            }
        }
    }
    pthread_mutex_unlock(&g_commandmutex);
}

void AddLinkListNode(const char *responseLine)
{
    if (g_response == NULL) {
        TELEPHONY_LOGE("response is null");
        return;
    }

    Line *line = (Line *)malloc(sizeof(Line));
    if (line == NULL) {
        TELEPHONY_LOGE("malloc memory error");
        return;
    }
    line->data = strdup(responseLine);
    line->next = NULL;
    if (g_response->last != NULL) {
        g_response->last->next = line;
    } else {
        g_response->head = line;
    }
    g_response->last = line;
}

int SendCommandLock(const char *command, const char *prefix, long long timeout, ResponseInfo **outResponse)
{
    const char *atCmd = "AT";
    int err;
    if (pthread_equal(g_reader, pthread_self()) != 0) {
        return AT_ERR_INVALID_THREAD;
    }

    TELEPHONY_LOGD("command %{public}s, NeedATPause:%{public}d, atCmd:%{public}s",
        command, g_isNeedATPause, atCmd);
    pthread_mutex_lock(&g_commandmutex);
    if (g_isNeedATPause) {
        pthread_cond_signal(&g_commandcond);
        err = SendCommandNoLock(atCmd, timeout, outResponse);
        if (err != 0) {
            TELEPHONY_LOGE("err = %{public}d", err);
        }

        if (g_atWatch != NULL) {
            g_atWatch();
        }
        g_isNeedATPause = false;
        alarm(0);
    }
    g_prefix = prefix;
    err = SendCommandNoLock(command, timeout, outResponse);
    pthread_mutex_unlock(&g_commandmutex);
    TELEPHONY_LOGD("err = %{public}d", err);
    // when timeout to process
    if (err == AT_ERR_TIMEOUT && g_onTimeout != NULL) {
        g_onTimeout();
    } else if (err == AT_ERR_GENERIC) {
        TELEPHONY_LOGD("OnReaderClosed() err = %{public}d", err);
        OnReaderClosed();
    }
    return err;
}

int SendCommandNetWorksLock(const char *command, const char *prefix, long long timeout, ResponseInfo **outResponse)
{
    int err;
    if (pthread_equal(g_reader, pthread_self()) != 0) {
        return AT_ERR_INVALID_THREAD;
    }
    TELEPHONY_LOGD("SendCommandNetWorksLock() command %{public}s", command);
    pthread_mutex_lock(&g_commandmutex);
    g_isNeedATPause = true;
    g_prefix = prefix;
    err = SendCommandNoLock(command, timeout, outResponse);
    pthread_mutex_unlock(&g_commandmutex);
    TELEPHONY_LOGD("SendCommandNetWorksLock() err = %{public}d", err);
    // when timeout to process
    if (err == AT_ERR_TIMEOUT) {
        err = AT_ERR_WAITING;
    }
    return err;
}

int SendCommandSmsLock(
    const char *command, const char *smsPdu, const char *prefix, long long timeout, ResponseInfo **outResponse)
{
    int err;
    if (pthread_equal(g_reader, pthread_self()) != 0) {
        return AT_ERR_INVALID_THREAD;
    }
    TELEPHONY_LOGD("SendCommandSmsLock() command %{public}s", command);
    pthread_mutex_lock(&g_commandmutex);
    g_prefix = prefix;
    g_smsPdu = smsPdu;
    err = SendCommandNoLock(command, timeout, outResponse);
    pthread_mutex_unlock(&g_commandmutex);
    TELEPHONY_LOGD("SendCommandSmsLock() err = %{public}d", err);
    // when timeout to process
    if (err == AT_ERR_TIMEOUT && g_onTimeout != NULL) {
        g_onTimeout();
    }
    return err;
}

int SendCommandNoLock(const char *command, long long timeout, ResponseInfo **outResponse)
{
    long long defaultTimeout = 5000;
    int err = 0;
    struct timespec time;
    if (g_response != NULL) {
        err = AT_ERR_COMMAND_PENDING;
        goto ERROR;
    }
    g_response = (ResponseInfo *)calloc(1, sizeof(ResponseInfo));
    if (g_response == NULL) {
        err = AT_ERR_GENERIC;
        goto ERROR;
    }
    err = WriteATCommand(command, 0, g_atFd);
    if (err != VENDOR_SUCCESS) {
        goto ERROR;
    }
    if (timeout != 0) {
        SetWaitTimeout(&time, timeout);
    } else {
        SetWaitTimeout(&time, defaultTimeout);
    }
    while (g_response->result == NULL && g_readerClosed == 0) {
        err = pthread_cond_timedwait(&g_commandcond, &g_commandmutex, &time);
        if (err == ETIMEDOUT) {
            err = AT_ERR_TIMEOUT;
            goto ERROR;
        }
    }
    if (outResponse == NULL) {
        FreeResponseInfo((ResponseInfo *)g_response);
    } else {
        *outResponse = (ResponseInfo *)g_response;
    }
    g_response = NULL;
    if (g_readerClosed > 0) {
        err = AT_ERR_CHANNEL_CLOSED;
        goto ERROR;
    }
    err = 0;
    return err;
ERROR:
    ClearCurCommand();
    return err;
}

void ClearCurCommand(void)
{
    if (g_response != NULL) {
        FreeResponseInfo((ResponseInfo *)g_response);
    }
    g_response = NULL;
    g_prefix = NULL;
}

void SetWatchFunction(void (*WatchFun)(void))
{
    g_atWatch = WatchFun;
}

void SetAtPauseFlag(bool isNeedPause)
{
    pthread_mutex_lock(&g_commandmutex);
    g_isNeedATPause = isNeedPause;
    pthread_mutex_unlock(&g_commandmutex);
}

bool GetAtPauseFlag(void)
{
    return g_isNeedATPause;
}