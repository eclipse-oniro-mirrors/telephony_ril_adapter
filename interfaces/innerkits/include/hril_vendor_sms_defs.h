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

#ifndef OHOS_RIL_VENDOR_SMS_DEFS_H
#define OHOS_RIL_VENDOR_SMS_DEFS_H

#include "hril_enum.h"
#include "hril_public_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t msgRef; /* TP-Message-Reference for GSM, and BearerData MessageId for CDMA
                     * from 3GPP2 C.S0015-B, v2.0, 4.5-1 */
    char *pdu; /* Protocol Data Unit */
    int32_t errCode; /* if unknown or not applicable, that is -1
                      * from 3GPP 27.005, 3.2.5 for GSM/UMTS,
                      * 3GPP2 N.S0005 (IS-41C) Table 171 for CDMA */
} HRilSmsResponse;

typedef struct {
    int32_t index;
    int32_t state;
    char *smsc;
    char *pdu; /* Protocol Data Unit */
} HRilSmsWriteSms;

typedef struct {
    int32_t tosca;
    char *address;
} HRilServiceCenterAddress;

/* From 3GPP TS 27.005  AT+CSCB=[<mode>[,<mids>[,<dcss>]]] */
typedef struct {
    int32_t mode;
    char *mids;
    char *dcss;
} HRilCBConfigInfo;

typedef struct {
    int32_t service;
    int32_t language;
    unsigned char checked;
} HRilCdmaCBConfigInfo;

/* From 3GPP TS 27.005   if text mode (+CMGF=1):
    <CR><LF>+CBM:
    <sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data><CR>
    <LF> if PDU mode
    (+CMGF=0):
    <CR><LF>+CBM: <length><CR><LF><pdu><CR><LF>
*/
typedef struct {
    int32_t sn;
    int32_t mid;
    int32_t page;
    int32_t pages;
    char *dcs;
    char *data;
    int32_t length;
    char *pdu;
} HRilCBConfigReportInfo;

typedef struct {
    int32_t digitMode;
    int32_t mode;
    int32_t type;
    int32_t plan;
    unsigned char number;
    unsigned char bytes[HRIL_MAX_CDMA_ADDRESS_LEN];
} HRilCdmaSmsAddress;

typedef struct {
    int32_t type;
    unsigned char odd;
    unsigned char number;
    unsigned char bytes[HRIL_MAX_CDMA_ADDRESS_LEN];
} HRilCdmaSmsSubAddress;

typedef struct {
    int32_t serviceId;
    unsigned char isExist;
    int32_t type;
    HRilCdmaSmsAddress address;
    HRilCdmaSmsSubAddress subAddress;
    int32_t size;
    unsigned char bytes[HRIL_MAX_CDMA_MESSAGE_LEN];
} HRilCdmaSmsMessageInfo;

typedef struct {
    void (*SendGsmSms)(const ReqDataInfo *requestInfo, const char *const *data, size_t dataLen);
    void (*SendSmsAck)(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen);
    void (*SendCdmaSms)(const ReqDataInfo *requestInfo, const char *data, size_t dataLen);
    void (*SendCdmaAck)(const ReqDataInfo *requestInfo, const char *data, size_t dataLen);
    void (*AddSimMessage)(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen);
    void (*DelSimMessage)(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen);
    void (*UpdateSimMessage)(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen);
    void (*SetSmscAddr)(const ReqDataInfo *requestInfo, const HRilServiceCenterAddress *data, size_t dataLen);
    void (*GetSmscAddr)(const ReqDataInfo *requestInfo);
    void (*SetCBConfig)(const ReqDataInfo *requestInfo, const HRilCBConfigInfo *data, size_t dataLen);
    void (*GetCBConfig)(const ReqDataInfo *requestInfo);
    void (*GetCdmaCBConfig)(const ReqDataInfo *requestInfo);
    void (*SetCdmaCBConfig)(const ReqDataInfo *requestInfo, const HRilCdmaCBConfigInfo *data, size_t dataLen);
    void (*AddCdmaSimMessage)(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen);
    void (*DelCdmaSimMessage)(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen);
    void (*UpdateCdmaSimMessage)(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen);
} HRilSmsReq;
#ifdef __cplusplus
}
#endif
#endif // OHOS_RIL_VENDOR_SMS_DEFS_H
