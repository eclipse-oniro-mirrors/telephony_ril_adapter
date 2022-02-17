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
#include <unistd.h>

#include "telephony_log_wrapper.h"
#include "hril_request.h"

using namespace std;

static int32_t InputInt32(int32_t start, int32_t end, string title)
{
    int32_t choice;
    bool firstFlg = true;
    const int32_t inBufLen = 1024;
    do {
        if (!firstFlg) {
            cin.clear();
            cin.ignore(inBufLen, '\n');
            cout << "---->Invalid Input, Please Enter Again !" << endl;
        }
        firstFlg = false;
        cout << "---->Please Enter " << title << " (" << start << "-" << end << "): ";
        cin >> choice;
    } while (cin.fail() || choice < start || choice > end);

    return choice;
}

namespace OHOS {
namespace Telephony {
namespace {
RilUnitTest::RilUnitTest(int32_t opt) : slotId_(opt) {}

RilUnitTest::~RilUnitTest() {}

const int32_t REASON = 2;
const int32_t REQ_INFO_P2 = 2;
const int32_t REQ_INFO_P3 = 3;

const int32_t STRESS_TEST_NUM = 10000;
const int32_t MAX_CALL_ID = 7;
const int32_t MAX_CALL_TYPE = 3;

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

void RilUnitTest::GetCallListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetCallListTest -->");
    mRilManager_->GetCallList(result);
    TELEPHONY_LOGI("RilUnitTest::GetCallListTest --> finished");
}

void RilUnitTest::RilCmDialTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    string phoneNum;
    int32_t clirMode = 0; /* Calling Line Identification Restriction . From TS 27.007 V3.4.0 (2000-03) */

    TELEPHONY_LOGI("RilUnitTest::RilCmDialTest -->");
    cout << "---->Please enter the phone number:";
    cin >> phoneNum;
    mRilManager_->RilCmDial(phoneNum, clirMode, result);

    TELEPHONY_LOGI("RilUnitTest::RilCmDialTest --> finished");
}

void RilUnitTest::RilCmDialStressTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    string phoneNum;
    int32_t clirMode = 0; /* Calling Line Identification Restriction . From TS 27.007 V3.4.0 (2000-03) */

    TELEPHONY_LOGI("RilUnitTest::RilCmDialStressTest -->");
    cout << "---->Please enter the phone number:";
    cin >> phoneNum;

    for (int32_t i = 0; i < STRESS_TEST_NUM; i++) {
        mRilManager_->RilCmDial(phoneNum, clirMode, result);
    }

    TELEPHONY_LOGI("RilUnitTest::RilCmDialStressTest --> finished");
}

void RilUnitTest::HangupTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::HangupTest -->");
    int32_t callIndex = InputInt32(1, MAX_CALL_ID, "Hangup call number");
    mRilManager_->Hangup(callIndex, result);
    TELEPHONY_LOGI("RilUnitTest::HangupTest --> finished");
}

void RilUnitTest::RejectTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::RejectTest -->");
    mRilManager_->Reject(result);
    TELEPHONY_LOGI("RilUnitTest::RejectTest --> finished");
}

void RilUnitTest::AnswerCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::AnswerCallTest -->");
    mRilManager_->Answer(result);
    TELEPHONY_LOGI("RilUnitTest::AnswerCallTest --> finished");
}

void RilUnitTest::HoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::HoldCallTest -->");
    mRilManager_->HoldCall(result);
    TELEPHONY_LOGI("RilUnitTest::HoldCallTest --> finished");
}

void RilUnitTest::UnHoldCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::UnHoldCallTest -->");
    mRilManager_->UnHoldCall(result);
    TELEPHONY_LOGI("RilUnitTest::UnHoldCallTest --> finished");
}

void RilUnitTest::SwitchCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SwitchCallTest -->");
    mRilManager_->SwitchCall(result);
    TELEPHONY_LOGI("RilUnitTest::SwitchCallTest --> finished");
}

void RilUnitTest::RilCmJoinCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::RilCmJoinCallTest -->");
    /* call type
     * 0: Voice call
     * 1: Video call: send one-way video, two-way voice
     * 2: Video call: one-way receiving video, two-way voice
     * 3: Video call: two-way video, two-way voice
     */
    int32_t callType = InputInt32(0, MAX_CALL_TYPE, "Call Type");
    mRilManager_->RilCmJoin(callType, result);
    TELEPHONY_LOGI("RilUnitTest::RilCmJoinCallTest --> finished");
}

void RilUnitTest::RilCmSplitCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::RilCmJoinCallTest -->");
    int32_t callIndex = InputInt32(1, MAX_CALL_ID, "The Split Call Number");
    /* call type
     * 0: Voice call
     * 1: Video call: send one-way video, two-way voice
     * 2: Video call: one-way receiving video, two-way voice
     * 3: Video call: two-way video, two-way voice
     */
    int32_t callType = InputInt32(0, MAX_CALL_TYPE, "Call Type");
    mRilManager_->RilCmSplit(callIndex, callType, result);
    TELEPHONY_LOGI("RilUnitTest::RilCmSplitCallTest --> finished");
}

void RilUnitTest::CallSupplementTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::CallSupplementTest -->");
    const int32_t HANGUP_ACTIVE = 2;
    cout << "---->CallSupplement Hangup hold wait: 1, Hangup active: 2: " << endl;
    int32_t hangupType = InputInt32(1, HANGUP_ACTIVE, "Option");
    mRilManager_->CallSupplement(hangupType, result);
    TELEPHONY_LOGI("RilUnitTest::CallSupplementTest --> finished");
}

