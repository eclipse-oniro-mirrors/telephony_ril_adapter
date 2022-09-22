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

#ifndef OHOS_HRIL_MANAGER_H
#define OHOS_HRIL_MANAGER_H

#include <list>
#include <mutex>
#include <unordered_map>

#include "hril_call.h"
#include "hril_data.h"
#include "hril_modem.h"
#include "hril_network.h"
#include "hril_sim.h"
#include "hril_sms.h"
#include "hril_timer_callback.h"
#include "v1_0/ipower_interface.h"

namespace OHOS {
namespace Telephony {
typedef enum : int32_t {
    RIL_REGISTER_IS_NONE = 0,
    RIL_REGISTER_IS_RUNNING,
} RegisterState;

class HRilManager : public IHRilReporter {
public:
    HRilManager();
    virtual ~HRilManager();

    int32_t GetMaxSimSlotCount();

    virtual ReqDataInfo *CreateHRilRequest(int32_t serial, int32_t slotId, int32_t request) override;
    virtual void ReleaseHRilRequest(int32_t request, ReqDataInfo *requestInfo) override;

    void RegisterCallFuncs(int32_t slotId, const HRilCallReq *callFuncs);
    void RegisterDataFuncs(int32_t slotId, const HRilDataReq *dataFuncs);
    void RegisterModemFuncs(int32_t slotId, const HRilModemReq *modemFuncs);
    void RegisterNetworkFuncs(int32_t slotId, const HRilNetworkReq *networkFuncs);
    void RegisterSimFuncs(int32_t slotId, const HRilSimReq *simFuncs);
    void RegisterSmsFuncs(int32_t slotId, const HRilSmsReq *smsFuncs);
    void ApplyRunningLock(void);
    void ReleaseRunningLock(void);

    void OnCallReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnDataReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnModemReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnNetworkReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnSimReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnSmsReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void SetRilCallback(const sptr<OHOS::HDI::Ril::V1_0::IRilCallback> callback);

    // Call
    int32_t SetEmergencyCallList(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::EmergencyInfoList &emergencyInfoList);
    int32_t GetEmergencyCallList(int32_t slotId, int32_t serialId);
    int32_t GetCallList(int32_t slotId, int32_t serialId);
    int32_t Dial(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DialInfo &dialInfo);
    int32_t Reject(int32_t slotId, int32_t serialId);
    int32_t Hangup(int32_t slotId, int32_t serialId, int32_t gsmIndex);
    int32_t Answer(int32_t slotId, int32_t serialId);
    int32_t HoldCall(int32_t slotId, int32_t serialId);
    int32_t UnHoldCall(int32_t slotId, int32_t serialId);
    int32_t SwitchCall(int32_t slotId, int32_t serialId);
    int32_t CombineConference(int32_t slotId, int32_t serialId, int32_t callType);
    int32_t SeparateConference(int32_t slotId, int32_t serialId, int32_t callIndex, int32_t callType);
    int32_t GetCallWaiting(int32_t slotId, int32_t serialId);
    int32_t SetCallWaiting(int32_t slotId, int32_t serialId, int32_t activate);
    int32_t GetCallTransferInfo(int32_t slotId, int32_t serialId, int32_t reason);
    int32_t SetCallTransferInfo(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::CallForwardSetInfo &callForwardSetInfo);
    int32_t GetCallRestriction(int32_t slotId, int32_t serialId, const std::string &fac);
    int32_t SetCallRestriction(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::CallRestrictionInfo &callRestrictionInfo);
    int32_t GetClip(int32_t slotId, int32_t serialId);
    int32_t SetClip(int32_t slotId, int32_t serialId, int32_t action);
    int32_t GetClir(int32_t slotId, int32_t serialId);
    int32_t SetClir(int32_t slotId, int32_t serialId, int32_t action);
    int32_t GetCallPreferenceMode(int32_t slotId, int32_t serialId);
    int32_t SetCallPreferenceMode(int32_t slotId, int32_t serialId, int32_t mode);
    int32_t SetUssd(int32_t slotId, int32_t serialId, const std::string &str);
    int32_t GetUssd(int32_t slotId, int32_t serialId);
    int32_t SetMute(int32_t slotId, int32_t serialId, int32_t mute);
    int32_t GetMute(int32_t slotId, int32_t serialId);
    int32_t GetCallFailReason(int32_t slotId, int32_t serialId);
    int32_t CallSupplement(int32_t slotId, int32_t serialId, int32_t type);
    int32_t SendDtmf(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo);
    int32_t StartDtmf(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo);
    int32_t StopDtmf(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo);
    int32_t SetBarringPassword(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SetBarringInfo &setBarringInfo);

