/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_RIL_INTERFACE_TEST_H
#define OHOS_RIL_INTERFACE_TEST_H

#include "cmath"
#include "cstdio"
#include "gtest/gtest.h"
#include "hdf_base.h"
#include "hdf_log.h"
#include "securec.h"
#include "stdlib.h"
#include "unistd.h"

namespace OHOS {
namespace Telephony {
class RilInterfaceTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_INTERFACE_TEST_H