void RilUnitTest::SendDtmfTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendDtmfTest -->");
    cout << "---->Please input dtmf key: ";
    char dtmfKey;
    cin >> dtmfKey;
    cout << endl << "====>Send Dtmf Key: " << dtmfKey << endl;
    mRilManager_->SendDtmf(dtmfKey, 1, result);
    TELEPHONY_LOGI("RilUnitTest::SendDtmfTest --> finished");
}

void RilUnitTest::StartDtmfTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::StartDtmfTest -->");
    cout << "---->Please input dtmf key: ";
    char dtmfKey;
    cin >> dtmfKey;
    cout << endl << "====>Start Dtmf Key: " << dtmfKey << endl;
    mRilManager_->StartDtmf(dtmfKey, 1, result);
    TELEPHONY_LOGI("RilUnitTest::StartDtmfTest --> finished");
}

void RilUnitTest::StopDtmfTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::StopDtmfTest -->");
    mRilManager_->StopDtmf(1, result);
    TELEPHONY_LOGI("RilUnitTest::StopDtmfTest --> finished");
}

void RilUnitTest::SetUssdCusdTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetUssdCusdTest -->");
    mRilManager_->SetUssd("12345678", result);
    TELEPHONY_LOGI("RilUnitTest::SetUssdCusdTest --> finished");
}

void RilUnitTest::GetUssdCusdTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetUssdCusdTest -->");
    mRilManager_->GetUssd(result);
    TELEPHONY_LOGI("RilUnitTest::GetUssdCusdTest --> finished");
}

void RilUnitTest::SetMuteTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetMuteTest -->");
    cout << "---->Unmute: 0, Mute: 1: " << endl;
    int32_t mute = InputInt32(0, 1, "Option");
    mRilManager_->SetMute(mute, result);
    TELEPHONY_LOGI("RilUnitTest::SetMuteTest --> finished");
}

void RilUnitTest::GetMuteTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetMuteTest -->");
    mRilManager_->GetMute(result);
    TELEPHONY_LOGI("RilUnitTest::GetMuteTest --> finished");
}

void RilUnitTest::GetEmergencyListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetEmergencyListTest -->");
    mRilManager_->GetEmergencyList(result);
    TELEPHONY_LOGI("RilUnitTest::GetEmergencyListTest --> finished");
}

void RilUnitTest::GetFailReasonTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetFailReasonTest -->");
    mRilManager_->GetFailReason(result);
    TELEPHONY_LOGI("RilUnitTest::GetFailReasonTest --> finished");
}

void RilUnitTest::IccRilSimIoForAppTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::IccRilSimIoForAppTest -->");

    cout << "176 : READ BINARY" << endl;
    cout << "178 : READ RECORD" << endl;
    cout << "192 : GET RESPONSE" << endl;
    cout << "214 : UPDATE BINARY" << endl;
    cout << "220 : UPDATE RECORD" << endl;
    cout << "242 : STATUS" << endl;
    cout << "203 : RETRIEVE DATA" << endl;
    cout << "219 : SET DATA" << endl;
    cout << "Please input command type:";
    int32_t curCommand;
    cin >> curCommand;
    cout << "Your input command type is : " << curCommand << endl;

    cout << "Please Input file id:";
    int32_t fileId;
    cin >> fileId;
    cout << "Your input file id is : " << fileId << endl;

    cout << "Please Input file path:";
    string filePath;
    cin >> filePath;
    cout << "Your input file path is : " << filePath << endl;

    cout << "Please Input p1:";
    int32_t p1;
    cin >> p1;
    cout << "Your input p1 is : " << p1 << endl;

    cout << "Please Input p2:";
    int32_t p2;
    cin >> p2;
    cout << "Your input p2 is : " << p2 << endl;

    cout << "Please Input p3:";
    int32_t p3;
    cin >> p3;
    cout << "Your input p3 is : " << p3 << endl;

    cout << "Please Input command data:";
    string cmdData;
    cin >> cmdData;
    cout << "Your input command data is : " << cmdData << endl;

    mRilManager_->GetSimIO(curCommand, fileId, p1, p2, p3, cmdData, filePath, result);
    TELEPHONY_LOGI("RilUnitTest::IccRilSimIoForAppTest --> IccRilSimIoForAppTest finished");
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
    mRilManager_->GetOperatorInfo(result->GetOwner(), static_cast<int32_t>(result->GetInnerEventId()));
    TELEPHONY_LOGI("RilUnitTest::GetRilCmOperatorTest --> GetRilCmOperatorTest finished");
}

void RilUnitTest::GetRilNetworkSearchInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilNetworkSearchInfoTest -->");
    mRilManager_->GetNetworkSearchInfo(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilNetworkSearchInfoTest --> GetRilNetworkSearchInfoTest finished");
}

void RilUnitTest::GetRilNetworkSelectionModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilNetworkSelectionModeTest -->");
    mRilManager_->GetNetworkSelectionMode(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilNetworkSelectionModeTest --> GetRilNetworkSelectionModeTest finished");
}

void RilUnitTest::GetRilPreferredNetwrokTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilPreferredNetwrokTest -->");
    mRilManager_->GetPreferredNetwork(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilPreferredNetwrokTest --> GetRilPreferredNetwrokTest finished");
}

void RilUnitTest::GetRilImsRegStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilImsRegStatusTest -->");
    mRilManager_->GetImsRegStatus(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilImsRegStatusTest --> GetRilImsRegStatusTest finished");
}

