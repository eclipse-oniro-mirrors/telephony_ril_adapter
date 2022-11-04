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

#include "ril_callback_test.h"
#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_0;
using namespace testing::ext;
const int32_t BANDWIDTH_HYSTERESIS_MS = 3000;
const int32_t BANDWIDTH_HYSTERESIS_KBPS = 50;
const int32_t MAX_UPLINK_LINK_BANDWIDTH[] = { 100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000 };
const int32_t MAX_DOWNLINK_LINK_BANDWIDTH[] = { 100, // VoIP
    500, // Web
    1000, // SD
    5000, // HD
    10000, // file
    20000, // 4K
    50000, // LTE
    100000,
    200000, // 5G
    500000, 1000000 };
sptr<IRil> g_rilInterface = nullptr;
RilCallbackTest g_callback;

void WaitFor(int32_t timeoutSecond)
{
    g_callback.WaitFor(WAIT_TIME_SECOND);
}

bool GetBoolResult(HdiId hdiId_)
{
    return g_callback.GetBoolResult(hdiId_);
}

int32_t GetSerialId()
{
    return g_callback.GetSerialId();
}

bool IsReady(int32_t slotId)
{
    if (g_rilInterface == nullptr) {
        return false;
    }
    return g_callback.IsReady(slotId);
}
void RilInterfaceTest::SetUpTestCase() {}

void RilInterfaceTest::TearDownTestCase() {}

void RilInterfaceTest::SetUp() {}

void RilInterfaceTest::TearDown() {}

HWTEST_F(RilInterfaceTest, CheckRilInstanceIsEmpty, Function | MediumTest | Level1)
{
    g_rilInterface = IRil::Get();
    if (g_rilInterface == nullptr) {
        return;
    }
    g_rilInterface->SetCallback(&g_callback);
}

HWTEST_F(RilInterfaceTest, SendRilAck, Function | MediumTest | Level1)
{
    if (g_rilInterface == nullptr) {
        return;
    }
    int32_t ret = g_rilInterface->SendRilAck();
    EXPECT_EQ(SUCCESS, ret);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSimStatus_V1_0100, Function | MediumTest | Level2)
{
    if (g_rilInterface == nullptr) {
        return;
    }
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_STATUS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSimStatus_V1_0200, Function | MediumTest | Level2)
{
    if (g_rilInterface == nullptr) {
        return;
    }
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_STATUS));
}

// Call
HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetEmergencyCallList_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_EMERGENCY_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetEmergencyCallList_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_EMERGENCY_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetEmergencyCallList_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
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
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_EMERGENCY_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetEmergencyCallList_V2_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
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
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_2, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_EMERGENCY_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallList_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallList_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_Dial_V1_0100, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_DIAL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_Dial_V1_0200, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_2, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_DIAL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_Reject_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_REJECT));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_Reject_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_REJECT));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_Hangup_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_HANGUP));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_Hangup_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_HANGUP));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_Answer_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_ANSWER));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_Answer_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_ANSWER));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_HoldCall_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_HOLD_CALL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_HoldCall_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_HOLD_CALL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnHoldCall_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_UNHOLD_CALL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnHoldCall_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_UNHOLD_CALL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SwitchCall_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SWITCH_CALL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SwitchCall_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SWITCH_CALL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_CombineConference_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_COMBINE_CONFERENCE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_CombineConference_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_COMBINE_CONFERENCE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SeparateConference_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SEPARATE_CONFERENCE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SeparateConference_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_2, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SEPARATE_CONFERENCE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallWaiting_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_WAITING));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallWaiting_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_WAITING));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCallWaiting_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_WAITING));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCallWaiting_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_WAITING));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallTransferInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_TRANSFER_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallTransferInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_TRANSFER_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCallTransferInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_TRANSFER_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCallTransferInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_2, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_TRANSFER_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallRestriction_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_1, GetSerialId(), fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_RESTRICTION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallRestriction_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_2, GetSerialId(), fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_RESTRICTION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCallRestriction_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_RESTRICTION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCallRestriction_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_2, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_RESTRICTION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetClip_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CLIP));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetClip_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CLIP));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetClip_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CLIP));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetClip_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CLIP));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetClir_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CLIR));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetClir_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CLIR));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetClir_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CLIR));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetClir_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CLIR));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCallPreferenceMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_PREFERENCE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCallPreferenceMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_PREFERENCE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallPreferenceMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_PREFERENCE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallPreferenceMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_PREFERENCE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetUssd_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, GetSerialId(), "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_USSD));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetUssd_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_2, GetSerialId(), "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_USSD));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetUssd_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_USSD));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetUssd_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_USSD));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetMute_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_MUTE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetMute_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_MUTE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetMute_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_MUTE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetMute_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_MUTE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallFailReason_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_FAIL_REASON));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCallFailReason_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_FAIL_REASON));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_CallSupplement_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_CALL_SUPPLEMENT));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_CallSupplement_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_CALL_SUPPLEMENT));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendDtmf_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SEND_DTMF));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendDtmf_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 1;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_2, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SEND_DTMF));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_StartDtmf_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_START_DTMF));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_StartDtmf_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_2, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_START_DTMF));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_StopDtmf_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_STOP_DTMF));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_StopDtmf_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_2, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_STOP_DTMF));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetBarringPassword_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_BARRING_PASSWORD));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetBarringPassword_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_2, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_BARRING_PASSWORD));
}

