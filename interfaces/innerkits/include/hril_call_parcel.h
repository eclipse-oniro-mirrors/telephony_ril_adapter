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

#ifndef OHOS_RIL_CALL_PARCEL_H
#define OHOS_RIL_CALL_PARCEL_H

#include "hril_base_parcel.h"

namespace OHOS {
namespace Telephony {
/* From 3GPP TS 27.007 V4.3.0 (2001-12) 7.7, AT + ATD */
struct DialInfo {
    int32_t serial;
    std::string address;
    int32_t clir; /* Calling Line Identification Restriction. From TS 27.007 V3.4.0 (2000-03) */
};

struct GetClipResult {
    int32_t result; /* query results */
    int32_t action; /* parameter sets/shows the result code presentation status in the TA */
    int32_t clipStat; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
};

struct GetClirResult {
    int32_t result; /* query results */
    int32_t action; /* parameter sets/shows the result code presentation status in the TA */
    int32_t clirStat; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
};

struct CallWaitResult {
    int32_t result; /* query results */
    int32_t status; /* parameter sets/shows the result code presentation status in the TA */
    int32_t classCw; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
};

struct CallRestrictionResult {
    int32_t result; /* query results */
    int32_t status; /* parameter sets/shows the result code presentation status in the TA */
    int32_t classCw; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
};

struct CallInfo {
    int32_t index;
    int32_t dir;
    int32_t state;
    int32_t mode;
    int32_t mpty;
    int32_t voiceDomain;
    int32_t callType;
    std::string number;
    int32_t type;
    std::string alpha;
};

struct CallInfoList {
    int32_t callSize;
    int32_t flag;
    std::vector<CallInfo> calls;
};

struct EmergencyInfo {
    int32_t index;
    int32_t total;
    std::string eccNum;
    int32_t category;
    int32_t simpresent;
    std::string mcc;
    int32_t abnormalService;
};

struct EmergencyInfoList {
    int32_t callSize;
    int32_t flag;
    std::vector<EmergencyInfo> calls;
};

struct CallForwardSetInfo {
    int32_t serial;
    int32_t reason;
    int32_t mode;
    std::string number;
    int32_t classx;
};

struct CallForwardQueryResult {
    int32_t serial;
    int32_t result; /* query results */
    int32_t status;
    int32_t classx;
    std::string number;
    int32_t type;
    int32_t reason;
    int32_t time;
};

struct CallForwardQueryInfoList {
    int32_t callSize;
    int32_t flag;
    std::vector<CallForwardQueryResult> calls;
};

struct UssdNoticeInfo {
    int32_t m; /* Integer value.
                            0: The network does not require a TE reply (USSD-Notify initiated by the network or TE
                            The network does not need further information after starting operation);
                            1: The network needs a TE reply (USSD-Request initiated by the network, or TE sent
                            After starting the operation, the network needs further information);
                            2: The USSD session is released by the network;
                            3: Other local clients have responded;
                            4: The operation is not supported;
                            5: The network timed out. */
    std::string str; /* USSD string, the maximum length is 160 characters. */
};

struct SsNoticeInfo {
    int32_t serviceType;
    int32_t requestType;
    int32_t serviceClass;
    int32_t result; /* the result of the SS request */
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
    int32_t status;
};

/*
 * The ringback voice event reported by the modem during dialing.
 * Note: Modem private commands, not a reported field specified by the 3gpp protocol.
 */
struct RingbackVoice {
    /* 0 network alerting; 1 local alerting */
    int32_t status;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_CALL_PARCEL_H
