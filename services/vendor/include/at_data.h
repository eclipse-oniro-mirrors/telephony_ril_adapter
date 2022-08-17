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

#ifndef OHOS_AT_DATA_H
#define OHOS_AT_DATA_H

#include <stddef.h>
#include <stdint.h>

#include "hril_vendor_data_defs.h"

#define NET_NODE "usb0"

// Default maxTransferUnit value
#define DEFAULT_MTU 0

#define MAX_CID 11 // GU is 1~16 (currently only 11 is supported, and it can be expanded to 16 in the future).
#define MIN_CID 1 // GU is 1~16 (currently only 11 is supported, and it can be expanded to 16 in the future).
#define MAX_PDP_NUM 11

#define ACTIVATE 1
#define DEACTIVATE 0

#define ADDR_MASK 0xFF
#define ADDR_OFFSET_0BIT 0
#define ADDR_OFFSET_8BIT 8
#define ADDR_OFFSET_16BIT 16
#define ADDR_OFFSET_24BIT 24
#define ADDR_OFFSET_32BIT 32
#define IPV4_INDEX_MAX 4
#define IPV4_MASK_LEN_MIN 8
#define IPV4_MASK_MIN 0x000000FF
#define IPV4_MASK_MAX 0xFCFFFFFF
#define IPV4_BIT_MASK 0x80

#define DELAY_WAIT_MS (1000 * 1000)

#define DEFAULT_CID (-1)

#define QUERY_MAX_COUNT 6
#define QUERY_DELAY_MS 500
#define DELAY_US_OFFSET 1000

#define CALLBACK_DELAY_MS 50

#define VERIFY_TYPE_MIN 0
#define VERIFY_TYPE_MAX 2

/* 3GPP TS 27.007 V17.3.0 (2021-09)
 * Mobile termination error result code +CME ERROR */
typedef enum {
    PDP_CME_ERR_INCORRECT_PASSWORD = 16, /* incorrect password */
    PDP_CME_ERR_NO_NETWORK_SERVICE = 30, /* no network service */
    PDP_CME_ERR_INCORRECT_PARAMETERS = 50, /* Incorrect parameters */
    PDP_CME_ERR_ROAMING_NOT_ALLOWED = 113, /* Roaming not allowed in this location area */
    PDP_CME_ERR_NETWORK_FAILURE = 117, /* Network failure */
    PDP_CME_ERR_SHORTAGE_RESOURCES = 126, /* Insufficient resources */
    PDP_CME_ERR_MISSING_OR_UNKNOWN_APN = 127, /* Missing or unknown APN */
    PDP_CME_ERR_UNKNOWN_PDP_ADDR_OR_TYPE = 128, /* Unknown PDP address or PDP type */
    PDP_CME_ERR_USER_VERIFICATION = 129, /* User authentication or authorization failed */
    PDP_CME_ERR_ACTIVATION_REJECTED_GGSN = 130, /* Activation rejected by GGSN, Serving GW or PDN GW */
    PDP_CME_ERR_SERVICE_ACTIVATION_REJECTED_UNSPECIFIED = 131, /* Activation rejected, unspecified */
    PDP_CME_ERR_SERVICE_OPTION_NOT_SUPPORTED = 132, /* Service option not supported */
    PDP_CME_ERR_SERVICE_OPTION_NOT_SUBSCRIBED = 133, /* Requested service option not subscribed */
    PDP_CME_ERR_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER = 134, /* Service option temporarily out of order */
    PDP_CME_ERR_NSAPI_ALREADY_USED = 135, /* NSAPI already used */
    PDP_CME_ERR_REGULAR_DEACTIVATION = 136, /* Regular deactivation */
    PDP_CME_ERR_UNKNOWN_PDP_CONTEXT = 143, /* Unknown PDP context */
    PDP_CME_ERR_PROTOCOL_ERRORS = 176, /* Protocol error, unspecified */
    PDP_CME_ERR_OPERATOR_DETERMINED_BARRING = 177, /* Operator Determined Barring */
    PDP_CME_ERR_NO_PDP_CONTEXT_ACTIVATED = 185, /* No PDP context activated */
    PDP_CME_ERR_IPV4_ONLY_ALLOWED = 192, /* PDP type IPv4 only allowed */
    PDP_CME_ERR_IPV6_ONLY_ALLOWED = 193, /* PDP type IPv6 only allowed */
    PDP_CME_ERR_IPV4V6_ONLY_ALLOWED = 196, /* PDP type IPv4v6 only allowed */
    PDP_CME_ERR_NON_IP_ONLY_ALLOWED = 197, /* PDP type non IP only allowed */
    PDP_CME_ERR_MULT_ACCESSES_PDN_NOT_ALLOWED = 200, /* Multiple accesses to a PDN connection not allowed */
} ModemErrorCode;

typedef struct {
    int32_t cid;
    char *ipType;
    char *apn;
} PDNInfo;

void ReqSetInitApnInfo(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
void ReqActivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
void ReqDeactivatePdpContext(const ReqDataInfo *requestInfo, const HRilDataInfo *data);
void ReqGetPdpContextList(const ReqDataInfo *requestInfo);
void ReqGetLinkBandwidthInfo(const ReqDataInfo *requestInfo, const int32_t cid);
void ReqSetLinkBandwidthReportingRule(const ReqDataInfo *requestInfo, const HRilLinkBandwidthReportingRule *data);
void PdpContextListUpdate(void);
void ReqSetDataPermitted(const ReqDataInfo *requestInfo, const int32_t dataPermitted);
#endif // OHOS_AT_DATA_H
