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

#include "hril_manager.h"

#include "hril_base.h"
#include "hril_notification.h"
#include "hril_request.h"
#include "parameter.h"

namespace OHOS {
namespace Telephony {
constexpr const char *MODULE_HRIL_CALL = "hrilCall";
constexpr const char *MODULE_HRIL_DATA = "hrilData";
constexpr const char *MODULE_HRIL_MODEM = "hrilModem";
constexpr const char *MODULE_HRIL_SIM = "hrilSim";
constexpr const char *MODULE_HRIL_NETWORK = "hrilNetwork";
constexpr const char *MODULE_HRIL_SMS = "hrilSms";
static bool g_isHrilManagerDestory = false;
static std::shared_ptr<HRilManager> g_manager = std::make_shared<HRilManager>();
static pthread_mutex_t dispatchMutex = PTHREAD_MUTEX_INITIALIZER;
std::shared_ptr<HRilManager> HRilManager::manager_ = g_manager;
std::unordered_map<int32_t, int32_t> HRilManager::notificationMap_ = {
#include "hril_notification_map.h"
};

using namespace OHOS::HDI::Power::V1_0;

static bool IsHrilManagerValid()
{
    if (g_isHrilManagerDestory || g_manager == nullptr) {
        return false;
    }
    return true;
}

int32_t HRilManager::GetMaxSimSlotCount()
{
    return hrilSimSlotCount_;
}

ReqDataInfo *HRilManager::CreateHRilRequest(int32_t serial, int32_t slotId, int32_t request)
{
    ReqDataInfo *requestInfo = nullptr;
    HRilSimSlotId simSlotId = (HRilSimSlotId)slotId;
    requestInfo = (ReqDataInfo *)malloc(sizeof(ReqDataInfo));
    if (requestInfo == nullptr) {
        return nullptr;
    }
    requestInfo->slotId = simSlotId;
    requestInfo->request = request;
    requestInfo->serial = serial;
    std::lock_guard<std::mutex> lockRequest(requestListLock_);
    auto iter = requestList_.find(request);
    if (iter != requestList_.end()) {
        std::list<ReqDataInfo *> &reqDataSet = iter->second;
        reqDataSet.push_back(requestInfo);
        TELEPHONY_LOGD("CreateHRilRequest requestId=%{public}d, list size: %{public}zu", request, reqDataSet.size());
    } else {
        TELEPHONY_LOGD("CreateHRilRequest create requestList, requestId=%{public}d", request);
        std::list<ReqDataInfo *> reqDataSet;
        reqDataSet.push_back(requestInfo);
        requestList_.emplace(request, reqDataSet);
    }
    return requestInfo;
}

void HRilManager::ReleaseHRilRequest(int32_t request, ReqDataInfo *requestInfo)
{
    std::lock_guard<std::mutex> lockRequest(requestListLock_);
    auto iter = requestList_.find(request);
    if (iter != requestList_.end()) {
        std::list<ReqDataInfo *> &reqDataSet = iter->second;
        auto it = find(reqDataSet.begin(), reqDataSet.end(), requestInfo);
        if (it != reqDataSet.end()) {
            if (*it != nullptr) {
                free(*it);
            }
            reqDataSet.erase(it);
        }
    }
}

template<typename ClassTypePtr, typename FuncType, typename... ParamTypes>
inline int32_t HRilManager::TaskSchedule(
    const std::string module, ClassTypePtr &_obj, FuncType &&_func, ParamTypes &&... _args)
{
    TELEPHONY_LOGD("%{public}s enter", module.c_str());
    if (_func == nullptr || _obj == nullptr) {
        TELEPHONY_LOGE("%{public}s func or obj is null pointer", module.c_str());
        return HDF_FAILURE;
    }
    pthread_mutex_lock(&dispatchMutex);
    int32_t ret = (_obj.get()->*(_func))(std::forward<ParamTypes>(_args)...);
    pthread_mutex_unlock(&dispatchMutex);
    return ret;
}

void HRilManager::RegisterCallFuncs(int32_t slotId, const HRilCallReq *callFuncs)
{
    if (hrilCall_[slotId] != nullptr) {
        hrilCall_[slotId]->RegisterCallFuncs(callFuncs);
    }
}

void HRilManager::RegisterDataFuncs(int32_t slotId, const HRilDataReq *dataFuncs)
{
    if (hrilData_[slotId] != nullptr) {
        hrilData_[slotId]->RegisterDataFuncs(dataFuncs);
    }
}

void HRilManager::RegisterModemFuncs(int32_t slotId, const HRilModemReq *modemFuncs)
{
    if (hrilModem_[slotId] != nullptr) {
        hrilModem_[slotId]->RegisterModemFuncs(modemFuncs);
    }
}

void HRilManager::RegisterNetworkFuncs(int32_t slotId, const HRilNetworkReq *networkFuncs)
{
    if (hrilNetwork_[slotId] != nullptr) {
        hrilNetwork_[slotId]->RegisterNetworkFuncs(networkFuncs);
    }
}

void HRilManager::RegisterSimFuncs(int32_t slotId, const HRilSimReq *simFuncs)
{
    if (hrilSim_[slotId] != nullptr) {
        hrilSim_[slotId]->RegisterSimFuncs(simFuncs);
    }
}

void HRilManager::RegisterSmsFuncs(int32_t slotId, const HRilSmsReq *smsFuncs)
{
    if (hrilSms_[slotId] != nullptr) {
        hrilSms_[slotId]->RegisterSmsFuncs(smsFuncs);
    }
}

static void RunningLockCallback(uint8_t *param)
{
    if (!IsHrilManagerValid() || param == nullptr) {
        TELEPHONY_LOGE("check nullptr fail.");
        return;
    }
    int serialNum = *reinterpret_cast<int *>(param);
    delete param;
    param = nullptr;
    std::lock_guard<std::mutex> lockRequest(g_manager->mutexRunningLock_);
    TELEPHONY_LOGD("RunningLockCallback, serialNum:%{public}d, runningSerialNum_:%{public}d", serialNum,
        static_cast<int>(g_manager->runningSerialNum_));
    if (g_manager->powerInterface_ == nullptr || serialNum != static_cast<int>(g_manager->runningSerialNum_)) {
        return;
    }
    g_manager->runningLockCount_ = 0;
    g_manager->powerInterface_->SuspendUnblock("HRilRunningLock");
    TELEPHONY_LOGD("RunningLockCallback, UnLock");
}

void HRilManager::ApplyRunningLock(void)
{
    if (!IsHrilManagerValid() || timerCallback_ == nullptr) {
        TELEPHONY_LOGE("check nullptr fail.");
        return;
    }

    std::lock_guard<std::mutex> lockRequest(mutexRunningLock_);
    if (powerInterface_ != nullptr) {
        powerInterface_->SuspendBlock("HRilRunningLock");
        struct timeval tv = { 0, RUNNING_LOCK_DEFAULT_TIMEOUT_US };
        runningLockCount_++;
        runningSerialNum_++;
        uint8_t *serialNum = reinterpret_cast<uint8_t *>(new int(runningSerialNum_));
        timerCallback_->HRilSetTimerCallbackInfo(RunningLockCallback, serialNum, &tv);
        TELEPHONY_LOGD("ApplyRunningLock, runningLockCount_:%{public}d, runningSerialNum_:%{public}d",
            static_cast<int>(runningLockCount_), static_cast<int>(runningSerialNum_));
    } else {
        /* Since the power management subsystem starts slower than the RilAdapter,
         * the wakelock needs to be recreated.
         */
        TELEPHONY_LOGW("The powerInterface_ is nullptr, needs to be recreated.");
        powerInterface_ = IPowerInterface::Get();
        if (powerInterface_ == nullptr) {
            TELEPHONY_LOGE("failed to get power hdi interface");
        }
    }
}

void HRilManager::ReleaseRunningLock(void)
{
    std::lock_guard<std::mutex> lockRequest(mutexRunningLock_);
    TELEPHONY_LOGD("ReleaseRunningLock, runningLockCount_:%{public}d", static_cast<int>(runningLockCount_));
    if (powerInterface_ == nullptr) {
        TELEPHONY_LOGE("powerInterface_ is nullptr");
        return;
    }
    if (runningLockCount_ > 1) {
        runningLockCount_--;
    } else {
        runningLockCount_ = 0;
        powerInterface_->SuspendUnblock("HRilRunningLock");
        TELEPHONY_LOGD("ReleaseRunningLock UnLock");
    }
}

template<typename T>
void HRilManager::OnReport(std::vector<std::unique_ptr<T>> &subModules, int32_t slotId, const ReportInfo *reportInfo,
    const uint8_t *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnReport reportInfo is null!!!");
        return;
    }
    if (slotId < 0 || static_cast<uint32_t>(slotId) >= subModules.size()) {
        TELEPHONY_LOGE("OnReport subModules out of bounds!!!");
        return;
    }
    TELEPHONY_LOGI("OnReport notifyId:%{public}d", reportInfo->notifyId);
    switch (reportInfo->type) {
        case static_cast<int32_t>(ReportType::HRIL_RESPONSE): {
            ReqDataInfo *reqInfo = (ReqDataInfo *)reportInfo->requestInfo;
            if (reqInfo == nullptr) {
                TELEPHONY_LOGE("OnReport reqInfo is null!!!");
                return;
            }
            TELEPHONY_LOGI("OnReport requestId:%{public}d", reqInfo->request);
            HRilRadioResponseInfo responseInfo = {};
            responseInfo.serial = reqInfo->serial;
            responseInfo.error = (HRilErrType)reportInfo->error;
            responseInfo.type = HRIL_RESPONSE_REQUEST;
            if (HRIL_NEED_ACK == reportInfo->ack) {
                ApplyRunningLock();
                responseInfo.type = HRIL_RESPONSE_REQUEST_MUST_ACK;
            }
            int32_t requestId = reqInfo->request;
            ReleaseHRilRequest(requestId, reqInfo);
            subModules[slotId]->template ProcessResponse<T>(requestId, responseInfo, response, responseLen);
            break;
        }
        case static_cast<int32_t>(ReportType::HRIL_NOTIFICATION): {
            int32_t notifyType = HRIL_RESPONSE_NOTICE;
            auto iter = notificationMap_.find(reportInfo->notifyId);
            if (iter != notificationMap_.end()) {
                TELEPHONY_LOGI("OnReport notifyId:%{public}d, value:%{public}d", reportInfo->notifyId, iter->second);
                if (NEED_LOCK == iter->second) {
                    ApplyRunningLock();
                    notifyType = HRIL_RESPONSE_NOTICE_MUST_ACK;
                }
            }
            subModules[slotId]->template ProcessNotify<T>(notifyType, reportInfo, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnCallReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    OnReport(hrilCall_, slotId, reportInfo, response, responseLen);
}

void HRilManager::OnDataReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    OnReport(hrilData_, slotId, reportInfo, response, responseLen);
}

void HRilManager::OnModemReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    OnReport(hrilModem_, slotId, reportInfo, response, responseLen);
}

void HRilManager::OnNetworkReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    OnReport(hrilNetwork_, slotId, reportInfo, response, responseLen);
}

void HRilManager::OnSimReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    OnReport(hrilSim_, slotId, reportInfo, response, responseLen);
}

