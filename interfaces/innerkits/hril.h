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
#include "hril_notification.h"
#include "hril_request.h"

#define HRIL_VERSION 1
#define HRIL_VERSION_MIN 1

typedef enum { HRIL_SIM_1, HRIL_SIM_NUM } HRilSimId;

typedef enum {
    HRIL_ERR_NULL_POINT = -1,
    HRIL_ERR_SUCCESS = 0,
    HRIL_ERR_GENERIC_FAILURE = 1,
    HRIL_ERR_INVALID_PARAMETER = 2,
    HRIL_ERR_MEMORY_FULL = 3,
    HRIL_ERR_CMD_SEND_FAILURE = 4,
    HRIL_ERR_CMD_NO_CARRIER = 5,
    HRIL_ERR_INVALID_RESPONSE = 6,
    HRIL_ERR_REPEAT_STATUS = 7,

    // network error
    HRIL_ERR_NO_SIMCARD_INSERTED = 10,
    HRIL_ERR_NEED_PIN_CODE = 11,
    HRIL_ERR_NEED_PUK_CODE = 12,
    HRIL_ERR_INVALID_MODEM_PARAMETER = 50,
} HRilErrno;

/* From 3GPP TS 27.007 V4.3.0 (2001-12) ATD%s%s */
typedef enum {
    HRIL_CALL_ACTIVATE = 0,
    HRIL_CALL_HOLDING = 1,
    HRIL_CALL_DIALING = 2, /* MO call */
    HRIL_CALL_ALERTING = 3, /* MO call */
    HRIL_CALL_INCOMING = 4, /* MT call */
    HRIL_CALL_WAITING = 5 /* MT call */
} HRilCallState;

typedef enum {
    HRIL_RADIO_POWER_STATE_UNAVAILABLE = -1,
    HRIL_RADIO_POWER_STATE_ON_MINI = 0,
    HRIL_RADIO_POWER_STATE_ON = 1,
    HRIL_RADIO_POWER_STATE_OFF = 4,
    HRIL_RADIO_POWER_STATE_FTM = 5,
    HRIL_RADIO_POWER_STATE_REBOOT = 6
} HRilRadioState;

typedef enum { HRIL_CALL_VOICE, HRIL_CALL_DATA, HRIL_CALL_FAX } HRilCallMode;

typedef enum {
    HRIL_RADIO_TECHNOLOGY_UNKNOWN,
    HRIL_RADIO_TECHNOLOGY_GSM,
    HRIL_RADIO_TECHNOLOGY_1xRTT,
    HRIL_RADIO_TECHNOLOGY_WCDMA
} HRilRadioAccessTechnology;

/* From 3GPP TS 27.007 V4.3.0 (2001-12)  AT+CGDCONT */
typedef struct {
    int index; /* Call identification number as described in 3GPP TS 22.030 [19] sub-clause 6.5.5.1.
                * This number can be used in +CHLD command operations */
    int dir; /* 0	mobile originated (MO) call
              * 1	mobile terminated (MT) call */
    HRilCallState state; /* call state */
    HRilCallMode mode; /* call mode */
    int mpty; /* 0	call is not one of multiparty (conference) call parties
               * 1	call is one of multiparty (conference) call parties */
#ifdef HISILICON
    int callType; /* 0: voice call
                   * 1: Video call: send one-way video, two-way voice
                   * 2: Video call: one-way receiving video, two-way voice
                   * 3: Video call: two-way video, two-way voice */
    int voiceDomain; /* Identifies the service domain.
                      * 0: CS domain phone
                      * 1: IMS domain phone */
#endif
    char *number; /* phone number in format specified by <type> */

    int type; /* Type of address octet in integer format (refer TS 24.008 [8] subclauses 10.5.4.7);
               * default 145 when dialling string includes international access code character "+",
               * otherwise 129. see 3GPP TS 27.007 V4.3.0 (2001-12) 6.1 */
    char *alpha; /* alphanumeric representation of <number> corresponding to the entry found in phonebook;
                  * used character set should be the one selected with command select TE character set +CSCS */
} HRilCallInfo;

