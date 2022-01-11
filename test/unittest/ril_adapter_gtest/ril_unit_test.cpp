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

#include "telephony_log_wrapper.h"

#include "hril_request.h"

using namespace testing::ext;
namespace OHOS {
namespace Telephony {
const int32_t CI = 1;
const int32_t REASON = 2;
const int32_t REQUESTINFO_FILEID = 2;
const int32_t REQUESTINFO_P1 = 4;
const int32_t REQUESTINFO_P2 = 5;
const int32_t REQUESTINFO_P3 = 6;

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
    memberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &RilUnitTest::GetCallListTest;
    memberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &RilUnitTest::GetRilCmSignalStrengthTest;
    memberFuncMap_[HREQ_CALL_DIAL] = &RilUnitTest::RilCmDialTest;
    memberFuncMap_[HREQ_CALL_REJECT] = &RilUnitTest::RejectCallTest;
    memberFuncMap_[HREQ_CALL_HANGUP] = &RilUnitTest::HangupRilCmConnectionTest;
    memberFuncMap_[HREQ_CALL_ANSWER] = &RilUnitTest::AcceptRilCmCallTest;
    memberFuncMap_[HREQ_CALL_HOLD_CALL] = &RilUnitTest::HoldCallTest;
    memberFuncMap_[HREQ_CALL_UNHOLD_CALL] = &RilUnitTest::UnHoldCallTest;
    memberFuncMap_[HREQ_CALL_SWITCH_CALL] = &RilUnitTest::SwitchCallTest;
    memberFuncMap_[HREQ_CALL_COMBINE_CONFERENCE] = &RilUnitTest::RilCmJoinCallTest;
    memberFuncMap_[HREQ_CALL_SEPARATE_CONFERENCE] = &RilUnitTest::RilCmSplitCallTest;
    memberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &RilUnitTest::GetRilCmOperatorTest;
    memberFuncMap_[HREQ_SMS_SEND_GSM_SMS] = &RilUnitTest::SendRilCmSmsTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &RilUnitTest::SendRilCmSmsMoreModeTest;
    memberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &RilUnitTest::SetRilCmRadioPowerTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_IO] = &RilUnitTest::GetSimIOTest;
    memberFuncMap_[HREQ_SIM_GET_IMSI] = &RilUnitTest::GetImsiTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &RilUnitTest::GetSimStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &RilUnitTest::GetRilCmCsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &RilUnitTest::GetRilCmPsRegStatusTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_ACK] = &RilUnitTest::SendSmsAckTest;
    memberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &RilUnitTest::ActivatePdpContextTest;
    memberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &RilUnitTest::DeactivatePdpContextTest;
    memberFuncMap_[HREQ_CALL_SET_USSD_CUSD] = &RilUnitTest::SetUssdCusdTest;
    memberFuncMap_[HREQ_CALL_GET_USSD_CUSD] = &RilUnitTest::GetUssdCusdTest;
    memberFuncMap_[HREQ_DATA_GET_LINK_BANDWIDTH_INFO] = &RilUnitTest::GetLinkBandwidthInfoTest;
}

void RilUnitTest::GetCallListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetCallListTest -->");
    mRilManager_->GetCallList(result);
    TELEPHONY_LOGI("RilUnitTest::GetCallListTest --> GetCallListTest finished");
}

void RilUnitTest::GetSimIOTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetSimIOTest -->");
    SimIoRequestInfo requestInfo;
    requestInfo.command = 1;
    requestInfo.fileId = REQUESTINFO_FILEID;
    requestInfo.path = "ReadIccFile";
    requestInfo.p1 = REQUESTINFO_P1;
    requestInfo.p2 = REQUESTINFO_P2;
    requestInfo.p3 = REQUESTINFO_P3;
    requestInfo.data = "7";

    mRilManager_->GetSimIO(requestInfo, result);
    TELEPHONY_LOGI("RilUnitTest::GetSimIOTest --> GetSimIOTest finished");
}

void RilUnitTest::GetImsiTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetImsiTest -->");
    mRilManager_->GetImsi("GetImsi", result);
    TELEPHONY_LOGI("RilUnitTest::GetImsiTest --> GetImsiTest finished");
}

void RilUnitTest::GetRilCmSignalStrengthTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmSignalStrengthTest -->");
    mRilManager_->GetRilCmSignalStrength(result->GetOwner(), static_cast<int>(result->GetInnerEventId()));
    TELEPHONY_LOGI("RilUnitTest::GetRilCmSignalStrengthTest --> GetRilCmSignalStrengthTest finished");
}

