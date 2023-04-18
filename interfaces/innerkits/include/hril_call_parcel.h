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

#ifndef OHOS_RIL_CALL_PARCEL_H
#define OHOS_RIL_CALL_PARCEL_H

#include "hril_base_parcel.h"

namespace OHOS {
namespace Telephony {
/* From 3GPP TS 27.007 V4.3.0 (2001-12) 7.7, AT + ATD */
struct DialInfo {
    int32_t serial = 0;
    std::string address = "";
    int32_t clir = 0; /* Calling Line Identification Restriction. From TS 27.007 V3.4.0 (2000-03) */
};

struct CallInfo {
    int32_t index = 0;
    int32_t dir = 0;
    int32_t state = 0;
    int32_t mode = 0;
    int32_t mpty = 0;
    int32_t voiceDomain = 0;
    int32_t callType = 0;
    std::string number = "";
    int32_t type = 0;
    std::string alpha = "";
};

struct CallInfoList {
    int32_t callSize = 0;
    int32_t flag = 0;
    std::vector<CallInfo> calls {};
};

struct EmergencyInfo {
    int32_t index = 0;
    int32_t total = 0;
    std::string eccNum = "";
    int32_t category = 0;
    int32_t simpresent = 0;
    std::string mcc = "";
    int32_t abnormalService = 0;
};

struct EmergencyInfoList {
    int32_t callSize = 0;
    int32_t flag = 0;
    std::vector<EmergencyInfo> calls {};
};

struct CallForwardSetInfo {
    int32_t serial = 0;
    int32_t reason = 0;
    int32_t mode = 0;
    std::string number = "";
    int32_t classx = 0;
};

struct UssdNoticeInfo {
    int32_t m = 0; /* Integer value.
                              0: The network does not require a TE reply (USSD-Notify initiated by the network or TE
                              The network does not need further information after starting operation);
                              1: The network needs a TE reply (USSD-Request initiated by the network, or TE sent
                              After starting the operation, the network needs further information);
                              2: The USSD session is released by the network;
                              3: Other local clients have responded;
                              4: The operation is not supported;
                              5: The network timed out. */
    std::string str = ""; /* USSD string, the maximum length is 160 characters. */
};

struct SsNoticeInfo {
    int32_t serviceType = 0;
    int32_t requestType = 0;
    int32_t serviceClass = 0;
    int32_t result = 0; /* the result of the SS request */
};

/*
 * Active reporting of SRVCC status is controlled by the +CIREP command.
 * This command complies with the 3GPP TS 27.007 protocol.
 */
struct SrvccStatus {
    /*
     * SRVCC status.
     *  1: SRVCC starts;
     *   2: SRVCC is successful;
     *   3: SRVCC is cancelled;
     *   4: SRVCC failed.
     */
    int32_t status = 1;
};

/*
 * The ringback voice event reported by the modem during dialing.
 * Note: Modem private commands, not a reported field specified by the 3gpp protocol.
 */
struct RingbackVoice {
    /* 0 network alerting; 1 local alerting */
    int32_t status = 0;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_CALL_PARCEL_H
