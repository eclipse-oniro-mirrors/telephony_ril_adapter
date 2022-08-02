/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#include "event_handler.h"
#include "ril_manager_test.h"

namespace OHOS {
namespace Telephony {
const int32_t NUM_CIRCLES = 4;
namespace {
class RilUnitTest {
public:
    explicit RilUnitTest(int32_t opt);
    ~RilUnitTest();

    void OnInit();
    void OnInitForRegister(const std::unique_ptr<AppExecFwk::EventHandler> &handler);

    void GetImeiStressTest(const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmDialStressTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetRilCmRadioPowerStressTest(const AppExecFwk::InnerEvent::Pointer &result);

    void GetCallListTest(const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmDialTest(const AppExecFwk::InnerEvent::Pointer &result);
    void HangupTest(const AppExecFwk::InnerEvent::Pointer &result);
    void RejectTest(const AppExecFwk::InnerEvent::Pointer &result);
    void AnswerCallTest(const AppExecFwk::InnerEvent::Pointer &result);
    void HoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void UnHoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SwitchCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void RilCmJoinCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void RilCmSplitCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void CallSupplementTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SendDtmfTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void StartDtmfTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void StopDtmfTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetUssdCusdTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetUssdCusdTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetMuteTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetMuteTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetEmergencyListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetEmergencyCallListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetFailReasonTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetBarringPasswordTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);

    void IccRilSimIoForAppTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmImsiForAppTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmSignalIntensityTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmIccCardStatusTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmCsRegStatusTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmPsRegStatusTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmOperatorTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SendRilCmSmsTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SendRilCmSmsMoreModeTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SendSmsAckTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetSmscAddrTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetSmscAddrTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SetRilCmRadioPowerTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilRadioPowerTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetImeiTest(const AppExecFwk::InnerEvent::Pointer &result);
    void AcknowledgeRilCmLastIncomingGsmSmsTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmCellInfoListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCurrentCellInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetRilNetworkSearchInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetRilNetworkSelectionModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetRilPreferredNetwrokTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetPhysicalChannelConfigTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetRilRadioCapabilityTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetRilImeiTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void ShutDownTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetMeidTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetVoiceRadioTechnologyTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetBasebandVersionTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SendDataPerformanceModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SendDataSleepModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetRilPreferredNetwrokTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetRilNetworkSelectionModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetRilLocationUpdateTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetRilNotificationFilterTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetRilDeviceStateTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetupRilCmDataCallTest(const AppExecFwk::InnerEvent::Pointer &result);
    void DeactivateRilCmDataCallTest(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmDataCallListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetInitialApnTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void OnProcessInput(int32_t what, const OHOS::AppExecFwk::InnerEvent::Pointer &event);
    void OnStressInput(int32_t what, const OHOS::AppExecFwk::InnerEvent::Pointer &event);
    void SimOpenLogicalChannel(const AppExecFwk::InnerEvent::Pointer &result);
    void SimCloseLogicalChannel(const AppExecFwk::InnerEvent::Pointer &result);
    void SimTransmitApduLogicalChannel(const AppExecFwk::InnerEvent::Pointer &result);
    void SimAuthentication(const AppExecFwk::InnerEvent::Pointer &result);
    void SetActiveSimTest(const AppExecFwk::InnerEvent::Pointer &result);
    void SimTransmitApduBasicChannel(const AppExecFwk::InnerEvent::Pointer &result);
    void UnlockSimLock(const AppExecFwk::InnerEvent::Pointer &result);
    void GetLinkBandwidthInfoTest(const AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPINTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPIN2Test(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPUKTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPUK2Test(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void ChangeSimPasswordTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void SetSimLockTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void GetSimLockStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result);
    void OnInitInterface();
    void OnInitStressInterface();
    void OnInitCallProcessInterface();
    void OnInitSmsProcessInterface();
    void OnInitSimProcessInterface();
    void OnInitDataProcessInterface();
    void OnInitNetworkProcessInterface();
    void OnInitModemProcessInterface();
    void OnInitProcessInterface();
    void GetTransmitApduChannelParam(
        int32_t &channelId, int32_t &type, int32_t &instruction, int32_t &p1, int32_t &p2, int32_t &p3);

    class DemoHandler : public AppExecFwk::EventHandler {
    public:
        explicit DemoHandler(const std::shared_ptr<AppExecFwk::EventRunner> &runner) : AppExecFwk::EventHandler(runner)
        {}
        ~DemoHandler() {}

        void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event) override;
    };

    std::unique_ptr<RilManagerTest> mRilManager_;
    int32_t slotId_;
    using RilManagerAndResponseTestFun = void (RilUnitTest::*)(const OHOS::AppExecFwk::InnerEvent::Pointer &event);
    std::map<uint32_t, RilManagerAndResponseTestFun> memberFuncMap_;
    std::map<uint32_t, RilManagerAndResponseTestFun> stressMemberFuncMap_;
};
} // namespace
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_UNITTEST_H
