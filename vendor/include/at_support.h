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

#ifndef OHOS_AT_SUPPORT_H
#define OHOS_AT_SUPPORT_H

#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include "telephony_log_c.h"

typedef struct Line {
    struct Line *next;
    char *data;
} Line;

typedef enum {
    VENDOR_TYPE_NONE = 0, /* indicates no cell information */
    VENDOR_TYPE_GSM,
    VENDOR_TYPE_LTE,
    VENDOR_TYPE_WCDMA,
} VendorType; // Radio Access Technology

typedef struct {
    int success;
    char *result;
    Line *head;
    Line *last;
} ResponseInfo;

typedef void (*OnNotify)(const char *s, const char *s1); // onNotify

void AddLinkListNode(const char *s);

int ATStartReadLoop(int fd, OnNotify func); // at_open

void ATCloseReadLoop(void); // at_close

void AtSetOnUnusual(void (*OnAtUnusual)(void));

void FreeResponseInfo(ResponseInfo *resp); // free ResponseResult

void *ReaderLoop(void *s); // readLoop

void ProcessResponse(const char *s, const char *pdu); // process response

int SendCommandLock(const char *command, const char *prefix, long long timeout, ResponseInfo **outResponse);

int SendCommandNetWorksLock(const char *command, const char *prefix, long long timeout, ResponseInfo **outResponse);

int SendCommandSmsLock(
    const char *command, const char *smsPdu, const char *prefix, long long timeout, ResponseInfo **outResponse);

int SendCommandNoLock(const char *command, long long timeout, ResponseInfo **outResponse);

void ClearCurCommand(void);

void SetWatchFunction(void (*WatchFun)(void));

void SetAtPauseFlag(bool isNeedPause);
bool GetAtPauseFlag(void);
#endif // OHOS_AT_SUPPORT_H