typedef struct {
    int state; /* from 3GPP TS 27.007 10.1.10 V4.3.0 (2001-12)
                * indicates the state of PDP context activation
                * 0 - deactivated
                * 1 - activated */
    int retryTime; /* if errorCode != 0, suggested retry time */
    int cid; /* from 3GPP TS 27.007 10.1.1 V4.3.0 (2001-12)
              * specifies a particular PDP context definition. The parameter is local to the TE-MT interface and
              * is used in other PDP context-related commands. */
    int active; /* 0: inactive, 1: active(physical link down), 2 : 2=active (physical link up) */
    char *type; /* PDP_type values from 3GPP TS 27.007 section 10.1.1.
                 * specifies the type of packet data protocol. The default value is manufacturer specific. */
    char *netPortName; /* Network interface name */
    char *address; /*  from 3GPP TS 27.007 10.1.1 V4.3.0 (2001-12)
                    *  a string parameter that identifies the MT in the address space applicable to the PDP. */
    char *dns; /* If the MT indicates more than two IP addresses of P-CSCF servers
                * or more than two IP addresses of DNS servers,
                * multiple lines of information per <cid> will be returned.
                * If the MT has dual stack capabilities,
                * First one line with the IPv4 parameters followed by one line with the IPv6 parameters. */
    char *gateway; /* network gateway address */
    int maxTransferUnit; /* Maximum Transfer Unit. The range of permitted values (minimum value = 1
                          * or if the initial PDP context is supported minimum value = 0)
                          * is returned by the test form of the command. */
} HRilDataCallResponse;

typedef struct {
    int cid; /* from 3GPP TS 27.007 10.1.1 V4.3.0 (2001-12)
              * specifies a particular PDP context definition. The parameter is local to the TE-MT interface and
              * is used in other PDP context-related commands. */
    char *apn; /* (Access Point Name) a string parameter
                * which is a logical name that is used to select the GGSN or the */
    char *type; /* PDP_type values from 3GPP TS 27.007 section 10.1.1.
                 * specifies the type of packet data protocol. The default value is manufacturer specific. */
} HRilDataInfo;

typedef enum {
    RADIO_TECH_3GPP = 1, /* 3GPP Technologies (GSM, WCDMA) */
    RADIO_TECH_3GPP2 = 2 /* 3GPP2 Technologies (CDMA) */
} HRilRadioTechnologyFamily;

typedef struct {
    int msgRef; /* TP-Message-Reference for GSM, and BearerData MessageId for CDMA
                 * from 3GPP2 C.S0015-B, v2.0, 4.5-1 */
    char *pdu; /* Protocol Data Unit */
    int errCode; /* if unknown or not applicable, that is -1
                  * from 3GPP 27.005, 3.2.5 for GSM/UMTS,
                  * 3GPP2 N.S0005 (IS-41C) Table 171 for CDMA */
} HRilSmsResponse;

typedef struct {
    int index;
    int state;
    char *smsc;
    char *pdu; /* Protocol Data Unit */
} HRilSmsWriteSms;

typedef struct {
    int tosca;
    char *address;
} HRilServiceCenterAddress;

/* From 3GPP TS 27.005  AT+CSCB=[<mode>[,<mids>[,<dcss>]]] */
typedef struct {
    int mode;
    char *mids;
    char *dcss;
} HRilCellBroadcastInfo;

/* From 3GPP TS 27.005   if text mode (+CMGF=1):
    <CR><LF>+CBM:
    <sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data><CR>
    <LF> if PDU mode
    (+CMGF=0):
    <CR><LF>+CBM: <length><CR><LF><pdu><CR><LF>
*/
typedef struct {
    int sn;
    int mid;
    int page;
    int pages;
    char *dcs;
    char *data;
    int length;
    char *pdu;
} HRilCellBroadcastReportInfo;

