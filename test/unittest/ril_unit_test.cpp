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

using namespace std;
namespace OHOS {
namespace Telephony {
namespace {
RilUnitTest::RilUnitTest(int32_t opt) : phoneID_(opt) {}

RilUnitTest::~RilUnitTest() {}

const int32_t CI = 1;
const int32_t COMMAND = 1;
const int32_t REASON = 2;
const int32_t FILEID = 2;
const int32_t P1 = 4;
const int32_t P2 = 5;
const int32_t P3 = 6;

void RilUnitTest::OnInit()
{
    TELEPHONY_LOGD("RilUnitTest OnInit");
    int32_t cdmaSubscription = 1;
    mRilManager_ = std::make_unique<RilManagerTest>(0, cdmaSubscription);
    if (mRilManager_ == nullptr) {
        return;
    }
    mRilManager_->OnInit();
}

void RilUnitTest::GetRilCmCurrentCallsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::GetRilCmCurrentCallsTest -->");
    mRilManager_->GetCallList(result);
    TELEPHONY_LOGD("RilUnitTest::GetRilCmCurrentCallsTest --> GetRilCmCurrentCallsTest finished");
}

void RilUnitTest::IccRilCmIoForAppTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::IccRilCmIoForAppTest -->");
    mRilManager_->RequestSimIO(COMMAND, FILEID, P1, P2, P3, "7", "ReadIccFile", result);
    TELEPHONY_LOGD("RilUnitTest::IccRilCmIoForAppTest --> IccRilCmIoForAppTest finished");
}

void RilUnitTest::GetRilCmImsiForAppTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::GetRilCmImsiForAppTest -->");
    mRilManager_->GetImsi("GetImsi", result);
    TELEPHONY_LOGD("RilUnitTest::GetRilCmImsiForAppTest --> GetRilCmImsiForAppTest finished");
}

void RilUnitTest::GetRilCmSignalIntensityTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::GetRilCmSignalIntensityTest -->");
    mRilManager_->GetRilCmSignalStrength(result->GetOwner(), static_cast<int>(result->GetInnerEventId()));
    TELEPHONY_LOGD("RilUnitTest::GetRilCmSignalIntensityTest --> GetRilCmSignalIntensityTest finished");
}

void RilUnitTest::GetRilCmIccCardStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::GetRilCmIccCardStatusTest -->");
    mRilManager_->GetSimStatus(result);
    TELEPHONY_LOGD("RilUnitTest::GetRilCmIccCardStatusTest --> GetRilCmIccCardStatusTest finished");
}

void RilUnitTest::RilCmDialByUusInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::RilCmDialByUusInfoTest -->");
    /* commented out in i_ril_manager.h, can't call it. */
    TELEPHONY_LOGD("RilUnitTest::RilCmDialByUusInfoTest --> RilCmDialByUusInfoTest finished");
}

void RilUnitTest::RilCmDialTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    std::string phoneNum;
    int32_t clirMode; /* Calling Line Identification Restriction . From TS 27.007 V3.4.0 (2000-03) */

    TELEPHONY_LOGD("RilUnitTest::RilCmDialTest -->");

    std::cout << "please enter the phone number:";
    std::cin >> phoneNum;
    clirMode = 0; // use subscription default value

    mRilManager_->RilCmDial(phoneNum, clirMode, result);
    TELEPHONY_LOGD("RilUnitTest::RilCmDialTest --> RilCmDialTest finished");
}

void RilUnitTest::RilCmRejectCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::RilCmRejectCallTest -->");
    mRilManager_->Reject(result);
    TELEPHONY_LOGD("RilUnitTest::RilCmRejectCallTest --> RilCmRejectCallTest finished");
}

void RilUnitTest::RilCmHoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::RilCmHoldCallTest -->");
    mRilManager_->Hold(result);
    TELEPHONY_LOGD("RilUnitTest::RilCmHoldCallTest --> RilCmHoldCallTest finished");
}

void RilUnitTest::RilCmActiveCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::RilCmActiveCallTest -->");
    mRilManager_->Active(result);
    TELEPHONY_LOGD("RilUnitTest::RilCmActiveCallTest --> RilCmActiveCallTest finished");
}

void RilUnitTest::RilCmSwapCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)

{
    TELEPHONY_LOGD("RilUnitTest::RilCmSwapCallTest -->");
    mRilManager_->Active(result);
    TELEPHONY_LOGD("RilUnitTest::RilCmSwapCallTest --> RilCmSwapCallTest finished");
}

