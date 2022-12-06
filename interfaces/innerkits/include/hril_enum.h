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

#ifndef OHOS_HRIL_ENUM_H
#define OHOS_HRIL_ENUM_H

#define HRIL_VERSION 1
#define HRIL_VERSION_MIN 1
#define HRIL_MAX_CDMA_MESSAGE_LEN 255
#define HRIL_MAX_CDMA_ADDRESS_LEN 36

#define TRUE 1
#define FALSE 0

#define PARAMETER_SIZE 128

typedef enum {
    HRIL_SIM_SLOT_0,
    HRIL_SIM_SLOT_1,
    HRIL_SIM_SLOT_2,
    HRIL_SIM_SLOT_3,
} HRilSimSlotId;

typedef enum {
    HRIL_ERR_NULL_POINT = -1,
    HRIL_ERR_SUCCESS = 0,
    HRIL_ERR_GENERIC_FAILURE,
    HRIL_ERR_INVALID_PARAMETER,
    HRIL_ERR_MEMORY_FULL,
    HRIL_ERR_CMD_SEND_FAILURE,
    HRIL_ERR_CMD_NO_CARRIER,
    HRIL_ERR_INVALID_RESPONSE,
    HRIL_ERR_REPEAT_STATUS,
    HRIL_ERR_NETWORK_SEARCHING,
    HRIL_ERR_NETWORK_SEARCHING_INTERRUPTED,
    HRIL_ERR_MODEM_DEVICE_CLOSE,

    // network error
    HRIL_ERR_NO_SIMCARD_INSERTED,
    HRIL_ERR_NEED_PIN_CODE,
    HRIL_ERR_NEED_PUK_CODE,
    HRIL_ERR_NETWORK_SEARCH_TIMEOUT,
    HRIL_ERR_PINPUK_PASSWORD_NOCORRECT,
    HRIL_ERR_INVALID_MODEM_PARAMETER = 50,
    HRIL_ERR_HDF_IPC_FAILURE = 65535,
} HRilErrNumber;

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
    HRIL_CALL_FAIL_NO_VALID_NUMBER = 1,
    HRIL_CALL_FAIL_NO_LINE_TO_TARGET_ADDRESS = 3,
    HRIL_ERR_CALL_CAUSE = 5
} HRilLastCallErrorCode;

typedef enum {
    HRIL_CALL_MODE_CS_ONLY = 1,
    HRIL_CALL_MODE_CS_1ST_PS_2ND,
    HRIL_CALL_MODE_PS_1ST_CS_2ND,
    HRIL_CALL_MODE_PS_ONLY,
} HRilVoiceCallMode;

typedef enum {
    HRIL_RADIO_POWER_STATE_UNAVAILABLE = -1,
    HRIL_RADIO_POWER_STATE_OFF = 0,
    HRIL_RADIO_POWER_STATE_ON = 1,
} HRilRadioState;

typedef enum { HRIL_CALL_VOICE, HRIL_CALL_DATA, HRIL_CALL_FAX } HRilCallMode;

typedef enum {
    NO_REG_MT_NO_SEARCH = 0,
    REG_MT_HOME = 1,
    NO_REG_MT_SEARCHING = 2,
    REG_MT_REJECTED = 3,
    REG_MT_UNKNOWN = 4,
    REG_MT_ROAMING = 5,
    REG_MT_EMERGENCY = 6,
} HRilRegStatus;

typedef enum {
    REG_NOT_NOTIFY = 0, /* AT command: +CREG,+CGREG,+CEREG,+C5GREG,n=0,Turn off notify function */
    REG_NOTIFY_STAT_ONLY, /* AT command: +CREG,+CGREG,+CEREG,+C5GREG,n=1,notify data format type 1 */
    REG_NOTIFY_STAT_LAC_CELLID, /* AT command: +CREG,+CGREG,+CEREG,+C5GREG,n=2,notify data format type 2 */
} HRilRegNotifyMode;

// 3GPP TS 27.007 V3.9.0 (2001-06) 7.11 Call forwarding number and conditions +CCFC
typedef enum {
    CALL_FORWARD_REASON_UNCONDITIONAL,
    CALL_FORWARD_REASON_MOBILE_BUSY,
    CALL_FORWARD_REASON_NO_REPLY,
    CALL_FORWARD_REASON_NO_REACHABLE,
    CALL_FORWARD_REASON_ALL_CF, /* all call forwarding (refer 3GPP TS 22.030 [19]) */
    CALL_FORWARD_REASON_ALL_CCF, /* all conditional call forwarding */
} CallForwardReasonType;

