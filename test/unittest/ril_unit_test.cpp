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

using namespace std;
namespace OHOS {
namespace Telephony {
namespace {
RilUnitTest::RilUnitTest(int32_t opt) : slotId_(opt) {}

RilUnitTest::~RilUnitTest() {}

const int32_t CI = 1;
const int32_t COMMAND = 1;
const int32_t REASON = 2;
const int32_t FILEID = 2;
const int32_t P1 = 4;
const int32_t P2 = 5;
const int32_t P3 = 6;
const int32_t REQINFO_P2 = 2;
const int32_t REQINFO_P3 = 3;

void RilUnitTest::OnInit()
{
    TELEPHONY_LOGI("RilUnitTest OnInit");
    int32_t cdmaSubscription = 1;
    mRilManager_ = std::make_unique<RilManagerTest>(0, cdmaSubscription);
    if (mRilManager_ == nullptr) {
        return;
    }
    mRilManager_->OnInit();
}

void RilUnitTest::GetRilCmCurrentCallsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmCurrentCallsTest -->");
    mRilManager_->GetCallList(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCmCurrentCallsTest --> GetRilCmCurrentCallsTest finished");
}

void RilUnitTest::IccRilCmIoForAppTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::IccRilCmIoForAppTest -->");
    mRilManager_->GetSimIO(COMMAND, FILEID, P1, P2, P3, "7", "ReadIccFile", result);
    TELEPHONY_LOGI("RilUnitTest::IccRilCmIoForAppTest --> IccRilCmIoForAppTest finished");
}

void RilUnitTest::GetRilCmImsiForAppTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmImsiForAppTest -->");
    mRilManager_->GetImsi("GetImsi", result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCmImsiForAppTest --> GetRilCmImsiForAppTest finished");
}

void RilUnitTest::GetRilCmSignalIntensityTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmSignalIntensityTest -->");
    mRilManager_->GetRilCmSignalStrength(result->GetOwner(), static_cast<int>(result->GetInnerEventId()));
    TELEPHONY_LOGI("RilUnitTest::GetRilCmSignalIntensityTest --> GetRilCmSignalIntensityTest finished");
}

void RilUnitTest::GetRilCmIccCardStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmIccCardStatusTest -->");
    mRilManager_->GetSimStatus(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCmIccCardStatusTest --> GetRilCmIccCardStatusTest finished");
}

void RilUnitTest::RilCmDialByUusInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::RilCmDialByUusInfoTest -->");
    /* commented out in i_ril_manager.h, can't call it. */
    TELEPHONY_LOGI("RilUnitTest::RilCmDialByUusInfoTest --> RilCmDialByUusInfoTest finished");
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

void RilUnitTest::RilCmRejectCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::RilCmRejectCallTest -->");
    mRilManager_->Reject(result);
    TELEPHONY_LOGI("RilUnitTest::RilCmRejectCallTest --> RilCmRejectCallTest finished");
}

void RilUnitTest::RilCmHoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::RilCmHoldCallTest -->");
    mRilManager_->HoldCall(result);
    TELEPHONY_LOGI("RilUnitTest::RilCmHoldCallTest --> RilCmHoldCallTest finished");
}

void RilUnitTest::RilCmActiveCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::RilCmActiveCallTest -->");
    mRilManager_->UnHoldCall(result);
    TELEPHONY_LOGI("RilUnitTest::RilCmActiveCallTest --> RilCmActiveCallTest finished");
}

void RilUnitTest::RilCmSwapCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)

{
    TELEPHONY_LOGI("RilUnitTest::RilCmSwapCallTest -->");
    mRilManager_->UnHoldCall(result);
    TELEPHONY_LOGI("RilUnitTest::RilCmSwapCallTest --> RilCmSwapCallTest finished");
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

    std::cout << "please enter the call type:";
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

    TELEPHONY_LOGI("RilUnitTest::RilCmJoinCallTest -->");

    std::cout << "please enter the call split number:";
    std::cin >> callIndex;

    std::cout << "please enter the call type:";
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

void RilUnitTest::GetRilCmCellInfoListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmCellInfoListTest -->");
    mRilManager_->GetCellInfoList(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCmCellInfoListTest --> GetRilCmCellInfoListTest finished");
}

void RilUnitTest::GetRilCurrentCellInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCurrentCellInfoTest -->");
    mRilManager_->GetCurrentCellInfo(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCurrentCellInfoTest --> GetRilCurrentCellInfoTest finished");
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

void RilUnitTest::AcknowledgeRilCmLastIncomingGsmSmsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::AcknowledgeRilCmLastIncomingGsmSmsTest -->");
    int32_t cause = 2;
    mRilManager_->SendSmsAck(true, cause, result);
    TELEPHONY_LOGI(
        "RilUnitTest::AcknowledgeRilCmLastIncomingGsmSmsTest --> AcknowledgeRilCmLastIncomingGsmSmsTest "
        "finished");
}

void RilUnitTest::SetupRilCmDataCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetupRilCmDataCallTest -->");
    RilDataProfileTest dataProfile(0, "cmnet", "IPV4V6", 1, "", "", "IPV4V6");
    mRilManager_->ActivatePdpContext(1, dataProfile, true, true, result);
    TELEPHONY_LOGI("RilUnitTest::SetupRilCmDataCallTest --> SetupRilCmDataCallTest finished");
}