/* From 3GPP TS 27.007 V4.3.0 (2001-12) ATD%s%s */
typedef struct {
    char *address; /* Type of address octet in integer format (refer TS 24.008 [8] subclauses 10.5.4.7);
                    * default 145 when dialling string includes international access code character "+", otherwise
                    * 129. */
    int clir; /* This command refers to CLIR service according to 3GPP TS 22.081 [3] that allows a calling
               * subscriber to enable or disable the presentation of the CLI to the called party when originating a
               * call. <n> (parameter sets the adjustment for outgoing calls): 0	presentation indicator is used
               * according to the subscription of the CLIR service 1	CLIR invocation 2	CLIR suppression <m>
               * (parameter shows the subscriber CLIR service status in the network): 0	CLIR not provisioned 1
               * CLIR provisioned in permanent mode 2	unknown (e.g. no network, etc.) 3	CLIR temporary mode
               * presentation restricted 4	CLIR temporary mode presentation allowed */
} HRilDial;

// 3GPP TS 27.007 V3.9.0 (2001-06) 7.11	Call forwarding number and conditions +CCFC
typedef enum {
    CALL_FORWARD_REASON_UNCONDITIONAL,
    CALL_FORWARD_REASON_MOBILE_BUSY,
    CALL_FORWARD_REASON_NO_REPLY,
    CALL_FORWARD_REASON_NO_REACHABLE,
    CALL_FORWARD_REASON_ALL_CF, /* all call forwarding (refer 3GPP TS 22.030 [19]) */
    CALL_FORWARD_REASON_ALL_CCF, /* all conditional call forwarding */
} CallForwardReasonType;

// 3GPP TS 27.007 V3.9.0 (2001-06) 7.11	Call forwarding number and conditions +CCFC
typedef enum {
    CALL_FORWARD_ClassX_VOICE = 1,
    CALL_FORWARD_ClassX_DATA = 2,
    CALL_FORWARD_ClassX_FAX = 4,
    CALL_FORWARD_ClassX_SMS = 8, /* short message service */
    CALL_FORWARD_ClassX_DCS = 16, /* data circuit sync */
    CALL_FORWARD_ClassX_DCAS = 32, /* dedicated packet access */
    CALL_FORWARD_ClassX_DPA = 64, /* all conditional call forwarding */
    CALL_FORWARD_ClassX_DPADA = 128, /* dedicated PAD access */
} CallForwardClassX;

// 3GPP TS 27.007 V3.9.0 (2001-06) 7.11	Call forwarding number and conditions +CCFC
typedef enum {
    CALL_FORWARD_MODE_DISABLE,
    CALL_FORWARD_MODE_ENABLE,
    CALL_FORWARD_MODE_QUERY_STATUS,
    CALL_FORWARD_MODE_REGISTRATION,
    CALL_FORWARD_MODE_ERASURE,
} CallForwardMode;

// 3GPP TS 27.007 V3.9.0 (2001-06) Call related supplementary services +CHLD
// 3GPP TS 27.007 V3.9.0 (2001-06) 7.22	Informative examples
typedef enum {
    TYPE_HANG_UP_HOLD_WAIT = 0,
    TYPE_HANG_UP_ACTIVE = 1,
} HRilCallSupplementType;

typedef struct {
    int reason; /* call forwarding type <0-5> */
    int mode; /* call forwarding operation mode */
    char *number; /* phone number */
    int classx; /* is a sum of integers each representing a class of information default 255
                 *  1	voice (telephony)
                 *  2	data (refers to all bearer services; with <mode>=2 this may refer only
                 *      to some bearer service if TA does not support values 16, 32, 64 and 128)
                 *  4	fax (facsimile services)
                 *  8	short message service
                 *  16	data circuit sync
                 *  32	data circuit async
                 *  64	dedicated packet access
                 *  128	dedicated PAD access
                 */
    int time; /* when "no reply" is enabled or queried, this gives the time in seconds to wait
               * before call is forwarded, default value 20
               */
    int status; /* 0: not active, 1: active */
} HRilCFInfo;

typedef struct {
    int callId;
    const char *dtmfKey;
    int onLength;
    int offLength;
    int stringLength;
} CallDTMFInfo;

typedef struct {
    const char *fac;
    int mode;
    const char *password;
} CallRestrictionInfo;

typedef struct {
    int status;
    int classx;
    char *number;
    int type; /* default 145 when dialling string includes international access code
               * character "+", otherwise 129,
               * other value refer TS 24.008 [8] sub-clause 10.5.4.7
               */
} HRilCFQueryInfo;

