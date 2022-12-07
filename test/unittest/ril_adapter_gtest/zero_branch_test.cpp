/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include "gtest/gtest.h"
#include "hril_data.h"
#include "hril_hdf.h"
#include "hril_manager.h"
#include "hril_modem.h"
#include "hril_network.h"
#include "hril_sim.h"
#include "hril_sms.h"
#include "telephony_log_wrapper.h"
#include "v1_0/iril.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;
class BranchTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void BranchTest::SetUpTestCase() {}

void BranchTest::TearDownTestCase() {}

void BranchTest::SetUp() {}

void BranchTest::TearDown() {}

bool TestCallInterface(std::shared_ptr<HRilManager> manager)
{
    OHOS::HDI::Ril::V1_0::EmergencyInfoList emergencyInfoList;
    EXPECT_NE(HDF_SUCCESS, manager->SetEmergencyCallList(0, 0, emergencyInfoList));
    EXPECT_NE(HDF_SUCCESS, manager->GetEmergencyCallList(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetCallList(0, 0));
    OHOS::HDI::Ril::V1_0::DialInfo dialInfo;
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
    OHOS::HDI::Ril::V1_0::CallForwardSetInfo callForwardSetInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetCallTransferInfo(0, 0, callForwardSetInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetCallRestriction(0, 0, "0"));
    OHOS::HDI::Ril::V1_0::CallRestrictionInfo callRestrictionInfo;
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
    OHOS::HDI::Ril::V1_0::DtmfInfo info;
    EXPECT_NE(HDF_SUCCESS, manager->SendDtmf(0, 0, info));
    EXPECT_NE(HDF_SUCCESS, manager->StartDtmf(0, 0, info));
    EXPECT_NE(HDF_SUCCESS, manager->StopDtmf(0, 0, info));
    const OHOS::HDI::Ril::V1_0::SetBarringInfo setBarringInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetBarringPassword(0, 0, setBarringInfo));
    return true;
}

bool TestDataInterface(std::shared_ptr<HRilManager> manager)
{
    OHOS::HDI::Ril::V1_0::DataCallInfo dataCallInfo;
    EXPECT_NE(HDF_SUCCESS, manager->ActivatePdpContext(0, 0, dataCallInfo));
    OHOS::HDI::Ril::V1_0::UniInfo uniInfo;
    EXPECT_NE(HDF_SUCCESS, manager->DeactivatePdpContext(0, 0, uniInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetPdpContextList(0, 0, uniInfo));
    OHOS::HDI::Ril::V1_0::DataProfileDataInfo dataProfileDataInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetInitApnInfo(0, 0, dataProfileDataInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetLinkBandwidthInfo(0, 0, 0));
    OHOS::HDI::Ril::V1_0::DataLinkBandwidthReportingRule dataLinkBandwidthReportingRule;
    EXPECT_NE(HDF_SUCCESS, manager->SetLinkBandwidthReportingRule(0, 0, dataLinkBandwidthReportingRule));
    EXPECT_NE(HDF_SUCCESS, manager->SetDataPermitted(0, 0, 0));
    OHOS::HDI::Ril::V1_0::DataProfilesInfo dataProfilesInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetDataProfileInfo(0, 0, dataProfilesInfo));
    OHOS::HDI::Ril::V1_0::DataPerformanceInfo dataPerformanceInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SendDataPerformanceMode(0, 0, dataPerformanceInfo));
    OHOS::HDI::Ril::V1_0::DataSleepInfo dataSleepInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SendDataSleepMode(0, 0, dataSleepInfo));
    return true;
}