void RilUnitTest::GetVoiceRadioTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetVoiceRadioTest -->");
    mRilManager_->GetVoiceRadio(result);
    TELEPHONY_LOGI("RilUnitTest::GetVoiceRadioTest --> GetVoiceRadioTest finished");
}

void RilUnitTest::GetRilPsAttachStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilPsAttachStatusTest -->");
    mRilManager_->GetPsAttachStatus(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilPsAttachStatusTest --> GetRilPsAttachStatusTest finished");
}

void RilUnitTest::SetRilPreferredNetwrokTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t preferredNetwork;

    TELEPHONY_LOGI("RilUnitTest::SetRilPreferredNetwrokTest -->");

    cout << "please enter the preferredNetwork:";
    cin >> preferredNetwork;

    mRilManager_->SetPreferredNetwrok(preferredNetwork, result);
    TELEPHONY_LOGI("RilUnitTest::SetRilPreferredNetwrokTest --> SetRilPreferredNetwrokTest finished");
}

void RilUnitTest::SetRilLocationUpdateTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t enableFlg;

    TELEPHONY_LOGI("RilUnitTest::SetRilLocationUpdateTest -->");

    cout << "please enter the enableFlg:";
    cin >> enableFlg;

    mRilManager_->SetLocationUpdate(enableFlg, result);
    TELEPHONY_LOGI("RilUnitTest::SetRilLocationUpdateTest --> SetRilLocationUpdateTest finished");
}

void RilUnitTest::SetRilPsAttachStatusTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t attachFlg;

    TELEPHONY_LOGI("RilUnitTest::SetRilPsAttachStatusTest -->");

    cout << "please enter the attachFlg:";
    cin >> attachFlg;

    mRilManager_->SetPsAttachStatus(attachFlg, result);
    TELEPHONY_LOGI("RilUnitTest::SetRilPsAttachStatusTest --> SetRilPsAttachStatusTest finished");
}

void RilUnitTest::SetRilNetworkSelectionModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t mode;
    string plmn;

    TELEPHONY_LOGI("RilUnitTest::SetRilNetworkSelectionModeTest -->");

    cout << "please enter the mode:";
    cin >> mode;

    if (mode != 0) {
        cout << "please enter the plmn:";
        cin >> plmn;
    }

    mRilManager_->SetNetworkSelectionMode(mode, plmn, result);
    TELEPHONY_LOGI("RilUnitTest::SetRilNetworkSelectionModeTest --> SetRilNetworkSelectionModeTest finished");
}

void RilUnitTest::SendRilCmSmsTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsTest -->");
    string smscPdu;
    string pdu;
    cout << "please entry smsc(Pdu):" << endl;
    cin >> smscPdu;
    cout << "please entry sms message(Pdu):" << endl;
    cin >> pdu;

    mRilManager_->SendSms(smscPdu, pdu, result->GetOwner(), result);
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsTest --> SendRilCmSmsTest finished");
}

void RilUnitTest::SendRilCmSmsMoreModeTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsMoreModeTest -->");
    string smscPdu;
    string pdu;
    cout << "please entry smsc(Pdu):" << endl;
    cin >> smscPdu;
    cout << "please entry sms message(Pdu):" << endl;
    cin >> pdu;

    mRilManager_->SendSmsMoreMode(smscPdu, pdu, result->GetOwner(), result);
    TELEPHONY_LOGI("RilUnitTest::SendRilCmSmsMoreModeTest --> SendRilCmSmsMoreModeTest finished");
}

void RilUnitTest::SendSmsAckTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SendSmsAckTest -->");
    const int32_t memExceededCause = 0xD3;
    bool success;
    int32_t cause;

    cout << "send sms ack types are as follows:" << endl <<
            "\t[0]: report OK" << endl <<
            "\t[1]: report ERROR(Memory Capacity Exceeded)" << endl;
    int32_t ackType = InputInt32(0, 1, "ack type");
    if (ackType == 1) {
        success = 0;
        cause = memExceededCause;
    } else {
        success = 1;
        cause = 0;
    }

    mRilManager_->SendSmsAck(success, cause, result);
    TELEPHONY_LOGI("RilUnitTest::SendSmsAckTest --> SendSmsAckTest finished");
}

void RilUnitTest::GetSmscAddrTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetSmscAddrTest -->");
    mRilManager_->GetSmscAddr(result);
    TELEPHONY_LOGI("RilUnitTest::GetSmscAddrTest --> GetSmscAddrTest finished");
}

void RilUnitTest::SetSmscAddrTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetSmscAddrTest -->");
    int32_t tosca;
    string address;
    cout << "please entry tosca:" << endl;
    cin >> tosca;
    cout << "please entry smsc:" << endl;
    cin >> address;
    mRilManager_->SetSmscAddr(tosca, address, result);
    TELEPHONY_LOGI("RilUnitTest::SetSmscAddrTest --> SetSmscAddrTest finished");
}

void RilUnitTest::SetRilCmRadioPowerTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetRilCmRadioPowerTest -->");

    int32_t fun = InputInt32(0, 0x7fffffff, "fun");
    mRilManager_->SetRadioState(fun, 0, result);

    TELEPHONY_LOGI("RilUnitTest::SetRilCmRadioPowerTest --> SetRilCmRadioPowerTest finished");
}

void RilUnitTest::SetRilCmRadioPowerStressTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetRilCmRadioPowerStressTest -->");
    const int32_t REQ_NUM_PER_CYCAL = 2;
    for (int32_t i = 0; i < (STRESS_TEST_NUM / REQ_NUM_PER_CYCAL); i++) {
        mRilManager_->SetRadioState(0, 0, result);
        mRilManager_->SetRadioState(1, 0, result);
    }

    TELEPHONY_LOGI("RilUnitTest::SetRilCmRadioPowerStressTest --> SetRilCmRadioPowerStressTest finished");
}