// Network
HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSignalStrength_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_SIGNAL_STRENGTH));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSignalStrength_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_SIGNAL_STRENGTH));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCsRegStatus_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_CS_REG_STATUS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCsRegStatus_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_CS_REG_STATUS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetPsRegStatus_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PS_REG_STATUS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetPsRegStatus_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PS_REG_STATUS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetOperatorInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_OPERATOR_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetOperatorInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_OPERATOR_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetNetworkSearchInformation_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetNetworkSearchInformation_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetNetworkSelectionMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SELECTION_MODE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetNetworkSelectionMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SELECTION_MODE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetNetworkSelectionMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(SelectionMode::MODE_TYPE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_NETWORK_SELECTION_MODE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetNetworkSelectionMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(SelectionMode::MODE_TYPE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_NETWORK_SELECTION_MODE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetPreferredNetwork_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_PREFERRED_NETWORK));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetPreferredNetwork_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_PREFERRED_NETWORK));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetPreferredNetwork_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PREFERRED_NETWORK));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetPreferredNetwork_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PREFERRED_NETWORK));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCellInfoList_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCellInfoList_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCurrentCellInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCurrentCellInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetPhysicalChannelConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetPhysicalChannelConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetLocateUpdates_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetLocateUpdates(SLOTID_1, GetSerialId(), RilRegNotifyMode::REG_NOTIFY_STAT_LAC_CELLID);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_LOCATE_UPDATES));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetLocateUpdates_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetLocateUpdates(SLOTID_2, GetSerialId(), RilRegNotifyMode::REG_NOTIFY_STAT_LAC_CELLID);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_LOCATE_UPDATES));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetNotificationFilter_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_ALL));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_NOTIFICATION_FILTER));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetNotificationFilter_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_ALL));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_NOTIFICATION_FILTER));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetDeviceState_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_DEVICE_STATE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetDeviceState_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_DEVICE_STATE));
}

/**
 * modem Test
 **/
HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_ShutDown_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_SHUT_DOWN));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_ShutDown_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_SHUT_DOWN));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetRadioState_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_SET_RADIO_STATUS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetRadioState_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_SET_RADIO_STATUS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetRadioState_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_RADIO_STATUS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetRadioState_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_RADIO_STATUS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetImei_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_IMEI));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetImei_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_IMEI));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetMeid_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_MEID));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetMeid_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_MEID));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetBasebandVersion_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_BASEBAND_VERSION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetBasebandVersion_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_BASEBAND_VERSION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetVoiceRadioTechnology_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_VOICE_RADIO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetVoiceRadioTechnology_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_VOICE_RADIO));
}

