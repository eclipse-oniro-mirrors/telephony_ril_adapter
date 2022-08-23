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

#include "ril_unit_test.h"

#include <iostream>

#include "hril_request.h"
#include "telephony_log_wrapper.h"

using namespace testing::ext;
namespace OHOS {
namespace Telephony {
const int32_t CI = 1;
const int32_t REASON = 2;
const int32_t REQUEST_INFO_FILEID = 2;
const int32_t REQUEST_INFO_P1 = 4;
const int32_t REQUEST_INFO_P2 = 5;
const int32_t REQUEST_INFO_P3 = 6;

void RilUnitTest::SetUp() {}

void RilUnitTest::SetUpTestCase()
{
    std::cout << "----------RilUnitTest  start ------------" << std::endl;
}

void RilUnitTest::TearDown() {}

void RilUnitTest::TearDownTestCase()
{
    std::cout << "----------RilUnitTest  end ------------" << std::endl;
}

void RilUnitTest::OnInit()
{
    TELEPHONY_LOGI("RilUnitTest OnInit");
    std::shared_ptr<AppExecFwk::EventRunner> runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler_ = std::make_shared<RilUnitTest::DemoHandler>(runner);
    const int32_t cdmaSubscription = 1;
    mRilManager_ = std::make_unique<RilManagerTest>(0, cdmaSubscription);
    if (mRilManager_ == nullptr) {
        return;
    }
    mRilManager_->OnInit();
}

std::shared_ptr<RilUnitTest::DemoHandler> RilUnitTest::GetHandler()
{
    return handler_;
}

void RilUnitTest::OnProcessTest(int32_t index, const OHOS::AppExecFwk::InnerEvent::Pointer &event)
{
    for (auto itFunc : memberFuncMap_) {
        int32_t val = static_cast<int32_t>(itFunc.first);
        if (val == index) {
            auto memberFunc = itFunc.second;
            (this->*memberFunc)(event);
        }
    }
}

void RilUnitTest::AddRequestToMap()
{
    AddCallRequestToMap();
    AddSmsRequestToMap();
    AddSimRequestToMap();
    AddDataRequestToMap();
    AddNetworkRequestToMap();
    AddModermRequestToMap();
}

void RilUnitTest::AddCallRequestToMap()
{
    memberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &RilUnitTest::GetCallListTest;
    memberFuncMap_[HREQ_CALL_DIAL] = &RilUnitTest::RilCmDialTest;
    memberFuncMap_[HREQ_CALL_REJECT] = &RilUnitTest::RejectCallTest;
    memberFuncMap_[HREQ_CALL_HANGUP] = &RilUnitTest::HangupRilCmConnectionTest;
    memberFuncMap_[HREQ_CALL_ANSWER] = &RilUnitTest::AcceptRilCmCallTest;
    memberFuncMap_[HREQ_CALL_HOLD_CALL] = &RilUnitTest::HoldCallTest;
    memberFuncMap_[HREQ_CALL_UNHOLD_CALL] = &RilUnitTest::UnHoldCallTest;
    memberFuncMap_[HREQ_CALL_SWITCH_CALL] = &RilUnitTest::SwitchCallTest;
    memberFuncMap_[HREQ_CALL_SET_EMERGENCY_LIST] = &RilUnitTest::SetEmergencyCallListTest;
    memberFuncMap_[HREQ_CALL_SET_BARRING_PASSWORD] = &RilUnitTest::SetBarringPasswordTest;
    memberFuncMap_[HREQ_CALL_COMBINE_CONFERENCE] = &RilUnitTest::RilCmJoinCallTest;
    memberFuncMap_[HREQ_CALL_SEPARATE_CONFERENCE] = &RilUnitTest::RilCmSplitCallTest;
    memberFuncMap_[HREQ_CALL_SET_USSD] = &RilUnitTest::SetUssdTest;
    memberFuncMap_[HREQ_CALL_GET_USSD] = &RilUnitTest::GetUssdTest;
    memberFuncMap_[HREQ_CALL_SET_CLIP] = &RilUnitTest::SetClipTest;
    memberFuncMap_[HREQ_CALL_GET_CLIP] = &RilUnitTest::GetClipTest;
    memberFuncMap_[HREQ_CALL_SET_CALL_WAITING] = &RilUnitTest::SetCallWaitingTest;
    memberFuncMap_[HREQ_CALL_GET_CALL_WAITING] = &RilUnitTest::GetCallWaitingTest;
    memberFuncMap_[HREQ_CALL_SET_CALL_RESTRICTION] = &RilUnitTest::SetCallRestrictionTest;
    memberFuncMap_[HREQ_CALL_GET_CALL_RESTRICTION] = &RilUnitTest::GetCallRestrictionTest;
    memberFuncMap_[HREQ_CALL_SET_CALL_TRANSFER_INFO] = &RilUnitTest::SetCallTransferInfoTest;
    memberFuncMap_[HREQ_CALL_GET_CALL_TRANSFER_INFO] = &RilUnitTest::GetCallTransferInfoTest;
    memberFuncMap_[HREQ_CALL_SET_CLIR] = &RilUnitTest::SetClirTest;
    memberFuncMap_[HREQ_CALL_GET_CLIR] = &RilUnitTest::GetClirTest;
}

void RilUnitTest::AddSmsRequestToMap()
{
    memberFuncMap_[HREQ_SMS_SEND_GSM_SMS] = &RilUnitTest::SendRilCmSmsTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &RilUnitTest::SendRilCmSmsMoreModeTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_ACK] = &RilUnitTest::SendSmsAckTest;
}

void RilUnitTest::AddSimRequestToMap()
{
    memberFuncMap_[HREQ_SIM_OPEN_LOGICAL_CHANNEL] = &RilUnitTest::SimOpenLogicalChannelTest;
    memberFuncMap_[HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL] = &RilUnitTest::SimTransmitApduLogicalChannelTest;
    memberFuncMap_[HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL] = &RilUnitTest::SimTransmitApduBasicChannelTest;
    memberFuncMap_[HREQ_SIM_CLOSE_LOGICAL_CHANNEL] = &RilUnitTest::SimCloseLogicalChannelTest;
    memberFuncMap_[HREQ_SIM_SET_ACTIVE_SIM] = &RilUnitTest::SetActiveSimTest;
    memberFuncMap_[HREQ_SIM_UNLOCK_PIN] = &RilUnitTest::UnLockPINTest;
    memberFuncMap_[HREQ_SIM_UNLOCK_PIN2] = &RilUnitTest::UnLockPIN2Test;
    memberFuncMap_[HREQ_SIM_UNLOCK_PUK] = &RilUnitTest::UnLockPUKTest;
    memberFuncMap_[HREQ_SIM_UNLOCK_PUK2] = &RilUnitTest::UnLockPUK2Test;
    memberFuncMap_[HREQ_SIM_CHANGE_SIM_PASSWORD] = &RilUnitTest::ChangeSimPasswordTest;
    memberFuncMap_[HREQ_SIM_SET_SIM_LOCK] = &RilUnitTest::SetSimLockTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_LOCK_STATUS] = &RilUnitTest::GetSimLockStatusTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_IO] = &RilUnitTest::GetSimIOTest;
    memberFuncMap_[HREQ_SIM_GET_IMSI] = &RilUnitTest::GetImsiTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &RilUnitTest::GetSimStatusTest;
    memberFuncMap_[HREQ_SIM_STK_SEND_TERMINAL_RESPONSE] = &RilUnitTest::SendTerminalResponseCmdTest;
    memberFuncMap_[HREQ_SIM_STK_SEND_ENVELOPE] = &RilUnitTest::SendEnvelopeCmdTest;
    memberFuncMap_[HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT] = &RilUnitTest::SendCallSetupRequestResultTest;
    memberFuncMap_[HREQ_SIM_STK_IS_READY] = &RilUnitTest::SimStkIsReadyTest;
}

