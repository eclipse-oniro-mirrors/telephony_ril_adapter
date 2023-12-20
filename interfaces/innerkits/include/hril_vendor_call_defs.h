/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_RIL_VENDOR_CALL_DEFS_H
#define OHOS_RIL_VENDOR_CALL_DEFS_H

#include <stddef.h>

#include "hril_enum.h"
#include "hril_public_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief From 3GPP TS 27.007 V4.3.0 (2001-12)  AT+CGDCONT
 */
typedef struct {
    /**
     * Call identification number as described in 3GPP TS 22.030 [19] sub-clause 6.5.5.1.
     * This number can be used in +CHLD command operations
     */
    int32_t index;

    /**
     * 0: mobile originated (MO) call
     * 1: mobile terminated (MT) call
     */
    int32_t dir;

    /** call state */
    HRilCallState state;

    /** call mode */
    HRilCallMode mode;

    /**
     * 0: call is not one of multiparty (conference) call parties
     * 1: call is one of multiparty (conference) call parties
     */
    int32_t mpty;

    /**
     * Identifies the service domain.
     * 0: CS domain phone
     * 1: IMS domain phone
     */
    int32_t voiceDomain;

    /**
     * 0: voice call
     * 1: Video call: send one-way video, two-way voice
     * 2: Video call: one-way receiving video, two-way voice
     * 3: Video call: two-way video, two-way voice
     */
    int32_t callType;

    /** Phone number in format specified by <type> */
    char *number;

    /**
     * Type of address octet in integer format (refer TS 24.008 [8] subclauses 10.5.4.7);
     * default 145 when dialling string includes international access code character "+",
     * otherwise 129. see 3GPP TS 27.007 V4.3.0 (2001-12) 6.1
     */
    int32_t type;

    /**
     * Alphanumeric representation of <number> corresponding to the entry found
     * in phonebook; used character set should be the one selected with command
     * select TE character set +CSCS
     */
    char *alpha;
} HRilCallInfo;

/**
 * @brief From 3GPP TS 27.007 V4.3.0 (2001-12) ATD%s%s
 */
typedef struct {
    /**
     * Type of address octet in integer format (refer TS 24.008 [8] subclauses 10.5.4.7);
     * default 145 when dialling string includes international access code character "+", otherwise
     * 129.
     */
    char *address;

    /**
     * This command refers to CLIR service according to 3GPP TS 22.081 [3] that allows a calling
     * subscriber to enable or disable the presentation of the CLI to the called party when originating
     * a call. <n> (parameter sets the adjustment for outgoing calls): 0	presentation indicator is used
     * according to the subscription of the CLIR service 1	CLIR invocation 2	CLIR suppression <m>
     * (parameter shows the subscriber CLIR service status in the network): 0	CLIR not provisioned 1
     * CLIR provisioned in permanent mode 2	unknown (e.g. no network, etc.) 3	CLIR temporary mode
     * presentation restricted 4	CLIR temporary mode presentation allowed
     */
    int32_t clir;
} HRilDial;

/**
 * @brief Defines the call forwarding information.
 */
typedef struct {
    /** Call forwarding type <0-5> */
    int32_t reason;

    /** Call forwarding operation mode */
    int32_t mode;

    /** Phone number */
    char *number;

    /**
     * Is a sum of integers each representing a class of information default 255
     * 1: voice (telephony)
     * 2: data (refers to all bearer services; with <mode>=2 this may refer
     *    only to some bearer service if TA does not support values 16, 32, 64 and 128)
     * 4: fax (facsimile services)
     * 8: short message service
     * 16: data circuit sync
     * 32: data circuit async
     * 64: dedicated packet access
     * 128: dedicated PAD access
     */
    int32_t classx;

    /**
     * when "no reply" is enabled or queried, this gives the time in seconds to wait
     * before call is forwarded, default value 20
     */
    int32_t time;

    /** 0: not active, 1: active */
    int32_t status;
} HRilCFInfo;

/**
 * @brief Defines the dual tone multi-frequency (DTMF) information.
 */