typedef struct {
    int action; /* parameter sets/shows the result code presentation status in the TA */
    int clipStat; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
} HRilGetClipResult;

typedef struct {
    int action; /* parameter sets/shows the result code presentation status in the TA */
    int clirStat; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
} HRilGetCallClirResult;

typedef struct {
    int status; /* 0 or 1 */
    /**
     * 1：voice (telephony)； 2：data； 4：fax (facsimile services)； 8：short message service；
     * 16：data circuit sync；
     * 32：data circuit async；
     * 64：dedicated
     * packet access； 128：dedicated PAD access.
     */
    int classCw;
} HRilCallWaitResult;

typedef struct {
    int status; /* 0 or 1 */
    /**
     * 1：voice (telephony)； 2：data； 4：fax (facsimile services)； 8：short message service；
     * 16：data circuit sync；
     * 32：data circuit async；
     * 64：dedicated
     * packet access； 128：dedicated PAD access.
     */
    int classCw;
} HRilCallRestrictionResult;

/* Form 3GPP TS 27.007 V4.3.0 (2001-12) 8.18, + CRSM */
typedef struct {
    int command; /* command passed on by the MT to the SIM; refer 3GPP TS 51.011 [28]
                  * 176	READ BINARY
                  * 178	READ RECORD
                  * 192	GET RESPONSE
                  * 214	UPDATE BINARY
                  * 220	UPDATE RECORD
                  * 242	STATUS
                  * 203	RETRIEVE DATA
                  * 219	SET DATA
                  */
    int fileid; /* this is the identifier of a elementary datafile on SIM.
                 * Mandatory for every command except STATUS. */
    int p1; /* parameters passed on by the MT to the SIM. These parameters are mandatory for every command,
             * except GET RESPONSE and STATUS. The values are described in 3GPP TS 51.011 [28]. */
    int p2; /* parameters passed on by the MT to the SIM. These parameters are mandatory for every command,
             * except GET RESPONSE and STATUS. The values are described in 3GPP TS 51.011 [28]. */
    int p3; /* parameters passed on by the MT to the SIM. These parameters are mandatory for every command,
             * except GET RESPONSE and STATUS. The values are described in 3GPP TS 51.011 [28]. */
    char *data; /* information which shall be written to the SIM (hexadecimal character format; refer +CSCS). */
    char *pathid; /* contains the path of an elementary file on the SIM/UICC in hexadecimal format
                   * as defined in ETSI TS 102 221 [60] (e.g. "7F205F70" in SIM and UICC case).
                   * The <pathid> shall only be used in the mode "select by path from MF"
                   * as defined in ETSI TS 102 221 [60]. */
} HRilSimIO;

/* Form TS 27.007.8.18 +CRSM */
typedef struct {
    int sw1; /* information from the SIM about the execution of the actual command.
              * These parameters are delivered to the TE in both cases,
              * on successful or failed execution of the command. */
    int sw2; /* information from the SIM about the execution of the actual command.
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

typedef enum {
    HRIL_CALL_FAIL_NO_VALID_NUMBER = 1,
    HRIL_CALL_FAIL_NO_LINE_TO_TARGET_ADDRESS = 3
} HRilLastCallErrorCode;

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

typedef enum {
    HRIL_SIM_NOT_READY = -1,
    HRIL_SIM_NOT_INSERTED = 0,
    HRIL_SIM_READY = 1,
    HRIL_SIM_PIN,
    HRIL_SIM_PUK,
    HRIL_SIM_PIN2,
    HRIL_SIM_PUK2,
    HRIL_PH_NET_PIN,
    HRIL_PH_NET_PUK,
    HRIL_PH_NET_SUB_PIN,
    HRIL_PH_NET_SUB_PUK,
    HRIL_PH_SP_PIN,
    HRIL_PH_SP_PUK
} HRilSimState;

typedef enum {
    HRIL_PIN_STATE_UNKNOWN,
    HRIL_PIN_NOT_VERIFIED,
    HRIL_PIN_VERIFIED,
    HRIL_PIN_DISABLED,
    HRIL_PIN_BLOCKED_ENABLED,
    HRIL_PIN_BLOCKED_PERM
} HRilSimPinState;

typedef enum { HRIL_UNKNOWN = 0, HRIL_SIM = 1, HRIL_USIM = 2 } HRilSimType;

typedef struct {
    int index; /* sim index */
    HRilSimType simType;
    HRilSimState simState;
} HRilCardState;