void HRilManager::OnSmsReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    OnReport(hrilSms_, slotId, reportInfo, response, responseLen);
}

HRilManager::HRilManager() : hrilSimSlotCount_(GetSimSlotCount())
{
    for (int32_t slotId = HRIL_SIM_SLOT_0; slotId < hrilSimSlotCount_; slotId++) {
        hrilCall_.push_back(std::make_unique<HRilCall>(slotId, *this));
        hrilModem_.push_back(std::make_unique<HRilModem>(slotId, *this));
        hrilNetwork_.push_back(std::make_unique<HRilNetwork>(slotId, *this));
        hrilSim_.push_back(std::make_unique<HRilSim>(slotId, *this));
        hrilSms_.push_back(std::make_unique<HRilSms>(slotId, *this));
        hrilData_.push_back(std::make_unique<HRilData>(slotId, *this));
    }
    timerCallback_ = std::make_unique<HRilTimerCallback>();
}

void HRilManager::SetRilCallback(sptr<OHOS::HDI::Ril::V1_0::IRilCallback> callback)
{
    TELEPHONY_LOGD("SetRilCallback");
    for (int32_t slotId = HRIL_SIM_SLOT_0; slotId < hrilSimSlotCount_; slotId++) {
        hrilCall_[slotId]->SetRilCallback(callback);
        hrilModem_[slotId]->SetRilCallback(callback);
        hrilNetwork_[slotId]->SetRilCallback(callback);
        hrilSim_[slotId]->SetRilCallback(callback);
        hrilSms_[slotId]->SetRilCallback(callback);
        hrilData_[slotId]->SetRilCallback(callback);
    }
}

