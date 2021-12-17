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
class RilRadioIndicationTest : public IPCObjectStub {
public:
    RilRadioIndicationTest(RilManagerTest *mRilManager);
    ~RilRadioIndicationTest();

    int OnRemoteRequest(
        uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) override;

private:
    void RadioStateUpdated(MessageParcel &data);
    void CallStateUpdated(MessageParcel &data);
    void NetworkStateNotify(MessageParcel &data);
    void NewSmsNotify(MessageParcel &data);
    void NewCdmaSmsNotify(MessageParcel &data);
    void SmsStatusReportNotify(MessageParcel &data);
    void NewSmsStoredOnSimNotify(MessageParcel &data);
    void SignalStrengthUpdated(MessageParcel &data);

private:
    RilManagerTest *mRilManager_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_RADIO_INDICATION_TEST_H