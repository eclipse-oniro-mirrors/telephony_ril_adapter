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
public:
    RilManagerTest(int32_t preferredNetworkType, int32_t cdmaSubscription);
    ~RilManagerTest();

    void OnInit();
    int32_t SendInt32Event(int32_t dispatchId, int32_t value);
    int32_t SendInt32sEvent(int32_t dispatchId, int32_t argCount, ...);
    int32_t SendStringEvent(int32_t dispatchId, const char *value);
    int32_t SendBufferEvent(int32_t dispatchId, OHOS::MessageParcel &eventData);
    int32_t SendCommonBufferEvent(int32_t dispatchId, const void *eventData, size_t dataLength);
    int32_t SetCellularRadioIndication();
    int32_t SetCellularRadioResponse();

    int32_t RilSendAck();
    void RilProcessResponseDone(std::shared_ptr<HRilRequestTest> request, HRilRadioResponseInfo &responseInfo);
    DataProfileDataInfo ConvertToHalDataProfile(RilDataProfileTest dp);
    GsmSmsMessageInfo ConstructGsmSendSmsRilRequest(std::string smscPdu, std::string pdu);
    int32_t GetCallList(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t RilCmDial(std::string address, int32_t clirMode, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t Reject(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t HoldCall(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t UnHoldCall(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SwitchCall(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetImsi(std::string aid, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t Hangup(int32_t gsmIndex, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t RilCmJoin(int32_t callType, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t RilCmSplit(int32_t callIndex, int32_t callType, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t Answer(const AppExecFwk::InnerEvent::Pointer &result);
    void RejectRilCmCall(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetRilCmSignalStrength(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int32_t what);
    int32_t GetCsRegStatus(const AppExecFwk::InnerEvent::Pointer &response);
    int32_t GetPsRegStatus(const AppExecFwk::InnerEvent::Pointer &response);
    int32_t GetOperatorInfo(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int32_t what);
    int32_t SetNotificationFilter(int32_t filter, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetNetworkSearchInfo(const AppExecFwk::InnerEvent::Pointer &response);
    int32_t GetNetworkSelectionMode(const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SetNetworkSelectionMode(int32_t mode, std::string plmn, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetPhysicalChannelConfig(const AppExecFwk::InnerEvent::Pointer &response);
    int32_t GetRadioCapability(const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SetDeviceState(int32_t deviceStateType, bool deviceStateOn, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SendSms(std::string smscPdu, std::string pdu, const std::shared_ptr<AppExecFwk::EventHandler> &handler,
        const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SendSmsMoreMode(std::string smscPdu, std::string pdu,
        const std::shared_ptr<AppExecFwk::EventHandler> &handler, const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SetRadioState(int32_t fan, int32_t rst, const AppExecFwk::InnerEvent::Pointer &response);
    int32_t ShutDown(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetRadioState(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetImei(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetMeid(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetVoiceRadioTechnology(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SendSmsAck(bool success, int32_t cause, const AppExecFwk::InnerEvent::Pointer &response);
    int32_t GetSimIO(SimIoRequestInfo data, const AppExecFwk::InnerEvent::Pointer &response);
    int32_t ActivatePdpContext(int32_t radioTechnology, RilDataProfileTest dataProfile, bool isRoaming,
        bool allowRoaming, const AppExecFwk::InnerEvent::Pointer &response);
    int32_t DeactivatePdpContext(int32_t ci, int32_t reason, const AppExecFwk::InnerEvent::Pointer &response);
    int32_t GetSimStatus(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetUssd(std::string str, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetUssd(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetClip(int32_t action, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetClip(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetCallWaiting(int32_t activate, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetCallWaiting(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetCallRestriction(std::string fac, int32_t mode, std::string password,
        const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetCallRestriction(std::string fac, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetCallTransferInfo(int32_t reason, int32_t mode, std::string number, int32_t classx,
        const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetCallTransferInfo(int32_t reason, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetClir(int32_t action, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetClir(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetLinkBandwidthInfo(const int32_t cid, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetDataProfileInfo(const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SetDataPermitted(const int32_t enabled, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SendDataPerformanceMode(const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SendDataSleepMode(const AppExecFwk::InnerEvent::Pointer &response);
    int32_t UnLockPin(const std::string &pin, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t UnLockPin2(const std::string &pin2, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t UnLockPuk(const std::string &puk, const std::string &pin, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t UnLockPuk2(const std::string &puk2, const std::string &pin2, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t ChangeSimPassword(const std::string &fac, const std::string &oldPassword, const std::string &newPassword,
        int32_t passwordLength, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetSimLock(const std::string &fac, int32_t mode, const std::string &password,
        const AppExecFwk::InnerEvent::Pointer &result);
    int32_t GetSimLockStatus(const std::string &fac, int32_t mode, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SendTerminalResponseCmd(const std::string &cmd, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SendEnvelopeCmd(const std::string &cmd, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SendCallSetupRequestResult(bool accept, const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SimStkIsReady(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetEmergencyCallList(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetBarringPassword(const std::string &fac, const std::string &oldPwd, const std::string &newPwd,
        const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SimOpenLogicalChannel(std::string appID, int32_t p2, const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SimCloseLogicalChannel(int32_t channelId, const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SimTransmitApduLogicalChannel(ApduSimIORequestInfo reqInfo,
        const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SimTransmitApduBasicChannel(ApduSimIORequestInfo reqInfo,
        const AppExecFwk::InnerEvent::Pointer &response);
    int32_t SetActiveSim(int32_t index, int32_t enable, const AppExecFwk::InnerEvent::Pointer &response);
    static const int32_t INVALID_WAKELOCK = -1;
    static const int32_t FOR_WAKELOCK = 0;
    static const int32_t FOR_ACK_WAKELOCK = 1;
    static const int32_t HRIL_ADAPTER_RADIO_INDICATION = 2001;
    static const int32_t HRIL_ADAPTER_RADIO_RESPONSE = 2002;
    static const int32_t RIL_ADAPTER_OEM_INDICATION = 2003;
    static const int32_t RIL_ADAPTER_OEM_RESPONSE = 2004;
    static const int32_t TELEPHONY_ERR_LOCAL_PTR_NULL = -1;
    static const int32_t TELEPHONY_ERR_SUCCESS = 0;
    sptr<IRemoteObject> cellularRadio_;
    int32_t cdmaSubscription_ = 0;

    static int32_t GetNextSerialId()
    {
        return nextSerialId_++;
    }

    static std::atomic_int nextSerialId_;
    static std::unordered_map<int32_t, std::shared_ptr<HRilRequestTest>> requestMap_;
    static int32_t slotId;

private:
    std::shared_ptr<HRilRequestTest> CreateRequest(int32_t request, const AppExecFwk::InnerEvent::Pointer &result);

    static constexpr const char *RIL_ADAPTER_SERVICE_NAME = "cellular_radio1";
    static constexpr int32_t PAYLOAD_NUM = 1234;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_MANAGER_TEST_H
