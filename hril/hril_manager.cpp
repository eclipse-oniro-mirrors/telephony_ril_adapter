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

namespace OHOS {
namespace Telephony {
using namespace std;

std::unique_ptr<HRilManager> g_manager = std::make_unique<HRilManager>();

pthread_rwlock_t *GetRadioServiceLock()
{
    return &g_radioServiceRwLock;
}

const char *GetRequestStringInfo(int32_t request)
{
    switch (request) {
        case HREQ_SIM_GET_SIM_STATUS:
            return "SIM_GET_SIM_STATUS";
        default:
            return "<unknown request>";
    }
}

void DispatchModule(int32_t slotId, int32_t cmd, const struct HdfSBuf *data)
{
    struct HdfSBuf *dataInfo = const_cast<struct HdfSBuf *>(data);
    int32_t ret = g_manager->Dispatch(slotId, cmd, dataInfo);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("HRilManager::Dispatch is failed!");
    }
}

void RegisterManagerNotifyCallback(HdfRemoteService *serviceCallbackInd)
{
    g_manager->RegisterModulesNotifyCallback(serviceCallbackInd);
}

void RegisterManagerResponseCallback(HdfRemoteService *serviceCallbackInd)
{
    g_manager->RegisterModulesResponseCallback(serviceCallbackInd);
}

extern "C" int32_t DispatchRequest(int32_t slotId, int32_t cmd, struct HdfSBuf *data)

{
    if (data == nullptr) {
        return HDF_ERR_INVALID_PARAM;
    }
    switch (cmd) {
        case HRIL_ADAPTER_RADIO_INDICATION: {
            HdfRemoteService *serviceCallbackInd = nullptr;
            serviceCallbackInd = HdfSBufReadRemoteService(data);
            if (serviceCallbackInd == nullptr) {
                TELEPHONY_LOGE("miss callback parameter");
                return HDF_ERR_INVALID_PARAM;
            }
            RegisterManagerNotifyCallback(serviceCallbackInd);
            break;
        }
        case HRIL_ADAPTER_RADIO_RESPONSE: {
            HdfRemoteService *serviceCallback = nullptr;
            serviceCallback = HdfSBufReadRemoteService(data);
            if (serviceCallback == nullptr) {
                TELEPHONY_LOGE("miss callback parameter");
                return HDF_ERR_INVALID_PARAM;
            }
            RegisterManagerResponseCallback(serviceCallback);
            break;
        }
        default:
            DispatchModule(slotId, cmd, data);
    }
    return HDF_SUCCESS;
}

ReqDataInfo *CreateHRilRequest(int32_t serial, int32_t slotId, int32_t request)
{
    ReqDataInfo *requestInfo = nullptr;
    HRilSimId socketId = (HRilSimId)slotId;
    requestInfo = (ReqDataInfo *)calloc(1, sizeof(ReqDataInfo));
    if (requestInfo == nullptr) {
        return nullptr;
    }
    requestInfo->simId = socketId;
    requestInfo->request = request;
    requestInfo->serial = serial;
    return requestInfo;
}

extern "C" void HRilRegOps(const HRilOps *hrilOps)
{
    if (hrilOps == nullptr) {
        TELEPHONY_LOGE("HRilRegOps: HRilRegOps * nullptr");
        return;
    }
    if (rilRegisterStatus > RIL_REGISTER_IS_NONE) {
        TELEPHONY_LOGE("HRilRegOps is running!!!!");
        return;
    }
    (void)memcpy_s(&g_callBacks, sizeof(HRilOps), hrilOps, sizeof(HRilOps));
    rilRegisterStatus = RIL_REGISTER_IS_RUNNING;
    if (g_callBacks.smsOps != nullptr) {
        g_manager->RegisterSmsFuncs(g_callBacks.smsOps);
    }
    if (g_callBacks.callOps != nullptr) {
        g_manager->RegisterCallFuncs(g_callBacks.callOps);
    }
    if (g_callBacks.dataOps != nullptr) {
        g_manager->RegisterDataFuncs(g_callBacks.dataOps);
    }
    if (g_callBacks.modemOps != nullptr) {
        g_manager->RegisterModemFuncs(g_callBacks.modemOps);
    }
    if (g_callBacks.networkOps != nullptr) {
        g_manager->RegisterNetworkFuncs(g_callBacks.networkOps);
    }
    if (g_callBacks.simOps != nullptr) {
        g_manager->RegisterSimFuncs(g_callBacks.simOps);
    }
}

extern "C" void OnCallReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    g_manager->OnCallReport(&reportInfo, response, responseLen);
}

extern "C" void OnDataReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    g_manager->OnDataReport(&reportInfo, response, responseLen);
}

