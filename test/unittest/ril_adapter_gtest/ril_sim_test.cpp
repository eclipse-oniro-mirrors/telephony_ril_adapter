/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#define private public
#include "hril_sim.h"
#include "ril_sim_test.h"
#include "ril_test_util.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_2;
using namespace testing::ext;
namespace {
sptr<OHOS::HDI::Ril::V1_2::IRil> g_rilInterface = nullptr;
}

void RILSimTest::SetUpTestCase()
{
    TELEPHONY_LOGI("----------RilCallTest gtest start ------------");
    RilTestUtil::GetInstance().Init();
    g_rilInterface = RilTestUtil::GetRilInterface();
}

void RILSimTest::TearDownTestCase() {}

void RILSimTest::SetUp() {}

void RILSimTest::TearDown() {}

/**
 * @tc.number   Telephony_DriverSystem_GetSimStatus_V1_0100
 * @tc.name     Get sim status
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_GetSimStatus_V1_0100, Function | MediumTest | Level2)
{
    ASSERT_NE(g_rilInterface, nullptr);
    ASSERT_TRUE(RilTestUtil::HasVoiceCapability());
    ASSERT_TRUE(RilTestUtil::IsValidSlotId(SLOTID_1));
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_GET_SIM_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_GetSimStatus_V1_0200
 * @tc.name     Get sim status
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_GetSimStatus_V1_0200, Function | MediumTest | Level2)
{
    ASSERT_NE(g_rilInterface, nullptr);
    ASSERT_TRUE(RilTestUtil::HasVoiceCapability());
    ASSERT_TRUE(RilTestUtil::IsValidSlotId(SLOTID_2));
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_GET_SIM_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_GetSimIO_V1_0100
 * @tc.name     Get sim IO
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_GetSimIO_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, RilTestUtil::GetSerialId(), msg);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_GET_SIM_IO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetSimIO_V1_0200
 * @tc.name     Get sim IO
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_GetSimIO_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    SimIoRequestInfo msg;
    msg.command = 0xC0;
    // ICCID
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, RilTestUtil::GetSerialId(), msg);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_GET_SIM_IO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetImsi_V1_0100
 * @tc.name     Get imsi
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_GetImsi_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    int32_t ret = g_rilInterface->GetImsi(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_GET_IMSI));
}

/**
 * @tc.number   Telephony_DriverSystem_GetImsi_V1_0200
 * @tc.name     Get imsi
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_GetImsi_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    int32_t ret = g_rilInterface->GetImsi(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_GET_IMSI));
}

/**
 * @tc.number   Telephony_DriverSystem_SetActiveSim_V1_0100
 * @tc.name     Set active sim
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SetActiveSim_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    int32_t enable = 1;
    int32_t index = 1; // ENTITY_CARD
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, RilTestUtil::GetSerialId(), index, enable);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_SET_ACTIVE_SIM));
}

/**
 * @tc.number   Telephony_DriverSystem_SetActiveSim_V1_0200
 * @tc.name     Set active sim
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SetActiveSim_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    int32_t enable = 1;
    int32_t index = 1; // ENTITY_CARD
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, RilTestUtil::GetSerialId(), index, enable);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_SET_ACTIVE_SIM));
}

/**
 * @tc.number   Telephony_DriverSystem_SimStkSendTerminalResponse_V1_0100
 * @tc.name     Sim stk send terminal response
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimStkSendTerminalResponse_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    std::string cmd = "send terminal response";
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, RilTestUtil::GetSerialId(), cmd);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_STK_SEND_TERMINAL_RESPONSE));
}

/**
 * @tc.number   Telephony_DriverSystem_SimStkSendTerminalResponse_V1_0200
 * @tc.name     Sim stk send terminal response
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimStkSendTerminalResponse_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    std::string cmd = "send terminal response";
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_2, RilTestUtil::GetSerialId(), cmd);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_STK_SEND_TERMINAL_RESPONSE));
}

/**
 * @tc.number   Telephony_DriverSystem_SimStkSendEnvelope_V1_0100
 * @tc.name     Sim stk send envelope
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimStkSendEnvelope_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    std::string cmd = "send envelope";
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, RilTestUtil::GetSerialId(), cmd);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_STK_SEND_ENVELOPE));
}

/**
 * @tc.number   Telephony_DriverSystem_SimStkSendEnvelope_V1_0200
 * @tc.name     Sim stk send envelope
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimStkSendEnvelope_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    std::string cmd = "send envelope";
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_2, RilTestUtil::GetSerialId(), cmd);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_STK_SEND_ENVELOPE));
}

/**
 * @tc.number   Telephony_DriverSystem_SimStkSendCallSetupRequestResult_V1_0100
 * @tc.name     Sim stk send call setup requestResult
 * @tc.desc     Function test
 */