// Call
int32_t HRilManager::SetEmergencyCallList(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::EmergencyInfoList &emergencyInfoList)
{
    return TaskSchedule(
        MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetEmergencyCallList, serialId, emergencyInfoList);
}

int32_t HRilManager::GetEmergencyCallList(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetEmergencyCallList, serialId);
}

int32_t HRilManager::GetCallList(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetCallList, serialId);
}

int32_t HRilManager::Dial(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DialInfo &dialInfo)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::Dial, serialId, dialInfo);
}

int32_t HRilManager::Reject(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::Reject, serialId);
}

int32_t HRilManager::Hangup(int32_t slotId, int32_t serialId, int32_t gsmIndex)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::Hangup, serialId, gsmIndex);
}

int32_t HRilManager::Answer(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::Answer, serialId);
}

int32_t HRilManager::HoldCall(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::HoldCall, serialId);
}

int32_t HRilManager::UnHoldCall(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::UnHoldCall, serialId);
}

int32_t HRilManager::SwitchCall(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SwitchCall, serialId);
}

int32_t HRilManager::CombineConference(int32_t slotId, int32_t serialId, int32_t callType)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::CombineConference, serialId, callType);
}

int32_t HRilManager::SeparateConference(int32_t slotId, int32_t serialId, int32_t callIndex, int32_t callType)
{
    return TaskSchedule(
        MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SeparateConference, serialId, callIndex, callType);
}

