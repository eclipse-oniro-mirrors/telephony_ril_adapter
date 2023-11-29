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

/**
 * @brief Defines the data call response.
 */
typedef struct {
    /** Data call fail reason. success is 0 */
    int32_t reason;

    /** If errorCode != 0, suggested retry time */
    int32_t retryTime;

    /**
     * From 3GPP TS 27.007 V17.3.0 (2021-09)
     * Specifies a particular PDP context definition. The parameter is local to
     * the TE-MT interface and is used in other PDP context-related commands.
     */
    int32_t cid;

    /**
     * From 3GPP TS 27.007 V17.3.0 (2021-09)
     * Indicates the state of PDP context activation
     * 0: deactivated
     * 1: activated
     */
    int32_t active;

    /**
     * PDP_type values from 3GPP TS 27.007 V17.3.0 (2021-09)
     * Specifies the type of packet data protocol. The default value is
     * manufacturer specific.
     */
    char *type;

    /**
     * Network interface name
     */
    char *netPortName;

    /**
     * From 3GPP TS 27.007 V17.3.0 (2021-09)
     * <local_addr and subnet_mask>: string type;
     * shows the IP address and subnet mask of the MT. The string is given as
     * dot-separated numeric (0-255) parameters on the form:
     * "a1.a2.a3.a4.m1.m2.m3.m4" for IPv4 or
     * "a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.
     * m1.m2.m3.m4.m5.m6.m7.m8.m9.m10.m11.m12.m13.m14.m15.m16" for IPv6.
     * When +CGPIAF is supported, its settings can influence the format of this
     * parameter returned with the execute form of +CGCONTRDP
     */
    char *address;

    /** Shows the IP address of the primary DNS server. */
    char *dns;

    /**
     * From 3GPP TS 27.007 V17.3.0 (2021-09)
     * string type; shows the IP address of the secondary DNS server.When
     * +CGPIAF is supported, its settings can influence the format of this
     * parameter returned with the execute form of +CGCONTRDP.
     */
    char *dnsSec;

    /**
     * Shows the Gateway Address of the MT. The string is given as
     * dot-separated numeric (0- 255) parameters.
     */
    char *gateway;

    /** Maximum Transfer Unit. shows the IPv4 MTU size in octets. */
    int32_t maxTransferUnit;

    /**
     * From 3GPP TS 27.007 V17.3.0 (2021-09)
     * string type; shows the IP address of the primary P-CSCF server.When
     * +CGPIAF is supported, its settings can 	 influence the format of this
     * parameter returned with the execute form of +CGCONTRDP.
     */
    char *pCscfPrimAddr;

    /**
     * From 3GPP TS 27.007 V17.3.0 (2021-09)
     * string type; shows the IP address of the secondary P-CSCF server. When
     * +CGPIAF is supported, its settings can influence the format of this
     * parameter returned with the execute form of +CGCONTRDP.
     */
    char *pCscfSecAddr;

    /**
     * From 3GPP TS 27.007 V17.3.0 (2021-09)
     * integer type; identifies the PDU session, see 3GPP TS 24.501 [161].
     */
    int32_t pduSessionId;
} HRilDataCallResponse;

/**
 * @brief Defines the data information. from 3GPP TS 27.007 V17.3.0 (2021-09)
 */
typedef struct {
    /**
     * Specifies a particular PDP context definition. The parameter is local to
     * the TE-MT interface and is used in other PDP context-related commands.
     */
    int32_t cid;

    /** Reason for the data request. */
    int32_t reason;

    /** Radio Access Technology */
    int32_t rat;

    /** Roaming Enable. eg: 1--enable, 0--disnable */
    int32_t roamingEnable;

    /**
     * Authentication protocol used for this PDP context.
     * 0: None. Used to indicate that no authentication protocol is used for this
     *    PDP context. Username and password are removed if previously specified.
     * 1: PAP
     * 2: CHAP
     */
    int32_t verType;

    /** User name for access to the IP network. */
    char *userName;

    /** Password for access to the IP network. */
    char *password;

    /** Access Point Name */
    char *apn;

    /**
     * PDP_type values from 3GPP TS 27.007 section 10.1.1.
     * Specifies the type of packet data protocol. The default value is
     * manufacturer specific.
     */
    char *type;

    /**
     * PDP_type values from 3GPP TS 27.007 section 10.1.1.
     * Specifies the type of packet data protocol. The default value is
     * manufacturer specific.
     */
    char *roamingType;
} HRilDataInfo;

