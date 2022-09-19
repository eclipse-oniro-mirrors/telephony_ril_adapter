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

#ifndef OHOS_RIL_DATA_PARCEL_H
#define OHOS_RIL_DATA_PARCEL_H

#include "hril_base_parcel.h"

namespace OHOS {
namespace Telephony {
struct SetupDataCallResultInfo {
    int32_t flag;
    int32_t reason; /* Data call fail reason. success is 0 */
    int32_t retryTime; /* if errorCode != 0, suggested retry time */
    int32_t cid; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                  * specifies a particular PDP context definition. The parameter is local to the TE-MT interface
                  * and is used in other PDP context-related commands. */
    int32_t active; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                     * indicates the state of PDP context activation
                     * 0 - deactivated
                     * 1 - activated */
    std::string type; /* PDP_type values from 3GPP TS 27.007 V17.3.0 (2021-09)
                       * specifies the type of packet data protocol. The default value is manufacturer specific. */
    std::string netPortName; /* Network interface name */
    std::string address; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                          * <local_addr and subnet_mask>: string type;
                          * shows the IP address and subnet mask of the MT. The string is given as dot-separated
                          * numeric (0-255) parameters on the form: "a1.a2.a3.a4.m1.m2.m3.m4" for IPv4 or
                          * "a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.
                          * m1.m2.m3.m4.m5.m6.m7.m8.m9.m10.m11.m12.m13.m14.m15.m16" for IPv6.
                          * When +CGPIAF is supported, its settings can influence the format of this parameter
                          * returned with the execute form of +CGCONTRDP */
    std::string dns; /* If the MT indicates more than two IP addresses of P-CSCF servers
                      * string type; shows the IP address of the secondary DNS server.When +CGPIAF is supported, its
                      * settings can influence the format of this parameter returned with the execute form of
                      * +CGCONTRDP. */
    std::string dnsSec; /* from 3GPP TS 27.007 10.1.23 V4.3.0 (2001-12)
                         * string type; shows the IP address of the secondary DNS server.When +CGPIAF is supported,
                         * its settings can influence the format of this parameter returned with the execute form of
                         * +CGCONTRDP. */
    std::string gateway; /* network gateway address */
    int32_t maxTransferUnit; /* Maximum Transfer Unit. The range of permitted values (minimum value = 1
                              * or if the initial PDP context is supported minimum value = 0)
                              * is returned by the test form of the command. */
    std::string pCscfPrimAddr; /* from 3GPP TS 27.007 10.1.23 V4.3.0 (2001-12)
                                * string type; shows the IP address of the primary P-CSCF server.When +CGPIAF is
                                * supported, its settings can influence the format of this parameter returned with
                                * the execute form of +CGCONTRDP. */
    std::string pCscfSecAddr; /* from 3GPP TS 27.007 10.1.23 V4.3.0 (2001-12)
                               * string type; shows the IP address of the secondary P-CSCF server. When +CGPIAF is
                               * supported, its settings can influence the format of this parameter returned with the
                               * execute form of +CGCONTRDP. */
    int32_t pduSessionId; /* from 3GPP TS 27.007 10.1.23 V4.3.0 (2001-12)
                           * integer type; identifies the PDU session, see 3GPP TS 24.501 [161]. */
};

struct DataCallResultList {
    size_t size; /* Size of DataCall List */
    std::vector<SetupDataCallResultInfo> dcList; /* DataCall List */
};

/* from 3GPP TS 27.007 10.1 V4.3.0 (2001-12) */
struct DataProfileDataInfo {
    int32_t serial;
    int32_t profileId; /* Id of data profiles */
    std::string apn; /* (Access Point Name) a string parameter
                      * which is a logical name that is used to select the GGSN or the */
    std::string protocol; /* (Packet Data Protocol type) a string parameter which
                           * specifies the type of packet data protocol */
    std::string roamingProtocol;
    int32_t verType; /* APN verification type */
    std::string userName;
    std::string password;
};

struct DataProfilesInfo {
    int32_t serial;
    int32_t profilesSize;
    std::vector<DataProfileDataInfo> profiles;
    bool isRoaming;
};

struct DataCallInfo {
    int32_t serial;
    int32_t radioTechnology; /* Voice radio technology */
    DataProfileDataInfo dataProfileInfo;
    bool modemCognitive; /* 1: APN setting setDataProfile. 0: no */
    bool roamingAllowed;
    bool isRoaming;
};

struct DataLinkBandwidthInfo {
    int32_t serial;
    int32_t cid; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                  * integer type; specifies a particular QoS flow definition, Traffic Flows
                  * definition and a PDP Context definition (see the +CGDCONT and +CGDSCONT commands). */
    int32_t qi; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                 * 0 5QI is selected by network
                 * [1 - 4]	value range for guaranteed bit rate QoS flows
                 * 65, 66, 67 values for guaranteed bit rate QoS flows
                 * [71 - 76] value range for guaranteed bit rate QoS flows
                 * [5 - 9] value range for non-guaranteed bit rate QoS flows
                 * 69, 70, 79, 80 values for non-guaranteed bit rate QoS flows
                 * [82 - 85] value range for delay critical guaranteed bit rate QoS flows
                 * [128 - 254] value range for Operator-specific 5QIs */
    int32_t dlGfbr; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                     * integer type; indicates DL GFBR in case of GBR 5QI. The value is in kbit/s. This parameter is
                     * omitted for a non-GBR 5QI (see 3GPP TS 24.501 [161]) */
    int32_t ulGfbr; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                     * integer type; indicates UL GFBR in case of GBR 5QI. The value is in kbit/s. This parameter is
                     * omitted for a non-GBR 5QI (see 3GPP TS 24.501 [161]). */
    int32_t dlMfbr; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                     * integer type; indicates DL MFBR in case of GBR 5QI. The value is in kbit/s. This parameter is
                     * omitted for a non-GBR 5QI (see 3GPP TS 24.501 [161]). */
    int32_t ulMfbr; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                     * integer type; indicates UL MFBR in case of GBR 5QI. The value is in kbit/s. This parameter is
                     * omitted for a non-GBR 5QI (see 3GPP TS 24.501 [161]). */
    int32_t ulSambr; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                      * integer type; indicates the UL session AMBR(see 3GPP TS 24.501 [161]).
                      * The value is in kbit /s. */
    int32_t dlSambr; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                      * integer type; indicates the DL session AMBR(see 3GPP TS 24.501 [161]).
                      * The value is in kbit/ s. */
    int32_t averagingWindow; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                              * integer type; indicates the averaging window(see 3GPP TS 24.501 [161]) .The value is
                              * in milliseconds. */
};

struct DataLinkBandwidthReportingRule {
    int32_t serial;
    int32_t rat;
    int32_t delayMs;
    int32_t delayUplinkKbps;
    int32_t delayDownlinkKbps;
    int32_t maximumUplinkKbpsSize;
    int32_t maximumDownlinkKbpsSize;
    std::vector<int32_t> maximumUplinkKbps;
    std::vector<int32_t> maximumDownlinkKbps;
};

struct DataPerformanceInfo {
    int32_t performanceEnable; /* Data Performance Enable. eg: 1-enable, 0-disable */
    int32_t enforce; /* Whether Enforce Data Performance. eg: 1-enable, 0-disable */
};

struct DataSleepInfo {
    int32_t sleepEnable; /* Sleep Mode Enable. eg: 1-enable, 0-disable */
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_DATA_PARCEL_H
