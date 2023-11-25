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

#include "ril_data_test.h"
#include "ril_test_util.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_2;
using namespace testing::ext;
namespace {
sptr<OHOS::HDI::Ril::V1_2::IRil> g_rilInterface = nullptr;
}

void RilDataTest::SetUpTestCase()
{
    TELEPHONY_LOGI("----------RilCallTest gtest start ------------");
    RilTestUtil::GetInstance().Init();
    g_rilInterface = RilTestUtil::GetRilInterface();
}

void RilDataTest::TearDownTestCase() {}

void RilDataTest::SetUp() {}

void RilDataTest::TearDown() {}

/**
 * @tc.number   Telephony_DriverSystem_ActivatePdpContext_V1_0100
 * @tc.name     Activate pdp context
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_ActivatePdpContext_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
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
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_ACTIVATE_PDP_CONTEXT));
}

/**
 * @tc.number   Telephony_DriverSystem_ActivatePdpContext_V1_0200
 * @tc.name     Activate pdp context
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_ActivatePdpContext_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
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
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_ACTIVATE_PDP_CONTEXT));
}

/**
 * @tc.number   Telephony_DriverSystem_DeactivatePdpContext_V1_0100
 * @tc.name     Deactivate pdp context
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_DeactivatePdpContext_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = CID;
    uniInfo.arg1 = REASON;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_DEACTIVATE_PDP_CONTEXT));
}

/**
 * @tc.number   Telephony_DriverSystem_DeactivatePdpContext_V1_0200
 * @tc.name     Deactivate pdp context
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_DeactivatePdpContext_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = CID;
    uniInfo.arg1 = REASON;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_DEACTIVATE_PDP_CONTEXT));
}

/**
 * @tc.number   Telephony_DriverSystem_GetPdpContextList_V1_0100
 * @tc.name     Get pdp context list
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_GetPdpContextList_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_1, serialId, uniInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_GET_PDP_CONTEXT_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_GetPdpContextList_V1_0200
 * @tc.name     Get pdp context list
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_GetPdpContextList_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_GET_PDP_CONTEXT_LIST));
}

/**
 * @tc.number   Telephony_DriverSystem_SetInitApnInfo_V1_0100
 * @tc.name     Set init apn info
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_SetInitApnInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
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
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_SET_INIT_APN_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_SetInitApnInfo_V1_0200
 * @tc.name     Set init apn info
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_SetInitApnInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
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
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_SET_INIT_APN_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetLinkCapability_V1_0100
 * @tc.name     Get link capability
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_GetLinkCapability_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_GET_LINK_CAPABILITY);
    EXPECT_GE(ret, -1);
}

/**
 * @tc.number   Telephony_DriverSystem_GetLinkCapability_V1_0200
 * @tc.name     Get link capability
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_GetLinkCapability_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_GET_LINK_CAPABILITY);
    EXPECT_GE(SUCCESS, ret);
}

/**
 * @tc.number   Telephony_DriverSystem_CleanAllConnections_V1_0100
 * @tc.name     Get link capability
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_CleanAllConnections_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CleanAllConnections(SLOTID_1, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_CLEAN_ALL_CONNECTIONS);
    EXPECT_GE(ret, -1);
}

/**
 * @tc.number   Telephony_DriverSystem_CleanAllConnections_V1_0200
 * @tc.name     Get link capability
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_CleanAllConnections_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->CleanAllConnections(SLOTID_2, RilTestUtil::GetSerialId());
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_CLEAN_ALL_CONNECTIONS);
    EXPECT_GE(SUCCESS, ret);
}

/**
 * @tc.number   Telephony_DriverSystem_GetLinkBandwidthInfo_V1_0100
 * @tc.name     Get link band width info
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_GetLinkBandwidthInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_1, RilTestUtil::GetSerialId(), CID);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_GET_LINK_BANDWIDTH_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_GetLinkBandwidthInfo_V1_0200
 * @tc.name     Get link band width info
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_GetLinkBandwidthInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_2, RilTestUtil::GetSerialId(), CID);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_GET_LINK_BANDWIDTH_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_SetLinkBandwidthReportingRule_V1_0100
 * @tc.name     Set link band width reporting rule
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_SetLinkBandwidthReportingRule_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
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
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, RilTestUtil::GetSerialId(), dLinkBandwidth);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetLinkBandwidthReportingRule_V1_0200
 * @tc.name     Set link band width reporting rule
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_SetLinkBandwidthReportingRule_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
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
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, RilTestUtil::GetSerialId(), dLinkBandwidth);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE));
}

/**
 * @tc.number   Telephony_DriverSystem_SetDataPermitted_V1_0100
 * @tc.name     Set data permitted
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_SetDataPermitted_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PERMITTED));
}

/**
 * @tc.number   Telephony_DriverSystem_SetDataPermitted_V1_0200
 * @tc.name     Set data permitted
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_SetDataPermitted_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, RilTestUtil::GetSerialId(), 1);
    RilTestUtil::WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PERMITTED));
}

/**
 * @tc.number   Telephony_DriverSystem_SetDataProfileInfo_V1_0100
 * @tc.name     Set data profile info
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_SetDataProfileInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
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
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PROFILE_INFO));
}

/**
 * @tc.number   Telephony_DriverSystem_SetDataProfileInfo_V1_0200
 * @tc.name     Set data profile info
 * @tc.desc     Function test
 */
HWTEST_F(RilDataTest, Telephony_DriverSystem_SetDataProfileInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!RilTestUtil::IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = RilTestUtil::GetSerialId();
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
    RilTestUtil::WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(RilTestUtil::GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PROFILE_INFO));
}
} // namespace Telephony
} // namespace OHOS
