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

#ifndef OHOS_RIL_TYPE_H
#define OHOS_RIL_TYPE_H

#include <locale>

#include "hril.h"

namespace OHOS {
namespace Telephony {
enum class HRilErrType : int32_t {
    /** No error */
    NONE = 0,

    /** An error that not included in bellow items */
    HRIL_ERR_GENERIC_FAILURE,

    /** Invalid parameter */
    HRIL_ERR_INVALID_PARAMETER,

    /** Full memory */
    HRIL_ERR_MEMORY_FULL,

    /** Send command failed */
    HRIL_ERR_CMD_SEND_FAILURE,

    /** NO CARRIER response returned */
    HRIL_ERR_CMD_NO_CARRIER,

    /** The response is invalid */
    HRIL_ERR_INVALID_RESPONSE,

    /** The new status of radio to set is same with previous */
    HRIL_ERR_REPEAT_STATUS,

    /** Network search error */
    HRIL_ERR_NETWORK_SEARCHING,

    /** Network search interrupted */
    HRIL_ERR_NETWORK_SEARCHING_INTERRUPTED,

    /** The AT channel is closed */
    HRIL_ERR_MODEM_DEVICE_CLOSE,

    /** No sim card error */
    HRIL_ERR_NO_SIMCARD_INSERTED,

    /** Need pin code */
    HRIL_ERR_NEED_PIN_CODE,

    /** Need puk code */
    HRIL_ERR_NEED_PUK_CODE,

    /** Network search timeout */
    HRIL_ERR_NETWORK_SEARCH_TIMEOUT,

    /** Pin or puk password is not correct */
    HRIL_ERR_PINPUK_PASSWORD_NOCORRECT,

    /** Invalid modem parameter */
    HRIL_ERR_INVALID_MODEM_PARAMETER = 50,

    /** IPC failure */
    HRIL_ERR_HDF_IPC_FAILURE = 300,

    /** Null point error */
    HRIL_ERR_NULL_POINT,

    /** Vendor not implement error. */
    HRIL_ERR_VENDOR_NOT_IMPLEMENT
};

enum class HRilNotiType { HRIL_NOTIFICATION, HRIL_NO_DEFINE };

/**
 * @brief Indicates the specific APN type.
 */
enum class HRilApnTypes : int32_t {
    /** None */
    NONE = 0,

    /** APN type for default data traffic */
    DEFAULT = 1,

    /** APN type for MMS traffic */
    MMS = 2,

    /** APN type for SUPL assisted GPS */
    SUPL = 4,

    /** APN type for DUN traffic */
    DUN = 8,

    /** APN type for HiPri traffic */
    HIPRI = 16,

    /** APN type for FOTA */
    FOTA = 32,

    /** APN type for IMS */
    IMS = 64,

    /** APN type for CBS */
    CBS = 128,

    /** APN type for IA Initial Attach APN */
    IA = 256,

    /** APN type for Emergency PDN */
    EMERGENCY = 512,

    ALL = 1023,
};

/**
 * @brief Indicates whether the hril notification need lock.
 */
enum HRilRunningLockTypes { UNNEED_LOCK, NEED_LOCK };

/**
 * @brief Indicates the hril response type.
 */
enum HRilResponseTypes {
    HRIL_RESPONSE_REQUEST,
    HRIL_RESPONSE_NOTICE,
    HRIL_RESPONSE_REQUEST_ACK,
    HRIL_RESPONSE_REQUEST_MUST_ACK,
    HRIL_RESPONSE_NOTICE_MUST_ACK,
};

/**
 * @brief From 3GPP TS 27.007 V4.3.0 (2001-12) 8.5, AT + CSQ.
 */
struct GsmRssi {
    /**
     * Received Signal Strength Indication, value range 0 ~ 31, max is 99,
     * if unknown then set to max
     */
    int32_t rxlev = 0;

    /**
     * Bit error rate, value range 0 ~ 7, max is 99, if unknown then set to
     * max as RXQUAL values in the table in TS 45.008 [20] subclauses 8.2.4.
     */
    int32_t ber = 0;
};

/**
 * @brief From 3GPP TS 27.007 V17.1.0 (2021-03) 8.69.
 */
struct CdmaRssi {
    /**
     * Absolute value of signal strength. This value is the actual Rssi value
     * multiplied by -1. e.g: Rssi is -75, then this response value will be 75.
     */
    int32_t absoluteRssi = 0;

