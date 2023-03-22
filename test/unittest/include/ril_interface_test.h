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
#include "hdf_base.h"
#include "hdf_log.h"
#include "ril_callback_test.h"
#include "securec.h"
#include "stdlib.h"
#include "unistd.h"
namespace OHOS {
namespace Telephony {
class RilInterfaceTest {
public:
    void GetImeiStressTest(int32_t slotId);
    void RilCmDialStressTest(int32_t slotId);
    void SetRadioStateStressTest(int32_t slotId);
    void GetCallListTest(int32_t slotId);
    void RilCmDialTest(int32_t slotId);
    void HangupTest(int32_t slotId);
    void RejectTest(int32_t slotId);
    void AnswerCallTest(int32_t slotId);
    void HoldCallTest(int32_t slotId);
    void UnHoldCallTest(int32_t slotId);
    void SwitchCallTest(int32_t slotId);
    void RilCmJoinCallTest(int32_t slotId);
    void RilCmSplitCallTest(int32_t slotId);
    void CallSupplementTest(int32_t slotId);
    void SendDtmfTest(int32_t slotId);
    void StartDtmfTest(int32_t slotId);
    void StopDtmfTest(int32_t slotId);
    void SetUssdCusdTest(int32_t slotId);
    void GetUssdCusdTest(int32_t slotId);
    void SetMuteTest(int32_t slotId);
    void GetMuteTest(int32_t slotId);
    void GetEmergencyListTest(int32_t slotId);
    void SetEmergencyCallListTest(int32_t slotId);
    void GetFailReasonTest(int32_t slotId);
    void SetBarringPasswordTest(int32_t slotId);
    void IccRilSimIoForAppTest(int32_t slotId);
    void GetRilCmImsiForAppTest(int32_t slotId);
    void GetRilCmSignalIntensityTest(int32_t slotId);
    void GetRilCmIccCardStatusTest(int32_t slotId);
    void GetRilCmCsRegStatusTest(int32_t slotId);
    void GetRilCmPsRegStatusTest(int32_t slotId);
    void GetRilCmOperatorTest(int32_t slotId);
    void SendRilCmSmsTest(int32_t slotId);
    void SendRilCmSmsMoreModeTest(int32_t slotId);
    void SendSmsAckTest(int32_t slotId);
    void GetSmscAddrTest(int32_t slotId);
    void SetSmscAddrTest(int32_t slotId);
    void SetRadioStateTest(int32_t slotId);
    void GetRadioStateTest(int32_t slotId);
    void GetImeiTest(int32_t slotId);
    void GetRilCmCellInfoListTest(int32_t slotId);
    void GetRilCurrentCellInfoTest(int32_t slotId);
    void GetRilNetworkSearchInfoTest(int32_t slotId);
    void GetRilNetworkSelectionModeTest(int32_t slotId);
    void GetRilPreferredNetwrokTest(int32_t slotId);
    void GetPhysicalChannelConfigTest(int32_t slotId);
    void GetRilImeiTest(int32_t slotId);
    void ShutDownTest(int32_t slotId);
    void GetMeidTest(int32_t slotId);
    void GetVoiceRadioTechnologyTest(int32_t slotId);
    void GetBasebandVersionTest(int32_t slotId);
    void SendDataPerformanceModeTest(int32_t slotId);
    void SendDataSleepModeTest(int32_t slotId);
    void SetRilPreferredNetworkTest(int32_t slotId);
    void SetRilNetworkSelectionModeTest(int32_t slotId);
    void SetRilLocationUpdateTest(int32_t slotId);
    void SetRilNotificationFilterTest(int32_t slotId);
    void SetRilDeviceStateTest(int32_t slotId);
    void ActivatePdpContextTest(int32_t slotId);
    void DeactivatePdpContextTest(int32_t slotId);
    void GetPdpContextListTest(int32_t slotId);
    void SetInitialApnTest(int32_t slotId);
    void SetDataProfileInfoTest(int32_t slotId);
    void SetDataPermittedTest(int32_t slotId);
    void SetLinkBandwidthReportingRuleTest(int32_t slotId);
    void OnProcessInput(int32_t what);
    void OnStressInput(int32_t what);
    void SimOpenLogicalChannelTest(int32_t slotId);
    void SimCloseLogicalChannelTest(int32_t slotId);
    void SimTransmitApduLogicalChannelTest(int32_t slotId);
    void SimAuthenticationTest(int32_t slotId);
    void SetActiveSimTest(int32_t slotId);
    void SimTransmitApduBasicChannelTest(int32_t slotId);
    void UnlockSimLockTest(int32_t slotId);
    void GetLinkBandwidthInfoTest(int32_t slotId);
    void UnLockPINTest(int32_t slotId);
    void UnLockPIN2Test(int32_t slotId);
    void UnLockPUKTest(int32_t slotId);
    void UnLockPUK2Test(int32_t slotId);
    void ChangeSimPasswordTest(int32_t slotId);
    void SetSimLockTest(int32_t slotId);
    void GetSimLockStatusTest(int32_t slotId);
    void SendTerminalResponseCmdTest(int32_t slotId);
    void SendEnvelopeCmdTest(int32_t slotId);
    void SendCallSetupRequestResultTest(int32_t slotId);
    void SimStkIsReadyTest(int32_t slotId);
    void OnInitInterface();
    void OnInitStressInterface();
    void OnInitCallProcessInterface();
    void OnInitSmsProcessInterface();
    void OnInitSimProcessInterface();
    void OnInitDataProcessInterface();
    void OnInitNetworkProcessInterface();
    void OnInitModemProcessInterface();
    void OnInitProcessInterface();
    int32_t GetSerialId();
    void InputTransmitApduChannelParam(OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &reqInfo);

private:
    using RilInterfaceTestTestFun = void (RilInterfaceTest::*)(int32_t slotId);
    std::map<uint32_t, RilInterfaceTestTestFun> memberFuncMap_;
    std::map<uint32_t, RilInterfaceTestTestFun> stressMemberFuncMap_;
};

} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_INTERFACE_TEST_H
