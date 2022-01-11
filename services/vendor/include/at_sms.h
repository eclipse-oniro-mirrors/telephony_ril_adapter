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

#ifndef OHOS_AT_SMS_H
#define OHOS_AT_SMS_H

#include "at_support.h"
#include "hril_vendor_sms_defs.h"

void ReqSendGsmSms(const ReqDataInfo *requestInfo, const char *const *data, size_t dataLen);
void ReqSendSmsAck(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen);
void ReqSendCdmaSms(const ReqDataInfo *requestInfo, const char *data, size_t dataLen);
void ReqSendCdmaSmsAck(const ReqDataInfo *requestInfo, const char *data, size_t dataLen);
void ReqWriteSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen);
void ReqDelSimMessage(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen);
void ReqSetSmscAddr(const ReqDataInfo *requestInfo, const HRilServiceCenterAddress *data, size_t dataLen);
void ReqGetSmscAddr(const ReqDataInfo *requestInfo);
void ReqSetCBConfig(const ReqDataInfo *requestInfo, const HRilCBConfigInfo *data, size_t dataLen);
void ReqGetCBConfig(const ReqDataInfo *requestInfo);
void ReqGetCdmaCBConfig(const ReqDataInfo *requestInfo);
void ReqSetCdmaCBConfig(const ReqDataInfo *requestInfo, const HRilCdmaCBConfigInfo *data, size_t dataLen);
void ReqAddCdmaSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen);
void ReqDelCdmaSimMessage(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen);
void ReqUpdateCdmaSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen);
int32_t ProcessCellBroadcast(char *pBuff, HRilCBConfigReportInfo *response);
#endif // OHOS_AT_SMS_H
