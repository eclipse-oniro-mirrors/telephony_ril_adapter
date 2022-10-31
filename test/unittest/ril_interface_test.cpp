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

#include "ril_interface_test.h"

using namespace std;
namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_0;
sptr<OHOS::HDI::Ril::V1_0::IRil> g_rilInterface = nullptr;
constexpr int32_t DEFAULT_CHOICE = -1;
constexpr int32_t MENU_OFFSET = 1;
constexpr int32_t WAIT_TIME = 500000;
constexpr int32_t IN_BUF_LEN = 1024;
constexpr int32_t MAX_RANDOM = 100000000;
constexpr int32_t MAX_CALL_TYPE = 3;
constexpr int32_t STRESS_TEST_NUM = 10000;
constexpr int32_t MAX_CALL_ID = 7;
constexpr int32_t HANGUP_ACTIVE = 2;
constexpr int32_t DTMF_ON_LEN = 300;
constexpr int32_t RAT_TYPE_LTE = 5;
constexpr int32_t BANDWIDTH_HYSTERESIS_MS = 3000;
constexpr int32_t BANDWIDTH_HYSTERESIS_KBPS = 50;
constexpr int32_t SIM_LOCK_MODE_QUERY = 2;
constexpr int32_t MAX_UPLINK_LINK_BANDWIDTH[] = {
    100,
    500,
    1000,
    5000,
    10000,
    20000,
    50000,
    100000,
    200000,
};
constexpr int32_t MAX_DOWNLINK_LINK_BANDWIDTH[] = {
    100, // VoIP
    500, // Web
    1000, // SD
    5000, // HD
    10000, // file
    20000, // 4K
    50000, // LTE
    100000,
    200000, // 5G
    500000,
    1000000,
};
typedef enum {
    HREQ_CALL_BASE = 0,
    HREQ_CALL_GET_CALL_LIST,
    HREQ_CALL_DIAL,
    HREQ_CALL_HANGUP,
    HREQ_CALL_REJECT,
    HREQ_CALL_ANSWER,
    HREQ_CALL_HOLD_CALL, // call hold value 6
    HREQ_CALL_UNHOLD_CALL, // call active value 6
    HREQ_CALL_SWITCH_CALL,
    HREQ_CALL_COMBINE_CONFERENCE,
    HREQ_CALL_SEPARATE_CONFERENCE, // Keep all other calls except the xth call
    HREQ_CALL_CALL_SUPPLEMENT,
    HREQ_CALL_SEND_DTMF,
    HREQ_CALL_START_DTMF,
    HREQ_CALL_STOP_DTMF,
    HREQ_CALL_SET_CLIP,
    HREQ_CALL_GET_CLIP,
    HREQ_CALL_GET_CALL_WAITING,
    HREQ_CALL_SET_CALL_WAITING,
    HREQ_CALL_GET_CALL_RESTRICTION,
    HREQ_CALL_SET_CALL_RESTRICTION,
    HREQ_CALL_GET_CALL_TRANSFER_INFO,
    HREQ_CALL_SET_CALL_TRANSFER_INFO,
    HREQ_CALL_GET_CLIR,
    HREQ_CALL_SET_CLIR,
    HREQ_CALL_GET_CALL_PREFERENCE,
    HREQ_CALL_SET_CALL_PREFERENCE,
    HREQ_CALL_SET_USSD,
    HREQ_CALL_GET_USSD,
    HREQ_CALL_SET_MUTE,
    HREQ_CALL_GET_MUTE,
    HREQ_CALL_GET_EMERGENCY_LIST,
    HREQ_CALL_SET_EMERGENCY_LIST,
    HREQ_CALL_GET_FAIL_REASON,
    HREQ_CALL_SET_BARRING_PASSWORD,

    HREQ_SMS_BASE = 100,
    HREQ_SMS_SEND_GSM_SMS,
    HREQ_SMS_SEND_CDMA_SMS,
    HREQ_SMS_ADD_SIM_MESSAGE,
    HREQ_SMS_DEL_SIM_MESSAGE,
    HREQ_SMS_UPDATE_SIM_MESSAGE,
    HREQ_SMS_SEND_SMS_MORE_MODE,
    HREQ_SMS_SEND_SMS_ACK,
    HREQ_SMS_SET_SMSC_ADDR,
    HREQ_SMS_GET_SMSC_ADDR,
    HREQ_SMS_SET_CB_CONFIG,
    HREQ_SMS_GET_CB_CONFIG,
    HREQ_SMS_GET_CDMA_CB_CONFIG,
    HREQ_SMS_SET_CDMA_CB_CONFIG,
    HREQ_SMS_ADD_CDMA_SIM_MESSAGE,
    HREQ_SMS_DEL_CDMA_SIM_MESSAGE,
    HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE,

    HREQ_SIM_BASE = 200,
    HREQ_SIM_GET_SIM_STATUS,
    HREQ_SIM_GET_IMSI,
    HREQ_SIM_GET_SIM_IO,
    HREQ_SIM_GET_SIM_LOCK_STATUS,
    HREQ_SIM_SET_SIM_LOCK,
    HREQ_SIM_CHANGE_SIM_PASSWORD,
    HREQ_SIM_UNLOCK_PIN,
    HREQ_SIM_UNLOCK_PUK,
    HREQ_SIM_GET_SIM_PIN_INPUT_TIMES,
    HREQ_SIM_UNLOCK_PIN2,
    HREQ_SIM_UNLOCK_PUK2,
    HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES,
    HREQ_SIM_SET_ACTIVE_SIM,
    HREQ_SIM_GET_RADIO_PROTOCOL,
    HREQ_SIM_SET_RADIO_PROTOCOL,
    HREQ_SIM_STK_SEND_TERMINAL_RESPONSE,
    HREQ_SIM_STK_SEND_ENVELOPE,
    HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT,
    HREQ_SIM_STK_IS_READY,
    HREQ_SIM_OPEN_LOGICAL_CHANNEL,
    HREQ_SIM_CLOSE_LOGICAL_CHANNEL,
    HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL,
    HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL,
    HREQ_SIM_AUTHENTICATION,
    HREQ_SIM_UNLOCK_SIM_LOCK,

    HREQ_DATA_BASE = 300,
    HREQ_DATA_SET_INIT_APN_INFO,
    HREQ_DATA_DEACTIVATE_PDP_CONTEXT,
    HREQ_DATA_ACTIVATE_PDP_CONTEXT,
    HREQ_DATA_GET_PDP_CONTEXT_LIST,
    HREQ_DATA_GET_LINK_BANDWIDTH_INFO,
    HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE,
    HREQ_DATA_SET_DATA_PROFILE_INFO,
    HREQ_DATA_SEND_DATA_PERFORMANCE_MODE,
    HREQ_DATA_SEND_DATA_SLEEP_MODE,
    HREQ_DATA_SET_DATA_PERMITTED,

    HREQ_NETWORK_BASE = 400,
    HREQ_NETWORK_GET_SIGNAL_STRENGTH,
    HREQ_NETWORK_GET_CS_REG_STATUS,
    HREQ_NETWORK_GET_PS_REG_STATUS,
    HREQ_NETWORK_GET_OPERATOR_INFO,
    HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION,
    HREQ_NETWORK_GET_NETWORK_SELECTION_MODE,
    HREQ_NETWORK_SET_NETWORK_SELECTION_MODE,
    HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST,
    HREQ_NETWORK_GET_CURRENT_CELL_INFO,
    HREQ_NETWORK_SET_PREFERRED_NETWORK,
    HREQ_NETWORK_GET_PREFERRED_NETWORK,
    HREQ_NETWORK_GET_RADIO_CAPABILITY,
    HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG,
    HREQ_NETWORK_SET_LOCATE_UPDATES,
    HREQ_NETWORK_SET_NOTIFICATION_FILTER,
    HREQ_NETWORK_SET_DEVICE_STATE,

    HREQ_COMMON_BASE = 500,
    HREQ_MODEM_SHUT_DOWN,
    HREQ_MODEM_SET_RADIO_STATUS,
    HREQ_MODEM_GET_RADIO_STATUS,
    HREQ_MODEM_GET_IMEI,
    HREQ_MODEM_GET_MEID,
    HREQ_MODEM_GET_BASEBAND_VERSION,
    HREQ_MODEM_GET_VOICE_RADIO,
    HREQ_MODEM_EXIT = 1000
} HRilRequest;

enum class TestMenu : int32_t { NONE = -1, EXIT, MODEM, CALL, NETWORK, SIM, DATA, SMS, STRESS };

static int32_t InputInt32(int32_t start, int32_t end, const string &title)
{
    int32_t choice;
    bool firstFlg = true;

    do {
        if (!firstFlg) {
            cin.clear();
            cin.ignore(IN_BUF_LEN, '\n');
            cout << "---->Invalid Input, Please Enter Again !" << endl;
        }
        firstFlg = false;
        cout << "---->Please Enter " << title << " (" << start << "-" << end << "): ";
        cin >> choice;
    } while (cin.fail() || choice < start || choice > end);

    return choice;
}

