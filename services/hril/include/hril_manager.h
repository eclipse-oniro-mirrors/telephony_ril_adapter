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
    sptr<OHOS::HDI::Power::V1_0::IPowerInterface> powerInterface_ { nullptr };
    std::unique_ptr<HRilTimerCallback> timerCallback_ = nullptr;
    std::unique_ptr<std::thread> eventLoop_ = nullptr;
    static const uint32_t RUNNING_LOCK_DEFAULT_TIMEOUT_S = 60; // 60s
    std::mutex mutexRunningLock_;
    std::atomic_uint runningLockCount_ = 0;
    std::atomic_int runningSerialNum_ = 0;
    int32_t GetMaxSimSlotCount();

    virtual int32_t ReportToParent(int32_t requestNum, const HdfSBuf *dataSbuf) override;
    virtual int32_t NotifyToParent(int32_t requestNum, const HdfSBuf *dataSbuf) override;
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

    int32_t Dispatch(int32_t slotId, int32_t cmd, struct HdfSBuf *data);

    void RegisterModulesNotifyCallback(const HdfRemoteService *serviceCallbackInd);

    void RegisterModulesResponseCallback(const HdfRemoteService *serviceCallbackInd);

    void OnCallReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnDataReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnModemReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnNetworkReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnSimReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);
    void OnSmsReport(int32_t slotId, const ReportInfo *reportInfo, const uint8_t *response, size_t responseLen);

private:
    template<typename T>
    void OnReport(std::vector<std::unique_ptr<T>> &subModules, int32_t slotId, const ReportInfo *reportInfo,
        const uint8_t *response, size_t responseLen);

    const int32_t hrilSimSlotCount_;
    std::vector<std::unique_ptr<HRilCall>> hrilCall_;
    std::vector<std::unique_ptr<HRilModem>> hrilModem_;
    std::vector<std::unique_ptr<HRilNetwork>> hrilNetwork_;
    std::vector<std::unique_ptr<HRilSim>> hrilSim_;
    std::vector<std::unique_ptr<HRilSms>> hrilSms_;
    std::vector<std::unique_ptr<HRilData>> hrilData_;

    const struct HdfRemoteService *serviceCallback_ = nullptr;
    const struct HdfRemoteService *serviceCallbackNotify_ = nullptr;
    std::unordered_map<int32_t, std::list<ReqDataInfo *>> requestList_;
    static std::unordered_map<int32_t, int32_t> notificationMap_;
    std::mutex requestListLock_;
};

#ifdef __cplusplus
extern "C" {
#endif

int32_t GetSimSlotCount(void);
int32_t DispatchRequest(int32_t cmd, struct HdfSBuf *data);
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