void RilUnitTest::GetSimStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetSimStatusTest -->");
    mRilManager_->GetSimStatus(result);
    TELEPHONY_LOGI("RilUnitTest::GetSimStatusTest --> GetSimStatusTest finished");
}

void RilUnitTest::RilCmDialTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    std::string phoneNum;
    int32_t clirMode; /* Calling Line Identification Restriction . From TS 27.007 V3.4.0 (2000-03) */

    TELEPHONY_LOGI("RilUnitTest::RilCmDialTest -->");

    std::cout << "please enter the phone number:";
    std::cin >> phoneNum;
    clirMode = 0; // use subscription default value

    mRilManager_->RilCmDial(phoneNum, clirMode, result);
    TELEPHONY_LOGI("RilUnitTest::RilCmDialTest --> RilCmDialTest finished");
}

void RilUnitTest::RejectCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::RejectCallTest -->");
    mRilManager_->Reject(result);
    TELEPHONY_LOGI("RilUnitTest::RejectCallTest --> RejectCallTest finished");
}

void RilUnitTest::HoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::HoldCallTest -->");
    mRilManager_->HoldCall(result);
    TELEPHONY_LOGI("RilUnitTest::HoldCallTest --> HoldCallTest finished");
}

void RilUnitTest::UnHoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::UnHoldCallTest -->");
    mRilManager_->UnHoldCall(result);
    TELEPHONY_LOGI("RilUnitTest::UnHoldCallTest --> UnHoldCallTest finished");
}

void RilUnitTest::SwitchCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SwitchCallTest -->");
    mRilManager_->SwitchCall(result);
    TELEPHONY_LOGI("RilUnitTest::SwitchCallTest --> SwitchCallTest finished");
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

    mRilManager_->RilCmJoin(callType, result);
    TELEPHONY_LOGI("RilUnitTest::RilCmJoinCallTest --> RilCmJoinCallTest finished");
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

    mRilManager_->RilCmSplit(callIndex, callType, result);
    TELEPHONY_LOGI("RilUnitTest::RilCmSplitCallTest --> RilCmSplitCallTest finished");
}

void RilUnitTest::HangupRilCmConnectionTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::HangupRilCmConnectionTest -->");
    mRilManager_->Hangup(static_cast<int>(result->GetInnerEventId()), result);
    TELEPHONY_LOGI("RilUnitTest::HangupRilCmConnectionTest --> HangupRilCmConnectionTest finished");
}

void RilUnitTest::AcceptRilCmCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::AcceptRilCmCallTest -->");
    mRilManager_->Answer(result);
    TELEPHONY_LOGI("RilUnitTest::AcceptRilCmCallTest --> AcceptRilCmCallTest finished");
}

void RilUnitTest::GetRilCmCsRegStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmCsRegStatusTest -->");
    mRilManager_->GetCsRegStatus(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCmCsRegStatusTest --> GetRilCmCsRegStatusTest finished");
}

void RilUnitTest::GetRilCmPsRegStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmPsRegStatusTest -->");
    mRilManager_->GetPsRegStatus(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCmPsRegStatusTest --> GetRilCmPsRegStatusTest finished");
}

void RilUnitTest::GetRilCmOperatorTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmOperatorTest -->");
    mRilManager_->GetOperatorInfo(result->GetOwner(), static_cast<int>(result->GetInnerEventId()));
    TELEPHONY_LOGI("RilUnitTest::GetRilCmOperatorTest --> GetRilCmOperatorTest finished");
}

void RilUnitTest::SendRilCmSmsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsTest -->");
    mRilManager_->SendSms("smscPDU", "pdu", result->GetOwner(), result);
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsTest --> SendRilCmSmsTest finished");
}

void RilUnitTest::SendRilCmSmsMoreModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsMoreModeTest -->");
    mRilManager_->SendSmsMoreMode("smscPDU", "pdu", result->GetOwner(), result);
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsMoreModeTest --> SendRilCmSmsMoreModeTest finished");
}

void RilUnitTest::SetRilCmRadioPowerTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetRilCmRadioPowerTest -->");
    mRilManager_->SetRadioState(1, 0, result);
    TELEPHONY_LOGI("RilUnitTest::SetRilCmRadioPowerTest --> SetRilCmRadioPowerTest finished");
}

void RilUnitTest::SendSmsAckTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendSmsAckTest -->");
    int32_t cause = 2;
    mRilManager_->SendSmsAck(true, cause, result);
    TELEPHONY_LOGI(
        "RilUnitTest::SendSmsAckTest --> SendSmsAckTest "
        "finished");
}

