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
#include <stdint.h>

#include "hril_enum.h"

#define MAX_CMD_LENGTH 500

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
    int voiceDomain; /* Identifies the service domain.
                      * 0: CS domain phone
                      * 1: IMS domain phone */
    int callType; /* 0: voice call
                   * 1: Video call: send one-way video, two-way voice
                   * 2: Video call: one-way receiving video, two-way voice
                   * 3: Video call: two-way video, two-way voice */
    char *number; /* phone number in format specified by <type> */

    int type; /* Type of address octet in integer format (refer TS 24.008 [8] subclauses 10.5.4.7);
               * default 145 when dialling string includes international access code character "+",
               * otherwise 129. see 3GPP TS 27.007 V4.3.0 (2001-12) 6.1 */
    char *alpha; /* alphanumeric representation of <number> corresponding to the entry found in phonebook;
                  * used character set should be the one selected with command select TE character set +CSCS */
} HRilCallInfo;

typedef struct {
    int smsSrvStatus; /* Service status of IMS SMS.
                       * 0: IMS SMS service is not available.
                       * 1: IMS SMS is in limited service.
                       * 2: IMS SMS is in full service. */
    int smsSrvRat; /* IMS SMS service domain.
                    * 0: IMS service is registered on LTE.
                    * 1: IMS service is registered on WIFI.
                    * 2: Reserve. */
    int voipSrvStatus; /* Service status of IMS VoIP.
                        * 0: IMS VoIP service is not available.
                        * 1: IMS VoIP is in limited service.
                        * 2: IMS VoIP is in full service. */
    int voipSrvRat; /* IMS VoIP service domain.
                     * 0: IMS service is registered on LTE.
                     * 1: IMS service is registered on WIFI.
                     * 2: Reserve. */
    int vtSrvStatus; /* Service status of IMS VT.
                      * 0: IMS VT service is not available.
                      * 1: IMS VT is in limited service.
                      * 2: IMS VT is in full service. */
    int vtSrvRat; /* IMS VT service domain.
                   * 0: IMS service is registered on LTE.
                   * 1: IMS service is registered on WIFI.
                   * 2: Reserve. */
    int vsSrvStatus; /* Service status of IMS VS.
                      * 0: IMS VS service is not available.
                      * 1: IMS VS is in limited service.
                      * 2: IMS VS is in full service. */
    int vsSrvRat; /* IMS VS service domain.
                   * 0: IMS service is registered on LTE.
                   * 1: IMS service is registered on WIFI.
                   * 2: Reserve. */
} HRilImsServiceStatus;

typedef struct {
    int callId; /* Call ID */
    int status; /* Indicates the call status of the corresponding callid.
                 * 0: Call originate(MO).
                 * 1: Call is Proceeding.
                 * 2: Alerting
                 * 3: Connected
                 * 4: Released
                 * 5: InComing
                 * 6: Waiting
                 * 7: Hold
                 * 8: Retrieve */
    int voiceDomain; /* Call domain for voice call.
                      * 0: CS domain voice call.
                      * 1: IMS domain voice call. */
} HRilCallStatusInfo;

typedef struct {
    int callId; /* Call ID, Value range: 1 ~ 7 */
    int duration; /* Call duration, unit: s. */
    int noCliCause; /* No caller number reason value.
                     * 0: The number is not available.
                     * 1: The user refused to provide a number.
                     * 2: Affected by other services.
                     * 3: Pay phone. */
    int ccCause; /* The value of the reason for the call error.
                  * Corresponds to the CS domain error code. */
} HRilCallEndInfo;

typedef struct {
    int callId; /* Call ID, Value range: 1 ~ 7 */
    int type; /* Call type.
               * 0: Voice call.
               * 9: Emergency call. */
} HRilCallConnectInfo;

typedef struct {
    char *type; /* business type..
                 * VOICE: Normal voice call.
                 * SYNC: Circuit domain 64kbit/s data service call (synchronous transparent mode).
                 * GPRS: PDP activation indication initiated by the network. */
    char *pdpType; /* Packet data protocol type.
                    * IP: Internet Protocol (IPV4).
                    * IPV6: IPV6 protocol.
                    * IPV4V6: IPV4 & IPV6 */
    char *pdpAddress; /* PDP address of IPV4 type. */
    char *l2p; /* Identifies the layer 2 protocol used between TE and MT. This parameter is ignored. */
    char *apn; /* Access point name, used to select GGSN or external packet data network;
                * if the value is empty, the contract value is used. */
} HRilCallCringInfo;