extern "C" void OnModemReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    g_manager->OnModemReport(&reportInfo, response, responseLen);
}

extern "C" void OnNetworkReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    g_manager->OnNetworkReport(&reportInfo, response, responseLen);
}

extern "C" void OnSimReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    g_manager->OnSimReport(&reportInfo, response, responseLen);
}

extern "C" void OnSmsReport(struct ReportInfo reportInfo, const void *response, size_t responseLen)
{
    g_manager->OnSmsReport(&reportInfo, response, responseLen);
}

int32_t HRilManager::Dispatch(int32_t slotId, int32_t code, struct HdfSBuf *data)
{
    if (hrilCall_ != nullptr && hrilCall_->IsCallRespOrNotify(code)) {
        hrilCall_->ProcessCallRequest(slotId, code, data);
        return HDF_SUCCESS;
    }
    if (hrilSms_ != nullptr && hrilSms_->IsSmsRespOrNotify(code)) {
        hrilSms_->ProcessSmsRequest(slotId, code, data);
        return HDF_SUCCESS;
    }
    if (hrilSim_ != nullptr && hrilSim_->IsSimRespOrNotify(code)) {
        hrilSim_->ProcessSimRequest(slotId, code, data);
        return HDF_SUCCESS;
    }
    if (hrilNetwork_ != nullptr && hrilNetwork_->IsNetworkRespOrNotify(code)) {
        hrilNetwork_->ProcessNetworkRequest(slotId, code, data);
        return HDF_SUCCESS;
    }
    if (hrilModem_ != nullptr && hrilModem_->IsModemRespOrNotify(code)) {
        hrilModem_->ProcessCommonRequest(slotId, code, data);
        return HDF_SUCCESS;
    }
    if (hrilData_ != nullptr && hrilData_->IsDataRespOrNotify(code)) {
        hrilData_->ProcessDataRequest(slotId, code, data);
        return HDF_SUCCESS;
    }
    return HDF_FAILURE;
}

void HRilManager::RegisterModulesNotifyCallback(HdfRemoteService *serviceCallbackInd)
{
    if (hrilCall_ != nullptr) {
        hrilCall_->RegisterNotifyCallback(serviceCallbackInd);
    }
    if (hrilSms_ != nullptr) {
        hrilSms_->RegisterNotifyCallback(serviceCallbackInd);
    }
    if (hrilSim_ != nullptr) {
        hrilSim_->RegisterNotifyCallback(serviceCallbackInd);
    }
    if (hrilNetwork_ != nullptr) {
        hrilNetwork_->RegisterNotifyCallback(serviceCallbackInd);
    }
    if (hrilModem_ != nullptr) {
        hrilModem_->RegisterNotifyCallback(serviceCallbackInd);
    }
    if (hrilData_ != nullptr) {
        hrilData_->RegisterNotifyCallback(serviceCallbackInd);
    }
}