    /**
     * integer type, ratio of the received energy per PN chip to the total
     * received power spectral density (see 3GPP TS 25.133 [95] subclauses)
     */
    int32_t ecno = 0;
};

/**
 * @brief Indicates some parameters which can reflect the strength of WCDMA signal.
 */
struct WCdmaRssi {
    /**
     * integer type, received signal strength level (see 3GPP TS 45.008 [20]
     * sub-clause 8.1.4) value 0~99.
     */
    int32_t rxlev = 0;

    /**
     * integer type, ratio of the received energy per PN chip to the total
     * received power spectral density (see 3GPP TS 25.133 [95] sub-clause).
     */
    int32_t ecio = 0;

    /**
     * integer type, received signal code power (see 3GPP TS 25.133 [95]
     * sub-clause 9.1.1.3 and 3GPP TS 25.123 [96] sub-clause 9.1.1.1.3).
     * range value 0 ~ 96, 255 not known or not detectable.
     */
    int32_t rscp = 0;

    /**
     * Bit error rate, value range 0 ~ 7, max is 99, if unknown then set to
     * max
     */
    int32_t ber = 0;
};

/**
 * @brief Indicates some parameters which can reflect the strength of LTE signal.
 */
struct LteRssi {
    /**
     * integer type, received signal strength level
     * (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99.
     */
    int32_t rxlev = 0;

    /**
     * integer type, reference signal received quality (see 3GPP TS 36.133 [96] sub-clause 9.1.7)
     * value range 0~33, 255 not known or not detectable.
     */
    int32_t rsrq = 0;

    /**
     * integer type, reference signal received power (see 3GPP TS 36.133 [96] sub-clause 9.1.4)
     * value range 0~97, 255 not known or not detectable.
     */
    int32_t rsrp = 0;

    /**
     * integer type, representing the signal-to-interference plus noise ratio, suitable for LTE mode
     * value range 0~251, 255 not known or not detectable.
     */
    int32_t snr = 0;
};

/**
 * @brief Indicates some parameters which can reflect the strength of TD-SCDMA signal.
 */
struct TdScdmaRssi {
    int32_t rscp = 0;
};

/**
 * @brief Indicates some parameters which can reflect the strength of NR signal.
 */
struct NrRssi {
    int32_t rsrp = 0;
    int32_t rsrq = 0;
    int32_t sinr = 0;
};

/**
 * @brief Indicates signal level of some RATs.
 */
struct Rssi {
    GsmRssi gw;
    CdmaRssi cdma;
    WCdmaRssi wcdma;
    LteRssi lte;
    TdScdmaRssi tdScdma;
    NrRssi nr;
};

/**
 * @brief From 3GPP TS 27.007 V4.3.0 (2001-12).
 */
struct GetCallWaitingInfo {
    /** 0: not active, 1:active */
    int32_t status = 0;

    /** <classx> default, SLM790 is 255 */
    int32_t typeClass = 0;
};

struct HRilResponseHeadInfo {
    int32_t slotId = 0;
    HRilResponseTypes type = HRilResponseTypes::HRIL_RESPONSE_REQUEST;
};

/**
 * @brief Indicates the response information, for example whether the
 * request is success, serial number, response type etc.
 */
struct HRilRadioResponseInfo {
    int32_t flag = -1;
    int32_t serial = -1;
    HRilErrType error = HRilErrType::NONE;
    HRilResponseTypes type = HRilResponseTypes::HRIL_RESPONSE_REQUEST;
};

struct HRilRadioStateInfo {
    int64_t flag = 0;
    int32_t state = 0;
};

enum HRilCommonNumber {
    HRIL_DEC = 10,
    HRIL_INVALID_HEX_CHAR = 16,
    HRIL_UPPER_CASE_LETTERS_OFFSET = 32,
    HRIL_ADAPTER_RADIO_INDICATION = 2001,
    HRIL_ADAPTER_RADIO_RESPONSE = 2002,
    HRIL_ADAPTER_RADIO_SEND_ACK,
};

enum HRilOperatorInfoResult {
    HRIL_LONE_NAME = 0,
    HRIL_SHORT_NAME,
    HRIL_NUMERIC,
};

enum HRilCircuitModeRegState {
    /** Not registered, MT is not searching an operator */
    HRIL_STAT_NO_REG_MT_NO_SEARCHING_OP = 0,

    /** Registered, home network */
    HRIL_STAT_REGISTERED_HOME_NETWORK = 1,

