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
#include <pthread.h>
#include <unistd.h>

#include "telephony_log_c.h"

#define DEFAULT_LONG_TIMEOUT 50000

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
    int32_t success;
    char *result;
    Line *head;
    Line *last;
} ResponseInfo;

typedef void (*OnNotify)(const char *s, const char *s1); // onNotify

int32_t ATStartReadLoop(int32_t fd, OnNotify func); // at_open

void ATCloseReadLoop(void); // at_close

void AtSetOnUnusual(void (*onAtUnusual)(void));

void FreeResponseInfo(ResponseInfo *resp); // free ResponseResult

int32_t SendCommandLock(const char *command, const char *prefix, long long timeout, ResponseInfo **outResponse);

int32_t SendCommandNetWorksLock(
    const char *command, const char *prefix, long long timeout, ResponseInfo **outResponse);

int32_t SendCommandSmsLock(
    const char *command, const char *smsPdu, const char *prefix, long long timeout, ResponseInfo **outResponse);

int32_t SendCommandNoLock(const char *command, long long timeout, ResponseInfo **outResponse);

void SetWatchFunction(void (*watchFun)(void));

void SetAtPauseFlag(bool isNeedPause);
bool GetAtPauseFlag(void);

#endif // OHOS_AT_SUPPORT_H
