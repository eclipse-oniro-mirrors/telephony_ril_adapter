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

#include "parameter.h"

#define NS_PER_S 1000000000
#define COUNT 1000

#define SUPPORT_SLOT_ID "persist.sys.support.slotid"
#define DEFAULT_SLOT_COUNT "1"
#define TEL_SIM_SLOT_COUNT "const.telephony.slotCount"

#define G_RESP_ERRORS 7
#define G_RESP_SUCCESS 2
#define G_RESP_SMS_NOTIFY 3
const int32_t G_CHAR_TO_INT = 10;
static const int32_t ERR = -1;
static const char *g_respErrors[G_RESP_ERRORS] = {
    "ERROR", "NO ANSWER", "+CME ERROR:", "NO CARRIER", "NO DIALTONE", "+CMS ERROR:", "COMMAND NOT SUPPORT"};

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

int32_t ReportStrWith(const char *s, const char *prefix)
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

int32_t IsResponseSuccess(const char *s)
{
    size_t i;
    for (i = 0; i < G_RESP_SUCCESS; i++) {
        if (ReportStrWith(s, g_respSuccess[i])) {
            return 1;
        }
    }
    return 0;
}

int32_t IsResponseError(const char *s)
{
    size_t i;
    for (i = 0; i < G_RESP_ERRORS; i++) {
        if (ReportStrWith(s, g_respErrors[i])) {
            return 1;
        }
    }
    return 0;
}

int32_t IsSms(const char *s)
{
    if (s == NULL) {
        return 0;
    }
    if (s[0] == '>') {
        return 1;
    }
    return 0;
}