/* From 3GPP TS 27.007 8.5 */
typedef struct {
    int rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99 */
    int ber; /* bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max */
} HRilGwRssi;

/* From 3GPP TS 27.007 8.5 */
typedef struct {
    int rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99 */
    int ecio; /* integer type, ratio of the received energy per PN chip to the total received power
               * spectral density (see 3GPP TS 25.133 [95] sub-clause). */
    int rscp; /* integer type, received signal code power (see 3GPP TS 25.133 [95] sub-clause 9.1.1.3 and
               * 3GPP TS 25.123 [96] sub-clause 9.1.1.1.3).
               * range  value 0 ~ 96, 255 not known or not detectable */
    int ber; /* bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max */
} HRilWcdmaRssi;

/* From 3GPP TS 27.007 8.69 */
typedef struct {
    int absoluteRssi; /* Absolute value of signal strength.  This value is the actual Rssi value
                       * multiplied by -1.
                       * e.g: Rssi is -75, then this response value will be 75 */
    int ecno; /* ratio of the received energy per PN chip to the total received power spectral density,
               * e.g: If the actual Ec/Io is -12.5 dB, then this response value will be 125.
               * from 3GPP TS 25.133[95] */
} HRilCdmaRssi;

typedef struct {
    int rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99 */
    int rsrq; /* integer type, reference signal received quality (see 3GPP TS 36.133 [96] sub-clause 9.1.7)
               * value range 0~33, 255 not known or not detectable */
    int rsrp; /* integer type, reference signal received power (see 3GPP TS 36.133 [96] sub-clause 9.1.4)
               * value range 0~97, 255 not known or not detectable */
    int snr; /* integer type, representing the signal-to-interference plus noise ratio, suitable for LTE mode
              * value range 0~251, 255 not known or not detectable */
} HRilLteRssi;

typedef struct {
    HRilGwRssi gwRssi;
    HRilCdmaRssi cdmaRssi;
    HRilWcdmaRssi wcdma;
    HRilLteRssi lte;
} HRilRssi;

typedef struct {
    char state[3];
    char radioTechnology[3];
} HRilRegStatus;

typedef struct ReqDataInfo {
    int serial;
    int request;
    HRilSimId simId;
} ReqDataInfo;

typedef enum {
    HRIL_RESPONSE = 0,
    HRIL_NOTIFICATION = 1,
} ReportType;

typedef enum {
    HRIL_REPORT_ERR_TYPE_NONE = 0,
    HRIL_REPORT_ERR_TYPE_GENERIC,
    HRIL_REPORT_ERR_TYPE_CME,
    HRIL_REPORT_ERR_TYPE_CMS,
} ReportErrorType;

typedef struct ModemReportErrorInfo {
    int errorNo;
    ReportErrorType errType;
} ModemReportErrorInfo;

struct ReportInfo {
    ReqDataInfo *requestInfo;
    int notifyId;
    ReportType type;
    HRilErrno error;
    ModemReportErrorInfo modemErrInfo;
};

/* From 3GPP TS 27.007 7.4 */
typedef struct {
    char *fac; /* ("SC","AO","OI","OX","AI","IR","AB","AG","AC","FD","PN","PU","PP") */
    int mode; /* 0 unlock
               * 1	lock
               * 2	query status */
    int status; /* 0 not active
                 * 1 active */
    char *passwd; /* shall be the same as password specified for the facility
                   * from the MT user interface or with command Change Password +CPWD */
    int classx; /* is a sum of integers each representing a class of information (default 7 - voice, data and fax):
                 * 1	voice (telephony)
                 * 2	data (refers to all bearer services; with <mode>=2 this may refer only to some bearer service
                 *      if TA does not support values 16, 32, 64 and 128)
                 * 4	fax (facsimile services)
                 * 8	short message service
                 * 16	data circuit sync
                 * 32	data circuit async
                 * 64	dedicated packet access
                 * 128	dedicated PAD access  */
} HRilSimClock;