void RilUnitTest::GetRilRadioPowerTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilRadioPowerTest -->");
    mRilManager_->GetRadioState(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilRadioPowerTest --> GetRilRadioPowerTest finished");
}

void RilUnitTest::GetImeiTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetImeiTest -->");
    mRilManager_->GetImei(result);
    TELEPHONY_LOGI("RilUnitTest::GetImeiTest --> GetImeiTest finished");
}

void RilUnitTest::GetImeiStressTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetImeiStressTest -->");

    for (int32_t i = 0; i < STRESS_TEST_NUM; i++) {
        mRilManager_->GetImei(result);
    }

    TELEPHONY_LOGI("RilUnitTest::GetImeiStressTest --> GetImeiStressTest finished");
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
    string apn;
    cout << "please enter the apn:";
    cin >> apn;
    string username;

    cout << "please enter the username:";
    cin >> username;

    string key;
    cout << "please enter the password:";
    cin >> key;

    string protocol;
    cout << "please enter the protocol:";
    cin >> protocol;
    RilDataProfileTest dataProfile(0, apn, protocol, 1, username, key, protocol);
    mRilManager_->ActivatePdpContext(1, dataProfile, true, true, result);
    TELEPHONY_LOGI("RilUnitTest::SetupRilCmDataCallTest --> SetupRilCmDataCallTest finished");
}

void RilUnitTest::DeactivateRilCmDataCallTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    int32_t cid;
    cout << "please enter the cid:";
    cin >> cid;
    TELEPHONY_LOGI("RilUnitTest::DeactivateRilCmDataCallTest -->");
    mRilManager_->DeactivatePdpContext(cid, REASON, result);
    TELEPHONY_LOGI("RilUnitTest::DeactivateRilCmDataCallTest --> DeactivateRilCmDataCallTest finished");
}

void RilUnitTest::GetRilCmDataCallListTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetRilCmDataCallListTest -->");
    mRilManager_->GetPdpContext(result);
    TELEPHONY_LOGI("RilUnitTest::GetRilCmDataCallListTest --> GetRilCmDataCallListTest finished");
}

void RilUnitTest::SetInitialApnTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SetInitialApnTest -->");
    string apn;
    cout << "please enter the apn:";
    cin >> apn;
    string username;

    cout << "please enter the username:";
    cin >> username;

    string key;
    cout << "please enter the password:";
    cin >> key;

    string protocol;
    cout << "please enter the protocol:";
    cin >> protocol;
    RilDataProfileTest dataProfile(0, apn, protocol, 1, username, key, protocol);
    mRilManager_->SetInitApnInfo(dataProfile, result);
    TELEPHONY_LOGI("RilUnitTest::SetInitialApnTest --> SetInitialApnTest finished");
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
            return;
        }
    }

    cout << "----> Unsupported Command!" << endl;
}

void RilUnitTest::OnStressInput(int32_t what, const OHOS::AppExecFwk::InnerEvent::Pointer &event)
{
    auto itFunc = stressMemberFuncMap_.find(what);
    if (itFunc != stressMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(event);
            return;
        }
    }

    cout << "----> Unsupported Command!" << endl;
}

void RilUnitTest::SimOpenLogicalChannel(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SimOpenLogicalChannel -->");
    mRilManager_->SimOpenLogicalChannel("appID", 0, result);
    TELEPHONY_LOGI("RilUnitTest::SimOpenLogicalChannel --> SimOpenLogicalChannel finished");
}

void RilUnitTest::SimCloseLogicalChannel(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SimCloseLogicalChannel -->");
    mRilManager_->SimCloseLogicalChannel(0, result);
    TELEPHONY_LOGI("RilUnitTest::SimCloseLogicalChannel --> SimCloseLogicalChannel finished");
}

void RilUnitTest::SimTransmitApduLogicalChannel(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::SimTransmitApduLogicalChannel -->");
    ApduSimIORequestInfo reqInfo = ApduSimIORequestInfo();
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0;
    reqInfo.instruction = 1;
    reqInfo.p1 = 1;
    reqInfo.p2 = REQ_INFO_P2;
    reqInfo.p3 = REQ_INFO_P3;
    reqInfo.data = "apdu";
    mRilManager_->SimTransmitApduLogicalChannel(reqInfo, result);
    TELEPHONY_LOGI("RilUnitTest::SimTransmitApduLogicalChannel --> SimTransmitApduLogicalChannel finished");
}

void RilUnitTest::UnlockSimLock(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::UnlockSimLock -->");
    mRilManager_->UnlockSimLock(0, "1234", result);
    TELEPHONY_LOGI("RilUnitTest::UnlockSimLock --> UnlockSimLock finished");
}

void RilUnitTest::GetLinkBandwidthInfoTest(const OHOS::AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilUnitTest::GetLinkBandwidthInfoTest -->");
    int32_t cid = 1;
    mRilManager_->GetLinkBandwidthInfo(cid, result);
    TELEPHONY_LOGI("RilUnitTest::GetLinkBandwidthInfoTest --> GetLinkBandwidthInfoTest finished");
}