/**
 * @brief Defines the band width information.
 */
typedef struct {
    /**
     * From 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
     * integer type; specifies a particular QoS flow definition, Traffic Flows
     * definition and a PDP Context definition (see the +CGDCONT and +CGDSCONT
     * commands).
     */
    int32_t cid;

    /**
     * From 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
     * 0 5QI is selected by network
     * [1 - 4] value range for guaranteed bit rate QoS flows
     * 65, 66, 67 values for guaranteed bit rate QoS flows
     * [71 - 76] value range for guaranteed bit rate QoS flows
     * [5 - 9] value range for non-guaranteed bit rate QoS flows
     * 69, 70, 79, 80 values for non-guaranteed bit rate QoS flows
     * [82 - 85] value range for delay critical guaranteed bit rate QoS flows
     * [128 - 254] value range for Operator-specific 5QIs
     */
    int32_t qi;

    /**
     * From 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
     * integer type; indicates DL GFBR in case of GBR 5QI. The value is in
     * kbit/s. This parameter is omitted for a non-GBR 5QI (see 3GPP TS 24.501
     * [161])
     */
    int32_t dlGfbr;

    /**
     * From 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
     * integer type; indicates UL GFBR in case of GBR 5QI. The value is in
     * kbit/s. This parameter is omitted for a non-GBR 5QI (see 3GPP TS 24.501
     * [161]).
     */
    int32_t ulGfbr;

    /**
     * From 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
     * integer type; indicates DL MFBR in case of GBR 5QI. The value is in
     * kbit/s. This parameter is omitted for a non-GBR 5QI (see 3GPP TS 24.501
     * [161]).
     */
    int32_t dlMfbr;

    /**
     * From 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
     * integer type; indicates UL MFBR in case of GBR 5QI. The value is in
     * kbit/s. This parameter is omitted for a non-GBR 5QI (see 3GPP TS 24.501
     * [161]).
     */
    int32_t ulMfbr;

    /**
     * From 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
     * integer type; indicates the UL session AMBR(see 3GPP TS 24.501 [161]).
     * The value is in kbit/s.
     */
    int32_t ulSambr;

    /**
     * From 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
     * integer type; indicates the DL session AMBR(see 3GPP TS 24.501 [161]).
     * The value is in kbit/s.
     */
    int32_t dlSambr;

    /**
     * From 3GPP TS 27.007 10.1.50 V4.3.0 (2021-10)
     * integer type; indicates the averaging window(see 3GPP TS 24.501 [161]).
     * The value is in milliseconds.
     */
    int32_t averagingWindow;
} HRilLinkBandwidthInfo;

/**
 * @brief Defines the network bandwidth reporting rule.
 */
typedef struct {
    /** Radio Access Technology*/
    RatType rat;

    /**
     * integer type; (0,1,2,…) indicates the targeted time between request to
     * transfer an SDU at one SAP to its delivery at the other SAP, in
     * milliseconds. If the parameter is set to '0' the subscribed value will
     * be requested (refer 3GPP TS 24.008 [8] clause 10.5.6.5).
     */
    int32_t delayMs;

    /** Uplink delay. */
    int32_t delayUplinkKbps;

    /** Downlink delay. */
    int32_t delayDownlinkKbps;

    /** Maximum number of uplink parameters. */
    int32_t maximumUplinkKbpsSize;

    /** Maximum number of downlink parameters. */
    int32_t maximumDownlinkKbpsSize;

    /** Maximum uplink parameter list. */
    int32_t *maximumUplinkKbps;

    /** Maximum downlink parameter list. */
    int32_t *maximumDownlinkKbps;
} HRilLinkBandwidthReportingRule;

