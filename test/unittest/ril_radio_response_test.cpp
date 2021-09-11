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

using namespace OHOS::Telephony;
RilRadioResponseTest::RilRadioResponseTest(RilManagerTest *mRilManager) : IPCObjectStub(std::u16string(u""))
{
    mRilManager_ = mRilManager;
}

RilRadioResponseTest::~RilRadioResponseTest() {}

int32_t RilRadioResponseTest::OnRemoteRequest(
    uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply, OHOS::MessageOption &option)
{
    switch (code) {
        case HREQ_SIM_GET_SIM_STATUS:
            OnResponseGetSimState(data);
            break;
        case HREQ_NETWORK_GET_CURRENT_CELL_INFORMATION:
            OnResponseGetRilCurrentCellInfo(data);
            break;
        default:
            break;
    }
    return 0;
}

void RilRadioResponseTest::OnResponseGetSimState(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::OnResponseGetSimState --> ");
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

void RilRadioResponseTest::OnResponseGetRilCurrentCellInfo(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::OnResponseGetRilCurrentCellInfo --> ");
    std::unique_ptr<CurrentCellInfo> currentCellInfo = std::make_unique<CurrentCellInfo>();
    if (currentCellInfo == nullptr) {
        return;
    }
    if (!currentCellInfo.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE(
            "ERROR : OnResponseGetRilCurrentCellInfo --> CurrentCellInfo.ReadFromParcel(data) failed !!!");
        return;
    }
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetRilCurrentCellInfo --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetCurrentCalls(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest OnResponseGetCurrentCalls -->");
    std::unique_ptr<CallInfoList> callInfo = std::unique_ptr<CallInfoList>();
    callInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetCurrentCalls -->data.ReadBuffer(readSpSize) failed");
        return;
    }
}

void RilRadioResponseTest::DialResponse(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest DialResponse read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::GetImsiResponse(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::GetImsiResponse --> ");
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

void RilRadioResponseTest::OnResponseHangupCall(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseHangupCall read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseRejectCall(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseRejectCall read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseHoldCall(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseHoldCall read spBuffer failed");
        return;
    }
    TELEPHONY_LOGD("RilRadioResponseTest OnResponseHoldCall back");
}

void RilRadioResponseTest::OnResponseActiveCall(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseActiveCall read spBuffer failed");
        return;
    }

    TELEPHONY_LOGD("RilRadioResponseTest OnResponseActiveCall back ");
}

void RilRadioResponseTest::OnResponseJoinCall(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseJoinCall OnResponseJoinCall read spBuffer failed");
        return;
    }

    TELEPHONY_LOGD("OnResponseJoinCall OnResponseJoinCall back ");
}

void RilRadioResponseTest::OnResponseSplitCall(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseSplitCall OnResponseSplitCall read spBuffer failed");
        return;
    }

    TELEPHONY_LOGD("OnResponseSplitCall OnResponseSplitCall back ");
}

void RilRadioResponseTest::OnResponseSwapCall(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSwapCall read spBuffer failed");
        return;
    }

    TELEPHONY_LOGD("RilRadioResponseTest OnResponseSwapCall back ");
}

void RilRadioResponseTest::OnResponseGetSignalQuality(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest OnResponseGetSignalQuality ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseActiveCall read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetIccIOState(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::OnResponseGetIccIOState --> ");
    std::unique_ptr<IccIoResultInfo> iccIoResult = std::make_unique<IccIoResultInfo>();
    if (iccIoResult == nullptr) {
        return;
    }
    if (!iccIoResult.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("OnResponseGetIccIOState --> iccIoResult.ReadFromParcel(data) failed !!!");
        return;
    }
    TELEPHONY_LOGD("OnResponseGetIccIOState --> iccIoResult.ReadFromParcel(data) success.");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetIccIOState --> read spBuffer(HRilRadioResponseInfo) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetCsRegStatus(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::OnResponseGetCsRegStatus start ");
    std::unique_ptr<CsRegStatusInfo> mCsRegStatusInfo = std::make_unique<CsRegStatusInfo>();
    if (mCsRegStatusInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter %{public}s: failed to obtain CsRegStatusInfo", __func__);
        return;
    }
    mCsRegStatusInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetIccIOState --> read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetNetworkRegistrationState(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::OnResponseGetNetworkRegistrationState start, but need struct");
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

void RilRadioResponseTest::OnResponseGetCurrentOperator(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest OnResponseGetCurrentOperator ");
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

void RilRadioResponseTest::SetRadioStatusResponse(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest SetRadioStatusResponse read spBuffer failed");
        return;
    }
    TELEPHONY_LOGD("SetRadioStatusResponse");
}

void RilRadioResponseTest::SendSmsResponse(OHOS::MessageParcel &data)
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

void RilRadioResponseTest::SendSmsMoreModeResponse(OHOS::MessageParcel &data)
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

void RilRadioResponseTest::ActivatePdpContextResponse(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::ActivatePdpContextResponse --> ");
    std::unique_ptr<SetupDataCallResultInfo> setupDataCallResultInfo = std::make_unique<SetupDataCallResultInfo>();
    if (setupDataCallResultInfo == nullptr) {
        return;
    }
    if (!setupDataCallResultInfo.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE(
            "ERROR : ActivatePdpContextResponse --> setupDataCallResultInfo.ReadFromParcel(data) failed !!!");
        return;
    }
    TELEPHONY_LOGD("ActivatePdpContextResponse --> setupDataCallResultInfo.ReadFromParcel(data) success");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : ActivatePdpContextResponse --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseAcceptRilCmCall(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseAcceptRilCmCall read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseDeactivateDataCall(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::OnResponseDeactivateDataCall --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseDeactivateDataCall --> ReadBuffer(data) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseSendNewMessageAckCdma(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSendNewMessageAckCdma read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseSetInitialApn(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSetInitialApn read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetImsRegistrationState(OHOS::MessageParcel &data)
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

void RilRadioResponseTest::OnResponseSendImsSms(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest OnResponseSendImsSms ");
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

void RilRadioResponseTest::OnResponseSetDataCallAllowed(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::OnResponseSetDataCallAllowed --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseSetDataCallAllowed --> ReadBuffer(data) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseSetModemDataProfile(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGD("RilRadioResponseTest::OnResponseSetModemDataProfile --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseSetModemDataProfile --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponsePullRadioLceData(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetNetworkSelectionModeTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}
void RilRadioResponseTest::OnRequestGetNetworkSearchInformationTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestSetNetworkSelectionModeTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}