void RilUnitTest::AddDataRequestToMap()
{
    memberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &RilUnitTest::ActivatePdpContextTest;
    memberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &RilUnitTest::DeactivatePdpContextTest;
    memberFuncMap_[HREQ_DATA_GET_LINK_BANDWIDTH_INFO] = &RilUnitTest::GetLinkBandwidthInfoTest;
    memberFuncMap_[HREQ_DATA_SET_DATA_PROFILE_INFO] = &RilUnitTest::SetDataProfileInfoTest;
    memberFuncMap_[HREQ_DATA_SET_DATA_PERMITTED] = &RilUnitTest::SetDataPermittedTest;
}

void RilUnitTest::AddNetworkRequestToMap()
{
    memberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &RilUnitTest::GetRilCmSignalStrengthTest;
    memberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &RilUnitTest::GetRilCmOperatorTest;
    memberFuncMap_[HREQ_NETWORK_SET_NOTIFICATION_FILTER] = &RilUnitTest::SetRilNotificationFilterTest;
    memberFuncMap_[HREQ_NETWORK_SET_DEVICE_STATE] = &RilUnitTest::SetRilDeviceStateTest;
    memberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &RilUnitTest::GetRilCmCsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &RilUnitTest::GetRilCmPsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION] = &RilUnitTest::GetRilNetworkSearchInfoTest;
    memberFuncMap_[HREQ_NETWORK_GET_NETWORK_SELECTION_MODE] = &RilUnitTest::GetRilNetworkSelectionModeTest;
    memberFuncMap_[HREQ_NETWORK_SET_NETWORK_SELECTION_MODE] = &RilUnitTest::SetRilNetworkSelectionModeTest;
    memberFuncMap_[HREQ_NETWORK_GET_RADIO_CAPABILITY] = &RilUnitTest::GetRilRadioCapabilityTest;
    memberFuncMap_[HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG] = &RilUnitTest::GetPhysicalChannelConfigTest;
}

void RilUnitTest::AddModermRequestToMap()
{
    memberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &RilUnitTest::SetRilCmRadioPowerTest;
    memberFuncMap_[HREQ_MODEM_SHUT_DOWN] = &RilUnitTest::ShutDownTest;
    memberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &RilUnitTest::GetRilRadioPowerTest;
    memberFuncMap_[HREQ_MODEM_GET_IMEI] = &RilUnitTest::GetImeiTest;
    memberFuncMap_[HREQ_MODEM_GET_MEID] = &RilUnitTest::GetMeidTest;
    memberFuncMap_[HREQ_MODEM_GET_VOICE_RADIO] = &RilUnitTest::GetVoiceRadioTechnologyTest;
}

