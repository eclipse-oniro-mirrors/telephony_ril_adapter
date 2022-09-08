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

#ifndef OHOS_RIL_VENDOR_SIM_DEFS_H
#define OHOS_RIL_VENDOR_SIM_DEFS_H

#include <stddef.h>

#include "hril_public_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Form 3GPP TS 27.007 V4.3.0 (2001-12) 8.18, + CRSM */
typedef struct {
    int32_t command; /* command passed on by the MT to the SIM; refer 3GPP TS 51.011 [28]
                      * 176 READ BINARY
                      * 178 READ RECORD
                      * 192 GET RESPONSE
                      * 214 UPDATE BINARY
                      * 220 UPDATE RECORD
                      * 242 STATUS
                      * 203 RETRIEVE DATA
                      * 219 SET DATA
                      */
    int32_t fileid; /* this is the identifier of an elementary datafile on SIM.
                     * Mandatory for every command except STATUS. */
    int32_t p1; /* parameters passed on by the MT to the SIM. These parameters are mandatory for every command,
                 * except GET RESPONSE and STATUS. The values are described in 3GPP TS 51.011 [28]. */
    int32_t p2; /* parameters passed on by the MT to the SIM. These parameters are mandatory for every command,
                 * except GET RESPONSE and STATUS. The values are described in 3GPP TS 51.011 [28]. */
    int32_t p3; /* parameters passed on by the MT to the SIM. These parameters are mandatory for every command,
                 * except GET RESPONSE and STATUS. The values are described in 3GPP TS 51.011 [28]. */
    char *data; /* information which shall be written to the SIM (hexadecimal character format; refer +CSCS). */
    char *pathid; /* contains the path of an elementary file on the SIM/UICC in hexadecimal format
                   * as defined in ETSI TS 102 221 [60] (e.g. "7F205F70" in SIM and UICC case).
                   * The <pathid> shall only be used in the mode "select by path from MF"
                   * as defined in ETSI TS 102 221 [60]. */
    char *pin2;
} HRilSimIO;

/* Form TS 27.007.8.18 +CRSM */
typedef struct {
    int32_t sw1; /* information from the SIM about the execution of the actual command.
                  * These parameters are delivered to the TE in both cases,
                  * on successful or failed execution of the command. */
    int32_t sw2; /* information from the SIM about the execution of the actual command.
                  * These parameters are delivered to the TE in both cases,
                  * on successful or failed execution of the command. */
    char *response; /* response of a successful completion of the command previously issued
                     * (hexadecimal character format; refer +CSCS). STATUS and GET RESPONSE return data,
                     * which gives information about the current elementary datafield.This information
                     * includes the type of file and its size (refer 3GPP TS 51.011 [28]). After READ BINARY,
                     * READ RECORD or RETRIEVE DATA command the requested data will be returned.
                     * <response> is not returned after a successful UPDATE BINARY,
                     * UPDATE RECORD or SET DATA command */
} HRilSimIOResponse;

typedef struct {
    int32_t sw1; /* information from the SIM about the execution of the actual command.
                  * These parameters are delivered to the TE in both cases,
                  * on successful or failed execution of the command. */
    int32_t sw2; /* information from the SIM about the execution of the actual command.
                  * These parameters are delivered to the TE in both cases,
                  * on successful or failed execution of the command. */
    int32_t channelId;
    char *response;
} HRilOpenLogicalChannelResponse;

typedef struct {
    int32_t index; /* sim index */
    HRilSimType simType;
    HRilSimState simState;
} HRilCardState;

typedef struct {
    int32_t result;
    int32_t remain;
} HRilLockStatus;

/* From 3GPP TS 27.007 7.4 */
typedef struct {
    char *fac; /* ("SC","AO","OI","OX","AI","IR","AB","AG","AC","FD","PN","PU","PP") */
    int32_t mode; /* 0 unlock
                   * 1 lock
                   * 2 query status */
    int32_t status; /* 0 not active
                     * 1 active */
    char *passwd; /* shall be the same as password specified for the facility
                   * from the MT user interface or with command Change Password +CPWD */
    int32_t classx; /* is a sum of integers each representing a class of information (default 7 - voice, data and
                     * fax): 1 voice (telephony) 2 data (refers to all bearer services; with <mode>=2 this may refer
                     * only to some bearer service if TA does not support values 16, 32, 64 and 128) 4 fax (facsimile
                     * services) 8 short message service 16 data circuit sync 32 data circuit async 64
                     * dedicated packet access 128 dedicated PAD access  */
} HRilSimClock;