typedef struct {
    /** Call ID */
    int32_t callId;

    /** DTMF keyword */
    const char *dtmfKey;

    /** Duration for playing the DTMF tone */
    int32_t onLength;

    /** Interval for sending DTMF signals */
    int32_t offLength;

    /** DTMF string length */
    int32_t stringLength;
} CallDtmfInfo;

/**
 * @brief Defines the call restriction password.
 */
typedef struct {
    /** Operation object */
    const char *fac;

    /**
     * Operation mode:
     * 0: deactivation
     * 1: activation
     */
    int32_t mode;

    /** Password */
    const char *password;
} CallRestrictionInfo;

/**
 * @brief Defines the call restriction password.
 */
typedef struct {
    /** Operation object */
    const char *fac;

    /** Old password */
    const char *oldPassword;

    /** New password */
    const char *newPassword;
} HRilSetBarringInfo;

/**
 * @brief Defines the call forwarding information.
 */
typedef struct {
    /** Call forwarding type <0-5> */
    int32_t reason;

    /** Call forwarding status. */
    int32_t status;

    /**
     * Is a sum of integers each representing a class of information default 255
     * 1: voice (telephony)
     * 2: data (refers to all bearer services; with <mode>=2 this may refer
     *    only to some bearer service if TA does not support values 16, 32, 64 and 128)
     * 4: fax (facsimile services)
     * 8: short message service
     * 16: data circuit sync
     * 32: data circuit async
     * 64: dedicated packet access
     * 128: dedicated PAD access
     */
    int32_t classx;

    /** Call forwarding number. */
    char *number;
    /**
     * Default 145 when dialling string includes international access code
     * character "+", otherwise 129, other value refer TS 24.008 [8]
     * sub-clause 10.5.4.7
     */
    int32_t type;

    /** Time. */
    int32_t time;
} HRilCFQueryInfo;

/**
 * @brief Defines the CLIP information.
 */
typedef struct {
    /** Parameter sets/shows the result code presentation status in the TA */
    int32_t action;

    /**
     * Parameter shows the subscriber CLIP service status in the network, <0-4>
     */
    int32_t clipStat;
} HRilGetClipResult;

/**
 * @brief Defines the CLIR information.
 */
typedef struct {
    /** Parameter sets/shows the result code presentation status in the TA */
    int32_t action;

    /**
     * Parameter shows the subscriber CLIP service status in the network,
     * <0-4>
     */
    int32_t clirStat;
} HRilGetCallClirResult;

/**
 * @brief Defines the call waiting information.
 */
typedef struct {
    /** Call wait status, could be 0 or 1 */
    int32_t status;

    /**
     * 1: voice (telephony);
     * 2: data;
     * 4: fax (facsimile services);
     * 8: short message service;
     * 16: data circuit sync;
     * 32: data circuit async;
     * 64: dedicated packet access;
     * 128: dedicated PAD access.
     */
    int32_t classCw;
} HRilCallWaitResult;

/**
 * @brief Defines the call barring information.
 */
typedef struct {
    /** Call restructuib status, could be 0 or 1 */
    int32_t status;

    /**
     * 1: voice (telephony);
     * 2: data;
     * 4: fax (facsimile services);
     * 8: short message service;
     * 16: data circuit sync;
     * 32: data circuit async;
     * 64: dedicated packet access;
     * 128: dedicated PAD access.
     */
    int32_t classCw;
} HRilCallRestrictionResult;

/**
 * @brief Defines the USSD result information.
 */
typedef struct {
    /**
     * Integer value.
     * 0: The network does not require a TE reply (USSD-Notify initiated by the network or TE
     *    The network does not need further information after starting operation);
     * 1: The network needs a TE reply (USSD-Request initiated by the network, or TE sent
     *    After starting the operation, the network needs further information);
     * 2: The USSD session is released by the network;
     * 3: Other local clients have responded;
     * 4: The operation is not supported;
     * 5: The network timed out.
     */
    int32_t m;

    /**
     * USSD string, the maximum length is 160 characters.
     */
    char *str;
} HRilUssdNoticeInfo;

/**
 * @brief Defines the supplementary service information.
 */