int32_t IsSmsNotify(const char *s)
{
    if (s == NULL) {
        return 0;
    }
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
    if (time == NULL) {
        return;
    }
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

int32_t SkipATPrefix(char **s)
{
    if (s == NULL || *s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    *s = strchr(*s, ':');
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    (*s)++;
    return 0;
}

void SkipSpace(char **s)
{
    if (s == NULL || *s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return;
    }
    while (**s != '\0' && **s == ' ') {
        (*s)++;
    }
}

int32_t NextInt(char **s, int32_t *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (s == NULL || *s == NULL || out == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return ERR;
    }
    ret = strsep(s, ",");
    while (*s != NULL && **s == ',') {
        (*s)++;
    }
    *out = (int32_t)strtol(ret, &end, HRIL_DEC);
    if (ret == end) {
        TELEPHONY_LOGE("strtol is fail, err:%{public}d", *out);
        return ERR;
    }
    return 0;
}

int64_t NextInt64(char **s, int64_t *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (s == NULL || *s == NULL || out == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return ERR;
    }
    ret = strsep(s, ",");
    while (*s != NULL && **s == ',') {
        (*s)++;
    }
    *out = (int64_t)strtoll(ret, &end, HRIL_DEC);
    if (ret == end) {
        TELEPHONY_LOGE("NextInt64 strtoll is fail");
        return ERR;
    }
    return 0;
}

int32_t NextIntNotSkipNextComma(char **s, int32_t *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (s == NULL || *s == NULL || out == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return ERR;
    }
    ret = strsep(s, ",");
    *out = (int32_t)strtol(ret, &end, HRIL_DEC);
    if (ret == end) {
        TELEPHONY_LOGE("strtol is fail, err:%{public}d", *out);
        return ERR;
    }
    return 0;
}

int32_t NextIntByRightBracket(char **s, int32_t *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (s == NULL || *s == NULL || out == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return ERR;
    }
    ret = strsep(s, ")");
    while (*s != NULL && **s == ')') {
        (*s)++;
    }
    *out = (int32_t)strtol(ret, &end, HRIL_DEC);
    if (ret == end) {
        TELEPHONY_LOGE("strtol is fail, err:%{public}d", *out);
        return ERR;
    }
    return 0;
}

void SkipNextComma(char **s)
{
    if (s == NULL || *s == NULL) {
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

int32_t NextIntFromHex(char **s, int32_t *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (s == NULL || *s == NULL || out == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return ERR;
    } else if (**s == '"') {
        (*s)++;
        ret = strsep(s, "\"");
        SkipNextComma(s);
    } else {
        ret = strsep(s, ",");
    }
    *out = (int32_t)strtol(ret, &end, HRIL_HEX);
    if (ret == end) {
        TELEPHONY_LOGE("strtol is fail, err:%{public}d", *out);
        return ERR;
    }
    return 0;
}

int32_t NextULongFromHex(char **s, uint64_t *out)
{
    char *ret = NULL;
    char *end = NULL;
    if (s == NULL || *s == NULL || out == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return ERR;
    } else if (**s == '"') {
        (*s)++;
        ret = strsep(s, "\"");
        SkipNextComma(s);
    } else {
        ret = strsep(s, ",");
    }
    *out = (uint64_t)strtoul(ret, &end, HRIL_HEX);
    if (ret == end) {
        TELEPHONY_LOGE("strtoul is fail");
        return ERR;
    }
    return 0;
}

int32_t NextStr(char **s, char **out)
{
    if (s == NULL || *s == NULL || out == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return ERR;
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
int32_t NextTxtStr(char **s, char **out)
{
    if (s == NULL || *s == NULL || out == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    SkipSpace(s);
    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null, after skip space.");
        return ERR;
    } else {
        *out = strsep(s, " ");
    }
    return 0;
}

int32_t NextBool(char **s, char *out)
{
    int32_t ret;
    int32_t value;

    if (*s == NULL) {
        TELEPHONY_LOGE("str parameter is null.");
        return ERR;
    }
    ret = NextInt(s, &value);
    if (ret < 0) {
        TELEPHONY_LOGE("NextInt is fail, ret:%{public}d", ret);
        return ERR;
    }
    if (!(value == 0 || value == 1)) {
        TELEPHONY_LOGE("Bool should be 0 or 1, value:%{public}d", value);
        return ERR;
    }
    if (out != NULL) {
        *out = (char)value;
    }
    return ret;
}

int32_t ParseReportError(char *str)
{
    int32_t ret = VENDOR_FAIL;
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
    int32_t ret = VENDOR_FAIL;
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
        if (ret > 0) {
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

int32_t ConvertCharToInt32(const char *s)
{
    if (s == NULL) {
        return 0;
    }
    char *str = (char *)s;
    int32_t ret = 0;
    char firstChar = *str;
    if ((firstChar == '+') || (firstChar == '-')) {
        ++str;
    }
    while (*str == '0') {
        ++str;
    }
    while (*str) {
        char tmp = *str;
        if ((*str < '0') || (*str > '9')) {
            return ret;
        } else {
            int32_t val = (int32_t)(tmp - '0');
            ret = (ret * G_CHAR_TO_INT) + val;
        }
        str++;
    }
    if (firstChar == '-') {
        ret = -ret;
    }
    return ret;
}

int32_t FindCommaCharNum(const char *srcStr)
{
    char *str = (char *)srcStr;
    if (str == NULL) {
        TELEPHONY_LOGE("srcStr parameter is null.");
        return ERR;
    }
    if (*str == '\0') {
        return ERR;
    }
    int32_t charNum = 0;
    while (*str != '\0') {
        if (*str == ',') {
            charNum++;
        }
        str++;
    }
    return charNum;
}

int32_t GetSimSlotCount(void)
{
    char simSlotCount[PARAMETER_SIZE] = {0};
    GetParameter(TEL_SIM_SLOT_COUNT, DEFAULT_SLOT_COUNT, simSlotCount, PARAMETER_SIZE);
    return atoi(simSlotCount);
}

int32_t GetSlotId(const ReqDataInfo *requestInfo)
{
    int32_t slotId = HRIL_SIM_SLOT_0;
    char strSlotId[PARAMETER_SIZE] = {0};

    if (requestInfo != NULL) {
        slotId = requestInfo->slotId;
    } else { // proactive notification
        if (GetParameter(SUPPORT_SLOT_ID, "", strSlotId, PARAMETER_SIZE) > 0) {
            slotId = atoi(strSlotId);
        }
    }
    if (slotId >= GetSimSlotCount()) {
        slotId = HRIL_SIM_SLOT_0;
        TELEPHONY_LOGE("slotId is invalid, slotId0 will be used. slotId:%{public}d", slotId);
    }
    return slotId;
}