void RilUnitTest::RilCmJoinCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t callType; /* call type
                       * 0: Voice call
                       * 1: Video call: send one-way video, two-way voice
                       * 2: Video call: one-way receiving video, two-way voice
                       * 3: Video call: two-way video, two-way voice
                       */

    TELEPHONY_LOGD("RilUnitTest::RilCmJoinCallTest -->");

    std::cout << "please enter the call type:";
    std::cin >> callType;

    mRilManager_->RilCmJoin(callType, result);
    TELEPHONY_LOGD("RilUnitTest::RilCmJoinCallTest --> RilCmJoinCallTest finished");
}

void RilUnitTest::RilCmSplitCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t nThCall;
    int32_t callType; /* call type
                       * 0: Voice call
                       * 1: Video call: send one-way video, two-way voice
                       * 2: Video call: one-way receiving video, two-way voice
                       * 3: Video call: two-way video, two-way voice
                       */

    TELEPHONY_LOGD("RilUnitTest::RilCmJoinCallTest -->");

    std::cout << "please enter the call split number:";
    std::cin >> nThCall;

    std::cout << "please enter the call type:";
    std::cin >> callType;

    mRilManager_->RilCmSplit(nThCall, callType, result);
    TELEPHONY_LOGD("RilUnitTest::RilCmSplitCallTest --> RilCmSplitCallTest finished");
}

void RilUnitTest::HangupRilCmConnectionTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::HangupRilCmConnectionTest -->");
    mRilManager_->Hangup(static_cast<int>(result->GetInnerEventId()), result);
    TELEPHONY_LOGD("RilUnitTest::HangupRilCmConnectionTest --> HangupRilCmConnectionTest finished");
}

void RilUnitTest::AcceptRilCmCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::AcceptRilCmCallTest -->");
    mRilManager_->Answer(result);
    TELEPHONY_LOGD("RilUnitTest::AcceptRilCmCallTest --> AcceptRilCmCallTest finished");
}

void RilUnitTest::GetRilCmCsRegStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::GetRilCmCsRegStatusTest -->");
    mRilManager_->GetCsRegStatus(result);
    TELEPHONY_LOGD("RilUnitTest::GetRilCmCsRegStatusTest --> GetRilCmCsRegStatusTest finished");
}

void RilUnitTest::GetRilCmPsRegStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::GetRilCmPsRegStatusTest -->");
    mRilManager_->GetPsRegStatus(result);
    TELEPHONY_LOGD("RilUnitTest::GetRilCmPsRegStatusTest --> GetRilCmPsRegStatusTest finished");
}

void RilUnitTest::SetRilLocationUpdateForNetworksTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::SetRilLocationUpdateForNetworksTest -->");
    mRilManager_->SetNetworkLocationUpdate(result);
    TELEPHONY_LOGD(
        "RilUnitTest::SetRilLocationUpdateForNetworksTest --> SetRilLocationUpdateForNetworksTest finished");
}

void RilUnitTest::GetRilCmOperatorTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::GetRilCmOperatorTest -->");
    mRilManager_->GetOperatorInfo(result->GetOwner(), static_cast<int>(result->GetInnerEventId()));
    TELEPHONY_LOGD("RilUnitTest::GetRilCmOperatorTest --> GetRilCmOperatorTest finished");
}

void RilUnitTest::SendRilCmSmsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::SendRilCmSmsTest -->");
    mRilManager_->SendSms("smscPDU", "pdu", result->GetOwner(), result);
    TELEPHONY_LOGD("RilUnitTest::SendRilCmSmsTest --> SendRilCmSmsTest finished");
}

void RilUnitTest::SendRilCmSmsMoreModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::SendRilCmSmsMoreModeTest -->");
    mRilManager_->SendSmsMoreMode("smscPDU", "pdu", result->GetOwner(), result);
    TELEPHONY_LOGD("RilUnitTest::SendRilCmSmsMoreModeTest --> SendRilCmSmsMoreModeTest finished");
}

void RilUnitTest::SetRilCmRadioPowerTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::SetRilCmRadioPowerTest -->");
    mRilManager_->SetRadioStatus(1, 0, result);
    TELEPHONY_LOGD("RilUnitTest::SetRilCmRadioPowerTest --> SetRilCmRadioPowerTest finished");
}

