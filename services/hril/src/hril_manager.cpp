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
using namespace std;

std::map<int, std::unique_ptr<HRilManager>> g_manager;
pthread_rwlock_t *GetRadioServiceLock()
{
    return &g_radioServiceRwLock;
}

// Distribute the request event to a specific card slot processing flow
static void DispatchModule(int32_t slotId, int32_t cmd, struct HdfSBuf *data)
{
    auto itFunc = g_manager.find(slotId);
    if (itFunc != g_manager.end()) {
        auto &manager = itFunc->second;
        if (manager != nullptr) {
            int32_t ret = manager->Dispatch(slotId, cmd, data);
            if (ret != HRIL_ERR_SUCCESS) {
                TELEPHONY_LOGE("HRilManager::Dispatch is failed!");
            }
        } else {
            TELEPHONY_LOGE("Manager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &manager);
        }
    } else {
        TELEPHONY_LOGE("Can not find slotId in g_manager: %{public}d!", slotId);
    }
}

static void RegisterManagerNotifyCallback(int32_t slotId, const HdfRemoteService *serviceCallbackInd)
{
    auto itFunc = g_manager.find(slotId);
    if (itFunc != g_manager.end()) {
        auto &manager = itFunc->second;
        if (manager != nullptr) {
            manager->RegisterModulesNotifyCallback(serviceCallbackInd);
        } else {
            TELEPHONY_LOGE("Manager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &manager);
        }
    } else {
        TELEPHONY_LOGE("Can not find slotId in g_manager: %{public}d!", slotId);
    }
}

static void RegisterManagerResponseCallback(int32_t slotId, const HdfRemoteService *serviceCallbackInd)
{
    auto itFunc = g_manager.find(slotId);
    if (itFunc != g_manager.end()) {
        auto &manager = itFunc->second;
        if (manager != nullptr) {
            manager->RegisterModulesResponseCallback(serviceCallbackInd);
        } else {
            TELEPHONY_LOGE("Manager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &manager);
        }
    } else {
        TELEPHONY_LOGE("Can not find slotId in g_manager: %{public}d!", slotId);
    }
}

int32_t HRilManager::Dispatch(int32_t slotId, int32_t code, struct HdfSBuf *data)
{
    if (hrilCall_ != nullptr && hrilCall_->IsCallRespOrNotify(code)) {
        hrilCall_->ProcessCallRequest(slotId, code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilSms_ != nullptr && hrilSms_->IsSmsRespOrNotify(code)) {
        hrilSms_->ProcessSmsRequest(slotId, code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilSim_ != nullptr && hrilSim_->IsSimRespOrNotify(code)) {
        hrilSim_->ProcessSimRequest(slotId, code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilNetwork_ != nullptr && hrilNetwork_->IsNetworkRespOrNotify(code)) {
        hrilNetwork_->ProcessNetworkRequest(slotId, code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilModem_ != nullptr && hrilModem_->IsModemRespOrNotify(code)) {
        hrilModem_->ProcessCommonRequest(slotId, code, data);
        return HRIL_ERR_SUCCESS;
    }
    if (hrilData_ != nullptr && hrilData_->IsDataRespOrNotify(code)) {
        hrilData_->ProcessDataRequest(slotId, code, data);
        return HRIL_ERR_SUCCESS;
    }
    return HDF_FAILURE;
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

ReqDataInfo *HRilManager::CreateHRilRequest(int32_t serial, int32_t slotId, int32_t request)
{
    ReqDataInfo *requestInfo = nullptr;
    HRilSimSlotId simSlotId = (HRilSimSlotId)slotId;
    requestInfo = (ReqDataInfo *)calloc(1, sizeof(ReqDataInfo));
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
        TELEPHONY_LOGI("CreateHRilRequest requestid=%{public}d, list size: %{public}zu", request, reqDataSet.size());
    } else {
        TELEPHONY_LOGI("CreateHRilRequest  create requestlist, requestid=%{public}d", request);
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

void HRilManager::RegisterCallFuncs(const HRilCallReq *callFuncs)
{
    if (hrilCall_ != nullptr) {
        hrilCall_->RegisterCallFuncs(callFuncs);
    }
}

void HRilManager::RegisterDataFuncs(const HRilDataReq *dataFuncs)
{
    if (hrilData_ != nullptr) {
        hrilData_->RegisterDataFuncs(dataFuncs);
    }
}

void HRilManager::RegisterModemFuncs(const HRilModemReq *modemFuncs)
{
    if (hrilModem_ != nullptr) {
        hrilModem_->RegisterModemFuncs(modemFuncs);
    }
}

void HRilManager::RegisterNetworkFuncs(const HRilNetworkReq *networkFuncs)
{
    if (hrilNetwork_ != nullptr) {
        hrilNetwork_->RegisterNetworkFuncs(networkFuncs);
    }
}

void HRilManager::RegisterSimFuncs(const HRilSimReq *simFuncs)
{
    if (hrilSim_ != nullptr) {
        hrilSim_->RegisterSimFuncs(simFuncs);
    }
}

void HRilManager::RegisterSmsFuncs(const HRilSmsReq *smsFuncs)
{
    if (hrilSms_ != nullptr) {
        hrilSms_->RegisterSmsFuncs(smsFuncs);
    }
}

void HRilManager::OnCallReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnCallReport reportInfo is null!!!");
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
            hrilCall_->ProcessCallResponse(slotId, requestId, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilCall_->ProcessCallNotify(slotId, reportInfo, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnDataReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnDataReport reportInfo is null!!!");
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
            hrilData_->ProcessDataResponse(slotId, requestId, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilData_->ProcessDataNotify(slotId, reportInfo, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnModemReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnModemReport reportInfo is null!!!");
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
            hrilModem_->ProcessModemResponse(slotId, requestId, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilModem_->ProcessModemNotify(slotId, reportInfo, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnNetworkReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnNetworkReport reportInfo is null!!!");
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
            hrilNetwork_->ProcessNetworkResponse(slotId, requestId, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilNetwork_->ProcessNetworkNotify(slotId, reportInfo, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnSimReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnSimReport reportInfo is null!!!");
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
            hrilSim_->ProcessSimResponse(slotId, requestId, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilSim_->ProcessSimNotify(slotId, reportInfo, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnSmsReport(
    int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnSmsReport reportInfo is null!!!");
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
            hrilSms_->ProcessSmsResponse(slotId, requestId, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilSms_->ProcessSmsNotify(slotId, reportInfo, response, responseLen);
            break;
        }
        default:
            break;
    }
}

HRilManager::HRilManager()
{
    hrilCall_ = std::make_unique<HRilCall>(*this);
    hrilModem_ = std::make_unique<HRilModem>(*this);
    hrilNetwork_ = std::make_unique<HRilNetwork>(*this);
    hrilSim_ = std::make_unique<HRilSim>(*this);
    hrilSms_ = std::make_unique<HRilSms>(*this);
    hrilData_ = std::make_unique<HRilData>(*this);
}

HRilManager::~HRilManager() {}

#ifdef __cplusplus
extern "C" {
#endif

int32_t DispatchRequest(int32_t slotId, int32_t cmd, struct HdfSBuf *data)
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
            RegisterManagerNotifyCallback(slotId, serviceCallbackInd);
            break;
        }
        case HRIL_ADAPTER_RADIO_RESPONSE: {
            HdfRemoteService *serviceCallback = HdfSBufReadRemoteService(data);
            if (serviceCallback == nullptr) {
                TELEPHONY_LOGE("miss callback parameter");
                return HDF_ERR_INVALID_PARAM;
            }
            RegisterManagerResponseCallback(slotId, serviceCallback);
            break;
        }
        default:
            DispatchModule(slotId, cmd, data);
    }
    return HRIL_ERR_SUCCESS;
}

void HRilRegOps(const HRilOps *hrilOps)
{
    int i;
    if (hrilOps == nullptr) {
        TELEPHONY_LOGE("HRilRegOps: HRilRegOps * nullptr");
        return;
    }
    if (rilRegisterStatus > RIL_REGISTER_IS_NONE) {
        TELEPHONY_LOGE("HRilRegOps is running!!!!");
        return;
    }
    rilRegisterStatus = RIL_REGISTER_IS_RUNNING;
    (void)memcpy_s(&g_callBacks, sizeof(HRilOps), hrilOps, sizeof(HRilOps));

    for (i = HRIL_SIM_SLOT_0; i < HRIL_SIM_SLOT_NUM; i++) {
        g_manager[i] = std::make_unique<HRilManager>();
        if (g_callBacks.smsOps != nullptr) {
            g_manager[i]->RegisterSmsFuncs(g_callBacks.smsOps);
        }
        if (g_callBacks.callOps != nullptr) {
            g_manager[i]->RegisterCallFuncs(g_callBacks.callOps);
        }
        if (g_callBacks.dataOps != nullptr) {
            g_manager[i]->RegisterDataFuncs(g_callBacks.dataOps);
        }
        if (g_callBacks.modemOps != nullptr) {
            g_manager[i]->RegisterModemFuncs(g_callBacks.modemOps);
        }
        if (g_callBacks.networkOps != nullptr) {
            g_manager[i]->RegisterNetworkFuncs(g_callBacks.networkOps);
        }
        if (g_callBacks.simOps != nullptr) {
            g_manager[i]->RegisterSimFuncs(g_callBacks.simOps);
        }
    }
}

void OnCallReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    auto itFunc = g_manager.find(slotId);
    if (itFunc != g_manager.end()) {
        auto &manager = itFunc->second;
        if (manager != nullptr) {
            manager->OnCallReport(slotId, &reportInfo, response, responseLen);
        } else {
            TELEPHONY_LOGE("Manager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &manager);
        }
    } else {
        TELEPHONY_LOGE("Can not find slotId in g_manager: %{public}d!", slotId);
    }
}

void OnDataReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    auto itFunc = g_manager.find(slotId);
    if (itFunc != g_manager.end()) {
        auto &manager = itFunc->second;
        if (manager != nullptr) {
            manager->OnDataReport(slotId, &reportInfo, response, responseLen);
        } else {
            TELEPHONY_LOGE("Manager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &manager);
        }
    } else {
        TELEPHONY_LOGE("Can not find slotId in g_manager: %{public}d!", slotId);
    }
}

void OnModemReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    auto itFunc = g_manager.find(slotId);
    if (itFunc != g_manager.end()) {
        auto &manager = itFunc->second;
        if (manager != nullptr) {
            manager->OnModemReport(slotId, &reportInfo, response, responseLen);
        } else {
            TELEPHONY_LOGE("Manager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &manager);
        }
    } else {
        TELEPHONY_LOGE("Can not find slotId in g_manager: %{public}d!", slotId);
    }
}

void OnNetworkReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    auto itFunc = g_manager.find(slotId);
    if (itFunc != g_manager.end()) {
        auto &manager = itFunc->second;
        if (manager != nullptr) {
            manager->OnNetworkReport(slotId, &reportInfo, response, responseLen);
        } else {
            TELEPHONY_LOGE("Manager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &manager);
        }
    } else {
        TELEPHONY_LOGE("Can not find slotId in g_manager: %{public}d!", slotId);
    }
}

void OnSimReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    auto itFunc = g_manager.find(slotId);
    if (itFunc != g_manager.end()) {
        auto &manager = itFunc->second;
        if (manager != nullptr) {
            manager->OnSimReport(slotId, &reportInfo, response, responseLen);
        } else {
            TELEPHONY_LOGE("Manager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &manager);
        }
    } else {
        TELEPHONY_LOGE("Can not find slotId in g_manager: %{public}d!", slotId);
    }
}

void OnSmsReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen)
{
    auto itFunc = g_manager.find(slotId);
    if (itFunc != g_manager.end()) {
        auto &manager = itFunc->second;
        if (manager != nullptr) {
            manager->OnSmsReport(slotId, &reportInfo, response, responseLen);
        } else {
            TELEPHONY_LOGE("Manager is nullptr, id:%{public}d, addr:%{public}p!", slotId, &manager);
        }
    } else {
        TELEPHONY_LOGE("Can not find slotId in g_manager: %{public}d!", slotId);
    }
}
#ifdef __cplusplus
}
#endif
} // namespace Telephony
} // namespace OHOS