int32_t HRilManager::GetCallWaiting(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetCallWaiting, serialId);
}

int32_t HRilManager::SetCallWaiting(int32_t slotId, int32_t serialId, int32_t activate)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetCallWaiting, serialId, activate);
}

int32_t HRilManager::GetCallTransferInfo(int32_t slotId, int32_t serialId, int32_t reason)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetCallTransferInfo, serialId, reason);
}

int32_t HRilManager::SetCallTransferInfo(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::CallForwardSetInfo &callForwardSetInfo)
{
    return TaskSchedule(
        MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetCallTransferInfo, serialId, callForwardSetInfo);
}

int32_t HRilManager::GetCallRestriction(int32_t slotId, int32_t serialId, const std::string &fac)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetCallRestriction, serialId, fac);
}

int32_t HRilManager::SetCallRestriction(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::CallRestrictionInfo &callRestrictionInfo)
{
    return TaskSchedule(
        MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetCallRestriction, serialId, callRestrictionInfo);
}

int32_t HRilManager::GetClip(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetClip, serialId);
}

int32_t HRilManager::SetClip(int32_t slotId, int32_t serialId, int32_t action)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetClip, serialId, action);
}

int32_t HRilManager::GetClir(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetClir, serialId);
}

int32_t HRilManager::SetClir(int32_t slotId, int32_t serialId, int32_t action)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetClir, serialId, action);
}

int32_t HRilManager::SetCallPreferenceMode(int32_t slotId, int32_t serialId, int32_t mode)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetCallPreferenceMode, serialId, mode);
}

int32_t HRilManager::GetCallPreferenceMode(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetCallPreferenceMode, serialId);
}

int32_t HRilManager::SetUssd(int32_t slotId, int32_t serialId, const std::string &str)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetUssd, serialId, str);
}

int32_t HRilManager::GetUssd(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetUssd, serialId);
}

int32_t HRilManager::SetMute(int32_t slotId, int32_t serialId, int32_t mute)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetMute, serialId, mute);
}

int32_t HRilManager::GetMute(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetMute, serialId);
}

int32_t HRilManager::GetCallFailReason(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::GetCallFailReason, serialId);
}

int32_t HRilManager::CallSupplement(int32_t slotId, int32_t serialId, int32_t type)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::CallSupplement, serialId, type);
}

int32_t HRilManager::SendDtmf(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SendDtmf, serialId, dtmfInfo);
}

int32_t HRilManager::StartDtmf(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::StartDtmf, serialId, dtmfInfo);
}

int32_t HRilManager::StopDtmf(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DtmfInfo &dtmfInfo)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::StopDtmf, serialId, dtmfInfo);
}

int32_t HRilManager::SetBarringPassword(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SetBarringInfo &setBarringInfo)
{
    return TaskSchedule(MODULE_HRIL_CALL, hrilCall_[slotId], &HRilCall::SetBarringPassword, serialId, setBarringInfo);
}