// 3GPP TS 27.007 V3.9.0 (2001-06) 7.11 Call forwarding number and conditions +CCFC
typedef enum {
    CALL_FORWARD_CLASS_X_VOICE = 1,
    CALL_FORWARD_CLASS_X_DATA = 2,
    CALL_FORWARD_CLASS_X_FAX = 4,
    CALL_FORWARD_CLASS_X_SMS = 8,     /* short message service */
    CALL_FORWARD_CLASS_X_DCS = 16,    /* data circuit sync */
    CALL_FORWARD_CLASS_X_DCAS = 32,   /* dedicated packet access */
    CALL_FORWARD_CLASS_X_DPA = 64,    /* all conditional call forwarding */
    CALL_FORWARD_CLASS_X_DPADA = 128, /* dedicated PAD access */
} CallForwardClassX;

// 3GPP TS 27.007 V3.9.0 (2001-06) 7.11 Call forwarding number and conditions +CCFC
typedef enum {
    CALL_FORWARD_MODE_DISABLE,
    CALL_FORWARD_MODE_ENABLE,
    CALL_FORWARD_MODE_QUERY_STATUS,
    CALL_FORWARD_MODE_REGISTRATION,
    CALL_FORWARD_MODE_ERASURE,
} CallForwardMode;

// 3GPP TS 27.007 V3.9.0 (2001-06) Call related supplementary services +CHLD
// 3GPP TS 27.007 V3.9.0 (2001-06) 7.22 Informative examples
typedef enum {
    TYPE_HANG_UP_HOLD_WAIT = 1,
    TYPE_HANG_UP_ACTIVE = 2,
} HRilCallSupplementType;

typedef enum {
    HRIL_RESPONSE = 0,
    HRIL_NOTIFICATION = 1,
} ReportType;

typedef enum {
    HRIL_UNLOCK_SUCCESS = 0,
    HRIL_UNLOCK_PASSWORD_ERR = 1,
    HRIL_UNLOCK_OTHER_ERR = 2,
} PinPukResultType;

typedef enum {
    HRIL_REPORT_ERR_TYPE_NONE = 0,
    HRIL_REPORT_ERR_TYPE_GENERIC,
    HRIL_REPORT_ERR_TYPE_CME,
    HRIL_REPORT_ERR_TYPE_CMS,
} ReportErrorType;

typedef enum {
    NETWORK_TYPE_UNKNOWN = 0, /* indicates no cell information */
    NETWORK_TYPE_GSM,
    NETWORK_TYPE_CDMA,
    NETWORK_TYPE_WCDMA,
    NETWORK_TYPE_TDSCDMA,
    NETWORK_TYPE_LTE,
    NETWORK_TYPE_NR
} RatType; /* Radio Access Technology  */

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

typedef enum {
    HRIL_NETWORK_AUTO = 0,
    HRIL_NETWORK_GSM = 1,
    HRIL_NETWORK_WCDMA = 2,
    HRIL_NETWORK_LTE = 3,
    HRIL_NETWORK_LTE_WCDMA = 4,
    HRIL_NETWORK_LTE_WCDMA_GSM = 5,
    HRIL_NETWORK_WCDMA_GSM = 6,
    HRIL_NETWORK_CDMA = 7,
    HRIL_NETWORK_EVDO = 8,
    HRIL_NETWORK_EVDO_CDMA = 9,
    HRIL_NETWORK_WCDMA_GSM_EVDO_CDMA = 10,
    HRIL_NETWORK_LTE_EVDO_CDMA = 11,
    HRIL_NETWORK_LTE_WCDMA_GSM_EVDO_CDMA = 12,
    HRIL_NETWORK_TDSCDMA = 13,
    HRIL_NETWORK_TDSCDMA_GSM = 14,
    HRIL_NETWORK_TDSCDMA_WCDMA = 15,
    HRIL_NETWORK_TDSCDMA_WCDMA_GSM = 16,
    HRIL_NETWORK_LTE_TDSCDMA = 17,
    HRIL_NETWORK_LTE_TDSCDMA_GSM = 18,
    HRIL_NETWORK_LTE_TDSCDMA_WCDMA = 19,
    HRIL_NETWORK_LTE_TDSCDMA_WCDMA_GSM = 20,
    HRIL_NETWORK_TDSCDMA_WCDMA_GSM_EVDO_CDMA = 21,
    HRIL_NETWORK_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA = 22,
    HRIL_NETWORK_NR = 31,
    HRIL_NETWORK_NR_LTE = 32,
    HRIL_NETWORK_NR_LTE_WCDMA = 33,
    HRIL_NETWORK_NR_LTE_WCDMA_GSM = 34,
    HRIL_NETWORK_NR_LTE_EVDO_CDMA = 35,
    HRIL_NETWORK_NR_LTE_WCDMA_GSM_EVDO_CDMA = 36,
    HRIL_NETWORK_NR_LTE_TDSCDMA = 37,
    HRIL_NETWORK_NR_LTE_TDSCDMA_GSM = 38,
    HRIL_NETWORK_NR_LTE_TDSCDMA_WCDMA = 39,
    HRIL_NETWORK_NR_LTE_TDSCDMA_WCDMA_GSM = 40,
    HRIL_NETWORK_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA = 41,
} HRilPreferredNetworkType;