void HRilManager::RegisterModulesResponseCallback(HdfRemoteService *serviceCallback)
{
    if (hrilCall_ != nullptr) {
        hrilCall_->RegisterResponseCallback(serviceCallback);
    }
    if (hrilSms_ != nullptr) {
        hrilSms_->RegisterResponseCallback(serviceCallback);
    }
    if (hrilSim_ != nullptr) {
        hrilSim_->RegisterResponseCallback(serviceCallback);
    }
    if (hrilNetwork_ != nullptr) {
        hrilNetwork_->RegisterResponseCallback(serviceCallback);
    }
    if (hrilModem_ != nullptr) {
        hrilModem_->RegisterResponseCallback(serviceCallback);
    }
    if (hrilData_ != nullptr) {
        hrilData_->RegisterResponseCallback(serviceCallback);
    }
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

void HRilManager::OnCallReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnCallReport reportInfo is null!!!");
        return;
    }
    switch (reportInfo->type) {
        case (int32_t)ReportType::HRIL_RESPONSE: {
            ReqDataInfo *reqInfo = nullptr;
            HRilSimId simId = HRIL_SIM_1;
            reqInfo = (ReqDataInfo *)reportInfo->requestInfo;
            simId = reqInfo->simId;
            HRilRadioResponseInfo responseInfo = {};
            responseInfo.serial = reqInfo->serial;
            responseInfo.error = (HRilErrType)reportInfo->error;
            hrilCall_->ProcessCallResponse(simId, reqInfo->request, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilCall_->ProcessCallNotify(
                reportInfo->notifyId, HRIL_NOTIFICATION_TYPE, reportInfo->error, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnDataReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnDataReport reportInfo is null!!!");
        return;
    }
    switch (reportInfo->type) {
        case (int32_t)ReportType::HRIL_RESPONSE: {
            ReqDataInfo *reqInfo = nullptr;
            HRilSimId simId = HRIL_SIM_1;
            reqInfo = (ReqDataInfo *)reportInfo->requestInfo;
            simId = reqInfo->simId;
            HRilRadioResponseInfo responseInfo = {};
            responseInfo.serial = reqInfo->serial;
            responseInfo.error = (HRilErrType)reportInfo->error;
            hrilData_->ProcessDataResponse(simId, reqInfo->request, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilData_->ProcessDataNotify(
                reportInfo->notifyId, HRIL_NOTIFICATION_TYPE, reportInfo->error, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnModemReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnModemReport reportInfo is null!!!");
        return;
    }
    switch (reportInfo->type) {
        case (int32_t)ReportType::HRIL_RESPONSE: {
            ReqDataInfo *reqInfo = nullptr;
            HRilSimId simId = HRIL_SIM_1;
            reqInfo = (ReqDataInfo *)reportInfo->requestInfo;
            simId = reqInfo->simId;
            HRilRadioResponseInfo responseInfo = {};
            responseInfo.serial = reqInfo->serial;
            responseInfo.error = (HRilErrType)reportInfo->error;
            hrilModem_->ProcessModemResponse(simId, reqInfo->request, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilModem_->ProcessModemNotify(
                reportInfo->notifyId, HRIL_NOTIFICATION_TYPE, reportInfo->error, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnNetworkReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnNetworkReport reportInfo is null!!!");
        return;
    }
    switch (reportInfo->type) {
        case (int32_t)ReportType::HRIL_RESPONSE: {
            ReqDataInfo *reqInfo = nullptr;
            HRilSimId simId = HRIL_SIM_1;
            reqInfo = (ReqDataInfo *)reportInfo->requestInfo;
            simId = reqInfo->simId;
            HRilRadioResponseInfo responseInfo = {};
            responseInfo.serial = reqInfo->serial;
            responseInfo.error = (HRilErrType)reportInfo->error;
            hrilNetwork_->ProcessNetworkResponse(simId, reqInfo->request, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilNetwork_->ProcessNetworkNotify(
                reportInfo->notifyId, HRIL_NOTIFICATION_TYPE, reportInfo->error, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnSimReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnSimReport reportInfo is null!!!");
        return;
    }
    switch (reportInfo->type) {
        case (int32_t)ReportType::HRIL_RESPONSE: {
            ReqDataInfo *reqInfo = nullptr;
            HRilSimId simId = HRIL_SIM_1;
            reqInfo = (ReqDataInfo *)reportInfo->requestInfo;
            simId = reqInfo->simId;
            HRilRadioResponseInfo responseInfo = {};
            responseInfo.serial = reqInfo->serial;
            responseInfo.error = (HRilErrType)reportInfo->error;
            hrilSim_->ProcessSimResponse(simId, reqInfo->request, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilSim_->ProcessSimNotify(
                reportInfo->notifyId, HRIL_NOTIFICATION_TYPE, reportInfo->error, response, responseLen);
            break;
        }
        default:
            break;
    }
}

void HRilManager::OnSmsReport(ReportInfo *reportInfo, const void *response, size_t responseLen)
{
    if (reportInfo == nullptr) {
        TELEPHONY_LOGE("OnSmsReport reportInfo is null!!!");
        return;
    }
    switch (reportInfo->type) {
        case (int32_t)ReportType::HRIL_RESPONSE: {
            ReqDataInfo *reqInfo = nullptr;
            HRilSimId simId = HRIL_SIM_1;
            reqInfo = (ReqDataInfo *)reportInfo->requestInfo;
            simId = reqInfo->simId;
            HRilRadioResponseInfo responseInfo = {};
            responseInfo.serial = reqInfo->serial;
            responseInfo.error = (HRilErrType)reportInfo->error;
            hrilSms_->ProcessSmsResponse(simId, reqInfo->request, responseInfo, response, responseLen);
            break;
        }
        case (int32_t)ReportType::HRIL_NOTIFICATION: {
            hrilSms_->ProcessSmsNotify(
                reportInfo->notifyId, HRIL_NOTIFICATION_TYPE, reportInfo->error, response, responseLen);
            break;
        }
        default:
            break;
    }
}

HRilManager::HRilManager()
{
    hrilCall_ = std::make_unique<HRilCall>();
    hrilModem_ = std::make_unique<HRilModem>();
    hrilNetwork_ = std::make_unique<HRilNetwork>();
    hrilSim_ = std::make_unique<HRilSim>();
    hrilSms_ = std::make_unique<HRilSms>();
    hrilData_ = std::make_unique<HRilData>();
}

HRilManager::~HRilManager() {}
} // namespace Telephony
} // namespace OHOS