// Data
int32_t HRilManager::ActivatePdpContext(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataCallInfo &dataCallInfo)
{
    return TaskSchedule(MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::ActivatePdpContext, serialId, dataCallInfo);
}

int32_t HRilManager::DeactivatePdpContext(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::UniInfo &uniInfo)
{
    return TaskSchedule(MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::DeactivatePdpContext, serialId, uniInfo);
}

int32_t HRilManager::GetPdpContextList(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::UniInfo &uniInfo)
{
    return TaskSchedule(MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::GetPdpContextList, serialId, uniInfo);
}

int32_t HRilManager::SetInitApnInfo(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataProfileDataInfo &dataProfileDataInfo)
{
    return TaskSchedule(MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::SetInitApnInfo, serialId, dataProfileDataInfo);
}

int32_t HRilManager::GetLinkBandwidthInfo(int32_t slotId, int32_t serialId, int32_t cid)
{
    return TaskSchedule(MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::GetLinkBandwidthInfo, serialId, cid);
}

int32_t HRilManager::SetLinkBandwidthReportingRule(int32_t slotId, int32_t serialId,
    const OHOS::HDI::Ril::V1_0::DataLinkBandwidthReportingRule &dataLinkBandwidthReportingRule)
{
    return TaskSchedule(MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::SetLinkBandwidthReportingRule, serialId,
        dataLinkBandwidthReportingRule);
}

int32_t HRilManager::SetDataPermitted(int32_t slotId, int32_t serialId, int32_t dataPermitted)
{
    return TaskSchedule(MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::SetDataPermitted, serialId, dataPermitted);
}

int32_t HRilManager::SetDataProfileInfo(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataProfilesInfo &dataProfilesInfo)
{
    return TaskSchedule(MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::SetDataProfileInfo, serialId, dataProfilesInfo);
}

int32_t HRilManager::SendDataPerformanceMode(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataPerformanceInfo &dataPerformanceInfo)
{
    return TaskSchedule(
        MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::SendDataPerformanceMode, serialId, dataPerformanceInfo);
}

int32_t HRilManager::SendDataSleepMode(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::DataSleepInfo &dataSleepInfo)
{
    return TaskSchedule(MODULE_HRIL_DATA, hrilData_[slotId], &HRilData::SendDataSleepMode, serialId, dataSleepInfo);
}

// Modem
int32_t HRilManager::SetRadioState(int32_t slotId, int32_t serialId, int32_t fun, int32_t rst)
{
    return TaskSchedule(MODULE_HRIL_MODEM, hrilModem_[slotId], &HRilModem::SetRadioState, serialId, fun, rst);
}

int32_t HRilManager::GetRadioState(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_MODEM, hrilModem_[slotId], &HRilModem::GetRadioState, serialId);
}

int32_t HRilManager::GetImei(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_MODEM, hrilModem_[slotId], &HRilModem::GetImei, serialId);
}

int32_t HRilManager::GetMeid(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_MODEM, hrilModem_[slotId], &HRilModem::GetMeid, serialId);
}

int32_t HRilManager::GetVoiceRadioTechnology(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_MODEM, hrilModem_[slotId], &HRilModem::GetVoiceRadioTechnology, serialId);
}

int32_t HRilManager::GetBasebandVersion(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_MODEM, hrilModem_[slotId], &HRilModem::GetBasebandVersion, serialId);
}

int32_t HRilManager::ShutDown(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_MODEM, hrilModem_[slotId], &HRilModem::ShutDown, serialId);
}

int32_t HRilManager::GetSimIO(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimIoRequestInfo &simIO)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::GetSimIO, serialId, simIO);
}

int32_t HRilManager::GetSimStatus(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::GetSimStatus, serialId);
}

int32_t HRilManager::GetImsi(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::GetImsi, serialId);
}

int32_t HRilManager::GetSimLockStatus(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::GetSimLockStatus, serialId, simLockInfo);
}

int32_t HRilManager::SetSimLock(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SetSimLock, serialId, simLockInfo);
}

int32_t HRilManager::ChangeSimPassword(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimPasswordInfo &simPassword)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::ChangeSimPassword, serialId, simPassword);
}

int32_t HRilManager::UnlockPin(int32_t slotId, int32_t serialId, const std::string &pin)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::UnlockPin, serialId, pin);
}

int32_t HRilManager::UnlockPuk(int32_t slotId, int32_t serialId, const std::string &puk, const std::string &pin)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::UnlockPuk, serialId, puk, pin);
}

