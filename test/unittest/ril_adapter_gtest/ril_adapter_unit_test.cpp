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

#include "ril_adapter_unit_test.h"

#include <iostream>
#include "telephony_log_wrapper.h"

using namespace std;
using namespace testing::ext;
namespace OHOS {
namespace Telephony {
const int32_t CI = 1;
const int32_t COMMAND = 1;
const int32_t REASON = 2;
const int32_t FILEID = 2;
const int32_t P1 = 4;
const int32_t P2 = 5;
const int32_t P3 = 6;

void RilAdapterUnitTest::SetUpTestCase()
{
    std::cout << "----------RilAdapterUnitTest  start ------------" << std::endl;
}

void RilAdapterUnitTest::TearDownTestCase()
{
    std::cout << "----------RilAdapterUnitTest  end ------------" << std::endl;
}

void RilAdapterUnitTest::SetUp() {}

void RilAdapterUnitTest::TearDown() {}

void RilAdapterUnitTest::OnInit()
{
    TELEPHONY_LOGD("RilAdapterUnitTest OnInit");
    int32_t cdmaSubscription = 1;
    mRilManager_ = std::make_unique<RilManagerTest>(0, cdmaSubscription);
    if (mRilManager_ == nullptr) {
        return;
    }
    mRilManager_->OnInit();
}

void RilAdapterUnitTest::GetRilCmCurrentCallsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmCurrentCallsTest -->");
    mRilManager_->GetCallList(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmCurrentCallsTest --> GetRilCmCurrentCallsTest finished");
}

void RilAdapterUnitTest::IccRilCmIoForAppTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::IccRilCmIoForAppTest -->");
    mRilManager_->RequestSimIO(COMMAND, FILEID, P1, P2, P3, "7", "ReadIccFile", result);
    TELEPHONY_LOGD("RilAdapterUnitTest::IccRilCmIoForAppTest --> IccRilCmIoForAppTest finished");
}

void RilAdapterUnitTest::GetRilCmImsiForAppTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmImsiForAppTest -->");
    mRilManager_->GetImsi("GetImsi", result);
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmImsiForAppTest --> GetRilCmImsiForAppTest finished");
}

void RilAdapterUnitTest::GetRilCmSignalIntensityTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmSignalIntensityTest -->");
    mRilManager_->GetRilCmSignalStrength(result->GetOwner(), static_cast<int>(result->GetInnerEventId()));
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmSignalIntensityTest --> GetRilCmSignalIntensityTest finished");
}

void RilAdapterUnitTest::GetRilCmIccCardStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmIccCardStatusTest -->");
    mRilManager_->GetSimStatus(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmIccCardStatusTest --> GetRilCmIccCardStatusTest finished");
}

void RilAdapterUnitTest::RilCmDialByUusInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmDialByUusInfoTest -->");
    /* commented out in i_ril_manager.h, can't call it. */
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmDialByUusInfoTest --> RilCmDialByUusInfoTest finished");
}

void RilAdapterUnitTest::RilCmDialTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    std::string phoneNum;
    int32_t clirMode; /* Calling Line Identification Restriction . From TS 27.007 V3.4.0 (2000-03) */

    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmDialTest -->");

    std::cout << "please enter the phone number:";
    std::cin >> phoneNum;
    clirMode = 0; // use subscription default value

    mRilManager_->RilCmDial(phoneNum, clirMode, result);
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmDialTest --> RilCmDialTest finished");
}

void RilAdapterUnitTest::RilCmRejectCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmRejectCallTest -->");
    mRilManager_->Reject(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmRejectCallTest --> RilCmRejectCallTest finished");
}

void RilAdapterUnitTest::RilCmHoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmHoldCallTest -->");
    mRilManager_->Hold(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmHoldCallTest --> RilCmHoldCallTest finished");
}

void RilAdapterUnitTest::RilCmActiveCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmActiveCallTest -->");
    mRilManager_->Active(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmActiveCallTest --> RilCmActiveCallTest finished");
}

void RilAdapterUnitTest::RilCmSwapCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)

{
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmSwapCallTest -->");
    mRilManager_->Active(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmSwapCallTest --> RilCmSwapCallTest finished");
}

void RilAdapterUnitTest::RilCmJoinCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t callType; /* call type
                       * 0: Voice call
                       * 1: Video call: send one-way video, two-way voice
                       * 2: Video call: one-way receiving video, two-way voice
                       * 3: Video call: two-way video, two-way voice
                       */

    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmJoinCallTest -->");

    std::cout << "please enter the call type:";
    std::cin >> callType;

    mRilManager_->RilCmJoin(callType, result);
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmJoinCallTest --> RilCmJoinCallTest finished");
}

