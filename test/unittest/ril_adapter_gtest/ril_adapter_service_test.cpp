/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
#define protected public

#include "gtest/gtest.h"
#include "hril_timer_callback.h"
#include "hril_event.h"
#include "hril_request.h"
#include "hril_enum.h"
#include "hril_base.h"
#include "hril_sms.h"
#include "hril_manager.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;

class RilAdapterServiceTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void RilAdapterServiceTest::SetUpTestCase() {}

void RilAdapterServiceTest::TearDownTestCase() {}

void RilAdapterServiceTest::SetUp() {}

void RilAdapterServiceTest::TearDown() {}

/**
 * @tc.number   Telephony_ImsBaseConnectionTest_0001
 * @tc.name     Test HRilBase
 * @tc.desc     Function test
 */
HWTEST_F(RilAdapterServiceTest, HRilBaseTest_0001, Function | MediumTest | Level1)
{
    char **response = nullptr;
    int32_t index = 0;
    int32_t len = 0;
    int32_t slotId = 0;
    HRilBase base(slotId);
    EXPECT_EQ(base.ConvertHexStringToInt(response, index, len), HRIL_ERR_GENERIC_FAILURE);

    HDI::Ril::V1_1::RilRadioResponseInfo responseInfo;
    responseInfo.slotId = slotId;
    base.BuildIHRilRadioResponseInfo(responseInfo);

    char str[] = "0123456789ABCDEF";
    EXPECT_EQ(base.StringToHex(str, strlen(str)), "30313233343536373839414243444546");
}

/**
 * @tc.number   Telephony_HRilSmsTest_0002
 * @tc.name     Test HRilSms
 * @tc.desc     Function test
 */
HWTEST_F(RilAdapterServiceTest, HRilSmsTest_0002, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    HRilSms hRilSms(slotId);
    EXPECT_FALSE(hRilSms.IsSmsRespOrNotify(HREQ_CALL_BASE));
    EXPECT_TRUE(hRilSms.IsSmsRespOrNotify(HREQ_SMS_BASE));
    EXPECT_FALSE(hRilSms.IsSmsRespOrNotify(HREQ_SIM_BASE));
    EXPECT_TRUE(hRilSms.IsSmsRespOrNotify(HNOTI_SMS_BASE));
    EXPECT_FALSE(hRilSms.IsSmsRespOrNotify(HNOTI_SIM_BASE));

    int32_t serialId = 0;
    OHOS::HDI::Ril::V1_1::GsmSmsMessageInfo gsmSmsMessageInfo;
    hRilSms.SendGsmSms(serialId, gsmSmsMessageInfo);

    OHOS::HDI::Ril::V1_1::SendCdmaSmsMessageInfo cdmaSmsMessageInfo;
    hRilSms.SendCdmaSms(serialId, cdmaSmsMessageInfo);

    int32_t index = 0;
    HRilSmsReq smsFuncs;
    hRilSms.RegisterSmsFuncs(&smsFuncs);
    EXPECT_NE(hRilSms.DelSimMessage(serialId, index), HRIL_ERR_INVALID_PARAMETER);

    OHOS::HDI::Ril::V1_1::CdmaCBConfigInfoList cellBroadcastInfoList;
    EXPECT_NE(hRilSms.SetCdmaCBConfig(serialId, cellBroadcastInfoList), HRIL_ERR_INVALID_PARAMETER);

    hRilSms.GetCdmaCBConfig(serialId);

    OHOS::HDI::Ril::V1_1::ServiceCenterAddress scAddress;
    hRilSms.SetSmscAddr(serialId, scAddress);
    hRilSms.GetSmscAddr(serialId);

    OHOS::HDI::Ril::V1_1::CBConfigInfo broadcastInfo;
    std::string str(60000, 'a');
    broadcastInfo.mids = str;
    EXPECT_EQ(hRilSms.SetCBConfig(serialId, broadcastInfo), HRIL_ERR_INVALID_PARAMETER);
}