void RilUnitTest::GetCallListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetCallListTest -->");
    int32_t ret = mRilManager_->GetCallList(result);
    TELEPHONY_LOGI("RilUnitTest::GetCallListTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetSimIOTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetSimIOTest -->");
    SimIoRequestInfo requestInfo;
    requestInfo.command = 1;
    requestInfo.fileId = REQUEST_INFO_FILEID;
    requestInfo.path = "ReadIccFile";
    requestInfo.p1 = REQUEST_INFO_P1;
    requestInfo.p2 = REQUEST_INFO_P2;
    requestInfo.p3 = REQUEST_INFO_P3;
    requestInfo.data = "7";
    int32_t ret = mRilManager_->GetSimIO(requestInfo, result);
    TELEPHONY_LOGI("RilUnitTest::GetSimIOTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetImsiTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetImsiTest -->");
    int32_t ret = mRilManager_->GetImsi("GetImsi", result);
    TELEPHONY_LOGI("RilUnitTest::GetImsiTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetRilCmSignalStrengthTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmSignalStrengthTest -->");
    int32_t ret =
        mRilManager_->GetRilCmSignalStrength(result->GetOwner(), static_cast<int32_t>(result->GetInnerEventId()));
    TELEPHONY_LOGI("RilUnitTest::GetRilCmSignalStrengthTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetSimStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetSimStatusTest -->");
    int32_t ret = mRilManager_->GetSimStatus(result);
    TELEPHONY_LOGI("RilUnitTest::GetSimStatusTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::RilCmDialTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    std::string phoneNum;
    int32_t clirMode; /* Calling Line Identification Restriction . From TS 27.007 V3.4.0 (2000-03) */

    TELEPHONY_LOGI("RilUnitTest::RilCmDialTest -->");

    std::cout << "please enter the phone number:";
    std::cin >> phoneNum;
    clirMode = 0; // use subscription default value

    int32_t ret = mRilManager_->RilCmDial(phoneNum, clirMode, result);
    TELEPHONY_LOGI("RilUnitTest::RilCmDialTest --> RilCmDialTest finished");
    TELEPHONY_LOGI("RilUnitTest::RilCmDialTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::RejectCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::RejectCallTest -->");
    int32_t ret = mRilManager_->Reject(result);
    TELEPHONY_LOGI("RilUnitTest::RejectCallTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::HoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::HoldCallTest -->");
    int32_t ret = mRilManager_->HoldCall(result);
    TELEPHONY_LOGI("RilUnitTest::HoldCallTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::UnHoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::UnHoldCallTest -->");
    int32_t ret = mRilManager_->UnHoldCall(result);
    TELEPHONY_LOGI("RilUnitTest::UnHoldCallTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SwitchCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SwitchCallTest -->");
    int32_t ret = mRilManager_->SwitchCall(result);
    TELEPHONY_LOGI("RilUnitTest::SwitchCallTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::RilCmJoinCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t callType; /* call type
                       * 0: Voice call
                       * 1: Video call: send one-way video, two-way voice
                       * 2: Video call: one-way receiving video, two-way voice
                       * 3: Video call: two-way video, two-way voice
                       */

    TELEPHONY_LOGI("RilUnitTest::RilCmJoinCallTest -->");

    std::cout << "please enter the call type:" << std::endl;
    std::cout << "0: Voice call" << std::endl;
    std::cout << "1: Video call: send one-way video, two-way voice" << std::endl;
    std::cout << "2: Video call: one-way receiving video, two-way voice" << std::endl;
    std::cout << "3: Video call: two-way video, two-way voice" << std::endl;
    std::cin >> callType;

    int32_t ret = mRilManager_->RilCmJoin(callType, result);
    TELEPHONY_LOGI("RilUnitTest::RilCmJoinCallTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::RilCmSplitCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t callIndex;
    int32_t callType; /* call type
                       * 0: Voice call
                       * 1: Video call: send one-way video, two-way voice
                       * 2: Video call: one-way receiving video, two-way voice
                       * 3: Video call: two-way video, two-way voice
                       */

    TELEPHONY_LOGI("RilUnitTest::RilCmSplitCallTest -->");

    std::cout << "please enter the call split number:";
    std::cin >> callIndex;

    std::cout << "please enter the call type:" << std::endl;
    std::cout << "0: Voice call" << std::endl;
    std::cout << "1: Video call: send one-way video, two-way voice" << std::endl;
    std::cout << "2: Video call: one-way receiving video, two-way voice" << std::endl;
    std::cout << "3: Video call: two-way video, two-way voice" << std::endl;
    std::cin >> callType;

    int32_t ret = mRilManager_->RilCmSplit(callIndex, callType, result);
    TELEPHONY_LOGI("RilUnitTest::RilCmSplitCallTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetEmergencyCallListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetEmergencyCallListTest -->");
    int32_t ret = mRilManager_->SetEmergencyCallList(result);
    TELEPHONY_LOGI("RilUnitTest::SetEmergencyCallListTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::HangupRilCmConnectionTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::HangupRilCmConnectionTest -->");
    int32_t ret = mRilManager_->Hangup(static_cast<int32_t>(result->GetInnerEventId()), result);
    TELEPHONY_LOGI("RilUnitTest::HangupRilCmConnectionTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::AcceptRilCmCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::AcceptRilCmCallTest -->");
    int32_t ret = mRilManager_->Answer(result);
    TELEPHONY_LOGI("RilUnitTest::AcceptRilCmCallTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetRilCmCsRegStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmCsRegStatusTest -->");
    int32_t ret = mRilManager_->GetCsRegStatus(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCmCsRegStatusTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetRilCmPsRegStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmPsRegStatusTest -->");
    int32_t ret = mRilManager_->GetPsRegStatus(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCmPsRegStatusTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetRilCmOperatorTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmOperatorTest -->");
    int32_t ret = mRilManager_->GetOperatorInfo(result->GetOwner(), static_cast<int32_t>(result->GetInnerEventId()));
    TELEPHONY_LOGI("RilUnitTest::GetRilCmOperatorTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetRilNotificationFilterTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetRilNotificationFilterTest -->");
    int32_t ret = mRilManager_->SetNotificationFilter(15, result);
    TELEPHONY_LOGI("RilUnitTest::SetRilNotificationFilterTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetRilDeviceStateTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetRilDeviceStateTest -->");
    int32_t ret = mRilManager_->SetDeviceState(0, 1, result);
    TELEPHONY_LOGI("RilUnitTest::SetRilDeviceStateTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetRilNetworkSearchInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilNetworkSearchInfoTest -->");
    int32_t ret = mRilManager_->GetNetworkSearchInfo(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilNetworkSearchInfoTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetRilNetworkSelectionModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilNetworkSelectionModeTest -->");
    int32_t ret = mRilManager_->GetNetworkSelectionMode(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilNetworkSelectionModeTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetRilNetworkSelectionModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetRilNetworkSelectionModeTest -->");
    int32_t mode = 1;
    string plmn = "46000";
    int32_t ret = mRilManager_->SetNetworkSelectionMode(mode, plmn, result);
    TELEPHONY_LOGI("RilUnitTest::SetRilNetworkSelectionModeTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetRilRadioCapabilityTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilRadioCapabilityTest -->");
    int32_t ret = mRilManager_->GetRadioCapability(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilRadioCapabilityTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetPhysicalChannelConfigTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetPhysicalChannelConfigTest -->");
    int32_t ret = mRilManager_->GetPhysicalChannelConfig(result);
    TELEPHONY_LOGI("RilUnitTest::GetPhysicalChannelConfigTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SendRilCmSmsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsTest -->");
    int32_t ret = mRilManager_->SendSms("smscPDU", "pdu", result->GetOwner(), result);
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SendRilCmSmsMoreModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsMoreModeTest -->");
    int32_t ret = mRilManager_->SendSmsMoreMode("smscPDU", "pdu", result->GetOwner(), result);
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsMoreModeTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetRilCmRadioPowerTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetRilCmRadioPowerTest -->");
    int32_t ret = mRilManager_->SetRadioState(1, 0, result);
    TELEPHONY_LOGI("RilUnitTest::SetRilCmRadioPowerTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::ShutDownTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::ShutDownTest -->");
    int32_t ret = mRilManager_->ShutDown(result);
    TELEPHONY_LOGI("RilUnitTest::ShutDownTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetRilRadioPowerTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilRadioPowerTest -->");
    int32_t ret = mRilManager_->GetRadioState(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilRadioPowerTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetImeiTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetImeiTest -->");
    int32_t ret = mRilManager_->GetImei(result);
    TELEPHONY_LOGI("RilUnitTest::GetImeiTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetMeidTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetMeidTest -->");
    int32_t ret = mRilManager_->GetMeid(result);
    TELEPHONY_LOGI("RilUnitTest::GetMeidTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetVoiceRadioTechnologyTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetVoiceRadioTechnologyTest -->");
    int32_t ret = mRilManager_->GetVoiceRadioTechnology(result);
    TELEPHONY_LOGI("RilUnitTest::GetVoiceRadioTechnologyTest --> finished, return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}


void RilUnitTest::SendDataSleepModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendDataSleepModeTest -->");
    int32_t ret = mRilManager_->SendDataSleepMode(result);
    TELEPHONY_LOGI("RilUnitTest::SendDataSleepModeTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SendSmsAckTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendSmsAckTest -->");
    int32_t cause = 2;
    int32_t ret = mRilManager_->SendSmsAck(true, cause, result);
    TELEPHONY_LOGI("RilUnitTest::SendSmsAckTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SimOpenLogicalChannelTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SimOpenLogicalChannelTest -->");
    int32_t ret = mRilManager_->SimOpenLogicalChannel("appID", 0, result);
    TELEPHONY_LOGI("RilUnitTest::SimOpenLogicalChannelTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SimTransmitApduLogicalChannelTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SimTransmitApduLogicalChannelTest -->");
    const int32_t REQ_INFO_P2 = 2;
    const int32_t REQ_INFO_P3 = 3;
    ApduSimIORequestInfo reqInfo = ApduSimIORequestInfo();
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0;
    reqInfo.instruction = 1;
    reqInfo.p1 = 1;
    reqInfo.p2 = REQ_INFO_P2;
    reqInfo.p3 = REQ_INFO_P3;
    reqInfo.data = "apdu";
    int32_t ret = mRilManager_->SimTransmitApduLogicalChannel(reqInfo, result);
    TELEPHONY_LOGI("RilUnitTest::SimTransmitApduLogicalChannelTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SimTransmitApduBasicChannelTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SimTransmitApduBasicChannelTest -->");
    const int32_t REQ_INFO_P2 = 2;
    const int32_t REQ_INFO_P3 = 3;
    ApduSimIORequestInfo reqInfo = ApduSimIORequestInfo();
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0;
    reqInfo.instruction = 1;
    reqInfo.p1 = 1;
    reqInfo.p2 = REQ_INFO_P2;
    reqInfo.p3 = REQ_INFO_P3;
    reqInfo.data = "apdu";
    int32_t ret = mRilManager_->SimTransmitApduBasicChannel(reqInfo, result);
    TELEPHONY_LOGI("RilUnitTest::SimTransmitApduBasicChannelTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SimCloseLogicalChannelTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SimCloseLogicalChannelTest -->");
    int32_t ret = mRilManager_->SimCloseLogicalChannel(0, result);
    TELEPHONY_LOGI("RilUnitTest::SimCloseLogicalChannelTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetActiveSimTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetActiveSimTest -->");
    int32_t index = 1;
    int32_t enable = 1;
    int32_t ret = mRilManager_->SetActiveSim(index, enable, result);
    TELEPHONY_LOGI("RilUnitTest::SetActiveSimTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::ActivatePdpContextTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::ActivatePdpContextTest -->");
    RilDataProfileTest dataProfile(0, "cmnet", "IPV4V6", 1, "", "", "IPV4V6");
    int32_t ret = mRilManager_->ActivatePdpContext(1, dataProfile, true, true, result);
    TELEPHONY_LOGI("RilUnitTest::ActivatePdpContextTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::DeactivatePdpContextTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::DeactivatePdpContextTest -->");
    int32_t ret = mRilManager_->DeactivatePdpContext(CI, REASON, result);
    TELEPHONY_LOGI("RilUnitTest::DeactivatePdpContextTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::DemoHandler::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    if (event == nullptr) {
        return;
    }
    event->GetInnerEventId();
}

void RilUnitTest::SetUssdTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetUssdTest -->");
    int32_t ret = mRilManager_->SetUssd("12345678", result);
    TELEPHONY_LOGI("RilUnitTest::SetUssdTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetUssdTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetUssdTest -->");
    int32_t ret = mRilManager_->GetUssd(result);
    TELEPHONY_LOGI("RilUnitTest::GetUssdTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetClipTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetClipTest -->");
    int32_t ret = mRilManager_->SetClip(1, result);
    TELEPHONY_LOGI("RilUnitTest::SetClipTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetClipTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetClipTest -->");
    int32_t ret = mRilManager_->GetClip(result);
    TELEPHONY_LOGI("RilUnitTest::GetClipTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetCallWaitingTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetCallWaitingTest -->");
    int32_t ret = mRilManager_->SetCallWaiting(1, result);
    TELEPHONY_LOGI("RilUnitTest::SetCallWaitingTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetBarringPasswordTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetBarringPasswordTest -->");
    std::string fac = "AB";
    std::string oldPwd = "1234";
    std::string newPwd = "3456";
    int32_t ret = mRilManager_->SetBarringPassword(fac, oldPwd, newPwd, result);
    TELEPHONY_LOGI("RilUnitTest::SetBarringPasswordTest --> SetBarringPasswordTest finished return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetCallWaitingTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetCallWaitingTest -->");
    int32_t ret = mRilManager_->GetCallWaiting(result);
    TELEPHONY_LOGI("RilUnitTest::GetCallWaitingTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetCallRestrictionTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetCallRestrictionTest -->");
    std::string fac = "AO";
    std::string password = "123456";
    int32_t ret = mRilManager_->SetCallRestriction(fac, 1, password, result);
    TELEPHONY_LOGI("RilUnitTest::SetCallRestrictionTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetCallRestrictionTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetCallRestrictionTest -->");
    std::string fac = "AO";
    int32_t ret = mRilManager_->GetCallRestriction(fac, result);
    TELEPHONY_LOGI("RilUnitTest::GetCallRestrictionTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetCallTransferInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetCallTransferInfoTest -->");
    std::string number = "123456789";
    int32_t ret = mRilManager_->SetCallTransferInfo(1, 1, number, 1, result);
    TELEPHONY_LOGI("RilUnitTest::SetCallTransferInfoTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetCallTransferInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetCallTransferInfoTest -->");
    int32_t ret = mRilManager_->GetCallTransferInfo(1, result);
    TELEPHONY_LOGI("RilUnitTest::GetCallTransferInfoTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetClirTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetClirTest -->");
    int32_t ret = mRilManager_->SetClir(1, result);
    TELEPHONY_LOGI("RilUnitTest::SetClirTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetClirTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetClirTest -->");
    int32_t ret = mRilManager_->GetClir(result);
    TELEPHONY_LOGI("RilUnitTest::GetClirTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetLinkBandwidthInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetLinkBandwidthInfoTest -->");
    int32_t cid = 1;
    int32_t ret = mRilManager_->GetLinkBandwidthInfo(cid, result);
    TELEPHONY_LOGI("RilUnitTest::GetLinkBandwidthInfoTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetDataProfileInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetDataProfileInfoTest -->");
    int32_t ret = mRilManager_->SetDataProfileInfo(result);
    TELEPHONY_LOGI("RilUnitTest::SetDataProfileInfoTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::UnLockPINTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::UnLockPINTest -->");
    std::string pin = "1234";
    int32_t ret = mRilManager_->UnLockPin(pin, result);
    TELEPHONY_LOGI("RilUnitTest::UnLockPINTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::UnLockPIN2Test(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::UnLockPIN2Test -->");
    std::string pin2 = "1234";
    int32_t ret = mRilManager_->UnLockPin2(pin2, result);
    TELEPHONY_LOGI("RilUnitTest::UnLockPIN2Test return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::UnLockPUKTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::UnLockPUKTest -->");
    std::string pin = "1234";
    std::string puk = "1234";
    int32_t ret = mRilManager_->UnLockPuk(pin, puk, result);
    TELEPHONY_LOGI("RilUnitTest::UnLockPUKTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::UnLockPUK2Test(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::UnLockPUK2Test -->");
    std::string pin2 = "1234";
    std::string puk2 = "1234";
    int32_t ret = mRilManager_->UnLockPuk2(pin2, puk2, result);
    TELEPHONY_LOGI("RilUnitTest::UnLockPUK2Test return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::ChangeSimPasswordTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::ChangeSimPasswordTest -->");
    std::string fac = "SC";
    std::string oldPassword = "1234";
    std::string newPassword = "1234";
    int32_t passwordLength = 4;
    int32_t ret = mRilManager_->ChangeSimPassword(fac, oldPassword, newPassword, passwordLength, result);
    TELEPHONY_LOGI("RilUnitTest::ChangeSimPasswordTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SetSimLockTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetSimLockTest -->");
    std::string fac = "SC";
    int32_t mode = 1;
    std::string passwd = "1234";
    int32_t ret = mRilManager_->SetSimLock(fac, mode, passwd, result);
    TELEPHONY_LOGI("RilUnitTest::SetSimLockTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::GetSimLockStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetSimLockStatusTest -->");
    std::string fac = "SC";
    int32_t mode = 2;
    int32_t ret = mRilManager_->GetSimLockStatus(fac, mode, result);
    TELEPHONY_LOGI("RilUnitTest::GetSimLockStatusTest return: %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SendTerminalResponseCmdTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendTerminalResponseCmdTest -->");
    std::string cmd = "Terminal Response";
    int32_t ret = mRilManager_->SendTerminalResponseCmd(cmd, result);
    TELEPHONY_LOGI("RilUnitTest::SendTerminalResponseCmdTest --> finished, ret = %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SendEnvelopeCmdTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendEnvelopeCmdTest -->");
    std::string cmd = "Envelope";
    int32_t ret = mRilManager_->SendTerminalResponseCmd(cmd, result);
    TELEPHONY_LOGI("RilUnitTest::SendEnvelopeCmdTest --> finished, ret = %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SendCallSetupRequestResultTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendCallSetupRequestResultTest -->");
    bool accept = true;
    int32_t ret = mRilManager_->SendCallSetupRequestResult(accept, result);
    TELEPHONY_LOGI("RilUnitTest::SendCallSetupRequestResultTest --> finished, ret = %{public}d", ret);
    ASSERT_EQ(0, ret);
}

void RilUnitTest::SimStkIsReadyTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SimStkIsReadyTest -->");
    int32_t ret = mRilManager_->SimStkIsReady(result);
    TELEPHONY_LOGI("RilUnitTest::SimStkIsReadyTest --> finished, ret = %{public}d", ret);
    ASSERT_EQ(0, ret);
}

#ifndef TEL_TEST_UNSUPPORT
HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetCallListTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_GET_CALL_LIST, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetCallListTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_GET_CALL_LIST, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetEmergencyCallListTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_EMERGENCY_LIST);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SET_EMERGENCY_LIST, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetEmergencyCallListTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_EMERGENCY_LIST);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SET_EMERGENCY_LIST, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetBarringPasswordTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_BARRING_PASSWORD);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SET_BARRING_PASSWORD, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetBarringPasswordTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_BARRING_PASSWORD);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SET_BARRING_PASSWORD, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RilCmDialTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_DIAL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_DIAL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RilCmDialTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_DIAL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_DIAL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RejectCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_REJECT);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_REJECT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RejectCallTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_REJECT);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_REJECT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_HangupRilCmConnectionTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_HANGUP);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_HANGUP, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_HangupRilCmConnectionTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_HANGUP);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_HANGUP, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_AcceptRilCmCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_ANSWER);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_ANSWER, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_AcceptRilCmCallTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_ANSWER);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_ANSWER, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_HoldCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_HOLD_CALL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_HOLD_CALL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_HoldCallTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_HOLD_CALL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_HOLD_CALL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnHoldCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_UNHOLD_CALL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_UNHOLD_CALL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnHoldCallTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_UNHOLD_CALL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_UNHOLD_CALL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SwitchCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SWITCH_CALL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SWITCH_CALL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SwitchCallTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SWITCH_CALL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SWITCH_CALL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RilCmJoinCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_COMBINE_CONFERENCE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_COMBINE_CONFERENCE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RilCmJoinCallTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_COMBINE_CONFERENCE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_COMBINE_CONFERENCE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RilCmSplitCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SEPARATE_CONFERENCE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SEPARATE_CONFERENCE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RilCmSplitCallTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SEPARATE_CONFERENCE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SEPARATE_CONFERENCE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmSignalStrengthTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_SIGNAL_STRENGTH);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_GET_SIGNAL_STRENGTH, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmSignalStrengthTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_SIGNAL_STRENGTH);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_GET_SIGNAL_STRENGTH, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmOperatorTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_OPERATOR_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_GET_OPERATOR_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmOperatorTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_OPERATOR_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_GET_OPERATOR_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmCsRegStatusTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_CS_REG_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_GET_CS_REG_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmCsRegStatusTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_CS_REG_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_GET_CS_REG_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmPsRegStatusTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_PS_REG_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_GET_PS_REG_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmPsRegStatusTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_PS_REG_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_GET_PS_REG_STATUS, event);
}