/* From 3GPP TS 27.007 7.5 */
typedef struct {
    char *fac; /* ("SC","AO","OI","OX","AI","IR","AB","AG","AC","FD","PN","PU","PP") */
    char *oldPassword; /* <oldPassword> shall be the same as password specified for the facility from the MT
                        * user interface or with command Change Password +CPWD and <newPassword> is the new
                        * password; maximum length of password can be determined with <passwordLength> */
    char *newPassword;
    int passwordLength; /* maximum length of the password for the facility */
} HRilSimPassword;

typedef struct {
    char *code;
    int times;
    int pukTimes;
    int pinTimes;
    int puk2Times;
    int pin2Times;
} HRilPinInputTimes;

typedef struct {
    int serial;
    int selectMode;
    char *oper;
} HRiSetNetworkModeInfo;

struct HRilReport {
    void (*OnCallReport)(struct ReportInfo reportInfo, const void *data, size_t dataLen);
    void (*OnDataReport)(struct ReportInfo reportInfo, const void *data, size_t dataLen);
    void (*OnModemReport)(struct ReportInfo reportInfo, const void *data, size_t dataLen);
    void (*OnNetworkReport)(struct ReportInfo reportInfo, const void *data, size_t dataLen);
    void (*OnSimReport)(struct ReportInfo reportInfo, const void *data, size_t dataLen);
    void (*OnSmsReport)(struct ReportInfo reportInfo, const void *data, size_t dataLen);
};

typedef struct {
    void (*GetCallList)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*Dial)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*Hangup)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*Reject)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*Answer)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*GetClip)(ReqDataInfo *requestInfo);
    void (*SetClip)(ReqDataInfo *requestInfo, int action);
    void (*Hold)(ReqDataInfo *requestInfo);
    void (*Active)(ReqDataInfo *requestInfo);
    void (*Swap)(ReqDataInfo *requestInfo);
    void (*Join)(ReqDataInfo *requestInfo, int callType);
    void (*Split)(ReqDataInfo *requestInfo, int nThCall, int callType);
    void (*CallSupplement)(ReqDataInfo *requestInfo, int type);
    void (*GetCallWait)(ReqDataInfo *requestInfo);
    void (*SetCallWait)(ReqDataInfo *requestInfo, int active);
    void (*SetCallForwarding)(ReqDataInfo *requestInfo, HRilCFInfo info);
    void (*GetCallForwarding)(ReqDataInfo *requestInfo, int reason);
    void (*GetCallRestriction)(ReqDataInfo *requestInfo, const char *fac);
    void (*SetCallRestriction)(ReqDataInfo *requestInfo, CallRestrictionInfo info);
    void (*GetClir)(ReqDataInfo *requestInfo);
    void (*SetClir)(ReqDataInfo *requestInfo, int action);
    void (*StartDtmf)(ReqDataInfo *requestInfo, CallDTMFInfo info);
    void (*SendDtmf)(ReqDataInfo *requestInfo, CallDTMFInfo info);
    void (*StopDtmf)(ReqDataInfo *requestInfo, CallDTMFInfo info);
} HRilCallReq;

typedef struct {
    void (*ActivatePdpContext)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
    void (*DeactivatePdpContext)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
} HRilDataReq;

typedef struct {
    void (*SetRadioStatus)(const ReqDataInfo *requestInfo, int fun, int rst);
    void (*GetRadioStatus)(const ReqDataInfo *requestInfo);
} HRilModemReq;

typedef struct {
    void (*GetSignalStrength)(const ReqDataInfo *requestInfo);
    void (*GetCsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetPsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetOperatorInfo)(const ReqDataInfo *requestInfo);
    void (*GetNetworkSearchInformation)(const ReqDataInfo *requestInfo);
    void (*GetNetworkSelectionMode)(const ReqDataInfo *requestInfo);
    void (*SetNetworkSelectionMode)(const ReqDataInfo *requestInfo, const HRiSetNetworkModeInfo *data);
} HRilNetworkReq;