typedef struct {
    char *number; /* Phone number, format determined by < type >. */
    int type; /* Number address type.
               * 129: Normal number.
               * 145: International number (starting with "+"). */
    int businessClass; /* Business category, the value is a combination of the following business categories,
                        * and the default value is 255.
                        * 1: voice (telephony);
                        * 2: data;
                        * 4: fax (facsimile services);
                        * 8: short message service;
                        * 16: data circuit sync;
                        * 32: data circuit async;
                        * 64: dedicated packet access;
                        * 128: dedicated PAD access. */
} HRilCallWaitInfo;

typedef struct {
    int reason; /* Data call fail reason. succes is 0 */
    int retryTime; /* if errorCode != 0, suggested retry time */
    int cid; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
              * specifies a particular PDP context definition. The parameter is local to the TE-MT interface and
              * is used in other PDP context-related commands. */
    int active; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                 * indicates the state of PDP context activation
                 * 0 - deactivated
                 * 1 - activated */
    char *type; /* PDP_type values from 3GPP TS 27.007 V17.3.0 (2021-09)
                 * specifies the type of packet data protocol. The default value is manufacturer specific. */
    char *netPortName; /* Network interface name */
    char *address; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                    * <local_addr and subnet_mask>: string type;
                    * shows the IP address and subnet mask of the MT. The string is given as dot-separated
                    * numeric (0-255) parameters on the form: "a1.a2.a3.a4.m1.m2.m3.m4" for IPv4 or
                    * "a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.
                    * m1.m2.m3.m4.m5.m6.m7.m8.m9.m10.m11.m12.m13.m14.m15.m16" for IPv6.
                    * When +CGPIAF is supported, its settings can influence the format of this parameter returned
                    * with the execute form of +CGCONTRDP */
    char *dns; /* If the MT indicates more than two IP addresses of P-CSCF servers
                * or more than two IP addresses of DNS servers,
                * multiple lines of information per <cid> will be returned.
                * If the MT has dual stack capabilities,
                * First one line with the IPv4 parameters followed by one line with the IPv6 parameters. */
    char *gateway; /* network gateway address */
    char *pcscfa; /* The P-CSCF(Proxy Call State Control Function) addresses,Empty if not IMS client. */
    int maxTransferUnit; /* Maximum Transfer Unit. The range of permitted values (minimum value = 1
                          * or if the initial PDP context is supported minimum value = 0)
                          * is returned by the test form of the command. */
} HRilDataCallResponse;

typedef struct {
    int cid; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
              * specifies a particular PDP context definition. The parameter is local to the TE-MT interface and
              * is used in other PDP context-related commands. */
    char *apn; /* (Access Point Name) a string parameter
                * which is a logical name that is used to select the GGSN or the */
    char *type; /* PDP_type values from 3GPP TS 27.007 V17.3.0 (2021-09)
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
} HRilCBConfigInfo;

typedef struct {
    int32_t service;
    int32_t language;
    unsigned char checked;
} HRilCdmaCBConfigInfo;

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
} HRilCBConfigReportInfo;

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
} CallDtmfInfo;

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
     * 1: voice (telephony);
     * 2: data;
     * 4: fax (facsimile services);
     * 8: short message service;
     * 16: data circuit sync;
     * 32: data circuit async;
     * 64: dedicated packet access;
     * 128: dedicated PAD access.
     */
    int classCw;
} HRilCallWaitResult;

typedef struct {
    int status; /* 0 or 1 */
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
    char *pin2;
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

typedef struct {
    int index; /* sim index */
    HRilSimType simType;
    HRilSimState simState;
} HRilCardState;

/* From 3GPP TS 27.007 8.5 */
typedef struct {
    int rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99 */
    int ber; /* bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max */
} HRilGsmRssi;

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
    HRilGsmRssi gsmRssi;
    HRilCdmaRssi cdmaRssi;
    HRilWcdmaRssi wcdmaRssi;
    HRilLteRssi lteRssi;
} HRilRssi;

typedef struct {
    char state[3];
    char radioTechnology[3];
} HRilRegStatus;

typedef struct ReqDataInfo {
    int serial;
    int request;
    HRilSimSlotId slotId;
} ReqDataInfo;

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
} HRilSetNetworkModeInfo;

typedef struct {
    int32_t notifyType;
    int32_t regState;
    int32_t lacCode;
    int32_t cellId;
    int32_t actType;
} HRilRegStatusInfo;

