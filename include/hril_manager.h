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
typedef struct RespCmdInfo RespCmdInfo;

typedef enum WakeMonitorType { DONT_WAKE, WAKE } WakeMonitorType;
typedef struct {
    int32_t request;
    int32_t (*respFunc)(
        int32_t slotId, int32_t responseType, HRilErrno e, const void *response, size_t responseLen);
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

class HRilManager {
public:
    HRilManager();
    virtual ~HRilManager();

    void RegisterCallFuncs(const HRilCallReq *callFuncs);
    void RegisterDataFuncs(const HRilDataReq *dataFuncs);
    void RegisterModemFuncs(const HRilModemReq *modemFuncs);
    void RegisterNetworkFuncs(const HRilNetworkReq *networkFuncs);
    void RegisterSimFuncs(const HRilSimReq *simFuncs);
    void RegisterSmsFuncs(const HRilSmsReq *smsFuncs);

    int32_t Dispatch(int32_t slotId, int32_t cmd, struct HdfSBuf *data);

    void RegisterModulesNotifyCallback(HdfRemoteService *serviceCallbackInd);

    void RegisterModulesResponseCallback(HdfRemoteService *serviceCallbackInd);

    void OnCallReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen);
    void OnDataReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen);
    void OnModemReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen);
    void OnNetworkReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen);
    void OnSimReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen);
    void OnSmsReport(struct ReportInfo *reportInfo, const void *response, size_t responseLen);

private:
    std::unique_ptr<HRilCall> hrilCall_;
    std::unique_ptr<HRilModem> hrilModem_;
    std::unique_ptr<HRilNetwork> hrilNetwork_;
    std::unique_ptr<HRilSim> hrilSim_;
    std::unique_ptr<HRilSms> hrilSms_;
    std::unique_ptr<HRilData> hrilData_;
};

extern "C" int32_t DispatchRequest(int32_t slotId, int32_t cmd, struct HdfSBuf *data);
extern "C" void HRilRegOps(const HRilOps *hrilOps);
extern "C" void OnCallReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern "C" void OnDataReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern "C" void OnModemReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern "C" void OnNetworkReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern "C" void OnSimReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
extern "C" void OnSmsReport(struct ReportInfo reportInfo, const void *response, size_t responseLen);
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_MANAGER_H