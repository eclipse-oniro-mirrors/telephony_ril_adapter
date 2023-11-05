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

#include "ril_sms_test.h"
#include "ril_test_util.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_2;
using namespace testing::ext;
namespace {
sptr<OHOS::HDI::Ril::V1_2::IRil> g_rilInterface = nullptr;
}

void RILSmsTest::SetUpTestCase()
{
    TELEPHONY_LOGI("----------RilCallTest gtest start ------------");
    RilTestUtil::GetInstance().Init();
    g_rilInterface = RilTestUtil::GetRilInterface();
}

void RILSmsTest::TearDownTestCase() {}

void RILSmsTest::SetUp() {}

void RILSmsTest::TearDown() {}

/**
 * @tc.number   Telephony_DriverSystem_AddSimMessage_V1_0100
 * @tc.name     Add sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_AddSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, RilTestUtil::GetSerialId(), msgIoInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_ADD_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_AddSimMessage_V1_0200
 * @tc.name     Add sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_AddSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, RilTestUtil::GetSerialId(), msgIoInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_ADD_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_UpdateSimMessage_V1_0100
 * @tc.name     Update sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_UpdateSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, RilTestUtil::GetSerialId(), msgIoInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_UPDATE_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_UpdateSimMessage_V1_0200
 * @tc.name     Update sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_UpdateSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, RilTestUtil::GetSerialId(), msgIoInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_UPDATE_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_DelSimMessage_V1_0100
 * @tc.name     Delete sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_DelSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelSimMessage(SLOTID_1, RilTestUtil::GetSerialId(), index);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_DEL_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_DelSimMessage_V1_0200
 * @tc.name     Delete sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_DelSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelSimMessage(SLOTID_2, RilTestUtil::GetSerialId(), index);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_DEL_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_AddCdmaSimMessage_V1_0100
 * @tc.name     Add cdma sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_AddCdmaSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, RilTestUtil::GetSerialId(), msgIoInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_ADD_CDMA_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_AddCdmaSimMessage_V1_0200
 * @tc.name     Add cdma sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_AddCdmaSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, RilTestUtil::GetSerialId(), msgIoInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_ADD_CDMA_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_UpdateCdmaSimMessage_V1_0100
 * @tc.name     Update cdma sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_UpdateCdmaSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, RilTestUtil::GetSerialId(), msgIoInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_UpdateCdmaSimMessage_V1_0200
 * @tc.name     Update cdma sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_UpdateCdmaSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, RilTestUtil::GetSerialId(), msgIoInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_DelCdmaSimMessage_V1_0100
 * @tc.name     Delete cdma sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_DelCdmaSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_1, RilTestUtil::GetSerialId(), index);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_DEL_CDMA_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_DelCdmaSimMessage_V1_0200
 * @tc.name     Delete cdma sim message
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_DelCdmaSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_2, RilTestUtil::GetSerialId(), index);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_DEL_CDMA_SIM_MESSAGE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetSmscAddr_V1_0100
 * @tc.name     Get smsc addr
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_GetSmscAddr_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_GET_SMSC_ADDR));
}

/**
 * @tc.number   Telephony_DriverSystem_GetSmscAddr_V1_0200
 * @tc.name     Get smsc addr
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_GetSmscAddr_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_GET_SMSC_ADDR));
}

/**
 * @tc.number   Telephony_DriverSystem_SetSmscAddr_V1_0100
 * @tc.name     Set smsc addr
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SetSmscAddr_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = RilTestUtil::GetCallback()->GetSmscAddr();
    address.tosca = TEST_TOSCA;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_1, RilTestUtil::GetSerialId(), address);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SET_SMSC_ADDR));
}

/**
 * @tc.number   Telephony_DriverSystem_SetSmscAddr_V1_0200
 * @tc.name     Set smsc addr
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SetSmscAddr_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = RilTestUtil::GetCallback()->GetSmscAddr();
    address.tosca = TEST_TOSCA;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_2, RilTestUtil::GetSerialId(), address);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SET_SMSC_ADDR));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCBConfig_V1_0100
 * @tc.name     Set cb config
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SetCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    CBConfigInfo info;
    info.mode = 1;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, RilTestUtil::GetSerialId(), info);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SET_CB_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCBConfig_V1_0200
 * @tc.name     Set cb config
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SetCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    CBConfigInfo info;
    info.mode = 1;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_2, RilTestUtil::GetSerialId(), info);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SET_CB_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCBConfig_V1_0100
 * @tc.name     Get cb config
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_GetCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_GET_CB_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCBConfig_V1_0200
 * @tc.name     Get cb config
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_GetCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_GET_CB_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCdmaCBConfig_V1_0100
 * @tc.name     Set cdma cb config
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SetCdmaCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, RilTestUtil::GetSerialId(), broadcastInfoList);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SET_CDMA_CB_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCdmaCBConfig_V1_0200
 * @tc.name     Set cdma cb config
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SetCdmaCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_2, RilTestUtil::GetSerialId(), broadcastInfoList);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SET_CDMA_CB_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCdmaCBConfig_V1_0100
 * @tc.name     Get cdma cb config
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_GetCdmaCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_GET_CDMA_CB_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCdmaCBConfig_V1_0200
 * @tc.name     Get cdma cb config
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_GetCdmaCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_GET_CDMA_CB_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_SendSmsMoreMode_V1_0100
 * @tc.name     Send sms more mode
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SendSmsMoreMode_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, RilTestUtil::GetSerialId(), msg);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_MORE_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_SendSmsMoreMode_V1_0200
 * @tc.name     Send sms more mode
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SendSmsMoreMode_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_2, RilTestUtil::GetSerialId(), msg);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_MORE_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_SendSmsAck_V1_0100
 * @tc.name     Send sms ack
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SendSmsAck_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    ModeData data;
    data.result = TEST_RESULT;
    data.mode = TEST_MODE;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_1, RilTestUtil::GetSerialId(), data);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_ACK));
}

/**
 * @tc.number   Telephony_DriverSystem_SendSmsAck_V1_0200
 * @tc.name     Send sms ack
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SendSmsAck_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.result = TEST_RESULT;
    data.mode = TEST_MODE;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, RilTestUtil::GetSerialId(), data);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_ACK));
}

/**
 * @tc.number   Telephony_DriverSystem_SendGsmSms_V1_0100
 * @tc.name     Send gsm sms
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SendGsmSms_V1_0100, Function | MediumTest | Level2)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, RilTestUtil::GetSerialId(), msg);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SEND_GSM_SMS));
}

/**
 * @tc.number   Telephony_DriverSystem_SendGsmSms_V1_0200
 * @tc.name     Send gsm sms
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SendGsmSms_V1_0200, Function | MediumTest | Level2)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_2, RilTestUtil::GetSerialId(), msg);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SEND_GSM_SMS));
}

/**
 * @tc.number   Telephony_DriverSystem_SendGsmSms_V1_0200
 * @tc.name     Send cdma sms
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SendCdmaSms_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, RilTestUtil::GetSerialId(), msg);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SEND_CDMA_SMS));
}

/**
 * @tc.number   Telephony_DriverSystem_SendCdmaSms_V1_0200
 * @tc.name     Send cdma sms
 * @tc.desc     Function test
 */
HWTEST_F(RILSmsTest, Telephony_DriverSystem_SendCdmaSms_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_2, RilTestUtil::GetSerialId(), msg);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SMS_SEND_CDMA_SMS));
}
} // namespace Telephony
} // namespace OHOS