typedef struct {
    /**
     * Service type:
     * 0: call forwarding unconditional
     * 1: call forwarding on busy
     * 2: call forwarding on no reply
     * 3: call forwarding not reachable (no network service, or power-off)
     */
    int32_t serviceType;

    /**
     * Request type:
     * 0: deactivation
     * 1: activated
     * 2: status query
     * 3: registration
     * 4: deletion
     */
    int32_t requestType;

    /** Service class. For details, see 3GPP TS 27.007. */
    int32_t serviceClass;

    /** The result of the SS request */
    int32_t result;
} HRilSsNoticeInfo;

/**
 * @brief Defines the emergency information.
 */
typedef struct {
    /** Number index */
    int32_t index;

    /** Total number of numbers */
    int32_t total;

    /** Emergency call number in string format */
    char *eccNum;

    /**
     * Emergency call type
     * 0: interface message of initiating an emergency call
     * 1: Bandit police
     * 2: Rescue
     * 4: Fire alarm
     * 8: Marine Police
     * 16: Alpine rescue
     */
    int32_t category;

    /**
     * The number is valid with or without a card
     * 0: valid without card
     * 1: Valid with card
     */
    int32_t simpresent;

    /** Country code */
    char *mcc;

    /**
     * Distinguishes CS domain abnormal service status
     * 0: all States are valid
     * 1: the CS domain is not in normal service
     */
    int32_t abnormalService;
} HRilEmergencyInfo;

/**
 * @brief Active reporting of SRVCC status is controlled by the +CIREP command.
 *
 * This command complies with the 3GPP TS 27.007 protocol.
 */
typedef struct {
    /**
     * SRVCC status.
     * 1: SRVCC starts;
     * 2: SRVCC is successful;
     * 3: SRVCC is cancelled;
     * 4: SRVCC failed.
     */
    int32_t status;
} HRilCallSrvccStatus;

/**
 * @brief Query the bandwidth information of the voice channel during the call.
 *
 * Note: Modem private commands, not a reported field specified by the 3gpp
 * protocol.
 */
typedef struct {
    /**
     * Integer value, voice channel type:
     * 0: No in-band audio information;
     * 1: In-band voice is available, narrowband voice;
     * 2: In-band voice is available, broadband voice.
     */
    int32_t status;

    /**
     * Integer value, call domain of voice call:
     * 0: 3GPP CS domain voice call;
     * 1: IMS domain voice call;
     * 2: 3GPP2 CS domain voice call.
     */
    int32_t voiceDomain;
} HRilCallCsChannelInfo;

/**
 * @brief Defines the supplementary service notifications information.
 */
typedef struct {
    /** Type of notification: 0 = code1(MO); 1 = code2(MT) */
    int32_t notificationType;

    /** TS 27.007 7.17 "code1" or "code2" */
    int32_t code;

    /** TS 27.007 7.17 "index" */
    int32_t index;

    /** TS 27.007 7.17 "type" (MT only) */
    int32_t type;

    /** TS 27.007 7.17 "number" (MT only) */
    char *number;
} HRilSuppSvcNotification;