void RilUnitTest::OnInitStressInterface()
{
    // set
    stressMemberFuncMap_[HREQ_CALL_DIAL] = &RilUnitTest::RilCmDialStressTest;

    // get
    stressMemberFuncMap_[HREQ_MODEM_GET_IMEI] = &RilUnitTest::GetImeiStressTest;

    // cfun
    stressMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &RilUnitTest::SetRilCmRadioPowerStressTest;
}

void RilUnitTest::OnInitProcessInterface()
{
    // call
    memberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &RilUnitTest::GetCallListTest;
    memberFuncMap_[HREQ_CALL_DIAL] = &RilUnitTest::RilCmDialTest;
    memberFuncMap_[HREQ_CALL_HANGUP] = &RilUnitTest::HangupTest;
    memberFuncMap_[HREQ_CALL_REJECT] = &RilUnitTest::RejectTest;
    memberFuncMap_[HREQ_CALL_ANSWER] = &RilUnitTest::AnswerCallTest;
    memberFuncMap_[HREQ_CALL_HOLD_CALL] = &RilUnitTest::HoldCallTest;
    memberFuncMap_[HREQ_CALL_UNHOLD_CALL] = &RilUnitTest::UnHoldCallTest;
    memberFuncMap_[HREQ_CALL_SWITCH_CALL] = &RilUnitTest::SwitchCallTest;

    memberFuncMap_[HREQ_CALL_COMBINE_CONFERENCE] = &RilUnitTest::RilCmJoinCallTest;
    memberFuncMap_[HREQ_CALL_SEPARATE_CONFERENCE] = &RilUnitTest::RilCmSplitCallTest;

    memberFuncMap_[HREQ_CALL_CALL_SUPPLEMENT] = &RilUnitTest::CallSupplementTest;
    memberFuncMap_[HREQ_CALL_SEND_DTMF] = &RilUnitTest::SendDtmfTest;
    memberFuncMap_[HREQ_CALL_START_DTMF] = &RilUnitTest::StartDtmfTest;
    memberFuncMap_[HREQ_CALL_STOP_DTMF] = &RilUnitTest::StopDtmfTest;

    memberFuncMap_[HREQ_CALL_SET_USSD] = &RilUnitTest::SetUssdCusdTest;
    memberFuncMap_[HREQ_CALL_GET_USSD] = &RilUnitTest::GetUssdCusdTest;

    memberFuncMap_[HREQ_CALL_SET_MUTE] = &RilUnitTest::SetMuteTest;
    memberFuncMap_[HREQ_CALL_GET_MUTE] = &RilUnitTest::GetMuteTest;
    memberFuncMap_[HREQ_CALL_GET_EMERGENCY_LIST] = &RilUnitTest::GetEmergencyListTest;
    memberFuncMap_[HREQ_CALL_GET_FAIL_REASON] = &RilUnitTest::GetFailReasonTest;

    // sms
    memberFuncMap_[HREQ_SMS_SEND_GSM_SMS] = &RilUnitTest::SendRilCmSmsTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &RilUnitTest::SendRilCmSmsMoreModeTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_ACK] = &RilUnitTest::SendSmsAckTest;
    memberFuncMap_[HREQ_SMS_SET_SMSC_ADDR] = &RilUnitTest::SetSmscAddrTest;
    memberFuncMap_[HREQ_SMS_GET_SMSC_ADDR] = &RilUnitTest::GetSmscAddrTest;

    // sim
    memberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &RilUnitTest::GetRilCmIccCardStatusTest;
    memberFuncMap_[HREQ_SIM_GET_IMSI] = &RilUnitTest::GetRilCmImsiForAppTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_IO] = &RilUnitTest::IccRilSimIoForAppTest;
    memberFuncMap_[HREQ_SIM_OPEN_LOGICAL_CHANNEL] = &RilUnitTest::SimOpenLogicalChannel;
    memberFuncMap_[HREQ_SIM_CLOSE_LOGICAL_CHANNEL] = &RilUnitTest::SimCloseLogicalChannel;
    memberFuncMap_[HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL] = &RilUnitTest::SimTransmitApduLogicalChannel;
    memberFuncMap_[HREQ_SIM_UNLOCK_SIM_LOCK] = &RilUnitTest::UnlockSimLock;

    // data
    memberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &RilUnitTest::SetupRilCmDataCallTest;
    memberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &RilUnitTest::DeactivateRilCmDataCallTest;
    memberFuncMap_[HREQ_DATA_GET_LINK_BANDWIDTH_INFO] = &RilUnitTest::GetLinkBandwidthInfoTest;
    memberFuncMap_[HREQ_DATA_GET_PDP_CONTEXT_LIST] = &RilUnitTest::GetRilCmDataCallListTest;
    memberFuncMap_[HREQ_DATA_SET_INIT_APN_INFO] = &RilUnitTest::SetInitialApnTest;

    // network
    memberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &RilUnitTest::GetRilCmSignalIntensityTest;
    memberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &RilUnitTest::GetRilCmCsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &RilUnitTest::GetRilCmPsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &RilUnitTest::GetRilCmOperatorTest;
    memberFuncMap_[HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION] = &RilUnitTest::GetRilNetworkSearchInfoTest;
    memberFuncMap_[HREQ_NETWORK_GET_NETWORK_SELECTION_MODE] = &RilUnitTest::GetRilNetworkSelectionModeTest;
    memberFuncMap_[HREQ_NETWORK_SET_NETWORK_SELECTION_MODE] = &RilUnitTest::SetRilNetworkSelectionModeTest;
    memberFuncMap_[HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST] = &RilUnitTest::GetRilCmCellInfoListTest;
    memberFuncMap_[HREQ_NETWORK_GET_CURRENT_CELL_INFO] = &RilUnitTest::GetRilCurrentCellInfoTest;
    memberFuncMap_[HREQ_NETWORK_GET_PREFERRED_NETWORK] = &RilUnitTest::GetRilPreferredNetwrokTest;
    memberFuncMap_[HREQ_NETWORK_SET_PREFERRED_NETWORK] = &RilUnitTest::SetRilPreferredNetwrokTest;
    memberFuncMap_[HREQ_NETWORK_GET_IMS_REG_STATUS] = &RilUnitTest::GetRilImsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_SET_PS_ATTACH_STATUS] = &RilUnitTest::SetRilPsAttachStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_PS_ATTACH_STATUS] = &RilUnitTest::GetRilPsAttachStatusTest;
    memberFuncMap_[HREQ_NETWORK_SET_LOCATE_UPDATES] = &RilUnitTest::SetRilLocationUpdateTest;

    // modem
    memberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &RilUnitTest::SetRilCmRadioPowerTest;
    memberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &RilUnitTest::GetRilRadioPowerTest;
    memberFuncMap_[HREQ_MODEM_GET_IMEI] = &RilUnitTest::GetImeiTest;
    memberFuncMap_[HREQ_MODEM_GET_VOICE_RADIO] = &RilUnitTest::GetVoiceRadioTest;
}

