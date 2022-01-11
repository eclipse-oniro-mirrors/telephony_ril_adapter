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

namespace OHOS {
namespace Telephony {
static void *g_lastNITZTimeData = nullptr;
static size_t g_lastNITZTimeDataSize;

typedef enum WakeMonitorType { DONT_WAKE, WAKE } WakeMonitorType;
typedef struct {
    int32_t request;
    int32_t (*respFunc)(
        int32_t slotId, int32_t responseType, HRilErrNumber e, const uint8_t *response, size_t responseLen);
    WakeMonitorType wakeType;
} UnsolResponseInfo;

typedef enum : int32_t {
    RIL_REGISTER_IS_NONE = 0,
    RIL_REGISTER_IS_RUNNING,
} RegisterState;

static int32_t rilRegisterStatus = 0;

static HRilOps g_callBacks = {
    0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static pthread_rwlock_t g_radioServiceRwLock = PTHREAD_RWLOCK_INITIALIZER;

pthread_rwlock_t *GetRadioServiceLock();

class HRilManager : public IHRilReporter, public std::enable_shared_from_this<HRilManager> {
public:
    HRilManager();
    virtual ~HRilManager();

    virtual int32_t ReportToParent(int32_t requestNum, const HdfSBuf *dataSbuf) override;
    virtual int32_t NotifyToParent(int32_t requestNum, const HdfSBuf *dataSbuf) override;
    virtual ReqDataInfo *CreateHRilRequest(int32_t serial, int32_t slotId, int32_t request) override;
    virtual void ReleaseHRilRequest(int32_t request, ReqDataInfo *requestInfo) override;

    void RegisterCallFuncs(const HRilCallReq *callFuncs);
    void RegisterDataFuncs(const HRilDataReq *dataFuncs);
    void RegisterModemFuncs(const HRilModemReq *modemFuncs);
    void RegisterNetworkFuncs(const HRilNetworkReq *networkFuncs);
    void RegisterSimFuncs(const HRilSimReq *simFuncs);
    void RegisterSmsFuncs(const HRilSmsReq *smsFuncs);

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
    std::unique_ptr<HRilCall> hrilCall_;
    std::unique_ptr<HRilModem> hrilModem_;
    std::unique_ptr<HRilNetwork> hrilNetwork_;
    std::unique_ptr<HRilSim> hrilSim_;
    std::unique_ptr<HRilSms> hrilSms_;
    std::unique_ptr<HRilData> hrilData_;

    const struct HdfRemoteService *serviceCallback_ = nullptr;
    const struct HdfRemoteService *serviceCallbackNotify_ = nullptr;
    std::unordered_map<int32_t, std::list<ReqDataInfo *>> requestList_;
    std::mutex requestListLock_;
};

#ifdef __cplusplus
extern "C" {
#endif

int32_t DispatchRequest(int32_t slotId, int32_t cmd, struct HdfSBuf *data);
void HRilRegOps(const HRilOps *hrilOps);
void OnCallReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnDataReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnModemReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnNetworkReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnSimReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
void OnSmsReport(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *response, size_t responseLen);
int32_t IsLoadedVendorLib(void);

#ifdef __cplusplus
}
#endif
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_IMPL_H