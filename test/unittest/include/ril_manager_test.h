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
    typedef struct UusInfo {
        int32_t uusDcs; /* 3GPP TS 23.038 [25] Cell Broadcast Data Coding Scheme(default 0) */
    } UusInfo;

public:
    RilManagerTest(int32_t preferredNetworkType, int32_t cdmaSubscription);
    ~RilManagerTest();

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
    void RilCmDial(std::string address, int32_t clirMode, const AppExecFwk::InnerEvent::Pointer &result);
    void Hangup(int32_t gsmIndex, const AppExecFwk::InnerEvent::Pointer &result);
    void Answer(const AppExecFwk::InnerEvent::Pointer &result);
    void Reject(const AppExecFwk::InnerEvent::Pointer &result);
    void HoldCall(const AppExecFwk::InnerEvent::Pointer &result);
    void UnHoldCall(const AppExecFwk::InnerEvent::Pointer &result);
    void SwitchCall(const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmJoin(int32_t callType, const AppExecFwk::InnerEvent::Pointer &result);
    void RilCmSplit(int32_t callIndex, int32_t callType, const AppExecFwk::InnerEvent::Pointer &result);
    void CallSupplement(int32_t hangupType, const AppExecFwk::InnerEvent::Pointer &result);
    void SendDtmf(char dtmfKey, int32_t index, const AppExecFwk::InnerEvent::Pointer &result);
    void StartDtmf(char dtmfKey, int32_t index, const AppExecFwk::InnerEvent::Pointer &result);
    void StopDtmf(int32_t index, const AppExecFwk::InnerEvent::Pointer &result);
    void SetUssd(std::string str, const AppExecFwk::InnerEvent::Pointer &result);
    void GetUssd(const AppExecFwk::InnerEvent::Pointer &result);
    void SetMute(int32_t mute, const AppExecFwk::InnerEvent::Pointer &result);
    void GetMute(const AppExecFwk::InnerEvent::Pointer &result);
    void GetEmergencyList(const AppExecFwk::InnerEvent::Pointer &result);
    int32_t SetEmergencyCallList(const AppExecFwk::InnerEvent::Pointer &result);
    void GetFailReason(const AppExecFwk::InnerEvent::Pointer &result);
    void SetBarringPassword(const std::string &fac, const std::string &oldPwd, const std::string &newPwd,
        const AppExecFwk::InnerEvent::Pointer &result);

    void GetImsi(std::string aid, const AppExecFwk::InnerEvent::Pointer &response);

    void GetRilCmSignalStrength(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int32_t what);

    void GetCsRegStatus(const AppExecFwk::InnerEvent::Pointer &response);
    void GetPsRegStatus(const AppExecFwk::InnerEvent::Pointer &response);
    void GetCellInfoList(const AppExecFwk::InnerEvent::Pointer &response);
    void GetCurrentCellInfo(const AppExecFwk::InnerEvent::Pointer &response);
    void GetOperatorInfo(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int32_t what);
    void GetNetworkSearchInfo(const AppExecFwk::InnerEvent::Pointer &response);
    void GetNetworkSelectionMode(const AppExecFwk::InnerEvent::Pointer &response);
    void GetPreferredNetwork(const AppExecFwk::InnerEvent::Pointer &response);
    void SetPreferredNetwrok(int32_t preferredNetwork, const AppExecFwk::InnerEvent::Pointer &result);
    void SetLocationUpdate(int32_t enableFlg, const AppExecFwk::InnerEvent::Pointer &result);
    void SetNotificationFilter(int32_t filter, const AppExecFwk::InnerEvent::Pointer &result);
    void SetDeviceState(int32_t deviceStateType, bool deviceStateOn, const AppExecFwk::InnerEvent::Pointer &result);
    void SetNetworkSelectionMode(int32_t mode, std::string plmn, const AppExecFwk::InnerEvent::Pointer &result);
    void ShutDown(const AppExecFwk::InnerEvent::Pointer &result);
    void GetMeid(const AppExecFwk::InnerEvent::Pointer &response);
    void GetVoiceRadio(const AppExecFwk::InnerEvent::Pointer &response);
    void GetBasebandVersion(const AppExecFwk::InnerEvent::Pointer &response);
    void SendSms(std::string smscPdu, std::string pdu, const std::shared_ptr<AppExecFwk::EventHandler> &handler,
        const AppExecFwk::InnerEvent::Pointer &response);
    void SendSmsMoreMode(std::string smscPdu, std::string pdu, const std::shared_ptr<AppExecFwk::EventHandler> &handler,
        const AppExecFwk::InnerEvent::Pointer &response);
    void SendSmsAck(bool success, int32_t cause, const AppExecFwk::InnerEvent::Pointer &response);
    void GetSmscAddr(const AppExecFwk::InnerEvent::Pointer &response);
    void SetSmscAddr(int32_t tosca, std::string address, const AppExecFwk::InnerEvent::Pointer &response);
    void SetRadioState(int32_t fan, int32_t rst, const AppExecFwk::InnerEvent::Pointer &response);
    void GetRadioState(const AppExecFwk::InnerEvent::Pointer &response);
    void GetImei(const AppExecFwk::InnerEvent::Pointer &response);
    void GetSimIO(int32_t command, int32_t fileId, int32_t p1, int32_t p2, int32_t p3, std::string data,
        std::string path, const AppExecFwk::InnerEvent::Pointer &response);
    void ActivatePdpContext(int32_t radioTechnology, RilDataProfileTest dataProfile, bool isRoaming, bool allowRoaming,
        const AppExecFwk::InnerEvent::Pointer &response);
    void DeactivatePdpContext(int32_t ci, int32_t reason, const AppExecFwk::InnerEvent::Pointer &response);
    void SetInitApnInfo(RilDataProfileTest dataProfile, const AppExecFwk::InnerEvent::Pointer &response);
    void GetPdpContext(const AppExecFwk::InnerEvent::Pointer &response);
    void GetSimStatus(const AppExecFwk::InnerEvent::Pointer &result);
    void SimOpenLogicalChannel(std::string appID, int32_t p2, const AppExecFwk::InnerEvent::Pointer &response);
    void SimCloseLogicalChannel(int32_t channelId, const AppExecFwk::InnerEvent::Pointer &response);
    void SimTransmitApduLogicalChannel(ApduSimIORequestInfo reqInfo, const AppExecFwk::InnerEvent::Pointer &response);
    void SimTransmitApduBasicChannel(ApduSimIORequestInfo reqInfo, const AppExecFwk::InnerEvent::Pointer &response);
    void SimAuthentication(SimAuthenticationRequestInfo reqInfo, const AppExecFwk::InnerEvent::Pointer &response);
    void SetActiveSim(int32_t slotId, int32_t enable, const AppExecFwk::InnerEvent::Pointer &response);
    void UnlockSimLock(int32_t lockType, std::string password, const AppExecFwk::InnerEvent::Pointer &response);
    void GetLinkBandwidthInfo(const int32_t cid, const AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPin(const std::string &pin, const AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPin2(const std::string &pin2, const AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPuk(const std::string &puk, const std::string &pin, const AppExecFwk::InnerEvent::Pointer &result);
    void UnLockPuk2(const std::string &puk2, const std::string &pin2, const AppExecFwk::InnerEvent::Pointer &result);
    void ChangeSimPassword(const std::string &fac, const std::string &oldPassword, const std::string &newPassword,
        int32_t passwordLength, const AppExecFwk::InnerEvent::Pointer &result);
    void SetSimLock(const std::string &fac, int32_t mode, const std::string &password,
        const AppExecFwk::InnerEvent::Pointer &result);
    void GetSimLockStatus(const std::string &fac, int32_t mode, const AppExecFwk::InnerEvent::Pointer &result);

    static const int32_t INVALID_WAKELOCK = -1;
    static const int32_t FOR_WAKELOCK = 0;
    static const int32_t FOR_ACK_WAKELOCK = 1;
    static const int32_t HRIL_ADAPTER_RADIO_INDICATION = 2001;
    static const int32_t HRIL_ADAPTER_RADIO_RESPONSE = 2002;
    static const int32_t RIL_ADAPTER_OEM_INDICATION = 2003;
    static const int32_t RIL_ADAPTER_OEM_RESPONSE = 2004;
    sptr<IRemoteObject> cellularRadio_;
    int32_t cdmaSubscription_ = 0;

    static int32_t GetNextSerialId()
    {
        return nextSerialId_++;
    }

    static std::atomic_int nextSerialId_;
    static std::unordered_map<int32_t, std::shared_ptr<HRilRequestTest>> requestMap_;

private:
    std::shared_ptr<HRilRequestTest> CreateRequest(int32_t request, const AppExecFwk::InnerEvent::Pointer &result);

    static constexpr const char *RIL_ADAPTER_SERVICE_NAME = "cellular_radio1";
    static constexpr int32_t PAYLOAD_NUM = 1234;
    int32_t slotId_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_MANAGER_TEST_H
