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
        explicit DemoHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner) : AppExecFwk::EventHandler(runner)
        {}
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
    void AddCallRequestToMap();
    void AddSmsRequestToMap();
    void AddSimRequestToMap();
    void AddDataRequestToMap();
    void AddNetworkRequestToMap();
    void AddModermRequestToMap();
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
    void SetRilNotificationFilterTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetRilDeviceStateTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilNetworkSearchInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetRilNetworkSelectionModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetRilNetworkSelectionModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetRilRadioCapabilityTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetPhysicalChannelConfigTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SendRilCmSmsTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SendRilCmSmsMoreModeTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetRilCmRadioPowerTest(const AppExecFwk::InnerEvent::Pointer &result);
    void ShutDownTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetRilRadioPowerTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetImeiTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetMeidTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetVoiceRadioTechnologyTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SendSmsAckTest(const AppExecFwk::InnerEvent::Pointer &result);
    void ActivatePdpContextTest(const AppExecFwk::InnerEvent::Pointer &result);
    void DeactivatePdpContextTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetUssdTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetUssdTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetClipTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetClipTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetCallWaitingTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetCallWaitingTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetCallRestrictionTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetCallRestrictionTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetCallTransferInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetCallTransferInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetClirTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetClirTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetLinkBandwidthInfoTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetDataProfileInfoTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SendDataPerformanceModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SendDataSleepModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPINTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPIN2Test(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPUKTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPUK2Test(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void ChangeSimPasswordTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetSimLockTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetSimLockStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetEmergencyCallListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetBarringPasswordTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SimOpenLogicalChannelTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SimTransmitApduLogicalChannelTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SimTransmitApduBasicChannelTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SimCloseLogicalChannelTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetActiveSimTest(const AppExecFwk::InnerEvent::Pointer &result);

private:
    std::unique_ptr<RilManagerTest> mRilManager_;
    using RilManagerAndResponseTestFun = void (RilUnitTest::*)(const OHOS::AppExecFwk::InnerEvent::Pointer &event);
    std::map<uint32_t, RilManagerAndResponseTestFun> memberFuncMap_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_UNITTEST_H
