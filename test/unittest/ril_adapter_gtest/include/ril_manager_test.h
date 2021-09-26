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

#ifndef OHOS_RIL_MANAGER_TEST_H
#define OHOS_RIL_MANAGER_TEST_H

#include <iservmgr_hdi.h>

#include "event_runner.h"

#include "hril_call_parcel.h"
#include "hril_data_parcel.h"
#include "hril_modem_parcel.h"
#include "hril_network_parcel.h"
#include "hril_sim_parcel.h"
#include "hril_sms_parcel.h"
#include "ril_data_profile_test.h"
#include "ril_radio_indication_test.h"

#define SAMPLE_WRITE_READ 123
namespace OHOS {
namespace Telephony {
enum CellularRadioState {
    RADIO_OFF, /* Radio explicitly powered off (eg CFUN=0) */
    RADIO_UNAVAILABLE, /* Radio unavailable (eg, resetting or not booted) */
    RADIO_ON /* Radio is on */
};

struct HRilRequestTest {
    int32_t serialId_;
    int32_t requestId_;
    AppExecFwk::InnerEvent::Pointer pointer_ = AppExecFwk::InnerEvent::Pointer(nullptr, nullptr);

    HRilRequestTest(int32_t serialId, int32_t requestId, const AppExecFwk::InnerEvent::Pointer &pointer)
    {
        serialId_ = serialId;
        requestId_ = requestId;
        pointer_ = std::move(const_cast<AppExecFwk::InnerEvent::Pointer &>(pointer));
    }
};

class RilManagerTest {
    typedef struct UusInfo {
        int uusDcs; /* 3GPP TS 23.038 [25] Cell Broadcast Data Coding Scheme(default 0) */
    } UusInfo;

public:
    RilManagerTest(int preferredNetworkType, int cdmaSubscription);
    ~RilManagerTest();

    static const bool RILMANAGER_LOGD = true;
    static const bool RILMANAGER_LOGV = false;

    void OnInit();
    int32_t SendInt32Event(int32_t dispatchId, int32_t value);
    int32_t SendStringEvent(int32_t dispatchId, const char *value);
    int32_t SendBufferEvent(int32_t dispatchId, OHOS::MessageParcel &eventData);
    int32_t SendCommonBufferEvent(int32_t dispatchId, const void *eventData, size_t dataLength);
    void SetCellularRadioIndication();
    void SetCellularRadioResponse();

    void RilSendAck();
    void RilProcessResponseDone(std::shared_ptr<HRilRequestTest> request, HRilRadioResponseInfo &responseInfo);
    DataProfileDataInfo ConvertToHalDataProfile(RilDataProfileTest dp);
    GsmSmsMessageInfo ConstructGsmSendSmsRilRequest(std::string smscPdu, std::string pdu);
    void GetCallList(const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmDial(std::string address, int clirMode, const AppExecFwk::InnerEvent::Pointer &result);

    void Reject(const AppExecFwk::InnerEvent::Pointer &result);

    void Hold(const AppExecFwk::InnerEvent::Pointer &result);

    void Active(const AppExecFwk::InnerEvent::Pointer &result);

    void Swap(const AppExecFwk::InnerEvent::Pointer &result);

    void GetImsi(std::string aid, const AppExecFwk::InnerEvent::Pointer &result);
    void Hangup(int32_t gsmIndex, const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmJoin(int32_t callType, const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmSplit(int32_t nThCall, int32_t callType, const AppExecFwk::InnerEvent::Pointer &result);

    void Answer(const AppExecFwk::InnerEvent::Pointer &result);
    void RejectRilCmCall(const AppExecFwk::InnerEvent::Pointer &result);
    void GetRilCmSignalStrength(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int what);

    void GetCsRegStatus(const AppExecFwk::InnerEvent::Pointer &response);
    void GetPsRegStatus(const AppExecFwk::InnerEvent::Pointer &response);
    void SetNetworkLocationUpdate(const AppExecFwk::InnerEvent::Pointer &response);
    void GetOperatorInfo(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int what);
    void SendSms(std::string smscPdu, std::string pdu, const std::shared_ptr<AppExecFwk::EventHandler> &handler,
        const AppExecFwk::InnerEvent::Pointer &response);
    void SendSmsMoreMode(std::string smscPdu, std::string pdu,
        const std::shared_ptr<AppExecFwk::EventHandler> &handler, const AppExecFwk::InnerEvent::Pointer &response);
    void SetRadioStatus(int fan, int rst, const AppExecFwk::InnerEvent::Pointer &response);
    void SendSmsAck(bool success, int32_t cause, const AppExecFwk::InnerEvent::Pointer &response);

    void RequestSimIO(int32_t command, int32_t fileId, int32_t p1, int32_t p2, int32_t p3, std::string data,
        std::string path, const AppExecFwk::InnerEvent::Pointer &response);
    void ActivatePdpContext(int32_t radioTechnology, RilDataProfileTest dataProfile, bool isRoaming,
        bool allowRoaming, const AppExecFwk::InnerEvent::Pointer &response);
    void DeactivatePdpContext(int32_t ci, int32_t reason, const AppExecFwk::InnerEvent::Pointer &response);
    void GetSimStatus(const AppExecFwk::InnerEvent::Pointer &result);

    static const int INVALID_WAKELOCK = -1;
    static const int FOR_WAKELOCK = 0;
    static const int FOR_ACK_WAKELOCK = 1;
    const int HRIL_ADAPTER_RADIO_INDICATION = 2001;
    const int HRIL_ADAPTER_RADIO_RESPONSE = 2002;
    const int RIL_ADAPTER_OEM_INDICATION = 2003;
    const int RIL_ADAPTER_OEM_RESPONSE = 2004;
    sptr<IRemoteObject> cellularRadio_;
    int32_t cdmaSubscription_ = 0;

    static int32_t GetNextSerialId()
    {
        return nextSerialId_++;
    }

    static std::atomic_int nextSerialId_;
    static std::unordered_map<int32_t, std::shared_ptr<HRilRequestTest>> requestMap_;

private:
    std::shared_ptr<HRilRequestTest> CreateRequest(int request, const AppExecFwk::InnerEvent::Pointer &result);

    static constexpr const char *RIL_ADAPTER_SERVICE_NAME = "cellular_radio1";
    static constexpr int PAYLOAD_NUM = 1234;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_MANAGER_TEST_H
