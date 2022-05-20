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

#include "ril_radio_indication_test.h"

#include "ril_manager_test.h"
#include "telephony_log_wrapper.h"

#include "hril_notification.h"

#include <iostream>

using namespace std;
using namespace OHOS::Telephony;

RilRadioIndicationTest::RilRadioIndicationTest(RilManagerTest *mRilManager) : IPCObjectStub(std::u16string(u""))
{
    mRilManager_ = mRilManager;
}

RilRadioIndicationTest::~RilRadioIndicationTest() {}

int32_t RilRadioIndicationTest::OnRemoteRequest(
    uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply, OHOS::MessageOption &option)
{
    int32_t slotId = data.ReadInt32();
    TELEPHONY_LOGI("RilAdapterTest ntf OnRemoteRequest code:%{public}d, slotId:%{public}d", code, slotId);
    if (slotId != HRIL_SIM_SLOT_0) {
        TELEPHONY_LOGE("RilAdapterTest ntf slotid abnormal");
    }
    const int32_t DEFAULT_VALUE = HRIL_ERR_SUCCESS;
    switch (code) {
        case HNOTI_MODEM_RADIO_STATE_UPDATED:
            RadioStateChange(data);
            break;
        case HNOTI_CALL_STATE_UPDATED:
            CallStateUpdated(data);
            break;
        case HNOTI_CALL_CRING_REPORT:
            CallCRingReport(data);
            break;
        case HNOTI_CALL_RINGBACK_VOICE_REPORT:
            CallRingbackVoiceReport(data);
            break;
        case HNOTI_CALL_SRVCC_STATUS_REPORT:
            CallSrvccStatusReport(data);
            break;
        case HNOTI_CALL_EMERGENCY_NUMBER_REPORT:
            CallEmergencyNumberReport(data);
            break;
        case HNOTI_NETWORK_CS_REG_STATUS_UPDATED:
            NetworkStateNotify(data);
            break;
        case HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED:
            GetSignalStrength(data);
            break;
        case HNOTI_NETWORK_IMS_REG_STATUS_UPDATED:
            ChangedImsNetworkState(data);
            break;
        case HNOTI_SMS_NEW_SMS:
            NewSmsNotify(data);
            break;
        case HNOTI_SMS_NEW_CDMA_SMS:
            NewCdmaSmsNotify(data);
            break;
        case HNOTI_SMS_STATUS_REPORT:
            SmsStatusReportNotify(data);
            break;
        case HNOTI_SIM_STATUS_CHANGED:
            SimStateChanged(data);
            break;
        case HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED:
            PdpContextListChangedNotify(data);
            break;
        case HNOTI_SIM_REFRESH_NOTIFY:
            SimRefreshNotify(data);
        case HNOTI_CALL_SS_REPORT:
            CallSsReport(data);
            break;
        default:
            break;
    }
    return DEFAULT_VALUE;
}

void RilRadioIndicationTest::RadioStateChange(OHOS::MessageParcel &data)
{
    int32_t radioState = data.ReadInt32();
    int32_t indicationType = data.ReadInt32();
    std::cout << "---->[NTF] RadioStateChange:" << endl
        << "====> [radioState]: " << radioState << endl;
    TELEPHONY_LOGI(
        "func :%{public}s indicationType: %{public}d state:%{public}d", __func__, indicationType, radioState);
}