void RilUnitTest::AcknowledgeRilCmLastIncomingGsmSmsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::AcknowledgeRilCmLastIncomingGsmSmsTest -->");
    int32_t cause = 2;
    mRilManager_->SendSmsAck(true, cause, result);
    TELEPHONY_LOGD(
        "RilUnitTest::AcknowledgeRilCmLastIncomingGsmSmsTest --> AcknowledgeRilCmLastIncomingGsmSmsTest "
        "finished");
}

void RilUnitTest::SetupRilCmDataCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::SetupRilCmDataCallTest -->");
    RilDataProfileTest dataProfile(0, "cmnet", "IPV4V6", 1, "", "", "IPV4V6");
    mRilManager_->ActivatePdpContext(1, dataProfile, true, true, result);
    TELEPHONY_LOGD("RilUnitTest::SetupRilCmDataCallTest --> SetupRilCmDataCallTest finished");
}

void RilUnitTest::DeactivateRilCmDataCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGD("RilUnitTest::DeactivateRilCmDataCallTest -->");
    mRilManager_->DeactivatePdpContext(CI, REASON, result);
    TELEPHONY_LOGD("RilUnitTest::DeactivateRilCmDataCallTest --> DeactivateRilCmDataCallTest finished");
}

void RilUnitTest::DemoHandler::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    event->GetInnerEventId();
}

void RilUnitTest::OnProcessInput(int32_t what, const OHOS::AppExecFwk::InnerEvent::Pointer &event)
{
    auto itFunc = memberFuncMap_.find(what);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(event);
        }
    }
}

void RilUnitTest::OnInitInterface()
{
    memberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &RilUnitTest::GetRilCmCurrentCallsTest;
    memberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &RilUnitTest::GetRilCmSignalIntensityTest;
    memberFuncMap_[HREQ_CALL_DIAL] = &RilUnitTest::RilCmDialTest;
    memberFuncMap_[HREQ_CALL_REJECT] = &RilUnitTest::RilCmRejectCallTest;
    memberFuncMap_[HREQ_CALL_HANGUP] = &RilUnitTest::HangupRilCmConnectionTest;
    memberFuncMap_[HREQ_CALL_ANSWER] = &RilUnitTest::AcceptRilCmCallTest;
    memberFuncMap_[HREQ_CALL_HOLD] = &RilUnitTest::RilCmHoldCallTest;
    memberFuncMap_[HREQ_CALL_ACTIVE] = &RilUnitTest::RilCmActiveCallTest;
    memberFuncMap_[HREQ_CALL_SWAP] = &RilUnitTest::RilCmSwapCallTest;
    memberFuncMap_[HREQ_CALL_JOIN] = &RilUnitTest::RilCmJoinCallTest;
    memberFuncMap_[HREQ_CALL_SPLIT] = &RilUnitTest::RilCmSplitCallTest;
    memberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &RilUnitTest::GetRilCmOperatorTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS] = &RilUnitTest::SendRilCmSmsTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &RilUnitTest::SendRilCmSmsMoreModeTest;
    memberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &RilUnitTest::SetRilCmRadioPowerTest;
    memberFuncMap_[HREQ_SIM_IO] = &RilUnitTest::IccRilCmIoForAppTest;
    memberFuncMap_[HREQ_SIM_GET_IMSI] = &RilUnitTest::GetRilCmImsiForAppTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &RilUnitTest::GetRilCmIccCardStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &RilUnitTest::GetRilCmCsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &RilUnitTest::GetRilCmPsRegStatusTest;
    memberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &RilUnitTest::SetupRilCmDataCallTest;
    memberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &RilUnitTest::DeactivateRilCmDataCallTest;
}
} // namespace
} // namespace Telephony
} // namespace OHOS

