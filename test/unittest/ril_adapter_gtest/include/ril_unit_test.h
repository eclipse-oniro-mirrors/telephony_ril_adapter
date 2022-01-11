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

#ifndef OHOS_RIL_UNITTEST_H
#define OHOS_RIL_UNITTEST_H

#include <gtest/gtest.h>

#include "event_handler.h"
#include "ril_manager_test.h"

namespace OHOS {
namespace Telephony {
class RilUnitTest : public testing::Test {
public:
    class DemoHandler : public AppExecFwk::EventHandler {
    public:
        DemoHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner) : AppExecFwk::EventHandler(runner) {}
        ~DemoHandler() {}

        void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event) override;
    };

    void SetUp();
    void TearDown();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void OnInit();
    void OnProcessTest(int32_t index, const OHOS::AppExecFwk::InnerEvent::Pointer &event);
    std::shared_ptr<RilUnitTest::DemoHandler> GetHandler();

public:
    std::shared_ptr<RilUnitTest::DemoHandler> handler_;

private:
    void AddRequestToMap();
    void GetCallListTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetSimIOTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetImsiTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmSignalStrengthTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetSimStatusTest(const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmDialTest(const AppExecFwk::InnerEvent::Pointer &result);
    void RejectCallTest(const AppExecFwk::InnerEvent::Pointer &result);
    void HoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void UnHoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SwitchCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void RilCmJoinCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void RilCmSplitCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void HangupRilCmConnectionTest(const AppExecFwk::InnerEvent::Pointer &result);
    void AcceptRilCmCallTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmCsRegStatusTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmPsRegStatusTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmOperatorTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SendRilCmSmsTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SendRilCmSmsMoreModeTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetRilCmRadioPowerTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SendSmsAckTest(const AppExecFwk::InnerEvent::Pointer &result);
    void ActivatePdpContextTest(const AppExecFwk::InnerEvent::Pointer &result);
    void DeactivatePdpContextTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetUssdCusdTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetUssdCusdTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetLinkBandwidthInfoTest(const AppExecFwk::InnerEvent::Pointer &result);

private:
    std::unique_ptr<RilManagerTest> mRilManager_;
    using RilManagerAndResponseTestFun = void (RilUnitTest::*)(const OHOS::AppExecFwk::InnerEvent::Pointer &event);
    std::map<uint32_t, RilManagerAndResponseTestFun> memberFuncMap_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_UNITTEST_H
