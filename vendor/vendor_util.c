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

#define NS_PER_S 1000000000
#define COUNT 1000

static const char *g_respErrors[] = {
    "ERROR",
    "NO ANSWER",
    "+CME ERROR:",
    "NO CARRIER",
    "NO DIALTONE",
    "+CMS ERROR:"
};

static const char *g_respSuccess[] = {
    "OK",
    "CONNECT"
};

static const char *g_respSmsNotify[] = {
    "+CMT:",
    "+CDS:",
    "+CBM:"
};

enum CommonNumber {
    HRIL_DEC = 10,
    HRIL_INVALID_HEX_CHAR = 16,
};

int ReportStrWith(const char *s, const char *prefix)
{
    const char *source = s;
    const char *dest = prefix;
    if (prefix == NULL || s == NULL) {
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
    for (i = 0; i < (sizeof(g_respSuccess) / sizeof((g_respSuccess)[0])); i++) {
        if (ReportStrWith(s, g_respSuccess[i])) {
            return 1;
        }
    }
    return 0;
}

int IsResponseError(const char *s)
{
    size_t i;
    for (i = 0; i < (sizeof(g_respErrors) / sizeof((g_respErrors)[0])); i++) {
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
    for (i = 0; i < (sizeof(g_respSmsNotify) / sizeof((g_respSmsNotify)[0])); i++) {
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
        return -1;
    }

    *s = strchr(*s, ':');

    if (*s == NULL) {
        return -1;
    }

    (*s)++;

    return 0;
}

void SkipSpace(char **s)
{
    if (*s == NULL) {
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
    if (*s == NULL || out == NULL) {
        return -1;
    }
    SkipSpace(s);
    if (*s == NULL) {
        return -1;
    }
    ret = strsep(s, ",");
    while (*s != NULL && **s == ',') {
        (*s)++;
    }
    *out = (int)strtol(ret, &end, HRIL_DEC);

    if (ret == end) {
        return -1;
    }
    return 0;
}

int NextIntFromHex(char **s, int *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (*s == NULL || out == NULL) {
        return -1;
    }
    SkipSpace(s);
    if (*s == NULL) {
        return -1;
    }
    ret = strsep(s, ",");
    while (*s != NULL && **s == ',') {
        (*s)++;
    }
    *out = (int)strtol(ret, &end, HRIL_INVALID_HEX_CHAR);

    if (ret == end) {
        return -1;
    }
    return 0;
}

void SkipNextComma(char **s)
{
    if (*s == NULL) {
        return;
    }

    while (**s != '\0' && **s != ',') {
        (*s)++;
    }

    if (**s == ',') {
        (*s)++;
    }
}

int NextStr(char **s, char **out)
{
    if (*s == NULL) {
        return -1;
    }
    SkipSpace(s);
    if (*s == NULL) {
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

int NextBool(char **s, char *out)
{
    int ret;
    int result;
    ret = NextInt(s, &result);
    if (ret < 0) {
        return -1;
    }
    // booleans should be 0 or 1
    if (!(result == 0 || result == 1)) {
        return -1;
    }
    if (out != NULL) {
        *out = (char)result;
    }
    return ret;
}