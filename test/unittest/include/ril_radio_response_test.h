/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_RIL_RADIO_RESPONSE_TEST_H
#define OHOS_RIL_RADIO_RESPONSE_TEST_H

#include <ipc_object_stub.h>

#include "ril_manager_test.h"
namespace OHOS {
namespace Telephony {
struct Tmp1 {
    int32_t serial;
    int32_t status;
};

class RilRadioResponseTest : public OHOS::IPCObjectStub {
public:
    RilRadioResponseTest(RilManagerTest *mRilManager);
    ~RilRadioResponseTest();

    int32_t OnRemoteRequest(
        uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply, OHOS::MessageOption &option) override;

    void OnResponseNullPara(std::string command, OHOS::MessageParcel &data);

    void OnResponseGetCallList(OHOS::MessageParcel &data);
    void OnResponseGetMute(OHOS::MessageParcel &data);
    void OnResponseGetEmergencyList(OHOS::MessageParcel &data);
    void OnResponseGetFailReason(OHOS::MessageParcel &data);

    void OnResponseGetImsi(OHOS::MessageParcel &data);
    void GetImsiResponse(OHOS::MessageParcel &data);
    void OnResponseGetRilCurrentCellInfo(OHOS::MessageParcel &data);
    void OnResponseGetSimState(OHOS::MessageParcel &data);
    void OnResponseGetSimIo(OHOS::MessageParcel &data);
    void OnResponseGetRilNeighboringCellInfoList(OHOS::MessageParcel &data);
    void OnResponseGetSignalQuality(OHOS::MessageParcel &data);
    void OnResponseGetCsRegStatus(OHOS::MessageParcel &data);
    void OnResponseGetNetworkRegistrationState(OHOS::MessageParcel &data);
    void OnResponseGetCurrentOperator(OHOS::MessageParcel &data);
    void OnResponseSetRadioStateResponse(OHOS::MessageParcel &data);
    void OnResponseGetRadioState(OHOS::MessageParcel &data);
    void OnResponseGetImei(OHOS::MessageParcel &data);
    void SendSmsResponse(OHOS::MessageParcel &data);
    void SendSmsMoreModeResponse(OHOS::MessageParcel &data);
    void OnResponseGetIccIOState(OHOS::MessageParcel &data);
    void OnResponseActivateDataCall(OHOS::MessageParcel &data);
    void OnResponseDeactivateDataCall(OHOS::MessageParcel &data);
    void OnResponseGetPdpContextList(OHOS::MessageParcel &data);
    void OnResponseSendNewMessageAckCdma(OHOS::MessageParcel &data);
    void OnResponseSetInitialApn(OHOS::MessageParcel &data);
    void OnResponseSendImsSms(OHOS::MessageParcel &data);
    void OnResponseSetDataCallAllowed(OHOS::MessageParcel &data);
    void OnResponseSetModemDataProfile(OHOS::MessageParcel &data);
    void OnResponsePullRadioLceData(OHOS::MessageParcel &data);
    void OnRequestGetNetworkSelectionModeTest(OHOS::MessageParcel &data);
    void OnRequestGetNetworkSearchInformationTest(OHOS::MessageParcel &data);
    void OnRequestSetNetworkPreferredNetworkModeTest(OHOS::MessageParcel &data);
    void OnRequestGetNetworkPreferredNetworkModeTest(OHOS::MessageParcel &data);
    void OnRequestSetLocationUpdatesTest(OHOS::MessageParcel &data);
    void OnRequestSetNotificationFilterTest(OHOS::MessageParcel &data);
    void OnRequestSetDeviceStateTest(OHOS::MessageParcel &data);
    void OnRequestGetPreferredNetworkTypeInfoTest(OHOS::MessageParcel &data);
    void OnRequestGetMeidTest(OHOS::MessageParcel &data);
    void OnRequestGetModemVoiceRadioTest(OHOS::MessageParcel &data);
    void OnRequestGetBasebandVersionTest(OHOS::MessageParcel &data);
    void OnRequestSetNetworkSelectionModeTest(OHOS::MessageParcel &data);
    void OnRequestGetLinkBandwidthInfoTest(OHOS::MessageParcel &data);
    void OnResponseSendRilCmSms(OHOS::MessageParcel &data);
    void OnResponseSendRilCmSmsMoreMode(OHOS::MessageParcel &data);
    void OnResponseSendSmsAck(OHOS::MessageParcel &data);
    void OnResponseGetSmscAddr(OHOS::MessageParcel &data);
    void OnResponseSetSmscAddr(OHOS::MessageParcel &data);
    void OnResponseLockStatus(OHOS::MessageParcel &data);
    void OnResponseOpenLogicalChannel(OHOS::MessageParcel &data);
    void OnResponseSimTransmitApduChannelTest(OHOS::MessageParcel &data);
    void OnResponseGetSimLockStatus(OHOS::MessageParcel &data);

private:
    RilManagerTest *mRilManager_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_RADIO_RESPONSE_test_H