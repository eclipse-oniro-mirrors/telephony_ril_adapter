/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "ril_manager_test.h"
#include "telephony_log_wrapper.h"

using namespace OHOS::Telephony;

RilRadioIndicationTest::RilRadioIndicationTest(RilManagerTest *mRilManager) : IPCObjectStub(std::u16string(u""))
{
    mRilManager_ = mRilManager;
}

RilRadioIndicationTest::~RilRadioIndicationTest() {}

int32_t RilRadioIndicationTest::OnRemoteRequest(
    uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply, OHOS::MessageOption &option)
{
    const int32_t DEFAULT_VALUE = 1;
    switch (code) {
        case HNOTI_MODEM_RADIO_STATE_UPDATED:
            RadioStateChange(data);
            break;
        case HNOTI_CALL_STATE_UPDATED:
            CallStateChgInd(data);
            break;
        case HNOTI_NETWORK_CS_REG_STATUS_UPDATED:
            NetworkStateNotify(data);
            break;
        case HNOTI_SMS_NEW_SMS:
            NewSmsNotify(data);
            break;
        case HNOTI_SMS_STATUS_REPORT:
            SmsStatusReportNotify(data);
            break;
        default:
            break;
    }
    return DEFAULT_VALUE;
}

void RilRadioIndicationTest::RadioStateChange(OHOS::MessageParcel &data)
{
    (void)data;
}

void RilRadioIndicationTest::CallStateChgInd(OHOS::MessageParcel &data)
{
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NetworkStateNotify(OHOS::MessageParcel &data)
{
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NewSmsNotify(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("NewSmsNotify");
    std::unique_ptr<SmsMessageInfo> smsMessage = std::make_unique<SmsMessageInfo>();
    if (smsMessage == nullptr) {
        return;
    }
    smsMessage.get()->ReadFromParcel(data);
    int32_t indicationType = smsMessage->indicationType;
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::SmsStatusReportNotify(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("SmsStatusReportNotify");
    std::unique_ptr<SmsMessageInfo> smsMessageInfo = std::make_unique<SmsMessageInfo>();
    if (smsMessageInfo == nullptr) {
        return;
    }
    smsMessageInfo.get()->ReadFromParcel(data);
    int32_t indicationType = smsMessageInfo.get()->indicationType;
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NewSmsStoredOnSimNotify(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioIndicationTest::NewSmsStoredOnSimNotify --> ");
    data.ReadInt32();
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::GetSignalStrength(OHOS::MessageParcel &data)
{
    size_t readSize = sizeof(struct Rssi);
    const uint8_t *buffer = data.ReadBuffer(readSize);
    if (buffer == nullptr) {
        TELEPHONY_LOGE("GetSignalStrength MessageParcel read buffer failed");
        return;
    }
    const struct Rssi *rssi = reinterpret_cast<const struct Rssi *>(buffer);
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
    struct std::unique_ptr<Rssi> mSignalStrength = std::make_unique<Rssi>();
    if (mSignalStrength == nullptr) {
        return;
    }
    errno_t err = memcpy_s(mSignalStrength.get(), readSize, rssi, readSize);
    if (err != EOK) {
        TELEPHONY_LOGE("GetSignalStrength memcpy_s failed");
    }
}

void RilRadioIndicationTest::ChangedDataCallList(OHOS::MessageParcel &data)
{
    std::unique_ptr<SetupDataCallResultInfo> setupDataCallResultInfo = std::make_unique<SetupDataCallResultInfo>();
    if (setupDataCallResultInfo == nullptr) {
        return;
    }
    setupDataCallResultInfo.get()->ReadFromParcel(data);
    const size_t READ_SP_SIZE = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *SP_BUFFER = data.ReadBuffer(READ_SP_SIZE);
    if (SP_BUFFER == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest ChangedDataCallList read SP_BUFFER failed");
        return;
    }
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::ChangedSimState(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioIndicationTest::ChangedSimState --> ");
    int32_t indicationType = data.ReadInt32();
    if (mRilManager_ == nullptr) {
        TELEPHONY_LOGE("mRilManager_ is null!");
        return;
    }
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::ConnectedReturnRilVersion(OHOS::MessageParcel &data)
{
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
    auto mResult_ = AppExecFwk::InnerEvent::Pointer(nullptr, nullptr);
}

void RilRadioIndicationTest::ChangedImsNetworkState(OHOS::MessageParcel &data)
{
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGD("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}