// Data
HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_ActivatePdpContext_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = 0;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_ACTIVATE_PDP_CONTEXT));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_ActivatePdpContext_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = 0;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_2, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_ACTIVATE_PDP_CONTEXT));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_DeactivatePdpContext_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = CID;
    uniInfo.arg1 = REASON;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_DEACTIVATE_PDP_CONTEXT));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_DeactivatePdpContext_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = CID;
    uniInfo.arg1 = REASON;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_DEACTIVATE_PDP_CONTEXT));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetPdpContextList_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_GET_PDP_CONTEXT_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetPdpContextList_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_GET_PDP_CONTEXT_LIST));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetInitApnInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
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
    int32_t ret = g_rilInterface->SetInitApnInfo(SLOTID_1, serialId, dataProfileInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_INIT_APN_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetInitApnInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
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
    int32_t ret = g_rilInterface->SetInitApnInfo(SLOTID_2, serialId, dataProfileInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_INIT_APN_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetLinkBandwidthInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_1, GetSerialId(), CID);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_GET_LINK_BANDWIDTH_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetLinkBandwidthInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_2, GetSerialId(), CID);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_GET_LINK_BANDWIDTH_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetLinkBandwidthReportingRule_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    uint32_t uplinkKbpsSize = sizeof(MAX_UPLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    uint32_t downlinkKbpsSize = sizeof(MAX_DOWNLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_LTE);
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
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetLinkBandwidthReportingRule_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    uint32_t uplinkKbpsSize = sizeof(MAX_UPLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    uint32_t downlinkKbpsSize = sizeof(MAX_DOWNLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_LTE);
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
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetDataPermitted_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PERMITTED));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetDataPermitted_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PERMITTED));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetDataProfileInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_1, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PROFILE_INFO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetDataProfileInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_2, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PROFILE_INFO));
}

// SIM
HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSimIO_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_IO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSimIO_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
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
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_IO));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetImsi_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImsi(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_IMSI));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetImsi_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImsi(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_IMSI));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetActiveSim_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t enable = 1;
    int32_t index = 1; // ENTITY_CARD
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), index, enable);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_SET_ACTIVE_SIM));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetActiveSim_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t enable = 1;
    int32_t index = 1; // ENTITY_CARD
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, GetSerialId(), index, enable);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_SET_ACTIVE_SIM));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimStkSendTerminalResponse_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string cmd = "send terminal response";
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, GetSerialId(), cmd);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_TERMINAL_RESPONSE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimStkSendTerminalResponse_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string cmd = "send terminal response";
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_2, GetSerialId(), cmd);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_TERMINAL_RESPONSE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimStkSendEnvelope_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string cmd = "send envelope";
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, GetSerialId(), cmd);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_ENVELOPE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimStkSendEnvelope_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string cmd = "send envelope";
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_2, GetSerialId(), cmd);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_ENVELOPE));
}

HWTEST_F(
    RilInterfaceTest, Telephony_DriverSystem_SimStkSendCallSetupRequestResult_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_1, GetSerialId(), true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT));
}