using namespace OHOS;
using namespace OHOS::Telephony;
static void PrintMenu()
{
    cout << "---->CALL------------------------------------------------------------" << endl;
    cout << HREQ_CALL_GET_CALL_LIST << "---->RilUnitTest::GetRilCmCurrentCallsTest " << endl;
    cout << HREQ_CALL_DIAL << "---->RilUnitTest::RilCmDialTest " << endl;
    cout << HREQ_CALL_REJECT << "---->RilUnitTest::RilCmRejectCallTest " << endl;
    cout << HREQ_CALL_HANGUP << "---->RilUnitTest::HangupRilCmConnectionTest " << endl;
    cout << HREQ_CALL_ANSWER << "---->RilUnitTest::AcceptRilCmCallTest " << endl;
    cout << HREQ_CALL_HOLD << "---->RilUnitTest::RilCmHoldCallTest " << endl;
    cout << HREQ_CALL_ACTIVE << "---->RilUnitTest::RilCmActiveCallTest " << endl;
    cout << HREQ_CALL_SWAP << "---->RilUnitTest::RilCmSwapCallTest " << endl;
    cout << HREQ_CALL_JOIN << "---->RilUnitTest::RilCmJoinCallTest " << endl;
    cout << HREQ_CALL_SPLIT << "---->RilUnitTest::RilCmSplitCallTest " << endl;
    cout << "---->NETWORK----------------------------------------------------------" << endl;
    cout << HREQ_NETWORK_GET_SIGNAL_STRENGTH << "---->RilUnitTest::GetRilCmSignalIntensityTest " << endl;
    cout << HREQ_NETWORK_GET_OPERATOR_INFO << "---->RilUnitTest::GetRilCmOperatorTest " << endl;
    cout << HREQ_NETWORK_GET_CS_REG_STATUS << "---->RilUnitTest::GetRilCmCsRegStatusTest " << endl;
    cout << HREQ_NETWORK_GET_PS_REG_STATUS << "---->RilUnitTest::GetRilCmPsRegStatusTest " << endl;
    cout << "---->SIM----------------------------------------------------------" << endl;
    cout << HREQ_SIM_IO << "---->RilUnitTest::IccRilCmIoForAppTest " << endl;
    cout << HREQ_SIM_GET_IMSI << "---->RilUnitTest::GetRilCmImsiForAppTest " << endl;
    cout << HREQ_SIM_GET_SIM_STATUS << "---->RilUnitTest::GetRilCmIccCardStatusTest " << endl;
    cout << "---->DATA----------------------------------------------------------" << endl;
    cout << HREQ_DATA_ACTIVATE_PDP_CONTEXT << "---->RilUnitTest::SetupRilCmDataCallTest " << endl;
    cout << HREQ_DATA_DEACTIVATE_PDP_CONTEXT << "---->RilUnitTest::DeactivateRilCmDataCallTest " << endl;
    cout << "---->SMS----------------------------------------------------------" << endl;
    cout << HREQ_SMS_SEND_SMS << "---->RilUnitTest::SendRilCmSmsTest " << endl;
    cout << HREQ_SMS_SEND_SMS_MORE_MODE << "---->RilUnitTest::SendRilCmSmsMoreModeTest " << endl;
    cout << HREQ_SMS_IMS_SEND_SMS << "---->RilUnitTest::SendRilCmImsGsmSmsTest " << endl;
    cout << HREQ_MODEM_SET_RADIO_STATUS << "---->RilUnitTest::SetRilCmRadioPowerTest " << endl;
    cout << HREQ_MODEM_EXIT << "---->exit " << endl;
}

namespace {
enum class UnitTestResult { RIL_TEST_SUCCESS = 0, RIL_TEST_FAIL = 1 };
}

int32_t main()
{
    static std::unique_ptr<OHOS::Telephony::RilUnitTest> rilUnitTest;
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilUnitTest::DemoHandler> handler;
    cout << "---->Test Enter";
    rilUnitTest = std::make_unique<OHOS::Telephony::RilUnitTest>(0);
    if (rilUnitTest == nullptr) {
        return static_cast<int32_t>(UnitTestResult::RIL_TEST_FAIL);
    }
    rilUnitTest->OnInit();
    runner = OHOS::AppExecFwk::EventRunner::Create("DemoHandler");
    handler = make_shared<RilUnitTest::DemoHandler>(runner);
    if (handler == nullptr) {
        return static_cast<int32_t>(UnitTestResult::RIL_TEST_FAIL);
    }
    int32_t mWhat = 1;

    bool loopFlag = true;

    rilUnitTest->OnInitInterface();
    while (loopFlag) {
        PrintMenu();
        std::cin >> mWhat;

        if (mWhat == 1000) {
            break;
        }

        if (mWhat < 0 || mWhat > HREQ_MODEM_SET_RADIO_STATUS) {
            cout << "<<<<----------------------------------->>>" << endl;
            cout << "---->Input value error, please retry!!" << endl;
            cout << "<<<<----------------------------------->>>" << endl;
            continue;
        }

        auto event = OHOS::AppExecFwk::InnerEvent::Get(mWhat);
        event->SetOwner(handler);
        rilUnitTest->OnProcessInput(mWhat, event);
    }
    cout << "---->Test end" << endl;
    return static_cast<int32_t>(UnitTestResult::RIL_TEST_SUCCESS);
}