int32_t HRilManager::UnlockPin2(int32_t slotId, int32_t serialId, const std::string &pin2)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::UnlockPin2, serialId, pin2);
}

int32_t HRilManager::UnlockPuk2(int32_t slotId, int32_t serialId, const std::string &puk2, const std::string &pin2)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::UnlockPuk2, serialId, puk2, pin2);
}

int32_t HRilManager::SetActiveSim(int32_t slotId, int32_t serialId, int32_t index, int32_t enable)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SetActiveSim, serialId, index, enable);
}

int32_t HRilManager::SimStkSendTerminalResponse(int32_t slotId, int32_t serialId, const std::string &strCmd)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SimStkSendTerminalResponse, serialId, strCmd);
}

int32_t HRilManager::SimStkSendEnvelope(int32_t slotId, int32_t serialId, const std::string &strCmd)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SimStkSendEnvelope, serialId, strCmd);
}

int32_t HRilManager::SimStkSendCallSetupRequestResult(int32_t slotId, int32_t serialId, int32_t accept)
{
    return TaskSchedule(
        MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SimStkSendCallSetupRequestResult, serialId, accept);
}

int32_t HRilManager::SimStkIsReady(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SimStkIsReady, serialId);
}

int32_t HRilManager::GetRadioProtocol(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::GetRadioProtocol, serialId);
}

int32_t HRilManager::SetRadioProtocol(
    int32_t slotId, int32_t serialId, const HDI::Ril::V1_0::RadioProtocol &radioProtocol)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SetRadioProtocol, serialId, radioProtocol);
}

int32_t HRilManager::SimOpenLogicalChannel(int32_t slotId, int32_t serialId, const std::string &appID, int32_t p2)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SimOpenLogicalChannel, serialId, appID, p2);
}

int32_t HRilManager::SimCloseLogicalChannel(int32_t slotId, int32_t serialId, int32_t channelId)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SimCloseLogicalChannel, serialId, channelId);
}

int32_t HRilManager::SimTransmitApduLogicalChannel(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO)
{
    return TaskSchedule(
        MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SimTransmitApduLogicalChannel, serialId, apduSimIO);
}

int32_t HRilManager::SimTransmitApduBasicChannel(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SimTransmitApduBasicChannel, serialId, apduSimIO);
}

int32_t HRilManager::SimAuthentication(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SimAuthenticationRequestInfo &simAuthInfo)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::SimAuthentication, serialId, simAuthInfo);
}

int32_t HRilManager::UnlockSimLock(int32_t slotId, int32_t serialId, int32_t lockType, const std::string &key)
{
    return TaskSchedule(MODULE_HRIL_SIM, hrilSim_[slotId], &HRilSim::UnlockSimLock, serialId, lockType, key);
}

// Network
int32_t HRilManager::GetSignalStrength(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetSignalStrength, serialId);
}

int32_t HRilManager::GetCsRegStatus(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetCsRegStatus, serialId);
}

int32_t HRilManager::GetPsRegStatus(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetPsRegStatus, serialId);
}

int32_t HRilManager::GetOperatorInfo(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetOperatorInfo, serialId);
}

int32_t HRilManager::GetNetworkSearchInformation(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetNetworkSearchInformation, serialId);
}

int32_t HRilManager::GetNetworkSelectionMode(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetNetworkSelectionMode, serialId);
}

int32_t HRilManager::SetNetworkSelectionMode(
    int32_t slotId, int32_t serialId, const HDI::Ril::V1_0::SetNetworkModeInfo &networkModeInfo)
{
    return TaskSchedule(
        MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::SetNetworkSelectionMode, serialId, networkModeInfo);
}

int32_t HRilManager::GetNeighboringCellInfoList(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetNeighboringCellInfoList, serialId);
}

int32_t HRilManager::GetCurrentCellInfo(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetCurrentCellInfo, serialId);
}

int32_t HRilManager::SetPreferredNetwork(int32_t slotId, int32_t serialId, int32_t preferredNetworkType)
{
    return TaskSchedule(
        MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::SetPreferredNetwork, serialId, preferredNetworkType);
}

int32_t HRilManager::GetPreferredNetwork(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetPreferredNetwork, serialId);
}

int32_t HRilManager::GetPhysicalChannelConfig(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::GetPhysicalChannelConfig, serialId);
}

