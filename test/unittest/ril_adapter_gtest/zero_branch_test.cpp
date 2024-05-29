/*
 * Copyright (C) 2022-2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License"));
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

#define private public
#define protected public
#include <sys/select.h>

#include "gtest/gtest.h"
#include "hril_data.h"
#include "hril_hdf.h"
#include "hril_manager.h"
#include "hril_modem.h"
#include "hril_network.h"
#include "hril_notification.h"
#include "hril_request.h"
#include "hril_sim.h"
#include "hril_sms.h"
#include "telephony_log_wrapper.h"
#include "v1_1/iril.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;
namespace {
constexpr int32_t WAIT_TELEPHONY_RETART_TIME = 60;
constexpr int32_t MAX_BUF_SIZE = 255;
constexpr const char *NUMBER = "123";
constexpr const char *COMMA_STR = ",";
constexpr const int32_t MAIL_DELAY_TIME = 50 * 1000;
constexpr const int32_t NUM_TWO = 2;
}

class BranchTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void BranchTest::SetUpTestCase() {}

void ReStartTelephony()
{
    FILE *fp;
    char buf[MAX_BUF_SIZE];
    std::string cmd = "pidof telephony";
    pid_t pid = -1;
    if ((fp = popen(cmd.c_str(), "r")) != nullptr) {
        if (fgets(buf, MAX_BUF_SIZE, fp) != nullptr) {
            pid = atoi(buf);
        }
        pclose(fp);
        fp = nullptr;
    }
    kill(pid, SIGKILL);
    sleep(WAIT_TELEPHONY_RETART_TIME);
}

void BranchTest::TearDownTestCase()
{
    ReStartTelephony();
}

void BranchTest::SetUp() {}

void BranchTest::TearDown() {}

bool TestCallInterface(std::shared_ptr<HRilManager> manager)
{
    OHOS::HDI::Ril::V1_1::EmergencyInfoList emergencyInfoList;
    EXPECT_NE(HDF_SUCCESS, manager->SetEmergencyCallList(0, 0, emergencyInfoList));
    EXPECT_NE(HDF_SUCCESS, manager->GetEmergencyCallList(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetCallList(0, 0));
    OHOS::HDI::Ril::V1_1::DialInfo dialInfo;
    EXPECT_NE(HDF_SUCCESS, manager->Dial(0, 0, dialInfo));
    EXPECT_NE(HDF_SUCCESS, manager->Reject(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->Hangup(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->Answer(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->HoldCall(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->UnHoldCall(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SwitchCall(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->CombineConference(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SeparateConference(0, 0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetCallWaiting(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetCallWaiting(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetCallTransferInfo(0, 0, 0));
    OHOS::HDI::Ril::V1_1::CallForwardSetInfo callForwardSetInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetCallTransferInfo(0, 0, callForwardSetInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetCallRestriction(0, 0, "0"));
    OHOS::HDI::Ril::V1_1::CallRestrictionInfo callRestrictionInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetCallRestriction(0, 0, callRestrictionInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetClip(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetClip(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetClir(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetClir(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetCallPreferenceMode(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetCallPreferenceMode(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetUssd(0, 0, "0"));
    EXPECT_NE(HDF_SUCCESS, manager->GetUssd(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetMute(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetMute(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetCallFailReason(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->CallSupplement(0, 0, 0));
    OHOS::HDI::Ril::V1_1::DtmfInfo info;
    EXPECT_NE(HDF_SUCCESS, manager->SendDtmf(0, 0, info));
    EXPECT_NE(HDF_SUCCESS, manager->StartDtmf(0, 0, info));
    EXPECT_NE(HDF_SUCCESS, manager->StopDtmf(0, 0, info));
    const OHOS::HDI::Ril::V1_1::SetBarringInfo setBarringInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetBarringPassword(0, 0, setBarringInfo));
    EXPECT_NE(HDF_SUCCESS, manager->SetVonrSwitch(0, 0, 0));
    return true;
}

bool TestDataInterface(std::shared_ptr<HRilManager> manager)
{
    OHOS::HDI::Ril::V1_1::DataCallInfo dataCallInfo;
    EXPECT_NE(HDF_SUCCESS, manager->ActivatePdpContext(0, 0, dataCallInfo));
    OHOS::HDI::Ril::V1_1::UniInfo uniInfo;
    EXPECT_NE(HDF_SUCCESS, manager->DeactivatePdpContext(0, 0, uniInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetPdpContextList(0, 0, uniInfo));
    OHOS::HDI::Ril::V1_1::DataProfileDataInfo dataProfileDataInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetInitApnInfo(0, 0, dataProfileDataInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetLinkBandwidthInfo(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetLinkCapability(0, 0));
    OHOS::HDI::Ril::V1_1::DataLinkBandwidthReportingRule dataLinkBandwidthReportingRule;
    EXPECT_NE(HDF_SUCCESS, manager->SetLinkBandwidthReportingRule(0, 0, dataLinkBandwidthReportingRule));
    EXPECT_NE(HDF_SUCCESS, manager->SetDataPermitted(0, 0, 0));
    OHOS::HDI::Ril::V1_1::DataProfilesInfo dataProfilesInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetDataProfileInfo(0, 0, dataProfilesInfo));
    OHOS::HDI::Ril::V1_1::DataPerformanceInfo dataPerformanceInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SendDataPerformanceMode(0, 0, dataPerformanceInfo));
    OHOS::HDI::Ril::V1_1::DataSleepInfo dataSleepInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SendDataSleepMode(0, 0, dataSleepInfo));
    EXPECT_NE(HDF_SUCCESS, manager->CleanAllConnections(0, 0));
    return true;
}

bool TestModemInterface(std::shared_ptr<HRilManager> manager)
{
    EXPECT_NE(HDF_SUCCESS, manager->SetRadioState(0, 0, 0, 1));
    EXPECT_NE(HDF_SUCCESS, manager->GetRadioState(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetImei(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetImeiSv(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetMeid(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetVoiceRadioTechnology(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetBasebandVersion(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->ShutDown(0, 0));
    return true;
}

bool TestSimInterface(std::shared_ptr<HRilManager> manager)
{
    OHOS::HDI::Ril::V1_1::SimIoRequestInfo simIO;
    EXPECT_NE(HDF_SUCCESS, manager->GetSimIO(0, 0, simIO));
    EXPECT_NE(HDF_SUCCESS, manager->GetSimStatus(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetImsi(0, 0));
    OHOS::HDI::Ril::V1_1::SimLockInfo simLockInfo;
    EXPECT_NE(HDF_SUCCESS, manager->GetSimLockStatus(0, 0, simLockInfo));
    EXPECT_NE(HDF_SUCCESS, manager->SetSimLock(0, 0, simLockInfo));
    OHOS::HDI::Ril::V1_1::SimPasswordInfo simPassword;
    EXPECT_NE(HDF_SUCCESS, manager->ChangeSimPassword(0, 0, simPassword));
    EXPECT_NE(HDF_SUCCESS, manager->UnlockPin(0, 0, ""));
    EXPECT_NE(HDF_SUCCESS, manager->UnlockPuk(0, 0, "", ""));
    EXPECT_NE(HDF_SUCCESS, manager->UnlockPin2(0, 0, ""));
    EXPECT_NE(HDF_SUCCESS, manager->UnlockPuk2(0, 0, "", ""));
    EXPECT_NE(HDF_SUCCESS, manager->SetActiveSim(0, 0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SimStkSendTerminalResponse(0, 0, "0"));
    EXPECT_NE(HDF_SUCCESS, manager->SimStkSendEnvelope(0, 0, "0"));
    EXPECT_NE(HDF_SUCCESS, manager->SimStkSendCallSetupRequestResult(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SimStkIsReady(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetRadioProtocol(0, 0));
    HDI::Ril::V1_1::RadioProtocol radioProtocol;
    EXPECT_NE(HDF_SUCCESS, manager->SetRadioProtocol(0, 0, radioProtocol));
    EXPECT_NE(HDF_SUCCESS, manager->SimOpenLogicalChannel(0, 0, "0", 0));
    EXPECT_NE(HDF_SUCCESS, manager->SimCloseLogicalChannel(0, 0, 0));
    OHOS::HDI::Ril::V1_1::ApduSimIORequestInfo apduSimIO;
    EXPECT_NE(HDF_SUCCESS, manager->SimTransmitApduLogicalChannel(0, 0, apduSimIO));
    EXPECT_NE(HDF_SUCCESS, manager->SimTransmitApduBasicChannel(0, 0, apduSimIO));
    OHOS::HDI::Ril::V1_1::SimAuthenticationRequestInfo simAuthInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SimAuthentication(0, 0, simAuthInfo));
    EXPECT_NE(HDF_SUCCESS, manager->UnlockSimLock(0, 0, 0, ""));
    OHOS::HDI::Ril::V1_2::NcfgOperatorInfo ncfgOperatorInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SendSimMatchedOperatorInfo(0, 0, ncfgOperatorInfo));
    return true;
}

bool TestNetWorkInterface(std::shared_ptr<HRilManager> manager)
{
    EXPECT_NE(HDF_SUCCESS, manager->GetSignalStrength(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetCsRegStatus(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetPsRegStatus(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetOperatorInfo(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetNetworkSearchInformation(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetNetworkSelectionMode(0, 0));
    HDI::Ril::V1_1::SetNetworkModeInfo networkModeInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetNetworkSelectionMode(0, 0, networkModeInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetNeighboringCellInfoList(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetCurrentCellInfo(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetPreferredNetwork(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetPreferredNetwork(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetPhysicalChannelConfig(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetLocateUpdates(0, 0, static_cast<HDI::Ril::V1_1::RilRegNotifyMode>(0)));
    EXPECT_NE(HDF_SUCCESS, manager->SetNotificationFilter(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetDeviceState(0, 0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetRrcConnectionState(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetNrOptionMode(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetNrOptionMode(0, 0, 1));
    EXPECT_NE(HDF_SUCCESS, manager->GetNrSsbId(0, 0));
    return true;
}

bool TestSmsInterface(std::shared_ptr<HRilManager> manager)
{
    OHOS::HDI::Ril::V1_1::GsmSmsMessageInfo gsmSmsMessageInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SendGsmSms(0, 0, gsmSmsMessageInfo));
    OHOS::HDI::Ril::V1_1::SendCdmaSmsMessageInfo cdmaSmsMessageInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SendCdmaSms(0, 0, cdmaSmsMessageInfo));
    OHOS::HDI::Ril::V1_1::SmsMessageIOInfo smsMessageIOInfo;
    EXPECT_NE(HDF_SUCCESS, manager->AddSimMessage(0, 0, smsMessageIOInfo));
    EXPECT_NE(HDF_SUCCESS, manager->DelSimMessage(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->UpdateSimMessage(0, 0, smsMessageIOInfo));
    EXPECT_NE(HDF_SUCCESS, manager->AddCdmaSimMessage(0, 0, smsMessageIOInfo));
    EXPECT_NE(HDF_SUCCESS, manager->DelCdmaSimMessage(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->UpdateCdmaSimMessage(0, 0, smsMessageIOInfo));
    OHOS::HDI::Ril::V1_1::ServiceCenterAddress serviceCenterAddress;
    EXPECT_NE(HDF_SUCCESS, manager->SetSmscAddr(0, 0, serviceCenterAddress));
    EXPECT_NE(HDF_SUCCESS, manager->GetSmscAddr(0, 0));
    OHOS::HDI::Ril::V1_1::CBConfigInfo cellBroadcastInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetCBConfig(0, 0, cellBroadcastInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetCBConfig(0, 0));
    OHOS::HDI::Ril::V1_1::CdmaCBConfigInfoList cdmaCBConfigInfoList;
    EXPECT_NE(HDF_SUCCESS, manager->SetCdmaCBConfig(0, 0, cdmaCBConfigInfoList));
    EXPECT_NE(HDF_SUCCESS, manager->GetCdmaCBConfig(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SendSmsMoreMode(0, 0, gsmSmsMessageInfo));
    OHOS::HDI::Ril::V1_1::ModeData modeData;
    EXPECT_NE(HDF_SUCCESS, manager->SendSmsAck(0, 0, modeData));
    return true;
}

/**
 * @tc.number   Telephony_HrilManager_Call_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Call_001, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    manager->RegisterCallFuncs(0, nullptr);
    manager->hrilCall_.clear();
    std::unique_ptr<HRilCall> call;
    manager->hrilCall_.push_back(std::move(call));
    EXPECT_EQ(true, TestCallInterface(manager));
    ReqDataInfo *requestInfo = manager->CreateHRilRequest(0, 0, 0);
    manager->ReleaseHRilRequest(0, requestInfo);
    manager->RegisterCallFuncs(0, nullptr);
    manager->ApplyRunningLock();
    manager->ReleaseRunningLock();
    manager->OnCallReport(0, nullptr, nullptr, 0);
    manager->OnDataReport(0, nullptr, nullptr, 0);
    manager->OnModemReport(0, nullptr, nullptr, 0);
    manager->OnNetworkReport(0, nullptr, nullptr, 0);
    manager->OnSimReport(0, nullptr, nullptr, 0);
    manager->OnSmsReport(0, nullptr, nullptr, 0);
    HRilRegOps(nullptr);
    ReleaseRilAdapter();
    OnTimerCallback(nullptr, nullptr, nullptr);
    EXPECT_EQ(manager->SendRilAck(), 0);
    manager->hrilCall_.clear();
    EXPECT_NE(manager->CloseUnFinishedUssd(0, 0), 0);
    EXPECT_GT(manager->GetMaxSimSlotCount(), 0);
}

/**
 * @tc.number  Telephony_HrilManager_Call_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Call_002, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    auto call = std::make_unique<HRilCall>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, call->GetCallListResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->DialResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->HangupResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->RejectResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->HoldCallResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->UnHoldCallResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SwitchCallResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->AnswerResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetClipResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetClipResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->CombineConferenceResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SeparateConferenceResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->CallSupplementResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallWaitingResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetCallWaitingResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallTransferInfoResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetCallTransferInfoResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallRestrictionResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetCallRestrictionResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetClirResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetClirResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->StartDtmfResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SendDtmfResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->StopDtmfResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallPreferenceModeResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetCallPreferenceModeResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetUssdResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetUssdResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetMuteResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetMuteResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetEmergencyCallListResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallFailReasonResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetEmergencyCallListResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->SetBarringPasswordResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->CallStateUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->CallUssdNotice(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->CallRingbackVoiceNotice(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->CallSrvccStatusNotice(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->CallEmergencyNotice(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->CallSsNotice(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->CallRsrvccStatusNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->SetVonrSwitchResponse(0, responseInfo, nullptr, 1));
}

/**
 * @tc.number   Telephony_HrilManager_Call_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Call_003, Function | MediumTest | Level2)
{
    auto call = std::make_unique<HRilCall>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, call->GetCallListResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallListResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->GetClipResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetClipResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->GetCallWaitingResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallWaitingResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->GetCallTransferInfoResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallTransferInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->GetClirResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetClirResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->GetCallRestrictionResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallRestrictionResponse(0, responseInfo, nullptr, 0));
    HRilCallInfo callInfo;
    callInfo.number = const_cast<char *>(NUMBER);
    callInfo.alpha = const_cast<char *>(NUMBER);
    EXPECT_NE(HDF_SUCCESS, call->GetCallListResponse(0, responseInfo, &callInfo, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallListResponse(0, responseInfo, &callInfo, sizeof(HRilCallInfo)));
    HDI::Ril::V1_1::CallInfoList callList;
    call->BuildICallList(callList, nullptr, sizeof(HRilCallInfo));
    HRilGetClipResult clipResult;
    EXPECT_NE(HDF_SUCCESS, call->GetClipResponse(0, responseInfo, &clipResult, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetClipResponse(0, responseInfo, &clipResult, sizeof(HRilGetClipResult)));
    HRilCallWaitResult waitResult;
    EXPECT_NE(HDF_SUCCESS, call->GetCallWaitingResponse(0, responseInfo, &waitResult, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallWaitingResponse(0, responseInfo, &waitResult, sizeof(HRilCallWaitResult)));
    HRilCFQueryInfo queryInfo;
    EXPECT_NE(HDF_SUCCESS, call->GetCallTransferInfoResponse(0, responseInfo, &queryInfo, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallTransferInfoResponse(0, responseInfo, &queryInfo, sizeof(HRilCFQueryInfo)));
    HRilGetCallClirResult clirResult;
    EXPECT_NE(HDF_SUCCESS, call->GetClirResponse(0, responseInfo, &clirResult, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetClirResponse(0, responseInfo, &clirResult, sizeof(HRilGetCallClirResult)));
    HRilCallRestrictionResult restrictionResult;
    EXPECT_NE(HDF_SUCCESS, call->GetCallRestrictionResponse(0, responseInfo, &restrictionResult, 1));
    EXPECT_NE(HDF_SUCCESS,
        call->GetCallRestrictionResponse(0, responseInfo, &restrictionResult, sizeof(HRilCallRestrictionResult)));
}

/**
 * @tc.number   Telephony_HrilManager_Call_004
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Call_004, Function | MediumTest | Level2)
{
    auto call = std::make_unique<HRilCall>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, call->GetCallPreferenceModeResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallPreferenceModeResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->GetMuteResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetMuteResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->GetUssdResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetUssdResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->GetCallFailReasonResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallFailReasonResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->GetEmergencyCallListResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetEmergencyCallListResponse(0, responseInfo, nullptr, 0));
    int32_t mode = 1;
    EXPECT_NE(HDF_SUCCESS, call->GetCallPreferenceModeResponse(0, responseInfo, &mode, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallPreferenceModeResponse(0, responseInfo, &mode, sizeof(int32_t)));
    int32_t mute = 1;
    EXPECT_NE(HDF_SUCCESS, call->GetMuteResponse(0, responseInfo, &mute, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetMuteResponse(0, responseInfo, &mute, sizeof(int32_t)));
    int32_t cusd = 1;
    EXPECT_NE(HDF_SUCCESS, call->GetUssdResponse(0, responseInfo, &cusd, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetUssdResponse(0, responseInfo, &cusd, sizeof(int32_t)));
    int32_t callFail = 1;
    EXPECT_NE(HDF_SUCCESS, call->GetCallFailReasonResponse(0, responseInfo, &callFail, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetCallFailReasonResponse(0, responseInfo, &callFail, sizeof(int32_t)));
    HDI::Ril::V1_1::EmergencyInfoList callList;
    call->BuildIEmergencyCallList(callList, nullptr, sizeof(HRilEmergencyInfo));
    HRilEmergencyInfo emgInfo;
    emgInfo.eccNum = const_cast<char *>(NUMBER);
    emgInfo.mcc = const_cast<char *>(NUMBER);
    EXPECT_NE(HDF_SUCCESS, call->GetEmergencyCallListResponse(0, responseInfo, &emgInfo, 1));
    EXPECT_NE(HDF_SUCCESS, call->GetEmergencyCallListResponse(0, responseInfo, &emgInfo, sizeof(HRilEmergencyInfo)));
}

/**
 * @tc.number   Telephony_HrilManager_Call_005
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Call_005, Function | MediumTest | Level2)
{
    auto call = std::make_unique<HRilCall>(0);
    HRilErrNumber err = HRilErrNumber::HRIL_ERR_NULL_POINT;
    EXPECT_NE(HDF_SUCCESS, call->CallUssdNotice(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->CallSsNotice(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->CallSrvccStatusNotice(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->CallRingbackVoiceNotice(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, call->CallEmergencyNotice(0, err, nullptr, 0));
    HRilUssdNoticeInfo ussdInfo;
    ussdInfo.str = const_cast<char *>(NUMBER);
    EXPECT_NE(HDF_SUCCESS, call->CallUssdNotice(0, err, &ussdInfo, 1));
    EXPECT_NE(HDF_SUCCESS, call->CallUssdNotice(0, err, &ussdInfo, sizeof(HRilUssdNoticeInfo)));
    HRilSsNoticeInfo ssInfo;
    EXPECT_NE(HDF_SUCCESS, call->CallSsNotice(0, err, &ssInfo, 1));
    EXPECT_NE(HDF_SUCCESS, call->CallSsNotice(0, err, &ssInfo, sizeof(HRilSsNoticeInfo)));
    HRilCallSrvccStatus srvccStatus;
    EXPECT_NE(HDF_SUCCESS, call->CallSrvccStatusNotice(0, err, &srvccStatus, 1));
    EXPECT_NE(HDF_SUCCESS, call->CallSrvccStatusNotice(0, err, &srvccStatus, sizeof(HRilCallSrvccStatus)));
    int32_t ringbackVoiceFlag = 0;
    EXPECT_NE(HDF_SUCCESS, call->CallRingbackVoiceNotice(0, err, &ringbackVoiceFlag, 1));
    EXPECT_NE(HDF_SUCCESS, call->CallRingbackVoiceNotice(0, err, &ringbackVoiceFlag, sizeof(int32_t)));
    HRilEmergencyInfo emgInfo;
    emgInfo.eccNum = const_cast<char *>(NUMBER);
    emgInfo.mcc = const_cast<char *>(NUMBER);
    EXPECT_NE(HDF_SUCCESS, call->CallEmergencyNotice(0, err, &emgInfo, 1));
    EXPECT_NE(HDF_SUCCESS, call->CallEmergencyNotice(0, err, &emgInfo, sizeof(HRilEmergencyInfo)));
}

/**
 * @tc.number   Telephony_HrilManager_Data_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Data_001, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    manager->RegisterDataFuncs(0, nullptr);
    manager->hrilData_.clear();
    std::unique_ptr<HRilData> data;
    manager->hrilData_.push_back(std::move(data));
    manager->RegisterDataFuncs(0, nullptr);
    EXPECT_EQ(true, TestDataInterface(manager));
}

/**
 * @tc.number   Telephony_HrilManager_Data_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Data_002, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    auto data = std::make_unique<HRilData>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, data->ActivatePdpContextResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->DeactivatePdpContextResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->GetPdpContextListResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->SetInitApnInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->PdpContextListUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->DataLinkCapabilityUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->GetLinkBandwidthInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->SetLinkBandwidthReportingRuleResponse(0, responseInfo, nullptr, 0));
    EXPECT_EQ(HDF_SUCCESS, data->SetDataProfileInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->SetDataPermittedResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->GetLinkCapabilityResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->CleanAllConnectionsResponse(0, responseInfo, nullptr, 0));
}

/**
 * @tc.number   Telephony_HrilManager_Data_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Data_003, Function | MediumTest | Level2)
{
    auto data = std::make_unique<HRilData>(0);
    HRilErrNumber err = HRilErrNumber::HRIL_ERR_NULL_POINT;
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, data->ActivatePdpContextResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, data->ActivatePdpContextResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->GetPdpContextListResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, data->GetPdpContextListResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->PdpContextListUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->DataLinkCapabilityUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->GetLinkCapabilityResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, data->GetLinkCapabilityResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->GetLinkBandwidthInfoResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, data->GetLinkBandwidthInfoResponse(0, responseInfo, nullptr, 0));
    HRilDataCallResponse pDataCall;
    int32_t len = sizeof(HRilDataCallResponse);
    pDataCall.type = const_cast<char *>(NUMBER);
    pDataCall.netPortName = const_cast<char *>(NUMBER);
    pDataCall.address = const_cast<char *>(NUMBER);
    pDataCall.dns = const_cast<char *>(NUMBER);
    pDataCall.dnsSec = const_cast<char *>(NUMBER);
    pDataCall.gateway = const_cast<char *>(NUMBER);
    pDataCall.pCscfPrimAddr = const_cast<char *>(NUMBER);
    pDataCall.pCscfSecAddr = const_cast<char *>(NUMBER);
    HDI::Ril::V1_1::SetupDataCallResultInfo result;
    data->SwitchRilDataToHal(nullptr, result);
    data->SwitchRilDataToHal(&pDataCall, result);
    EXPECT_NE(HDF_SUCCESS, data->ActivatePdpContextResponse(0, responseInfo, &pDataCall, 1));
    EXPECT_NE(HDF_SUCCESS, data->ActivatePdpContextResponse(0, responseInfo, &pDataCall, len));
    HDI::Ril::V1_1::DataCallResultList dataCallResultList;
    dataCallResultList.dcList.push_back(result);
    data->SwitchHRilDataListToHal(nullptr, 0, dataCallResultList.dcList);
    data->SwitchHRilDataListToHal(&pDataCall, len, dataCallResultList.dcList);
    EXPECT_NE(HDF_SUCCESS, data->GetPdpContextListResponse(0, responseInfo, &pDataCall, 1));
    EXPECT_NE(HDF_SUCCESS, data->GetPdpContextListResponse(0, responseInfo, &pDataCall, len));
    EXPECT_NE(HDF_SUCCESS, data->PdpContextListUpdated(0, err, &pDataCall, 1));
    EXPECT_NE(HDF_SUCCESS, data->PdpContextListUpdated(0, err, &pDataCall, len));
    HRilDataLinkCapability capability;
    EXPECT_NE(HDF_SUCCESS, data->DataLinkCapabilityUpdated(0, err, &capability, 1));
    EXPECT_NE(HDF_SUCCESS, data->DataLinkCapabilityUpdated(0, err, &capability, sizeof(HRilDataLinkCapability)));
    EXPECT_NE(HDF_SUCCESS, data->GetLinkCapabilityResponse(0, responseInfo, &capability, 1));
    EXPECT_NE(
        HDF_SUCCESS, data->GetLinkCapabilityResponse(0, responseInfo, &capability, sizeof(HRilDataLinkCapability)));
    HRilLinkBandwidthInfo info;
    EXPECT_NE(HDF_SUCCESS, data->GetLinkBandwidthInfoResponse(0, responseInfo, &info, 1));
    EXPECT_NE(HDF_SUCCESS, data->GetLinkBandwidthInfoResponse(0, responseInfo, &info, sizeof(HRilLinkBandwidthInfo)));
}

/**
 * @tc.number   Telephony_HrilManager_Modem_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Modem_001, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    manager->RegisterModemFuncs(0, nullptr);
    manager->hrilModem_.clear();
    std::unique_ptr<HRilModem> data;
    manager->hrilModem_.push_back(std::move(data));
    manager->RegisterModemFuncs(0, nullptr);
    EXPECT_EQ(true, TestModemInterface(manager));
}

/**
 * @tc.number   Telephony_HrilManager_Modem_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Modem_002, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    auto modem = std::make_unique<HRilModem>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, modem->ShutDownResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->SetRadioStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetRadioStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiSvResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetMeidResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetVoiceRadioTechnologyResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetBasebandVersionResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->RadioStateUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->VoiceRadioTechUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->DsdsModeUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
}

/**
 * @tc.number   Telephony_HrilManager_Modem_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Modem_003, Function | MediumTest | Level2)
{
    auto modem = std::make_unique<HRilModem>(0);
    HRilErrNumber err = HRilErrNumber::HRIL_ERR_NULL_POINT;
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, modem->RadioStateUpdated(0, err, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, modem->RadioStateUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->VoiceRadioTechUpdated(0, err, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, modem->VoiceRadioTechUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->DsdsModeUpdated(0, err, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, modem->DsdsModeUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetRadioStateResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetRadioStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiSvResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiSvResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetMeidResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetMeidResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetBasebandVersionResponse(0, responseInfo, nullptr, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetBasebandVersionResponse(0, responseInfo, nullptr, 0));
    responseInfo.error = HDI::Ril::V1_1::RilErrType::RIL_ERR_GENERIC_FAILURE;
    EXPECT_NE(HDF_SUCCESS, modem->GetVoiceRadioTechnologyResponse(0, responseInfo, nullptr, 0));
    responseInfo.error = HDI::Ril::V1_1::RilErrType::NONE;
    EXPECT_NE(HDF_SUCCESS, modem->GetVoiceRadioTechnologyResponse(0, responseInfo, nullptr, 0));
    int32_t in = 1;
    EXPECT_NE(HDF_SUCCESS, modem->RadioStateUpdated(0, err, &in, 1));
    EXPECT_NE(HDF_SUCCESS, modem->RadioStateUpdated(0, err, &in, sizeof(int32_t)));
    HRilVoiceRadioInfo radioInfo;
    radioInfo.sysModeName = const_cast<char *>(NUMBER);
    radioInfo.actName = const_cast<char *>(NUMBER);
    EXPECT_NE(HDF_SUCCESS, modem->VoiceRadioTechUpdated(0, err, &radioInfo, 1));
    EXPECT_NE(HDF_SUCCESS, modem->VoiceRadioTechUpdated(0, err, &radioInfo, sizeof(HRilVoiceRadioInfo)));
    EXPECT_NE(HDF_SUCCESS, modem->DsdsModeUpdated(0, err, &in, 1));
    EXPECT_NE(HDF_SUCCESS, modem->DsdsModeUpdated(0, err, &in, sizeof(int32_t)));
    EXPECT_NE(HDF_SUCCESS, modem->GetRadioStateResponse(0, responseInfo, &in, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetRadioStateResponse(0, responseInfo, &in, sizeof(int32_t)));
    char *str = const_cast<char *>(NUMBER);
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiResponse(0, responseInfo, str, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiResponse(0, responseInfo, str, sizeof(NUMBER)));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiSvResponse(0, responseInfo, str, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiSvResponse(0, responseInfo, str, sizeof(NUMBER)));
    EXPECT_NE(HDF_SUCCESS, modem->GetMeidResponse(0, responseInfo, str, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetMeidResponse(0, responseInfo, str, sizeof(NUMBER)));
    EXPECT_NE(HDF_SUCCESS, modem->GetBasebandVersionResponse(0, responseInfo, str, 1));
    EXPECT_NE(HDF_SUCCESS, modem->GetBasebandVersionResponse(0, responseInfo, str, sizeof(NUMBER)));
    EXPECT_NE(
        HDF_SUCCESS, modem->GetVoiceRadioTechnologyResponse(0, responseInfo, &radioInfo, sizeof(HRilVoiceRadioInfo)));
}

/**
 * @tc.number   Telephony_HrilManager_Network_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Sim_001, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    manager->hrilSim_.clear();
    std::unique_ptr<HRilSim> data;
    manager->hrilSim_.push_back(std::move(data));
    EXPECT_EQ(true, TestSimInterface(manager));
}

/**
 * @tc.number   Telephony_HrilManager_Sim_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Sim_002, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    auto sim = std::make_unique<HRilSim>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, sim->GetSimIOResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->GetSimStatusResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->GetImsiResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->GetSimLockStatusResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SetSimLockResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->ChangeSimPasswordResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->UnlockPinResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->UnlockPukResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->UnlockPin2Response(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->UnlockPuk2Response(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SetActiveSimResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimStkSendTerminalResponseResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimStkSendEnvelopeResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimStkSendCallSetupRequestResultResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimStkIsReadyResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->GetRadioProtocolResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SetRadioProtocolResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimOpenLogicalChannelResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimCloseLogicalChannelResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimTransmitApduLogicalChannelResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimTransmitApduBasicChannelResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimAuthenticationResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->UnlockSimLockResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SendSimMatchedOperatorInfoResponse(0, responseInfo, nullptr, 0));

    EXPECT_NE(HDF_SUCCESS, sim->SimStateUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimStkSessionEndNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimStkProactiveNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimStkAlphaNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimStkEventNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimStkCallSetupNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimRefreshNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sim->SimRadioProtocolUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
}

/**
 * @tc.number   Telephony_HrilManager_Network_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Network_001, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    manager->hrilNetwork_.clear();
    std::unique_ptr<HRilNetwork> data;
    manager->hrilNetwork_.push_back(std::move(data));
    EXPECT_EQ(true, TestNetWorkInterface(manager));
}

/**
 * @tc.number   Telephony_HrilManager_Network_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Network_002, Function | MediumTest | Level3)
{
    auto manager = std::make_shared<HRilManager>();
    auto network = std::make_unique<HRilNetwork>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, network->GetSignalStrengthResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCsRegStatusResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPsRegStatusResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetOperatorInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNetworkSearchInformationResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNetworkSelectionModeResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SetNetworkSelectionModeResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SetPreferredNetworkResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPreferredNetworkResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse_1_2(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_1(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_2(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPhysicalChannelConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SetLocateUpdatesResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SetNotificationFilterResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SetDeviceStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetRrcConnectionStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNrOptionModeResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SetNrOptionModeResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNrSsbIdResponse(0, responseInfo, nullptr, 0));
    HRilRegStatusInfo regStatusInfo;
    EXPECT_NE(HDF_SUCCESS, network->NetworkCsRegStatusUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCsRegStatusUpdated(
                               0, HRilErrNumber::HRIL_ERR_NULL_POINT, (const uint8_t *)(&regStatusInfo), 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCsRegStatusUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT,
                               (const uint8_t *)(&regStatusInfo), sizeof(HRilRegStatusInfo)));
    EXPECT_NE(HDF_SUCCESS, network->NetworkPsRegStatusUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkPsRegStatusUpdated(
                               0, HRilErrNumber::HRIL_ERR_NULL_POINT, (const uint8_t *)(&regStatusInfo), 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkPsRegStatusUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT,
                               (const uint8_t *)(&regStatusInfo), sizeof(HRilRegStatusInfo)));
    EXPECT_NE(HDF_SUCCESS, network->SignalStrengthUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkTimeUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkTimeZoneUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkPhyChnlCfgUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated_1_2(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->ResidentNetworkUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
}

/**
 * @tc.number   Telephony_HrilManager_Network_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Network_003, Function | MediumTest | Level3)
{
    auto manager = std::make_shared<HRilManager>();
    manager->hrilNetwork_.clear();
    std::unique_ptr<HRilNetwork> data;
    manager->hrilNetwork_.push_back(std::move(data));

    struct ReportInfo reportNotification;
    reportNotification.error = static_cast<HRilErrNumber>(0);
    reportNotification.notifyId = HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED;
    reportNotification.type = HRIL_NOTIFICATION;
    manager->OnNetworkReport(0, nullptr, nullptr, 0);
    reportNotification.notifyId = HNOTI_NETWORK_RRC_CONNECTION_STATE_UPDATED;
    manager->OnNetworkReport(0, nullptr, nullptr, 0);

    struct ReportInfo reportResponse;
    reportResponse.error = static_cast<HRilErrNumber>(0);
    reportResponse.type = HRIL_RESPONSE;
    ReqDataInfo *operatorRequestInfo = (ReqDataInfo *)malloc(sizeof(ReqDataInfo));
    operatorRequestInfo->serial = 0;
    operatorRequestInfo->request = HREQ_NETWORK_GET_OPERATOR_INFO;
    reportResponse.requestInfo = operatorRequestInfo;
    manager->OnNetworkReport(0, nullptr, nullptr, 0);
    ReqDataInfo *signalRequestInfo = (ReqDataInfo *)malloc(sizeof(ReqDataInfo));
    operatorRequestInfo->serial = 0;
    signalRequestInfo->request = HREQ_NETWORK_GET_SIGNAL_STRENGTH;
    reportResponse.requestInfo = signalRequestInfo;
    manager->OnNetworkReport(0, nullptr, nullptr, 0);
    EXPECT_EQ(true, TestNetWorkInterface(manager));
}

/**
 * @tc.number   Telephony_HrilManager_Network_004
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Network_004, Function | MediumTest | Level3)
{
    auto network = std::make_unique<HRilNetwork>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    responseInfo.error = HDI::Ril::V1_1::RilErrType::RIL_ERR_GENERIC_FAILURE;
    EXPECT_NE(HDF_SUCCESS, network->GetNrSsbIdResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_1(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_2(0, responseInfo, nullptr, 0));
    responseInfo.error = HDI::Ril::V1_1::RilErrType::NONE;
    EXPECT_NE(HDF_SUCCESS, network->GetNrSsbIdResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_1(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_2(0, responseInfo, nullptr, 0));
    NrCellSsbIdsVendor nrSsbIdInfo;
    EXPECT_NE(HDF_SUCCESS, network->GetNrSsbIdResponse(0, responseInfo, &nrSsbIdInfo, 0));
    nrSsbIdInfo.nbCellCount = 5;
    EXPECT_NE(HDF_SUCCESS, network->GetNrSsbIdResponse(0, responseInfo, &nrSsbIdInfo, sizeof(NrCellSsbIdsVendor)));
    CurrentCellInfoList info = { 1, nullptr };
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse(0, responseInfo, &info, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_1(0, responseInfo, &info, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_2(0, responseInfo, &info, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse(0, responseInfo, &info, sizeof(CurrentCellInfoList)));
    EXPECT_NE(
        HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_1(0, responseInfo, &info, sizeof(CurrentCellInfoList)));
    EXPECT_NE(
        HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_2(0, responseInfo, &info, sizeof(CurrentCellInfoList)));
    CurrentCellInfoVendor infoVendor;
    info.currentCellInfo = &infoVendor;
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse(0, responseInfo, &info, sizeof(CurrentCellInfoList)));
    EXPECT_NE(
        HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_1(0, responseInfo, &info, sizeof(CurrentCellInfoList)));
    EXPECT_NE(
        HDF_SUCCESS, network->GetCurrentCellInfoResponse_1_2(0, responseInfo, &info, sizeof(CurrentCellInfoList)));
}

/**
 * @tc.number   Telephony_HrilManager_Network_005
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Network_005, Function | MediumTest | Level3)
{
    auto network = std::make_unique<HRilNetwork>(0);
    HDI::Ril::V1_1::CurrentCellInfo cellInfo;
    HDI::Ril::V1_1::CurrentCellInfo_1_1 cellInfo_1_1;
    HDI::Ril::V1_2::CurrentCellInfo_1_2 cellInfo_1_2;
    CurrentCellInfoVendor infoVendor;
    infoVendor.ratType = NETWORK_TYPE_GSM;
    network->FillCurrentCellInfo(cellInfo, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_1, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_2, &infoVendor);
    infoVendor.ratType = NETWORK_TYPE_LTE;
    network->FillCurrentCellInfo(cellInfo, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_1, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_2, &infoVendor);
    infoVendor.ratType = NETWORK_TYPE_WCDMA;
    network->FillCurrentCellInfo(cellInfo, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_1, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_2, &infoVendor);
    infoVendor.ratType = NETWORK_TYPE_CDMA;
    network->FillCurrentCellInfo(cellInfo, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_1, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_2, &infoVendor);
    infoVendor.ratType = NETWORK_TYPE_TDSCDMA;
    network->FillCurrentCellInfo(cellInfo, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_1, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_2, &infoVendor);
    infoVendor.ratType = NETWORK_TYPE_NR;
    network->FillCurrentCellInfo(cellInfo, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_1, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_2, &infoVendor);
    infoVendor.ratType = NETWORK_TYPE_UNKNOWN;
    network->FillCurrentCellInfo(cellInfo, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_1, &infoVendor);
    network->FillCurrentCellInfo(cellInfo_1_2, &infoVendor);
}

/**
 * @tc.number   Telephony_HrilManager_Network_006
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Network_006, Function | MediumTest | Level3)
{
    auto network = std::make_unique<HRilNetwork>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    CellInfo cellInfo;
    HDI::Ril::V1_1::CellNearbyInfo cellNearbyInfo;
    HDI::Ril::V1_2::CellNearbyInfo_1_2 cellNearbyInfo_1_2;
    cellInfo.ratType = NETWORK_TYPE_GSM;
    network->FillCellNearbyInfo(cellNearbyInfo, &cellInfo);
    network->FillCellNearbyInfo(cellNearbyInfo_1_2, &cellInfo);
    cellInfo.ratType = NETWORK_TYPE_LTE;
    network->FillCellNearbyInfo(cellNearbyInfo, &cellInfo);
    network->FillCellNearbyInfo(cellNearbyInfo_1_2, &cellInfo);
    cellInfo.ratType = NETWORK_TYPE_WCDMA;
    network->FillCellNearbyInfo(cellNearbyInfo, &cellInfo);
    network->FillCellNearbyInfo(cellNearbyInfo_1_2, &cellInfo);
    cellInfo.ratType = NETWORK_TYPE_CDMA;
    network->FillCellNearbyInfo(cellNearbyInfo, &cellInfo);
    network->FillCellNearbyInfo(cellNearbyInfo_1_2, &cellInfo);
    cellInfo.ratType = NETWORK_TYPE_TDSCDMA;
    network->FillCellNearbyInfo(cellNearbyInfo, &cellInfo);
    network->FillCellNearbyInfo(cellNearbyInfo_1_2, &cellInfo);
    cellInfo.ratType = NETWORK_TYPE_NR;
    network->FillCellNearbyInfo(cellNearbyInfo, &cellInfo);
    network->FillCellNearbyInfo(cellNearbyInfo_1_2, &cellInfo);
    cellInfo.ratType = NETWORK_TYPE_UNKNOWN;
    network->FillCellNearbyInfo(cellNearbyInfo, &cellInfo);
    network->FillCellNearbyInfo(cellNearbyInfo_1_2, &cellInfo);
    responseInfo.error = HDI::Ril::V1_1::RilErrType::RIL_ERR_GENERIC_FAILURE;
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse_1_2(0, responseInfo, nullptr, 0));
    responseInfo.error = HDI::Ril::V1_1::RilErrType::NONE;
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse_1_2(0, responseInfo, nullptr, 0));
    CellInfoList list = { 1, nullptr };
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse(0, responseInfo, &list, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse_1_2(0, responseInfo, &list, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse(0, responseInfo, &list, sizeof(CellInfoList)));
    EXPECT_NE(
        HDF_SUCCESS, network->GetNeighboringCellInfoListResponse_1_2(0, responseInfo, &list, sizeof(CellInfoList)));
    list.cellNearbyInfo = &cellInfo;
    EXPECT_NE(HDF_SUCCESS, network->GetNeighboringCellInfoListResponse(0, responseInfo, &list, sizeof(CellInfoList)));
    EXPECT_NE(
        HDF_SUCCESS, network->GetNeighboringCellInfoListResponse_1_2(0, responseInfo, &list, sizeof(CellInfoList)));
}

/**
 * @tc.number   Telephony_HrilManager_Network_007
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Network_007, Function | MediumTest | Level3)
{
    auto network = std::make_unique<HRilNetwork>(0);
    HRilErrNumber err = HRilErrNumber::HRIL_ERR_NULL_POINT;
    HRilRssi hrilRssi;
    EXPECT_NE(HDF_SUCCESS, network->SignalStrengthUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SignalStrengthUpdated(0, err, &hrilRssi, 0));
    EXPECT_NE(HDF_SUCCESS, network->SignalStrengthUpdated(0, err, &hrilRssi, sizeof(HRilRssi)));
    EXPECT_NE(HDF_SUCCESS, network->NetworkTimeUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkTimeUpdated(0, err, NUMBER, sizeof(NUMBER)));
    EXPECT_NE(HDF_SUCCESS, network->NetworkTimeZoneUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkTimeZoneUpdated(0, err, NUMBER, sizeof(NUMBER)));
    int32_t contextIds[] = { 1, 1, 1 };
    HRilPhyChannelConfig configInfo = { HRIL_SERVING_CELL_PRIMARY, RADIO_TECHNOLOGY_GSM, 1, 1, 1, 1, 1, 1, 1 };
    configInfo.contextIds = contextIds;
    HRilChannelConfigList configInfoList = { 1, &configInfo };
    EXPECT_NE(HDF_SUCCESS, network->NetworkPhyChnlCfgUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkPhyChnlCfgUpdated(0, err, &configInfoList, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkPhyChnlCfgUpdated(0, err, &configInfoList, sizeof(HRilChannelConfigList)));
    CurrentCellInfoVendor infoVendor;
    CurrentCellInfoList info = { 1, &infoVendor };
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated(0, err, &info, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated(0, err, &info, sizeof(CurrentCellInfoList)));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated_1_1(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated_1_1(0, err, &info, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated_1_1(0, err, &info, sizeof(CurrentCellInfoList)));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated_1_2(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated_1_2(0, err, &info, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated_1_2(0, err, &info, sizeof(CurrentCellInfoList)));
    EXPECT_NE(HDF_SUCCESS, network->ResidentNetworkUpdated(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->ResidentNetworkUpdated(0, err, NUMBER, sizeof(NUMBER)));
}

/**
 * @tc.number   Telephony_HrilManager_Network_008
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Network_008, Function | MediumTest | Level3)
{
    auto network = std::make_unique<HRilNetwork>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    responseInfo.error = HDI::Ril::V1_1::RilErrType::RIL_ERR_GENERIC_FAILURE;
    EXPECT_NE(HDF_SUCCESS, network->GetSignalStrengthResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCsRegStatusResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPsRegStatusResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetOperatorInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNetworkSearchInformationResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNetworkSelectionModeResponse(0, responseInfo, nullptr, 0));
    responseInfo.error = HDI::Ril::V1_1::RilErrType::NONE;
    EXPECT_NE(HDF_SUCCESS, network->GetSignalStrengthResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCsRegStatusResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPsRegStatusResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetOperatorInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNetworkSearchInformationResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNetworkSelectionModeResponse(0, responseInfo, nullptr, 0));
    HRilRssi hrilRssi;
    EXPECT_NE(HDF_SUCCESS, network->GetSignalStrengthResponse(0, responseInfo, &hrilRssi, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetSignalStrengthResponse(0, responseInfo, &hrilRssi, sizeof(HRilRssi)));
    HRilRegStatusInfo statusInfo;
    EXPECT_NE(HDF_SUCCESS, network->GetCsRegStatusResponse(0, responseInfo, &statusInfo, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetCsRegStatusResponse(0, responseInfo, &statusInfo, sizeof(HRilRegStatusInfo)));
    EXPECT_NE(HDF_SUCCESS, network->GetPsRegStatusResponse(0, responseInfo, &statusInfo, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPsRegStatusResponse(0, responseInfo, &statusInfo, sizeof(HRilRegStatusInfo)));
    EXPECT_NE(HDF_SUCCESS, network->GetOperatorInfoResponse(0, responseInfo, &NUMBER, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetOperatorInfoResponse(0, responseInfo, &NUMBER, sizeof(NUMBER)));
    AvailableOperInfo *operInfo = (AvailableOperInfo *)malloc(2 * sizeof(AvailableOperInfo));
    operInfo->longName = const_cast<char *>(NUMBER);
    operInfo->shortName = const_cast<char *>(NUMBER);
    operInfo->numeric = const_cast<char *>(NUMBER);
    EXPECT_NE(HDF_SUCCESS, network->GetNetworkSearchInformationResponse(0, responseInfo, &operInfo, 0));
    EXPECT_NE(HDF_SUCCESS,
        network->GetNetworkSearchInformationResponse(0, responseInfo, &operInfo, sizeof(AvailableOperInfo *)));
    int32_t selectMode = 1;
    EXPECT_NE(HDF_SUCCESS, network->GetNetworkSelectionModeResponse(0, responseInfo, &selectMode, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNetworkSelectionModeResponse(0, responseInfo, &selectMode, sizeof(int32_t)));
}

/**
 * @tc.number   Telephony_HrilManager_Network_009
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Network_009, Function | MediumTest | Level3)
{
    auto network = std::make_unique<HRilNetwork>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    responseInfo.error = HDI::Ril::V1_1::RilErrType::RIL_ERR_GENERIC_FAILURE;
    EXPECT_NE(HDF_SUCCESS, network->GetPreferredNetworkResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPhysicalChannelConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetRrcConnectionStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNrOptionModeResponse(0, responseInfo, nullptr, 0));
    responseInfo.error = HDI::Ril::V1_1::RilErrType::NONE;
    EXPECT_NE(HDF_SUCCESS, network->GetPreferredNetworkResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPhysicalChannelConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetRrcConnectionStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNrOptionModeResponse(0, responseInfo, nullptr, 0));
    int32_t networkType = 1;
    EXPECT_NE(HDF_SUCCESS, network->GetPreferredNetworkResponse(0, responseInfo, &networkType, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPreferredNetworkResponse(0, responseInfo, &networkType, sizeof(int32_t)));
    int32_t contextIds[] = { 1, 1, 1 };
    HRilPhyChannelConfig configInfo = { HRIL_SERVING_CELL_PRIMARY, RADIO_TECHNOLOGY_GSM, 1, 1, 1, 1, 1, 1, 1 };
    configInfo.contextIds = contextIds;
    HRilChannelConfigList configInfoList = { 1, &configInfo };
    EXPECT_NE(HDF_SUCCESS, network->GetPhysicalChannelConfigResponse(0, responseInfo, &configInfoList, 0));
    EXPECT_NE(HDF_SUCCESS,
        network->GetPhysicalChannelConfigResponse(0, responseInfo, &configInfoList, sizeof(HRilChannelConfigList)));
    int32_t connState = 1;
    EXPECT_NE(HDF_SUCCESS, network->GetRrcConnectionStateResponse(0, responseInfo, &connState, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetRrcConnectionStateResponse(0, responseInfo, &connState, sizeof(int32_t)));
    int32_t nrOptionMode = 1;
    EXPECT_NE(HDF_SUCCESS, network->GetNrOptionModeResponse(0, responseInfo, &nrOptionMode, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetNrOptionModeResponse(0, responseInfo, &nrOptionMode, sizeof(int32_t)));
    int32_t mode = 0;
    EXPECT_NE(HDF_SUCCESS, network->SetLocateUpdates(0, static_cast<HDI::Ril::V1_1::RilRegNotifyMode>(mode)));
    mode = -1;
    EXPECT_NE(HDF_SUCCESS, network->SetLocateUpdates(0, static_cast<HDI::Ril::V1_1::RilRegNotifyMode>(mode)));
    mode = 3;
    EXPECT_NE(HDF_SUCCESS, network->SetLocateUpdates(0, static_cast<HDI::Ril::V1_1::RilRegNotifyMode>(mode)));
}

/**
 * @tc.number   Telephony_HrilManager_Sms_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Sms_001, Function | MediumTest | Level3)
{
    auto manager = std::make_shared<HRilManager>();
    manager->RegisterSmsFuncs(0, nullptr);
    manager->hrilSms_.clear();
    std::unique_ptr<HRilSms> data;
    manager->hrilSms_.push_back(std::move(data));
    manager->RegisterSmsFuncs(0, nullptr);
    EXPECT_EQ(true, TestSmsInterface(manager));
}

/**
 * @tc.number   Telephony_HrilManager_Sms_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Sms_002, Function | MediumTest | Level3)
{
    auto manager = std::make_shared<HRilManager>();
    auto sms = std::make_unique<HRilSms>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, sms->SendGsmSmsResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->SendCdmaSmsResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->AddSimMessageResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->DelSimMessageResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->UpdateSimMessageResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->SetSmscAddrResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->GetSmscAddrResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->SetCBConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->GetCBConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->SetCdmaCBConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->GetCdmaCBConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->SendSmsMoreModeResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->SendSmsAckResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->AddCdmaSimMessageResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->DelCdmaSimMessageResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->UpdateCdmaSimMessageResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->SmsStatusReportNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->NewSmsNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->NewCdmaSmsNotify(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
}

/**
 * @tc.number   Telephony_HrilManager_Sms_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Sms_003, Function | MediumTest | Level3)
{
    auto sms = std::make_unique<HRilSms>(0);
    OHOS::HDI::Ril::V1_1::ServiceCenterAddress scAddress;
    sms->SetSmscAddr(0, scAddress);
    int32_t count = 300;
    std::string str(count, '1');
    scAddress.address = str;
    sms->SetSmscAddr(0, scAddress);
    std::string startMid;
    std::string endMid;
    ASSERT_FALSE(sms->SplitValue("", startMid, endMid, "-"));
    ASSERT_FALSE(sms->SplitValue("-", startMid, endMid, "-"));
    ASSERT_FALSE(sms->SplitValue("1-", startMid, endMid, "-"));
    ASSERT_TRUE(sms->SplitValue("123", startMid, endMid, "-"));
    ASSERT_TRUE(sms->SplitValue("0-1", startMid, endMid, "-"));
    std::vector<std::string> mids;
    sms->SplitMids("", mids, COMMA_STR);
    sms->SplitMids("123", mids, COMMA_STR);
    sms->SplitMids("0-1,2-3", mids, COMMA_STR);
    OHOS::HDI::Ril::V1_1::CBConfigInfo broadcastInfo;
    sms->SetCBConfig(0, broadcastInfo);
    broadcastInfo.mids = str;
    sms->SetCBConfig(0, broadcastInfo);
    broadcastInfo.mids = "0-1,2-3";
    sms->SetCBConfig(0, broadcastInfo);
    broadcastInfo.mids = "0-255";
    sms->SetCBConfig(0, broadcastInfo);
    std::vector<HRilCBConfigInfo> configInfo;
    broadcastInfo.dcss = "-";
    ASSERT_FALSE(sms->GetHRilCBConfigInfo(configInfo, broadcastInfo));
    broadcastInfo.mids = ",";
    EXPECT_EQ(sms->SetCBConfig(0, broadcastInfo), HRIL_ERR_INVALID_PARAMETER);
    ASSERT_FALSE(sms->GetHRilCBConfigInfo(configInfo, broadcastInfo));
}

/**
 * @tc.number   Telephony_HrilManager_Sms_004
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Sms_004, Function | MediumTest | Level3)
{
    auto sms = std::make_unique<HRilSms>(0);
    sms->smsFuncs_ = nullptr;
    EXPECT_NE(HDF_SUCCESS, sms->DelSimMessage(0, 0));
    EXPECT_NE(HDF_SUCCESS, sms->RequestWithStrings(0, 0, 0));
    OHOS::HDI::Ril::V1_1::CdmaCBConfigInfoList cellBroadcastInfoList;
    EXPECT_NE(HDF_SUCCESS, sms->SetCdmaCBConfig(0, cellBroadcastInfoList));
    OHOS::HDI::Ril::V1_1::ModeData modeData;
    EXPECT_NE(HDF_SUCCESS, sms->SendSmsAck(0, modeData));
    EXPECT_NE(HDF_SUCCESS, sms->DelCdmaSimMessage(0, 0));
    OHOS::HDI::Ril::V1_1::SmsMessageIOInfo info;
    int32_t count = 300;
    std::string str(count, '1');
    EXPECT_NE(HDF_SUCCESS, sms->AddSimMessage(0, info));
    EXPECT_NE(HDF_SUCCESS, sms->UpdateSimMessage(0, info));
    EXPECT_NE(HDF_SUCCESS, sms->AddCdmaSimMessage(0, info));
    EXPECT_EQ(HDF_SUCCESS, sms->UpdateCdmaSimMessage(0, info));
    info.smscPdu = str;
    EXPECT_NE(HDF_SUCCESS, sms->AddSimMessage(0, info));
    EXPECT_NE(HDF_SUCCESS, sms->UpdateSimMessage(0, info));
    info.pdu = str;
    EXPECT_NE(HDF_SUCCESS, sms->AddSimMessage(0, info));
    EXPECT_NE(HDF_SUCCESS, sms->UpdateSimMessage(0, info));
    EXPECT_EQ(HDF_SUCCESS, sms->UpdateCdmaSimMessage(0, info));
    info.pdu = "123";
    EXPECT_NE(HDF_SUCCESS, sms->AddCdmaSimMessage(0, info));
    HRilCdmaSmsMessageInfo cdmaMessageInfo;
    ASSERT_TRUE(sms->CheckCdmaPduLength(cdmaMessageInfo, str));
    ASSERT_FALSE(sms->CheckCdmaPduLength(cdmaMessageInfo, "123"));
    count = 20;
    std::string starStr(count, '*');
    ASSERT_FALSE(sms->CheckCdmaPduLength(cdmaMessageInfo, starStr));
    std::string numStrOne(count, '1');
    ASSERT_FALSE(sms->CheckCdmaPduLength(cdmaMessageInfo, numStrOne));
    count = 22;
    std::string numStrTwo(count, '1');
    ASSERT_FALSE(sms->CheckCdmaPduLength(cdmaMessageInfo, numStrTwo));
    count = 23;
    std::string numStrThree(count, '1');
    ASSERT_FALSE(sms->CheckCdmaPduLength(cdmaMessageInfo, numStrThree));
}

/**
 * @tc.number   Telephony_HrilManager_Sms_005
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Sms_005, Function | MediumTest | Level3)
{
    auto sms = std::make_unique<HRilSms>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    HRilErrNumber err = HRilErrNumber::HRIL_ERR_NULL_POINT;
    responseInfo.error = HDI::Ril::V1_1::RilErrType::RIL_ERR_GENERIC_FAILURE;
    EXPECT_NE(HDF_SUCCESS, sms->GetSmscAddrResponse(0, responseInfo, nullptr, 0));
    responseInfo.error = HDI::Ril::V1_1::RilErrType::NONE;
    EXPECT_NE(HDF_SUCCESS, sms->GetSmscAddrResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->GetCBConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->GetCdmaCBConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->SmsStatusReportNotify(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->NewSmsNotify(0, err, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, sms->NewCdmaSmsNotify(0, err, nullptr, 0));
    HRilServiceCenterAddress scAddress;
    EXPECT_NE(HDF_SUCCESS, sms->GetSmscAddrResponse(0, responseInfo, &scAddress, 0));
    EXPECT_NE(HDF_SUCCESS, sms->GetSmscAddrResponse(0, responseInfo, &scAddress, sizeof(HRilServiceCenterAddress)));
    scAddress.address = const_cast<char *>(NUMBER);
    EXPECT_NE(HDF_SUCCESS, sms->GetSmscAddrResponse(0, responseInfo, &scAddress, sizeof(HRilServiceCenterAddress)));
    HRilCBConfigInfo cellBroadcastInfo;
    EXPECT_NE(HDF_SUCCESS, sms->GetCBConfigResponse(0, responseInfo, &cellBroadcastInfo, 0));
    EXPECT_NE(HDF_SUCCESS, sms->GetCBConfigResponse(0, responseInfo, &cellBroadcastInfo, sizeof(HRilCBConfigInfo)));
    cellBroadcastInfo.endOfCodeScheme = 1;
    cellBroadcastInfo.endOfServiceId = 1;
    HDI::Ril::V1_1::CBConfigInfo configInfo;
    ASSERT_TRUE(sms->GetCBConfigInfo(&cellBroadcastInfo, 1, configInfo));
    cellBroadcastInfo.startOfCodeScheme = 2;
    ASSERT_FALSE(sms->GetCBConfigInfo(&cellBroadcastInfo, 1, configInfo));
    cellBroadcastInfo.startOfServiceId = 3;
    ASSERT_FALSE(sms->GetCBConfigInfo(&cellBroadcastInfo, 1, configInfo));
    HRilCdmaCBConfigInfo cdmaConfig;
    EXPECT_NE(HDF_SUCCESS, sms->GetCdmaCBConfigResponse(0, responseInfo, &cdmaConfig, 0));
    EXPECT_NE(HDF_SUCCESS, sms->GetCdmaCBConfigResponse(0, responseInfo, &cdmaConfig, sizeof(HRilCdmaCBConfigInfo)));
    uint8_t *data = (uint8_t *)malloc(2 * sizeof(uint8_t));
    EXPECT_NE(HDF_SUCCESS, sms->SmsStatusReportNotify(0, err, data, 0));
    EXPECT_NE(HDF_SUCCESS, sms->SmsStatusReportNotify(0, err, data, 1));
    EXPECT_NE(HDF_SUCCESS, sms->SmsStatusReportNotify(0, err, data, NUM_TWO));
    HRilSmsResponse smsResponse;
    EXPECT_NE(HDF_SUCCESS, sms->NewSmsNotify(0, err, &smsResponse, 0));
    EXPECT_NE(HDF_SUCCESS, sms->NewSmsNotify(0, err, &smsResponse, sizeof(HRilSmsResponse)));
    EXPECT_NE(HDF_SUCCESS, sms->NewCdmaSmsNotify(0, err, &smsResponse, 0));
    EXPECT_NE(HDF_SUCCESS, sms->NewCdmaSmsNotify(0, err, &smsResponse, sizeof(HRilSmsResponse)));
    smsResponse.pdu = const_cast<char *>(NUMBER);
    EXPECT_NE(HDF_SUCCESS, sms->NewSmsNotify(0, err, &smsResponse, sizeof(HRilSmsResponse)));
    EXPECT_NE(HDF_SUCCESS, sms->NewCdmaSmsNotify(0, err, &smsResponse, sizeof(HRilSmsResponse)));
}

/**
 * @tc.number   Telephony_HrilManager_Sms_006
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Sms_006, Function | MediumTest | Level3)
{
    auto sms = std::make_unique<HRilSms>(0);
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    HRilErrNumber err = HRilErrNumber::HRIL_ERR_NULL_POINT;
    responseInfo.error = HDI::Ril::V1_1::RilErrType::RIL_ERR_GENERIC_FAILURE;
    sms->MakeSendSmsResult(responseInfo, 0, nullptr, 0);
    responseInfo.error = HDI::Ril::V1_1::RilErrType::NONE;
    sms->MakeSendSmsResult(responseInfo, 0, nullptr, 0);
    EXPECT_EQ(HDF_SUCCESS, sms->NewSmsStoredOnSimNotify(1, err, nullptr, 0));
    ASSERT_FALSE(sms->RequestWithInts(nullptr, nullptr, 0, 0));
    ASSERT_FALSE(sms->RequestWithInts(nullptr, nullptr, 0, 1));
    sms->MakeCBConfigResult(nullptr, 0);
    int32_t recordNumber = 0;
    EXPECT_EQ(HDF_SUCCESS, sms->NewSmsStoredOnSimNotify(1, err, &recordNumber, 0));
    EXPECT_NE(HDF_SUCCESS, sms->NewSmsStoredOnSimNotify(1, err, &recordNumber, sizeof(int32_t)));
    HRilSmsResponse smsResponse;
    sms->MakeSendSmsResult(responseInfo, 0, &smsResponse, 0);
    sms->MakeSendSmsResult(responseInfo, 0, &smsResponse, sizeof(HRilSmsResponse));
    smsResponse.pdu = const_cast<char *>(NUMBER);
    sms->MakeSendSmsResult(responseInfo, 0, &smsResponse, sizeof(HRilSmsResponse));
    HRilCBConfigReportInfo reportInfo;
    sms->MakeCBConfigResult(&reportInfo, 0);
    sms->MakeCBConfigResult(&reportInfo, sizeof(HRilCBConfigReportInfo));
    reportInfo.data = const_cast<char *>(NUMBER);
    reportInfo.pdu = const_cast<char *>(NUMBER);
    reportInfo.dcs = const_cast<char *>(NUMBER);
    sms->MakeCBConfigResult(&reportInfo, sizeof(HRilCBConfigReportInfo));
}

/**
 * @tc.number   Telephony_HrilManager_HRilEvent_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_HRilEvent_001, Function | MediumTest | Level3)
{
    auto event = std::make_unique<HRilEvent>();
    struct timeval now;
    struct timeval timeout = { 0, MAIL_DELAY_TIME };
    event->GetNowTime(now);
    event->SetNormalDestory(true);
    ASSERT_TRUE(event->IsNormalDestory());
    ASSERT_FALSE(event->GetNextTimeOut(now));

    HRilEventMessage eventMsg = { 0 };
    event->AddTimerEvent(eventMsg, timeout);
    ASSERT_TRUE(event->GetNextTimeOut(timeout));
    struct timeval timeZero = { 0 };
    event->AddTimerEvent(eventMsg, timeZero);

    event->ProcessTimerList();
    fd_set rfds;
    event->TimerEventInit();
    eventMsg.fd = 1;
    event->AddEventMessage(eventMsg);
    event->ProcessEvents(&rfds, 1);
    event->ProcessPendingList();
}

/**
 * @tc.number   Telephony_HrilManager_HRilManager_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_HRilManager_001, Function | MediumTest | Level3)
{
    auto manager = std::make_shared<HRilManager>();
    manager->RegisterNetworkFuncs(0, nullptr);
    manager->ApplyRunningLock();
    manager->ReleaseRunningLock();
    InitRilAdapter();
    int32_t slotId = 0;
    HRilUssdNoticeInfo ussdNoticeInfo;
    ussdNoticeInfo.m = 1;
    ussdNoticeInfo.str = const_cast<char *>(NUMBER);
    struct ReportInfo report;
    report.error = static_cast<HRilErrNumber>(1);
    report.notifyId = HNOTI_CALL_USSD_REPORT;
    report.type = HRIL_NOTIFICATION;
    OnCallReport(slotId, report, (const uint8_t *)&ussdNoticeInfo, sizeof(HRilUssdNoticeInfo));
    HRilDataCallResponse dataCallResponse;
    dataCallResponse.type = const_cast<char *>(NUMBER);
    dataCallResponse.netPortName = const_cast<char *>(NUMBER);
    dataCallResponse.address = const_cast<char *>(NUMBER);
    dataCallResponse.dns = const_cast<char *>(NUMBER);
    dataCallResponse.dnsSec = const_cast<char *>(NUMBER);
    dataCallResponse.gateway = const_cast<char *>(NUMBER);
    dataCallResponse.pCscfPrimAddr = const_cast<char *>(NUMBER);
    dataCallResponse.pCscfSecAddr = const_cast<char *>(NUMBER);
    report.notifyId = HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED;
    OnDataReport(slotId, report, (const uint8_t *)&dataCallResponse, sizeof(HRilDataCallResponse));
    HRilDsdsMode dsdsMode = HRIL_DSDS_MODE_V2;
    report.notifyId = HNOTI_MODEM_DSDS_MODE_UPDATED;
    OnModemReport(slotId, report, (const uint8_t *)&dsdsMode, sizeof(HRilDsdsMode));
    const char *plmn = NUMBER;
    report.notifyId = HNOTI_NETWORK_RESIDENT_NETWORK_UPDATED;
    OnNetworkReport(slotId, report, (const uint8_t *)&plmn, sizeof(*plmn));
    const uint8_t *responseSim;
    report.notifyId = HNOTI_SIM_STK_PROACTIVE_NOTIFY;
    OnSimReport(slotId, report, responseSim, 0);
    HRilSmsResponse smsResponse;
    smsResponse.pdu = const_cast<char *>(NUMBER);
    report.notifyId = HNOTI_SMS_NEW_SMS;
    OnSmsReport(slotId, report, (const uint8_t *)&smsResponse, sizeof(HRilSmsResponse));
    manager->hrilNetwork_.clear();
    manager->RegisterNetworkFuncs(0, nullptr);
}

/**
 * @tc.number   Telephony_HrilManager_HrilBase_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_HrilBase_001, Function | MediumTest | Level3)
{
    HRilBase base { 0 };
    EXPECT_EQ(HRIL_ERR_GENERIC_FAILURE, base.ConvertHexStringToInt(nullptr, 0, 0));
    EXPECT_EQ(10, base.ConvertHexCharToInt('a'));
    EXPECT_EQ(0, base.ConvertHexCharToInt('0'));
    EXPECT_EQ(9, base.ConvertHexCharToInt('9'));
    EXPECT_EQ(HRIL_INVALID_HEX_CHAR, base.ConvertHexCharToInt('z'));
    ASSERT_TRUE(base.ConvertHexStringToBytes(nullptr, 0) == nullptr);
    ASSERT_TRUE(base.ConvertHexStringToBytes(this, 0) == nullptr);
    ASSERT_FALSE(base.ConvertToString(nullptr, ""));
    char *dest = nullptr;
    base.CopyToCharPoint(&dest, "");
    ASSERT_TRUE(base.ConvertToString(&dest, ""));
}

/**
 * @tc.number   Telephony_CreateHRilRequest_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_CreateHRilRequest_001, Function | MediumTest | Level3)
{
    auto &hrilData = HRilManager::manager_->hrilData_[0];
    EXPECT_NE(hrilData->CreateHRilRequest(0, 0), nullptr);
    HRilManager::manager_ = nullptr;
    EXPECT_EQ(hrilData->CreateHRilRequest(0, 0), nullptr);
}

} // namespace Telephony
} // namespace OHOS