/**
 * @brief Defines the data service performance mode.
 */
typedef struct {
    /** Performance Enable. eg: 1-enable, 0-disable */
    int32_t performanceEnable;

    /** Whether Enforce Data Performance. eg: 1-enable, 0-disable */
    int32_t enforce;
} HRilDataPerformanceInfo;

/**
 * @brief Defines the sleep mode for data services.
 */
typedef struct {
    /** Sleep Mode Enable. eg: 1-enable, 0-disable */
    int32_t sleepEnable;
} HRilDataSleepInfo;

/**
 * @brief Defines the data link capability.
 */
typedef struct {
    /** Primary downlink capability in kbps. */
    int32_t primaryDownlinkKbps;

    /** Primary uplink capability in kbps. */
    int32_t primaryUplinkKbps;

    /** Secondary downlink capability in kbps. */
    int32_t secondaryDownlinkKbps;

    /** Secondary uplink capability in kbps. */
    int32_t secondaryUplinkKbps;
} HRilDataLinkCapability;

/**
 * @brief Defines the data request method.
 */
typedef struct {
    /**
     * @brief Sets the initial default network access technology (APN).
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*SetInitApnInfo)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);

    /**
     * @brief Activates the packet data protocol (PDP) context.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param data Indicates the data information.
     * @see ReqDataInfo | HRilDataInfo
     */
    void (*ActivatePdpContext)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);

    /**
     * @brief Deactivates the PDP context.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param data Indicates the data information.
     * @see ReqDataInfo | HRilDataInfo
     */
    void (*DeactivatePdpContext)(const ReqDataInfo *requestInfo, const HRilDataInfo *data);

    /**
     * @brief Obtains the PDP context list.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetPdpContextList)(const ReqDataInfo *requestInfo);

    /**
     * @brief Obtains the current link information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param cid Indicates a particular PDP context definition.
     * @see ReqDataInfo
     */
    void (*GetLinkBandwidthInfo)(const ReqDataInfo *requestInfo, int32_t cid);

    /**
     * @brief Sets the rule for reporting the data link bandwidth information.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param data Indicates the network bandwidth reporting rule.
     * @see ReqDataInfo | HRilLinkBandwidthReportingRule
     */
    void (*SetLinkBandwidthReportingRule)(const ReqDataInfo *requestInfo, const HRilLinkBandwidthReportingRule *data);

    /**
     * @brief Sets the PDP context information for the data service.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param data Indicates the data information.
     * @param len Indicates the length of data.
     * @see ReqDataInfo | HRilDataInfo
     */
    void (*SetDataProfileInfo)(const ReqDataInfo *requestInfo, const HRilDataInfo *data, int32_t len);

    /**
     * @brief Sends the data service performance mode.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param data Indicates the data service performance mode.
     * @see ReqDataInfo | HRilDataPerformanceInfo
     */
    void (*SendDataPerformanceMode)(const ReqDataInfo *requestInfo, const HRilDataPerformanceInfo *data);

    /**
     * @brief Sends the data service sleep mode.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param data Indicates the sleep mode for data services.
     * @see ReqDataInfo | HRilDataSleepInfo
     */
    void (*SendDataSleepMode)(const ReqDataInfo *requestInfo, const HRilDataSleepInfo *data);

    /**
     * @brief Enables the data service of a SIM card slot.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @param enabled Whether to enable the data service.
     * @see ReqDataInfo
     */
    void (*SetDataPermitted)(const ReqDataInfo *requestInfo, int32_t enabled);

    /**
     * @brief Get link capability.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*GetLinkCapability)(const ReqDataInfo *requestInfo);

    /**
     * @brief Clean all data connections.
     *
     * @param requestInfo Request data info, for details, see {@link
     * ReqDataInfo}.
     * @see ReqDataInfo
     */
    void (*CleanAllConnections)(const ReqDataInfo *requestInfo);
} HRilDataReq;
#ifdef __cplusplus
}
#endif
#endif // OHOS_RIL_VENDOR_DATA_DEFS_H