void RilUnitTest::ActivatePdpContextTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::ActivatePdpContextTest -->");
    RilDataProfileTest dataProfile(0, "cmnet", "IPV4V6", 1, "", "", "IPV4V6");
    mRilManager_->ActivatePdpContext(1, dataProfile, true, true, result);
    TELEPHONY_LOGI("RilUnitTest::ActivatePdpContextTest --> ActivatePdpContextTest finished");
}

void RilUnitTest::DeactivatePdpContextTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::DeactivatePdpContextTest -->");
    mRilManager_->DeactivatePdpContext(CI, REASON, result);
    TELEPHONY_LOGI("RilUnitTest::DeactivatePdpContextTest --> DeactivatePdpContextTest finished");
}

void RilUnitTest::DemoHandler::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    event->GetInnerEventId();
}

void RilUnitTest::SetUssdCusdTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetUssdCusdTest -->");
    mRilManager_->SetUssdCusd("12345678", result);
    TELEPHONY_LOGI("RilUnitTest::SetUssdCusdTest --> SetUssdCusdTest finished");
}

void RilUnitTest::GetUssdCusdTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetUssdCusdTest -->");
    mRilManager_->GetUssdCusd(result);
    TELEPHONY_LOGI("RilUnitTest::GetUssdCusdTest --> GetUssdCusdTest finished");
}

void RilUnitTest::GetLinkBandwidthInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetLinkBandwidthInfoTest -->");
    int32_t cid = 1;
    mRilManager_->GetLinkBandwidthInfo(cid, result);
    TELEPHONY_LOGI("RilUnitTest::GetLinkBandwidthInfoTest --> GetLinkBandwidthInfoTest finished");
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetCallListTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_CALL_LIST);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_GET_CALL_LIST, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RilCmDialTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_DIAL);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_DIAL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RejectCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_REJECT);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_REJECT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_HangupRilCmConnectionTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_HANGUP);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_HANGUP, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_AcceptRilCmCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_ANSWER);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_ANSWER, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_HoldCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_HOLD_CALL);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_HOLD_CALL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_UnHoldCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_UNHOLD_CALL);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_UNHOLD_CALL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SwitchCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SWITCH_CALL);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_SWITCH_CALL, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RilCmJoinCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_COMBINE_CONFERENCE);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_COMBINE_CONFERENCE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_RilCmSplitCallTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SEPARATE_CONFERENCE);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_SEPARATE_CONFERENCE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmSignalStrengthTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_SIGNAL_STRENGTH);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_NETWORK_GET_SIGNAL_STRENGTH, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmOperatorTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_OPERATOR_INFO);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_NETWORK_GET_OPERATOR_INFO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmCsRegStatusTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_CS_REG_STATUS);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_NETWORK_GET_CS_REG_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetRilCmPsRegStatusTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_NETWORK_GET_PS_REG_STATUS);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_NETWORK_GET_PS_REG_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetSimIOTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_SIM_IO);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_SIM_GET_SIM_IO, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetImsiTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_IMSI);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_SIM_GET_IMSI, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetSimStatusTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SIM_GET_SIM_STATUS);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_SIM_GET_SIM_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendSmsAckTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SMS_SEND_SMS_ACK);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_SMS_SEND_SMS_ACK, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_ActivatePdpContextTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_ACTIVATE_PDP_CONTEXT);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_DATA_ACTIVATE_PDP_CONTEXT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_DeactivatePdpContextTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_DEACTIVATE_PDP_CONTEXT);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_DATA_DEACTIVATE_PDP_CONTEXT, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendRilCmSmsTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SMS_SEND_GSM_SMS);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_SMS_SEND_GSM_SMS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SendRilCmSmsMoreModeTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_SMS_SEND_SMS_MORE_MODE);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_SMS_SEND_SMS_MORE_MODE, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetRilCmRadioPowerTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_MODEM_SET_RADIO_STATUS);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_MODEM_SET_RADIO_STATUS, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_SetUssdCusdTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_SET_USSD_CUSD);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_SET_USSD_CUSD, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetUssdCusdTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_CALL_GET_USSD_CUSD);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_CALL_GET_USSD_CUSD, event);
}

HWTEST_F(RilUnitTest, Telephony_RilAdapter_GetLinkBandwidthInfoTest_0100, Function | MediumTest | Level3)
{
    OnInit();
    auto event = OHOS::AppExecFwk::InnerEvent::Get(HREQ_DATA_GET_LINK_BANDWIDTH_INFO);
    event->SetOwner(GetHandler());
    OnProcessTest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, event);
}
} // namespace Telephony
} // namespace OHOS