void RilAdapterUnitTest::RilCmSplitCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t nThCall;
    int32_t callType; /* call type
                       * 0: Voice call
                       * 1: Video call: send one-way video, two-way voice
                       * 2: Video call: one-way receiving video, two-way voice
                       * 3: Video call: two-way video, two-way voice
                       */

    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmSplitCallTest -->");

    std::cout << "please enter the call split number:";
    std::cin >> nThCall;

    std::cout << "please enter the call type:";
    std::cin >> callType;

    mRilManager_->RilCmSplit(nThCall, callType, result);
    TELEPHONY_LOGD("RilAdapterUnitTest::RilCmSplitCallTest --> RilCmSplitCallTest finished");
}

void RilAdapterUnitTest::HangupRilCmConnectionTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::HangupRilCmConnectionTest -->");
    mRilManager_->Hangup(static_cast<int>(result->GetInnerEventId()), result);
    TELEPHONY_LOGD("RilAdapterUnitTest::HangupRilCmConnectionTest --> HangupRilCmConnectionTest finished");
}

void RilAdapterUnitTest::AcceptRilCmCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::AcceptRilCmCallTest -->");
    mRilManager_->Answer(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::AcceptRilCmCallTest --> AcceptRilCmCallTest finished");
}

void RilAdapterUnitTest::GetRilCmCsRegStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmCsRegStatusTest -->");
    mRilManager_->GetCsRegStatus(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmCsRegStatusTest --> GetRilCmCsRegStatusTest finished");
}

void RilAdapterUnitTest::GetRilCmPsRegStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmPsRegStatusTest -->");
    mRilManager_->GetPsRegStatus(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmPsRegStatusTest --> GetRilCmPsRegStatusTest finished");
}

void RilAdapterUnitTest::SetRilLocationUpdateForNetworksTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::SetRilLocationUpdateForNetworksTest -->");
    mRilManager_->SetNetworkLocationUpdate(result);
    TELEPHONY_LOGD("RilAdapterUnitTest::SetRilLocationUpdateForNetworksTest --> finished");
}

void RilAdapterUnitTest::GetRilCmOperatorTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmOperatorTest -->");
    mRilManager_->GetOperatorInfo(result->GetOwner(), static_cast<int>(result->GetInnerEventId()));
    TELEPHONY_LOGD("RilAdapterUnitTest::GetRilCmOperatorTest --> GetRilCmOperatorTest finished");
}

void RilAdapterUnitTest::SendRilCmSmsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::SendRilCmSmsTest -->");
    mRilManager_->SendSms("smscPDU", "pdu", result->GetOwner(), result);
    TELEPHONY_LOGD("RilAdapterUnitTest::SendRilCmSmsTest --> SendRilCmSmsTest finished");
}

void RilAdapterUnitTest::SendRilCmSmsMoreModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::SendRilCmSmsMoreModeTest -->");
    mRilManager_->SendSmsMoreMode("smscPDU", "pdu", result->GetOwner(), result);
    TELEPHONY_LOGD("RilAdapterUnitTest::SendRilCmSmsMoreModeTest --> SendRilCmSmsMoreModeTest finished");
}

void RilAdapterUnitTest::SetRilCmRadioPowerTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::SetRilCmRadioPowerTest -->");
    mRilManager_->SetRadioStatus(1, 0, result);
    TELEPHONY_LOGD("RilAdapterUnitTest::SetRilCmRadioPowerTest --> SetRilCmRadioPowerTest finished");
}

void RilAdapterUnitTest::AcknowledgeRilCmLastIncomingGsmSmsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::AcknowledgeRilCmLastIncomingGsmSmsTest -->");
    int32_t cause = 2;
    mRilManager_->SendSmsAck(true, cause, result);
    TELEPHONY_LOGD(
        "RilAdapterUnitTest::AcknowledgeRilCmLastIncomingGsmSmsTest --> AcknowledgeRilCmLastIncomingGsmSmsTest "
        "finished");
}

void RilAdapterUnitTest::SetupRilCmDataCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::SetupRilCmDataCallTest -->");
    RilDataProfileTest dataProfile(0, "cmnet", "IPV4V6", 1, "", "", "IPV4V6");
    mRilManager_->ActivatePdpContext(1, dataProfile, true, true, result);
    TELEPHONY_LOGD("RilAdapterUnitTest::SetupRilCmDataCallTest --> SetupRilCmDataCallTest finished");
}

void RilAdapterUnitTest::DeactivateRilCmDataCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilAdapterUnitTest::DeactivateRilCmDataCallTest -->");
    mRilManager_->DeactivatePdpContext(CI, REASON, result);
    TELEPHONY_LOGD("RilAdapterUnitTest::DeactivateRilCmDataCallTest --> DeactivateRilCmDataCallTest finished");
}

