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

#ifndef OHOS_RIL_H
#define OHOS_RIL_H

#include <sys/time.h>

#include "hril_vendor_call_defs.h"
#include "hril_vendor_data_defs.h"
#include "hril_vendor_modem_defs.h"
#include "hril_vendor_network_defs.h"
#include "hril_vendor_sim_defs.h"
#include "hril_vendor_sms_defs.h"

#define MAX_CMD_LENGTH 500

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    HRilLastCallErrorCode errorCode; /* Execution command causes the TA to return one or more lines of information
                                      * text <report>, determined by the ME manufacturer, which should offer
                                      * the user of the TA an extended report of the reason for
                                      * the failure in the last unsuccessful call setup (originating or answering) or
                                      * in call modification;
                                      * the last call release;
                                      * the last unsuccessful GPRS attach or unsuccessful PDP context activation;
                                      * the last GPRS detach or PDP context deactivation. */
    char *vendorError; /* Error codes of vendor. */
} HRilLastCallErrorCodeInfo;

typedef void (*HRilCallbackFun)(uint8_t *param);

struct HRilReport {
    void (*OnCallReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnDataReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnModemReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnNetworkReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnSimReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnSmsReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnTimerCallback)(HRilCallbackFun func, uint8_t *param, const struct timeval *tv);
};

typedef struct {
    int32_t version;
    const HRilCallReq *callOps;
    const HRilSimReq *simOps;
    const HRilSmsReq *smsOps;
    const HRilDataReq *dataOps;
    const HRilNetworkReq *networkOps;
    const HRilModemReq *modemOps;
} HRilOps;

const HRilOps *RilInitOps(const struct HRilReport *reportOps);
#ifdef __cplusplus
}
#endif
#endif // OHOS_HRIL_H