void RilRadioIndicationTest::CallStateUpdated(OHOS::MessageParcel &data)
{
    cout << endl << "---->[NTF] CallStateUpdated" << endl;
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::CallCRingReport(OHOS::MessageParcel &data)
{
    cout << endl << "---->[NTF] CallCRingReport: RING!" << endl;
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::CallRingbackVoiceReport(OHOS::MessageParcel &data)
{
    cout << endl << "---->[NTF] CallRingbackVoiceReport: " << endl;
    std::shared_ptr<RingbackVoice> ringbackVoice = std::make_shared<RingbackVoice>();
    if (ringbackVoice == nullptr) {
        TELEPHONY_LOGE("ERROR : ringbackVoice == nullptr !!!");
        return;
    }
    ringbackVoice->ReadFromParcel(data);
    cout << "====>status: " << ringbackVoice->status << endl;
}

void RilRadioIndicationTest::CallSrvccStatusReport(OHOS::MessageParcel &data)
{
    cout << endl << "---->[NTF] CallSrvccStatusReport: " << endl;
    std::shared_ptr<SrvccStatus> srvccStatus = std::make_shared<SrvccStatus>();
    if (srvccStatus == nullptr) {
        TELEPHONY_LOGE("ERROR : srvccStatus == nullptr !!!");
        return;
    }
    srvccStatus->ReadFromParcel(data);
    cout << "====>status: " << srvccStatus->status << endl;
}

void RilRadioIndicationTest::CallEmergencyNumberReport(OHOS::MessageParcel &data)
{
    cout << endl << "---->[NTF] CallEmergencyNumberReport: " << endl;
    std::shared_ptr<EmergencyInfo> emcInfo = std::make_shared<EmergencyInfo>();
    if (emcInfo == nullptr) {
        TELEPHONY_LOGE("ERROR : emcInfo == nullptr !!!");
        return;
    }
    emcInfo->ReadFromParcel(data);
    cout << "====> [index]: " << emcInfo->index << "/" << emcInfo->total
        << "\tcategory: " << emcInfo->category
        << "\tmcc: " << emcInfo->mcc
        << "\teccNum: " << emcInfo->eccNum << endl;
}

void RilRadioIndicationTest::NetworkStateNotify(OHOS::MessageParcel &data)
{
    cout << endl << "---->[NTF] NetworkStateNotify" << endl;
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NewSmsNotify(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("entry");
    unique_ptr<SmsMessageInfo> smsMessageInfo = make_unique<SmsMessageInfo>();
    if (smsMessageInfo == nullptr) {
        return;
    }
    smsMessageInfo.get()->ReadFromParcel(data);
    int32_t indicationType = smsMessageInfo->indicationType;
    cout << "---->[NTF] NewSms:" << endl
        << "====> [indicationType]: " << smsMessageInfo->indicationType << endl
        << "====> [size]: " << smsMessageInfo->size << endl
        << "====> [pdu]: ";
    for (int i = 0; i < smsMessageInfo->pdu.size(); i++) {
        printf("%02x", smsMessageInfo->pdu[i]);
    }
    cout << endl;
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NewCdmaSmsNotify(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("NewCdmaSmsNotify");
    std::unique_ptr<CdmaSmsInfo> smsMessageInfo = std::make_unique<CdmaSmsInfo>();
    if (smsMessageInfo == nullptr) {
        return;
    }
    smsMessageInfo.get()->ReadFromParcel(data);
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::SmsStatusReportNotify(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("entry");
    unique_ptr<SmsMessageInfo> smsMessageInfo = make_unique<SmsMessageInfo>();
    if (smsMessageInfo == nullptr) {
        return;
    }
    smsMessageInfo.get()->ReadFromParcel(data);
    int32_t indicationType = smsMessageInfo.get()->indicationType;
    cout << "---->[NTF] SmsStatusReport:" << endl
        << "====> [indicationType]: " << smsMessageInfo->indicationType << endl
        << "====> [size]: " << smsMessageInfo->size << endl
        << "====> [pdu]: ";
    for (int i = 0; i < smsMessageInfo->pdu.size(); i++) {
        printf("%02x", smsMessageInfo->pdu[i]);
    }
    cout << endl;
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::NewSmsStoredOnSimNotify(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioIndicationTest::NewSmsStoredOnSimNotify --> ");
    data.ReadInt32();
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
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
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
    struct std::unique_ptr<Rssi> mSignalStrength = std::make_unique<Rssi>();
    if (mSignalStrength == nullptr) {
        return;
    }
    if (memcpy_s(mSignalStrength.get(), readSize, rssi, readSize) != EOK) {
        TELEPHONY_LOGE("GetSignalStrength memcpy_s failed");
        return;
    }

    cout << "---->[NTF] Network Signal Strength updated:" << endl;
    cout << "====> [lte.rsrp]: " << mSignalStrength->lte.rsrp << endl;
}

void RilRadioIndicationTest::PdpContextListChangedNotify(OHOS::MessageParcel &data)
{
    std::unique_ptr<DataCallResultList> dataCallList = std::make_unique<DataCallResultList>();
    if (dataCallList == nullptr) {
        TELEPHONY_LOGE("dataCallList is null ptr!");
        return;
    }
    if (!dataCallList.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE(
            "ERROR : PdpContextListChangedNotify --> dataCallList.ReadFromParcel(data) failed !!!");
        return;
    }
    TELEPHONY_LOGI("PdpContextListChangedNotify --> dataCallList.ReadFromParcel(data) success");

    for (auto &setupDataCallResultInfo : dataCallList->dcList) {
        cout << "PdpContextListChangedNotify: flag = " << setupDataCallResultInfo.flag << endl;
        cout << "PdpContextListChangedNotify: reason = " << setupDataCallResultInfo.reason << endl;
        cout << "PdpContextListChangedNotify: retryTime = " << setupDataCallResultInfo.retryTime << endl;
        cout << "PdpContextListChangedNotify: cid = " << setupDataCallResultInfo.cid << endl;
        cout << "PdpContextListChangedNotify: active = " << setupDataCallResultInfo.active << endl;
        cout << "PdpContextListChangedNotify: type = " << setupDataCallResultInfo.type << endl;
        cout << "PdpContextListChangedNotify: netPortName = " << setupDataCallResultInfo.netPortName << endl;
        cout << "PdpContextListChangedNotify: address = " << setupDataCallResultInfo.address << endl;
        cout << "PdpContextListChangedNotify: dns = " << setupDataCallResultInfo.dns << endl;
        cout << "PdpContextListChangedNotify: dnsSec = " << setupDataCallResultInfo.dnsSec << endl;
        cout << "PdpContextListChangedNotify: gateway = " << setupDataCallResultInfo.gateway << endl;
        cout << "PdpContextListChangedNotify: pCscfPrimAddr = " << setupDataCallResultInfo.pCscfPrimAddr << endl;
        cout << "PdpContextListChangedNotify: pCscfSecAddr = " << setupDataCallResultInfo.pCscfSecAddr << endl;
        cout << "PdpContextListChangedNotify: maxTransferUnit = " << setupDataCallResultInfo.maxTransferUnit << endl;
    }
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : PdpContextListChangedNotify --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }

    int32_t indicationType = data.ReadInt32();
    cout << "PdpContextListChangedNotify: indicate type =" << indicationType << endl;
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::SimStateChanged(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioIndicationTest::SimStateChanged --> ");
    cout << "---->[NTF] Sim State Changed" << endl;
    int32_t indicationType = data.ReadInt32();
    if (mRilManager_ == nullptr) {
        TELEPHONY_LOGE("mRilManager_ is null!");
        return;
    }
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::ConnectedReturnRilVersion(OHOS::MessageParcel &data)
{
    int32_t indicationType = data.ReadInt32();
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
    auto mResult_ = AppExecFwk::InnerEvent::Pointer(nullptr, nullptr);
}

void RilRadioIndicationTest::ChangedImsNetworkState(OHOS::MessageParcel &data)
{
    cout << endl << "---->[NTF] ImsNetworkState: " << endl;
    std::shared_ptr<ImsRegStatusInfo> imsState = std::make_shared<ImsRegStatusInfo>();
    if (imsState == nullptr) {
        TELEPHONY_LOGE("ERROR : imsState == nullptr !!!");
        return;
    }
    imsState->ReadFromParcel(data);
    cout << "====>ims reg status: " << imsState->regInfo << endl;
}

void RilRadioIndicationTest::SimRefreshNotify(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioIndicationTest::SimRefreshNotify --> ");
    cout << "---->[NTF] ICC Refresh Changed" << endl;
    int32_t indicationType = data.ReadInt32();
    if (mRilManager_ == nullptr) {
        TELEPHONY_LOGE("mRilManager_ is null!");
        return;
    }
    TELEPHONY_LOGI("func :%{public}s indicationType: %{public}d", __func__, indicationType);
}

void RilRadioIndicationTest::CallSsReport(OHOS::MessageParcel &data)
{
    cout << endl << "---->[NTF] CallSsReport: " << endl;
    std::shared_ptr<SsNoticeInfo> ssInfo = std::make_shared<SsNoticeInfo>();
    if (ssInfo == nullptr) {
        TELEPHONY_LOGE("ERROR : ssInfo == nullptr !!!");
        return;
    }
    ssInfo->ReadFromParcel(data);
    cout << "====> serviceType: " << ssInfo->serviceType
        << "\trequestType: " << ssInfo->requestType
        << "\tserviceClass: " << ssInfo->serviceClass
        << "\tresult: " << ssInfo->result << endl;
}