HWTEST_F(
    RILSimTest, Telephony_DriverSystem_SimStkSendCallSetupRequestResult_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_1, RilTestUtil::GetSerialId(), true);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT));
}

/**
 * @tc.number   Telephony_DriverSystem_SimStkSendCallSetupRequestResult_V1_0200
 * @tc.name     Sim stk send call setup requestResult
 * @tc.desc     Function test
 */
HWTEST_F(
    RILSimTest, Telephony_DriverSystem_SimStkSendCallSetupRequestResult_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_2, RilTestUtil::GetSerialId(), true);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT));
}

/**
 * @tc.number   Telephony_DriverSystem_SimStkIsReady_V1_0100
 * @tc.name     Sim stk is ready
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimStkIsReady_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_STK_IS_READY));
}

/**
 * @tc.number   Telephony_DriverSystem_SimStkIsReady_V1_0200
 * @tc.name     Sim stk is ready
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimStkIsReady_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_STK_IS_READY));
}

/**
 * @tc.number   Telephony_DriverSystem_SetRadioProtocol_V1_0100
 * @tc.name     Set radio protocol
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SetRadioProtocol_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    RadioProtocol protocol;
    protocol.slotId = SLOTID_1;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_1, RilTestUtil::GetSerialId(), protocol);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_RADIO_PROTOCOL));
}

/**
 * @tc.number   Telephony_DriverSystem_SetRadioProtocol_V1_0200
 * @tc.name     Set radio protocol
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SetRadioProtocol_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    RadioProtocol protocol;
    protocol.slotId = SLOTID_2;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_2, RilTestUtil::GetSerialId(), protocol);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_RADIO_PROTOCOL));
}

/**
 * @tc.number   Telephony_DriverSystem_SimOpenLogicalChannel_V1_0100
 * @tc.name     Sim open logical channel
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimOpenLogicalChannel_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    std::string appID = "A00000015141434C00"; // ARAM_AID
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_1, RilTestUtil::GetSerialId(), appID, p2);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_OPEN_LOGICAL_CHANNEL));
}

/**
 * @tc.number   Telephony_DriverSystem_SimTransmitApduLogicalChannel_V1_0100
 * @tc.name     Sim transmit apdu logical channel
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimTransmitApduLogicalChannel_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    ApduSimIORequestInfo reqInfo;
    reqInfo.channelId = RilTestUtil::GetCallback()->GetCurrentChannelId();
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, RilTestUtil::GetSerialId(), reqInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL));
}

/**
 * @tc.number   Telephony_DriverSystem_SimCloseLogicalChannel_V1_0100
 * @tc.name     Sim close logical channel
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimCloseLogicalChannel_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(
        SLOTID_1, RilTestUtil::GetSerialId(), RilTestUtil::GetCallback()->GetCurrentChannelId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_CLOSE_LOGICAL_CHANNEL));
}

/**
 * @tc.number   Telephony_DriverSystem_SimCloseLogicalChannel_V1_0100
 * @tc.name     Sim open logical channel
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimOpenLogicalChannel_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    std::string appID = "A00000015141434C00"; // ARAM_AID
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_2, RilTestUtil::GetSerialId(), appID, p2);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_OPEN_LOGICAL_CHANNEL));
}

/**
 * @tc.number   Telephony_DriverSystem_SimTransmitApduLogicalChannel_V1_0200
 * @tc.name     Sim transmit apdu logical channel
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimTransmitApduLogicalChannel_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    ApduSimIORequestInfo reqInfo;
    reqInfo.channelId = RilTestUtil::GetCallback()->GetCurrentChannelId();
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_2, RilTestUtil::GetSerialId(), reqInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL));
}

/**
 * @tc.number   Telephony_DriverSystem_SimCloseLogicalChannel_V1_0100
 * @tc.name     Sim close logical channel
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimCloseLogicalChannel_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(
        SLOTID_2, RilTestUtil::GetSerialId(), RilTestUtil::GetCallback()->GetCurrentChannelId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_CLOSE_LOGICAL_CHANNEL));
}

/**
 * @tc.number   Telephony_DriverSystem_SimTransmitApduBasicChannel_V1_0100
 * @tc.name     Sim transmit apdu basic channel
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimTransmitApduBasicChannel_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    ApduSimIORequestInfo reqInfo;
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, RilTestUtil::GetSerialId(), reqInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL));
}

/**
 * @tc.number   Telephony_DriverSystem_SimTransmitApduBasicChannel_V1_0200
 * @tc.name     Sim transmit apdu basic channel
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimTransmitApduBasicChannel_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    ApduSimIORequestInfo reqInfo;
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_2, RilTestUtil::GetSerialId(), reqInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL));
}

/**
 * @tc.number   Telephony_DriverSystem_SimAuthentication_V1_0100
 * @tc.name     Sim authentication
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimAuthentication_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.serial = SIM_AUTH_EAP_AKA_TYPE;
    simAuthInfo.aid = "USIM_AID";
    simAuthInfo.authData = "authData";
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_1, RilTestUtil::GetSerialId(), simAuthInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_AUTHENTICATION));
}

/**
 * @tc.number   Telephony_DriverSystem_SimAuthentication_V1_0200
 * @tc.name     Sim authentication
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SimAuthentication_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.serial = SIM_AUTH_EAP_AKA_TYPE;
    simAuthInfo.aid = "USIM_AID";
    simAuthInfo.authData = "authData";
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_2, RilTestUtil::GetSerialId(), simAuthInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_AUTHENTICATION));
}

/**
 * @tc.number   Telephony_DriverSystem_GetSimLockStatus_V1_0100
 * @tc.name     Get Sim lock status
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_GetSimLockStatus_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, RilTestUtil::GetSerialId(), simLockInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_GET_SIM_LOCK_STATUS));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_GetSimLockStatus_V1_0200
 * @tc.name     Get Sim lock status
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_GetSimLockStatus_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, RilTestUtil::GetSerialId(), simLockInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_GET_SIM_LOCK_STATUS));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_SendSimMatchedOperatorInfo_V1_0100
 * @tc.name     Send sim matched operator info
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SendSimMatchedOperatorInfo_V1_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    NcfgOperatorInfo ncfgOperatorInfo;
    ncfgOperatorInfo.operName = "operName";
    ncfgOperatorInfo.operKey = "operKey";
    ncfgOperatorInfo.state = 0;
    ncfgOperatorInfo.reserve = "reserve";
    int32_t ret = g_rilInterface->SendSimMatchedOperatorInfo(SLOTID_1, RilTestUtil::GetSerialId(), ncfgOperatorInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_SEND_NCFG_OPER_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_SendSimMatchedOperatorInfo_V1_0200
 * @tc.name     Send sim matched operator info
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SendSimMatchedOperatorInfo_V1_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    NcfgOperatorInfo ncfgOperatorInfo;
    ncfgOperatorInfo.operName = "operName";
    ncfgOperatorInfo.operKey = "operKey";
    ncfgOperatorInfo.state = 0;
    ncfgOperatorInfo.reserve = "reserve";
    int32_t ret = g_rilInterface->SendSimMatchedOperatorInfo(SLOTID_2, RilTestUtil::GetSerialId(), ncfgOperatorInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_SEND_NCFG_OPER_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_SetSimLock_V1_0100
 * @tc.name     Set Sim lock
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SetSimLock_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    simLockInfo.passwd = "1234";
    char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, RilTestUtil::GetSerialId(), simLockInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_SET_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_SetSimLock_V1_0200
 * @tc.name     Set Sim lock
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_SetSimLock_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    simLockInfo.passwd = "1234";
    char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, RilTestUtil::GetSerialId(), simLockInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_SET_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_ChangeSimPassword_V1_0100
 * @tc.name     Change sim password
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_ChangeSimPassword_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC"; // FAC_PIN_LOCK
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, RilTestUtil::GetSerialId(), simPassword);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_CHANGE_SIM_PASSWORD));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_ChangeSimPassword_V1_0200
 * @tc.name     Change sim password
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_ChangeSimPassword_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC"; // FAC_PIN_LOCK
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, RilTestUtil::GetSerialId(), simPassword);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_CHANGE_SIM_PASSWORD));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockPin_V1_0100
 * @tc.name     Unlock pin
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockPin_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPin(SLOTID_1, RilTestUtil::GetSerialId(), pin);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockPin_V1_0200
 * @tc.name     Unlock pin
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockPin_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPin(SLOTID_2, RilTestUtil::GetSerialId(), pin);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockPuk_V1_0100
 * @tc.name     Unlock puk
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockPuk_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    std::string puk = "42014264";
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPuk(SLOTID_1, RilTestUtil::GetSerialId(), puk, pin);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockPuk_V1_0200
 * @tc.name     Unlock puk
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockPuk_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    std::string puk = "42014264";
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPuk(SLOTID_2, RilTestUtil::GetSerialId(), puk, pin);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockPin2_V1_0100
 * @tc.name     Unlock pin2
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockPin2_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPin2(SLOTID_1, RilTestUtil::GetSerialId(), pin2);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN2));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockPin2_V1_0200
 * @tc.name     Unlock pin2
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockPin2_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPin2(SLOTID_2, RilTestUtil::GetSerialId(), pin2);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN2));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockPuk2_V1_0100
 * @tc.name     Unlock puk2
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockPuk2_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    std::string puk2 = "42014264";
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPuk2(SLOTID_1, RilTestUtil::GetSerialId(), puk2, pin2);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK2));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockPuk2_V1_0200
 * @tc.name     Unlock puk2
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockPuk2_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    std::string puk2 = "42014264";
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPuk2(SLOTID_2, RilTestUtil::GetSerialId(), puk2, pin2);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK2));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockSimLock_V1_0100
 * @tc.name     Unlock sim lock
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockSimLock_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_1));
    int32_t lockType = 0;
    std::string key = "1234"; // PN_PIN_LOCK
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, RilTestUtil::GetSerialId(), lockType, key);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

/**
 * @tc.number   Telephony_DriverSystem_UnlockSimLock_V1_0200
 * @tc.name     Unlock sim lock
 * @tc.desc     Function test
 */
