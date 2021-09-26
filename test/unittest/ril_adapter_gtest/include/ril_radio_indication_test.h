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

#ifndef OHOS_RIL_RADIO_INDICATION_TEST_H
#define OHOS_RIL_RADIO_INDICATION_TEST_H

#include <ipc_object_stub.h>
#include <securec.h>

namespace OHOS {
namespace Telephony {
class RilManagerTest;

class RilRadioIndicationTest : public OHOS::IPCObjectStub {
public:
    RilRadioIndicationTest(RilManagerTest *mRilManager);
    ~RilRadioIndicationTest();

    int OnRemoteRequest(uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply,
        OHOS::MessageOption &option) override;
    void RadioStateChange(OHOS::MessageParcel &data);
    void CallStateChgInd(OHOS::MessageParcel &data);
    void NetworkStateNotify(OHOS::MessageParcel &data);
    void NewSmsNotify(OHOS::MessageParcel &data);
    void SmsStatusReportNotify(OHOS::MessageParcel &data);
    void NewSmsStoredOnSimNotify(OHOS::MessageParcel &data);
    void GetSignalStrength(OHOS::MessageParcel &data);
    void ChangedDataCallList(OHOS::MessageParcel &data);
    void UpdatedSimFile(OHOS::MessageParcel &data);
    void ChangedSimState(OHOS::MessageParcel &data);
    void ConnectedReturnRilVersion(OHOS::MessageParcel &data);
    void ChangedImsNetworkState(OHOS::MessageParcel &data);

private:
    RilManagerTest *mRilManager_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_RADIO_INDICATION_TEST_H