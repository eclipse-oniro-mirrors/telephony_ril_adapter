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

#ifndef OHOS_VENDOR_CHANNEL_H
#define OHOS_VENDOR_CHANNEL_H

#include <stdint.h>

// readLine from modem device
const char *ReadResponse(int32_t atFd);
// writeLine to modem device
int32_t WriteATCommand(const char *s, int32_t isPdu, int32_t atFd);
#endif // OHOS_CHANNEL_H
