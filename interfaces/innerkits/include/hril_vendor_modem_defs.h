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

#ifndef OHOS_RIL_VENDOR_MODEM_DEFS_H
#define OHOS_RIL_VENDOR_MODEM_DEFS_H

#include "hril_public_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumerates RIL system service states.
 */
typedef struct {
    /**
     * System service status
     */
    HRilSrvStatus srvStatus;
    /**
     * System service domain
     */
    HRilSrvDomain srvDomain;
    /**
     * Roaming status
     */
    HRilRoamStatus roamStatus;
    /**
     * SIM card status
     */
    HRilSimStatus simStatus;
    /**
     * SIM card lock status
     */
    HRilSimLockStatus lockStatus;
    /**
     * System mode
     */
    HRilSysMode sysMode;
    /**
     * String corresponding to the system mode
     */
    char *sysModeName;
    /**
     * Radio access technology type. For details, see {@link RilRadioTech}.
     */
    HRilRadioTech actType;
    /**
     * String corresponding to the radio access technology type
     */
    char *actName;
} HRilVoiceRadioInfo;

typedef struct {
    void (*SetRadioState)(const ReqDataInfo *requestInfo, int32_t fun, int32_t rst);
    void (*GetRadioState)(const ReqDataInfo *requestInfo);
    void (*GetImei)(const ReqDataInfo *requestInfo);
    void (*GetImeiSv)(const ReqDataInfo *requestInfo);
    void (*GetMeid)(const ReqDataInfo *requestInfo);
    void (*GetVoiceRadioTechnology)(const ReqDataInfo *requestInfo);
    void (*GetBasebandVersion)(const ReqDataInfo *requestInfo);
    void (*ShutDown)(const ReqDataInfo *requestInfo);
} HRilModemReq;
#ifdef __cplusplus
}
#endif
#endif // OHOS_RIL_VENDOR_MODEM_DEFS_H