typedef enum {
    HRIL_USIM_INVALID = 0,
    HRIL_USIM_VALID = 1,
    HRIL_USIM_CS_INVALID = 2,
    HRIL_USIM_PS_INVALID = 3,
    HRIL_USIM_CS_PS_INVALID = 4,
    HRIL_ROM_SIM = 240,
    HRIL_NO_USIM = 255,
} HRilSimStatus;

typedef enum {
    HRIL_SIM_CARD_UNLOCK = 0,
    HRIL_SIM_CARD_LOCK = 1,
} HRilSimLockStatus;

typedef enum {
    HRIL_NO_ROAM = 0,
    HRIL_ROAMING = 1,
    HRIL_ROAM_UNKNOWN = 2,
} HRilRoamStatus;

typedef enum {
    HRIL_NO_SRV_SERVICE = 0,
    HRIL_RESTRICTED_SERVICE = 1,
    HRIL_SERVICE_VALID = 2,
    HRIL_REGIONAL_SERVICE = 3,
    HRIL_ENERGY_SAVING_SERVICE = 4,
} HRilSrvStatus;

typedef enum {
    HRIL_NO_DOMAIN_SERVICE = 0,
    HRIL_CS_SERVICE = 1,
    HRIL_PS_SERVICE = 2,
    HRIL_CS_PS_SERVICE = 3,
    HRIL_CS_PS_SEARCHING = 4,
    HRIL_CDMA_NOT_SUPPORT = 255,
} HRilSrvDomain;

typedef enum {
    HRIL_NO_SYSMODE_SERVICE = 0,
    HRIL_GSM_MODE = 1,
    HRIL_CDMA_MODE = 2,
    HRIL_WCDMA_MODE = 3,
    HRIL_TDSCDMA_MODE = 4,
    HRIL_WIMAX_MODE = 5,
    HRIL_LTE_MODE = 6,
    HRIL_LTE_CA_MODE = 7,
    HRIL_NR_MODE = 8,
} HRilSysMode;

typedef enum {
    HRIL_ACT_NO_SERVICE = 0,
    HRIL_ACT_GSM = 1,
    HRIL_ACT_GPRS = 2,
    HRIL_ACT_EDGE = 3,
    HRIL_ACT_IS95A = 21,
    HRIL_ACT_IS95B = 22,
    HRIL_ACT_CDMA2000_1X = 23,
    HRIL_ACT_EVDO_REL0 = 24,
    HRIL_ACT_EVDO_RELA = 25,
    HRIL_ACT_EVDO_RELB = 26,
    HRIL_ACT_HYBRID_CDMA2000_1X = 27,
    HRIL_ACT_HYBRID_EVDO_REL0 = 28,
    HRIL_ACT_HYBRID_EVDO_RELA = 29,
    HRIL_ACT_HYBRID_EVDO_RELB = 30,
    HRIL_ACT_WCDMA = 41,
    HRIL_ACT_HSDPA = 42,
    HRIL_ACT_HSUPA = 43,
    HRIL_ACT_HSPA = 44,
    HRIL_ACT_HSPAP = 45,
    HRIL_ACT_DC_HSPAP = 46,
    HRIL_ACT_TDSCDMA = 61,
    HRIL_ACT_802_16E = 81,
    HRIL_ACT_LTE = 101,
    HRIL_ACT_LTE_CA,
    HRIL_ACT_NR,
} HRilVoiceSubMode;

