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

/**
 * @brief Defines the SIM data request information.
 *
 * Form 3GPP TS 27.007 V4.3.0 (2001-12) 8.18, + CRSM
 */
typedef struct {
    /**
     * Command passed on by the MT to the SIM; refer 3GPP TS 51.011 [28]
     * 176 READ BINARY
     * 178 READ RECORD
     * 192 GET RESPONSE
     * 214 UPDATE BINARY
     * 220 UPDATE RECORD
     * 242 STATUS
     * 203 RETRIEVE DATA
     * 219 SET DATA
     */
    int32_t command;
    /**
     * This is the identifier of an elementary datafile on SIM.
     * Mandatory for every command except STATUS.
     */
    int32_t fileid;
    /**
     * Parameters passed on by the MT to the SIM. These parameters are mandatory for every command,
     * except GET RESPONSE and STATUS. The values are described in 3GPP TS 51.011 [28].
     */
    int32_t p1;
    /**
     * Parameters passed on by the MT to the SIM. These parameters are mandatory for every command,
     * except GET RESPONSE and STATUS. The values are described in 3GPP TS 51.011 [28].
     */
    int32_t p2;
    /**
     * Parameters passed on by the MT to the SIM. These parameters are mandatory for every command,
     * except GET RESPONSE and STATUS. The values are described in 3GPP TS 51.011 [28].
     */
    int32_t p3;
    /**
     *  Information which shall be written to the SIM (hexadecimal character format; refer +CSCS).
     */
    char *data;
    /**
     * Contains the path of an elementary file on the SIM/UICC in hexadecimal format
     * as defined in ETSI TS 102 221 [60] (e.g. "7F205F70" in SIM and UICC case).
     * The <pathid> shall only be used in the mode "select by path from MF"
     * as defined in ETSI TS 102 221 [60].
     */
    char *pathid;
    /**
     * PIN2
     */
    char *pin2;
} HRilSimIO;

/**
 * @brief Defines the response to the SIM data request.
 *
 * Form TS 27.007.8.18 +CRSM
 */
typedef struct {
    /**
     * Information from the SIM about the execution of the actual command.
     * These parameters are delivered to the TE in both cases,
     * on successful or failed execution of the command.
     */
    int32_t sw1;
    /**
     * Information from the SIM about the execution of the actual command.
     * These parameters are delivered to the TE in both cases,
     * on successful or failed execution of the command.
     */
    int32_t sw2;
    /**
     * Response of a successful completion of the command previously issued
     * (hexadecimal character format; refer +CSCS). STATUS and GET RESPONSE return data,
     * which gives information about the current elementary datafield.This information
     * includes the type of file and its size (refer 3GPP TS 51.011 [28]). After READ BINARY,
     * READ RECORD or RETRIEVE DATA command the requested data will be returned.
     * <response> is not returned after a successful UPDATE BINARY,
     * UPDATE RECORD or SET DATA command
     */
    char *response;
} HRilSimIOResponse;

/**
 * @brief Defines the response to the request for enabling the logical channel of the APDU.
 */
typedef struct {
    /**
     * Information from the SIM about the execution of the actual command.
     * These parameters are delivered to the TE in both cases,
     * on successful or failed execution of the command.
     */
    int32_t sw1;
    /**
     * Information from the SIM about the execution of the actual command.
     * These parameters are delivered to the TE in both cases,
     * on successful or failed execution of the command.
     */
    int32_t sw2;
    /**
     * ID of the opened logical channel
     */
    int32_t channelId;
    /**
     * Response information
     */
    char *response;
} HRilOpenLogicalChannelResponse;

/**
 * @brief Defines the SIM card status information.
 */
typedef struct {
    /**
     * SIM card index
     */
    int32_t index;
    /**
     * SIM card type:
     */
    HRilSimType simType;
    /**
     * SIM card status:
     */
    HRilSimState simState;
} HRilCardState;

/**
 * @brief Defines the lock status information.
 */
typedef struct {
    /**
     * Query result. For details, see {@link RilErrType}.
     */
    int32_t result;
    /**
     * Number of remaining attempts
     */
    int32_t remain;
} HRilLockStatus;

/**
 * @brief Defines the SIM card lock information.
 *
 * From 3GPP TS 27.007 7.4
 */