void RilUnitTest::DeactivateRilCmDataCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::DeactivateRilCmDataCallTest -->");
    mRilManager_->DeactivatePdpContext(CI, REASON, result);
    TELEPHONY_LOGI("RilUnitTest::DeactivateRilCmDataCallTest --> DeactivateRilCmDataCallTest finished");
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

void RilUnitTest::OpenLogicalSimIO(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::OpenLogicalSimIOResponse -->");
    mRilManager_->OpenLogicalSimIO("appID", 0, result);
    TELEPHONY_LOGI("RilUnitTest::OpenLogicalSimIOResponse --> OpenLogicalSimIOResponse finished");
}

void RilUnitTest::CloseLogicalSimIO(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::CloseLogicalSimIOResponse -->");
    mRilManager_->CloseLogicalSimIO(0, result);
    TELEPHONY_LOGI("RilUnitTest::CloseLogicalSimIOResponse --> CloseLogicalSimIOResponse finished");
}

void RilUnitTest::TransmitApduSimIO(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::TransmitApduSimIOResponse -->");
    ApduSimIORequestInfo reqInfo = ApduSimIORequestInfo();
    reqInfo.serial = 0;
    reqInfo.chanId = 1;
    reqInfo.type = 0;
    reqInfo.instruction = 1;
    reqInfo.p1 = 1;
    reqInfo.p2 = REQINFO_P2;
    reqInfo.p3 = REQINFO_P3;
    reqInfo.data = "apdu";
    mRilManager_->TransmitApduSimIO(reqInfo, result);
    TELEPHONY_LOGI("RilUnitTest::TransmitApduSimIOResponse --> TransmitApduSimIOResponse finished");
}

void RilUnitTest::UnlockSimLock(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::UnlocksimLockResponse -->");
    mRilManager_->UnlockSimLock(0, "1234", result);
    TELEPHONY_LOGI("RilUnitTest::UnlocksimLockResponse --> UnlocksimLockResponse finished");
}

void RilUnitTest::GetLinkBandwidthInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetLinkBandwidthInfoTest -->");
    int32_t cid = 1;
    mRilManager_->GetLinkBandwidthInfo(cid, result);
    TELEPHONY_LOGI("RilUnitTest::GetLinkBandwidthInfoTest --> GetLinkBandwidthInfoTest finished");
}

