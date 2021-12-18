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

#include "vendor_util.h"

#include <stdlib.h>

#define NS_PER_S 1000000000
#define COUNT 1000

const int G_RESP_ERRORS = 6;
const int G_RESP_SUCCESS = 2;
const int G_RESP_SMS_NOTIFY = 3;

static const char *g_respErrors[G_RESP_ERRORS] = {
    "ERROR", "NO ANSWER", "+CME ERROR:", "NO CARRIER", "NO DIALTONE", "+CMS ERROR:"};

static const char *g_respSuccess[G_RESP_SUCCESS] = {"OK", "CONNECT"};

static const char *g_respSmsNotify[G_RESP_SMS_NOTIFY] = {"+CMT:", "+CDS:", "+CBM:"};

enum CommonNumber {
    HRIL_DEC = 10,
    HRIL_HEX = 16,
};

int32_t GenerateCommand(char *buffer, size_t bufferLen, const char *fmt, ...)
{
    int32_t ret;
    va_list va;

    va_start(va, fmt);
    ret = vsprintf_s(buffer, bufferLen, fmt, va);
    va_end(va);

    return ret;
}

int ReportStrWith(const char *s, const char *prefix)
{
    const char *source = s;
    const char *dest = prefix;
    if (prefix == NULL || s == NULL) {
        TELEPHONY_LOGE("str or prefix parameter is null.");
        return 0;
    }
    for (; *source != '\0' && *dest != '\0'; source++, dest++) {
        if (*source != *dest) {
            return 0;
        }
    }
    return *dest == '\0';
}

int IsResponseSuccess(const char *s)
{
    size_t i;
    for (i = 0; i < G_RESP_SUCCESS; i++) {
        if (ReportStrWith(s, g_respSuccess[i])) {
            return 1;
        }
    }
    return 0;
}

int IsResponseError(const char *s)
{
    size_t i;
    for (i = 0; i < G_RESP_ERRORS; i++) {
        if (ReportStrWith(s, g_respErrors[i])) {
            return 1;
        }
    }
    return 0;
}

int IsSms(const char *s)
{
    if (s[0] == '>') {
        return 1;
    }
    return 0;
}

int IsSmsNotify(const char *s)
{
    size_t i;
    for (i = 0; i < G_RESP_SMS_NOTIFY; i++) {
        if (ReportStrWith(s, g_respSmsNotify[i])) {
            return 1;
        }
    }
    return 0;
}

void SetWaitTimeout(struct timespec *time, long long msec)
{
    struct timeval now;
    long def = 1000L;
    gettimeofday(&now, (struct timezone *)NULL);
    time->tv_nsec = (now.tv_usec + (msec % COUNT) * def) * def;
    time->tv_sec = now.tv_sec + (msec / COUNT);
    if (time->tv_nsec >= NS_PER_S) {
        time->tv_nsec -= NS_PER_S;
        time->tv_sec++;
    }
}

int SkipATPrefix(char **s)
{
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return -1;
    }
    *s = strchr(*s, ':');
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return -1;
    }
    (*s)++;
    return 0;
}

void SkipSpace(char **s)
{
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return;
    }
    while (**s != '\0' && **s == ' ') {
        (*s)++;
    }
}

int NextInt(char **s, int *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return -1;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return -1;
    }
    ret = strsep(s, ",");
    while (*s != NULL && **s == ',') {
        (*s)++;
    }
    *out = (int)strtol(ret, &end, HRIL_DEC);
    if (ret == end) {
        TELEPHONY_LOGE("strtol is fail, err:%{public}d, ret:%{public}s", *out, ret);
        return -1;
    }
    return 0;
}

void SkipNextComma(char **s)
{
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return;
    }
    while (**s != '\0' && **s != ',') {
        (*s)++;
    }
    if (**s == ',') {
        (*s)++;
    }
}

int NextIntFromHex(char **s, int *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return -1;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return -1;
    } else if (**s == '"') {
        (*s)++;
        ret = strsep(s, "\"");
        SkipNextComma(s);
    } else {
        ret = strsep(s, ",");
    }
    *out = (int)strtol(ret, &end, HRIL_HEX);
    if (ret == end) {
        TELEPHONY_LOGE("strtol is fail, err:%{public}d, ret:%{public}s", *out, ret);
        return -1;
    }
    return 0;
}

uint64_t NextULongFromHex(char **s, uint64_t *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return -1;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return -1;
    } else if (**s == '"') {
        (*s)++;
        ret = strsep(s, "\"");
        SkipNextComma(s);
    } else {
        ret = strsep(s, ",");
    }
    *out = (uint64_t)strtoul(ret, &end, HRIL_HEX);
    if (ret == end) {
        TELEPHONY_LOGE("strtoul is fail, ret:%{public}s", ret);
        return -1;
    }
    return 0;
}

int NextStr(char **s, char **out)
{
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return -1;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return -1;
    } else if (**s == '"') {
        (*s)++;
        *out = strsep(s, "\"");
        SkipNextComma(s);
    } else {
        *out = strsep(s, ",");
    }
    return 0;
}

/* +CRING: GPRS "IP","00.00.00.00",,"abc.com"
 * get GPRS
 */
int NextTxtStr(char **s, char **out)
{
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return -1;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return -1;
    } else {
        *out = strsep(s, " ");
    }
    return 0;
}

int NextBool(char **s, char *out)
{
    int ret;
    int result;

    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return -1;
    }
    ret = NextInt(s, &result);
    if (ret < 0) {
        TELEPHONY_LOGE("NextInt is fail, ret:%{public}d, src:%{public}s", ret, *s);
        return -1;
    }
    // booleans should be 0 or 1
    if (!(result == 0 || result == 1)) {
        TELEPHONY_LOGE("booleans should be 0 or 1, result:%{public}d", result);
        return -1;
    }
    if (out != NULL) {
        *out = (char)result;
    }
    return ret;
}

int ParseReportError(char *str)
{
    int ret = VENDOR_FAIL;
    char *pStr = str;

    if (pStr == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ret;
    }
    if (!SkipATPrefix(&pStr)) {
        NextInt(&pStr, &ret);
    }
    return ret;
}

ModemReportErrorInfo GetReportErrorInfo(const ResponseInfo *response)
{
    int ret = VENDOR_FAIL;
    ModemReportErrorInfo errInfo;

    errInfo.errType = HRIL_REPORT_ERR_TYPE_GENERIC;
    errInfo.errorNo = HRIL_ERR_GENERIC_FAILURE;
    if ((response != NULL) && (!response->success)) {
        char *pStr = response->result;
        if (ReportStrWith(pStr, "+CME ERROR:")) {
            errInfo.errType = HRIL_REPORT_ERR_TYPE_CME;
        } else if (ReportStrWith(pStr, "+CMS ERROR:")) {
            errInfo.errType = HRIL_REPORT_ERR_TYPE_CMS;
        } else {
            return errInfo;
        }
        ret = ParseReportError(pStr);
        if (ret >= 0) {
            errInfo.errorNo = ret;
        }
    }
    return errInfo;
}

ModemReportErrorInfo InitModemReportErrorInfo(void)
{
    ModemReportErrorInfo errInfo;
    errInfo.errorNo = HRIL_ERR_SUCCESS;
    errInfo.errType = HRIL_REPORT_ERR_TYPE_NONE;
    return errInfo;
}