void RilUnitTest::OnInitInterface()
{
    OnInitStressInterface();
    OnInitProcessInterface();
}

} // namespace
} // namespace Telephony
} // namespace OHOS

using namespace OHOS;
using namespace OHOS::Telephony;

static int32_t PrintMenu()
{
    const int32_t maxServiceType = 7;
    cout << endl << "---->Test Module:" << endl;
    cout << "----> 0. EXIT --------------------------" << endl;
    cout << "----> 1. MODEM -------------------------" << endl;
    cout << "----> 2. CALL --------------------------" << endl;
    cout << "----> 3. NETWORK -----------------------" << endl;
    cout << "----> 4. SIM ---------------------------" << endl;
    cout << "----> 5. DATA --------------------------" << endl;
    cout << "----> 6. SMS ---------------------------" << endl;
    cout << "----> 7. STRESS ------------------------" << endl;
    int32_t choice = InputInt32(0, maxServiceType, "Test Module Index");
    cout << "---->You choose: " << choice << endl;
    return choice;
}

static int32_t PrintCallMenu()
{
    cout << "---->[MODULE]CALL:" << endl;
    cout << "----> [" << HREQ_CALL_BASE             << "] ---->Back to the previous menu."      << endl;
    cout << "----> [" << HREQ_CALL_GET_CALL_LIST    << "] ---->[ HREQ_CALL_GET_CALL_LIST ]"     << endl;
    cout << "----> [" << HREQ_CALL_DIAL             << "] ---->[ HREQ_CALL_DIAL ]"              << endl;
    cout << "----> [" << HREQ_CALL_HANGUP           << "] ---->[ HREQ_CALL_HANGUP ]"            << endl;
    cout << "----> [" << HREQ_CALL_REJECT           << "] ---->[ HREQ_CALL_REJECT ]"            << endl;
    cout << "----> [" << HREQ_CALL_ANSWER           << "] ---->[ HREQ_CALL_ANSWER ]"            << endl;
    cout << "----> [" << HREQ_CALL_HOLD_CALL        << "] ---->[ HREQ_CALL_HOLD_CALL ]"         << endl;
    cout << "----> [" << HREQ_CALL_UNHOLD_CALL      << "] ---->[ HREQ_CALL_UNHOLD_CALL ]"       << endl;
    cout << "----> [" << HREQ_CALL_SWITCH_CALL      << "] ---->[ HREQ_CALL_SWITCH_CALL ]"       << endl;

    cout << "----> [" << HREQ_CALL_COMBINE_CONFERENCE   << "] ---->[ HREQ_CALL_COMBINE_CONFERENCE ]"    << endl;
    cout << "----> [" << HREQ_CALL_SEPARATE_CONFERENCE  << "] ---->[ HREQ_CALL_SEPARATE_CONFERENCE ]"   << endl;

    cout << "----> [" << HREQ_CALL_CALL_SUPPLEMENT  << "] ---->[ HREQ_CALL_CALL_SUPPLEMENT ]"   << endl;
    cout << "----> [" << HREQ_CALL_SEND_DTMF        << "] ---->[ HREQ_CALL_SEND_DTMF ]"         << endl;
    cout << "----> [" << HREQ_CALL_START_DTMF       << "] ---->[ HREQ_CALL_START_DTMF ]"        << endl;
    cout << "----> [" << HREQ_CALL_STOP_DTMF        << "] ---->[ HREQ_CALL_STOP_DTMF ]"         << endl;

    cout << "----> [" << HREQ_CALL_SET_USSD         << "] ---->[ HREQ_CALL_SET_USSD ]"          << endl;
    cout << "----> [" << HREQ_CALL_GET_USSD         << "] ---->[ HREQ_CALL_GET_USSD ]"          << endl;

    cout << "----> [" << HREQ_CALL_SET_MUTE         << "] ---->[ HREQ_CALL_SET_MUTE ]"          << endl;
    cout << "----> [" << HREQ_CALL_GET_MUTE         << "] ---->[ HREQ_CALL_GET_MUTE ]"          << endl;
    cout << "----> [" << HREQ_CALL_GET_EMERGENCY_LIST   << "] ---->[ HREQ_CALL_GET_EMERGENCY_LIST ]"     << endl;
    cout << "----> [" << HREQ_CALL_GET_FAIL_REASON  << "] ---->[ HREQ_CALL_GET_FAIL_REASON ]"   << endl;

    int32_t choice = InputInt32(HREQ_CALL_BASE, HREQ_SMS_BASE - 1, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_CALL_BASE) ? -1 : choice;
    return choice;
}