typedef struct {
    void (*GetSimStatus)(const ReqDataInfo *requestInfo);
    void (*GetSimIO)(const ReqDataInfo *requestInfo, const HRilSimIO *data, size_t dataLen);
    void (*GetSimImsi)(const ReqDataInfo *requestInfo);
    void (*GetSimIccID)(const ReqDataInfo *requestInfo);
    void (*GetSimLockStatus)(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
    void (*SetSimLock)(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
    void (*ChangeSimPassword)(const ReqDataInfo *requestInfo, const HRilSimPassword *data, size_t dataLen);
    void (*EnterSimPin)(const ReqDataInfo *requestInfo, const char *pin);
    void (*UnlockSimPin)(const ReqDataInfo *requestInfo, const char *puk, const char *pin);
    void (*GetSimPinInputTimes)(const ReqDataInfo *requestInfo);
} HRilSimReq;

typedef struct {
    void (*SendSms)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*SendSmsAck)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*StorageSms)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*DeleteSms)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*UpdateSms)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*SetSmsCenterAddress)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*GetSmsCenterAddress)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
    void (*SetCellBroadcast)(ReqDataInfo *requestInfo, const void *data, size_t dataLen);
} HRilSmsReq;

typedef struct {
    int version;
    const HRilCallReq *callOps;
    const HRilSimReq *simOps;
    const HRilSmsReq *smsOps;
    const HRilDataReq *dataOps;
    const HRilNetworkReq *networkOps;
    const HRilModemReq *modemOps;
} HRilOps;

typedef enum {
    RAT_TYPE_NONE = 0, /* indicates no cell information */
    RAT_TYPE_GSM,
    RAT_TYPE_LTE,
    RAT_TYPE_WCDMA,
} RatType; /* Radio Access Technology  */

typedef struct {
    int band; /* value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900 */
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int bsic; /* cell sit code 0~63 */
    int cellId;
    int lac; /* Location area code 0~FFFF */
    int rxlev; /* <RXLEV> dbm -120~37 */
    int rxQuality; /* 0~7 */
    int ta; /* 0~63 */
} CellRatGsm;

typedef struct {
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int cellId;
    int pci; /* Physical cell ID. 0~503 */
    int tac; /* Tracking Area Code 0~FFFF */
    int rsrp; /* Reference Signal Received Power -140~-44， dBm */
    int rsrq; /* Reference Signal Received Quality -19.5~-3 */
    int rssi; /* Receiving signal strength in dbm 90~-25 */
} CellRatLte;

typedef struct {
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int psc; /* Primary Scrambling Code. 0~511 */
    int cellId;
    int lac; /* Tracking Area Code 0~FFFF */
    int rscp; /* Reference Signal Received Power -140~-44, dBm */
    int rxlev; /* Reference Signal Received Quality -19.5~-3 */
    int ecno; /* Receiving signal strength in dbm 90~-25 */
    int drx; /* Discontinuous reception cycle length. 6~9 */
    int ura; /* UTRAN Registration Area Identity. 0~65535 */
} CellRatWcdma;

// cellList
typedef struct {
    int band; /* value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900 */
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~1023 */
    int bsic; /* cell sit code 0~63 */
    int cellId;
    int lac; /* Location area code 0~FFFF */
    int rxlev; /* <RXLEV> dbm -120~37 */
} CellListRatGsm;

typedef struct {
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~39649 */
    int pci; /* Physical cell ID. 0~503 */
    int rsrp; /* Reference Signal Received Power -140~-44, dBm */
    int rsrq; /* Reference Signal Received Quality -19.5~-3 */
    int rxlev; /* <RXLEV> dbm -120~37 */
} CellListRatLte;

typedef struct {
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~16383 */
    int psc; /* Primary Scrambling Code. 0~511 */
    int rscp; /* Received Signal Code Power in dBm -120~25, dBm */
    int ecno; /* The ratio of power per modulation bit to noise spectral density. -25~0 */
} CellListRatWcdma;

typedef struct {
    char *longName;
    char *shortName;
    char *numeric;
    int status;
    int rat;
} AvailableOperInfo;

const HRilOps *RilInitOps(const struct HRilReport *g_ReportOps);
#endif // OHOS_HRIL_H