    /**
     * Not registered, but MT is currently trying to attach or searching an
     * operator
     */
    HRIL_STAT_NO_REGISTERED_MT_TRY_ATTACH = 2,

    /** Registration denied */
    HRIL_STAT_REGISTERED_DENIED = 3,

    /** Unknown (e.g. out of GERAN/UTRAN coverage) */
    HRIL_STAT_UNKNOWN = 4,

    /** Registered, roaming */
    HRIL_STAT_REGISTERED_ROAMING = 5,
};

enum HRilServiceSupportStat {
    HRIL_SERVICE_NO_SUPPORT = 0,
    HRIL_SERVICE_SUPPORT = 1,
};

/**
 * @brief Indicates the data denied reason, from 3GPP TS 27.007 V17.1.0 9.2.2.1.1.
 */
enum HRilReasonDataDenied {
    HREASON_GPRS_SERVICE_NOT_ALLOW = 0,
    HREASON_GPRS_AND_NON_GPRS_SERVICE_NOT_ALLOW = 1,
    HREASON_MS_IDENTITY_CANNOT_BE_DERIVED = 2,
    HREASON_IMPLICITLY_DETACHED = 3,
    HREASON_GPRS_SERVICE_NOT_ALLOW_IN_PLMN = 4,
    HREASON_MSC_TEM_NOT_REACH = 5,
    HREASON_NO_DPD_CONTEXT_ACTIVATED = 6,
};

enum HRilNetworkMode {
    HRIL_NETWORK_AUTOMATIC_MODE = 0,
    HRIL_NETWORK_MANUAL_MODE,
};

/**
 * @brief Indicates the specific IMS type.
 */
enum HRilIMSRegType {
    /** VoLTE type */
    NETWORK_IMS_REG_FEATURE_TYPE_VOLTE = 0x01,

    /** SMS over IP type */
    NETWORK_IMS_REG_FEATURE_TYPE_SMS = 0x02,

    /** RCS type */
    NETWORK_IMS_REG_FEATURE_TYPE_RCS = 0x04,

    /** VT type */
    NETWORK_IMS_REG_FEATURE_TYPE_VT = 0x08,
};

/** CS registration response */
static constexpr uint32_t HRIL_CS_REG_STATE = 1;
static constexpr uint32_t HRIL_CS_REG_RESP_ACT = 4;
static constexpr uint32_t HRIL_RSSNR = 7;
static constexpr uint32_t HRIL_CS_REG_RESP_ROAMING_INDICATOR = 10;
static constexpr uint32_t HRIL_CS_REG_RESP_SYSTEM_IS_IN_PRL = 11;
static constexpr uint32_t HRIL_CS_REG_RESP_DEFAULT_ROAMING_INDICATOR = 12;
static constexpr uint32_t HRIL_CS_REG_RESP_TIMING_ADVANCE = 13;
static constexpr uint32_t HRIL_CS_REG_STATUS_MAX_LEN = 5;

/** PS registration response */
static constexpr uint32_t HRIL_PS_REG_STATUS_MAX_LEN = 5;
static constexpr uint32_t HRIL_PS_RADIO_TECHNOLOGY = 4;
static constexpr uint32_t HRIL_PS_DENIED_ERROR_CODE = 4;
static constexpr uint32_t HRIL_PS_MAX_DATA_CALLS = 5;
static constexpr uint32_t HRIL_PS_DEF_DATA_CALLS_VAL = 1;

static constexpr uint32_t HRIL_LOCATION_REG_STATE = 1;
static constexpr uint32_t HRIL_LOCATION_LOC = 2;
static constexpr uint32_t HRIL_LOCATION_CI = 3;

/** IMS registration response */
static constexpr uint32_t HRIL_IMS_REG_STATUS_MAX_LEN = 2;
static constexpr uint32_t HRIL_IMS_REG_STATUS = 0;
static constexpr uint32_t HRIL_IMS_REG_FLAG = 1;

static constexpr uint32_t HRIL_NETWORKS_SELECT_MODE = 0;

/** Get the number of SIM cards in the system configuration */
inline const int32_t HRIL_SYSPARA_SIZE = 128;
inline constexpr const char *HRIL_DEFAULT_SLOT_COUNT = "1";
inline constexpr const char *HRIL_TEL_SIM_SLOT_COUNT = "const.telephony.slotCount";

/** Interface token */
inline const std::u16string HRIL_INTERFACE_TOKEN = u"ohos.telephony.hril";
} // namespace Telephony
} // namespace OHOS
#endif // TELEPHONY_N_TYPE_H
