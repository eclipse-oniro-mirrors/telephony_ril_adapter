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

#include "ril_radio_response_test.h"

#include "telephony_log_wrapper.h"

#include "hril_request.h"

namespace OHOS {
namespace Telephony {
RilRadioResponseTest::RilRadioResponseTest(RilManagerTest *mRilManager) : IPCObjectStub(std::u16string(u""))
{
    mRilManager_ = mRilManager;
}

RilRadioResponseTest::~RilRadioResponseTest() {}

int32_t RilRadioResponseTest::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    switch (code) {
        case HREQ_SIM_GET_SIM_STATUS:
            OnResponseGetSimState(data);
            break;
        case HREQ_NETWORK_GET_CURRENT_CELL_INFO:
            OnResponseGetRilCurrentCellInfo(data);
            break;
        case HREQ_NETWORK_SET_NOTIFICATION_FILTER:
            OnRequestSetNotificationFilterTest(data);
            break;
        case HREQ_NETWORK_SET_DEVICE_STATE:
            OnRequestSetDeviceStateTest(data);
            break;
        default:
            break;
    }
    return 0;
}

void RilRadioResponseTest::OnResponseGetSimState(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetSimState --> ");
    std::unique_ptr<CardStatusInfo> cardStatusInfo = std::make_unique<CardStatusInfo>();
    if (cardStatusInfo == nullptr) {
        return;
    }
    if (!cardStatusInfo.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("ERROR : OnResponseGetSimState --> CardStatusInfo.ReadFromParcel(data) failed !!!");
        return;
    }
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetSimState --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetRilCurrentCellInfo(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetRilCurrentCellInfo --> ");
    std::unique_ptr<CurrentCellInfo> currentCellInfo = std::make_unique<CurrentCellInfo>();
    if (currentCellInfo == nullptr) {
        return;
    }
    if (!currentCellInfo.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("ERROR : OnResponseGetRilCurrentCellInfo --> CurrentCellInfo.ReadFromParcel(data) failed !!!");
        return;
    }
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetRilCurrentCellInfo --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetCurrentCalls(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest OnResponseGetCurrentCalls -->");
    std::unique_ptr<CallInfoList> callInfo = std::unique_ptr<CallInfoList>();
    callInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetCurrentCalls -->data.ReadBuffer(readSpSize) failed");
        return;
    }
}

void RilRadioResponseTest::DialResponse(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest DialResponse read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::GetImsiResponse(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::GetImsiResponse --> ");
    const char *buffer = data.ReadCString();
    if (buffer == nullptr) {
        TELEPHONY_LOGE("ERROR : GetImsiResponse --> data.ReadCString() failed !!!");
        return;
    }
    std::unique_ptr<std::string> imsi = std::make_unique<std::string>(buffer);
    if (imsi == nullptr) {
        TELEPHONY_LOGE("ERROR : GetImsiResponse --> imsi is null!!!");
        return;
    }
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : GetImsiResponse --> read spBuffer(HRilRadioResponseInfo) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseHangupCall(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseHangupCall read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseRejectCall(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseRejectCall read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseHoldCall(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseHoldCall read spBuffer failed");
        return;
    }
    TELEPHONY_LOGI("RilRadioResponseTest OnResponseHoldCall back");
}

void RilRadioResponseTest::OnResponseActiveCall(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseActiveCall read spBuffer failed");
        return;
    }

    TELEPHONY_LOGI("RilRadioResponseTest OnResponseActiveCall back ");
}

void RilRadioResponseTest::OnResponseJoinCall(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseJoinCall read spBuffer failed");
        return;
    }

    TELEPHONY_LOGI("RilRadioResponseTest OnResponseJoinCall back ");
}

void RilRadioResponseTest::OnResponseSplitCall(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSplitCall read spBuffer failed");
        return;
    }

    TELEPHONY_LOGI("RilRadioResponseTest OnResponseSplitCall back ");
}

void RilRadioResponseTest::OnResponseSwapCall(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSwapCall read spBuffer failed");
        return;
    }

    TELEPHONY_LOGI("RilRadioResponseTest OnResponseSwapCall back ");
}

void RilRadioResponseTest::OnResponseGetSignalQuality(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest OnResponseGetSignalQuality ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseGetSignalQuality read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetIccIOState(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetIccIOState --> ");
    std::unique_ptr<IccIoResultInfo> iccIoResult = std::make_unique<IccIoResultInfo>();
    if (iccIoResult == nullptr) {
        return;
    }
    if (!iccIoResult.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("OnResponseGetIccIOState --> iccIoResult.ReadFromParcel(data) failed !!!");
        return;
    }
    TELEPHONY_LOGI("OnResponseGetIccIOState --> iccIoResult.ReadFromParcel(data) success.");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetIccIOState --> read spBuffer(HRilRadioResponseInfo) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetCsRegStatus(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetCsRegStatus start ");
    std::unique_ptr<CsRegStatusInfo> mCsRegStatusInfo = std::make_unique<CsRegStatusInfo>();
    if (mCsRegStatusInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter %{public}s: failed to obtain CsRegStatusInfo", __func__);
        return;
    }
    mCsRegStatusInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetCsRegStatus --> read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetNetworkRegistrationState(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetNetworkRegistrationState start, but need struct");
    std::unique_ptr<PsRegStatusResultInfo> psRegStateResult = std::make_unique<PsRegStatusResultInfo>();
    if (psRegStateResult == nullptr) {
        return;
    }
    psRegStateResult.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetNetworkRegistrationState --> read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetCurrentOperator(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest OnResponseGetCurrentOperator ");
    std::unique_ptr<OperatorInfoResult> mOperatorInfo = std::make_unique<OperatorInfoResult>();
    if (mOperatorInfo == nullptr) {
        return;
    }
    mOperatorInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseGetCurrentOperator read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::SetRadioStateResponse(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest SetRadioStateResponse read spBuffer failed");
        return;
    }
    TELEPHONY_LOGI("SetRadioStateResponse");
}

void RilRadioResponseTest::SendSmsResponse(MessageParcel &data)
{
    std::unique_ptr<SendSmsResultInfo> mSendSmsResultInfo = std::make_unique<SendSmsResultInfo>();
    if (mSendSmsResultInfo == nullptr) {
        return;
    }
    mSendSmsResultInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("SendSmsResponse --> read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::SendSmsMoreModeResponse(MessageParcel &data)
{
    std::unique_ptr<SendSmsResultInfo> mSendSmsResultInfo = std::make_unique<SendSmsResultInfo>();
    if (mSendSmsResultInfo == nullptr) {
        return;
    }
    mSendSmsResultInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("SendSmsMoreModeResponse --> read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::ActivatePdpContextResponse(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::ActivatePdpContextResponse --> ");
    std::unique_ptr<SetupDataCallResultInfo> setupDataCallResultInfo = std::make_unique<SetupDataCallResultInfo>();
    if (setupDataCallResultInfo == nullptr) {
        return;
    }
    if (!setupDataCallResultInfo.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE(
            "ERROR : ActivatePdpContextResponse --> setupDataCallResultInfo.ReadFromParcel(data) failed !!!");
        return;
    }
    TELEPHONY_LOGI("ActivatePdpContextResponse --> setupDataCallResultInfo.ReadFromParcel(data) success");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : ActivatePdpContextResponse --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseAcceptRilCmCall(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseAcceptRilCmCall read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseDeactivateDataCall(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseDeactivateDataCall --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseDeactivateDataCall --> ReadBuffer(data) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseSendNewMessageAckCdma(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSendNewMessageAckCdma read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseSetInitialApn(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSetInitialApn read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetImsRegistrationState(MessageParcel &data)
{
    UniInfo mUniversalInfo;
    std::unique_ptr<UniInfo> mUniInfo = std::make_unique<UniInfo>();
    if (mUniInfo == nullptr) {
        return;
    }
    mUniInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseGetImsRegistrationState read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseSendImsSms(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest OnResponseSendImsSms ");
    std::unique_ptr<SendSmsResultInfo> mSendSmsResultInfo = std::make_unique<SendSmsResultInfo>();
    if (mSendSmsResultInfo == nullptr) {
        return;
    }
    mSendSmsResultInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSendImsSms read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseSetDataCallAllowed(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseSetDataCallAllowed --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseSetDataCallAllowed --> ReadBuffer(data) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseSetModemDataProfile(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseSetModemDataProfile --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseSetModemDataProfile --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponsePullRadioLceData(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetNetworkSelectionModeTest(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetNetworkSelectionModeTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}
void RilRadioResponseTest::OnRequestGetNetworkSearchInformationTest(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetNetworkSearchInformationTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestSetNetworkSelectionModeTest(MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestSetNetworkSelectionModeTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestSetNotificationFilterTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestSetNotificationFilterTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestSetNotificationFilterTest --> ");
}

void RilRadioResponseTest::OnRequestSetDeviceStateTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestSetDeviceStateTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestSetDeviceStateTest --> ");
}

void RilRadioResponseTest::OnRequestSetUssdTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestSetUssdTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestSetUssdTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetUssdTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestGetUssdTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetUssdTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetLinkBandwidthInfoTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestGetLinkBandwidthInfoTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetLinkBandwidthInfoTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestSetClipTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestSetClipTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestSetClipTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetClipTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestGetClipTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetClipTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestSetCallWaitingTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestSetCallWaitingTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestSetCallWaitingTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetCallWaitingTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestGetCallWaitingTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetCallWaitingTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestSetCallRestrictionTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestSetCallRestrictionTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestSetCallRestrictionTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetCallRestrictionTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestGetCallRestrictionTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetCallRestrictionTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestSetCallTransferInfoTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestSetCallTransferInfoTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestSetCallTransferInfoTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetCallTransferInfoTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestGetCallTransferInfoTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetCallTransferInfoTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestSetClirTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestSetClirTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestSetClirTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetClirTest(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestGetClirTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetClirTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}
} // namespace Telephony
} // namespace OHOS