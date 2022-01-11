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

#ifndef OHOS_AT_NETWORK_H
#define OHOS_AT_NETWORK_H

#include "hril_vendor_network_defs.h"

#define MAX_REG_INFO_ITEM 5
#define MAX_5GREG_INFO_ITEM 7
#define MAX_IMS_REG_INFO_ITEM 2
#define MAX_IMS_INFO_ITEM 3
#define DEFAULT_ADD_NUM 8
#define DEFAULT_INDEX 64
#undef DEFAULT_TIMEOUT
#define DEFAULT_TIMEOUT 5000
#define AUTO_TYPE "00"
#define GSM_TYPE "01"
#define WCDMA_TYPE "02"
#define LTE_TYPE "03"
#define LTE_WCDMA_GSM_TYPE "030201"
#define WCDMA_GSM_TYPE "0201"
#define LTE_WCDMA_TYPE "0302"

void ReqGetImsRegStatus(const ReqDataInfo *requestInfo);
void ReqGetSignalStrength(const ReqDataInfo *requestInfo);
void ReqGetCsRegStatus(const ReqDataInfo *requestInfo);
void ReqGetPsRegStatus(const ReqDataInfo *requestInfo);
void ReqGetOperatorInfo(const ReqDataInfo *requestInfo);
void ReqGetCellInfoList(const ReqDataInfo *requestInfo);
void ReqGetCurrentCellInfo(const ReqDataInfo *requestInfo);
void ReqSetPsAttachStatus(const ReqDataInfo *requestInfo, const int32_t *data);
void ReqGetPsAttachStatus(const ReqDataInfo *requestInfo);
void ReqGetNetworkSearchInformation(const ReqDataInfo *requestInfo);
void ReqGetNetworkSelectionMode(const ReqDataInfo *requestInfo);
void ReqSetNetworkSelectionMode(const ReqDataInfo *requestInfo, const HRilSetNetworkModeInfo *data);
void ReqGetRadioCapability(const ReqDataInfo *requestInfo);
void ReqSetRadioCapability(const ReqDataInfo *requestInfo, const HRilRadioCapability *data);
void ReqSetPreferredNetwork(const ReqDataInfo *requestInfo, const int32_t *data);
void ReqGetPreferredNetwork(const ReqDataInfo *requestInfo);
void ReqGetPhysicalChannelConfig(const ReqDataInfo *requestInfo);
void ReqSetLocateUpdates(const ReqDataInfo *requestInfo, HRilRegNotifyMode mode);
void ProcessPhyChnlCfgNotify(struct ReportInfo reportInfo, char *srcstr);
int32_t ProcessRegStatus(const char *s, const HRilRegStatusInfo *hrilRegStateInfo);
int32_t ProcessImsRegStatus(const char *s, const HRilImsRegStatusInfo *imsRegStatusInfo, int expectInfoNum);
int32_t ProcessParamSignalStrength(const char *result, HRilRssi *hrilRssi);
int32_t ProcessParamSignalStrengthNotify(const char *result, HRilRssi *hrilRssi);
int32_t ProcessOperListToUse(const char *list);
void PerformTimeOut(int sigFlag);
int32_t ParseOperListInfo(
    const char *lineInfo, int count, AvailableOperInfo *pOperInfo, AvailableOperInfo **ppOperInfo);
void NotifyNetWorkTime(void);
void GetNetworkSearchInformationPause(void);
#endif
