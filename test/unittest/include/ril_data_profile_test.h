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

#ifndef OHOS_RIL_DATA_PROFILE_TEST_H
#define OHOS_RIL_DATA_PROFILE_TEST_H

namespace OHOS {
namespace Telephony {
class RilDataProfileTest {
public:
    RilDataProfileTest(int profileId, std::string apn, std::string protocol, int verType, std::string userName,
        std::string password, std::string roamingProtocol)
        : profileId(profileId), apn(apn), protocol(protocol), verType(verType), userName(userName),
          password(password), roamingProtocol(roamingProtocol)
    {}

    virtual ~RilDataProfileTest() = default;

public:
    int profileId;
    std::string apn;
    std::string protocol;
    int verType;
    std::string userName;
    std::string password;
    std::string roamingProtocol;
};
} // namespace Telephony
} // namespace OHOS
#endif // HOS_RIL_DATA_PROFILE_TEST_H