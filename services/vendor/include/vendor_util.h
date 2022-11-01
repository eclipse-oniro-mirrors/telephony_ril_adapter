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

typedef enum {
    AT_RESPONSE_SIM_PIN_REQUIRE = 11,
    AT_RESPONSE_SIM_FAILURE = 13,
    AT_RESPONSE_INCORRECT_PASSWORD = 16,
} ATPesponseCMEErr;

int32_t GenerateCommand(char *buffer, size_t bufferLen, const char *fmt, ...);

int32_t ReportStrWith(const char *line, const char *prefix);

int32_t IsResponseSuccess(const char *line);

int32_t IsResponseError(const char *line);

int32_t IsSms(const char *line);

int32_t IsSmsNotify(const char *line);

void SetWaitTimeout(struct timespec *time, long long msec);

int32_t SkipATPrefix(char **s);

void SkipSpace(char **s);

int32_t NextInt(char **s, int32_t *out);

int64_t NextInt64(char **s, int64_t *out);

int32_t NextIntNotSkipNextComma(char **s, int32_t *out);

int32_t NextIntByRightBracket(char **s, int32_t *out);

int32_t NextIntFromHex(char **s, int32_t *out);

int32_t NextStr(char **s, char **out);

int32_t NextTxtStr(char **s, char **out);

int32_t NextBool(char **s, char *out);

int32_t ParseReportError(char *str);

int32_t NextULongFromHex(char **s, uint64_t *out);

ModemReportErrorInfo GetReportErrorInfo(const ResponseInfo *response);

ModemReportErrorInfo InitModemReportErrorInfo(void);

int32_t ConvertCharToInt32(const char *s);

int32_t FindCommaCharNum(const char *srcStr);

int32_t GetSimSlotCount(void);

int32_t GetSlotId(const ReqDataInfo *requestInfo);
#endif // OHOS_VENDOR_UTIL_H
