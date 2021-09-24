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

#include "ril_manager_test.h"

namespace OHOS {
namespace Telephony {
struct Tmp1 {
    int serial;
    int32_t status;
};

class RilRadioResponseTest : public OHOS::IPCObjectStub {
public:
    RilRadioResponseTest(RilManagerTest *mRilManager);
    ~RilRadioResponseTest();

    int OnRemoteRequest(uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply,
        OHOS::MessageOption &option) override;
    void OnResponseGetSimState(OHOS::MessageParcel &data);
    void OnResponseGetCurrentCalls(OHOS::MessageParcel &data);
    void OnResponseGetRilCurrentCellInfo(OHOS::MessageParcel &data);
    void DialResponse(OHOS::MessageParcel &data);
    void GetImsiResponse(OHOS::MessageParcel &data);
    void OnResponseHangupCall(OHOS::MessageParcel &data);
    void OnResponseRejectCall(OHOS::MessageParcel &data);
    void OnResponseHoldCall(OHOS::MessageParcel &data);
    void OnResponseActiveCall(OHOS::MessageParcel &data);
    void OnResponseSwapCall(OHOS::MessageParcel &data);
    void OnResponseJoinCall(OHOS::MessageParcel &data);
    void OnResponseSplitCall(OHOS::MessageParcel &data);
    void OnResponseGetLastCallErrorCode(OHOS::MessageParcel &data);
    void OnResponseGetSignalQuality(OHOS::MessageParcel &data);
    void OnResponseGetCsRegStatus(OHOS::MessageParcel &data);
    void OnResponseGetNetworkRegistrationState(OHOS::MessageParcel &data);
    void OnResponseGetCurrentOperator(OHOS::MessageParcel &data);
    void SetRadioStatusResponse(OHOS::MessageParcel &data);
    void SendSmsResponse(OHOS::MessageParcel &data);
    void SendSmsMoreModeResponse(OHOS::MessageParcel &data);
    void ActivatePdpContextResponse(OHOS::MessageParcel &data);
    void OnResponseGetIccIOState(OHOS::MessageParcel &data);
    void OnResponseAcceptRilCmCall(OHOS::MessageParcel &data);
    void OnResponseDeactivateDataCall(OHOS::MessageParcel &data);
    void OnResponseSendNewMessageAckCdma(OHOS::MessageParcel &data);
    void OnResponseSetInitialApn(OHOS::MessageParcel &data);
    void OnResponseGetImsRegistrationState(OHOS::MessageParcel &data);
    void OnResponseSendImsSms(OHOS::MessageParcel &data);
    void OnResponseSetDataCallAllowed(OHOS::MessageParcel &data);
    void OnResponseSetModemDataProfile(OHOS::MessageParcel &data);
    void OnResponsePullRadioLceData(OHOS::MessageParcel &data);
    void OnRequestGetNetworkSelectionModeTest(OHOS::MessageParcel &data);
    void OnRequestGetNetworkSearchInformationTest(OHOS::MessageParcel &data);
    void OnRequestSetNetworkSelectionModeTest(OHOS::MessageParcel &data);

private:
    RilManagerTest *mRilManager_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_RADIO_RESPONSE_test_H