void RilUnitTest::OnInitInterface()
{
    memberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &RilUnitTest::GetRilCmCurrentCallsTest;
    memberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &RilUnitTest::GetRilCmSignalIntensityTest;
    memberFuncMap_[HREQ_CALL_DIAL] = &RilUnitTest::RilCmDialTest;
    memberFuncMap_[HREQ_CALL_REJECT] = &RilUnitTest::RilCmRejectCallTest;
    memberFuncMap_[HREQ_CALL_HANGUP] = &RilUnitTest::HangupRilCmConnectionTest;
    memberFuncMap_[HREQ_CALL_ANSWER] = &RilUnitTest::AcceptRilCmCallTest;
    memberFuncMap_[HREQ_CALL_HOLD_CALL] = &RilUnitTest::RilCmHoldCallTest;
    memberFuncMap_[HREQ_CALL_UNHOLD_CALL] = &RilUnitTest::RilCmActiveCallTest;
    memberFuncMap_[HREQ_CALL_SWITCH_CALL] = &RilUnitTest::RilCmSwapCallTest;
    memberFuncMap_[HREQ_CALL_COMBINE_CONFERENCE] = &RilUnitTest::RilCmJoinCallTest;
    memberFuncMap_[HREQ_CALL_SEPARATE_CONFERENCE] = &RilUnitTest::RilCmSplitCallTest;
    memberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &RilUnitTest::GetRilCmOperatorTest;
    memberFuncMap_[HREQ_SMS_SEND_GSM_SMS] = &RilUnitTest::SendRilCmSmsTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &RilUnitTest::SendRilCmSmsMoreModeTest;
    memberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &RilUnitTest::SetRilCmRadioPowerTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_IO] = &RilUnitTest::IccRilCmIoForAppTest;
    memberFuncMap_[HREQ_SIM_GET_IMSI] = &RilUnitTest::GetRilCmImsiForAppTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &RilUnitTest::GetRilCmIccCardStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &RilUnitTest::GetRilCmCsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &RilUnitTest::GetRilCmPsRegStatusTest;
    memberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &RilUnitTest::SetupRilCmDataCallTest;
    memberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &RilUnitTest::DeactivateRilCmDataCallTest;
    memberFuncMap_[HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST] = &RilUnitTest::GetRilCmCellInfoListTest;
    memberFuncMap_[HREQ_NETWORK_GET_CURRENT_CELL_INFO] = &RilUnitTest::GetRilCurrentCellInfoTest;
    memberFuncMap_[HREQ_CALL_SET_USSD_CUSD] = &RilUnitTest::SetUssdCusdTest;
    memberFuncMap_[HREQ_CALL_GET_USSD_CUSD] = &RilUnitTest::GetUssdCusdTest;
    memberFuncMap_[HREQ_SIM_OPEN_LOGICAL_SIM_IO] = &RilUnitTest::OpenLogicalSimIO;
    memberFuncMap_[HREQ_SIM_CLOSE_LOGICAL_SIM_IO] = &RilUnitTest::CloseLogicalSimIO;
    memberFuncMap_[HREQ_SIM_TRANSMIT_APDU_SIM_IO] = &RilUnitTest::TransmitApduSimIO;
    memberFuncMap_[HREQ_SIM_UNLOCK_SIM_LOCK] = &RilUnitTest::UnlockSimLock;
    memberFuncMap_[HREQ_DATA_GET_LINK_BANDWIDTH_INFO] = &RilUnitTest::GetLinkBandwidthInfoTest;
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
    cout << HREQ_CALL_HOLD_CALL << "---->RilUnitTest::RilCmHoldCallTest " << endl;
    cout << HREQ_CALL_UNHOLD_CALL << "---->RilUnitTest::RilCmActiveCallTest " << endl;
    cout << HREQ_CALL_SWITCH_CALL << "---->RilUnitTest::RilCmSwapCallTest " << endl;
    cout << HREQ_CALL_COMBINE_CONFERENCE << "---->RilUnitTest::RilCmJoinCallTest " << endl;
    cout << HREQ_CALL_SEPARATE_CONFERENCE << "---->RilUnitTest::RilCmSplitCallTest " << endl;
    cout << HREQ_CALL_SET_USSD_CUSD << "---->RilUnitTest::SetUssdCusdTest " << endl;
    cout << HREQ_CALL_GET_USSD_CUSD << "---->RilUnitTest::GetUssdCusdTest " << endl;
    cout << "---->NETWORK----------------------------------------------------------" << endl;
    cout << HREQ_NETWORK_GET_SIGNAL_STRENGTH << "---->RilUnitTest::GetRilCmSignalIntensityTest " << endl;
    cout << HREQ_NETWORK_GET_OPERATOR_INFO << "---->RilUnitTest::GetRilCmOperatorTest " << endl;
    cout << HREQ_NETWORK_GET_CS_REG_STATUS << "---->RilUnitTest::GetRilCmCsRegStatusTest " << endl;
    cout << HREQ_NETWORK_GET_PS_REG_STATUS << "---->RilUnitTest::GetRilCmPsRegStatusTest " << endl;
    cout << HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST << "---->RilUnitTest::GetRilCmCellInfoListTest " << endl;
    cout << "---->SIM----------------------------------------------------------" << endl;
    cout << HREQ_SIM_GET_SIM_IO << "---->RilUnitTest::IccRilCmIoForAppTest " << endl;
    cout << HREQ_SIM_GET_IMSI << "---->RilUnitTest::GetRilCmImsiForAppTest " << endl;
    cout << HREQ_SIM_GET_SIM_STATUS << "---->RilUnitTest::GetRilCmIccCardStatusTest " << endl;
    cout << HREQ_SIM_OPEN_LOGICAL_SIM_IO << "---->RilUnitTest::OpenLogicalSimIOResponse " << endl;
    cout << HREQ_SIM_CLOSE_LOGICAL_SIM_IO << "---->RilUnitTest::CloseLogicalSimIOResponse " << endl;
    cout << HREQ_SIM_TRANSMIT_APDU_SIM_IO << "---->RilUnitTest::TransmitApduSimIOResponse " << endl;
    cout << HREQ_SIM_UNLOCK_SIM_LOCK << "---->RilUnitTest::UnlockSimLockResponse " << endl;
    cout << "---->DATA----------------------------------------------------------" << endl;
    cout << HREQ_DATA_ACTIVATE_PDP_CONTEXT << "---->RilUnitTest::SetupRilCmDataCallTest " << endl;
    cout << HREQ_DATA_DEACTIVATE_PDP_CONTEXT << "---->RilUnitTest::DeactivateRilCmDataCallTest " << endl;
    cout << HREQ_DATA_GET_LINK_BANDWIDTH_INFO << "---->RilUnitTest::GetLinkBandwidthInfoTest "
         << endl;
    cout << "---->SMS----------------------------------------------------------" << endl;
    cout << HREQ_SMS_SEND_GSM_SMS << "---->RilUnitTest::SendRilCmSmsTest " << endl;
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