int32_t HRilManager::SetLocateUpdates(int32_t slotId, int32_t serialId, const HDI::Ril::V1_0::RilRegNotifyMode mode)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::SetLocateUpdates, serialId, mode);
}

int32_t HRilManager::SetNotificationFilter(int32_t slotId, int32_t serialId, int32_t newFilter)
{
    return TaskSchedule(
        MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::SetNotificationFilter, serialId, newFilter);
}

int32_t HRilManager::SetDeviceState(int32_t slotId, int32_t serialId, int32_t deviceStateType, int32_t deviceStateOn)
{
    return TaskSchedule(MODULE_HRIL_NETWORK, hrilNetwork_[slotId], &HRilNetwork::SetDeviceState, serialId,
        deviceStateType, deviceStateOn);
}

// Sms
int32_t HRilManager::SendGsmSms(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::GsmSmsMessageInfo &gsmSmsMessageInfo)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::SendGsmSms, serialId, gsmSmsMessageInfo);
}

int32_t HRilManager::SendCdmaSms(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SendCdmaSmsMessageInfo &cdmaSmsMessageInfo)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::SendCdmaSms, serialId, cdmaSmsMessageInfo);
}

int32_t HRilManager::AddSimMessage(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::AddSimMessage, serialId, smsMessageIOInfo);
}

int32_t HRilManager::DelSimMessage(int32_t slotId, int32_t serialId, int32_t index)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::DelSimMessage, serialId, index);
}

int32_t HRilManager::UpdateSimMessage(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::UpdateSimMessage, serialId, smsMessageIOInfo);
}

int32_t HRilManager::AddCdmaSimMessage(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::AddCdmaSimMessage, serialId, smsMessageIOInfo);
}

int32_t HRilManager::DelCdmaSimMessage(int32_t slotId, int32_t serialId, int32_t index)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::DelCdmaSimMessage, serialId, index);
}

int32_t HRilManager::UpdateCdmaSimMessage(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::UpdateCdmaSimMessage, serialId, smsMessageIOInfo);
}

int32_t HRilManager::SetSmscAddr(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::ServiceCenterAddress &serviceCenterAddress)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::SetSmscAddr, serialId, serviceCenterAddress);
}

int32_t HRilManager::GetSmscAddr(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::GetSmscAddr, serialId);
}

int32_t HRilManager::SetCBConfig(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::CBConfigInfo &cellBroadcastInfo)
{
    return HRilManager::TaskSchedule(
        MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::SetCBConfig, serialId, cellBroadcastInfo);
}

int32_t HRilManager::GetCBConfig(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::GetCBConfig, serialId);
}

int32_t HRilManager::SetCdmaCBConfig(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::CdmaCBConfigInfoList &cdmaCBConfigInfoList)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::SetCdmaCBConfig, serialId, cdmaCBConfigInfoList);
}

int32_t HRilManager::GetCdmaCBConfig(int32_t slotId, int32_t serialId)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::GetCdmaCBConfig, serialId);
}

int32_t HRilManager::SendSmsMoreMode(
    int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::GsmSmsMessageInfo &gsmSmsMessageInfo)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::SendSmsMoreMode, serialId, gsmSmsMessageInfo);
}

int32_t HRilManager::SendSmsAck(int32_t slotId, int32_t serialId, const OHOS::HDI::Ril::V1_0::ModeData &modeData)
{
    return TaskSchedule(MODULE_HRIL_SMS, hrilSms_[slotId], &HRilSms::SendSmsAck, serialId, modeData);
}

int32_t HRilManager::SendRilAck()
{
    ReleaseRunningLock();
    return HRIL_ERR_SUCCESS;
}

HRilManager::~HRilManager()
{
    g_isHrilManagerDestory = true;
    if (timerCallback_ == nullptr || timerCallback_->event_ == nullptr ||
        timerCallback_->event_->IsNormalDestory()) {
        return;
    }
    timerCallback_->event_->SetNormalDestory(true);
    timerCallback_->OnTriggerEvent();
    if (eventLoop_ == nullptr || !eventLoop_->joinable()) {
        return;
    }
    eventLoop_->join();
    TELEPHONY_LOGI("~HRilManager end");
}