typedef struct {
    /**
     * ("SC","AO","OI","OX","AI","IR","AB","AG","AC","FD","PN","PU","PP")
     */
    char *fac;
    /**
     * 0 unlock
     * 1 lock
     * 2 query status
     */
    int32_t mode;
    /**
     * 0 not active
     * 1 active
     */
    int32_t status;
    /**
     * Shall be the same as password specified for the facility
     * from the MT user interface or with command Change Password +CPWD
     */
    char *passwd;
    /**
     * Is a sum of integers each representing a class of information (default 7 - voice, data and
     * fax): 1 voice (telephony) 2 data (refers to all bearer services; with <mode>=2 this may refer
     * only to some bearer service if TA does not support values 16, 32, 64 and 128) 4 fax (facsimile
     * services) 8 short message service 16 data circuit sync 32 data circuit async 64
     * dedicated packet access 128 dedicated PAD access
     */
    int32_t classx;
} HRilSimClock;

/**
 * @brief Defines the SIM card password information.
 *
 * From 3GPP TS 27.007 7.5
 */
typedef struct {
    /**
     * ("SC","AO","OI","OX","AI","IR","AB","AG","AC","FD","PN","PU","PP")
     */
    char *fac;
    /**
     *  <oldPassword> shall be the same as password specified for the facility from the MT
     * user interface or with command Change Password +CPWD and <newPassword> is the new
     * password
     */
    char *oldPassword;
    char *newPassword;
    /**
     * Maximum length of the password for the facility
     */
    int32_t passwordLength;
} HRilSimPassword;

/**
 * @brief Defines the maximum number of SIM password attempts.
 */
typedef struct {
    char *code;
    /**
     * Number of remaining password attempts
     */
    int32_t times;
    /**
     * Number of remaining PUK attempts
     */
    int32_t pukTimes;
    /**
     * Number of remaining PIN attempts
     */
    int32_t pinTimes;
    /**
     * Number of remaining PUK2 attempts
     */
    int32_t puk2Times;
    /**
     * Number of remaining PIN2 attempts
     */
    int32_t pin2Times;
} HRilPinInputTimes;

/**
 * @brief Enumerates radio protocol phases.
 */
typedef struct {
    /**
     * Session ID
     */
    int32_t sessionId;
    /**
     * Radio protocol parameters.
     */
    HRilRadioProtocolPhase phase;
    /**
     * Radio protocol technology:
     */
    int32_t technology;
    /**
     * Modem ID, corresponding to slotId at the bottom layer
     */
    int32_t modemId;
    /**
     * Radio protocol status.
     */
    HRilRadioProtocolStatus status;
} HRilRadioProtocol;

/**
 * @brief Defines the APDU data transmission request information.
 */
typedef struct {
    /**
     * Channel ID
     */
    int32_t channelId;
    /**
     * APDU instruction type. For details, see ETSI 102 221 [55].
     */
    int32_t type;
    /**
     * APDU instruction. For details, see ETSI 102 221 [55].
     */
    int32_t instruction;
    /**
     * Command parameter 1 of the SIM data request. For details, see 3GPP TS 51.011[28].
     */
    int32_t p1;
    /**
     * Command parameter 2 of the SIM data request. For details, see 3GPP TS 51.011[28].
     */
    int32_t p2;
    /**
     * Command parameter 3 of the SIM data request. For details, see 3GPP TS 51.011[28].
     * If p3 is a negative value, a 4-byte APDU is sent to the SIM card.
     */
    int32_t p3;
    /**
     * Data to be transmitted
     */
    char *data;
} HRilApduSimIO;

/**
 * @brief Defines the SIM card authentication request information.
 */
typedef struct {
    /**
     * Request SN
     */
    int32_t serial;
    /**
     * App ID
     */
    char *aid;
    /**
     * Authentication data
     */
    char *data;
} HRilSimAuthenticationRequestInfo;

/**
 * @brief Defines ncfg request information.
 */
typedef struct {
    /**
     * Operator Name Matched with SIM card
     */
    char *operName;
    /**
     * Operator Key Matched with SIM card
     */
    char *operKey;
    /**
     * Current SIM State
     */
    int state;
    /**
     * Reserved Field
     */
    char *reserve;
} HRilNcfgOperatorInfo;

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
    void (*SimStkSendEnvelopeResponseContainStatus)(const ReqDataInfo *requestInfo, const char *strCmd);
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
    void (*SendSimMatchedOperatorInfo)(const ReqDataInfo *requestInfo, HRilNcfgOperatorInfo *data, size_t dataLen);
    /**
     * @brief Set SIM card power state.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param state The power state of SIM card to set, value as following:
     * - 0 indicates CardPowerState#CARD_POWER_DOWN
     * - 1 indicates CardPowerState#CARD_POWER_UP
     * - 2 indicates CardPowerState#CARD_POWER_UP_PASS_THROUGH
     * @see ReqDataInfo
     */
    void (*SetSimPowerState)(const ReqDataInfo *requestInfo, int32_t state);
} HRilSimReq;
#ifdef __cplusplus
}
#endif
#endif // OHOS_RIL_VENDOR_SIM_DEFS_H