/**
 * @tc.number   Telephony_HRilSmsTest_0003
 * @tc.name     Test HRilSms
 * @tc.desc     Function test
 */
HWTEST_F(RilAdapterServiceTest, HRilSmsTest_0003, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    HRilSms hRilSms(slotId);
    int32_t serialId = 0;
    OHOS::HDI::Ril::V1_1::ModeData gsmSmsMessageInfo;
    EXPECT_NE(hRilSms.SendSmsAck(serialId, gsmSmsMessageInfo), HRIL_ERR_INVALID_PARAMETER);

    OHOS::HDI::Ril::V1_1::SmsMessageIOInfo message;
    message.pdu = "0123456789abcdefABCDEF";
    hRilSms.AddCdmaSimMessage(serialId, message);

    HRilCdmaSmsMessageInfo cdmaSmsInfo;
    hRilSms.CreateCdmaMessageInfo(cdmaSmsInfo, message.pdu);

    int32_t index = 0;
    HRilSmsReq smsFuncs;
    hRilSms.RegisterSmsFuncs(&smsFuncs);
    EXPECT_NE(hRilSms.DelCdmaSimMessage(serialId, index), HRIL_ERR_INVALID_PARAMETER);

    int32_t indType = 0;
    HRilErrNumber error = HRIL_ERR_HDF_IPC_FAILURE;
    char response[] = {"213234343345"} ;
    size_t responseLen = strlen(response);
    hRilSms.CBConfigNotify(indType, error, response, responseLen);

    EXPECT_FALSE(hRilSms.IsSmsResponse(HREQ_CALL_BASE));
    EXPECT_TRUE(hRilSms.IsSmsResponse(HREQ_SMS_BASE));
    EXPECT_FALSE(hRilSms.IsSmsResponse(HREQ_SIM_BASE));

    EXPECT_FALSE(hRilSms.IsSmsNotification(HREQ_SIM_BASE));
    EXPECT_TRUE(hRilSms.IsSmsNotification(HNOTI_SMS_BASE));
    EXPECT_FALSE(hRilSms.IsSmsNotification(HNOTI_SIM_BASE));
}
/**
 * @tc.number   Telephony_RilAdapterServiceTest_0004
 * @tc.name     Test RilAdapterService
 * @tc.desc     Function test
 */
HWTEST_F(RilAdapterServiceTest, RilAdapterServiceTest_0004, Function | MediumTest | Level1)
{
    HRilTimerCallback hRilTimerCallback;
    int32_t fd = 0;
    int16_t events = 0;
    std::shared_ptr<void> param = std::make_shared<int>(42);
    hRilTimerCallback.TimerCallback(fd, events, param);
    ASSERT_NE(param, nullptr);

    param = nullptr;
    hRilTimerCallback.TimerCallback(fd, events, param);
    ASSERT_EQ(param, nullptr);
}

/**
 * @tc.number   Telephony_RilAdapterServiceTest_0005
 * @tc.name     Test RilAdapterService
 * @tc.desc     Function test
 */
HWTEST_F(RilAdapterServiceTest, RilAdapterServiceTest_0005, Function | MediumTest | Level1)
{
    HRilTimerCallback hRilTimerCallback;
    HRilCallbackFun func = nullptr;
    uint8_t *param = nullptr;
    timeval *tv = nullptr;
    ASSERT_EQ(hRilTimerCallback.HRilSetTimerCallbackInfo(func, param, tv), nullptr);
}

/**
 * @tc.number   Telephony_HRilEventTest_0006
 * @tc.name     Test HRilEvent
 * @tc.desc     Function test
 */
HWTEST_F(RilAdapterServiceTest, HRilEventTest_0006, Function | MediumTest | Level1)
{
    HRilEvent hRilEvent;
    int32_t index = 0;
    hRilEvent.ProcessEvents(nullptr, index);
    EXPECT_EQ(index, 0);
}
} // namespace Telephony
} // namespace OHOS