typedef enum {
    RADIO_TECHNOLOGY_UNKNOWN = 0,
    RADIO_TECHNOLOGY_GSM = 1,
    RADIO_TECHNOLOGY_1XRTT = 2,
    RADIO_TECHNOLOGY_WCDMA = 3,
    RADIO_TECHNOLOGY_HSPA = 4,
    RADIO_TECHNOLOGY_HSPAP = 5,
    RADIO_TECHNOLOGY_TD_SCDMA = 6,
    RADIO_TECHNOLOGY_EVDO = 7,
    RADIO_TECHNOLOGY_EHRPD = 8,
    RADIO_TECHNOLOGY_LTE = 9,
    RADIO_TECHNOLOGY_LTE_CA = 10,
    RADIO_TECHNOLOGY_IWLAN = 11,
    RADIO_TECHNOLOGY_NR = 12,
    RADIO_TECHNOLOGY_INVALID = 65535,
} HRilRadioTech;

/* reference 3GPP TS 24.008 V17.4.0 (2021-09)
 * Unsuccessful PDP context activation initiated by the MS */
typedef enum {
    HRIL_PDP_ERR_NONE = 0,
    HRIL_PDP_ERR_RETRY,
    HRIL_PDP_ERR_UNKNOWN = 65535, /* Unknown error */
    HRIL_PDP_ERR_OPERATOR_DETERMINED_BARRING = 8, /* Operator Determined Barring---No need to retry */
    HRIL_PDP_ERR_SHORTAGE_RESOURCES = 26, /* insufficient resources */
    HRIL_PDP_ERR_MISSING_OR_UNKNOWN_APN, /* missing or unknown APN---No need to retry */
    HRIL_PDP_ERR_UNKNOWN_PDP_ADDR_OR_TYPE, /* unknown PDP address or PDP type---No need to retry */
    HRIL_PDP_ERR_USER_VERIFICATION, /* user authentication failed---No need to retry */
    HRIL_PDP_ERR_ACTIVATION_REJECTED_GGSN, /* activation rejected by GGSN, Serving GW or PDN GW---No need to retry */
    HRIL_PDP_ERR_ACTIVATION_REJECTED_UNSPECIFIED, /* activation rejected, unspecified */
    HRIL_PDP_ERR_SERVICE_OPTION_NOT_SUPPORTED, /* service option not supported---No need to retry */
    HRIL_PDP_ERR_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED, /* requested service option not subscribed
                                                           * ---No need to retry */
    HRIL_PDP_ERR_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER, /* service option temporarily out of order */
    HRIL_PDP_ERR_NSAPI_ALREADY_USED, /* NSAPI already used---No need to retry */
    HRIL_PDP_ERR_IPV4_ONLY_ALLOWED = 50, /* PDP type IPv4 only allowed---No need to retry */
    HRIL_PDP_ERR_IPV6_ONLY_ALLOWED, /* PDP type IPv6 only allowed---No need to retry */
    HRIL_PDP_ERR_IPV4V6_ONLY_ALLOWED = 57, /* PDP type IPv4v6 only allowed---No need to retry */
    HRIL_PDP_ERR_NON_IP_ONLY_ALLOWED, /* PDP type non IP only allowed---No need to retry */
    HRIL_PDP_ERR_MAX_NUM_OF_PDP_CONTEXTS = 65, /* maximum number of PDP contexts reached---No need to retry */
    HRIL_PDP_ERR_APN_NOT_SUPPORTED_IN_CURRENT_RAT_PLMN, /* requested APN not supported in current RAT
                                                         * and PLMN combination */
    HRIL_PDP_ERR_PROTOCOL_ERRORS = 95, /* #95 - 111: protocol errors---No need to retry */
    HRIL_PDP_ERR_APN_RESTRICTION_VALUE_INCOMPATIBLE = 112, /* APN restriction value incompatible
                                                            * with active PDP context */
    HRIL_PDP_ERR_MULT_ACCESSES_PDN_NOT_ALLOWED = 113, /* Multiple accesses to a PDN connection not allowed */
} HRilPdpErrorReason;

typedef enum {
    HRIL_EVENT_COUNT_0 = 0,
    HRIL_EVENT_COUNT_1,
    HRIL_EVENT_COUNT_2,
    HRIL_EVENT_COUNT_3,
    HRIL_EVENT_COUNT_4
} HRilSendEventCount;

typedef enum { HRIL_SIM_TYPE_UNKNOWN = 0, HRIL_SIM_TYPE_SIM, HRIL_SIM_TYPE_USIM } HRilSimType;