typedef struct {
    /**
     * @brief Gets the call status list.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}
     * @see ReqDataInfo
     */
    void (*GetCallList)(const ReqDataInfo *requestInfo);

    /**
     * @brief Makes a call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}
     * @param data HRilDial, for details, see {@link HRilDial}
     * @param dataLen The length of data.
     * @see ReqDataInfo | HRilDial
     */
    void (*Dial)(const ReqDataInfo *requestInfo, const HRilDial *data, size_t dataLen);

    /**
     * @brief Ends a call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}
     * @param data gsmIndex, Indicates the connection index
     * @param dataLen The length of data.
     * @see ReqDataInfo
     */
    void (*Hangup)(const ReqDataInfo *requestInfo, const uint32_t *data, size_t dataLen);

    /**
     * @brief Rejects a call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*Reject)(const ReqDataInfo *requestInfo);

    /**
     * @brief Answers a call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}
     * @see ReqDataInfo
     */
    void (*Answer)(const ReqDataInfo *requestInfo);

    /**
     * @brief Obtains the calling line identification presentation (CLIP)
     * information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}
     * @see ReqDataInfo
     */
    void (*GetClip)(const ReqDataInfo *requestInfo);

    /**
     * @brief Sets the calling line identification presentation (CLIP)
     * information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param action Whether to enable or disable the CLIP function. The value
     * 0 means to disable the CLIP function, and the value 1 means the
     * opposite.
     * @see ReqDataInfo
     */
    void (*SetClip)(const ReqDataInfo *requestInfo, int32_t action);

    /**
     * @brief Holds a call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*HoldCall)(const ReqDataInfo *requestInfo);

    /**
     * @brief Unholds a call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*UnHoldCall)(const ReqDataInfo *requestInfo);

    /**
     * @brief Switches a call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*SwitchCall)(const ReqDataInfo *requestInfo);

    /**
     * @brief Combines calls into a conference call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param callType Call type.
     * @see ReqDataInfo
     */
    void (*CombineConference)(const ReqDataInfo *requestInfo, int32_t callType);

    /**
     * @brief Separates calls from a conference call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param callType Call type.
     * @see ReqDataInfo
     */
    void (*SeparateConference)(const ReqDataInfo *requestInfo, int32_t callIndex, int32_t callType);

    /**
     * @brief Holds and resumes a call.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param type Type of ending calls. The value 0 indicates that the call is
     * ended directly; The value 1 means release the held call and the wait
     * call; The value 2 means that release the active call and recover the
     * held call; The value 3 means that release all calls.
     * @see ReqDataInfo
     */
    void (*CallSupplement)(const ReqDataInfo *requestInfo, int32_t type);

    /**
     * @brief Gets the call waiting status.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetCallWaiting)(const ReqDataInfo *requestInfo);

    /**
     * @brief Sets the call waiting status.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param active Whether to enable or disable call waiting. The value
     * <b>0</b> means to disable the call waiting function, and the value
     * <b>1</b> means the opposite.
     * @see ReqDataInfo
     */
    void (*SetCallWaiting)(const ReqDataInfo *requestInfo, int32_t active);

    /**
     * @brief Sets the call forwarding status.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param info Call forwarding information, for details, see {@link
     * CallForwardSetInfo}.
     * @see ReqDataInfo
     */
    void (*SetCallTransferInfo)(const ReqDataInfo *requestInfo, HRilCFInfo info);

    /**
     * @brief Gets the call forwarding information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param reason Call forwarding type, The value 0 indicates unconditional
     * call forwarding, 1 indicates call forwarding when the user is busy, 2
     * indicates call forwarding when the user does not reply, and 3 indicates
     * call forwarding when the user is unreachable.
     * @see ReqDataInfo
     */
    void (*GetCallTransferInfo)(const ReqDataInfo *requestInfo, int32_t reason);

    /**
     * @brief Gets the call restriction information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param fac Object for call restriction.
     * @see ReqDataInfo
     */
    void (*GetCallRestriction)(const ReqDataInfo *requestInfo, const char *fac);

    /**
     * @brief Sets the call restriction information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param info Call restriction information.
     * @see ReqDataInfo
     */
    void (*SetCallRestriction)(const ReqDataInfo *requestInfo, CallRestrictionInfo info);

    /**
     * @brief Obtains the calling line identification restriction (CLIR)
     * information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetClir)(const ReqDataInfo *requestInfo);

    /**
     * @brief Sets the CLIR information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param action Whether to enable or disable the CLIR function. The value
     * 0 means to disable the CLIR function, and the value 1 means the
     * opposite.
     * @see ReqDataInfo
     */
    void (*SetClir)(const ReqDataInfo *requestInfo, int32_t action);

    /**
     * @brief Enables DTMF.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param info DTMF information.
     * @see ReqDataInfo | CallDtmfInfo
     */
    void (*StartDtmf)(const ReqDataInfo *requestInfo, CallDtmfInfo info);

    /**
     * @brief Sends dual tone multi-frequency (DTMF).
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param info DTMF information.
     * @see ReqDataInfo | CallDtmfInfo
     */
    void (*SendDtmf)(const ReqDataInfo *requestInfo, CallDtmfInfo info);

    /**
     * @brief Disables DTMF.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param info DTMF information.
     * @see ReqDataInfo | CallDtmfInfo
     */
    void (*StopDtmf)(const ReqDataInfo *requestInfo, CallDtmfInfo info);

    /**
     * @brief Gets IMS call list.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetImsCallList)(const ReqDataInfo *requestInfo);

    /**
     * @brief Obtains the call preference mode.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetCallPreferenceMode)(const ReqDataInfo *requestInfo);

    /**
     * @brief Sets the call preference mode.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*SetCallPreferenceMode)(const ReqDataInfo *requestInfo, int32_t mode);

    /**
     * @brief Gets LTE IMS switch status.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetLteImsSwitchStatus)(const ReqDataInfo *requestInfo);

    /**
     * @brief Sets LTE IMS switch status.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param active Whether to enable or disable call waiting. The value
     * <b>0</b> means to disable the call waiting function, and the value
     * <b>1</b> means the opposite.
     * @see ReqDataInfo
     */
    void (*SetLteImsSwitchStatus)(const ReqDataInfo *requestInfo, int32_t active);

    /**
     * @brief Sets unstructured supplementary service data (USSD) information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param str USSD information.
     * @see ReqDataInfo
     */
    void (*SetUssd)(const ReqDataInfo *requestInfo, const char *str);

    /**
     * @brief Obtains the USSD information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetUssd)(const ReqDataInfo *requestInfo);

    /**
     * @brief Sets the mute mode.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param mute Whether to enable the mute function. The value 0 means to
     * disable the mute function, and the value 1 means the opposite.
     * @see ReqDataInfo
     */
    void (*SetMute)(const ReqDataInfo *requestInfo, int32_t mute);

    /**
     * @brief Obtains the mute mode.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetMute)(const ReqDataInfo *requestInfo);

    /**
     * @brief Obtains the emergency number.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetEmergencyCallList)(const ReqDataInfo *requestInfo);

    /**
     * @brief Obtains the call failure cause.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetCallFailReason)(const ReqDataInfo *requestInfo);

    /**
     * @brief Sets the emergency call number.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param emergencyInfo Emergency call information.
     * @param len The size of emergencyInfo.
     * @see ReqDataInfo
     */
    void (*SetEmergencyCallList)(const ReqDataInfo *requestInfo, HRilEmergencyInfo *emergencyInfo, const int len);

    /**
     * @brief Change call barring facility password.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param info Call restriction information.
     * @see ReqDataInfo
     */
    void (*SetBarringPassword)(const ReqDataInfo *requestInfo, HRilSetBarringInfo info);

    /**
     * @brief Close unfinished unstructured supplementary service data (USSD).
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*CloseUnFinishedUssd)(const ReqDataInfo *requestInfo);
    void (*ExplicitCallTransferConnection)(const ReqDataInfo *requestInfo);
    void (*SetVonrSwitch)(const ReqDataInfo *requestInfo, int32_t status);

    /**
     * @brief Enable or disable supplementary service related notification from the network.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param enable Whether to enable notification, 0 indicates disable, 1 indicates enable.
     * @see ReqDataInfo
     */
    void (*SetSuppSrvNotification)(const ReqDataInfo *requestInfo, int32_t enable);

    /**
     * @brief Get the TTY mode.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetTTYMode)(const ReqDataInfo *requestInfo);

    /**
     * @brief Set the TTY mode.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param mode The TTY mode to set, value as following:
     * - 0 indicates TTYMode#TTY_MODE_OFF
     * - 1 indicates TTYMode#TTY_MODE_FULL
     * - 2 indicates TTYMode#TTY_MODE_HCO
     * - 3 indicates TTYMode#TTY_MODE_VCO
     * @see ReqDataInfo
     */
    void (*SetTTYMode)(const ReqDataInfo *requestInfo, int32_t mode);
} HRilCallReq;
#ifdef __cplusplus
}
#endif
#endif // OHOS_RIL_VENDOR_CALL_DEFS_H