/* From 3GPP TS 27.007 7.5 */
typedef struct {
    char *fac; /* ("SC","AO","OI","OX","AI","IR","AB","AG","AC","FD","PN","PU","PP") */
    char *oldPassword; /* <oldPassword> shall be the same as password specified for the facility from the MT
                        * user interface or with command Change Password +CPWD and <newPassword> is the new
                        * password; maximum length of password can be determined with <passwordLength> */
    char *newPassword;
    int32_t passwordLength; /* maximum length of the password for the facility */
} HRilSimPassword;

typedef struct {
    char *code;
    int32_t times;
    int32_t pukTimes;
    int32_t pinTimes;
    int32_t puk2Times;
    int32_t pin2Times;
} HRilPinInputTimes;

typedef struct {
    int32_t sessionId;
    HRilRadioProtocolPhase phase;
    int32_t technology;
    int32_t modemId;
    HRilRadioProtocolStatus status;
} HRilRadioProtocol;

typedef struct {
    int32_t channelId;
    int32_t type;
    int32_t instruction;
    int32_t p1;
    int32_t p2;
    int32_t p3;
    char *data;
} HRilApduSimIO;

typedef struct {
    int32_t serial;
    char *aid;
    char *data;
} HRilSimAuthenticationRequestInfo;

typedef struct {
    void (*GetSimStatus)(const ReqDataInfo *requestInfo);
    void (*GetSimIO)(const ReqDataInfo *requestInfo, const HRilSimIO *data, size_t dataLen);
    void (*GetSimImsi)(const ReqDataInfo *requestInfo);
    void (*GetSimLockStatus)(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
    void (*SetSimLock)(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
    void (*ChangeSimPassword)(const ReqDataInfo *requestInfo, const HRilSimPassword *data, size_t dataLen);
    void (*UnlockPin)(const ReqDataInfo *requestInfo, const char *pin);
    void (*UnlockPuk)(const ReqDataInfo *requestInfo, const char *puk, const char *pin);
    void (*GetSimPinInputTimes)(const ReqDataInfo *requestInfo);
    void (*UnlockPin2)(const ReqDataInfo *requestInfo, const char *pin2);
    void (*UnlockPuk2)(const ReqDataInfo *requestInfo, const char *puk2, const char *pin2);
    void (*GetSimPin2InputTimes)(const ReqDataInfo *requestInfo);
    void (*SetActiveSim)(const ReqDataInfo *requestInfo, int32_t index, int32_t enable);
    void (*SimStkSendTerminalResponse)(const ReqDataInfo *requestInfo, const char *strCmd);
    void (*SimStkSendEnvelope)(const ReqDataInfo *requestInfo, const char *strCmd);
    void (*SimStkSendCallSetupRequestResult)(const ReqDataInfo *requestInfo, int32_t accept);
    void (*SimStkIsReady)(const ReqDataInfo *requestInfo);
    void (*GetRadioProtocol)(const ReqDataInfo *requestInfo);
    void (*SetRadioProtocol)(const ReqDataInfo *requestInfo, const HRilRadioProtocol *data);
    void (*SimOpenLogicalChannel)(const ReqDataInfo *requestInfo, const char *appID, int32_t p2);
    void (*SimCloseLogicalChannel)(const ReqDataInfo *requestInfo, int32_t channelId);
    void (*SimTransmitApduLogicalChannel)(const ReqDataInfo *requestInfo, HRilApduSimIO *data, size_t dataLen);
    void (*SimTransmitApduBasicChannel)(const ReqDataInfo *requestInfo, HRilApduSimIO *data, size_t dataLen);
    void (*SimAuthentication)(const ReqDataInfo *requestInfo, HRilSimAuthenticationRequestInfo *data, size_t dataLen);
    void (*UnlockSimLock)(const ReqDataInfo *requestInfo, int32_t lockType, const char *password);
} HRilSimReq;
#ifdef __cplusplus
}
#endif
#endif // OHOS_RIL_VENDOR_SIM_DEFS_H
