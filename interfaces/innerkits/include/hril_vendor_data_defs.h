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

#ifndef OHOS_RIL_VENDOR_DATA_DEFS_H
#define OHOS_RIL_VENDOR_DATA_DEFS_H

#include "hril_public_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t reason; /* Data call fail reason. success is 0 */
    int32_t retryTime; /* if errorCode != 0, suggested retry time */
    int32_t cid; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                  * specifies a particular PDP context definition. The parameter is local to the TE-MT interface and
                  * is used in other PDP context-related commands. */
    int32_t active; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
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
    char *dns; /* shows the IP address of the primary DNS server. */
    char *dnsSec; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                   * string type; shows the IP address of the secondary DNS server.When +CGPIAF is supported,
                   * its settings can influence the format of this parameter returned with the execute form of
                   * +CGCONTRDP. */
    char *gateway; /* shows the Gateway Address of the MT.
                    * The string is given as dot-separated numeric (0- 255) parameters. */
    int32_t maxTransferUnit; /* Maximum Transfer Unit. shows the IPv4 MTU size in octets. */
    char *pCscfPrimAddr; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                          * string type; shows the IP address of the primary P-CSCF server.When +CGPIAF is
                          * supported, its settings can 	 influence the format of this parameter returned with
                          * the execute form of +CGCONTRDP. */
    char *pCscfSecAddr; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                         * string type; shows the IP address of the secondary P-CSCF server. When +CGPIAF is
                         * supported, its settings can influence the format of this parameter returned with the
                         * execute form of +CGCONTRDP. */
    int32_t pduSessionId; /* from 3GPP TS 27.007 V17.3.0 (2021-09)
                           * integer type; identifies the PDU session, see 3GPP TS 24.501 [161]. */
} HRilDataCallResponse;

typedef struct { /* from 3GPP TS 27.007 V17.3.0 (2021-09) */
    int32_t cid; /* specifies a particular PDP context definition. The parameter is local to the TE-MT interface and
                  * is used in other PDP context-related commands. */
    int32_t reason; /* Reason for the data request. */
    int32_t rat; /* Radio Access Technology */
    int32_t roamingEnable; /* Roaming Enable. eg: 1--enable, 0--disnable */
    int32_t verType; /* Authentication protocol used for this PDP context.
                      * 0 None. Used to indicate that no authentication protocol is used for this PDP context.
                      *    Username and password are removed if previously specified.
                      * 1 PAP
                      * 2 CHAP */
    char *userName; /* User name for access to the IP network. */
    char *password; /* Password for access to the IP network. */
    char *apn; /* (Access Point Name) a string parameter
                * which is a logical name that is used to select the GGSN or the */
    char *type; /* PDP_type values from 3GPP TS 27.007 section 10.1.1.
                 * specifies the type of packet data protocol. The default value is manufacturer specific. */
    char *roamingType; /* PDP_type values from 3GPP TS 27.007 section 10.1.1.
                        * specifies the type of packet data protocol. The default value is manufacturer specific. */
} HRilDataInfo;

typedef struct {
    int32_t cid; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                  * integer type; specifies a particular QoS flow definition, Traffic Flows
                  * definition and a PDP Context definition (see the +CGDCONT and +CGDSCONT commands). */
    int32_t qi; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                 * 0 5QI is selected by network
                 * [1 - 4] value range for guaranteed bit rate QoS flows
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
                      * The value is in kbit/s. */
    int32_t dlSambr; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                      * integer type; indicates the DL session AMBR(see 3GPP TS 24.501 [161]).
                      * The value is in kbit/s. */
    int32_t averagingWindow; /* from 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
                              * integer type; indicates the averaging window(see 3GPP TS 24.501 [161]).
                              * The value is in milliseconds. */
} HRilLinkBandwidthInfo;

typedef struct {
    RatType rat;
    int32_t delayMs; /* integer type; (0,1,2,…) indicates the targeted time between request to transfer an SDU
                      * at one SAP to its delivery at the other SAP, in milliseconds. If the parameter is set to
                      * '0' the subscribed value will be requested (refer 3GPP TS 24.008 [8] clause 10.5.6.5). */
    int32_t delayUplinkKbps;
    int32_t delayDownlinkKbps;
    int32_t maximumUplinkKbpsSize;
    int32_t maximumDownlinkKbpsSize;
    int32_t *maximumUplinkKbps;
    int32_t *maximumDownlinkKbps;
} HRilLinkBandwidthReportingRule;

typedef struct {
    int32_t performanceEnable; /* Performance Enable. eg: 1-enable, 0-disable */
    int32_t enforce; /* Whether Enforce Data Performance. eg: 1-enable, 0-disable */
} HRilDataPerformanceInfo;

typedef struct {
    int32_t sleepEnable; /* Sleep Mode Enable. eg: 1-enable, 0-disable */
} HRilDataSleepInfo;

typedef struct {
    void (*SetInitApnInfo)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
    void (*ActivatePdpContext)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
    void (*DeactivatePdpContext)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
    void (*GetPdpContextList)(const ReqDataInfo *requestInfo);
    void (*GetLinkBandwidthInfo)(const ReqDataInfo *requestInfo, int32_t cid);
    void (*SetLinkBandwidthReportingRule)(const ReqDataInfo *requestInfo, const HRilLinkBandwidthReportingRule *data);
    void (*SetDataProfileInfo)(const ReqDataInfo *requestInfo, const HRilDataInfo *data, int32_t len);
    void (*SendDataPerformanceMode)(const ReqDataInfo *requestInfo, const HRilDataPerformanceInfo *data);
    void (*SendDataSleepMode)(const ReqDataInfo *requestInfo, const HRilDataSleepInfo *data);
    void (*SetDataPermitted)(const ReqDataInfo *requestInfo, int32_t enabled);
} HRilDataReq;
#ifdef __cplusplus
}
#endif
#endif // OHOS_RIL_VENDOR_DATA_DEFS_H