typedef enum { HRIL_UNNEED_ACK = 0, HRIL_NEED_ACK } HRilAckTypes;

typedef enum {
    NOTIFICATION_FILTER_ALL = -1,
    NOTIFICATION_FILTER_NONE = 0,
    NOTIFICATION_FILTER_SIGNAL_STRENGTH = 1,
    NOTIFICATION_FILTER_NETWORK_STATE = 2,
    NOTIFICATION_FILTER_DATA_CALL = 4,
    NOTIFICATION_FILTER_LINK_CAPACITY = 8,
    NOTIFICATION_FILTER_PHYSICAL_CHANNEL_CONFIG = 16
} HRilNotificationFilter;

typedef enum {
    POWER_SAVE_MODE,
    CHARGING_STATE,
    LOW_DATA_STATE
} HRilDeviceStateType;

typedef enum {
    HRIL_SERVING_CELL_UNKNOWN,
    HRIL_SERVING_CELL_PRIMARY,
    HRIL_SERVING_CELL_SECONDARY
} HRilCellConnectionStatus;

typedef enum {
    HRIL_RADIO_PROTOCOL_PHASE_INITIAL, /* LM is configured is initial value and value after COMPLETE. */
    HRIL_RADIO_PROTOCOL_PHASE_CHECK, /* CHECK is sent before UPDATE and indicates that an UPDATE will be
                                      * forthcoming with these same parameters. */
    HRIL_RADIO_PROTOCOL_PHASE_UPDATE, /* UPDATE is sent after all LM's receive CHECK and returned success,
                                       * if any CHECK's fail no UPDATE will be sent. */
    HRIL_RADIO_PROTOCOL_PHASE_NOTIFY, /* NOTIFY is sent with HNOTI_SIM_RADIO_PROTOCOL_UPDATED. */
    HRIL_RADIO_PROTOCOL_PHASE_COMPLETE /* COMPLETE is sent after all previous phases have completed.
                                        * If an error occurs in any previous commands the
                                        * and fields will be the prior configuration thus
                                        * restoring the configuration to the previous value. An error returned
                                        * by this command will generally be ignored or may cause that logical
                                        * modem to be removed from service */
} HRilRadioProtocolPhase;

typedef enum {
    HRIL_RADIO_PROTOCOL_TECH_UNKNOWN = 1 << RADIO_TECHNOLOGY_UNKNOWN,
    HRIL_RADIO_PROTOCOL_TECH_GSM = 1 << RADIO_TECHNOLOGY_GSM,
    HRIL_RADIO_PROTOCOL_TECH_1XRTT = 1 << RADIO_TECHNOLOGY_1XRTT,
    HRIL_RADIO_PROTOCOL_TECH_WCDMA = 1 << RADIO_TECHNOLOGY_WCDMA,
    HRIL_RADIO_PROTOCOL_TECH_HSPA = 1 << RADIO_TECHNOLOGY_HSPA,
    HRIL_RADIO_PROTOCOL_TECH_HSPAP = 1 << RADIO_TECHNOLOGY_HSPAP,
    HRIL_RADIO_PROTOCOL_TECH_TD_SCDMA = 1 << RADIO_TECHNOLOGY_TD_SCDMA,
    HRIL_RADIO_PROTOCOL_TECH_EVDO = 1 << RADIO_TECHNOLOGY_EVDO,
    HRIL_RADIO_PROTOCOL_TECH_EHRPD = 1 << RADIO_TECHNOLOGY_EHRPD,
    HRIL_RADIO_PROTOCOL_TECH_LTE = 1 << RADIO_TECHNOLOGY_LTE,
    HRIL_RADIO_PROTOCOL_TECH_LTE_CA = 1 << RADIO_TECHNOLOGY_LTE_CA,
    HRIL_RADIO_PROTOCOL_TECH_IWLAN = 1 << RADIO_TECHNOLOGY_IWLAN,
    HRIL_RADIO_PROTOCOL_TECH_NR = 1 << RADIO_TECHNOLOGY_NR
} HRilRadioProtocolTech;

typedef enum {
    HRIL_RADIO_PROTOCOL_STATUS_NONE,
    HRIL_RADIO_PROTOCOL_STATUS_SUCCESS,
    HRIL_RADIO_PROTOCOL_STATUS_FAIL
} HRilRadioProtocolStatus;

#endif // OHOS_HRIL_ENUM_H