HWTEST_F(
    RilInterfaceTest, Telephony_DriverSystem_SimStkSendCallSetupRequestResult_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_2, GetSerialId(), true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimStkIsReady_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_IS_READY));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimStkIsReady_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_IS_READY));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetRadioProtocol_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    RadioProtocol protocol;
    protocol.slotId = SLOTID_1;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_1, GetSerialId(), protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_RADIO_PROTOCOL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetRadioProtocol_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    RadioProtocol protocol;
    protocol.slotId = SLOTID_2;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_2, GetSerialId(), protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_RADIO_PROTOCOL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimOpenLogicalChannel_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string appID = "A00000015141434C00"; // ARAM_AID
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_1, GetSerialId(), appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_OPEN_LOGICAL_CHANNEL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimTransmitApduLogicalChannel_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.channelId = g_callback.GetCurrentChannelId();
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimCloseLogicalChannel_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_1, GetSerialId(), g_callback.GetCurrentChannelId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_CLOSE_LOGICAL_CHANNEL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimOpenLogicalChannel_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string appID = "A00000015141434C00"; // ARAM_AID
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_2, GetSerialId(), appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_OPEN_LOGICAL_CHANNEL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimTransmitApduLogicalChannel_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.channelId = g_callback.GetCurrentChannelId();
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_2, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimCloseLogicalChannel_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_2, GetSerialId(), g_callback.GetCurrentChannelId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_CLOSE_LOGICAL_CHANNEL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimTransmitApduBasicChannel_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimTransmitApduBasicChannel_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_2, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimAuthentication_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = "aid";
    simAuthInfo.authData = "authData";
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_AUTHENTICATION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SimAuthentication_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = "aid";
    simAuthInfo.authData = "authData";
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_2, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_AUTHENTICATION));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSimLockStatus_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_LOCK_STATUS));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSimLockStatus_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_LOCK_STATUS));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetSimLock_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    simLockInfo.passwd = "1234";
    char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_SET_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetSimLock_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    simLockInfo.passwd = "1234";
    char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_SET_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_ChangeSimPassword_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC"; // FAC_PIN_LOCK
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_CHANGE_SIM_PASSWORD));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_ChangeSimPassword_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC"; // FAC_PIN_LOCK
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_CHANGE_SIM_PASSWORD));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockPin_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPin(SLOTID_1, GetSerialId(), pin);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockPin_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPin(SLOTID_2, GetSerialId(), pin);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockPuk_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string puk = "42014264";
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPuk(SLOTID_1, GetSerialId(), puk, pin);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockPuk_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string puk = "42014264";
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPuk(SLOTID_2, GetSerialId(), puk, pin);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockPin2_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPin2(SLOTID_1, GetSerialId(), pin2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN2));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockPin2_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPin2(SLOTID_2, GetSerialId(), pin2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN2));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockPuk2_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string puk2 = "42014264";
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPuk2(SLOTID_1, GetSerialId(), puk2, pin2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK2));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockPuk2_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string puk2 = "42014264";
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPuk2(SLOTID_2, GetSerialId(), puk2, pin2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK2));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockSimLock_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t lockType = 0;
    std::string key = "1234"; // PN_PIN_LOCK
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, GetSerialId(), lockType, key);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UnlockSimLock_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t lockType = 0;
    std::string key = "1234"; // PN_PIN_LOCK
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, GetSerialId(), lockType, key);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

// Sms
HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_AddSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_ADD_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_AddSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_ADD_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UpdateSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_UPDATE_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UpdateSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_UPDATE_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_DelSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelSimMessage(SLOTID_1, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_DEL_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_DelSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelSimMessage(SLOTID_2, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_DEL_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_AddCdmaSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_ADD_CDMA_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_AddCdmaSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_ADD_CDMA_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UpdateCdmaSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_UpdateCdmaSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_DelCdmaSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_1, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_DEL_CDMA_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_DelCdmaSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_2, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_DEL_CDMA_SIM_MESSAGE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSmscAddr_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_SMSC_ADDR));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetSmscAddr_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_SMSC_ADDR));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetSmscAddr_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = g_callback.GetSmscAddr();
    address.tosca = TEST_TOSCA;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_1, GetSerialId(), address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_SMSC_ADDR));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetSmscAddr_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = g_callback.GetSmscAddr();
    address.tosca = TEST_TOSCA;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_2, GetSerialId(), address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_SMSC_ADDR));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CBConfigInfo info;
    info.mode = 1;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_CB_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    CBConfigInfo info;
    info.mode = 1;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_2, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_CB_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_CB_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_CB_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCdmaCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_CDMA_CB_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SetCdmaCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_2, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_CDMA_CB_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCdmaCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_CDMA_CB_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_GetCdmaCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_CDMA_CB_CONFIG));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendSmsMoreMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_MORE_MODE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendSmsMoreMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_MORE_MODE));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendSmsAck_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ModeData data;
    data.result = TEST_RESULT;
    data.mode = TEST_MODE;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_1, GetSerialId(), data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_ACK));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendSmsAck_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.result = TEST_RESULT;
    data.mode = TEST_MODE;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, GetSerialId(), data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_ACK));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendGsmSms_V1_0100, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_GSM_SMS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendGsmSms_V1_0200, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_GSM_SMS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendCdmaSms_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_CDMA_SMS));
}

HWTEST_F(RilInterfaceTest, Telephony_DriverSystem_SendCdmaSms_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_CDMA_SMS));
}

} // namespace Telephony
} // namespace OHOS
