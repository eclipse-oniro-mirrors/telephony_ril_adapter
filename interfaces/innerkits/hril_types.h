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

#ifndef OHOS_RIL_TYPE_H
#define OHOS_RIL_TYPE_H

#include <locale>
#include "hril.h"

namespace OHOS {
namespace Telephony {
enum class HRilErrType {
    NONE = 0,
    HRIL_ERR_GENERIC_FAILURE,
    HRIL_ERR_INVALID_PARAMETER,
    HRIL_ERR_MEMORY_FULL,
    HRIL_ERR_CMD_SEND_FAILURE,
    HRIL_ERR_CMD_NO_CARRIER,
    HRIL_ERR_INVALID_RESPONSE,
};

enum class HRilNotiType { HRIL_NOTIFICATION, HRIL_NO_DEFINE };

enum class HRilApnTypes : int32_t {
    NONE = 0,
    DEFAULT = 1,
    MMS = 2,
    SUPL = 4,
    DUN = 8,
    HIPRI = 16,
    FOTA = 32,
    IMS = 64,
    CBS = 128,
    IA = 256,
    EMERGENCY = 512,
    ALL = 1023,
};

/* From 3GPP TS 27.007 V4.3.0 (2001-12) 8.5, AT + CSQ */
struct GsmRssi {
    uint32_t rxlev; /* Received Signal Strength Indication, value range 0 ~ 31, max is 99, if unknown then set to
                       max */
    uint32_t ber; /* bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max
                   * as RXQUAL values in the table in TS 45.008 [20] subclauses 8.2.4. */
};

/* From 3GPP TS 27.007 V17.1.0 (2021-03) 8.69 */
struct CdmaRssi {
    uint32_t absoluteRssi; /* Absolute value of signal strength. This value is the actual Rssi value
                            * multiplied by -1.
                            * e.g: Rssi is -75, then this response value will be 75 */
    uint32_t ecno; /* integer type, ratio of the received energy per PN chip to the total received power spectral
                    * density (see 3GPP TS 25.133 [95] subclauses) */
};

struct WCdmaRssi {
    uint32_t rxlev; /* integer type, received signal strength level
                     * (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99 */
    uint32_t ecio; /* integer type, ratio of the received energy per PN chip to the total received power
                    * spectral density (see 3GPP TS 25.133 [95] sub-clause). */
    uint32_t rscp; /* integer type, received signal code power (see 3GPP TS 25.133 [95] sub-clause 9.1.1.3 and
                    * 3GPP TS 25.123 [96] sub-clause 9.1.1.1.3).
                    * range  value 0 ~ 96, 255 not known or not detectable */
    uint32_t ber; /* bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max */
};

struct LteRssi {
    uint32_t rxlev; /* integer type, received signal strength level
                     * (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99 */
    uint32_t rsrq; /* integer type, reference signal received quality (see 3GPP TS 36.133 [96] sub-clause 9.1.7)
                    * value range 0~33, 255 not known or not detectable */
    uint32_t rsrp; /* integer type, reference signal received power (see 3GPP TS 36.133 [96] sub-clause 9.1.4)
                    * value range 0~97, 255 not known or not detectable */
    uint32_t snr; /* integer type, representing the signal-to-interference plus noise ratio, suitable for LTE mode
                   * value range 0~251, 255 not known or not detectable */
};

struct Rssi {
    int32_t slotId;
    GsmRssi gw;
    CdmaRssi cdma;
    WCdmaRssi wcdma;
    LteRssi lte;
};

/* from 3GPP TS 27.007 V4.3.0 (2001-12) */
struct GetCallWaitingInfo {
    uint32_t status; /* 0	not active
                      * 1	active */
    uint32_t typeClass; /* <classx> default, SLM790 is 255 */
};

// phone type
enum HRilPhoneNetType {
    PHONE_NET_TYPE_GSM = 1, // gsm
    PHONE_NET_TYPE_CDMA = 2, // cdma
};

struct HRilRadioResponseInfo {
    int32_t flag;
    int32_t serial;
    HRilErrType error;
};

struct HRilRadioStateInfo {
    int64_t flag;
    int32_t state;
};

enum HRilCommonNumber {
    HRIL_DEC = 10,
    HRIL_INVALID_HEX_CHAR = 16,
    HRIL_UPPER_CASE_LETTERS_OFFSET = 32,
    HRIL_ADAPTER_RADIO_INDICATION = 2001,
    HRIL_ADAPTER_RADIO_RESPONSE = 2002
};

enum HRilOperatorInfoResult {
    HRIL_NUMERIC = 0,
    HRIL_SHORT_NAME,
    HRIL_LONE_NAME,
};
namespace {
enum HRilNetWorkStatus {
    UNKNOWN,
    AVAILABLE,
    CURRENT,
    FORBIDDEN,
};
}
enum HRilCircuitModeRegState {
    HRIL_STAT_NO_REG_MT_NO_SEARCHING_OP = 0, /* not registered, MT is not searching an operator */
    HRIL_STAT_REGISTERED_HOME_NETWORK = 1, /* registered, home network */
    HRIL_STAT_NO_REGISTERED_MT_TRY_ATTACH = 2, /* not registered, but MT is currently trying
                                                * to attach or searching an operator */
    HRIL_STAT_REGISTERED_DENIED = 3, /* registration denied */
    HRIL_STAT_UNKNOWN = 4, /* unknown (e.g. out of GERAN/UTRAN coverage) */
    HRIL_STAT_REGISTERED_ROAMING = 5, /* registered, roaming */
};

enum HRilServiceSupportStat {
    HRIL_SERVICE_NO_SUPPORT = 0,
    HRIL_SERVICE_SUPPORT = 1,
};

enum HRiRadioTechnology {
    HRIL_RADIO_GSM = 0,
    HRIL_RADIO_GSM_COMPACT = 1,
    HRIL_RADIO_UTRAN = 2,
    HRIL_RADIO_EGPRS = 3,
    HRIL_RADIO_HSDPA = 4,
    HRIL_RADIO_HSUPA = 5,
    HRIL_RADIO_HSDPA_HSUPA = 6,
    HRIL_RADIO_EUTRAN = 7,
};

/* from 3GPP TS 27.007 V17.1.0 9.2.2.1.1 */
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

// cs registration response
static constexpr uint32_t HRIL_CS_REG_STATE = 1;
static constexpr uint32_t HRIL_CS_REG_RESP_ACT = 4;
static constexpr uint32_t HRIL_RSSNR = 7;
static constexpr uint32_t HRIL_CS_REG_RESP_ROAMING_INDICATOR = 10;
static constexpr uint32_t HRIL_CS_REG_RESP_SYSTEM_IS_IN_PRL = 11;
static constexpr uint32_t HRIL_CS_REG_RESP_DEFAULT_ROAMING_INDICATOR = 12;
static constexpr uint32_t HRIL_CS_REG_RESP_TIMING_ADVANCE = 13;
static constexpr uint32_t HRIL_CS_REG_STATUS_MAX_LEN = 5;
// ps registration response
static constexpr uint32_t HRIL_PS_REG_STATUS_MAX_LEN = 5;
static constexpr uint32_t HRIL_PS_REG_STATE = 1;
static constexpr uint32_t HRIL_PS_RADIO_TECHNOLOGY = 4;
static constexpr uint32_t HRIL_PS_DENIED_ERROR_CODE = 4;
static constexpr uint32_t HRIL_PS_MAX_DATA_CALLS = 5;
static constexpr uint32_t HRIL_PS_DEF_DATA_CALLS_VAL = 1;

static constexpr uint32_t HRIL_LOCATION_REG_STATE = 1;
static constexpr uint32_t HRIL_LOCATION_LOC = 2;
static constexpr uint32_t HRIL_LOCATION_CI = 3;

static constexpr uint32_t HRIL_NETWORKS_SELECT_MODE = 0;
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_TYPE_H