#ifdef __cplusplus
extern "C" {
#endif

int32_t GetSimSlotCount()
{
    char simSlotCount[HRIL_SYSPARA_SIZE] = { 0 };
    GetParameter(HRIL_TEL_SIM_SLOT_COUNT, HRIL_DEFAULT_SLOT_COUNT, simSlotCount, HRIL_SYSPARA_SIZE);
    return std::atoi(simSlotCount);
}

static void HRilBootUpEventLoop()
{
    if (!IsHrilManagerValid() || g_manager->timerCallback_ == nullptr) {
        return;
    }
    g_manager->timerCallback_->EventLoop();
}

void HRilInit(void)
{
    if (!IsHrilManagerValid()) {
        TELEPHONY_LOGE("HRilInit: g_manager is nullptr");
        return;
    }
    if (g_manager->powerInterface_ == nullptr) {
        g_manager->powerInterface_ = IPowerInterface::Get();
        if (g_manager->powerInterface_ == nullptr) {
            TELEPHONY_LOGE("failed to get power hdi interface");
        }
    }
    if (g_manager->eventLoop_ != nullptr) {
        TELEPHONY_LOGD("eventLoop_ has exit");
        return;
    }
    g_manager->eventLoop_ = std::make_unique<std::thread>(HRilBootUpEventLoop);
}

void HRilRegOps(const HRilOps *hrilOps)
{
    static HRilOps callBacks = { 0 };
    static RegisterState rilRegisterStatus = RIL_REGISTER_IS_NONE;

    if (hrilOps == nullptr || !IsHrilManagerValid()) {
        TELEPHONY_LOGE("HRilRegOps: param is nullptr");
        return;
    }
    if (rilRegisterStatus > RIL_REGISTER_IS_NONE) {
        TELEPHONY_LOGE("HRilRegOps is running!!!!");
        return;
    }
    rilRegisterStatus = RIL_REGISTER_IS_RUNNING;
    (void)memcpy_s(&callBacks, sizeof(HRilOps), hrilOps, sizeof(HRilOps));
    for (int32_t slotId = HRIL_SIM_SLOT_0; slotId < g_manager->GetMaxSimSlotCount(); slotId++) {
        if (callBacks.smsOps != nullptr) {
            g_manager->RegisterSmsFuncs(slotId, callBacks.smsOps);
        }
        if (callBacks.callOps != nullptr) {
            g_manager->RegisterCallFuncs(slotId, callBacks.callOps);
        }
        if (callBacks.dataOps != nullptr) {
            g_manager->RegisterDataFuncs(slotId, callBacks.dataOps);
        }
        if (callBacks.modemOps != nullptr) {
            g_manager->RegisterModemFuncs(slotId, callBacks.modemOps);
        }
        if (callBacks.networkOps != nullptr) {
            g_manager->RegisterNetworkFuncs(slotId, callBacks.networkOps);
        }
        if (callBacks.simOps != nullptr) {
            g_manager->RegisterSimFuncs(slotId, callBacks.simOps);
        }
    }
}

void OnCallReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (!IsHrilManagerValid()) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d", slotId);
        return;
    }
    g_manager->OnCallReport(slotId, &reportInfo, response, responseLen);
}

void OnDataReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (!IsHrilManagerValid()) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d", slotId);
        return;
    }
    g_manager->OnDataReport(slotId, &reportInfo, response, responseLen);
}

void OnModemReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (!IsHrilManagerValid()) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d", slotId);
        return;
    }
    g_manager->OnModemReport(slotId, &reportInfo, response, responseLen);
}

void OnNetworkReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (!IsHrilManagerValid()) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d", slotId);
        return;
    }
    g_manager->OnNetworkReport(slotId, &reportInfo, response, responseLen);
}

void OnSimReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (!IsHrilManagerValid()) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d", slotId);
        return;
    }
    g_manager->OnSimReport(slotId, &reportInfo, response, responseLen);
}

void OnSmsReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (!IsHrilManagerValid()) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d", slotId);
        return;
    }
    g_manager->OnSmsReport(slotId, &reportInfo, response, responseLen);
}

void OnTimerCallback(HRilCallbackFun func, uint8_t *param, const struct timeval *tv)
{
    if (!IsHrilManagerValid() || g_manager->timerCallback_ == nullptr) {
        TELEPHONY_LOGE("HrilManager or timerCallback is nullptr");
        return;
    }
    g_manager->timerCallback_->HRilSetTimerCallbackInfo(func, param, tv);
}

#ifdef __cplusplus
}
#endif
} // namespace Telephony
} // namespace OHOS
