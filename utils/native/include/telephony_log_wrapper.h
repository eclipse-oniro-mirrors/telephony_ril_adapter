/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef TELEPHONY_LOG_WRAPPER_H
#define TELEPHONY_LOG_WRAPPER_H

#include <string>

#include "hilog/log.h"

namespace OHOS {
namespace Telephony {
#ifndef LOG_DOMAIN
#define LOG_DOMAIN 0xD001F00
#endif
#ifndef TELEPHONY_LOG_TAG
#define TELEPHONY_LOG_TAG "TelephonySubsystem"
#endif

static constexpr OHOS::HiviewDFX::HiLogLabel TELEPHONY_LABEL = {LOG_CORE, LOG_DOMAIN, TELEPHONY_LOG_TAG};

#define OHOS_DEBUG
#ifndef OHOS_DEBUG
#define PRINT_TELEPHONY_LOG(op, fmt, ...) (void)OHOS::HiviewDFX::HiLog::op(TELEPHONY_LABEL, fmt, ##__VA_ARGS__)
#else
// Gets the raw file name of the file.
// This function is a function executed by the compiler, that is,
// it has been executed at compile time. When the program runs,
// it directly refers to the value calculated by this function
// and does not consume CPU for calculation.
inline constexpr const char *GetRawFileName(const char *path)
{
    char ch = '/';
    const char *start = path;
    // get the end of the string
    while (*start++) {
        ;
    }
    while (--start != path && *start != ch) {
        ;
    }

    return (*start == ch) ? ++start : path;
}

#define PRINT_TELEPHONY_LOG(op, fmt, ...)                                                                        \
    (void)OHOS::HiviewDFX::HiLog::op(TELEPHONY_LABEL, "[%{public}s-(%{public}s:%{public}d)] " fmt, __FUNCTION__, \
        GetRawFileName(__FILE__), __LINE__, ##__VA_ARGS__)
#endif

#define TELEPHONY_LOGE(fmt, ...) PRINT_TELEPHONY_LOG(Error, fmt, ##__VA_ARGS__)
#define TELEPHONY_LOGW(fmt, ...) PRINT_TELEPHONY_LOG(Warn, fmt, ##__VA_ARGS__)
#define TELEPHONY_LOGI(fmt, ...) PRINT_TELEPHONY_LOG(Info, fmt, ##__VA_ARGS__)
#define TELEPHONY_LOGF(fmt, ...) PRINT_TELEPHONY_LOG(Fatal, fmt, ##__VA_ARGS__)
#define TELEPHONY_LOGD(fmt, ...) PRINT_TELEPHONY_LOG(Debug, fmt, ##__VA_ARGS__)
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_TELEPHONY_LOG_WRAPPER_H