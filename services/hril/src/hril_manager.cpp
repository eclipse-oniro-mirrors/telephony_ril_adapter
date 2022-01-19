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
#include "hril_request.h"

namespace OHOS {
namespace Telephony {
static std::unique_ptr<HRilManager> g_manager = std::make_unique<HRilManager>();

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
            int requestId = reqInfo->request;
            ReleaseHRilRequest(requestId, reqInfo);
            subModules[slotId]->template ProcessResponse<T>(requestId, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            subModules[slotId]->template ProcessNotify<T>(reportInfo, response, responseLen);
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
{
    for (int32_t slotId = HRIL_SIM_SLOT_0; slotId < HRIL_SIM_SLOT_NUM; slotId++) {
        hrilCall_.push_back(std::make_unique<HRilCall>(slotId, *this));
        hrilModem_.push_back(std::make_unique<HRilModem>(slotId, *this));
        hrilNetwork_.push_back(std::make_unique<HRilNetwork>(slotId, *this));
        hrilSim_.push_back(std::make_unique<HRilSim>(slotId, *this));
        hrilSms_.push_back(std::make_unique<HRilSms>(slotId, *this));
        hrilData_.push_back(std::make_unique<HRilData>(slotId, *this));
    }
}

HRilManager::~HRilManager() {}

#ifdef __cplusplus
extern "C" {
#endif

int32_t DispatchRequest(int32_t cmd, struct HdfSBuf *data)
{
    if (data == nullptr) {
        TELEPHONY_LOGE("miss callback parameter");
        return HDF_ERR_INVALID_PARAM;
    }
    switch (cmd) {
        case HRIL_ADAPTER_RADIO_INDICATION: {
            HdfRemoteService *serviceCallbackInd = HdfSBufReadRemoteService(data);
            if (serviceCallbackInd == nullptr) {
                TELEPHONY_LOGE("miss callback parameter");
                return HDF_ERR_INVALID_PARAM;
            }
            RegisterManagerNotifyCallback(serviceCallbackInd);
            break;
        }
        case HRIL_ADAPTER_RADIO_RESPONSE: {
            HdfRemoteService *serviceCallback = HdfSBufReadRemoteService(data);
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

void HRilRegOps(const HRilOps *hrilOps)
{
    static HRilOps callBacks = {0};
    static RegisterState rilRegisterStatus = RIL_REGISTER_IS_NONE;

    if (hrilOps == nullptr) {
        TELEPHONY_LOGE("HRilRegOps: HRilRegOps * nullptr");
        return;
    }
    if (rilRegisterStatus > RIL_REGISTER_IS_NONE) {
        TELEPHONY_LOGE("HRilRegOps is running!!!!");
        return;
    }
    rilRegisterStatus = RIL_REGISTER_IS_RUNNING;

    (void)memcpy_s(&callBacks, sizeof(HRilOps), hrilOps, sizeof(HRilOps));
    for (int32_t slotId = HRIL_SIM_SLOT_0; slotId < HRIL_SIM_SLOT_NUM; slotId++) {
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
#ifdef __cplusplus
}
#endif
} // namespace Telephony
} // namespace OHOS