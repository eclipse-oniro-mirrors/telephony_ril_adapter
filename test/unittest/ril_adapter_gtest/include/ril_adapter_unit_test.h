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
const int NUM_CIRCLES = 4;
class RilAdapterUnitTest : public testing::Test {
public:
    // execute before first testcase
    static void SetUpTestCase();
    void SetUp();
    void TearDown();
    static void TearDownTestCase();

    void OnInit();
    void OnInitForRegister(const std::unique_ptr<AppExecFwk::EventHandler> &handler);
    void GetRilCmCurrentCallsTest(const AppExecFwk::InnerEvent::Pointer &result);
    void IccRilCmIoForAppTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmImsiForAppTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmSignalIntensityTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetLastCallErrorCodeTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmIccCardStatusTest(const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmDialByUusInfoTest(const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmDialTest(const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmRejectCallTest(const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmHoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void RilCmActiveCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void RilCmSwapCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
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
    void AcknowledgeRilCmLastIncomingGsmSmsTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetupRilCmDataCallTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetRilLocationUpdateForNetworksTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void DeactivateRilCmDataCallTest(const AppExecFwk::InnerEvent::Pointer &result);
    void OnProcessInput(int32_t what, const OHOS::AppExecFwk::InnerEvent::Pointer &event);
    void OnInitInterface();
    void SetSlotId(int slotId);

    class DemoHandler : public AppExecFwk::EventHandler {
    public:
        DemoHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner) : AppExecFwk::EventHandler(runner) {}
        ~DemoHandler() {}

        void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event) override;
    };

    std::unique_ptr<RilManagerTest> mRilManager_;
    int slotId_;
    using RilManagerAndResponseTestFun = void (RilAdapterUnitTest::*)(
        const OHOS::AppExecFwk::InnerEvent::Pointer &event);
    std::map<uint32_t, RilManagerAndResponseTestFun> memberFuncMap_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_UNITTEST_H
