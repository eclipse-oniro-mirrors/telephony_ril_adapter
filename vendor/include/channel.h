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

#ifndef OHOS_CHANNEL_H
#define OHOS_CHANNEL_H
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include "telephony_log_c.h"
void ClearCurBuffer(char **processed);

char *FindEndOfLine(char *cur);

void SkipUnUseChar(void);

const char *ReadResponse(int atFd); // readLine

int WriteATCommand(const char *s, int isPdu, int atFd); // writeLine

#endif // OHOS_CHANNEL_H
