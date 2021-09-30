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

#include "hril.h"

#define MAX_REG_INFO_ITEM 5
#undef DEFAULT_TIMEOUT
#define DEFAULT_TIMEOUT 5000

void ReqGetSignalStrength(const ReqDataInfo *requestInfo);
void ReqGetCsRegStatus(const ReqDataInfo *requestInfo);
void ReqGetPsRegStatus(const ReqDataInfo *requestInfo);
void ReqGetOperatorInfo(const ReqDataInfo *requestInfo);
void RequestGetNetworkSearchInformation(const ReqDataInfo *requestInfo);
void RequestQueryNetworkSelectionMode(const ReqDataInfo *requestInfo);
void RequestSetAutomaticModeForNetworks(const ReqDataInfo *requestInfo, const HRiSetNetworkModeInfo *data);
int ProcessRegStatus(const char *s, char **response, int count);
int ProcessParamSignalStrength(const char *result, HRilRssi *hrilRssi);
int ProcessOperListToUse(const char *list);
void PerformTimeOut(int sigFlag);
int ParseOperListInfo(
    const char *lineInfo, int count, AvailableOperInfo *pOperInfo, AvailableOperInfo **ppOperInfo);
void GetNetworkSearchInformationPause(void);

#endif