    // Data
    int32_t ActivatePdpContext(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataCallInfo &dataCallInfo);
    int32_t DeactivatePdpContext(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::UniInfo &uniInfo);
    int32_t GetPdpContextList(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::UniInfo &uniInfo);
    int32_t SetInitApnInfo(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataProfileDataInfo &dataProfileDataInfo);
    int32_t GetLinkBandwidthInfo(int32_t slotId, int32_t serialId, int32_t cid);
    int32_t SetLinkBandwidthReportingRule(int32_t slotId, int32_t serialId,
        const OHOS::HDI::Ril::V1_0::DataLinkBandwidthReportingRule &dataLinkBandwidthReportingRule);
    int32_t SetDataPermitted(int32_t slotId, int32_t serialId, int32_t dataPermitted);
    int32_t SetDataProfileInfo(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataProfilesInfo &dataProfilesInfo);
    int32_t SendDataPerformanceMode(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataPerformanceInfo &dataPerformanceInfo);
    int32_t SendDataSleepMode(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataSleepInfo &dataSleepInfo);

    int32_t SetRadioState(int32_t slotId, int32_t serialId, int32_t fun, int32_t rst);
    int32_t GetRadioState(int32_t slotId, int32_t serialId);
    int32_t GetImei(int32_t slotId, int32_t serialId);
    int32_t GetMeid(int32_t slotId, int32_t serialId);
    int32_t GetVoiceRadioTechnology(int32_t slotId, int32_t serialId);
    int32_t GetBasebandVersion(int32_t slotId, int32_t serialId);
    int32_t ShutDown(int32_t slotId, int32_t serialId);

    int32_t GetSimIO(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimIoRequestInfo &simIO);
    int32_t GetSimStatus(int32_t slotId, int32_t serialId);
    int32_t GetImsi(int32_t slotId, int32_t serialId);
    int32_t GetSimLockStatus(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo);
    int32_t SetSimLock(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo);
    int32_t ChangeSimPassword(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimPasswordInfo &simPassword);
    int32_t UnlockPin(int32_t slotId, int32_t serialId, const std::string &pin);
    int32_t UnlockPuk(int32_t slotId, int32_t serialId, const std::string &puk, const std::string &pin);
    int32_t UnlockPin2(int32_t slotId, int32_t serialId, const std::string &pin2);
    int32_t UnlockPuk2(int32_t slotId, int32_t serialId, const std::string &puk2, const std::string &pin2);
    int32_t SetActiveSim(int32_t slotId, int32_t serialId, int32_t index, int32_t enable);
    int32_t SimStkSendTerminalResponse(int32_t slotId, int32_t serialId, const std::string &strCmd);
    int32_t SimStkSendEnvelope(int32_t slotId, int32_t serialId, const std::string &strCmd);
    int32_t SimStkSendCallSetupRequestResult(int32_t slotId, int32_t serialId, int32_t accept);
    int32_t SimStkIsReady(int32_t slotId, int32_t serialId);
    int32_t GetRadioProtocol(int32_t slotId, int32_t serialId);
    int32_t SetRadioProtocol(int32_t slotId, int32_t serialId, const HDI::Ril::V1_0::RadioProtocol &radioProtocol);
    int32_t SimOpenLogicalChannel(int32_t slotId, int32_t serialId, const std::string &appID, int32_t p2);
    int32_t SimCloseLogicalChannel(int32_t slotId, int32_t serialId, int32_t channelId);
    int32_t SimTransmitApduLogicalChannel(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO);
    int32_t SimTransmitApduBasicChannel(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO);
    int32_t SimAuthentication(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimAuthenticationRequestInfo &simAuthInfo);
    int32_t UnlockSimLock(int32_t slotId, int32_t serialId, int32_t lockType, const std::string &key);