typedef struct {
    int32_t notifyType;
    int32_t regInfo;
    int32_t extInfo;
} HRilImsRegStatusInfo;

typedef struct {
    int32_t digitMode;
    int32_t mode;
    int32_t type;
    int32_t plan;
    unsigned char number;
    unsigned char bytes[HRIL_MAX_CDMA_ADDRESS_LEN];
} HRilCdmaSmsAddress;

typedef struct {
    int32_t type;
    unsigned char odd;
    unsigned char number;
    unsigned char bytes[HRIL_MAX_CDMA_ADDRESS_LEN];
} HRilCdmaSmsSubAddress;

typedef struct {
    int32_t serviceId;
    unsigned char isExist;
    int32_t type;
    HRilCdmaSmsAddress address;
    HRilCdmaSmsSubAddress subAddress;
    int32_t size;
    unsigned char bytes[HRIL_MAX_CDMA_MESSAGE_LEN];
} HRilCdmaSmsMessageInfo;

typedef void (*HRilCallbackFun)(const uint8_t *args);

struct HRilReport {
    void (*OnCallReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnDataReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnModemReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnNetworkReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnSimReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnSmsReport)(int32_t slotId, struct ReportInfo reportInfo, const uint8_t *data, size_t dataLen);
    void (*OnTimerCallbackHandler)(HRilCallbackFun callback, const uint8_t *args, uint64_t triggerTime);
};

typedef struct {
    void (*GetCallList)(const ReqDataInfo *requestInfo);
    void (*Dial)(const ReqDataInfo *requestInfo, const HRilDial *data, size_t dataLen);
    void (*Hangup)(const ReqDataInfo *requestInfo, const uint32_t *data, size_t dataLen);
    void (*Reject)(const ReqDataInfo *requestInfo);
    void (*Answer)(const ReqDataInfo *requestInfo);
    void (*GetClip)(const ReqDataInfo *requestInfo);
    void (*SetClip)(const ReqDataInfo *requestInfo, int action);
    void (*HoldCall)(const ReqDataInfo *requestInfo);
    void (*UnHoldCall)(const ReqDataInfo *requestInfo);
    void (*SwitchCall)(const ReqDataInfo *requestInfo);
    void (*CombineConference)(const ReqDataInfo *requestInfo, int callType);
    void (*SeparateConference)(const ReqDataInfo *requestInfo, int callIndex, int callType);
    void (*CallSupplement)(const ReqDataInfo *requestInfo, int type);
    void (*GetCallWaiting)(const ReqDataInfo *requestInfo);
    void (*SetCallWaiting)(const ReqDataInfo *requestInfo, int active);
    void (*SetCallTransferInfo)(const ReqDataInfo *requestInfo, HRilCFInfo info);
    void (*GetCallTransferInfo)(const ReqDataInfo *requestInfo, int reason);
    void (*GetCallRestriction)(const ReqDataInfo *requestInfo, const char *fac);
    void (*SetCallRestriction)(const ReqDataInfo *requestInfo, CallRestrictionInfo info);
    void (*GetClir)(const ReqDataInfo *requestInfo);
    void (*SetClir)(const ReqDataInfo *requestInfo, int action);
    void (*StartDtmf)(const ReqDataInfo *requestInfo, CallDtmfInfo info);
    void (*SendDtmf)(const ReqDataInfo *requestInfo, CallDtmfInfo info);
    void (*StopDtmf)(const ReqDataInfo *requestInfo, CallDtmfInfo info);
    void (*GetImsCallList)(const ReqDataInfo *requestInfo);
    void (*GetCallPreferenceMode)(const ReqDataInfo *requestInfo);
    void (*SetCallPreferenceMode)(const ReqDataInfo *requestInfo, int mode);
    void (*GetLteImsSwitchStatus)(const ReqDataInfo *requestInfo);
    void (*SetLteImsSwitchStatus)(const ReqDataInfo *requestInfo, int active);
} HRilCallReq;

typedef struct {
    void (*ActivatePdpContext)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
    void (*DeactivatePdpContext)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
    void (*GetPdpContextList)(const ReqDataInfo *requestInfo);
} HRilDataReq;

typedef struct {
    void (*SetRadioState)(const ReqDataInfo *requestInfo, int fun, int rst);
    void (*GetRadioState)(const ReqDataInfo *requestInfo);
} HRilModemReq;

