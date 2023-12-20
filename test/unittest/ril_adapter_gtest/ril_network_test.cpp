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

#include "ril_network_test.h"
#include "ril_test_util.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_2;
using namespace testing::ext;
namespace {
sptr<OHOS::HDI::Ril::V1_2::IRil> g_rilInterface = nullptr;
}

void RILNetworkTest::SetUpTestCase()
{
    TELEPHONY_LOGI("----------RilCallTest gtest start ------------");
    RilTestUtil::GetInstance().Init();
    g_rilInterface = RilTestUtil::GetRilInterface();
}

void RILNetworkTest::TearDownTestCase() {}

void RILNetworkTest::SetUp() {}

void RILNetworkTest::TearDown() {}

/**
 * @tc.number   Telephony_DriverSystem_GetSignalStrength_V1_0100
 * @tc.name     Get signal strength
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetSignalStrength_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_SIGNAL_STRENGTH));
}

/**
 * @tc.number   Telephony_DriverSystem_GetSignalStrength_V1_0200
 * @tc.name     Get signal strength
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetSignalStrength_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_SIGNAL_STRENGTH));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCsRegStatus_V1_0100
 * @tc.name     Get cs register status
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetCsRegStatus_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_CS_REG_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCsRegStatus_V1_0200
 * @tc.name     Get cs register status
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetCsRegStatus_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_CS_REG_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_GetPsRegStatus_V1_0100
 * @tc.name     Get ps register status
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetPsRegStatus_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_PS_REG_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_GetPsRegStatus_V1_0200
 * @tc.name     Get ps register status
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetPsRegStatus_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_PS_REG_STATUS));
}

/**
 * @tc.number   Telephony_DriverSystem_GetOperatorInfo_V1_0100
 * @tc.name     Get operator info
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetOperatorInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_OPERATOR_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetOperatorInfo_V1_0200
 * @tc.name     Get operator info
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetOperatorInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_OPERATOR_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetNetworkSearchInformation_V1_0100
 * @tc.name     Get network search information
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetNetworkSearchInformation_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION));
}

/**
 * @tc.number   Telephony_DriverSystem_GetNetworkSearchInformation_V1_0200
 * @tc.name     Get network search information
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetNetworkSearchInformation_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION));
}

/**
 * @tc.number   Telephony_DriverSystem_GetNetworkSelectionMode_V1_0100
 * @tc.name     Get network search selection mode
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetNetworkSelectionMode_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SELECTION_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetNetworkSelectionMode_V1_0200
 * @tc.name     Get network search selection mode
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetNetworkSelectionMode_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SELECTION_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetNetworkSelectionMode_V1_0100
 * @tc.name     Set network search selection mode
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetNetworkSelectionMode_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(SelectionMode::MODE_TYPE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, RilTestUtil::GetSerialId(), setNetworkModeInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_NETWORK_SELECTION_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetNetworkSelectionMode_V1_0200
 * @tc.name     Set network search selection mode
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetNetworkSelectionMode_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(SelectionMode::MODE_TYPE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, RilTestUtil::GetSerialId(), setNetworkModeInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_NETWORK_SELECTION_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetPreferredNetwork_V1_0100
 * @tc.name     Set preferred network
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetPreferredNetwork_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, RilTestUtil::GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE));
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_PREFERRED_NETWORK));
}

/**
 * @tc.number   Telephony_DriverSystem_SetPreferredNetwork_V1_0200
 * @tc.name     Set preferred network
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetPreferredNetwork_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, RilTestUtil::GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE));
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_PREFERRED_NETWORK));
}

/**
 * @tc.number   Telephony_DriverSystem_GetPreferredNetwork_V1_0100
 * @tc.name     Get preferred network
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetPreferredNetwork_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_PREFERRED_NETWORK));
}

/**
 * @tc.number   Telephony_DriverSystem_GetPreferredNetwork_V1_0200
 * @tc.name     Get preferred network
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetPreferredNetwork_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_PREFERRED_NETWORK));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCellInfoList_V1_0100
 * @tc.name     Get cell information list
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetCellInfoList_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCellInfoList_V1_0200
 * @tc.name     Get cell information list
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetCellInfoList_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCurrentCellInfo_V1_0100
 * @tc.name     Get current cell information
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetCurrentCellInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetCurrentCellInfo_V1_0200
 * @tc.name     Get current cell information
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetCurrentCellInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetPhysicalChannelConfig_V1_0100
 * @tc.name     Get physical channel config
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetPhysicalChannelConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_GetPhysicalChannelConfig_V1_0200
 * @tc.name     Get physical channel config
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetPhysicalChannelConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG));
}

/**
 * @tc.number   Telephony_DriverSystem_SetLocateUpdates_V1_0100
 * @tc.name     Set locate updates
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetLocateUpdates_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetLocateUpdates(SLOTID_1, RilTestUtil::GetSerialId(),
            RilRegNotifyMode::REG_NOTIFY_STAT_LAC_CELLID);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_LOCATE_UPDATES));
}

/**
 * @tc.number   Telephony_DriverSystem_SetLocateUpdates_V1_0200
 * @tc.name     Set locate updates
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetLocateUpdates_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetLocateUpdates(SLOTID_2, RilTestUtil::GetSerialId(),
            RilRegNotifyMode::REG_NOTIFY_STAT_LAC_CELLID);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_LOCATE_UPDATES));
}

/**
 * @tc.number   Telephony_DriverSystem_SetNotificationFilter_V1_0100
 * @tc.name     SetNotificationFilter
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetNotificationFilter_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, RilTestUtil::GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_ALL));
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_NOTIFICATION_FILTER));
}

/**
 * @tc.number   Telephony_DriverSystem_SetNotificationFilter_V1_0200
 * @tc.name     SetNotificationFilter
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetNotificationFilter_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, RilTestUtil::GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_ALL));
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_NOTIFICATION_FILTER));
}

/**
 * @tc.number   Telephony_DriverSystem_SetDeviceState_V1_0100
 * @tc.name     Set device state
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetDeviceState_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(
        SLOTID_1, RilTestUtil::GetSerialId(), static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_DEVICE_STATE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetDeviceState_V1_0200
 * @tc.name     Set device state
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetDeviceState_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(
        SLOTID_2, RilTestUtil::GetSerialId(), static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 0);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_DEVICE_STATE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetNrOptionMode_V1_0100
 * @tc.name     Set the option mode of NR.
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetNrOptionMode_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNrOptionMode(SLOTID_1, RilTestUtil::GetSerialId(), NR_OPTION_NSA_ONLY);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_NR_OPTION_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetNrOptionMode_V1_0200
 * @tc.name     Set the option mode of NR.
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_SetNrOptionMode_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNrOptionMode(SLOTID_2, RilTestUtil::GetSerialId(), NR_OPTION_NSA_ONLY);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_SET_NR_OPTION_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetNrOptionMode_V1_0100
 * @tc.name     Get the option mode of NR.
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetNrOptionMode_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNrOptionMode(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NR_OPTION_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetNrOptionMode_V1_0200
 * @tc.name     Get the option mode of NR.
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetNrOptionMode_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNrOptionMode(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NR_OPTION_MODE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetRrcConnectionState_V1_0100
 * @tc.name     Get the rrc connection state
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetRrcConnectionState_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_RRC_CONNECTION_STATE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetRrcConnectionState_V1_0200
 * @tc.name     Get the rrc connection state
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetRrcConnectionState_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_RRC_CONNECTION_STATE));
}

/**
 * @tc.number   Telephony_DriverSystem_GetNrSsbId_V1_0100
 * @tc.name     Get the nr ssb Id information
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetNrSsbId_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNrSsbId(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NR_SSBID_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetNrSsbId_V1_0200
 * @tc.name     Get the nr ssb Id information
 * @tc.desc     Function test
 */
HWTEST_F(RILNetworkTest, Telephony_DriverSystem_GetNrSsbId_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNrSsbId(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_NETWORK_GET_NR_SSBID_INFO));
}
} // namespace Telephony
} // namespace OHOS