void RilAdapterUnitTest::DemoHandler::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    event->GetInnerEventId();
}

void RilAdapterUnitTest::OnProcessInput(int32_t what, const OHOS::AppExecFwk::InnerEvent::Pointer &event)
{
    auto itFunc = memberFuncMap_.find(what);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(event);
        }
    }
}

void RilAdapterUnitTest::OnInitInterface()
{
    memberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &RilAdapterUnitTest::GetRilCmCurrentCallsTest;
    memberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &RilAdapterUnitTest::GetRilCmSignalIntensityTest;
    memberFuncMap_[HREQ_CALL_DIAL] = &RilAdapterUnitTest::RilCmDialTest;
    memberFuncMap_[HREQ_CALL_REJECT] = &RilAdapterUnitTest::RilCmRejectCallTest;
    memberFuncMap_[HREQ_CALL_HANGUP] = &RilAdapterUnitTest::HangupRilCmConnectionTest;
    memberFuncMap_[HREQ_CALL_ANSWER] = &RilAdapterUnitTest::AcceptRilCmCallTest;
    memberFuncMap_[HREQ_CALL_HOLD] = &RilAdapterUnitTest::RilCmHoldCallTest;
    memberFuncMap_[HREQ_CALL_ACTIVE] = &RilAdapterUnitTest::RilCmActiveCallTest;
    memberFuncMap_[HREQ_CALL_SWAP] = &RilAdapterUnitTest::RilCmSwapCallTest;
    memberFuncMap_[HREQ_CALL_JOIN] = &RilAdapterUnitTest::RilCmJoinCallTest;
    memberFuncMap_[HREQ_CALL_SPLIT] = &RilAdapterUnitTest::RilCmSplitCallTest;
    memberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &RilAdapterUnitTest::GetRilCmOperatorTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS] = &RilAdapterUnitTest::SendRilCmSmsTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &RilAdapterUnitTest::SendRilCmSmsMoreModeTest;
    memberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &RilAdapterUnitTest::SetRilCmRadioPowerTest;
    memberFuncMap_[HREQ_SIM_IO] = &RilAdapterUnitTest::IccRilCmIoForAppTest;
    memberFuncMap_[HREQ_SIM_GET_IMSI] = &RilAdapterUnitTest::GetRilCmImsiForAppTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &RilAdapterUnitTest::GetRilCmIccCardStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &RilAdapterUnitTest::GetRilCmCsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &RilAdapterUnitTest::GetRilCmPsRegStatusTest;
    memberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &RilAdapterUnitTest::SetupRilCmDataCallTest;
    memberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &RilAdapterUnitTest::DeactivateRilCmDataCallTest;
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_GetRilCmCurrentCallsTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    GetRilCmCurrentCallsTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_RilCmDialTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    RilCmDialTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_RilCmRejectCallTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    RilCmRejectCallTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_HangupRilCmConnectionTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    HangupRilCmConnectionTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_AcceptRilCmCallTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    AcceptRilCmCallTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_RilCmHoldCallTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    RilCmHoldCallTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_RilCmActiveCallTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    RilCmActiveCallTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_RilCmSwapCallTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    RilCmSwapCallTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_RilCmJoinCallTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    RilCmJoinCallTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_RilCmSplitCallTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    RilCmSplitCallTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_GetRilCmSignalIntensityTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    GetRilCmSignalIntensityTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_GetRilCmOperatorTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    GetRilCmOperatorTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_GetRilCmCsRegStatusTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    GetRilCmCsRegStatusTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_GetRilCmPsRegStatusTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    GetRilCmPsRegStatusTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_IccRilCmIoForAppTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    IccRilCmIoForAppTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_GetRilCmImsiForAppTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    GetRilCmImsiForAppTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_GetRilCmIccCardStatusTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    GetRilCmIccCardStatusTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_SetupRilCmDataCallTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    SetupRilCmDataCallTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_DeactivateRilCmDataCallTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    DeactivateRilCmDataCallTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_SendRilCmSmsTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    SendRilCmSmsTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_SendRilCmSmsMoreModeTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    SendRilCmSmsMoreModeTest(event);
}

HWTEST_F(RilAdapterUnitTest, Telephony_RilAdapter_SetRilCmRadioPowerTest_0100, Function | MediumTest | Level3)
{
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilAdapterUnitTest::DemoHandler> handler;
    OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilAdapterUnitTest::DemoHandler>(runner);

    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(handler);
    SetRilCmRadioPowerTest(event);
}
} // namespace Telephony
} // namespace OHOS

using namespace OHOS;
using namespace OHOS::Telephony;
namespace {
enum class UnitTestResult { RIL_TEST_SUCCESS = 0, RIL_TEST_FAIL = 1 };
}