typedef struct {
    void (*GetImsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetSignalStrength)(const ReqDataInfo *requestInfo);
    void (*GetCsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetPsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetOperatorInfo)(const ReqDataInfo *requestInfo);
    void (*GetCellInfoList)(const ReqDataInfo *requestInfo);
    void (*GetCurrentCellInfo)(const ReqDataInfo *requestInfo);
    void (*GetNetworkSearchInformation)(const ReqDataInfo *requestInfo);
    void (*GetNetworkSelectionMode)(const ReqDataInfo *requestInfo);
    void (*SetNetworkSelectionMode)(const ReqDataInfo *requestInfo, const HRilSetNetworkModeInfo *data);
    void (*GetPreferredNetwork)(const ReqDataInfo *requestInfo);
    void (*SetPreferredNetwork)(const ReqDataInfo *requestInfo, const int32_t *data);
    void (*GetImei)(const ReqDataInfo *requestInfo);
    void (*SetPsAttachStatus)(const ReqDataInfo *requestInfo, const int32_t *data);
    void (*GetPsAttachStatus)(const ReqDataInfo *requestInfo);
} HRilNetworkReq;

typedef struct {
    void (*GetSimStatus)(const ReqDataInfo *requestInfo);
    void (*GetSimIO)(const ReqDataInfo *requestInfo, const HRilSimIO *data, size_t dataLen);
    void (*GetSimImsi)(const ReqDataInfo *requestInfo);
    void (*GetSimIccID)(const ReqDataInfo *requestInfo);
    void (*GetSimLockStatus)(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
    void (*SetSimLock)(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen);
    void (*ChangeSimPassword)(const ReqDataInfo *requestInfo, const HRilSimPassword *data, size_t dataLen);
    void (*UnlockPin)(const ReqDataInfo *requestInfo, const char *pin);
    void (*UnlockPuk)(const ReqDataInfo *requestInfo, const char *puk, const char *pin);
    void (*GetSimPinInputTimes)(const ReqDataInfo *requestInfo);
    void (*UnlockPin2)(const ReqDataInfo *requestInfo, const char *pin2);
    void (*UnlockPuk2)(const ReqDataInfo *requestInfo, const char *puk2, const char *pin2);
    void (*GetSimPin2InputTimes)(const ReqDataInfo *requestInfo);
    void (*SetActiveSim)(const ReqDataInfo *requestInfo, int index, int enable);
} HRilSimReq;

typedef struct {
    void (*SendGsmSms)(const ReqDataInfo *requestInfo, const char *const *data, size_t dataLen);
    void (*SendSmsAck)(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen);
    void (*SendCdmaSms)(const ReqDataInfo *requestInfo, const HRilCdmaSmsMessageInfo *data, size_t dataLen);
    void (*SendCdmaAck)(const ReqDataInfo *requestInfo, const char *data, size_t dataLen);
    void (*AddSimMessage)(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen);
    void (*DelSimMessage)(const ReqDataInfo *requestInfo, const int *data, size_t dataLen);
    void (*UpdateSimMessage)(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen);
    void (*SetSmscAddr)(const ReqDataInfo *requestInfo, const HRilServiceCenterAddress *data, size_t dataLen);
    void (*GetSmscAddr)(const ReqDataInfo *requestInfo);
    void (*SetCBConfig)(const ReqDataInfo *requestInfo, const HRilCBConfigInfo *data, size_t dataLen);
    void (*GetCBConfig)(const ReqDataInfo *requestInfo);
    void (*GetCdmaCBConfig)(const ReqDataInfo *requestInfo);
    void (*SetCdmaCBConfig)(const ReqDataInfo *requestInfo, const HRilCdmaCBConfigInfo *data, size_t dataLen);
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

typedef struct {
    int ratType;
    int mcc;
    int mnc;

    union {
        CellRatGsm gsm;
        CellRatLte lte;
        CellRatWcdma wcdma;
    } ServiceCellParas;
} CurrentCellInfoVendor;

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
    int ratType;

    union {
        CellListRatGsm gsm;
        CellListRatLte lte;
        CellListRatWcdma wcdma;
    } ServiceCellParas;
} CellInfo;

typedef struct {
    int32_t itemNum;
    CellInfo *cellNearbyInfo;
} CellInfoList;

typedef struct {
    char *longName;
    char *shortName;
    char *numeric;
    int status;
    int rat;
} AvailableOperInfo;

#endif // OHOS_HRIL_H