int32_t RilInterfaceTest::GetSerialId()
{
    return rand() % MAX_RANDOM;
}

void RilInterfaceTest::GetCallListTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetCallListTest -->" << endl;
    int32_t ret = g_rilInterface->GetCallList(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetCallListTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::RilCmDialTest(int32_t slotId)
{
    cout << "RilInterfaceTest::RilCmDialTest -->" << endl;
    DialInfo dialInfo = {};
    dialInfo.clir = 0;
    cout << "---->Please enter the phone number:";
    cin >> dialInfo.address;
    int32_t ret = g_rilInterface->Dial(slotId, GetSerialId(), dialInfo);
    cout << "RilInterfaceTest::RilCmDialTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::RilCmDialStressTest(int32_t slotId)
{
    cout << "RilInterfaceTest::RilCmDialStressTest -->" << endl;
    DialInfo dialInfo = {};
    dialInfo.clir = 0;
    cout << "---->Please enter the phone number:";
    cin >> dialInfo.address;
    int32_t ret = -1;
    for (int32_t i = 0; i < STRESS_TEST_NUM; i++) {
        ret = g_rilInterface->Dial(slotId, GetSerialId(), dialInfo);
    }
    cout << "RilInterfaceTest::RilCmDialStressTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::HangupTest(int32_t slotId)
{
    cout << "RilInterfaceTest::HangupTest -->" << endl;
    int32_t callIndex = InputInt32(1, MAX_CALL_ID, "Hangup call number");
    int ret = g_rilInterface->Hangup(slotId, GetSerialId(), callIndex);
    cout << "RilInterfaceTest::HangupTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::RejectTest(int32_t slotId)
{
    cout << "RilInterfaceTest::RejectTest -->" << endl;
    int ret = g_rilInterface->Reject(slotId, GetSerialId());
    cout << "RilInterfaceTest::RejectTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::AnswerCallTest(int32_t slotId)
{
    cout << "RilInterfaceTest::AnswerCallTest -->" << endl;
    int ret = g_rilInterface->Answer(slotId, GetSerialId());
    cout << "RilInterfaceTest::AnswerCallTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::HoldCallTest(int32_t slotId)
{
    cout << "RilInterfaceTest::HoldCallTest -->" << endl;
    int ret = g_rilInterface->HoldCall(slotId, GetSerialId());
    cout << "RilInterfaceTest::HoldCallTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::UnHoldCallTest(int32_t slotId)
{
    cout << "RilInterfaceTest::UnHoldCallTest -->" << endl;
    int ret = g_rilInterface->UnHoldCall(slotId, GetSerialId());
    cout << "RilInterfaceTest::UnHoldCall finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SwitchCallTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SwitchCallTest -->" << endl;
    int ret = g_rilInterface->SwitchCall(slotId, GetSerialId());
    cout << "RilInterfaceTest::SwitchCallTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::RilCmJoinCallTest(int32_t slotId)
{
    cout << "RilInterfaceTest::RilCmJoinCallTest -->" << endl;
    int32_t callType = InputInt32(0, MAX_CALL_TYPE, "Call Type");
    int ret = g_rilInterface->CombineConference(slotId, GetSerialId(), callType);
    cout << "RilInterfaceTest::RilCmJoinCallTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::RilCmSplitCallTest(int32_t slotId)
{
    cout << "RilInterfaceTest::RilCmSplitCallTest -->" << endl;
    int32_t callIndex = InputInt32(1, MAX_CALL_ID, "The Split Call Number");
    cout << "call type:" << endl;
    cout << "0: Voice call" << endl;
    cout << "1: Video call: send one-way video, two-way voice" << endl;
    cout << "2: Video call: one-way receiving video, two-way voice" << endl;
    cout << "3: Video call: two-way video, two-way voice" << endl;
    int32_t callType = InputInt32(0, MAX_CALL_TYPE, "Call Type");
    int ret = g_rilInterface->SeparateConference(slotId, GetSerialId(), callIndex, callType);
    cout << "RilInterfaceTest::RilCmSplitCallTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::CallSupplementTest(int32_t slotId)
{
    cout << "RilInterfaceTest::CallSupplementTest -->" << endl;
    cout << "---->CallSupplement Hangup hold wait: 1, Hangup active: 2: " << endl;
    int32_t hangupType = InputInt32(1, HANGUP_ACTIVE, "Option");
    int ret = g_rilInterface->CallSupplement(slotId, GetSerialId(), hangupType);
    cout << "RilInterfaceTest::CallSupplementTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SendDtmfTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SendDtmfTest -->" << endl;
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 1;
    dtmfInfo.onLength = DTMF_ON_LEN;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    cout << "---->Please input dtmf key: ";
    cin >> dtmfInfo.dtmfKey;
    cout << endl << "====>Send Dtmf Key: " << dtmfInfo.dtmfKey << endl;

    int ret = g_rilInterface->SendDtmf(slotId, GetSerialId(), dtmfInfo);
    cout << "RilInterfaceTest::SendDtmfTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::StartDtmfTest(int32_t slotId)
{
    cout << "RilInterfaceTest::StartDtmfTest -->" << endl;
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 1;
    cout << "---->Please input dtmf key: ";
    cin >> dtmfInfo.dtmfKey;
    cout << endl << "====>Start Dtmf Key: " << dtmfInfo.dtmfKey << endl;
    int ret = g_rilInterface->StartDtmf(slotId, GetSerialId(), dtmfInfo);
    cout << "RilInterfaceTest::StartDtmfTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::StopDtmfTest(int32_t slotId)
{
    cout << "RilInterfaceTest::StopDtmfTest -->" << endl;
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 1;
    cout << "---->Please input dtmf key: ";
    cin >> dtmfInfo.dtmfKey;
    cout << endl << "====>Start Dtmf Key: " << dtmfInfo.dtmfKey << endl;
    int ret = g_rilInterface->StopDtmf(slotId, GetSerialId(), dtmfInfo);
    cout << "RilInterfaceTest::StopDtmfTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetUssdCusdTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetUssdCusdTest -->" << endl;
    int ret = g_rilInterface->SetUssd(slotId, GetSerialId(), "12345678");
    cout << "RilInterfaceTest::SetUssdCusdTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetUssdCusdTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetUssdCusdTest -->" << endl;
    int ret = g_rilInterface->GetUssd(1, GetSerialId());
    cout << "RilInterfaceTest::GetUssdCusdTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetMuteTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetMuteTest -->" << endl;
    cout << "---->Unmute: 0, Mute: 1: " << endl;
    int32_t mute = InputInt32(0, 1, "Option");
    int ret = g_rilInterface->SetMute(slotId, GetSerialId(), mute);
    cout << "RilInterfaceTest::SetMuteTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetMuteTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetMuteTest -->" << endl;
    int ret = g_rilInterface->GetMute(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetMuteTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetEmergencyListTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetEmergencyListTest -->" << endl;
    int32_t ret = g_rilInterface->GetEmergencyCallList(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetEmergencyListTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetEmergencyCallListTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetEmergencyCallListTest -->" << endl;
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(slotId, GetSerialId(), emergencyInfoList);
    cout << "RilInterfaceTest::SetEmergencyCallListTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetBarringPasswordTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetBarringPasswordTest -->" << endl;
    SetBarringInfo setBarringInfo;
    setBarringInfo.fac = "AB";

    cout << "Please input old password:";
    cin >> setBarringInfo.oldPassword;
    cout << "Your input old password is : " << setBarringInfo.oldPassword << endl;

    cout << "Please input new password:";
    cin >> setBarringInfo.newPassword;
    cout << "Your input new password is : " << setBarringInfo.newPassword << endl;
    int32_t ret = g_rilInterface->SetBarringPassword(slotId, GetSerialId(), setBarringInfo);
    cout << "RilInterfaceTest::SetBarringPasswordTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetFailReasonTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetFailReasonTest -->" << endl;
    int32_t ret = g_rilInterface->GetCallFailReason(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetFailReasonTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::IccRilSimIoForAppTest(int32_t slotId)
{
    cout << "RilInterfaceTest::IccRilSimIoForAppTest -->" << endl;
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
    SimIoRequestInfo msg;
    msg.command = curCommand;
    msg.fileId = fileId;
    msg.p1 = p1;
    msg.p2 = p2;
    msg.p3 = p3;
    msg.data = cmdData;
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(slotId, GetSerialId(), msg);
    cout << "RilInterfaceTest::IccRilSimIoForAppTest --> finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilCmImsiForAppTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilCmImsiForAppTest -->" << endl;
    int32_t ret = g_rilInterface->GetImsi(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetRilCmImsiForAppTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilCmSignalIntensityTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilCmSignalIntensityTest -->" << endl;
    int32_t ret = g_rilInterface->GetSignalStrength(slotId, GetSerialId());
    cout << "GetRilCmSignalIntensityTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilCmIccCardStatusTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilCmIccCardStatusTest -->" << endl;
    int32_t ret = g_rilInterface->GetSimStatus(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetRilCmIccCardStatusTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilCmCsRegStatusTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilCmCsRegStatusTest -->" << endl;
    int32_t ret = g_rilInterface->GetCsRegStatus(slotId, GetSerialId());
    cout << "GetRilCmCsRegStatusTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilCmPsRegStatusTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilCmPsRegStatusTest -->" << endl;
    int32_t ret = g_rilInterface->GetPsRegStatus(slotId, GetSerialId());
    cout << "GetRilCmPsRegStatusTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilCmCellInfoListTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilCmCellInfoListTest -->" << endl;
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetRilCmOperatorTest -->" << endl;
    cout << "GetRilCmCellInfoListTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilCurrentCellInfoTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilCurrentCellInfoTest -->" << endl;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(slotId, GetSerialId());
    cout << "GetRilCurrentCellInfoTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilCmOperatorTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilCmOperatorTest -->" << endl;
    int32_t ret = g_rilInterface->GetOperatorInfo(slotId, GetSerialId());
    cout << "GetRilCmOperatorTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilNetworkSearchInfoTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilNetworkSearchInfoTest -->" << endl;
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(slotId, GetSerialId());
    cout << "GetRilNetworkSearchInfoTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilNetworkSelectionModeTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilNetworkSelectionModeTest -->" << endl;
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(slotId, GetSerialId());
    cout << "GetRilNetworkSelectionModeTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetRilPreferredNetwrokTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRilPreferredNetwrokTest -->" << endl;
    int32_t ret = g_rilInterface->GetPreferredNetwork(slotId, GetSerialId());
    cout << "GetRilPreferredNetwrokTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetRilPreferredNetworkTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetRilPreferredNetworkTest -->" << endl;

    int32_t preferredNetwork;
    cout << "please enter the preferredNetwork:";
    cin >> preferredNetwork;
    cout << preferredNetwork << endl;

    int32_t ret = g_rilInterface->SetPreferredNetwork(slotId, GetSerialId(), preferredNetwork);
    cout << "SetRilPreferredNetworkTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetRilLocationUpdateTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetRilLocationUpdateTest -->" << endl;

    int32_t mode;
    cout << "please enter the mode[0-2]:";
    cin >> mode;
    cout << mode << endl;

    int32_t ret = g_rilInterface->SetLocateUpdates(slotId, GetSerialId(), static_cast<RilRegNotifyMode>(mode));
    cout << "SetRilLocationUpdateTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetRilNotificationFilterTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetRilNotificationFilterTest -->" << endl;

    int32_t filter;
    cout << "please enter the filter:";
    cin >> filter;
    cout << filter << endl;

    int32_t ret = g_rilInterface->SetNotificationFilter(slotId, GetSerialId(), filter);
    cout << "SetRilNotificationFilterTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetRilDeviceStateTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetRilDeviceStateTest -->" << endl;

    int32_t deviceStateType;
    bool deviceStateOn;

    cout << "please enter the deviceStateType:" << endl;
    cin >> deviceStateType;
    cout << deviceStateType << endl;
    cout << "please enter the deviceStateOn:" << endl;
    cin >> deviceStateOn;
    cout << deviceStateOn << endl;

    int32_t ret = g_rilInterface->SetDeviceState(slotId, GetSerialId(), deviceStateType, deviceStateOn);
    cout << "SetRilDeviceStateTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetRilNetworkSelectionModeTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetRilNetworkSelectionModeTest -->" << endl;

    int32_t mode;
    std::string oper;
    int64_t flag;
    cout << "please enter the mode:";
    cin >> mode;
    cout << mode << endl;

    if (mode != 0) {
        cout << "please enter the operator:";
        cin >> oper;
        cout << oper << endl;
    }
    cout << "please enter the flag:";
    cin >> flag;
    cout << flag << endl;
    SetNetworkModeInfo networkModeInfo = {};
    networkModeInfo.selectMode = mode;
    networkModeInfo.oper = oper;
    networkModeInfo.flag = flag;
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(slotId, GetSerialId(), networkModeInfo);
    cout << "SetRilNetworkSelectionModeTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetPhysicalChannelConfigTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetPhysicalChannelConfigTest -->" << endl;
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(slotId, GetSerialId());
    cout << "GetPhysicalChannelConfigTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SendRilCmSmsTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SendRilCmSmsTest -->" << endl;
    string smscPdu;
    string pdu;
    int32_t state;

    cout << "please entry smsc(Pdu):" << endl;
    cin >> smscPdu;
    cout << "please entry sms message(Pdu):" << endl;
    cin >> pdu;
    cout << "please entry sms message state:" << endl;
    cin >> state;

    GsmSmsMessageInfo gsmSmsMessageInfo;
    gsmSmsMessageInfo.serial = GetSerialId();
    gsmSmsMessageInfo.smscPdu = smscPdu;
    gsmSmsMessageInfo.pdu = pdu;
    gsmSmsMessageInfo.state = state;

    int32_t ret = g_rilInterface->SendGsmSms(slotId, GetSerialId(), gsmSmsMessageInfo);
    cout << "RilInterfaceTest::SendRilCmSmsTest --> SendRilCmSmsTest finished ret:" << ret << endl << endl;
}

void RilInterfaceTest::SendRilCmSmsMoreModeTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SendRilCmSmsMoreModeTest -->" << endl;
    string smscPdu;
    string pdu;
    int32_t state;

    cout << "please entry smsc(Pdu):" << endl;
    cin >> smscPdu;
    cout << "please entry sms message(Pdu):" << endl;
    cin >> pdu;
    cout << "please entry sms message state:" << endl;
    cin >> state;

    GsmSmsMessageInfo gsmSmsMessageInfo;
    gsmSmsMessageInfo.serial = GetSerialId();
    gsmSmsMessageInfo.smscPdu = smscPdu;
    gsmSmsMessageInfo.pdu = pdu;
    gsmSmsMessageInfo.state = state;

    int32_t ret = g_rilInterface->SendGsmSms(slotId, GetSerialId(), gsmSmsMessageInfo);
    cout << "RilInterfaceTest::SendRilCmSmsMoreModeTest --> SendRilCmSmsMoreModeTest finished ret:" << ret << endl
         << endl;
}

void RilInterfaceTest::SendSmsAckTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SendSmsAckTest -->" << endl;
    const int32_t memExceededCause = 0xD3;
    bool success;
    int32_t cause;
    string pdu;

    cout << "send sms ack types are as follows:" << endl
         << "\t[0]: report OK" << endl
         << "\t[1]: report ERROR(Memory Capacity Exceeded)" << endl;
    int32_t ackType = InputInt32(0, 1, "ack type");
    if (ackType == 1) {
        success = 0;
        cause = memExceededCause;
    } else {
        success = 1;
        cause = 0;
    }
    cout << "please entry sms message(Pdu):" << endl;
    cin >> pdu;

    ModeData modeData;
    modeData.serial = GetSerialId();
    modeData.result = success;
    modeData.mode = cause;
    modeData.pdu = pdu;

    int32_t ret = g_rilInterface->SendSmsAck(slotId, GetSerialId(), modeData);
    cout << "RilInterfaceTest::SendSmsAckTest --> SendSmsAckTest finished ret:" << ret << endl << endl;
}

void RilInterfaceTest::GetSmscAddrTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetSmscAddrTest -->" << endl;
    int32_t ret = g_rilInterface->GetSmscAddr(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetSmscAddrTest --> GetSmscAddrTest finished ret:" << ret << endl << endl;
}

void RilInterfaceTest::SetSmscAddrTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetSmscAddrTest -->" << endl;
    int32_t tosca;
    string address;
    cout << "please entry tosca:" << endl;
    cin >> tosca;
    cout << "please entry smsc:" << endl;
    cin >> address;

    ServiceCenterAddress serviceCenterAddress;
    serviceCenterAddress.serial = GetSerialId();
    serviceCenterAddress.tosca = tosca;
    serviceCenterAddress.address = address;

    int32_t ret = g_rilInterface->SetSmscAddr(slotId, GetSerialId(), serviceCenterAddress);
    cout << "RilInterfaceTest::SetSmscAddrTest --> SetSmscAddrTest finished ret:" << ret << endl << endl;
}

void RilInterfaceTest::GetImeiTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetImeiTest -->" << endl;
    int32_t ret = g_rilInterface->GetImei(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetImeiTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetImeiStressTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetImeiStressTest -->" << endl;
    for (int32_t i = 0; i < STRESS_TEST_NUM; i++) {
        g_rilInterface->GetImei(slotId, GetSerialId());
    }
    cout << "RilInterfaceTest::GetImeiStressTest --> GetImeiStressTest finished" << endl << endl;
}

void RilInterfaceTest::GetMeidTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetMeidTest -->" << endl;
    int32_t ret = g_rilInterface->GetMeid(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetMeidTestfinish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetVoiceRadioTechnologyTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetVoiceRadioTechnologyTest -->" << endl;
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetVoiceRadioTechnologyTestfinish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetBasebandVersionTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetBasebandVersionTest -->" << endl;
    int32_t ret = g_rilInterface->GetBasebandVersion(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetBasebandVersionTestfinish ret : " << ret << endl << endl;
}

void RilInterfaceTest::ShutDownTest(int32_t slotId)
{
    cout << "RilInterfaceTest::ShutDownTest -->" << endl;
    int32_t ret = g_rilInterface->ShutDown(slotId, GetSerialId());
    cout << "RilInterfaceTest::ShutDownTestfinish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetRadioStateTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetRadioStateTest -->" << endl;
    int32_t fun = InputInt32(0, 0x7fffffff, "fun");
    int32_t ret = g_rilInterface->SetRadioState(slotId, GetSerialId(), fun, 0);
    cout << "RilInterfaceTest::SetRadioStateTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetRadioStateStressTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetRadioStateStressTest -->" << endl;
    for (int32_t i = 0; i < STRESS_TEST_NUM; i++) {
        g_rilInterface->SetRadioState(slotId, GetSerialId(), 0, 0);
        g_rilInterface->SetRadioState(slotId, GetSerialId(), 1, 0);
    }
    cout << "RilInterfaceTest::SetRadioStateStressTest --> SetRadioStateStressTest finished" << endl << endl;
}

void RilInterfaceTest::GetRadioStateTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetRadioStateTest -->" << endl;
    int32_t ret = g_rilInterface->GetRadioState(slotId, GetSerialId());
    cout << "RilInterfaceTest::GetRadioStateTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::ActivatePdpContextTest(int32_t slotId)
{
    cout << "RilInterfaceTest::ActivatePdpContextTest -->" << endl;
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";

    cout << "please enter the apn:";
    cin >> dataProfileInfo.apn;
    cout << "please enter the username:";
    cin >> dataProfileInfo.userName;
    cout << "please enter the password:";
    cin >> dataProfileInfo.password;
    cout << "please enter the protocol:";
    cin >> dataProfileInfo.protocol;

    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = 0;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(slotId, serialId, dataCallInfo);
    cout << "ActivatePdpContextTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetDataProfileInfoTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetDataProfileInfoTest -->" << endl;
    int32_t size = 0;
    cout << "please enter the profile size:";
    cin >> size;
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = size;
    for (int32_t i = 0; i < size; i++) {
        string apn;
        cout << "please enter the apn:";
        cin >> apn;
        string username;

        cout << "please enter the username:";
        cin >> username;

        string dataInfoPwd;
        cout << "please enter the password:";
        cin >> dataInfoPwd;

        string protocol;
        cout << "please enter the protocol:";
        cin >> protocol;
        DataProfileDataInfo dataInfo;
        dataInfo.serial = serialId;
        dataInfo.profileId = i;
        dataInfo.apn = apn;
        dataInfo.protocol = protocol;
        dataInfo.roamingProtocol = "IPV4V6";
        dataInfo.authenticationType = 0;
        dataInfo.userName = username;
        dataInfo.password = dataInfoPwd;
        dataProfilesInfo.profiles.push_back(dataInfo);
    }
    int32_t ret = g_rilInterface->SetDataProfileInfo(slotId, serialId, dataProfilesInfo);
    cout << "SetDataProfileInfoTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetDataPermittedTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetDataPermittedTestTest -->" << endl;
    int32_t enabled = 0;
    cout << "please enter the enabled:";
    cin >> enabled;
    int32_t ret = g_rilInterface->SetDataPermitted(slotId, GetSerialId(), enabled);
    cout << "SetDataPermittedTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::SendDataPerformanceModeTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SendDataPerformanceModeTest -->" << endl;
    DataPerformanceInfo dataPerformanceInfo;
    cout << "please enter the performanceEnable:";
    cin >> dataPerformanceInfo.performanceEnable;
    cout << "please enter the enforce:";
    cin >> dataPerformanceInfo.enforce;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(slotId, GetSerialId(), dataPerformanceInfo);
    cout << "SendDataPerformanceModeTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::SendDataSleepModeTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SendDataSleepModeTest -->" << endl;
    DataSleepInfo dataSleepInfo;
    cout << "please enter the sleepEnable:";
    cin >> dataSleepInfo.sleepEnable;
    int32_t ret = g_rilInterface->SendDataSleepMode(slotId, GetSerialId(), dataSleepInfo);
    cout << "SendDataSleepModeTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::DeactivatePdpContextTest(int32_t slotId)
{
    cout << "RilInterfaceTest::DeactivatePdpContextTest -->" << endl;
    UniInfo uniInfo;
    cout << "please enter the cid:";
    cin >> uniInfo.gsmIndex;
    cout << "please enter the reason:";
    cin >> uniInfo.arg1;
    int32_t ret = g_rilInterface->DeactivatePdpContext(slotId, GetSerialId(), uniInfo);
    cout << "DeactivatePdpContextTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetPdpContextListTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetPdpContextListTest -->" << endl;
    int32_t serialId = GetSerialId();
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    cout << "please enter the cid:";
    cin >> uniInfo.gsmIndex;
    int32_t ret = g_rilInterface->GetPdpContextList(slotId, serialId, uniInfo);
    cout << "GetPdpContextListTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetInitialApnTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetInitialApnTest -->" << endl;
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;

    cout << "please enter the apn:";
    cin >> dataProfileInfo.apn;
    cout << "please enter the username:";
    cin >> dataProfileInfo.userName;
    cout << "please enter the password:";
    cin >> dataProfileInfo.password;
    cout << "please enter the protocol:";
    cin >> dataProfileInfo.protocol;

    int32_t ret = g_rilInterface->SetInitApnInfo(slotId, serialId, dataProfileInfo);
    cout << "SetInitialApnTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetLinkBandwidthInfoTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetLinkBandwidthInfoTest -->" << endl;
    int32_t cid;
    cout << "please enter the cid:";
    cin >> cid;
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(slotId, GetSerialId(), cid);
    cout << "GetLinkBandwidthInfoTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetLinkBandwidthReportingRuleTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetLinkBandwidthReportingRuleTest -->" << endl;
    int32_t serialId = GetSerialId();
    uint32_t uplinkKbpsSize = sizeof(MAX_UPLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    uint32_t downlinkKbpsSize = sizeof(MAX_DOWNLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = RAT_TYPE_LTE;
    dLinkBandwidth.delayMs = BANDWIDTH_HYSTERESIS_MS;
    dLinkBandwidth.delayUplinkKbps = BANDWIDTH_HYSTERESIS_KBPS;
    dLinkBandwidth.delayDownlinkKbps = BANDWIDTH_HYSTERESIS_KBPS;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(MAX_UPLINK_LINK_BANDWIDTH[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(MAX_DOWNLINK_LINK_BANDWIDTH[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(slotId, serialId, dLinkBandwidth);
    cout << "SetLinkBandwidthReportingRuleTest finished ret : " << ret << endl << endl;
}

void RilInterfaceTest::OnProcessInput(int32_t what)
{
    if (g_rilInterface == nullptr) {
        cout << "g_rilInterface is null";
        return;
    }
    auto itFunc = memberFuncMap_.find(what);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            cout << "Please input slotId:";
            int32_t slotId;
            cin >> slotId;
            (this->*memberFunc)(slotId);
            return;
        }
    }

    cout << "----> Unsupported Command!" << endl;
}

void RilInterfaceTest::OnStressInput(int32_t what)
{
    if (g_rilInterface == nullptr) {
        cout << "g_rilInterface is null";
        return;
    }
    auto itFunc = stressMemberFuncMap_.find(what);
    if (itFunc != stressMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            cout << "Please input slotId:";
            int32_t slotId;
            cin >> slotId;
            (this->*memberFunc)(slotId);
            return;
        }
    }
    cout << "----> Unsupported Command!" << endl;
}

void RilInterfaceTest::SimOpenLogicalChannelTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SimOpenLogicalChannelTest -->" << endl;
    std::string appId;
    int32_t p2;
    cout << "input appId:";
    cin >> appId;
    cout << "input p2:";
    cin >> p2;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(slotId, GetSerialId(), appId, p2);
    cout << "RilInterfaceTest::SimOpenLogicalChannelTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SimCloseLogicalChannelTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SimCloseLogicalChannelTest -->" << endl;
    int32_t channelId;
    cout << "input channelId:";
    cin >> channelId;
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(slotId, GetSerialId(), channelId);
    cout << "RilInterfaceTest::SimCloseLogicalChannelTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::InputTransmitApduChannelParam(ApduSimIORequestInfo &reqInfo)
{
    cout << "input channelId:";
    cin >> reqInfo.channelId;
    cout << "input type:";
    cin >> reqInfo.type;
    cout << "input instruction:";
    cin >> reqInfo.instruction;
    cout << "input p1:";
    cin >> reqInfo.p1;
    cout << "input p2:";
    cin >> reqInfo.p2;
    cout << "input p3:";
    cin >> reqInfo.p3;
}

void RilInterfaceTest::SimTransmitApduLogicalChannelTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SimTransmitApduLogicalChannelTest -->" << endl;
    ApduSimIORequestInfo reqInfo = ApduSimIORequestInfo();
    InputTransmitApduChannelParam(reqInfo);
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(slotId, GetSerialId(), reqInfo);
    cout << "RilInterfaceTest::SimTransmitApduLogicalChannelTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SimTransmitApduBasicChannelTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SimTransmitApduBasicChannelTest -->" << endl;
    ApduSimIORequestInfo reqInfo = ApduSimIORequestInfo();
    InputTransmitApduChannelParam(reqInfo);
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(slotId, GetSerialId(), reqInfo);
    cout << "RilInterfaceTest::SimTransmitApduBasicChannelTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SimAuthenticationTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SimAuthenticationTest -->" << endl;
    SimAuthenticationRequestInfo reqInfo = SimAuthenticationRequestInfo();
    cout << "input aid:";
    cin >> reqInfo.aid;
    cout << "input authData:";
    cin >> reqInfo.authData;
    int32_t ret = g_rilInterface->SimAuthentication(slotId, GetSerialId(), reqInfo);
    cout << "RilInterfaceTest::SimAuthenticationTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetActiveSimTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetActiveSimTest -->" << endl;
    int32_t index;
    cout << "please enter the index:";
    cin >> index;
    int32_t enable;
    cout << "please enter the enable:";
    cin >> enable;
    int32_t ret = g_rilInterface->SetActiveSim(slotId, GetSerialId(), index, enable);
    cout << "RilInterfaceTest::SetActiveSimTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::UnlockSimLockTest(int32_t slotId)
{
    cout << "RilInterfaceTest::UnlockSimLockTest -->" << endl;
    int lockType;
    cout << "input lockType:";
    cin >> lockType;
    string password = "";
    cout << "input password:";
    cin >> password;
    int32_t ret = g_rilInterface->UnlockSimLock(slotId, GetSerialId(), lockType, password);
    cout << "RilInterfaceTest::UnlockSimLockTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::UnLockPINTest(int32_t slotId)
{
    cout << "RilInterfaceTest::UnLockPINTest -->" << endl;
    std::string pin;
    cout << "please enter the pin:";
    cin >> pin;
    int32_t ret = g_rilInterface->UnlockPin(slotId, GetSerialId(), pin);
    cout << "RilInterfaceTest::UnLockPINTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::UnLockPIN2Test(int32_t slotId)
{
    cout << "RilInterfaceTest::UnLockPIN2Test -->" << endl;
    std::string pin2;
    cout << "please enter the pin2:";
    cin >> pin2;
    int32_t ret = g_rilInterface->UnlockPin2(slotId, GetSerialId(), pin2);
    cout << "RilInterfaceTest::UnLockPIN2Test finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::UnLockPUKTest(int32_t slotId)
{
    cout << "RilInterfaceTest::UnLockPUKTest -->" << endl;
    std::string puk;
    cout << "please enter the puk:";
    cin >> puk;
    std::string pin;
    cout << "please enter the pin:";
    cin >> pin;
    int32_t ret = g_rilInterface->UnlockPuk(slotId, GetSerialId(), puk, pin);
    cout << "RilInterfaceTest::UnLockPUKTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::UnLockPUK2Test(int32_t slotId)
{
    cout << "RilInterfaceTest::UnLockPUK2Test -->" << endl;
    std::string puk2;
    cout << "please enter the puk2:";
    cin >> puk2;
    std::string pin2;
    cout << "please enter the pin2:";
    cin >> pin2;
    int32_t ret = g_rilInterface->UnlockPuk2(slotId, GetSerialId(), puk2, pin2);
    cout << "RilInterfaceTest::UnLockPUK2Test finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::ChangeSimPasswordTest(int32_t slotId)
{
    cout << "RilInterfaceTest::ChangeSimPasswordTest -->" << endl;
    std::string fac;
    cout << "please enter the fac, SC for pin1, FD for pin2:";
    cin >> fac;
    std::string oldPassword;
    cout << "please enter the oldPassword:";
    cin >> oldPassword;
    std::string newPassword;
    cout << "please enter the newPassword:";
    cin >> newPassword;
    SimPasswordInfo simPassword;
    simPassword.fac = fac;
    simPassword.oldPassword = oldPassword;
    simPassword.newPassword = newPassword;
    simPassword.passwordLength = newPassword.length();
    int32_t ret = g_rilInterface->ChangeSimPassword(slotId, GetSerialId(), simPassword);
    cout << "RilInterfaceTest::ChangeSimPasswordTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SetSimLockTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SetSimLockTest -->" << endl;
    std::string fac;
    cout << "please enter the fac, SC for pin1, FD for pin2:";
    cin >> fac;
    int32_t mode;
    cout << "please enter the mode:";
    cin >> mode;
    std::string password;
    cout << "please enter the password:";
    cin >> password;
    SimLockInfo simLockInfo;
    simLockInfo.fac = fac;
    simLockInfo.mode = mode;
    simLockInfo.classx = 0;
    simLockInfo.passwd = password;
    int32_t ret = g_rilInterface->SetSimLock(slotId, GetSerialId(), simLockInfo);
    cout << "RilInterfaceTest::SetSimLockTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::GetSimLockStatusTest(int32_t slotId)
{
    cout << "RilInterfaceTest::GetSimLockStatusTest -->" << endl;
    std::string fac;
    cout << "please enter the fac, SC for pin1, FD for pin2:";
    cin >> fac;
    SimLockInfo simLockInfo;
    simLockInfo.fac = fac;
    simLockInfo.mode = SIM_LOCK_MODE_QUERY;
    simLockInfo.classx = 0;
    int32_t ret = g_rilInterface->GetSimLockStatus(slotId, GetSerialId(), simLockInfo);
    cout << "RilInterfaceTest::GetSimLockStatusTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SendTerminalResponseCmdTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SendTerminalResponseCmdTest -->" << endl;
    std::string cmd;
    cout << "please enter terminal response command:";
    cin >> cmd;
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(slotId, GetSerialId(), cmd);
    cout << "RilInterfaceTest::SendTerminalResponseCmdTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SendEnvelopeCmdTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SendEnvelopeCmdTest -->" << endl;
    std::string cmd;
    cout << "please enter envelope command:";
    cin >> cmd;
    int32_t ret = g_rilInterface->SimStkSendEnvelope(slotId, GetSerialId(), cmd);
    cout << "RilInterfaceTest::SendEnvelopeCmdTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SendCallSetupRequestResultTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SendCallSetupRequestResultTest -->" << endl;
    int32_t accept;
    cout << "please enter call setup request result:";
    cin >> accept;
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(slotId, GetSerialId(), accept);
    cout << "RilInterfaceTest::SendCallSetupRequestResultTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::SimStkIsReadyTest(int32_t slotId)
{
    cout << "RilInterfaceTest::SimStkIsReadyTest -->" << endl;
    int32_t ret = g_rilInterface->SimStkIsReady(slotId, GetSerialId());
    cout << "RilInterfaceTest::SimStkIsReadyTest finish ret : " << ret << endl << endl;
}

void RilInterfaceTest::OnInitStressInterface()
{
    stressMemberFuncMap_[HREQ_CALL_DIAL] = &RilInterfaceTest::RilCmDialStressTest;
    stressMemberFuncMap_[HREQ_MODEM_GET_IMEI] = &RilInterfaceTest::GetImeiStressTest;
    stressMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &RilInterfaceTest::SetRadioStateStressTest;
}

void RilInterfaceTest::OnInitCallProcessInterface()
{
    // // call
    memberFuncMap_[HREQ_CALL_GET_CALL_LIST] = &RilInterfaceTest::GetCallListTest;
    memberFuncMap_[HREQ_CALL_DIAL] = &RilInterfaceTest::RilCmDialTest;
    memberFuncMap_[HREQ_CALL_HANGUP] = &RilInterfaceTest::HangupTest;
    memberFuncMap_[HREQ_CALL_REJECT] = &RilInterfaceTest::RejectTest;
    memberFuncMap_[HREQ_CALL_ANSWER] = &RilInterfaceTest::AnswerCallTest;
    memberFuncMap_[HREQ_CALL_HOLD_CALL] = &RilInterfaceTest::HoldCallTest;
    memberFuncMap_[HREQ_CALL_UNHOLD_CALL] = &RilInterfaceTest::UnHoldCallTest;
    memberFuncMap_[HREQ_CALL_SWITCH_CALL] = &RilInterfaceTest::SwitchCallTest;

    memberFuncMap_[HREQ_CALL_COMBINE_CONFERENCE] = &RilInterfaceTest::RilCmJoinCallTest;
    memberFuncMap_[HREQ_CALL_SEPARATE_CONFERENCE] = &RilInterfaceTest::RilCmSplitCallTest;

    memberFuncMap_[HREQ_CALL_CALL_SUPPLEMENT] = &RilInterfaceTest::CallSupplementTest;
    memberFuncMap_[HREQ_CALL_SEND_DTMF] = &RilInterfaceTest::SendDtmfTest;
    memberFuncMap_[HREQ_CALL_START_DTMF] = &RilInterfaceTest::StartDtmfTest;
    memberFuncMap_[HREQ_CALL_STOP_DTMF] = &RilInterfaceTest::StopDtmfTest;

    memberFuncMap_[HREQ_CALL_SET_USSD] = &RilInterfaceTest::SetUssdCusdTest;
    memberFuncMap_[HREQ_CALL_GET_USSD] = &RilInterfaceTest::GetUssdCusdTest;

    memberFuncMap_[HREQ_CALL_SET_MUTE] = &RilInterfaceTest::SetMuteTest;
    memberFuncMap_[HREQ_CALL_GET_MUTE] = &RilInterfaceTest::GetMuteTest;
    memberFuncMap_[HREQ_CALL_GET_EMERGENCY_LIST] = &RilInterfaceTest::GetEmergencyListTest;
    memberFuncMap_[HREQ_CALL_SET_EMERGENCY_LIST] = &RilInterfaceTest::SetEmergencyCallListTest;
    memberFuncMap_[HREQ_CALL_GET_FAIL_REASON] = &RilInterfaceTest::GetFailReasonTest;
    memberFuncMap_[HREQ_CALL_SET_BARRING_PASSWORD] = &RilInterfaceTest::SetBarringPasswordTest;
}

void RilInterfaceTest::OnInitSmsProcessInterface()
{
    // sms
    memberFuncMap_[HREQ_SMS_SEND_GSM_SMS] = &RilInterfaceTest::SendRilCmSmsTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_MORE_MODE] = &RilInterfaceTest::SendRilCmSmsMoreModeTest;
    memberFuncMap_[HREQ_SMS_SEND_SMS_ACK] = &RilInterfaceTest::SendSmsAckTest;
    memberFuncMap_[HREQ_SMS_SET_SMSC_ADDR] = &RilInterfaceTest::SetSmscAddrTest;
    memberFuncMap_[HREQ_SMS_GET_SMSC_ADDR] = &RilInterfaceTest::GetSmscAddrTest;
}

void RilInterfaceTest::OnInitSimProcessInterface()
{
    // sim
    memberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &RilInterfaceTest::GetRilCmIccCardStatusTest;
    memberFuncMap_[HREQ_SIM_GET_IMSI] = &RilInterfaceTest::GetRilCmImsiForAppTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_IO] = &RilInterfaceTest::IccRilSimIoForAppTest;
    memberFuncMap_[HREQ_SIM_OPEN_LOGICAL_CHANNEL] = &RilInterfaceTest::SimOpenLogicalChannelTest;
    memberFuncMap_[HREQ_SIM_CLOSE_LOGICAL_CHANNEL] = &RilInterfaceTest::SimCloseLogicalChannelTest;
    memberFuncMap_[HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL] = &RilInterfaceTest::SimTransmitApduLogicalChannelTest;
    memberFuncMap_[HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL] = &RilInterfaceTest::SimTransmitApduBasicChannelTest;
    memberFuncMap_[HREQ_SIM_AUTHENTICATION] = &RilInterfaceTest::SimAuthenticationTest;
    memberFuncMap_[HREQ_SIM_SET_ACTIVE_SIM] = &RilInterfaceTest::SetActiveSimTest;
    memberFuncMap_[HREQ_SIM_UNLOCK_SIM_LOCK] = &RilInterfaceTest::UnlockSimLockTest;
    memberFuncMap_[HREQ_SIM_UNLOCK_PIN] = &RilInterfaceTest::UnLockPINTest;
    memberFuncMap_[HREQ_SIM_UNLOCK_PIN2] = &RilInterfaceTest::UnLockPIN2Test;
    memberFuncMap_[HREQ_SIM_UNLOCK_PUK] = &RilInterfaceTest::UnLockPUKTest;
    memberFuncMap_[HREQ_SIM_UNLOCK_PUK2] = &RilInterfaceTest::UnLockPUK2Test;
    memberFuncMap_[HREQ_SIM_CHANGE_SIM_PASSWORD] = &RilInterfaceTest::ChangeSimPasswordTest;
    memberFuncMap_[HREQ_SIM_SET_SIM_LOCK] = &RilInterfaceTest::SetSimLockTest;
    memberFuncMap_[HREQ_SIM_GET_SIM_LOCK_STATUS] = &RilInterfaceTest::GetSimLockStatusTest;
    memberFuncMap_[HREQ_SIM_STK_SEND_TERMINAL_RESPONSE] = &RilInterfaceTest::SendTerminalResponseCmdTest;
    memberFuncMap_[HREQ_SIM_STK_SEND_ENVELOPE] = &RilInterfaceTest::SendEnvelopeCmdTest;
    memberFuncMap_[HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT] = &RilInterfaceTest::SendCallSetupRequestResultTest;
    memberFuncMap_[HREQ_SIM_STK_IS_READY] = &RilInterfaceTest::SimStkIsReadyTest;
}

void RilInterfaceTest::OnInitDataProcessInterface()
{
    // data
    memberFuncMap_[HREQ_DATA_SET_INIT_APN_INFO] = &RilInterfaceTest::SetInitialApnTest;
    memberFuncMap_[HREQ_DATA_DEACTIVATE_PDP_CONTEXT] = &RilInterfaceTest::DeactivatePdpContextTest;
    memberFuncMap_[HREQ_DATA_ACTIVATE_PDP_CONTEXT] = &RilInterfaceTest::ActivatePdpContextTest;
    memberFuncMap_[HREQ_DATA_GET_PDP_CONTEXT_LIST] = &RilInterfaceTest::GetPdpContextListTest;
    memberFuncMap_[HREQ_DATA_GET_LINK_BANDWIDTH_INFO] = &RilInterfaceTest::GetLinkBandwidthInfoTest;
    memberFuncMap_[HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE] = &RilInterfaceTest::SetLinkBandwidthReportingRuleTest;
    memberFuncMap_[HREQ_DATA_SET_DATA_PROFILE_INFO] = &RilInterfaceTest::SetDataProfileInfoTest;
    memberFuncMap_[HREQ_DATA_SEND_DATA_PERFORMANCE_MODE] = &RilInterfaceTest::SendDataPerformanceModeTest;
    memberFuncMap_[HREQ_DATA_SEND_DATA_SLEEP_MODE] = &RilInterfaceTest::SendDataSleepModeTest;
    memberFuncMap_[HREQ_DATA_SET_DATA_PERMITTED] = &RilInterfaceTest::SetDataPermittedTest;
}

void RilInterfaceTest::OnInitNetworkProcessInterface()
{
    // network
    memberFuncMap_[HREQ_NETWORK_GET_SIGNAL_STRENGTH] = &RilInterfaceTest::GetRilCmSignalIntensityTest;
    memberFuncMap_[HREQ_NETWORK_GET_CS_REG_STATUS] = &RilInterfaceTest::GetRilCmCsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_PS_REG_STATUS] = &RilInterfaceTest::GetRilCmPsRegStatusTest;
    memberFuncMap_[HREQ_NETWORK_GET_OPERATOR_INFO] = &RilInterfaceTest::GetRilCmOperatorTest;
    memberFuncMap_[HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION] = &RilInterfaceTest::GetRilNetworkSearchInfoTest;
    memberFuncMap_[HREQ_NETWORK_GET_NETWORK_SELECTION_MODE] = &RilInterfaceTest::GetRilNetworkSelectionModeTest;
    memberFuncMap_[HREQ_NETWORK_SET_NETWORK_SELECTION_MODE] = &RilInterfaceTest::SetRilNetworkSelectionModeTest;
    memberFuncMap_[HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST] = &RilInterfaceTest::GetRilCmCellInfoListTest;
    memberFuncMap_[HREQ_NETWORK_GET_CURRENT_CELL_INFO] = &RilInterfaceTest::GetRilCurrentCellInfoTest;
    memberFuncMap_[HREQ_NETWORK_SET_PREFERRED_NETWORK] = &RilInterfaceTest::SetRilPreferredNetworkTest;
    memberFuncMap_[HREQ_NETWORK_GET_PREFERRED_NETWORK] = &RilInterfaceTest::GetRilPreferredNetwrokTest;
    memberFuncMap_[HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG] = &RilInterfaceTest::GetPhysicalChannelConfigTest;
    memberFuncMap_[HREQ_NETWORK_SET_LOCATE_UPDATES] = &RilInterfaceTest::SetRilLocationUpdateTest;
    memberFuncMap_[HREQ_NETWORK_SET_NOTIFICATION_FILTER] = &RilInterfaceTest::SetRilNotificationFilterTest;
    memberFuncMap_[HREQ_NETWORK_SET_DEVICE_STATE] = &RilInterfaceTest::SetRilDeviceStateTest;
}

void RilInterfaceTest::OnInitModemProcessInterface()
{
    memberFuncMap_[HREQ_MODEM_SHUT_DOWN] = &RilInterfaceTest::ShutDownTest;
    memberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &RilInterfaceTest::SetRadioStateTest;
    memberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &RilInterfaceTest::GetRadioStateTest;
    memberFuncMap_[HREQ_MODEM_GET_IMEI] = &RilInterfaceTest::GetImeiTest;
    memberFuncMap_[HREQ_MODEM_GET_MEID] = &RilInterfaceTest::GetMeidTest;
    memberFuncMap_[HREQ_MODEM_GET_VOICE_RADIO] = &RilInterfaceTest::GetVoiceRadioTechnologyTest;
    memberFuncMap_[HREQ_MODEM_GET_BASEBAND_VERSION] = &RilInterfaceTest::GetBasebandVersionTest;
}

void RilInterfaceTest::OnInitProcessInterface()
{
    OnInitCallProcessInterface();
    OnInitSmsProcessInterface();
    OnInitSimProcessInterface();
    OnInitDataProcessInterface();
    OnInitNetworkProcessInterface();
    OnInitModemProcessInterface();
}

void RilInterfaceTest::OnInitInterface()
{
    OnInitStressInterface();
    OnInitProcessInterface();
}
} // namespace Telephony
} // namespace OHOS

using namespace OHOS;
using namespace OHOS::Telephony;

static int32_t PrintMenu()
{
    cout << endl << "---->Test Module:" << endl;
    cout << "----> 0. EXIT --------------------------" << endl;
    cout << "----> 1. MODEM -------------------------" << endl;
    cout << "----> 2. CALL --------------------------" << endl;
    cout << "----> 3. NETWORK -----------------------" << endl;
    cout << "----> 4. SIM ---------------------------" << endl;
    cout << "----> 5. DATA --------------------------" << endl;
    cout << "----> 6. SMS ---------------------------" << endl;
    cout << "----> 7. STRESS ------------------------" << endl;
    int32_t choice = InputInt32(0, static_cast<int32_t>(TestMenu::STRESS), "Test Module Index");
    cout << "---->You choose: " << choice << endl;
    return choice;
}

static int32_t PrintStressMenu()
{
    cout << "---->[MODULE]STRESS:" << endl;
    cout << "----> [" << 0 << "] ---->Back to the previous menu." << endl;
    cout << "----> [" << HREQ_CALL_DIAL << "] ---->[ HREQ_CALL_DIAL ]" << endl;
    cout << "----> [" << HREQ_MODEM_SET_RADIO_STATUS << "] ---->[ HREQ_MODEM_SET_RADIO_STATUS ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_IMEI << "] ---->[ HREQ_MODEM_GET_IMEI ]" << endl;

    int32_t choice = InputInt32(HREQ_CALL_BASE, HREQ_MODEM_EXIT - 1, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_CALL_BASE) ? DEFAULT_CHOICE : choice;
    return choice;
}

static int32_t PrintCallMenu()
{
    cout << "---->[MODULE]CALL:" << endl;
    cout << "----> [" << HREQ_CALL_BASE << "] ---->Back to the previous menu." << endl;
    cout << "----> [" << HREQ_CALL_GET_CALL_LIST << "] ---->[ HREQ_CALL_GET_CALL_LIST ]" << endl;
    cout << "----> [" << HREQ_CALL_DIAL << "] ---->[ HREQ_CALL_DIAL ]" << endl;
    cout << "----> [" << HREQ_CALL_HANGUP << "] ---->[ HREQ_CALL_HANGUP ]" << endl;
    cout << "----> [" << HREQ_CALL_REJECT << "] ---->[ HREQ_CALL_REJECT ]" << endl;
    cout << "----> [" << HREQ_CALL_ANSWER << "] ---->[ HREQ_CALL_ANSWER ]" << endl;
    cout << "----> [" << HREQ_CALL_HOLD_CALL << "] ---->[ HREQ_CALL_HOLD_CALL ]" << endl;
    cout << "----> [" << HREQ_CALL_UNHOLD_CALL << "] ---->[ HREQ_CALL_UNHOLD_CALL ]" << endl;
    cout << "----> [" << HREQ_CALL_SWITCH_CALL << "] ---->[ HREQ_CALL_SWITCH_CALL ]" << endl;
    cout << "----> [" << HREQ_CALL_COMBINE_CONFERENCE << "] ---->[ HREQ_CALL_COMBINE_CONFERENCE ]" << endl;
    cout << "----> [" << HREQ_CALL_SEPARATE_CONFERENCE << "] ---->[ HREQ_CALL_SEPARATE_CONFERENCE ]" << endl;
    cout << "----> [" << HREQ_CALL_CALL_SUPPLEMENT << "] ---->[ HREQ_CALL_CALL_SUPPLEMENT ]" << endl;
    cout << "----> [" << HREQ_CALL_SEND_DTMF << "] ---->[ HREQ_CALL_SEND_DTMF ]" << endl;
    cout << "----> [" << HREQ_CALL_START_DTMF << "] ---->[ HREQ_CALL_START_DTMF ]" << endl;
    cout << "----> [" << HREQ_CALL_STOP_DTMF << "] ---->[ HREQ_CALL_STOP_DTMF ]" << endl;
    cout << "----> [" << HREQ_CALL_SET_USSD << "] ---->[ HREQ_CALL_SET_USSD ]" << endl;
    cout << "----> [" << HREQ_CALL_GET_USSD << "] ---->[ HREQ_CALL_GET_USSD ]" << endl;
    cout << "----> [" << HREQ_CALL_SET_MUTE << "] ---->[ HREQ_CALL_SET_MUTE ]" << endl;
    cout << "----> [" << HREQ_CALL_GET_MUTE << "] ---->[ HREQ_CALL_GET_MUTE ]" << endl;
    cout << "----> [" << HREQ_CALL_GET_EMERGENCY_LIST << "] ---->[ HREQ_CALL_GET_EMERGENCY_LIST ]" << endl;
    cout << "----> [" << HREQ_CALL_SET_EMERGENCY_LIST << "] ---->[ HREQ_CALL_SET_EMERGENCY_LIST ]" << endl;
    cout << "----> [" << HREQ_CALL_GET_FAIL_REASON << "] ---->[ HREQ_CALL_GET_FAIL_REASON ]" << endl;
    cout << "----> [" << HREQ_CALL_SET_BARRING_PASSWORD << "] ---->[ HREQ_CALL_SET_BARRING_PASSWORD ]" << endl;

    int32_t choice = InputInt32(HREQ_CALL_BASE, HREQ_SMS_BASE - MENU_OFFSET, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_CALL_BASE) ? DEFAULT_CHOICE : choice;
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

    int32_t choice = InputInt32(HREQ_SMS_BASE, HREQ_SIM_BASE - MENU_OFFSET, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_SMS_BASE) ? DEFAULT_CHOICE : choice;
    return choice;
}

static int32_t PrintSimMenu()
{
    cout << "---->[MODULE]SIM:" << endl;
    cout << "----> [" << HREQ_SIM_BASE << "] ---->Back to the previous menu." << endl;
    cout << "----> [" << HREQ_SIM_GET_SIM_STATUS << "] ---->[ HREQ_SIM_GET_SIM_STATUS ]" << endl;
    cout << "----> [" << HREQ_SIM_GET_IMSI << "] ---->[ HREQ_SIM_GET_IMSI ]" << endl;
    cout << "----> [" << HREQ_SIM_GET_SIM_IO << "] ---->[ HREQ_SIM_GET_SIM_IO ]" << endl;
    cout << "----> [" << HREQ_SIM_SET_ACTIVE_SIM << "] ---->[ HREQ_SIM_SET_ACTIVE_SIM ]" << endl;
    cout << "----> [" << HREQ_SIM_OPEN_LOGICAL_CHANNEL << "] ---->[ HREQ_SIM_OPEN_LOGICAL_CHANNEL ]" << endl;
    cout << "----> [" << HREQ_SIM_CLOSE_LOGICAL_CHANNEL << "] ---->[ HREQ_SIM_CLOSE_LOGICAL_CHANNEL ]" << endl;
    cout << "----> [" << HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL << "] ---->[ HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL ]"
         << endl;
    cout << "----> [" << HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL << "] ---->[ HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL ]"
         << endl;
    cout << "----> [" << HREQ_SIM_AUTHENTICATION << "] ---->[ HREQ_SIM_AUTHENTICATION ]" << endl;
    cout << "----> [" << HREQ_SIM_UNLOCK_SIM_LOCK << "] ---->[ HREQ_SIM_UNLOCK_SIM_LOCK ]" << endl;
    cout << "----> [" << HREQ_SIM_UNLOCK_PIN << "] ---->[ HREQ_SIM_UNLOCK_PIN ]" << endl;
    cout << "----> [" << HREQ_SIM_UNLOCK_PIN2 << "] ---->[ HREQ_SIM_UNLOCK_PIN2 ]" << endl;
    cout << "----> [" << HREQ_SIM_UNLOCK_PUK << "] ---->[ HREQ_SIM_UNLOCK_PUK ]" << endl;
    cout << "----> [" << HREQ_SIM_UNLOCK_PUK2 << "] ---->[ HREQ_SIM_UNLOCK_PUK2 ]" << endl;
    cout << "----> [" << HREQ_SIM_CHANGE_SIM_PASSWORD << "] ---->[ HREQ_SIM_CHANGE_SIM_PASSWORD ]" << endl;
    cout << "----> [" << HREQ_SIM_SET_SIM_LOCK << "] ---->[ HREQ_SIM_SET_SIM_LOCK ]" << endl;
    cout << "----> [" << HREQ_SIM_GET_SIM_LOCK_STATUS << "] ---->[ HREQ_SIM_GET_SIM_LOCK_STATUS ]" << endl;
    cout << "----> [" << HREQ_SIM_STK_SEND_TERMINAL_RESPONSE << "] ---->[ HREQ_SIM_STK_SEND_TERMINAL_RESPONSE ]"
         << endl;
    cout << "----> [" << HREQ_SIM_STK_SEND_ENVELOPE << "] ---->[ HREQ_SIM_STK_SEND_ENVELOPE ]" << endl;
    cout << "----> [" << HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT
         << "] ---->[ HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT ]" << endl;
    cout << "----> [" << HREQ_SIM_STK_IS_READY << "] ---->[ HREQ_SIM_STK_IS_READY ]" << endl;

    int32_t choice = InputInt32(HREQ_SIM_BASE, HREQ_DATA_BASE - MENU_OFFSET, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_SIM_BASE) ? DEFAULT_CHOICE : choice;
    return choice;
}

static int32_t PrintDataMenu()
{
    cout << "---->[MODULE]DATA:" << endl;
    cout << "----> [" << HREQ_DATA_BASE << "] ---->Back to the previous menu." << endl;
    cout << "----> [" << HREQ_DATA_SET_INIT_APN_INFO << "] ---->[ HREQ_DATA_SET_INIT_APN_INFO ]" << endl;
    cout << "----> [" << HREQ_DATA_DEACTIVATE_PDP_CONTEXT << "] ---->[ HREQ_DATA_DEACTIVATE_PDP_CONTEXT ]" << endl;
    cout << "----> [" << HREQ_DATA_ACTIVATE_PDP_CONTEXT << "] ---->[ HREQ_DATA_ACTIVATE_PDP_CONTEXT ]" << endl;
    cout << "----> [" << HREQ_DATA_GET_PDP_CONTEXT_LIST << "] ---->[ HREQ_DATA_GET_PDP_CONTEXT_LIST ]" << endl;
    cout << "----> [" << HREQ_DATA_GET_LINK_BANDWIDTH_INFO << "] ---->[ HREQ_DATA_GET_LINK_BANDWIDTH_INFO ]" << endl;
    cout << "----> [" << HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE
         << "] ---->[ HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE ]" << endl;
    cout << "----> [" << HREQ_DATA_SET_DATA_PROFILE_INFO << "] ---->[ HREQ_DATA_SET_DATA_PROFILE_INFO ]" << endl;
    cout << "----> [" << HREQ_DATA_SEND_DATA_PERFORMANCE_MODE << "] ---->[ HREQ_DATA_SEND_DATA_PERFORMANCE_MODE ]"
         << endl;
    cout << "----> [" << HREQ_DATA_SEND_DATA_SLEEP_MODE << "] ---->[ HREQ_DATA_SEND_DATA_SLEEP_MODE ]" << endl;
    cout << "----> [" << HREQ_DATA_SET_DATA_PERMITTED << "] ---->[ HREQ_DATA_SET_DATA_PERMITTED ]" << endl;

    int32_t choice = InputInt32(HREQ_DATA_BASE, HREQ_NETWORK_BASE - MENU_OFFSET, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_DATA_BASE) ? DEFAULT_CHOICE : choice;
    return choice;
}

static int32_t PrintNetworkMenu()
{
    cout << "---->[MODULE]NETWORK:" << endl;
    cout << "----> [" << HREQ_NETWORK_BASE << "] ---->Back to the previous menu." << endl;
    cout << "----> [" << HREQ_NETWORK_GET_SIGNAL_STRENGTH << "] ---->[ HREQ_NETWORK_GET_SIGNAL_STRENGTH ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_CS_REG_STATUS << "] ---->[ HREQ_NETWORK_GET_CS_REG_STATUS ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_PS_REG_STATUS << "] ---->[ HREQ_NETWORK_GET_PS_REG_STATUS ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_OPERATOR_INFO << "] ---->[ HREQ_NETWORK_GET_OPERATOR_INFO ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION
         << "] ---->[ HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_NETWORK_SELECTION_MODE << "] ---->[ HREQ_NETWORK_GET_NETWORK_SELECTION_MODE ]"
         << endl;
    cout << "----> [" << HREQ_NETWORK_SET_NETWORK_SELECTION_MODE << "] ---->[ HREQ_NETWORK_SET_NETWORK_SELECTION_MODE ]"
         << endl;
    cout << "----> [" << HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST
         << "] ---->[ HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_CURRENT_CELL_INFO << "] ---->[ HREQ_NETWORK_GET_CURRENT_CELL_INFO ]" << endl;
    cout << "----> [" << HREQ_NETWORK_SET_PREFERRED_NETWORK << "] ---->[ HREQ_NETWORK_SET_PREFERRED_NETWORK ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_PREFERRED_NETWORK << "] ---->[ HREQ_NETWORK_GET_PREFERRED_NETWORK ]" << endl;
    cout << "----> [" << HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG
         << "] ---->[ HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG ]" << endl;
    cout << "----> [" << HREQ_NETWORK_SET_LOCATE_UPDATES << "] ---->[ HREQ_NETWORK_SET_LOCATE_UPDATES ]" << endl;
    cout << "----> [" << HREQ_NETWORK_SET_NOTIFICATION_FILTER << "] ---->[ HREQ_NETWORK_SET_NOTIFICATION_FILTER ]"
         << endl;
    cout << "----> [" << HREQ_NETWORK_SET_DEVICE_STATE << "] ---->[ HREQ_NETWORK_SET_DEVICE_STATE ]" << endl;

    int32_t choice = InputInt32(HREQ_NETWORK_BASE, HREQ_COMMON_BASE - MENU_OFFSET, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_NETWORK_BASE) ? DEFAULT_CHOICE : choice;
    return choice;
}

static int32_t PrintModemMenu()
{
    cout << "---->[MODULE]MODEM:" << endl;
    cout << "----> [" << HREQ_COMMON_BASE << "] ---->Back to the previous menu." << endl;
    cout << "----> [" << HREQ_MODEM_SHUT_DOWN << "] ---->[ HREQ_MODEM_SHUT_DOWN ]" << endl;
    cout << "----> [" << HREQ_MODEM_SET_RADIO_STATUS << "] ---->[ HREQ_MODEM_SET_RADIO_STATUS ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_RADIO_STATUS << "] ---->[ HREQ_MODEM_GET_RADIO_STATUS ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_IMEI << "] ---->[ HREQ_MODEM_GET_IMEI ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_MEID << "] ---->[ HREQ_MODEM_GET_MEID ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_VOICE_RADIO << "] ---->[ HREQ_MODEM_GET_VOICE_RADIO ]" << endl;
    cout << "----> [" << HREQ_MODEM_GET_BASEBAND_VERSION << "] ---->[ HREQ_MODEM_GET_BASEBAND_VERSION ]" << endl;

    int32_t choice = InputInt32(HREQ_COMMON_BASE, HREQ_MODEM_EXIT - MENU_OFFSET, "Command");
    cout << "---->You choose: " << choice << endl;
    choice = (choice == HREQ_COMMON_BASE) ? DEFAULT_CHOICE : choice;
    return choice;
}

static int32_t SwitchMenu(TestMenu module, bool *loopFlag)
{
    int32_t mWhat = static_cast<int32_t>(TestMenu::NONE);
    switch (module) {
        case TestMenu::EXIT:
            if (loopFlag != nullptr) {
                *loopFlag = false;
            }
            break;
        case TestMenu::MODEM:
            mWhat = PrintModemMenu();
            break;
        case TestMenu::CALL:
            mWhat = PrintCallMenu();
            break;
        case TestMenu::NETWORK:
            mWhat = PrintNetworkMenu();
            break;
        case TestMenu::SIM:
            mWhat = PrintSimMenu();
            break;
        case TestMenu::DATA:
            mWhat = PrintDataMenu();
            break;
        case TestMenu::SMS:
            mWhat = PrintSmsMenu();
            break;
        case TestMenu::STRESS:
            mWhat = PrintStressMenu();
        default:
            break;
    }
    return mWhat;
}

int32_t main()
{
    cout << "---->Ril Adapter Test Enter" << endl;
    g_rilInterface = IRil::Get();
    if (g_rilInterface == nullptr) {
        cout << "g_rilInterface is null" << endl;
        return 0;
    }
    RilCallbackTest callback_;
    g_rilInterface->SetCallback(&callback_);
    RilInterfaceTest rilInterfaceTest;
    rilInterfaceTest.OnInitInterface();
    bool loopFlag = true;
    while (loopFlag) {
        int32_t mWhat = static_cast<int32_t>(TestMenu::NONE);
        int32_t module = static_cast<int32_t>(TestMenu::NONE);
        module = PrintMenu();
        mWhat = SwitchMenu(static_cast<TestMenu>(module), &loopFlag);
        if (mWhat <= 0) {
            continue;
        }
        if (module == static_cast<int32_t>(TestMenu::STRESS)) {
            rilInterfaceTest.OnStressInput(mWhat);
        } else {
            rilInterfaceTest.OnProcessInput(mWhat);
        }
        usleep(WAIT_TIME);
    }
    cout << "---->Ril Adapter Test end" << endl;
    return 0;
}