bool TestModemInterface(std::shared_ptr<HRilManager> manager)
{
    EXPECT_NE(HDF_SUCCESS, manager->SetRadioState(0, 0, 0, 1));
    EXPECT_NE(HDF_SUCCESS, manager->GetRadioState(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetImei(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetMeid(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetVoiceRadioTechnology(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetBasebandVersion(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->ShutDown(0, 0));
    return true;
}

bool TestSimInterface(std::shared_ptr<HRilManager> manager)
{
    OHOS::HDI::Ril::V1_0::SimIoRequestInfo simIO;
    EXPECT_NE(HDF_SUCCESS, manager->GetSimIO(0, 0, simIO));
    EXPECT_NE(HDF_SUCCESS, manager->GetSimStatus(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetImsi(0, 0));
    OHOS::HDI::Ril::V1_0::SimLockInfo simLockInfo;
    EXPECT_NE(HDF_SUCCESS, manager->GetSimLockStatus(0, 0, simLockInfo));
    EXPECT_NE(HDF_SUCCESS, manager->SetSimLock(0, 0, simLockInfo));
    OHOS::HDI::Ril::V1_0::SimPasswordInfo simPassword;
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
    HDI::Ril::V1_0::RadioProtocol radioProtocol;
    EXPECT_NE(HDF_SUCCESS, manager->SetRadioProtocol(0, 0, radioProtocol));
    EXPECT_NE(HDF_SUCCESS, manager->SimOpenLogicalChannel(0, 0, "0", 0));
    EXPECT_NE(HDF_SUCCESS, manager->SimCloseLogicalChannel(0, 0, 0));
    OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo apduSimIO;
    EXPECT_NE(HDF_SUCCESS, manager->SimTransmitApduLogicalChannel(0, 0, apduSimIO));
    EXPECT_NE(HDF_SUCCESS, manager->SimTransmitApduBasicChannel(0, 0, apduSimIO));
    OHOS::HDI::Ril::V1_0::SimAuthenticationRequestInfo simAuthInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SimAuthentication(0, 0, simAuthInfo));
    EXPECT_NE(HDF_SUCCESS, manager->UnlockSimLock(0, 0, 0, ""));
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
    HDI::Ril::V1_0::SetNetworkModeInfo networkModeInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetNetworkSelectionMode(0, 0, networkModeInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetNeighboringCellInfoList(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetCurrentCellInfo(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetPreferredNetwork(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetPreferredNetwork(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->GetPhysicalChannelConfig(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetLocateUpdates(0, 0, static_cast<HDI::Ril::V1_0::RilRegNotifyMode>(0)));
    EXPECT_NE(HDF_SUCCESS, manager->SetNotificationFilter(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SetDeviceState(0, 0, 0, 0));
    return true;
}

bool TestSmsInterface(std::shared_ptr<HRilManager> manager)
{
    OHOS::HDI::Ril::V1_0::GsmSmsMessageInfo gsmSmsMessageInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SendGsmSms(0, 0, gsmSmsMessageInfo));
    OHOS::HDI::Ril::V1_0::SendCdmaSmsMessageInfo cdmaSmsMessageInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SendCdmaSms(0, 0, cdmaSmsMessageInfo));
    OHOS::HDI::Ril::V1_0::SmsMessageIOInfo smsMessageIOInfo;
    EXPECT_NE(HDF_SUCCESS, manager->AddSimMessage(0, 0, smsMessageIOInfo));
    EXPECT_NE(HDF_SUCCESS, manager->DelSimMessage(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->UpdateSimMessage(0, 0, smsMessageIOInfo));
    EXPECT_NE(HDF_SUCCESS, manager->AddCdmaSimMessage(0, 0, smsMessageIOInfo));
    EXPECT_NE(HDF_SUCCESS, manager->DelCdmaSimMessage(0, 0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->UpdateCdmaSimMessage(0, 0, smsMessageIOInfo));
    OHOS::HDI::Ril::V1_0::ServiceCenterAddress serviceCenterAddress;
    EXPECT_NE(HDF_SUCCESS, manager->SetSmscAddr(0, 0, serviceCenterAddress));
    EXPECT_NE(HDF_SUCCESS, manager->GetSmscAddr(0, 0));
    OHOS::HDI::Ril::V1_0::CBConfigInfo cellBroadcastInfo;
    EXPECT_NE(HDF_SUCCESS, manager->SetCBConfig(0, 0, cellBroadcastInfo));
    EXPECT_NE(HDF_SUCCESS, manager->GetCBConfig(0, 0));
    OHOS::HDI::Ril::V1_0::CdmaCBConfigInfoList cdmaCBConfigInfoList;
    EXPECT_NE(HDF_SUCCESS, manager->SetCdmaCBConfig(0, 0, cdmaCBConfigInfoList));
    EXPECT_NE(HDF_SUCCESS, manager->GetCdmaCBConfig(0, 0));
    EXPECT_NE(HDF_SUCCESS, manager->SendSmsMoreMode(0, 0, gsmSmsMessageInfo));
    OHOS::HDI::Ril::V1_0::ModeData modeData;
    EXPECT_NE(HDF_SUCCESS, manager->SendSmsAck(0, 0, modeData));
    return true;
}

/**
 * @tc.number   Telephony_HrilManager_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Call_001, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    manager->hrilCall_.clear();
    std::unique_ptr<HRilCall> call;
    manager->hrilCall_.push_back(std::move(call));
    EXPECT_EQ(true, TestCallInterface(manager));
}

/**
 * @tc.number  Telephony_HrilManager_Call_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Call_002, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    auto call = std::make_unique<HRilCall>(0, *manager);
    HRilRadioResponseInfo responseInfo;
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
}

/**
 * @tc.number   Telephony_HrilManager_Data_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Data_001, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    manager->hrilData_.clear();
    std::unique_ptr<HRilData> data;
    manager->hrilData_.push_back(std::move(data));
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
    auto data = std::make_unique<HRilData>(0, *manager);
    HRilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, data->ActivatePdpContextResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->DeactivatePdpContextResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->GetPdpContextListResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->SetInitApnInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->PdpContextListUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->GetLinkBandwidthInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->SetLinkBandwidthReportingRuleResponse(0, responseInfo, nullptr, 0));
    EXPECT_EQ(HDF_SUCCESS, data->SetDataProfileInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, data->SetDataPermittedResponse(0, responseInfo, nullptr, 0));
}

/**
 * @tc.number   Telephony_HrilManager_Modem_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Modem_001, Function | MediumTest | Level2)
{
    auto manager = std::make_shared<HRilManager>();
    manager->hrilModem_.clear();
    std::unique_ptr<HRilModem> data;
    manager->hrilModem_.push_back(std::move(data));
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
    auto modem = std::make_unique<HRilModem>(0, *manager);
    HRilRadioResponseInfo responseInfo;
    EXPECT_NE(HDF_SUCCESS, modem->ShutDownResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->SetRadioStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetRadioStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetImeiResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetMeidResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetVoiceRadioTechnologyResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->GetBasebandVersionResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->RadioStateUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, modem->VoiceRadioTechUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
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
    auto sim = std::make_unique<HRilSim>(0, *manager);
    HRilRadioResponseInfo responseInfo;
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
    auto network = std::make_unique<HRilNetwork>(0, *manager);
    HRilRadioResponseInfo responseInfo;
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
    EXPECT_NE(HDF_SUCCESS, network->GetCurrentCellInfoResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->GetPhysicalChannelConfigResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SetLocateUpdatesResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SetNotificationFilterResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SetDeviceStateResponse(0, responseInfo, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCsRegStatusUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkPsRegStatusUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->SignalStrengthUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkTimeUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkTimeZoneUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkPhyChnlCfgUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
    EXPECT_NE(HDF_SUCCESS, network->NetworkCurrentCellUpdated(0, HRilErrNumber::HRIL_ERR_NULL_POINT, nullptr, 0));
}

/**
 * @tc.number   Telephony_HrilManager_Sms_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_Sms_001, Function | MediumTest | Level3)
{
    auto manager = std::make_shared<HRilManager>();
    manager->hrilSms_.clear();
    std::unique_ptr<HRilSms> data;
    manager->hrilSms_.push_back(std::move(data));
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
    auto sms = std::make_unique<HRilSms>(0, *manager);
    HRilRadioResponseInfo responseInfo;
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
 * @tc.number   Telephony_HrilManager_Sms_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_HrilManager_HrilBase_001, Function | MediumTest | Level3)
{
    HRilManager manager;
    HRilBase base { 0, manager };
    EXPECT_EQ(HRIL_ERR_GENERIC_FAILURE, base.ConvertHexStringToInt(nullptr, 0, 0));
    EXPECT_EQ(10, base.ConvertHexCharToInt('a'));
    EXPECT_EQ(0, base.ConvertHexCharToInt('0'));
    EXPECT_EQ(HRIL_INVALID_HEX_CHAR, base.ConvertHexCharToInt('z'));
    ASSERT_TRUE(base.ConvertHexStringToBytes(nullptr, 0) == nullptr);
    ASSERT_TRUE(base.ConvertHexStringToBytes(this, 0) == nullptr);
    ASSERT_FALSE(base.ConvertToString(nullptr, ""));
    char *dest = nullptr;
    ASSERT_TRUE(base.ConvertToString(&dest, ""));
}

} // namespace Telephony
} // namespace OHOS
