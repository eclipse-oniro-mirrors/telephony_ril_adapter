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

#include "ril_manager_test.h"

namespace OHOS {
namespace Telephony {
class RilRadioIndicationTest : public OHOS::IPCObjectStub {
public:
    RilRadioIndicationTest(RilManagerTest *mRilManager);
    ~RilRadioIndicationTest();

    int32_t OnRemoteRequest(
        uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply, OHOS::MessageOption &option) override;
    void RadioStateChange(OHOS::MessageParcel &data);
    void CallStateUpdated(OHOS::MessageParcel &data);
    void CallCRingReport(OHOS::MessageParcel &data);
    void CallRingbackVoiceReport(OHOS::MessageParcel &data);
    void CallSrvccStatusReport(OHOS::MessageParcel &data);
    void CallEmergencyNumberReport(OHOS::MessageParcel &data);
    void NetworkStateNotify(OHOS::MessageParcel &data);
    void NetworkPhyChnlCfgUpdated(OHOS::MessageParcel &data);
    void NetworkCurrentCellUpdated(OHOS::MessageParcel &data);
    void VoiceRadioTechUpdated(OHOS::MessageParcel &data);
    void NewSmsNotify(OHOS::MessageParcel &data);
    void NewCdmaSmsNotify(OHOS::MessageParcel &data);
    void SmsStatusReportNotify(OHOS::MessageParcel &data);
    void NewSmsStoredOnSimNotify(OHOS::MessageParcel &data);
    void GetSignalStrength(OHOS::MessageParcel &data);
    void PdpContextListChangedNotify(OHOS::MessageParcel &data);
    void UpdatedSimFile(OHOS::MessageParcel &data);
    void SimStateChanged(OHOS::MessageParcel &data);
    void ConnectedReturnRilVersion(OHOS::MessageParcel &data);
    void ChangedImsNetworkState(OHOS::MessageParcel &data);
    void CallSsReport(OHOS::MessageParcel &data);
    void SimRefreshNotify(OHOS::MessageParcel &data);

private:
    RilManagerTest *mRilManager_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_RADIO_INDICATION_TEST_H
