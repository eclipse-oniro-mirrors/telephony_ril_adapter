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

typedef enum {
    HRIL_SIM_SLOT_1,
#if (SIM_SLOT_NUM >= 2) /* SIM_SLOT_NUM is defined in Makefile. */
    HRIL_SIM_SLOT_2,
#endif
    HRIL_SIM_SLOT_NUM
} HRilSimSlotId;

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
    HRIL_ERR_NETWORK_SEARCHING = 8,
    HRIL_ERR_NETWORK_SEARCHING_INTERRUPTED = 9,

    // network error
    HRIL_ERR_NO_SIMCARD_INSERTED = 10,
    HRIL_ERR_NEED_PIN_CODE = 11,
    HRIL_ERR_NEED_PUK_CODE = 12,
    HRIL_ERR_NETWORK_SEARCH_TIMEOUT = 13,
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
    HRIL_CALL_MODE_CS_ONLY = 1,
    HRIL_CALL_MODE_CS_1ST_PS_2ND,
    HRIL_CALL_MODE_PS_1ST_CS_2ND,
    HRIL_CALL_MODE_PS_ONLY,
} HRilVoiceCallMode;

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
    TYPE_HANG_UP_HOLD_WAIT = 1,
    TYPE_HANG_UP_ACTIVE = 2,
} HRilCallSupplementType;

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
    HRIL_NETWORK_AUTO,
    HRIL_NETWORK_GSM,
    HRIL_NETWORK_WCDMA,
    HRIL_NETWORK_LTE,
    HRIL_NETWORK_LTE_WCDMA,
    HRIL_NETWORK_LTE_WCDMA_GSM,
    HRIL_NETWORK_WCDMA_GSM,
} HRilPreferredNetworkType;

typedef enum { HRIL_UNKNOWN = 0, HRIL_SIM = 1, HRIL_USIM = 2 } HRilSimType;

#endif // OHOS_HRIL_ENUM_H