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

#include "vendor_channel.h"

#include <stddef.h>
#include <sys/types.h>

#include "errno.h"
#include "securec.h"
#include "string.h"
#include "telephony_log_c.h"
#include "unistd.h"
#include "vendor_util.h"

static char g_buffer[MAX_RESPONSE_LEN + 1];
static char *g_bufferCur = g_buffer;

static char *FindEndOfLine(char *cur)
{
    char *endLine = cur;
    int32_t count = 2;
    if (endLine == NULL) {
        return NULL;
    }
    if (endLine[0] == '>' && endLine[1] == ' ' && endLine[count] == '\0') {
        return endLine + count;
    }
    while (*endLine != '\0' && *endLine != '\r' && *endLine != '\n') {
        endLine++;
    }
    return (*endLine != '\0') ? endLine : NULL;
}

static void SkipUnUseChar(void)
{
    while (*g_bufferCur == '\r' || *g_bufferCur == '\n') {
        g_bufferCur++;
    }
}

static void ClearCurBuffer(char **processed)
{
    if (processed == NULL) {
        return;
    }
    g_buffer[0] = '\0';
    g_bufferCur = g_buffer;
    *processed = g_bufferCur;
}

static char *ProcessLastResponse(char **processed)
{
    if (processed == NULL) {
        return NULL;
    }
    char *endLine = NULL;
    if (*g_bufferCur == '\0') {
        ClearCurBuffer(processed);
    } else {
        SkipUnUseChar();
        endLine = FindEndOfLine(g_bufferCur);
        if (endLine == NULL) {
            TELEPHONY_LOGE("g_bufferCur endLine is null\n");
            size_t len;
            len = strlen(g_bufferCur);
            (void)memmove_s(g_buffer, MAX_RESPONSE_LEN + 1, g_bufferCur, len + 1);
            *processed = g_buffer + len;
            g_bufferCur = g_buffer;
        }
    }
    return endLine;
}

const char *ReadResponse(int32_t atFd)
{
    ssize_t size;
    char *processed = NULL;
    char *endEol = NULL;
    char *ret = NULL;
    // process last response data
    endEol = ProcessLastResponse(&processed);
    while (endEol == NULL) {
        if (MAX_RESPONSE_LEN == (processed - &g_buffer[0])) {
            ClearCurBuffer(&processed);
        }
        do {
            size = read(atFd, processed, (MAX_RESPONSE_LEN - (processed - &g_buffer[0])));
        } while (size < 0 && errno == EINTR);
        if (size > 0) {
            processed[size] = '\0';
            SkipUnUseChar();
            endEol = FindEndOfLine(g_bufferCur);
            processed += size;
        } else if (size == 0) {
            TELEPHONY_LOGE("atchannel: EOF reached");
            return NULL;
        } else {
            TELEPHONY_LOGE("read error %{public}s", strerror(errno));
            return NULL;
        }
    }
    *endEol = '\0';
    ret = g_bufferCur;
    g_bufferCur = endEol + 1;
    return ret;
}

int32_t WriteATCommand(const char *s, int32_t isPdu, int32_t atFd)
{
    if (s == NULL) {
        return AT_ERR_GENERIC;
    }
    ssize_t ret;
    size_t i = 0;
    size_t len = strlen(s);
    if (atFd < 0) {
        return AT_ERR_CHANNEL_CLOSED;
    }

    while (i < len) {
        do {
            ret = write(atFd, s + i, len - i);
        } while (ret < 0 && errno == EINTR);

        if (ret < 0) {
            return AT_ERR_GENERIC;
        }
        i += ret;
    }
    if (isPdu != 0) {
        do {
            ret = write(atFd, "\x1A", 1);
        } while ((ret < 0 && errno == EINTR) || (ret == 0));
    } else {
        do {
            ret = write(atFd, "\r", 1);
        } while ((ret < 0 && errno == EINTR) || (ret == 0));
    }
    if (ret < 0) {
        return AT_ERR_GENERIC;
    }
    return VENDOR_SUCCESS;
}
