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

#include "ril_call_test.h"
#include "ril_test_util.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_2;
using namespace testing::ext;

namespace {
sptr<OHOS::HDI::Ril::V1_2::IRil> g_rilInterface = nullptr;
}

void RilCallTest::SetUpTestCase()
{
    TELEPHONY_LOGI("----------RilCallTest gtest start ------------");
    RilTestUtil::GetInstance().Init();
    g_rilInterface = RilTestUtil::GetRilInterface();
}

void RilCallTest::TearDownTestCase() {}

void RilCallTest::SetUp() {}

void RilCallTest::TearDown() {}

/**
 * @tc.number   SendRilAck
 * @tc.name     Send ril ack
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, SendRilAck, Function | MediumTest | Level1)
{
    if (g_rilInterface == nullptr) {
        return;
    }
    int32_t ret = g_rilInterface->SendRilAck();
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number   Telephony_DriverSystem_GetEmergencyCallList_V1_0100
 * @tc.name     Get emergency call list
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetEmergencyCallList_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_EMERGENCY_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_GetEmergencyCallList_V1_0200
 * @tc.name     Get emergency call list
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetEmergencyCallList_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_EMERGENCY_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_SetEmergencyCallList_V1_0100
 * @tc.name     Set emergency call list
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetEmergencyCallList_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
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
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, RilTestUtil::GetSerialId(), emergencyInfoList);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_EMERGENCY_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_SetEmergencyCallList_V2_0100
 * @tc.name     Set emergency call list
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetEmergencyCallList_V2_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
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
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_2, RilTestUtil::GetSerialId(), emergencyInfoList);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_EMERGENCY_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallList_V1_0100
 * @tc.name     Get call list
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallList_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallList_V1_0200
 * @tc.name     Get call list
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallList_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallList_V1_0200
 * @tc.name     Get call list
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_Dial_V1_0100, Function | MediumTest | Level2)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, RilTestUtil::GetSerialId(), dialInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_DIAL));
}

/**
 * @tc.number   Telephony_DriverSystem_Dial_V1_0200
 * @tc.name     Dial
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_Dial_V1_0200, Function | MediumTest | Level2)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_2, RilTestUtil::GetSerialId(), dialInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_DIAL));
}

/**
 * @tc.number   Telephony_DriverSystem_Reject_V1_0100
 * @tc.name     Reject
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_Reject_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_REJECT));
}

/**
 * @tc.number   Telephony_DriverSystem_Reject_V1_0200
 * @tc.name     Reject
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_Reject_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_REJECT));
}

/**
 * @tc.number   Telephony_DriverSystem_Hangup_V1_0100
 * @tc.name     Hangup
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_Hangup_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, RilTestUtil::GetSerialId(), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_HANGUP));
}

/**
 * @tc.number   Telephony_DriverSystem_Hangup_V1_0200
 * @tc.name     Hangup
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_Hangup_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_2, RilTestUtil::GetSerialId(), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_HANGUP));
}

/**
 * @tc.number   Telephony_DriverSystem_Answer_V1_0100
 * @tc.name     Answer
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_Answer_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_ANSWER));
}

/**
 * @tc.number   Telephony_DriverSystem_Answer_V1_0200
 * @tc.name     Answer
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_Answer_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_ANSWER));
}

/**
 * @tc.number   Telephony_DriverSystem_HoldCall_V1_0100
 * @tc.name     HoldCall
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_HoldCall_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_HOLD_CALL));
}

/**
 * @tc.number   Telephony_DriverSystem_HoldCall_V1_0200
 * @tc.name     HoldCall
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_HoldCall_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_HOLD_CALL));
}

/**
 * @tc.number   Telephony_DriverSystem_UnHoldCall_V1_0100
 * @tc.name     UnHoldCall
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_UnHoldCall_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_UNHOLD_CALL));
}

/**
 * @tc.number   Telephony_DriverSystem_UnHoldCall_V1_0200
 * @tc.name     UnHoldCall
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_UnHoldCall_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_UNHOLD_CALL));
}

/**
 * @tc.number   Telephony_DriverSystem_SwitchCall_V1_0100
 * @tc.name     Switch call
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SwitchCall_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SWITCH_CALL));
}

/**
 * @tc.number   Telephony_DriverSystem_SwitchCall_V1_0200
 * @tc.name     Switch call
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SwitchCall_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SWITCH_CALL));
}

/**
 * @tc.number   Telephony_DriverSystem_CombineConference_V1_0100
 * @tc.name     Combine conference
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_CombineConference_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, RilTestUtil::GetSerialId(), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_COMBINE_CONFERENCE));
}

/**
 * @tc.number   Telephony_DriverSystem_CombineConference_V1_0200
 * @tc.name     Combine conference
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_CombineConference_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_2, RilTestUtil::GetSerialId(), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_COMBINE_CONFERENCE));
}

/**
 * @tc.number   Telephony_DriverSystem_SeparateConference_V1_0100
 * @tc.name     Separate conference
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SeparateConference_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, RilTestUtil::GetSerialId(), 1, 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SEPARATE_CONFERENCE));
}

/**
 * @tc.number   Telephony_DriverSystem_SeparateConference_V1_0200
 * @tc.name     Separate conference
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SeparateConference_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_2, RilTestUtil::GetSerialId(), 1, 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SEPARATE_CONFERENCE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallWaiting_V1_0100
 * @tc.name     Get call waiting
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallWaiting_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_WAITING));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallWaiting_V1_0200
 * @tc.name     Get call waiting
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallWaiting_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_WAITING));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCallWaiting_V1_0100
 * @tc.name     Set call waiting
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetCallWaiting_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, RilTestUtil::GetSerialId(), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CALL_WAITING));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCallWaiting_V1_0200
 * @tc.name     Set call waiting
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetCallWaiting_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_2, RilTestUtil::GetSerialId(), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CALL_WAITING));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallTransferInfo_V1_0100
 * @tc.name     Get call transfer info
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallTransferInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_TRANSFER_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallTransferInfo_V1_0200
 * @tc.name     Get call transfer info
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallTransferInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_2, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_TRANSFER_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCallTransferInfo_V1_0100
 * @tc.name     Set call transfer info
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetCallTransferInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, RilTestUtil::GetSerialId(), callForwardSetInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CALL_TRANSFER_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCallTransferInfo_V1_0200
 * @tc.name     Set call transfer info
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetCallTransferInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_2, RilTestUtil::GetSerialId(), callForwardSetInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CALL_TRANSFER_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallRestriction_V1_0100
 * @tc.name     Get call restriction
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallRestriction_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_1, RilTestUtil::GetSerialId(), fac);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_RESTRICTION));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallRestriction_V1_0200
 * @tc.name     Get call restriction
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallRestriction_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_2, RilTestUtil::GetSerialId(), fac);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_RESTRICTION));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCallRestriction_V1_0100
 * @tc.name     Set call restriction
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetCallRestriction_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, RilTestUtil::GetSerialId(), callRestrictionInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CALL_RESTRICTION));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallRestriction_V1_0200
 * @tc.name     Set call restriction
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetCallRestriction_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_2, RilTestUtil::GetSerialId(), callRestrictionInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CALL_RESTRICTION));
}

/**
 * @tc.number   Telephony_DriverSystem_GetClip_V1_0100
 * @tc.name     Get clip
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetClip_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CLIP));
}

/**
 * @tc.number   Telephony_DriverSystem_GetClip_V1_0200
 * @tc.name     Get clip
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetClip_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CLIP));
}

/**
 * @tc.number   Telephony_DriverSystem_SetClip_V1_0100
 * @tc.name     Set clip
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetClip_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CLIP));
}

/**
 * @tc.number   Telephony_DriverSystem_SetClip_V1_0200
 * @tc.name     Set clip
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetClip_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_2, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CLIP));
}

/**
 * @tc.number   Telephony_DriverSystem_GetClir_V1_0100
 * @tc.name     Get clip
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetClir_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CLIR));
}

/**
 * @tc.number   Telephony_DriverSystem_GetClir_V1_0200
 * @tc.name     Get clip
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetClir_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CLIR));
}

/**
 * @tc.number   Telephony_DriverSystem_SetClir_V1_0100
 * @tc.name     Set clir
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetClir_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CLIR));
}

/**
 * @tc.number   Telephony_DriverSystem_SetClir_V1_0200
 * @tc.name     Set clir
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetClir_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_2, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CLIR));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCallPreferenceMode_V1_0100
 * @tc.name     Set call preference mode
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetCallPreferenceMode_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CALL_PREFERENCE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCallPreferenceMode_V1_0200
 * @tc.name     Set call preference mode
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetCallPreferenceMode_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_2, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_CALL_PREFERENCE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallPreferenceMode_V1_0100
 * @tc.name     Get call preference mode
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallPreferenceMode_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_PREFERENCE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetCallPreferenceMode_V1_0200
 * @tc.name     Get call preference mode
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallPreferenceMode_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_CALL_PREFERENCE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetUssd_V1_0100
 * @tc.name     Set ussd
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetUssd_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, RilTestUtil::GetSerialId(), "12345678");
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_USSD));
}

/**
 * @tc.number   Telephony_DriverSystem_SetUssd_V1_0200
 * @tc.name     Set ussd
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetUssd_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_2, RilTestUtil::GetSerialId(), "12345678");
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_USSD));
}

/**
 * @tc.number   Telephony_DriverSystem_GetUssd_V1_0100
 * @tc.name     Get ussd
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetUssd_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_USSD));
}

/**
 * @tc.number   Telephony_DriverSystem_GetUssd_V1_0200
 * @tc.name     Get ussd
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetUssd_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_USSD));
}

/**
 * @tc.number   Telephony_DriverSystem_SetMute_V1_0100
 * @tc.name     Set mute
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetMute_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_MUTE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetMute_V1_0200
 * @tc.name     Set mute
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetMute_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_2, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_MUTE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetMute_V1_0100
 * @tc.name     Get mute
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetMute_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_MUTE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetMute_V1_0200
 * @tc.name     Get mute
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetMute_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_MUTE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallFailReason_V1_0100
 * @tc.name     Get call fail reason
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallFailReason_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_FAIL_REASON));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCallFailReason_V1_0200
 * @tc.name     Get call fail reason
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_GetCallFailReason_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_GET_FAIL_REASON));
}

/**
 * @tc.number   Telephony_DriverSystem_CallSupplement_V1_0100
 * @tc.name     Call supplement
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_CallSupplement_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_CALL_SUPPLEMENT));
}

/**
 * @tc.number   Telephony_DriverSystem_CallSupplement_V1_0200
 * @tc.name     Call supplement
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_CallSupplement_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_2, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_CALL_SUPPLEMENT));
}

/**
 * @tc.number   Telephony_DriverSystem_SendDtmf_V1_0100
 * @tc.name     Send dtmf
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SendDtmf_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, RilTestUtil::GetSerialId(), dtmfInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SEND_DTMF));
}

/**
 * @tc.number   Telephony_DriverSystem_SendDtmf_V1_0200
 * @tc.name     Send dtmf
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SendDtmf_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 1;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_2, RilTestUtil::GetSerialId(), dtmfInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SEND_DTMF));
}

/**
 * @tc.number   Telephony_DriverSystem_StartDtmf_V1_0100
 * @tc.name     Start dtmf
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_StartDtmf_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, RilTestUtil::GetSerialId(), dtmfInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_START_DTMF));
}

/**
 * @tc.number   Telephony_DriverSystem_StartDtmf_V1_0200
 * @tc.name     Start dtmf
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_StartDtmf_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_2, RilTestUtil::GetSerialId(), dtmfInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_START_DTMF));
}

/**
 * @tc.number   Telephony_DriverSystem_StopDtmf_V1_0100
 * @tc.name     Stop dtmf
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_StopDtmf_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, RilTestUtil::GetSerialId(), dtmfInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_STOP_DTMF));
}

/**
 * @tc.number   Telephony_DriverSystem_StopDtmf_V1_0200
 * @tc.name     Stop dtmf
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_StopDtmf_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_2, RilTestUtil::GetSerialId(), dtmfInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_STOP_DTMF));
}

/**
 * @tc.number   Telephony_DriverSystem_SetBarringPassword_V1_0100
 * @tc.name     Set barring password
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetBarringPassword_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, RilTestUtil::GetSerialId(), setBarringInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_BARRING_PASSWORD));
}

/**
 * @tc.number   Telephony_DriverSystem_SetBarringPassword_V1_0200
 * @tc.name     Set barring password
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetBarringPassword_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_2, RilTestUtil::GetSerialId(), setBarringInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_SET_BARRING_PASSWORD));
}

/**
 * @tc.number   Telephony_DriverSystem_CloseUnFinishedUssd_V1_0100
 * @tc.name     Set barring password
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_CloseUnFinishedUssd_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CloseUnFinishedUssd(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_CLOSE_UNFINISHED_USSD));
}

/**
 * @tc.number   Telephony_DriverSystem_CloseUnFinishedUssd_V1_0200
 * @tc.name     Set barring password
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_CloseUnFinishedUssd_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->CloseUnFinishedUssd(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_CALL_CLOSE_UNFINISHED_USSD));
}

/**
 * @tc.number   Telephony_DriverSystem_SetVonrSwitch_V1_0100
 * @tc.name     Set vonr switch
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetVonrSwitch_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_1, RilTestUtil::GetSerialId(), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SET_VONR_SWITCH));
}

/**
 * @tc.number   Telephony_DriverSystem_SetVonrSwitch_V1_0200
 * @tc.name     Set vonr switch
 * @tc.desc     Function test
 */
HWTEST_F(RilCallTest, Telephony_DriverSystem_SetVonrSwitch_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_2, RilTestUtil::GetSerialId(), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_SET_VONR_SWITCH));
}
} // namespace Telephony
} // namespace OHOS