    // Network
    int32_t GetSignalStrength(int32_t slotId, int32_t serialId);
    int32_t GetCsRegStatus(int32_t slotId, int32_t serialId);
    int32_t GetPsRegStatus(int32_t slotId, int32_t serialId);
    int32_t GetOperatorInfo(int32_t slotId, int32_t serialId);
    int32_t GetNetworkSearchInformation(int32_t slotId, int32_t serialId);
    int32_t GetNetworkSelectionMode(int32_t slotId, int32_t serialId);
    int32_t SetNetworkSelectionMode(
        int32_t slotId, int32_t serialId, const HDI::Ril::V1_0::SetNetworkModeInfo &networkModeInfo);
    int32_t GetNeighboringCellInfoList(int32_t slotId, int32_t serialId);
    int32_t GetCurrentCellInfo(int32_t slotId, int32_t serialId);
    int32_t SetPreferredNetwork(int32_t slotId, int32_t serialId, int32_t preferredNetworkType);
    int32_t GetPreferredNetwork(int32_t slotId, int32_t serialId);
    int32_t GetPhysicalChannelConfig(int32_t slotId, int32_t serialId);
    int32_t SetLocateUpdates(int32_t slotId, int32_t serialId, const HDI::Ril::V1_0::RilRegNotifyMode mode);
    int32_t SetNotificationFilter(int32_t slotId, int32_t serialId, int32_t newFilter);
    int32_t SetDeviceState(int32_t slotId, int32_t serialId, int32_t deviceStateType, int32_t deviceStateOn);

    // Sms
    int32_t SendGsmSms(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::GsmSmsMessageInfo &gsmSmsMessageInfo);
    int32_t SendCdmaSms(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SendCdmaSmsMessageInfo &cdmaSmsMessageInfo);
    int32_t AddSimMessage(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo);
    int32_t DelSimMessage(int32_t slotId, int32_t serialId, int32_t index);
    int32_t UpdateSimMessage(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo);
    int32_t AddCdmaSimMessage(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo);
    int32_t DelCdmaSimMessage(int32_t slotId, int32_t serialId, int32_t index);
    int32_t UpdateCdmaSimMessage(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo);
    int32_t SetSmscAddr(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::ServiceCenterAddress &serviceCenterAddress);
    int32_t GetSmscAddr(int32_t slotId, int32_t serialId);
    int32_t SetCBConfig(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::CBConfigInfo &cellBroadcastInfo);
    int32_t GetCBConfig(int32_t slotId, int32_t serialId);
    int32_t SetCdmaCBConfig(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::CdmaCBConfigInfoList &cdmaCBConfigInfoList);
    int32_t GetCdmaCBConfig(int32_t slotId, int32_t serialId);
    int32_t SendSmsMoreMode(
        int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::GsmSmsMessageInfo &gsmSmsMessageInfo);
    int32_t SendSmsAck(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::ModeData &modeData);

    int32_t SendRilAck();

public:
    sptr<OHOS::HDI::Power::V1_0::IPowerInterface> powerInterface_ { nullptr };
    std::unique_ptr<HRilTimerCallback> timerCallback_ = nullptr;
    std::unique_ptr<std::thread> eventLoop_ = nullptr;
    static const uint32_t RUNNING_LOCK_DEFAULT_TIMEOUT_US = 200 * 1000; // 200ms
    std::mutex mutexRunningLock_;
    std::atomic_uint runningLockCount_ = 0;
    std::atomic_int runningSerialNum_ = 0;
    static std::shared_ptr<HRilManager> manager_;

private:
    template<typename T>
    void OnReport(std::vector<std::unique_ptr<T>> &subModules, int32_t slotId, const ReportInfo *reportInfo,
        const uint8_t *response, size_t responseLen);
    template<typename ClassTypePtr, typename FuncType, typename... ParamTypes>
    inline int32_t TaskSchedule(
        const std::string _module, ClassTypePtr &_obj, FuncType &&_func, ParamTypes &&... _args);

private:
    const int32_t hrilSimSlotCount_;
    std::vector<std::unique_ptr<HRilCall>> hrilCall_;
    std::vector<std::unique_ptr<HRilModem>> hrilModem_;
    std::vector<std::unique_ptr<HRilNetwork>> hrilNetwork_;
    std::vector<std::unique_ptr<HRilSim>> hrilSim_;
    std::vector<std::unique_ptr<HRilSms>> hrilSms_;
    std::vector<std::unique_ptr<HRilData>> hrilData_;
    std::unordered_map<int32_t, std::list<ReqDataInfo *>> requestList_;
    static std::unordered_map<int32_t, int32_t> notificationMap_;
    std::mutex requestListLock_;
};

#ifdef __cplusplus
extern "C" {
#endif

int32_t GetSimSlotCount(void);
void HRilRegOps(const HRilOps *hrilOps);
void OnCallReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnDataReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnModemReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnNetworkReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnSimReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnSmsReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnTimerCallback(HRilCallbackFun func, uint8_t *param, const struct timeval *tv);

#ifdef __cplusplus
}
#endif
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_IMPL_H
