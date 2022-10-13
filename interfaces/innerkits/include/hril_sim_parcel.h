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

#ifndef OHOS_RIL_SIM_PARCEL_H
#define OHOS_RIL_SIM_PARCEL_H

#include "hril_base_parcel.h"

namespace OHOS {
namespace Telephony {
struct CardStatusInfo {
    int32_t index;
    int32_t simType;
    int32_t simState;
};

/* Form 3GPP TS 27.007 V4.3.0 (2001-12) 8.18, + CRSM */
struct SimIoRequestInfo {
    int32_t command; /* command passed on by the ME to the SIM; refer GSM 51.011 [28] */
    int32_t fileId; /* this is the identifier of an elementary datafile on SIM.
                     * Mandatory for every command except STATUS */
    int32_t p1; /* parameters passed on by the MT to the SIM.
                 * These parameters are mandatory for every command,
                 * except GET RESPONSE and STATUS.
                 * The values are described in 3GPP TS 51.011 [28] */
    int32_t p2;
    int32_t p3;
    std::string data; /* information which shall be written to the SIM
                       * (hexadecimal character format; refer +CSCS). */
    std::string path; /* contains the path of an elementary file on the SIM/UICC
                       * in hexadecimal format as defined in ETSI TS 102 221 [60]
                       * (e.g. "7F205F70" in SIM and UICC case).
                       * The <pathid> shall only be used in the mode
                       * "select by path from MF" as defined in  ETSI TS 102 221 [60] */
    int32_t serial;
    std::string pin2;
    std::string aid;
};

/* Form 3GPP TS 27.007 V4.3.0 (2001-12) 8.18, + CRSM */
struct IccIoResultInfo {
    int32_t sw1; /* information from the SIM about the execution of the actual command.
                  * These parameters are delivered to the TE in both cases,
                  * on successful or failed execution of the command */
    int32_t sw2;
    std::string response;
};

struct SimLockInfo {
    int32_t serial;
    std::string fac; /* String type, which specifies the object operated by this command.
                      * "SC": SIM PIN1;
                      * "AO": prohibit all outgoing calls;
                      * "OI": All international outgoing calls are prohibited;
                      * "OX": All international outgoing calls are prohibited, except for the country of origin;
                      * "AI": All incoming calls are prohibited;
                      * "IR": When roaming outside the home area, all incoming calls are prohibited;
                      * "AB": prohibit all services (applicable only when <mode>=0);
                      * "AG": Outgoing call is prohibited (applicable only when <mode>=0);
                      * "AC": Incoming calls are prohibited (applicable only when <mode>=0);
                      * "FD": FDN;
                      * "PN": lock the network;
                      * "PU": lock subnet;
                      * "PP": Lock SP. */
    int32_t mode;
    int32_t status;
    std::string passwd;
    int32_t classx;
};

struct SimPasswordInfo {
    int32_t serial;
    std::string fac; /* String type, which specifies the object operated by this command.
                      * “SC”：SIM PIN1；
                      * “P2”：SIM PIN2；
                      * "OI": All international outgoing calls are prohibited;
                      * "OX": All international outgoing calls are prohibited, except for the country of origin;
                      * "AI": All incoming calls are prohibited;
                      * "IR": When roaming outside the home area, all incoming calls are prohibited;
                      * "AB": prohibit all services (applicable only when <mode>=0);
                      * "AG": Outgoing call is prohibited (applicable only when <mode>=0);
                      * "AC": Incoming call service is prohibited (applicable only when <mode>=0). */
    std::string oldPassword;
    std::string newPassword;
    int32_t passwordLength; /* Max length of oldPassword or newPassword */
};

struct SimPinInputTimes {
    int32_t serial;
    std::string code; /* param of string.
                       * SIM PIN2 is mean that SIM PIN2 request. SIM PUK2 is mean that SIM PUK2 request. */
    int32_t times; /* The remaining number of entries, for PIN2, the maximum number of entries is 3 times;
                    * for PUK2, the maximum number of entries is 10 times. */
    int32_t pukTimes; /* The remaining number of PUK, the maximum number of entries is 10 times. */
    int32_t pinTimes; /* The remaining number of PIN, the maximum number of entries is 3 times. */
    int32_t puk2Times; /* The remaining number of PUK2, the maximum number of entries is 10 times. */
    int32_t pin2Times; /* The remaining number of PIN2, the maximum number of entries is 3 times. */
};

struct ApduSimIORequestInfo {
    int32_t serial;
    int32_t channelId;
    int32_t type;
    int32_t instruction;
    int32_t p1;
    int32_t p2;
    int32_t p3;
    std::string data;
};

struct SimAuthenticationRequestInfo {
    int32_t serial;
    std::string aid;
    std::string authData;
};

struct OpenLogicalChannelResponse {
    int32_t sw1; /* information from the SIM about the execution of the actual command.
                  * These parameters are delivered to the TE in both cases,
                  * on successful or failed execution of the command */
    int32_t sw2;
    int32_t channelId;
    std::string response;
};

struct LockStatusResp {
    int32_t result;
    int32_t remain;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_SIM_PARCEL_H