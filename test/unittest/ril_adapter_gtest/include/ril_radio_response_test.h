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

#ifndef OHOS_RIL_RADIO_RESPONSE_test_H
#define OHOS_RIL_RADIO_RESPONSE_test_H

#include <iservice_registry.h>

#include "ril_manager_test.h"

namespace OHOS {
namespace Telephony {
struct Tmp1 {
    int serial;
    int32_t status;
};

class RilRadioResponseTest : public IPCObjectStub {
public:
    RilRadioResponseTest(RilManagerTest *mRilManager);
    ~RilRadioResponseTest();

    int OnRemoteRequest(
        uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) override;
    void OnResponseGetSimState(MessageParcel &data);
    void OnResponseGetCurrentCalls(MessageParcel &data);
    void OnResponseGetRilCurrentCellInfo(MessageParcel &data);
    void DialResponse(MessageParcel &data);
    void GetImsiResponse(MessageParcel &data);
    void OnResponseHangupCall(MessageParcel &data);
    void OnResponseRejectCall(MessageParcel &data);
    void OnResponseHoldCall(MessageParcel &data);
    void OnResponseActiveCall(MessageParcel &data);
    void OnResponseSwapCall(MessageParcel &data);
    void OnResponseJoinCall(MessageParcel &data);
    void OnResponseSplitCall(MessageParcel &data);
    void OnResponseGetLastCallErrorCode(MessageParcel &data);
    void OnResponseGetSignalQuality(MessageParcel &data);
    void OnResponseGetCsRegStatus(MessageParcel &data);
    void OnResponseGetNetworkRegistrationState(MessageParcel &data);
    void OnResponseGetCurrentOperator(MessageParcel &data);
    void SetRadioStateResponse(MessageParcel &data);
    void SendSmsResponse(MessageParcel &data);
    void SendSmsMoreModeResponse(MessageParcel &data);
    void ActivatePdpContextResponse(MessageParcel &data);
    void OnResponseGetIccIOState(MessageParcel &data);
    void OnResponseAcceptRilCmCall(MessageParcel &data);
    void OnResponseDeactivateDataCall(MessageParcel &data);
    void OnResponseSendNewMessageAckCdma(MessageParcel &data);
    void OnResponseSetInitialApn(MessageParcel &data);
    void OnResponseGetImsRegistrationState(MessageParcel &data);
    void OnResponseSendImsSms(MessageParcel &data);
    void OnResponseSetDataCallAllowed(MessageParcel &data);
    void OnResponseSetModemDataProfile(MessageParcel &data);
    void OnResponsePullRadioLceData(MessageParcel &data);
    void OnRequestGetNetworkSelectionModeTest(MessageParcel &data);
    void OnRequestGetNetworkSearchInformationTest(MessageParcel &data);
    void OnRequestSetNetworkSelectionModeTest(MessageParcel &data);
    void OnRequestSetUssdCusdTest(MessageParcel &data);
    void OnRequestGetUssdCusdTest(MessageParcel &data);
    void OnRequestGetLinkBandwidthInfoTest(MessageParcel &data);

private:
    RilManagerTest *mRilManager_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_RADIO_RESPONSE_test_H