/**
 * @tc.number Telephony_RilAdapter_SetRilNotificationFilterTest_0100 to do ...
 * @tc.name Set notification filter of the card 1
 * @tc.desc Function test
 * @tc.require: issueI5BFY5
 */
HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetRilNotificationFilterTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_SET_NOTIFICATION_FILTER);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_SET_NOTIFICATION_FILTER, event);
}

/**
 * @tc.number Telephony_RilAdapter_SetRilNotificationFilterTest_1100 to do ...
 * @tc.name Set notification filter of the card 2
 * @tc.desc Function test
 * @tc.require: issueI5BFY5
 */
HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetRilNotificationFilterTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_SET_NOTIFICATION_FILTER);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_SET_NOTIFICATION_FILTER, event);
}

/**
 * @tc.number Telephony_RilAdapter_SetRilDeviceStateTest_0100 to do ...
 * @tc.name Set device state of the card 1
 * @tc.desc Function test
 * @tc.require: issueI5BFY5
 */
HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetRilDeviceStateTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_SET_DEVICE_STATE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_SET_DEVICE_STATE, event);
}

/**
 * @tc.number Telephony_RilAdapter_SetRilDeviceStateTest_1100 to do ...
 * @tc.name Set device state of the card 2
 * @tc.desc Function test
 * @tc.require: issueI5BFY5
 */
HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetRilDeviceStateTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_SET_DEVICE_STATE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_SET_DEVICE_STATE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilNetworkSearchInfoTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilNetworkSearchInfoTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilNetworkSelectionModeTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_NETWORK_SELECTION_MODE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_GET_NETWORK_SELECTION_MODE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilNetworkSelectionModeTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_NETWORK_SELECTION_MODE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_GET_NETWORK_SELECTION_MODE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetRilNetworkSelectionModeTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_SET_NETWORK_SELECTION_MODE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_SET_NETWORK_SELECTION_MODE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetRilNetworkSelectionModeTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_SET_NETWORK_SELECTION_MODE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_SET_NETWORK_SELECTION_MODE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilRadioCapabilityTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_RADIO_CAPABILITY);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_GET_RADIO_CAPABILITY, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilRadioCapabilityTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_RADIO_CAPABILITY);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_GET_RADIO_CAPABILITY, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetPhysicalChannelConfigTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetPhysicalChannelConfigTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetSimIOTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_SIM_IO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_GET_SIM_IO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetSimIOTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_SIM_IO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_GET_SIM_IO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetImsiTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_IMSI);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_GET_IMSI, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetImsiTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_IMSI);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_GET_IMSI, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetSimStatusTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_SIM_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_GET_SIM_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetSimStatusTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_SIM_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_GET_SIM_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendSmsAckTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SMS_SEND_SMS_ACK);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SMS_SEND_SMS_ACK, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendSmsAckTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SMS_SEND_SMS_ACK);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SMS_SEND_SMS_ACK, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimOpenLogicalChannelTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_OPEN_LOGICAL_CHANNEL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_OPEN_LOGICAL_CHANNEL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimOpenLogicalChannelTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_OPEN_LOGICAL_CHANNEL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_OPEN_LOGICAL_CHANNEL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimTransmitApduLogicalChannelTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimTransmitApduLogicalChannelTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimTransmitApduBasicChannelTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimTransmitApduBasicChannelTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimCloseLogicalChannelTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_CLOSE_LOGICAL_CHANNEL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_CLOSE_LOGICAL_CHANNEL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimCloseLogicalChannelTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_CLOSE_LOGICAL_CHANNEL);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_CLOSE_LOGICAL_CHANNEL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetActiveSimTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_SET_ACTIVE_SIM);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_SET_ACTIVE_SIM, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetActiveSimTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_SET_ACTIVE_SIM);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_SET_ACTIVE_SIM, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_ActivatePdpContextTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_ACTIVATE_PDP_CONTEXT);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_DATA_ACTIVATE_PDP_CONTEXT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_ActivatePdpContextTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_ACTIVATE_PDP_CONTEXT);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_DATA_ACTIVATE_PDP_CONTEXT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_DeactivatePdpContextTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_DEACTIVATE_PDP_CONTEXT);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_DATA_DEACTIVATE_PDP_CONTEXT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_DeactivatePdpContextTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_DEACTIVATE_PDP_CONTEXT);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_DATA_DEACTIVATE_PDP_CONTEXT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendRilCmSmsTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SMS_SEND_GSM_SMS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SMS_SEND_GSM_SMS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendRilCmSmsTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SMS_SEND_GSM_SMS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SMS_SEND_GSM_SMS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendRilCmSmsMoreModeTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SMS_SEND_SMS_MORE_MODE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SMS_SEND_SMS_MORE_MODE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendRilCmSmsMoreModeTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SMS_SEND_SMS_MORE_MODE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SMS_SEND_SMS_MORE_MODE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetRilCmRadioPowerTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_SET_RADIO_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_MODEM_SET_RADIO_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetRilCmRadioPowerTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_SET_RADIO_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_MODEM_SET_RADIO_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_ShutDownTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_SHUT_DOWN);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_MODEM_SHUT_DOWN, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_ShutDownTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_SHUT_DOWN);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_MODEM_SHUT_DOWN, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilRadioPowerTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_GET_RADIO_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_MODEM_GET_RADIO_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilRadioPowerTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_GET_RADIO_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_MODEM_GET_RADIO_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetImeiTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_GET_IMEI);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_MODEM_GET_IMEI, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetImeiTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_GET_IMEI);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_MODEM_GET_IMEI, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetMeidTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_GET_MEID);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_MODEM_GET_MEID, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetMeidTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_GET_MEID);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_MODEM_GET_MEID, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetVoiceRadioTechnologyTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_GET_VOICE_RADIO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_MODEM_GET_VOICE_RADIO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetVoiceRadioTechnologyTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_GET_VOICE_RADIO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_MODEM_GET_VOICE_RADIO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetUssdTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_USSD);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SET_USSD, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetUssdTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_USSD);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SET_USSD, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetUssdTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_USSD);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_GET_USSD, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetUssdTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_USSD);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_GET_USSD, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetClipTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CLIP);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SET_CLIP, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetClipTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CLIP);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SET_CLIP, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetClipTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CLIP);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_GET_CLIP, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetClipTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CLIP);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_GET_CLIP, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetCallWaitingTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CALL_WAITING);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SET_CALL_WAITING, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetCallWaitingTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CALL_WAITING);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SET_CALL_WAITING, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetCallWaitingTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_WAITING);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_GET_CALL_WAITING, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetCallWaitingTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_WAITING);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_GET_CALL_WAITING, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetCallRestrictionTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CALL_RESTRICTION);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SET_CALL_RESTRICTION, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetCallRestrictionTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CALL_RESTRICTION);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SET_CALL_RESTRICTION, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetCallRestrictionTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_RESTRICTION);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_GET_CALL_RESTRICTION, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetCallRestrictionTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_RESTRICTION);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_GET_CALL_RESTRICTION, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetCallTransferInfoTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CALL_TRANSFER_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SET_CALL_TRANSFER_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetCallTransferInfoTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CALL_TRANSFER_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SET_CALL_TRANSFER_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetCallTransferInfoTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_TRANSFER_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_GET_CALL_TRANSFER_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetCallTransferInfoTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_TRANSFER_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_GET_CALL_TRANSFER_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetClirTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CLIR);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_SET_CLIR, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetClirTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_CLIR);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_SET_CLIR, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetClirTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CLIR);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_CALL_GET_CLIR, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetClirTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CLIR);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_CALL_GET_CLIR, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetLinkBandwidthInfoTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_GET_LINK_BANDWIDTH_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetLinkBandwidthInfoTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_GET_LINK_BANDWIDTH_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetDataPermittedTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_SET_DATA_PERMITTED);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_DATA_SET_DATA_PERMITTED, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetDataProfileInfoTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_SET_DATA_PROFILE_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_DATA_SET_DATA_PROFILE_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetDataProfileInfoTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_SET_DATA_PROFILE_INFO);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_DATA_SET_DATA_PROFILE_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetSimLockStatusTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_SIM_LOCK_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_GET_SIM_LOCK_STATUS, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetSimLockStatusTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_SIM_LOCK_STATUS);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_GET_SIM_LOCK_STATUS, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetSimLockTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_SET_SIM_LOCK);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_SET_SIM_LOCK, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetSimLockTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_SET_SIM_LOCK);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_SET_SIM_LOCK, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_ChangeSimPasswordTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_CHANGE_SIM_PASSWORD);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_CHANGE_SIM_PASSWORD, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_ChangeSimPasswordTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_CHANGE_SIM_PASSWORD);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_CHANGE_SIM_PASSWORD, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnLockPinTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_UNLOCK_PIN);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_UNLOCK_PIN, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnLockPinTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_UNLOCK_PIN);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_UNLOCK_PIN, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnLockPukTest_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_UNLOCK_PUK);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_UNLOCK_PUK, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnLockPukTest_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_UNLOCK_PUK);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_UNLOCK_PUK, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnLockPin2Test_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_UNLOCK_PIN2);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_UNLOCK_PIN2, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnLockPin2Test_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_UNLOCK_PIN2);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_UNLOCK_PIN2, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnLockPuk2Test_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_UNLOCK_PUK2);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_UNLOCK_PUK2, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnLockPuk2Test_1100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_UNLOCK_PUK2);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_UNLOCK_PUK2, event);
#endif
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendTerminalResponseCmdTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_STK_SEND_TERMINAL_RESPONSE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_STK_SEND_TERMINAL_RESPONSE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendTerminalResponseCmdTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_STK_SEND_TERMINAL_RESPONSE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_STK_SEND_TERMINAL_RESPONSE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendEnvelopeCmdTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendEnvelopeCmdTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendCallSetupRequestResultTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_STK_SEND_ENVELOPE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_STK_SEND_ENVELOPE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendCallSetupRequestResultTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_STK_SEND_ENVELOPE);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_STK_SEND_ENVELOPE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimStkIsReadyTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_STK_IS_READY);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_0;
    OnProcessTest(HREQ_SIM_STK_IS_READY, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SimStkIsReadyTest_1100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_STK_IS_READY);
    event->SetOwner(GetHandler());
    RilManagerTest::slotId = HRIL_SIM_SLOT_1;
    OnProcessTest(HREQ_SIM_STK_IS_READY, event);
}

#else // TEL_TEST_UNSUPPORT

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetLinkBandwidthInfoTest_0100, Function | MediumTest | Level3)
{
    EXPECT_TRUE(true);
}
#endif // TEL_TEST_UNSUPPORT
} // namespace Telephony
} // namespace OHOS