static int32_t PrintSmsMenu()
{
    cout << "---->[MODULE]SMS:" << endl;
    cout << "----> [" << HREQ_SMS_BASE << "] ---->Back to the previous menu." << endl;
    cout << "----> [" << HREQ_SMS_SEND_GSM_SMS << "] ---->[ HREQ_SMS_SEND_GSM_SMS ]" << endl;
    cout << "----> [" << HREQ_SMS_SEND_SMS_MORE_MODE << "] ---->[ HREQ_SMS_SEND_SMS_MORE_MODE ]" << endl;
    cout << "----> [" << HREQ_SMS_SEND_SMS_ACK << "] ---->[ HREQ_SMS_SEND_SMS_ACK ]" << endl;
    cout << "----> [" << HREQ_SMS_SET_SMSC_ADDR << "] ---->[ HREQ_SMS_SET_SMSC_ADDR ]" << endl;
    cout << "----> [" << HREQ_SMS_GET_SMSC_ADDR << "] ---->[ HREQ_SMS_GET_SMSC_ADDR ]" << endl;

    int32_t choice = InputInt32(HREQ_SMS_BASE, HREQ_SIM_BASE - 1, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_SMS_BASE) ? -1 : choice;
    return choice;
}

static int32_t PrintSimMenu()
{
    cout << "---->[MODULE]SIM:" << endl;
    cout << "----> [" << HREQ_SIM_BASE                  << "] ---->Back to the previous menu."          << endl;
    cout << "----> [" << HREQ_SIM_GET_SIM_STATUS        << "] ---->[ HREQ_SIM_GET_SIM_STATUS ]"         << endl;
    cout << "----> [" << HREQ_SIM_GET_IMSI              << "] ---->[ HREQ_SIM_GET_IMSI ]"               << endl;
    cout << "----> [" << HREQ_SIM_GET_SIM_IO            << "] ---->[ HREQ_SIM_GET_SIM_IO ]"             << endl;
    cout << "----> [" << HREQ_SIM_OPEN_LOGICAL_CHANNEL  << "] ---->[ HREQ_SIM_OPEN_LOGICAL_CHANNEL ]"   << endl;
    cout << "----> [" << HREQ_SIM_CLOSE_LOGICAL_CHANNEL << "] ---->[ HREQ_SIM_CLOSE_LOGICAL_CHANNEL ]"  << endl;
    cout << "----> [" << HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL
        << "] ---->[ HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL ]" << endl;
    cout << "----> [" << HREQ_SIM_UNLOCK_SIM_LOCK       << "] ---->[ HREQ_SIM_UNLOCK_SIM_LOCK ]"        << endl;

    int32_t choice = InputInt32(HREQ_SIM_BASE, HREQ_DATA_BASE - 1, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_SIM_BASE) ? -1 : choice;
    return choice;
}

static int32_t PrintDataMenu()
{
    cout << "---->[MODULE]DATA:" << endl;
    cout << "----> [" << HREQ_DATA_BASE                     << "] ---->Back to the previous menu."             << endl;
    cout << "----> [" << HREQ_DATA_ACTIVATE_PDP_CONTEXT     << "] ---->[ HREQ_DATA_ACTIVATE_PDP_CONTEXT ]"     << endl;
    cout << "----> [" << HREQ_DATA_DEACTIVATE_PDP_CONTEXT   << "] ---->[ HREQ_DATA_DEACTIVATE_PDP_CONTEXT ]"   << endl;
    cout << "----> [" << HREQ_DATA_GET_LINK_BANDWIDTH_INFO  << "] ---->[ HREQ_DATA_GET_LINK_BANDWIDTH_INFO ]"  << endl;
    cout << "----> [" << HREQ_DATA_GET_PDP_CONTEXT_LIST     << "] ---->[ HREQ_DATA_GET_PDP_CONTEXT_LIST ]"     << endl;
    cout << "----> [" << HREQ_DATA_SET_INIT_APN_INFO        << "] ---->[ HREQ_DATA_SET_INIT_APN_INFO ]"        << endl;

    int32_t choice = InputInt32(HREQ_DATA_BASE, HREQ_NETWORK_BASE - 1, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_DATA_BASE) ? -1 : choice;
    return choice;
}

