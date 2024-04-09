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

#include "ril_modem_test.h"
#include "ril_test_util.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_3;
using namespace testing::ext;
namespace {
sptr<OHOS::HDI::Ril::V1_3::IRil> g_rilInterface = nullptr;
}

void RILModemTest::SetUpTestCase()
{
    TELEPHONY_LOGI("----------RilCallTest gtest start ------------");
    RilTestUtil::GetInstance().Init();
    g_rilInterface = RilTestUtil::GetRilInterface();
}

void RILModemTest::TearDownTestCase() {}

void RILModemTest::SetUp() {}

void RILModemTest::TearDown() {}

/**
 * @tc.number   Telephony_DriverSystem_ShutDown_V1_0100
 * @tc.name     Shut down
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_ShutDown_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_SHUT_DOWN));
}

/**
 * @tc.number   Telephony_DriverSystem_ShutDown_V1_0200
 * @tc.name     Shut down
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_ShutDown_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_SHUT_DOWN));
}

/**
 * @tc.number   Telephony_DriverSystem_SetRadioState_V1_0100
 * @tc.name     Set radio state
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_SetRadioState_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, RilTestUtil::GetSerialId(), 1, 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_SET_RADIO_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_SetRadioState_V1_0200
 * @tc.name     Set radio state
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_SetRadioState_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, RilTestUtil::GetSerialId(), 1, 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_SET_RADIO_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_GetRadioState_V1_0100
 * @tc.name     Get radio state
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetRadioState_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_RADIO_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_GetRadioState_V1_0200
 * @tc.name     Get radio state
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetRadioState_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_RADIO_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_GetImei_V1_0100
 * @tc.name     Get imei
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetImei_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_IMEI));
}

/**
 * @tc.number   Telephony_DriverSystem_GetImei_V1_0200
 * @tc.name     Get imei
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetImei_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_IMEI));
}

/**
 * @tc.number   Telephony_DriverSystem_GetImeiSv_V1_0100
 * @tc.name     Get imeiSv
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetImeiSv_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImeiSv(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_IMEISV));
}

/**
 * @tc.number   Telephony_DriverSystem_GetImeiSv_V1_0200
 * @tc.name     Get imeiSv
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetImeiSv_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImeiSv(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_IMEISV));
}

/**
 * @tc.number   Telephony_DriverSystem_GetMeid_V1_0100
 * @tc.name     Get meid
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetMeid_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_MEID));
}

/**
 * @tc.number   Telephony_DriverSystem_GetMeid_V1_0200
 * @tc.name     Get meid
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetMeid_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_MEID));
}

/**
 * @tc.number   Telephony_DriverSystem_GetBasebandVersion_V1_0100
 * @tc.name     Get base band version
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetBasebandVersion_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_BASEBAND_VERSION));
}

/**
 * @tc.number   Telephony_DriverSystem_GetBasebandVersion_V1_0200
 * @tc.name     Get base band version
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetBasebandVersion_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_BASEBAND_VERSION));
}

/**
 * @tc.number   Telephony_DriverSystem_GetVoiceRadioTechnology_V1_0100
 * @tc.name     Get voice radio technology
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetVoiceRadioTechnology_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_VOICE_RADIO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetVoiceRadioTechnology_V1_0200
 * @tc.name     Get voice radio technology
 * @tc.desc     Function test
 */
HWTEST_F(RILModemTest, Telephony_DriverSystem_GetVoiceRadioTechnology_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_MODEM_GET_VOICE_RADIO));
}
} // namespace Telephony
} // namespace OHOS
