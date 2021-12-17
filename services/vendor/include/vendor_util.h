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

#ifndef OHOS_VENDOR_UTIL_H
#define OHOS_VENDOR_UTIL_H

#include <sys/time.h>
#include <securec.h>
#include <time.h>

#include "at_support.h"
#include "hril.h"
#include "telephony_log_c.h"

#define MAX_RESPONSE_LEN (8 * 1024)

#define INT_DEFAULT_VALUE (-1)
#define STR_DEFAULT_VALUE ""

typedef enum {
    VENDOR_FAIL = -1,
    VENDOR_SUCCESS,
    VENDOR_ERR_PROCESS,
    AT_ERR_TIMEOUT,
    AT_ERR_COMMAND_PENDING,
    AT_ERR_CHANNEL_CLOSED,
    AT_ERR_GENERIC,
    AT_ERR_WAITING,
    AT_ERR_INVALID_THREAD
} VendorErrorType;

int32_t GenerateCommand(char *buffer, size_t bufferLen, const char *fmt, ...);

int ReportStrWith(const char *line, const char *prefix);

int IsResponseSuccess(const char *line);

int IsResponseError(const char *line);

int IsSms(const char *line);

int IsSmsNotify(const char *line);

void SetWaitTimeout(struct timespec *time, long long msec);

int SkipATPrefix(char **s);

void SkipSpace(char **s);

int NextInt(char **s, int *out);

int NextIntFromHex(char **s, int *out);

int NextStr(char **s, char **out);

int NextTxtStr(char **s, char **out);

int NextBool(char **s, char *out);

int ParseReportError(char *str);

uint64_t NextULongFromHex(char **s, uint64_t *out);

ModemReportErrorInfo GetReportErrorInfo(const ResponseInfo *response);

ModemReportErrorInfo InitModemReportErrorInfo(void);
#endif // OHOS_VENDOR_UTIL_H