static int32_t PrintNetworkMenu()
{
    cout << "---->[MODULE]NETWORK:" << endl;
    cout << "----> [" << HREQ_NETWORK_BASE                  << "] ---->Back to the previous menu."           << endl;
    cout << "----> [" << HREQ_NETWORK_GET_SIGNAL_STRENGTH   << "] ---->[ HREQ_NETWORK_GET_SIGNAL_STRENGTH ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_CS_REG_STATUS     << "] ---->[ HREQ_NETWORK_GET_CS_REG_STATUS ]"   << endl;
    cout << "----> [" << HREQ_NETWORK_GET_PS_REG_STATUS     << "] ---->[ HREQ_NETWORK_GET_PS_REG_STATUS ]"   << endl;
    cout << "----> [" << HREQ_NETWORK_GET_OPERATOR_INFO     << "] ---->[ HREQ_NETWORK_GET_OPERATOR_INFO ]"   << endl;

    cout << "----> [" << HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION
        << "] ---->[ HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION ]" << endl;

    cout << "----> [" << HREQ_NETWORK_GET_NETWORK_SELECTION_MODE
        << "] ---->[ HREQ_NETWORK_GET_NETWORK_SELECTION_MODE ]"     << endl;

    cout << "----> [" << HREQ_NETWORK_SET_NETWORK_SELECTION_MODE
        << "] ---->[ HREQ_NETWORK_SET_NETWORK_SELECTION_MODE ]"     << endl;

    cout << "----> [" << HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST
        << "] ---->[ HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST ]"  << endl;

    cout << "----> [" << HREQ_NETWORK_GET_CURRENT_CELL_INFO << "] ---->[ HREQ_NETWORK_GET_CURRENT_CELL_INFO ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_PREFERRED_NETWORK << "] ---->[ HREQ_NETWORK_GET_PREFERRED_NETWORK ]" << endl;
    cout << "----> [" << HREQ_NETWORK_SET_PREFERRED_NETWORK << "] ---->[ HREQ_NETWORK_SET_PREFERRED_NETWORK ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_IMS_REG_STATUS    << "] ---->[ HREQ_NETWORK_GET_IMS_REG_STATUS ]"    << endl;
    cout << "----> [" << HREQ_NETWORK_SET_PS_ATTACH_STATUS  << "] ---->[ HREQ_NETWORK_SET_PS_ATTACH_STATUS ]"  << endl;
    cout << "----> [" << HREQ_NETWORK_GET_PS_ATTACH_STATUS  << "] ---->[ HREQ_NETWORK_GET_PS_ATTACH_STATUS ]"  << endl;
    cout << "----> [" << HREQ_NETWORK_SET_LOCATE_UPDATES    << "] ---->[ HREQ_NETWORK_SET_LOCATE_UPDATES ]"    << endl;

    int32_t choice = InputInt32(HREQ_NETWORK_BASE, HREQ_COMMON_BASE - 1, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_NETWORK_BASE) ? -1 : choice;
    return choice;
}

static int32_t PrintModemMenu()
{
    cout << "---->[MODULE]MODEM:" << endl;
    cout << "----> [" << HREQ_COMMON_BASE << "] ---->Back to the previous menu."      << endl;
    cout << "----> [" << HREQ_MODEM_SET_RADIO_STATUS << "] ---->[ HREQ_MODEM_SET_RADIO_STATUS ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_RADIO_STATUS << "] ---->[ HREQ_MODEM_GET_RADIO_STATUS ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_IMEI << "] ---->[ HREQ_MODEM_GET_IMEI ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_VOICE_RADIO << "] ---->[ HREQ_MODEM_GET_VOICE_RADIO ]" << endl;

    int32_t choice = InputInt32(HREQ_COMMON_BASE, HREQ_MODEM_EXIT - 1, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_COMMON_BASE) ? -1 : choice;
    return choice;
}

static int32_t PrintStressMenu()
{
    cout << "---->[MODULE]STRESS:" << endl;
    cout << "----> [" << 0 << "] ---->Back to the previous menu." << endl;
    cout << "----> [" << HREQ_CALL_DIAL << "] ---->[ HREQ_CALL_DIAL ]" << endl;
    cout << "----> [" << HREQ_MODEM_SET_RADIO_STATUS << "] ---->[ HREQ_MODEM_SET_RADIO_STATUS ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_IMEI << "] ---->[ HREQ_MODEM_GET_IMEI ]" << endl;

    int32_t choice = InputInt32(0, HREQ_MODEM_EXIT - 1, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == 0) ? -1 : choice;
    return choice;
}

namespace {
enum class UnitTestResult { RIL_TEST_SUCCESS = 0, RIL_TEST_FAIL = 1 };
}

static int32_t SwitchMenu(int32_t module, bool *loopFlag)
{
    int32_t mWhat = -1;
    switch (module) {
        case 0: /* exit */
            *loopFlag = false;
            break;
        case 1: /* modem */
            mWhat = PrintModemMenu();
            break;
        case 2: /* call */
            mWhat = PrintCallMenu();
            break;
        case 3: /* network */
            mWhat = PrintNetworkMenu();
            break;
        case 4: /* sim */
            mWhat = PrintSimMenu();
            break;
        case 5: /* data */
            mWhat = PrintDataMenu();
            break;
        case 6: /* sms */
            mWhat = PrintSmsMenu();
            break;
        case 7: /* stress */
            mWhat = PrintStressMenu();
            break;
        default:
            break;
    }
    return mWhat;
}

int32_t main()
{
    static std::unique_ptr<OHOS::Telephony::RilUnitTest> rilUnitTest;
    static std::shared_ptr<AppExecFwk::EventRunner> runner;
    static std::shared_ptr<RilUnitTest::DemoHandler> handler;
    cout << "---->Ril Adapter Test Enter V1.0.18" << endl;
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

    rilUnitTest->OnInitInterface();

    bool loopFlag = true;
    int32_t mWhat = -1;
    int32_t module = -1;
    while (loopFlag) {
        module = PrintMenu();
        mWhat = SwitchMenu(module, &loopFlag);
        if (mWhat > 0) {
            auto event = OHOS::AppExecFwk::InnerEvent::Get(mWhat);
            event->SetOwner(handler);

            if (module == 7) {
                rilUnitTest->OnStressInput(mWhat, event);
            } else {
                rilUnitTest->OnProcessInput(mWhat, event);
            }

            /* 等待结果回复打印 */
            usleep(500000);
        }
    }

    cout << "---->Ril Adapter Test end" << endl;
    return static_cast<int32_t>(UnitTestResult::RIL_TEST_SUCCESS);
}