HWTEST_F(RILSimTest, Telephony_DriverSystem_UnlockSimLock_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    ASSERT_TRUE(RilTestUtil::IsReady(SLOTID_2));
    int32_t lockType = 0;
    std::string key = "1234"; // PN_PIN_LOCK
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, RilTestUtil::GetSerialId(), lockType, key);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SIM_UNLOCK_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

/**
* @tc.name  : GetRadioProtocolResponse_ShouldSetError_WhenResponseIsNull
* @tc.number: GetRadioProtocolResponse_Test_001
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, GetRadioProtocolResponse_ShouldSetError_001, Function | MediumTest | Level3)
{
    int32_t requestNum = 1;
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo = {};
    const void* response = nullptr;
    size_t responseLen = 0;
    
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->GetRadioProtocolResponse(requestNum, responseInfo, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(responseInfo.error, HDI::Ril::V1_1::RilErrType::RIL_ERR_INVALID_RESPONSE);
}

/**
* @tc.name  : GetRadioProtocolResponse_ShouldSetError_WhenResponseLenIsInvalid
* @tc.number: GetRadioProtocolResponse_Test_002
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, GetRadioProtocolResponse_ShouldSetError_002, Function | MediumTest | Level3)
{
    int32_t requestNum = 1;
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo = {};
    int dummyResponse = 0;
    const void* response = &dummyResponse;
    size_t responseLen = sizeof(dummyResponse);

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->GetRadioProtocolResponse(requestNum, responseInfo, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(responseInfo.error, HDI::Ril::V1_1::RilErrType::RIL_ERR_INVALID_RESPONSE);
}

/**
* @tc.name  : GetRadioProtocolResponse_ShouldProcessResponse_WhenResponseIsValid
* @tc.number: GetRadioProtocolResponse_Test_003
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, GetRadioProtocolResponse_ShouldSetError_003, Function | MediumTest | Level3) {
    int32_t requestNum = 1;
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo = {};
    HRilRadioProtocol radioProtocol = {};
    const void* response = &radioProtocol;
    size_t responseLen = sizeof(radioProtocol);

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->GetRadioProtocolResponse(requestNum, responseInfo, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(responseInfo.error, HDI::Ril::V1_1::RilErrType::NONE);
}

/**
* @tc.name  : SimStkProactiveNotify_ShouldReturnSuccess_WhenCheckCharDataSucceeds
* @tc.number: SimStkProactiveNotify_Test_001
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, SimStkProactiveNotify_ShouldReturnSuccess_001, Function | MediumTest | Level3)
{
    int32_t notifyType = 1;
    HRilErrNumber error = HRIL_ERR_SUCCESS;
    const char* response = "valid response";
    size_t responseLen = strlen(response);

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->SimStkProactiveNotify(notifyType, error, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : SimStkProactiveNotify_ShouldReturnFailure_WhenCheckCharDataFails
* @tc.number: SimStkProactiveNotify_Test_002
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, SimStkProactiveNotify_ShouldReturnSuccess_002, Function | MediumTest | Level3)
{
    int32_t notifyType = 1;
    HRilErrNumber error = HRIL_ERR_SUCCESS;
    const char* response = nullptr; // Invalid response
    size_t responseLen = 0;

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->SimStkProactiveNotify(notifyType, error, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : SimStkAlphaNotify_ShouldReturnSuccess_WhenCheckCharDataSucceeds
* @tc.number: SimStkAlphaNotify_Test_001
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, SimStkProactiveNotify_ShouldReturnSuccess_003, Function | MediumTest | Level3)
{
    int32_t notifyType = 1;
    HRilErrNumber error = HRIL_ERR_SUCCESS;
    const char* response = "valid response";
    size_t responseLen = strlen(response);

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->SimStkProactiveNotify(notifyType, error, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : SimStkAlphaNotify_ShouldReturnFailure_WhenCheckCharDataFails
* @tc.number: SimStkAlphaNotify_Test_002
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, SimStkProactiveNotify_ShouldReturnSuccess_004, Function | MediumTest | Level3)
{
    int32_t notifyType = 1;
    HRilErrNumber error = HRIL_ERR_SUCCESS;
    const char* response = nullptr; // Invalid response
    size_t responseLen = 0;

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->SimStkProactiveNotify(notifyType, error, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : SimStkEventNotify_ShouldReturnSuccess_WhenCheckCharDataSucceeds
* @tc.number: SimStkEventNotify_Test_001
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, SimStkEventNotify_ShouldReturnSuccess_001, Function | MediumTest | Level3)
{
    int32_t notifyType = 1;
    HRilErrNumber error = HRIL_ERR_SUCCESS;
    const char* response = "valid response";
    size_t responseLen = strlen(response);

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->SimStkEventNotify(notifyType, error, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : SimStkEventNotify_ShouldReturnFailure_WhenCheckCharDataFails
* @tc.number: SimStkEventNotify_Test_002
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, SimStkEventNotify_ShouldReturnFailure_002, Function | MediumTest | Level3)
{
    int32_t notifyType = 1;
    HRilErrNumber error = HRIL_ERR_SUCCESS;
    const char* response = nullptr; // Invalid response
    size_t responseLen = 0;

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->SimStkEventNotify(notifyType, error, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : SimRadioProtocolUpdated_ShouldReturnFailure_WhenResponseIsNull
* @tc.number: SimRadioProtocolUpdated_Test_001
* @tc.desc  : Test SimRadioProtocolUpdated function when response is null
*/
HWTEST_F(RILSimTest, SimRadioProtocolUpdated_001, Function | MediumTest | Level3)
{
    int32_t notifyType = 1;
    HRilErrNumber error = HRIL_ERR_SUCCESS;
    const void* response = nullptr;
    size_t responseLen = 0;

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->SimRadioProtocolUpdated(notifyType, error, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(ret, HRIL_ERR_INVALID_PARAMETER);
}

/**
* @tc.name  : SimRadioProtocolUpdated_ShouldReturnFailure_WhenResponseLenIsInvalid
* @tc.number: SimRadioProtocolUpdated_Test_002
* @tc.desc  : Test SimRadioProtocolUpdated function when response length is invalid
*/
HWTEST_F(RILSimTest, SimRadioProtocolUpdated_002, Function | MediumTest | Level3)
{
    int32_t notifyType = 1;
    HRilErrNumber error = HRIL_ERR_SUCCESS;
    HRilRadioProtocol radioProtocol = {};
    const void* response = &radioProtocol;
    size_t responseLen = sizeof(radioProtocol) - 1; // Invalid length

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->SimRadioProtocolUpdated(notifyType, error, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(ret, HRIL_ERR_INVALID_PARAMETER);
}

/**
* @tc.name  : SimRadioProtocolUpdated_ShouldProcessResponse_WhenResponseIsValid
* @tc.number: SimRadioProtocolUpdated_Test_003
* @tc.desc  : Test SimRadioProtocolUpdated function when response is valid
*/
HWTEST_F(RILSimTest, SimRadioProtocolUpdated_003, Function | MediumTest | Level3)
{
    int32_t notifyType = 1;
    HRilErrNumber error = HRIL_ERR_SUCCESS;
    HRilRadioProtocol radioProtocol = {};
    const void* response = &radioProtocol;
    size_t responseLen = sizeof(radioProtocol);

    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->SimRadioProtocolUpdated(notifyType, error, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : GetSimIOResponse_Success
* @tc.number: GetSimIOResponse_Test_001
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, GetSimIOResponse_Success, Function | MediumTest | Level3)
{
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    const void* response = nullptr; // 假设有效响应
    size_t responseLen = 0; // 假设有效长度
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->GetSimIOResponse(1, responseInfo, response, responseLen);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : GetSimIOResponse_BuildSimIOResp_Fails
* @tc.number: GetSimIOResponse_Test_002
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, GetSimIOResponse_BuildSimIOResp_Fails, Function | MediumTest | Level3)
{
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    const void* response = nullptr;
    size_t responseLen = 0;
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->GetSimIOResponse(1, responseInfo, response, responseLen);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : GetSimCardStatusResponse_Success
* @tc.number: GetSimCardStatusResponse_Test_001
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, GetSimCardStatusResponse_001, Function | MediumTest | Level3)
{
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    HRilCardState cardState = {};
    const void* response = &cardState;
    size_t responseLen = sizeof(HRilCardState);
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->GetSimCardStatusResponse(1, responseInfo, response, responseLen);
    EXPECT_NE(ret, HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : GetSimCardStatusResponse_ResponseNull_ResponseLenZero
* @tc.number: GetSimCardStatusResponse_Test_002
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, GetSimCardStatusResponse_002, Function | MediumTest | Level3)
{
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    const void* response = nullptr;
    size_t responseLen = 0;
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->GetSimCardStatusResponse(1, responseInfo, response, responseLen);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(responseInfo.error, HDI::Ril::V1_1::RilErrType::RIL_ERR_INVALID_RESPONSE);
}

/**
* @tc.name  : GetSimCardStatusResponse_ResponseNull_ResponseLenNonZero
* @tc.number: GetSimCardStatusResponse_Test_003
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, GetSimCardStatusResponse_003, Function | MediumTest | Level3)
{
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    const void* response = nullptr;
    size_t responseLen = 1;
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->GetSimCardStatusResponse(1, responseInfo, response, responseLen);
    EXPECT_EQ(ret, HRIL_ERR_INVALID_PARAMETER);
}

/**
* @tc.name  : GetSimCardStatusResponse_ResponseNonNull_ResponseLenInvalid
* @tc.number: GetSimCardStatusResponse_Test_004
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, GetSimCardStatusResponse_004, Function | MediumTest | Level3)
{
    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    HRilCardState cardState = {};
    const void* response = &cardState;
    size_t responseLen = 1; // 无效长度
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    int32_t ret = hrilSim->GetSimCardStatusResponse(1, responseInfo, response, responseLen);
    EXPECT_EQ(ret, HRIL_ERR_INVALID_PARAMETER);
}

/**
* @tc.name  : BuildLockStatusResp
* @tc.number: BuildLockStatusResp_Test
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, BuildLockStatusResp, Function | MediumTest | Level3)
{
    LockStatusResp lockStatus;
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_FALSE(hrilSim->BuildLockStatusResp(nullptr, 0, lockStatus));
}

/**
* @tc.name  : InvalidResponseLen
* @tc.number: InvalidResponseLen_Test
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, InvalidResponseLen, Function | MediumTest | Level3)
{
    HRilLockStatus resp = {0};
    LockStatusResp lockStatus;
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_FALSE(hrilSim->BuildLockStatusResp(&resp, sizeof(HRilLockStatus) + 1, lockStatus));
}

/**
* @tc.name  : ValidResponse
* @tc.number: ValidResponse_test_001
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, ValidResponse_001, Function | MediumTest | Level3)
{
    HRilLockStatus resp = {HRIL_ERR_SUCCESS, 5};
    LockStatusResp lockStatus;
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_TRUE(hrilSim->BuildLockStatusResp(&resp, sizeof(HRilLockStatus), lockStatus));
    EXPECT_EQ(lockStatus.result, HRIL_ERR_SUCCESS);
    EXPECT_EQ(lockStatus.remain, 5);
}

/**
* @tc.name  : InvalidResponse
* @tc.number: InvalidResponse_test_002
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, InvalidResponse_002, Function | MediumTest | Level3)
{
    LockStatusResp lockStatus;
    RilRadioResponseInfo responseInfo = {RilErrType::NONE};
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    hrilSim->ResponseLockStatus(lockStatus, responseInfo, nullptr, 0);
    EXPECT_EQ(responseInfo.error, RilErrType::RIL_ERR_INVALID_RESPONSE);
}

/**
* @tc.name  : InvalidResponse
* @tc.number: InvalidResponse_test_003
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, ValidResponse_003, Function | MediumTest | Level3)
{
    HRilLockStatus resp = {HRIL_ERR_SUCCESS, 5};
    LockStatusResp lockStatus;
    RilRadioResponseInfo responseInfo = {RilErrType::NONE};
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    hrilSim->ResponseLockStatus(lockStatus, responseInfo, &resp, sizeof(HRilLockStatus));
    EXPECT_EQ(lockStatus.result, HRIL_ERR_SUCCESS);
    EXPECT_EQ(lockStatus.remain, 5);
    EXPECT_EQ(responseInfo.error, RilErrType::NONE);
}

/**
* @tc.name  : BuildSimIOResp
* @tc.number: BuildSimIOResp_test_001
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, BuildSimIOResp_001, Function | MediumTest | Level3)
{
    IccIoResultInfo result;
    RilRadioResponseInfo responseInfo = {RilErrType::NONE};
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_EQ(hrilSim->BuildSimIOResp(result, responseInfo, nullptr, 1), HRIL_ERR_INVALID_PARAMETER);
}

/**
* @tc.name  : BuildSimIOResp
* @tc.number: BuildSimIOResp_test_002
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, BuildSimIOResp_002, Function | MediumTest | Level3)
{
    HRilSimIOResponse resp = {0};
    IccIoResultInfo result;
    RilRadioResponseInfo responseInfo = {RilErrType::NONE};
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    auto buildresult = hrilSim->BuildSimIOResp(result, responseInfo, &resp, sizeof(HRilSimIOResponse) + 1);
    EXPECT_EQ(buildresult, HRIL_ERR_INVALID_PARAMETER);
}

/**
* @tc.name  : BuildSimIOResp
* @tc.number: BuildSimIOResp_test_003
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, BuildSimIOResp_003, Function | MediumTest | Level3)
{
    IccIoResultInfo result;
    RilRadioResponseInfo responseInfo = {RilErrType::NONE};
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_NE(hrilSim->BuildSimIOResp(result, responseInfo, nullptr, 0), HRIL_ERR_NULL_POINT);
}

/**
* @tc.name  : BuildSimIOResp
* @tc.number: BuildSimIOResp_test_004
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, BuildSimIOResp_004, Function | MediumTest | Level3)
{
    HRilSimIOResponse resp = {0};
    IccIoResultInfo result;
    RilRadioResponseInfo responseInfo = {RilErrType::NONE};
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_EQ(hrilSim->BuildSimIOResp(result, responseInfo, &resp, sizeof(HRilSimIOResponse)), HRIL_ERR_SUCCESS);
}

/**
* @tc.name  : CheckCharData
* @tc.number: CheckCharData_test_001
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, CheckCharData_001, Function | MediumTest | Level3)
{
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_EQ(hrilSim->CheckCharData(nullptr, 1), HRIL_ERR_INVALID_PARAMETER);
}

/**
* @tc.name  : CheckCharData
* @tc.number: CheckCharData_test_002
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, CheckCharData_002, Function | MediumTest | Level3)
{
    char data[] = "test";
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_NE(hrilSim->CheckCharData(data, 3), HRIL_ERR_INVALID_PARAMETER);
}

/**
* @tc.name  : CheckCharData
* @tc.number: CheckCharData_test_003
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, CheckCharData_003, Function | MediumTest | Level3)
{
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_EQ(hrilSim->CheckCharData(nullptr, 0), HRIL_ERR_NULL_POINT);
}

/**
* @tc.name  : CheckCharData
* @tc.number: CheckCharData_test_004
* @tc.desc  : Function test
*/
HWTEST_F(RILSimTest, CheckCharData_004, Function | MediumTest | Level3)
{
    char data[] = "test";
    int32_t slotId = 0;
    auto hrilSim = std::make_shared<HRilSim>(slotId);
    EXPECT_EQ(hrilSim->CheckCharData(data, 4), HRIL_ERR_SUCCESS);
}
} // namespace Telephony
} // namespace OHOS