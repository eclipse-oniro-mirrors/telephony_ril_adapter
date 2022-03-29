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

#include "parameter.h"

#include "hril_base.h"
#include "hril_request.h"
#include "hril_notification.h"

namespace OHOS {
namespace Telephony {
static std::unique_ptr<HRilManager> g_manager = std::make_unique<HRilManager>();

std::unordered_map<int32_t, int32_t> HRilManager::notificationMap_ = {
#include "hril_notification_map.h"
};

// Distribute the request event to a specific card slot processing flow
static int32_t DispatchModule(int32_t slotId, int32_t cmd, struct HdfSBuf *data)
{
    if (g_manager == nullptr) {
        TELEPHONY_LOGE("Manager is nullptr, id:%{public}d!", slotId);
        return HDF_FAILURE;
    }
    int32_t ret = g_manager->Dispatch(slotId, cmd, data);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("HRilManager::Dispatch is failed!");
        return ret;
    }
    return ret;
}

static int32_t RegisterManagerNotifyCallback(const HdfRemoteService *serviceCallbackInd)
{
    if (g_manager == nullptr) {
        TELEPHONY_LOGE("HRilManager is nullptr!");
        return HDF_FAILURE;
    }
    g_manager->RegisterModulesNotifyCallback(serviceCallbackInd);
    return HRIL_ERR_SUCCESS;
}

static int32_t RegisterManagerResponseCallback(const HdfRemoteService *serviceCallbackResp)
{
    if (g_manager == nullptr) {
        TELEPHONY_LOGE("HRilManager is nullptr!");
        return HDF_FAILURE;
    }
    g_manager->RegisterModulesResponseCallback(serviceCallbackResp);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilManager::GetMaxSimSlotCount()
{
    return hrilSimSlotCount_;
}

int32_t HRilManager::Dispatch(int32_t slotId, int32_t code, struct HdfSBuf *data)
{
    if (hrilCall_[slotId] != nullptr && hrilCall_[slotId]->IsCallRespOrNotify(code)) {
        hrilCall_[slotId]->ProcessRequest<HRilCall>(code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilSms_[slotId] != nullptr && hrilSms_[slotId]->IsSmsRespOrNotify(code)) {
        hrilSms_[slotId]->ProcessRequest<HRilSms>(code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilSim_[slotId] != nullptr && hrilSim_[slotId]->IsSimRespOrNotify(code)) {
        hrilSim_[slotId]->ProcessRequest<HRilSim>(code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilNetwork_[slotId] != nullptr && hrilNetwork_[slotId]->IsNetworkRespOrNotify(code)) {
        hrilNetwork_[slotId]->ProcessRequest<HRilNetwork>(code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilModem_[slotId] != nullptr && hrilModem_[slotId]->IsModemRespOrNotify(code)) {
        hrilModem_[slotId]->ProcessRequest<HRilModem>(code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilData_[slotId] != nullptr && hrilData_[slotId]->IsDataRespOrNotify(code)) {
        hrilData_[slotId]->ProcessRequest<HRilData>(code, data);
        return HRIL_ERR_SUCCESS;
    }
    return HDF_FAILURE;
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
        TELEPHONY_LOGI("CreateHRilRequest requestId=%{public}d, list size: %{public}zu", request, reqDataSet.size());
    } else {
        TELEPHONY_LOGI("CreateHRilRequest  create requestList, requestId=%{public}d", request);
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

int32_t HRilManager::ReportToParent(int32_t requestNum, const HdfSBuf *dataSbuf)
{
    int32_t ret;
    if (serviceCallback_ != nullptr && serviceCallback_->dispatcher != nullptr) {
        ret = serviceCallback_->dispatcher->Dispatch(
            const_cast<HdfRemoteService *>(serviceCallback_), requestNum, const_cast<HdfSBuf *>(dataSbuf), nullptr);
    } else {
        TELEPHONY_LOGE("it is null, serviceCallback_=%{public}p", serviceCallback_);
        ret = HDF_FAILURE;
    }
    return ret;
}

int32_t HRilManager::NotifyToParent(int32_t requestNum, const HdfSBuf *dataSbuf)
{
    int32_t ret;
    if (serviceCallbackNotify_ != nullptr && serviceCallbackNotify_->dispatcher != nullptr) {
        ret = serviceCallbackNotify_->dispatcher->Dispatch(const_cast<HdfRemoteService *>(serviceCallbackNotify_),
            requestNum, const_cast<HdfSBuf *>(dataSbuf), nullptr);
    } else {
        TELEPHONY_LOGE("it is null, serviceCallbackNotify_=%{public}p", serviceCallbackNotify_);
        ret = HDF_FAILURE;
    }
    return ret;
}

void HRilManager::RegisterModulesNotifyCallback(const HdfRemoteService *serviceCallbackInd)
{
    serviceCallbackNotify_ = serviceCallbackInd;
}

void HRilManager::RegisterModulesResponseCallback(const HdfRemoteService *serviceCallback)
{
    serviceCallback_ = serviceCallback;
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
    if (g_manager == nullptr || param == nullptr) {
        TELEPHONY_LOGE("check nullptr fail.");
        return;
    }
    int serialNum = *((int *)param);
    delete param;
    param = nullptr;
    std::lock_guard<std::mutex> lockRequest(g_manager->mutexRunningLock_);
    TELEPHONY_LOGI("RunningLockCallback, serialNum:%{public}d, runningSerialNum_:%{public}d",
        serialNum, (int)g_manager->runningSerialNum_);
    if (g_manager->runningLock_ == nullptr || serialNum != (int)g_manager->runningSerialNum_) {
        return;
    }
    g_manager->runningLockCount_ = 0;
    g_manager->runningLock_->UnLock();
    TELEPHONY_LOGI("RunningLockCallback, UnLock");
}

void HRilManager::ApplyRunningLock(void)
{
    if (g_manager == nullptr || timerCallback_ == nullptr) {
        TELEPHONY_LOGE("check nullptr fail.");
        return;
    }
    std::lock_guard<std::mutex> lockRequest(mutexRunningLock_);
    if (runningLock_ != nullptr) {
        runningLock_->Lock();
        struct timeval tv = {RUNNING_LOCK_DEFAULT_TIMEOUT_S, 0};
        runningLockCount_++;
        runningSerialNum_++;
        int *serialNum = new int((int)runningSerialNum_);
        timerCallback_->HRilSetTimerCallbackInfo(RunningLockCallback, (uint8_t *)serialNum, &tv);
        TELEPHONY_LOGI("ApplyRunningLock, runningLockCount_:%{public}d, runningSerialNum_:%{public}d",
            (int)runningLockCount_, (int)runningSerialNum_);
    } else {
        /* Since the power management subsystem starts slower than the RilAdapter,
         * the wakelock needs to be recreated.
         */
        TELEPHONY_LOGW("The runningLock is nullptr, needs to be recreated.");
        auto &powerMgrClient = PowerMgr::PowerMgrClient::GetInstance();
        g_manager->runningLock_ =
            powerMgrClient.CreateRunningLock("HRilRunningLock", PowerMgr::RunningLockType::RUNNINGLOCK_BACKGROUND);
    }
}

void HRilManager::ReleaseRunningLock(void)
{
    std::lock_guard<std::mutex> lockRequest(mutexRunningLock_);
    TELEPHONY_LOGI("ReleaseRunningLock, runningLockCount_:%{public}d", (int)runningLockCount_);
    if (runningLock_ == nullptr) {
        TELEPHONY_LOGE("runningLock_ is nullptr");
        return;
    }
    if (runningLockCount_ > 1) {
        runningLockCount_--;
    } else {
        runningLockCount_ = 0;
        runningLock_->UnLock();
        TELEPHONY_LOGI("ReleaseRunningLock UnLock");
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
    switch (reportInfo->type) {
        case (int32_t)ReportType::HRIL_RESPONSE: {
            ReqDataInfo *reqInfo = (ReqDataInfo *)reportInfo->requestInfo;
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
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
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

void HRilManager::OnSimReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    OnReport(hrilSim_, slotId, reportInfo, response, responseLen);
}

void HRilManager::OnSmsReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    OnReport(hrilSms_, slotId, reportInfo, response, responseLen);
}

HRilManager::HRilManager()
    : hrilSimSlotCount_(GetSimSlotCount())
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

HRilManager::~HRilManager() {}

#ifdef __cplusplus
extern "C" {
#endif

int32_t GetSimSlotCount()
{
    char simSlotCount[HRIL_SYSPARA_SIZE] = {0};
    GetParameter(HRIL_TEL_SIM_SLOT_COUNT.c_str(),
        HRIL_DEFAULT_SLOT_COUNT.c_str(), simSlotCount, HRIL_SYSPARA_SIZE);
    return std::atoi(simSlotCount);
}

int32_t DispatchRequest(int32_t cmd, struct HdfSBuf *data)
{
    if (data == nullptr) {
        TELEPHONY_LOGE("miss callback parameter");
        return HDF_ERR_INVALID_PARAM;
    }
    switch (cmd) {
        case HRIL_ADAPTER_RADIO_SEND_ACK: {
            if (g_manager == nullptr) {
                TELEPHONY_LOGE("g_manager is nullptr! cmd:%{public}d", cmd);
                return HDF_ERR_INVALID_OBJECT;
            }
            g_manager->ReleaseRunningLock();
            break;
        }
        case HRIL_ADAPTER_RADIO_INDICATION: {
            HdfRemoteService *serviceCallbackInd = HdfSbufReadRemoteService(data);
            if (serviceCallbackInd == nullptr) {
                TELEPHONY_LOGE("miss callback parameter");
                return HDF_ERR_INVALID_PARAM;
            }
            RegisterManagerNotifyCallback(serviceCallbackInd);
            break;
        }
        case HRIL_ADAPTER_RADIO_RESPONSE: {
            HdfRemoteService *serviceCallback = HdfSbufReadRemoteService(data);
            if (serviceCallback == nullptr) {
                TELEPHONY_LOGE("miss callback parameter");
                return HDF_ERR_INVALID_PARAM;
            }
            RegisterManagerResponseCallback(serviceCallback);
            break;
        }
        default:
            int32_t slotId = 0;
            if (!HdfSbufReadInt32(data, &slotId)) {
                TELEPHONY_LOGE("Hdf sbuf read slotId is failed!");
            }
            return DispatchModule(slotId, cmd, data);
    }
    return HRIL_ERR_SUCCESS;
}

static void HRilBootUpEventLoop()
{
    if (g_manager == nullptr || g_manager->timerCallback_ == nullptr) {
        return;
    }
    g_manager->timerCallback_->EventLoop();
}

void HRilRegOps(const HRilOps *hrilOps)
{
    static HRilOps callBacks = {0};
    static RegisterState rilRegisterStatus = RIL_REGISTER_IS_NONE;

    if (hrilOps == nullptr || g_manager == nullptr) {
        TELEPHONY_LOGE("HRilRegOps: param is nullptr");
        return;
    }
    if (rilRegisterStatus > RIL_REGISTER_IS_NONE) {
        TELEPHONY_LOGE("HRilRegOps is running!!!!");
        return;
    }
    rilRegisterStatus = RIL_REGISTER_IS_RUNNING;
    auto &powerMgrClient = PowerMgr::PowerMgrClient::GetInstance();
    g_manager->runningLock_ =
        powerMgrClient.CreateRunningLock("HRilRunningLock", PowerMgr::RunningLockType::RUNNINGLOCK_BACKGROUND);
    g_manager->eventLoop_ = std::make_unique<std::thread>(HRilBootUpEventLoop);
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
    if (g_manager == nullptr) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &g_manager);
        return;
    }
    g_manager->OnCallReport(slotId, &reportInfo, response, responseLen);
}

void OnDataReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_manager == nullptr) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &g_manager);
        return;
    }
    g_manager->OnDataReport(slotId, &reportInfo, response, responseLen);
}

void OnModemReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_manager == nullptr) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &g_manager);
        return;
    }
    g_manager->OnModemReport(slotId, &reportInfo, response, responseLen);
}

void OnNetworkReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_manager == nullptr) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &g_manager);
        return;
    }
    g_manager->OnNetworkReport(slotId, &reportInfo, response, responseLen);
}

void OnSimReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_manager == nullptr) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &g_manager);
        return;
    }
    g_manager->OnSimReport(slotId, &reportInfo, response, responseLen);
}

void OnSmsReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    if (g_manager == nullptr) {
        TELEPHONY_LOGE("HrilManager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &g_manager);
        return;
    }
    g_manager->OnSmsReport(slotId, &reportInfo, response, responseLen);
}

void OnTimerCallback(HRilCallbackFun func, uint8_t *param, const struct timeval *tv)
{
    if (g_manager == nullptr || g_manager->timerCallback_ == nullptr) {
        TELEPHONY_LOGE("HrilManager or timerCallback is nullptr, addr:%{public}p!", &g_manager);
        return;
    }
    g_manager->timerCallback_->HRilSetTimerCallbackInfo(func, param, tv);
}

#ifdef __cplusplus
}
#endif
} // namespace Telephony
} // namespace OHOS