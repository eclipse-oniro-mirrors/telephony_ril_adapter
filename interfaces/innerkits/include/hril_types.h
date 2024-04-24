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
 * @brief From 3GPP TS 27.007 V4.3.0 (2001-12).
 */
struct GetCallWaitingInfo {
    /** 0: not active, 1:active */
    int32_t status = 0;

    /** <classx> default, SLM790 is 255 */
    int32_t typeClass = 0;
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
inline constexpr const char *HRIL_DEFAULT_VSIM_MODEM_COUNT = "0";
inline constexpr const char *HRIL_VSIM_MODEM_COUNT_STR = "const.telephony.vsimModemCount";

/** Interface token */
inline const std::u16string HRIL_INTERFACE_TOKEN = u"ohos.telephony.hril";
} // namespace Telephony
} // namespace OHOS
#endif // TELEPHONY_N_TYPE_H
