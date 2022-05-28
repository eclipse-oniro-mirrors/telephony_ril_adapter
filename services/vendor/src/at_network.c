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

#include "at_network.h"

#include <signal.h>

#include "vendor_report.h"
#include "vendor_util.h"

#include "hril_notification.h"

static struct ReportInfo g_reportInfoForOperListToUse;
static pthread_mutex_t g_networkSearchInformationMutex = PTHREAD_MUTEX_INITIALIZER;

struct ResponseAck {
    ResponseInfo *responseInfo;
    uint8_t *respDataPointer;
    int32_t respDataLen;
};

// RadioAccessFamily defines
const int32_t RAF_UNKNOWN = 1 << RADIO_TECHNOLOGY_UNKNOWN;
const int32_t RAF_GSM = 1 << RADIO_TECHNOLOGY_GSM;
const int32_t RAF_1XRTT = 1 << RADIO_TECHNOLOGY_1XRTT;
const int32_t RAF_WCDMA = 1 << RADIO_TECHNOLOGY_WCDMA;
const int32_t RAF_HSPA = 1 << RADIO_TECHNOLOGY_HSPA;
const int32_t RAF_HSPAP = 1 << RADIO_TECHNOLOGY_HSPAP;
const int32_t RAF_TD_SCDMA = 1 << RADIO_TECHNOLOGY_TD_SCDMA;
const int32_t RAF_EVDO = 1 << RADIO_TECHNOLOGY_EVDO;
const int32_t RAF_EHRPD = 1 << RADIO_TECHNOLOGY_EHRPD;
const int32_t RAF_LTE = 1 << RADIO_TECHNOLOGY_LTE;
const int32_t RAF_LTE_CA = 1 << RADIO_TECHNOLOGY_LTE_CA;
const int32_t RAF_NR = 1 << RADIO_TECHNOLOGY_NR;

// Group
const int32_t GSM = RAF_GSM;
const int32_t CDMA = RAF_1XRTT;
const int32_t EVDO = RAF_EVDO | RAF_EHRPD;
const int32_t HS = RAF_WCDMA | RAF_HSPA | RAF_HSPAP;
const int32_t WCDMA = HS;
const int32_t LTE = RAF_LTE | RAF_LTE_CA;
const int32_t NR = RAF_NR;

// NG
const int32_t RAF_2G = GSM | CDMA;
const int32_t RAF_3G = WCDMA | EVDO | RAF_TD_SCDMA;
const int32_t RAF_4G = LTE;
const int32_t RAF_5G = NR;
const int32_t RAF_AUTO = RAF_2G | RAF_3G | RAF_4G | RAF_5G;

static int32_t GetResponseErrorCode(ResponseInfo *pResponseInfo)
{
    char *pLine = NULL;
    int32_t ret = HRIL_ERR_GENERIC_FAILURE;
    if (pResponseInfo && pResponseInfo->result) {
        pLine = pResponseInfo->result;
        SkipATPrefix(&pLine);
        NextInt(&pLine, &ret);
    }

    if (ret == -1) {
        ret = HRIL_ERR_INVALID_RESPONSE;
    }
    TELEPHONY_LOGI("networks response error code: %{public}d", ret);
    return ret;
}

static int32_t ResponseNetworkReport(
    int32_t slotId, const ReqDataInfo *requestInfo, int32_t err, struct ResponseAck *respDataAck)
{
    if (requestInfo == NULL) {
        TELEPHONY_LOGE("requestInfo is nullptr!");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (respDataAck == NULL) {
        TELEPHONY_LOGE("respDataAck is nullptr!");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo;
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(slotId, reportInfo, (const uint8_t *)(respDataAck->respDataPointer), respDataAck->respDataLen);
    if (respDataAck->responseInfo != NULL) {
        FreeResponseInfo(respDataAck->responseInfo);
    }
    return err;
}

static HRilRadioTech ConvertActToRadioTech(HRilActTech actType)
{
    switch (actType) {
        case HRIL_RADIO_GSM:
        case HRIL_RADIO_GSM_COMPACT:
        case HRIL_RADIO_EGPRS:
            return RADIO_TECHNOLOGY_GSM;
        case HRIL_RADIO_HSDPA_HSUPA:
        case HRIL_RADIO_HSDPA:
        case HRIL_RADIO_HSUPA:
            return RADIO_TECHNOLOGY_HSPA;
        case HRIL_RADIO_HSPAP:
            return RADIO_TECHNOLOGY_HSPAP;
        case HRIL_RADIO_UTRAN:
            return RADIO_TECHNOLOGY_WCDMA;
        case HRIL_RADIO_EUTRAN:
            return RADIO_TECHNOLOGY_LTE;
        case HRIL_RADIO_CDMA:
        case HRIL_RADIO_CDMA_IS95A:
        case HRIL_RADIO_CDMA_IS95B:
            return RADIO_TECHNOLOGY_1XRTT;
        case HRIL_RADIO_CDMA_EVDO_0:
        case HRIL_RADIO_CDMA_EVDO_A:
        case HRIL_RADIO_CDMA_EVDO_B:
            return RADIO_TECHNOLOGY_EVDO;
        case HRIL_RADIO_CDMA_EHRPD:
            return RADIO_TECHNOLOGY_EHRPD;
        case HRIL_RADIO_TDSCDMA:
            return RADIO_TECHNOLOGY_TD_SCDMA;
        case HRIL_RADIO_LTE_CA:
            return RADIO_TECHNOLOGY_LTE_CA;
        case HRIL_RADIO_NR:
            return RADIO_TECHNOLOGY_NR;
        default:
            return RADIO_TECHNOLOGY_UNKNOWN;
    }
}

static int32_t FindSemicolonCharNum(const char *srcStr)
{
    char *str = (char *)srcStr;
    if (str == NULL) {
        TELEPHONY_LOGE("Semicolon srcStr parameter is null.");
        return -1;
    }
    if (*str == '\0') {
        return -1;
    }
    int32_t charNum = 0;
    while (*str != '\0') {
        if (*str == ';') {
            charNum++;
        }
        str++;
    }
    return charNum;
}

static HRilRegStatus ConvertIntToHRilRegStatus(int32_t reg)
{
    if ((reg >= NO_REG_MT_NO_SEARCH) && (reg <= REG_MT_EMERGENCY)) {
        return (HRilRegStatus)reg;
    } else {
        return REG_MT_UNKNOWN;
    }
}

static int32_t ExtractRegStatus(const char *s, const HRilRegStatusInfo *hrilRegInfo)
{
    char *str = (char *)s;
    HRilRegStatusInfo *regStateInfo = (HRilRegStatusInfo *)hrilRegInfo;
    int32_t info[MAX_5GREG_INFO_ITEM] = {0, 0, 0, RADIO_TECHNOLOGY_INVALID, 0, 0, 0};
    const int32_t FORMAT_ONE_COMMA_NUM = 0;
    const int32_t FORMAT_TWO_COMMA_NUM = 3;
    const int32_t FORMAT_TWO_5G_COMMA_NUM = 6;
    int32_t commaNum = FindCommaCharNum(str);
    if (NextInt(&str, &(info[REG_STAT_POS])) != 0) {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    if (commaNum == FORMAT_ONE_COMMA_NUM) {
    } else if ((commaNum == FORMAT_TWO_COMMA_NUM) || (commaNum == FORMAT_TWO_5G_COMMA_NUM)) {
        if (NextIntFromHex(&str, &(info[REG_LAC_POS])) != 0) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        if (NextIntFromHex(&str, &(info[REG_CELL_ID_POS])) != 0) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        if (NextInt(&str, &(info[REG_ACT_POS])) != 0) {
            return HRIL_ERR_INVALID_RESPONSE;
        }
        if (commaNum == FORMAT_TWO_5G_COMMA_NUM) {
            if (NextInt(&str, &(info[REG_NR_AVAILABLE_POS])) != 0) {
                return HRIL_ERR_INVALID_RESPONSE;
            }
            if (NextInt(&str, &(info[REG_EN_DC_AVAILABLE_POS])) != 0) {
                return HRIL_ERR_INVALID_RESPONSE;
            }
            if (NextInt(&str, &(info[REG_DC_NR_RESTRICTED_POS])) != 0) {
                return HRIL_ERR_INVALID_RESPONSE;
            }
        }
    } else {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    regStateInfo->regStatus = ConvertIntToHRilRegStatus(info[REG_STAT_POS]);
    regStateInfo->lacCode = info[REG_LAC_POS];
    regStateInfo->cellId = info[REG_CELL_ID_POS];
    regStateInfo->actType = ConvertActToRadioTech((HRilActTech)info[REG_ACT_POS]);
    regStateInfo->isNrAvailable = ((info[REG_NR_AVAILABLE_POS] == 0) ? 0 : 1);
    regStateInfo->isEnDcAvailable = ((info[REG_EN_DC_AVAILABLE_POS] == 0) ? 0 : 1);
    regStateInfo->isDcNrRestricted = ((info[REG_DC_NR_RESTRICTED_POS] == 0) ? 0 : 1);
    return HRIL_ERR_SUCCESS;
}

static int32_t ParseRegStatusStr(const char *srcStr, HRilRegStatusInfo *hrilRegInfo)
{
    if (srcStr == NULL || hrilRegInfo == NULL) {
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (ReportStrWith(srcStr, "+CREG:")) {
        hrilRegInfo->regType = CS_REG_TYPE;
    } else if (ReportStrWith(srcStr, "+CGREG:")) {
        hrilRegInfo->regType = PS_REG_TYPE;
    } else {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    char *str = (char *)srcStr;
    if (SkipATPrefix(&str) < 0) {
        TELEPHONY_LOGE("skip failed: [%{public}s]", str);
        return HRIL_ERR_INVALID_RESPONSE;
    }
    const int32_t NUM_ONE = 1;
    const int32_t NUM_THREE = 3;
    const int32_t NUM_FOUR = 4;
    const int32_t NUM_SIX = 6;
    const int32_t NUM_SEVEN = 7;
    hrilRegInfo->notifyMode = REG_NOTIFY_STAT_LAC_CELLID;
    hrilRegInfo->regMsgType = REG_RESPONSE_TYPE;
    int32_t commaNum = FindCommaCharNum(str);
    if (commaNum == 0) {
        hrilRegInfo->regMsgType = REG_NOTIFY_TYPE;
        hrilRegInfo->notifyMode = REG_NOTIFY_STAT_ONLY;
    } else if (commaNum == NUM_ONE) {
        hrilRegInfo->notifyMode = REG_NOTIFY_STAT_ONLY;
    } else if (commaNum == NUM_FOUR || commaNum == NUM_SEVEN) {
    } else if (commaNum == NUM_THREE || commaNum == NUM_SIX) {
        hrilRegInfo->regMsgType = REG_NOTIFY_TYPE;
    } else {
        hrilRegInfo->regMsgType = REG_NOTIFY_TYPE;
        return HRIL_ERR_INVALID_RESPONSE;
    }
    int32_t skip = 0;
    if (hrilRegInfo->regMsgType == REG_RESPONSE_TYPE) {
        if ((NextInt(&str, &skip)) != 0) {
            TELEPHONY_LOGE("skip failed: [%{public}s]", str);
            return HRIL_ERR_INVALID_RESPONSE;
        }
        if ((skip != (int32_t)REG_NOTIFY_STAT_LAC_CELLID) && (skip != (int32_t)REG_NOTIFY_STAT_ONLY)) {
            TELEPHONY_LOGE("notifyType check failed: [%{public}d]", skip);
            return HRIL_ERR_INVALID_RESPONSE;
        }
    }
    return ExtractRegStatus(str, hrilRegInfo);
}

int32_t ProcessRegStatus(const char *s, const HRilRegStatusInfo *hrilRegStateInfo)
{
    char *str = (char *)s;
    HRilRegStatusInfo *regStateInfo = (HRilRegStatusInfo *)hrilRegStateInfo;
    if ((str == NULL) || (regStateInfo == NULL)) {
        TELEPHONY_LOGE("ProcessRegStatus s or regStateInfo param is null");
        return HRIL_ERR_NULL_POINT;
    } else {
        (void)memset_s(regStateInfo, sizeof(HRilRegStatusInfo), 0, sizeof(HRilRegStatusInfo));
        return ParseRegStatusStr(str, regStateInfo);
    }
}

static void ParseGetGsmSignalStrength(const char *line, HRilRssi *hrilRssi)
{
    char *lineStr = (char *)line;
    if (lineStr == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&lineStr, &hrilRssi->gsmRssi.rxlev);
    NextInt(&lineStr, &hrilRssi->gsmRssi.ber);
    hrilRssi->gsmRssi.rxlev = -hrilRssi->gsmRssi.rxlev;
}

static void ParseGetLteSignalStrength(const char *line, HRilRssi *hrilRssi)
{
    char *lineStr = (char *)line;
    if (lineStr == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&lineStr, &hrilRssi->lteRssi.rxlev);
    NextInt(&lineStr, &hrilRssi->lteRssi.rsrq);
    NextInt(&lineStr, &hrilRssi->lteRssi.rsrp);
    NextInt(&lineStr, &hrilRssi->lteRssi.snr);
    hrilRssi->lteRssi.rxlev = -hrilRssi->lteRssi.rxlev;
    hrilRssi->lteRssi.rsrq = -hrilRssi->lteRssi.rsrq;
    hrilRssi->lteRssi.rsrp = -hrilRssi->lteRssi.rsrp;
}

static void ParseGetWcdmaSignalStrength(const char *line, HRilRssi *hrilRssi)
{
    char *lineStr = (char *)line;
    if (lineStr == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&lineStr, &hrilRssi->wcdmaRssi.rxlev);
    NextInt(&lineStr, &hrilRssi->wcdmaRssi.ecio);
    NextInt(&lineStr, &hrilRssi->wcdmaRssi.rscp);
    NextInt(&lineStr, &hrilRssi->wcdmaRssi.ber);
    hrilRssi->wcdmaRssi.rxlev = -hrilRssi->wcdmaRssi.rxlev;
    hrilRssi->wcdmaRssi.ecio = -hrilRssi->wcdmaRssi.ecio;
    hrilRssi->wcdmaRssi.rscp = -hrilRssi->wcdmaRssi.rscp;
}

static void ParseGetTdScdmaSignalStrength(const char *line, HRilRssi *hrilRssi)
{
    char *lineStr = (char *)line;
    if (lineStr == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&lineStr, &hrilRssi->tdScdmaRssi.rscp);
    hrilRssi->tdScdmaRssi.rscp = -hrilRssi->tdScdmaRssi.rscp;
}

static void ParseGetCdmaSignalStrength(const char *line, HRilRssi *hrilRssi)
{
    char *lineStr = (char *)line;
    if (lineStr == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&lineStr, &hrilRssi->cdmaRssi.absoluteRssi);
    NextInt(&lineStr, &hrilRssi->cdmaRssi.ecno);
    hrilRssi->cdmaRssi.absoluteRssi = -hrilRssi->cdmaRssi.absoluteRssi;
    hrilRssi->cdmaRssi.ecno = -hrilRssi->cdmaRssi.ecno;
}

static void ParseGetNrSignalStrength(const char *line, HRilRssi *hrilRssi)
{
    char *lineStr = (char *)line;
    if (lineStr == NULL || hrilRssi == NULL) {
        TELEPHONY_LOGE("line or hrilRssi is null!!!");
        return;
    }
    NextInt(&lineStr, &hrilRssi->nrRssi.rsrp);
    NextInt(&lineStr, &hrilRssi->nrRssi.rsrq);
    NextInt(&lineStr, &hrilRssi->nrRssi.sinr);
    hrilRssi->nrRssi.rsrp = -hrilRssi->nrRssi.rsrp;
    hrilRssi->nrRssi.rsrq = -hrilRssi->nrRssi.rsrq;
}

int32_t ProcessParamSignalStrength(const char *result, HRilRssi *hrilRssi)
{
    char *resultStr = (char *)result;
    char *c = NULL;
    int32_t tmp = 0;
    int32_t err = SkipATPrefix(&resultStr);
    if (err < 0) {
        TELEPHONY_LOGE("skip failed: [%{public}s]", resultStr);
        return err;
    }
    err = NextInt(&resultStr, &tmp);
    TELEPHONY_LOGI("ProcessParamSignalStrength  enter -->, result %{public}s", resultStr);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }
    err = NextInt(&resultStr, &tmp);
    TELEPHONY_LOGI("ProcessParamSignalStrength  enter -->, result %{public}s", resultStr);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }
    err = NextStr(&resultStr, &c);
    TELEPHONY_LOGI("ProcessParamSignalStrength  enter -->, result %{public}s", resultStr);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }
    if (!strcmp(c, "GSM")) {
        TELEPHONY_LOGI("ProcessParamSignalStrength  enter GSM-->, result %{public}s", resultStr);
        ParseGetGsmSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "LTE")) {
        TELEPHONY_LOGI("ProcessParamSignalStrength  enter LTE-->, result %{public}s", resultStr);
        ParseGetLteSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "WCDMA")) {
        TELEPHONY_LOGI("ProcessParamSignalStrength  enter WCDMA-->, result %{public}s", resultStr);
        ParseGetWcdmaSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "TDSCDMA")) {
        TELEPHONY_LOGI("ProcessParamSignalStrength  enter TDSCDMA-->, result %{public}s", resultStr);
        ParseGetTdScdmaSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "CDMA")) {
        TELEPHONY_LOGI("ProcessParamSignalStrength  enter CDMA-->, result %{public}s", resultStr);
        ParseGetCdmaSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "NR")) {
        TELEPHONY_LOGI("ProcessParamSignalStrength  enter NR-->, result %{public}s", resultStr);
        ParseGetNrSignalStrength(resultStr, hrilRssi);
    }
    return HRIL_ERR_SUCCESS;
}

int32_t ProcessParamSignalStrengthNotify(const char *result, HRilRssi *hrilRssi)
{
    char *resultStr = (char *)result;
    char *c = NULL;
    int32_t err = SkipATPrefix(&resultStr);
    TELEPHONY_LOGI("ProcessParamSignalStrengthNotify  enter -->, resultStr %{public}s", resultStr);
    if (err < 0) {
        TELEPHONY_LOGE("skip failed: %{public}s", resultStr);
        return err;
    }

    err = NextStr(&resultStr, &c);
    if (err < 0) {
        TELEPHONY_LOGE("read failed: %{public}d", err);
        return err;
    }

    if (!strcmp(c, "GSM")) {
        ParseGetGsmSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "LTE")) {
        ParseGetLteSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "WCDMA")) {
        ParseGetWcdmaSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "TDSCDMA")) {
        ParseGetTdScdmaSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "CDMA")) {
        ParseGetCdmaSignalStrength(resultStr, hrilRssi);
    } else if (!strcmp(c, "NR")) {
        ParseGetNrSignalStrength(resultStr, hrilRssi);
    }
    return HRIL_ERR_SUCCESS;
}

static int32_t CheckImsStatusValid(
    const int32_t *imsStatusArray, HRilImsRegStatusInfo *imsRegStatusInfo, int32_t realInfoNum, int32_t expectInfoNum)
{
    if ((imsStatusArray == NULL) || (imsRegStatusInfo == NULL)) {
        TELEPHONY_LOGE("imsStatusArray or imsRegStatusInfo is invalid");
        return HRIL_ERR_INVALID_RESPONSE;
    }
    const int32_t IMS_NOTIFY_NUM = 2;
    const int32_t IMS_EXT_INFO_NOTIFY_TYPE = 2;
    const int32_t REG_INFO_MAX_VALUE = 1;
    const int32_t IMS_STATUS_INDEX = 2;
    if (expectInfoNum == IMS_NOTIFY_NUM) {
        if ((realInfoNum == IMS_NOTIFY_NUM) && (imsStatusArray[0] <= REG_INFO_MAX_VALUE)) {
            imsRegStatusInfo->notifyType = IMS_EXT_INFO_NOTIFY_TYPE;
            imsRegStatusInfo->regInfo = imsStatusArray[0];
            imsRegStatusInfo->extInfo = imsStatusArray[1];
        } else if ((realInfoNum == 1) && (imsStatusArray[0] == 0)) {
            imsRegStatusInfo->notifyType = IMS_EXT_INFO_NOTIFY_TYPE;
            imsRegStatusInfo->regInfo = 0;
            imsRegStatusInfo->extInfo = 0;
        } else {
            TELEPHONY_LOGE("CheckImsStatusValid: Invalid response");
            return HRIL_ERR_INVALID_RESPONSE;
        }
    } else if (expectInfoNum == MAX_IMS_INFO_ITEM) {
        if (realInfoNum == IMS_NOTIFY_NUM) {
            if ((imsStatusArray[0] != IMS_EXT_INFO_NOTIFY_TYPE) || (imsStatusArray[1] != 0)) {
                TELEPHONY_LOGE("CheckImsStatusValid: Invalid response");
                return HRIL_ERR_INVALID_RESPONSE;
            } else {
                imsRegStatusInfo->notifyType = IMS_EXT_INFO_NOTIFY_TYPE;
                imsRegStatusInfo->regInfo = 0;
                imsRegStatusInfo->extInfo = 0;
            }
        } else {
            imsRegStatusInfo->notifyType = imsStatusArray[0];
            imsRegStatusInfo->regInfo = imsStatusArray[1];
            imsRegStatusInfo->extInfo = imsStatusArray[IMS_STATUS_INDEX];
        }
    }
    if (imsRegStatusInfo->notifyType != IMS_EXT_INFO_NOTIFY_TYPE) {
        TELEPHONY_LOGE("Ims Status n Value is invalid");
        return HRIL_ERR_INVALID_RESPONSE;
    }
    if ((imsRegStatusInfo->regInfo < 0) || (imsRegStatusInfo->regInfo > REG_INFO_MAX_VALUE)) {
        TELEPHONY_LOGE("Ims Status regInfo Value is invalid");
        return HRIL_ERR_INVALID_RESPONSE;
    }
    return HRIL_ERR_SUCCESS;
}

int32_t ProcessImsRegStatus(const char *s, const HRilImsRegStatusInfo *imsRegStatusInfo, int32_t expectInfoNum)
{
    char *str = (char *)s;
    if ((str == NULL) || (imsRegStatusInfo == NULL)) {
        TELEPHONY_LOGE("ProcessIMSRegStatus s or imsRegStatusInfo param is null");
        return HRIL_ERR_NULL_POINT;
    } else {
        TELEPHONY_LOGI("result: %{public}s", str);
        int32_t err = SkipATPrefix(&str);
        if (err < 0) {
            TELEPHONY_LOGE("skip failed: [%{public}s]", str);
            return HRIL_ERR_INVALID_RESPONSE;
        }
        int32_t commaNum = FindCommaCharNum(str);
        if ((commaNum < 0) || (commaNum >= MAX_IMS_INFO_ITEM)) {
            TELEPHONY_LOGE("check failed commaNum: [%{public}d]", commaNum);
            return HRIL_ERR_INVALID_RESPONSE;
        }
        int32_t info[MAX_IMS_INFO_ITEM] = {0, 0, 0};
        for (int32_t i = 0; i <= commaNum; i++) {
            err = NextInt(&str, &(info[i]));
            if (err == -1) {
                info[i] = -1;
                return HRIL_ERR_INVALID_RESPONSE;
            }
        }
        if (CheckImsStatusValid(info, (HRilImsRegStatusInfo *)imsRegStatusInfo, commaNum + 1, expectInfoNum) !=
            HRIL_ERR_SUCCESS) {
            TELEPHONY_LOGE("ProcessImsRegStatus: Invalid response");
            return HRIL_ERR_INVALID_RESPONSE;
        }
        return HRIL_ERR_SUCCESS;
    }
}

void ReqGetImsRegStatus(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    int32_t err = HRIL_ERR_SUCCESS;
    char *result = NULL;
    struct ReportInfo reportInfo;
    const long TIME_OUT = DEFAULT_TIMEOUT;
    ResponseInfo *responseInfo = NULL;

    TELEPHONY_LOGI("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    int32_t ret = SendCommandLock("AT+CIREG?", "+CIREG:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }

    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("AT+CIREG send failed");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    HRilImsRegStatusInfo imsRegStatusInfo = {0};
    ret = ProcessImsRegStatus(result, &imsRegStatusInfo, MAX_IMS_INFO_ITEM);
    if (ret != 0) {
        TELEPHONY_LOGE("GetImsRegStatusResponse CIREGU format  unexpected: %{public}s", result);
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(requestInfo->slotId, reportInfo, (const uint8_t *)&imsRegStatusInfo, sizeof(HRilImsRegStatusInfo));
    FreeResponseInfo(responseInfo);
}

void ReqGetSignalStrength(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    int32_t err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    const long TIME_OUT = DEFAULT_TIMEOUT;
    const int32_t REPORT_SIZE = 20;
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    TELEPHONY_LOGI("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    int32_t ret = SendCommandLock("AT^HCSQ?", "^HCSQ:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("responseInfo is nullptr!");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }

    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("AT^HCSQ send failed");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    if (result == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    HRilRssi hrilRssi = {0};
    if (strlen(result) > REPORT_SIZE) {
        ret = ProcessParamSignalStrength(result, &hrilRssi);
    } else {
        ret = ProcessParamSignalStrengthNotify(result, &hrilRssi);
    }
    if (ret != 0) {
        err = HRIL_ERR_GENERIC_FAILURE;
        TELEPHONY_LOGE("SignalStrength is null!");
    }

    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(requestInfo->slotId, reportInfo, (const uint8_t *)&hrilRssi, sizeof(HRilRssi));
    FreeResponseInfo(responseInfo);
}

void ReqGetCsRegStatus(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    int32_t err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    const long TIME_OUT = DEFAULT_TIMEOUT;

    int32_t ret = SendCommandLock("AT+CREG?", "+CREG:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("responseInfo is nullptr!");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    HRilRegStatusInfo regStatusInfo = {0};
    ret = ProcessRegStatus(result, &regStatusInfo);
    if (ret != 0) {
        TELEPHONY_LOGE("ReqGetCsRegStatus CREG format  unexpected: %{public}s", result);
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(requestInfo->slotId, reportInfo, (const uint8_t *)&regStatusInfo, sizeof(HRilRegStatusInfo));
    FreeResponseInfo(responseInfo);
}

void ReqGetPsRegStatus(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    int32_t err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    const long TIME_OUT = DEFAULT_TIMEOUT;

    int32_t ret = SendCommandLock("AT+CGREG?", "+CGREG:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("responseInfo is nullptr!");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    HRilRegStatusInfo regStatusInfo = {0};
    ret = ProcessRegStatus(result, &regStatusInfo);
    if (ret != 0) {
        TELEPHONY_LOGE("ReqGetPsRegStatus CGREG format  unexpected: %{public}s", result);
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(requestInfo->slotId, reportInfo, (const uint8_t *)&regStatusInfo, sizeof(HRilRegStatusInfo));
    FreeResponseInfo(responseInfo);
}

void ReqGetOperatorInfo(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    int32_t err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    const int32_t numCount = 3;
    char *response[numCount] = {"", "", ""};
    const long TIME_OUT = DEFAULT_TIMEOUT;

    int32_t ret =
        SendCommandLock("AT+COPS=3,0;+COPS?;+COPS=3,1;+COPS?;+COPS=3,2;+COPS?", "+COPS:", TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("responseInfo is nullptr!");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    Line *pLine = responseInfo->head;
    for (int32_t i = 0; pLine != NULL; i++, pLine = pLine->next) {
        int32_t skip = 0;
        result = pLine->data;
        SkipATPrefix(&result);
        ret = NextInt(&result, &skip);
        if (ret == -1) {
            TELEPHONY_LOGE("read failed");
            break;
        }
        ret = NextInt(&result, &skip);
        if (ret == -1) {
            TELEPHONY_LOGE("read failed");
            break;
        }
        ret = NextStr(&result, &response[i]);
        TELEPHONY_LOGI("result[%{public}d]: %{public}s", i, response[i]);
        if (ret == -1) {
            response[i] = "";
            TELEPHONY_LOGE("read failed");
            break;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(requestInfo->slotId, reportInfo, (const uint8_t *)response, numCount);
    FreeResponseInfo(responseInfo);
}

static int32_t MoveLeftBracket(char **pStr)
{
    if (*pStr == NULL) {
        TELEPHONY_LOGE("pStr is nullptr");
        return -1;
    }
    *pStr = strchr(*pStr, '(');
    if (*pStr == NULL) {
        TELEPHONY_LOGE("pStr is nullptr");
        return -1;
    }
    (*pStr)++;
    return 0;
}

void GetNetworkSearchInformationPause(void)
{
    TELEPHONY_LOGI("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    pthread_mutex_lock(&g_networkSearchInformationMutex);
    g_reportInfoForOperListToUse.error = HRIL_ERR_NETWORK_SEARCHING_INTERRUPTED;
    if (g_reportInfoForOperListToUse.requestInfo != NULL) {
        OnNetworkReport(g_reportInfoForOperListToUse.requestInfo->slotId,
            g_reportInfoForOperListToUse, NULL, 0);
    }
    SetAtPauseFlag(false);
    if (g_reportInfoForOperListToUse.requestInfo != NULL) {
        g_reportInfoForOperListToUse.requestInfo = NULL;
    }
    pthread_mutex_unlock(&g_networkSearchInformationMutex);
}

void PerformTimeOut(int32_t sigFlag)
{
    if (SIGALRM == sigFlag) {
        pthread_mutex_lock(&g_networkSearchInformationMutex);
        bool sendFlag = GetAtPauseFlag();
        TELEPHONY_LOGI("enter to [%{public}s]:%{public}d", __func__, __LINE__);
        if (sendFlag) {
            g_reportInfoForOperListToUse.error = HRIL_ERR_NETWORK_SEARCH_TIMEOUT;
            if (g_reportInfoForOperListToUse.requestInfo != NULL) {
                OnNetworkReport(g_reportInfoForOperListToUse.requestInfo->slotId,
                    g_reportInfoForOperListToUse, NULL, 0);
            }
            SetAtPauseFlag(false);
            if (g_reportInfoForOperListToUse.requestInfo != NULL) {
                g_reportInfoForOperListToUse.requestInfo = NULL;
            }
            TELEPHONY_LOGI("ReqGetNetworkSearchInformation response timeout!");
        }
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
    }
    return;
}

void ReqGetNetworkSearchInformation(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    const long TIME_OUT = 1000;
    ResponseInfo *responseInfo = NULL;
    const int32_t SECOND = 120;
    TELEPHONY_LOGI("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    pthread_mutex_lock(&g_networkSearchInformationMutex);
    alarm(0);
    if (g_reportInfoForOperListToUse.requestInfo != NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NETWORK_SEARCHING, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
        TELEPHONY_LOGE("ReqGetNetworkSearchInformation repeat Send!");
        return;
    }
    if (signal(SIGALRM, PerformTimeOut) == SIG_ERR) {
        TELEPHONY_LOGE("ReqGetNetworkSearchInformation signal PerformTimeOut is SIG_ERR!");
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
        return;
    }
    int32_t ret = SendCommandNetWorksLock("AT+COPS=?", "+COPS:", TIME_OUT, &responseInfo);

    g_reportInfoForOperListToUse = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    if ((ret != 0 && ret != AT_ERR_WAITING) || (responseInfo != NULL && !responseInfo->success)) {
        TELEPHONY_LOGE("send AT CMD failed!");
        SetAtPauseFlag(false);
        g_reportInfoForOperListToUse.error = GetResponseErrorCode(responseInfo);
        OnNetworkReport(requestInfo->slotId, g_reportInfoForOperListToUse, NULL, 1);
        if (responseInfo != NULL) {
            FreeResponseInfo(responseInfo);
        }
        if (g_reportInfoForOperListToUse.requestInfo != NULL) {
            g_reportInfoForOperListToUse.requestInfo = NULL;
        }
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
        return;
    }
    alarm(SECOND);
    if (responseInfo != NULL) {
        FreeResponseInfo(responseInfo);
    }
    pthread_mutex_unlock(&g_networkSearchInformationMutex);
    SetWatchFunction(GetNetworkSearchInformationPause);
}

static int32_t ParseOperInfoItem(
    char **lineInfo, char **const longNameInfo, char **const shortNameInfo, char **const numericInfo)
{
    if (lineInfo == NULL || longNameInfo == NULL || shortNameInfo == NULL || numericInfo == NULL) {
        return -1;
    }
    char *data = *lineInfo;
    char **longNameInfoStr = (char **)longNameInfo;
    char **shortNameInfoStr = (char **)shortNameInfo;
    char **numericInfoStr = (char **)numericInfo;
    int32_t ret = NextStr(&data, longNameInfoStr);
    if (ret < 0) {
        return -1;
    }
    TELEPHONY_LOGI("ok , longName:%{public}s", *longNameInfoStr);

    ret = NextStr(&data, shortNameInfoStr);
    if (ret < 0) {
        return -1;
    }
    TELEPHONY_LOGI("ok , shortName:%{public}s", *shortNameInfoStr);

    ret = NextStr(&data, numericInfoStr);
    if (ret < 0) {
        return -1;
    }
    TELEPHONY_LOGI("ok , numeric:%{public}s", *numericInfoStr);
    *lineInfo = data;
    return 0;
}

int32_t ParseOperListInfo(
    const char *lineInfo, int32_t count, AvailableOperInfo *pOperInfo, AvailableOperInfo **ppOperInfo)
{
    if (lineInfo == NULL || pOperInfo == NULL) {
        return 0;
    }
    int32_t state = 0;
    int32_t rat = 0;
    int32_t operCount = 0;
    char *line = (char *)lineInfo;
    int32_t item = count;
    for (int32_t i = 0; i < item && operCount < item; i++) {
        int32_t ret = MoveLeftBracket(&line);
        if (ret < 0) {
            break;
        }
        ret = NextInt(&line, &state);
        if (ret < 0) {
            break;
        }
        char *longName = NULL;
        char *shortName = NULL;
        char *numeric = NULL;
        ret = ParseOperInfoItem(&line, &longName, &shortName, &numeric);
        if (ret < 0) {
            break;
        }
        ret = NextIntByRightBracket(&line, &rat);
        if (ret < 0) {
            break;
        }
        if (state > 0) {
            pOperInfo[operCount].status = state;
            TELEPHONY_LOGI("pOperInfo , status:%{public}d", pOperInfo[i].status);
            pOperInfo[operCount].longName = longName;
            TELEPHONY_LOGI("pOperInfo , longName:%{public}s", longName);
            pOperInfo[operCount].numeric = numeric;
            TELEPHONY_LOGI("pOperInfo , numeric:%{public}s", numeric);
            pOperInfo[operCount].shortName = shortName;
            TELEPHONY_LOGI("pOperInfo , shortName:%{public}s", shortName);
            pOperInfo[operCount].rat = rat;
            TELEPHONY_LOGI("pOperInfo , rat:%{public}d", rat);
            operCount++;
        }
    }
    return operCount;
}

int32_t ParseListItemNum(const char *list)
{
    int32_t item = 0;
    if (list == NULL) {
        TELEPHONY_LOGI("ProcessOperListToUse result is null");
    } else {
        char *line = (char *)list;
        while (*line != '\0') {
            if (*line == ')') {
                item++;
            }
            line++;
        }
    }
    return item;
}

static void DealNetworkSearchInformation(
    int32_t operCount, AvailableOperInfo **ppOperInfo, AvailableOperInfo *pOperInfo)
{
    if (operCount == 0) {
        pthread_mutex_lock(&g_networkSearchInformationMutex);
        SetAtPauseFlag(false);
        alarm(0);
        if (g_reportInfoForOperListToUse.requestInfo != NULL) {
            g_reportInfoForOperListToUse.error = HRIL_ERR_INVALID_RESPONSE;
            OnNetworkReport(g_reportInfoForOperListToUse.requestInfo->slotId, g_reportInfoForOperListToUse, NULL, 0);
            g_reportInfoForOperListToUse.requestInfo = NULL;
        }
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
    } else {
        pthread_mutex_lock(&g_networkSearchInformationMutex);
        SetAtPauseFlag(false);
        alarm(0);
        if (g_reportInfoForOperListToUse.requestInfo != NULL) {
            OnNetworkReport(g_reportInfoForOperListToUse.requestInfo->slotId, g_reportInfoForOperListToUse,
                (const uint8_t *)ppOperInfo, operCount);
            g_reportInfoForOperListToUse.requestInfo = NULL;
        }
        pthread_mutex_unlock(&g_networkSearchInformationMutex);
    }
    if (ppOperInfo != NULL) {
        free(ppOperInfo);
    }
    if (pOperInfo != NULL) {
        free(pOperInfo);
    }
}

static int32_t ErrorHandling(void)
{
    TELEPHONY_LOGE("This command resolution is not supported.");
    return HRIL_ERR_GENERIC_FAILURE;
}

static int32_t ParseCellInfoGsm(const char *str, CellInfo *ci)
{
    if (str == NULL || ci == NULL) {
        return -1;
    }
    char *pStr = (char *)str;
    char *pat = NULL;
    (void)memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    TELEPHONY_LOGI("ParseCellInfoGsm %{public}s", pStr);
    if (SkipATPrefix(&pStr) < 0) {
        return ErrorHandling();
    }
    if (NextStr(&pStr, &pat) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.gsm.band) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.gsm.arfcn) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.gsm.bsic) < 0) {
        return ErrorHandling();
    }
    if (NextIntFromHex(&pStr, &ci->ServiceCellParas.gsm.cellId) < 0) {
        return ErrorHandling();
    }
    if (NextIntFromHex(&pStr, &ci->ServiceCellParas.gsm.lac) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.gsm.rxlev) < 0) {
        return ErrorHandling();
    }
    ci->ratType = NETWORK_TYPE_GSM;
    return HRIL_ERR_SUCCESS;
}

static int32_t ParseCellInfoLte(const char *str, CellInfo *ci)
{
    if (str == NULL || ci == NULL) {
        return -1;
    }
    char *pStr = (char *)str;
    char *pat = NULL;
    (void)memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    TELEPHONY_LOGI("ParseCellInfoLte %{public}s", pStr);
    if (SkipATPrefix(&pStr) < 0) {
        return ErrorHandling();
    }
    if (NextStr(&pStr, &pat) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.lte.arfcn) < 0) {
        return ErrorHandling();
    }
    if (NextIntFromHex(&pStr, &ci->ServiceCellParas.lte.pci) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.lte.rsrp) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.lte.rsrq) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.lte.rxlev) < 0) {
        return ErrorHandling();
    }
    ci->ratType = NETWORK_TYPE_LTE;
    return HRIL_ERR_SUCCESS;
}

static int32_t ParseCellInfoWcdma(const char *str, CellInfo *ci)
{
    if (str == NULL || ci == NULL) {
        return -1;
    }
    char *pStr = (char *)str;
    char *pat = NULL;
    (void)memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    TELEPHONY_LOGI("ParseCellInfoWcdma %{public}s", pStr);
    if (SkipATPrefix(&pStr) < 0) {
        return ErrorHandling();
    }
    if (NextStr(&pStr, &pat) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.wcdma.arfcn) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.wcdma.psc) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.wcdma.rscp) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.wcdma.ecno) < 0) {
        return ErrorHandling();
    }
    ci->ratType = NETWORK_TYPE_WCDMA;
    return HRIL_ERR_SUCCESS;
}

static int32_t ParseCellInfoCdma(const char *str, CellInfo *ci)
{
    if (str == NULL || ci == NULL) {
        return -1;
    }
    char *pStr = (char *)str;
    char *pat = NULL;
    (void)memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    TELEPHONY_LOGI("ParseCellInfoCdma %{public}s", pStr);
    if (SkipATPrefix(&pStr) < 0) {
        return ErrorHandling();
    }
    if (NextStr(&pStr, &pat) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.cdma.systemId) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.cdma.networkId) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.cdma.baseId) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.cdma.zoneId) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.cdma.pilotPn) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.cdma.pilotStrength) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.cdma.channel) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.cdma.longitude) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.cdma.latitude) < 0) {
        return ErrorHandling();
    }
    ci->ratType = NETWORK_TYPE_CDMA;
    return HRIL_ERR_SUCCESS;
}

static int32_t ParseCellInfoTdscdma(const char *str, CellInfo *ci)
{
    if (str == NULL || ci == NULL) {
        return -1;
    }
    char *pStr = (char *)str;
    char *pat = NULL;
    (void)memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    TELEPHONY_LOGI("ParseCellInfoTdscdma %{public}s", pStr);
    if (SkipATPrefix(&pStr) < 0) {
        return ErrorHandling();
    }
    if (NextStr(&pStr, &pat) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.tdscdma.arfcn) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.tdscdma.syncId) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.tdscdma.sc) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.tdscdma.cellId) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.tdscdma.lac) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.tdscdma.rscp) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.tdscdma.drx) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.tdscdma.rac) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.tdscdma.cpid) < 0) {
        return ErrorHandling();
    }
    ci->ratType = NETWORK_TYPE_TDSCDMA;
    return HRIL_ERR_SUCCESS;
}

static int32_t ParseCellInfoNr(const char *str, CellInfo *ci)
{
    if (str == NULL || ci == NULL) {
        return -1;
    }
    char *pStr = (char *)str;
    char *pat = NULL;
    (void)memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
    TELEPHONY_LOGI("ParseCellInfoNr %{public}s", pStr);
    if (SkipATPrefix(&pStr) < 0) {
        return ErrorHandling();
    }
    if (NextStr(&pStr, &pat) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.nr.nrArfcn) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.nr.pci) < 0) {
        return ErrorHandling();
    }
    if (NextInt(&pStr, &ci->ServiceCellParas.nr.tac) < 0) {
        return ErrorHandling();
    }
    if (NextInt64(&pStr, &ci->ServiceCellParas.nr.nci) < 0) {
        return ErrorHandling();
    }
    ci->ratType = NETWORK_TYPE_NR;
    return HRIL_ERR_SUCCESS;
}

static int32_t ParseCellInfos(const char *str, const CellInfo *cellInfo)
{
    CellInfo *ci = (CellInfo *)cellInfo;
    char *pStr = (char *)str;
    if (pStr == NULL) {
        TELEPHONY_LOGE("pStr is null.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    TELEPHONY_LOGI("ParseCellInfos  %{public}s", pStr);
    if (ReportStrWith(pStr, "^MONNC: GSM")) {
        if (ParseCellInfoGsm(pStr, ci) != 0) {
            TELEPHONY_LOGE("parse gsm command failed!");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    } else if (ReportStrWith(pStr, "^MONNC: WCDMA")) {
        if (ParseCellInfoWcdma(pStr, ci) != 0) {
            TELEPHONY_LOGE("parse wcdma command failed!");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    } else if (ReportStrWith(pStr, "^MONNC: CDMA")) {
        if (ParseCellInfoCdma(pStr, ci) != 0) {
            TELEPHONY_LOGE("parse cdma command failed!");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    } else if (ReportStrWith(pStr, "^MONNC: TDSCDMA")) {
        if (ParseCellInfoTdscdma(pStr, ci) != 0) {
            TELEPHONY_LOGE("parse tdscdma command failed!");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    } else if (ReportStrWith(pStr, "^MONNC: LTE")) {
        if (ParseCellInfoLte(pStr, ci) != 0) {
            TELEPHONY_LOGE("parse lte command failed!");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    } else if (ReportStrWith(pStr, "^MONNC: NR")) {
        if (ParseCellInfoNr(pStr, ci) != 0) {
            TELEPHONY_LOGE("parse nr command failed!");
            return HRIL_ERR_GENERIC_FAILURE;
        }
    } else if (ReportStrWith(pStr, "^MONNC: NONE")) {
        (void)memset_s(ci, sizeof(CellInfo), 0, sizeof(CellInfo));
        TELEPHONY_LOGI("ParseCellInfos ^MONNC: NONE branch");
    } else {
        TELEPHONY_LOGE("%{public}s This command resolution not supported.", pStr);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    return HRIL_ERR_SUCCESS;
}

void ReqGetNeighboringCellInfoList(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int32_t err = HRIL_ERR_SUCCESS;
    int32_t countCellInfo = 0;
    int32_t index = 0;
    ResponseInfo *responseInfo = NULL;
    Line *pLine = NULL;
    CellInfo *cellInfo = NULL;
    CellInfoList cellInfoList = {0, NULL};
    int32_t ret = SendCommandLock("AT^MONNC", "^MONNC:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    respDataAck.respDataPointer = (uint8_t *)&cellInfoList;
    respDataAck.respDataLen = sizeof(CellInfoList);
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("responseInfo is nullptr");
        err = HRIL_ERR_INVALID_RESPONSE;
        ResponseNetworkReport(HRIL_SIM_SLOT_0, requestInfo, err, &respDataAck);
        return;
    }
    if (ret != 0 || responseInfo->success == 0) {
        TELEPHONY_LOGE("send AT CMD failed!");
        err = (ret != 0) ? HRIL_ERR_GENERIC_FAILURE : HRIL_ERR_CMD_SEND_FAILURE;
        ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        return;
    }
    for (countCellInfo = 0, pLine = responseInfo->head; pLine != NULL; pLine = pLine->next) {
        countCellInfo++;
    }
    if (countCellInfo == 0) {
        ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    TELEPHONY_LOGI("countCellInfo:%{public}d", countCellInfo);
    cellInfo = (CellInfo *)calloc(countCellInfo, sizeof(CellInfo));
    if (cellInfo == NULL) {
        TELEPHONY_LOGE("cellInfoList alloc failed!");
        ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_MEMORY_FULL, &respDataAck);
        return;
    }
    for (pLine = responseInfo->head; pLine != NULL; pLine = pLine->next) {
        ret = ParseCellInfos(pLine->data, &cellInfo[index]);
        if (ret != 0) {
            continue;
        }
        index++;
    }

    cellInfoList.itemNum = index;
    cellInfoList.cellNearbyInfo = cellInfo;
    ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
    free(cellInfo);
    return;
}

static void ParseGetGsmCellInfoLine(char *line, CurrentCellInfoVendor *response)
{
    if (line == NULL || response == NULL) {
        return;
    }
    NextInt(&line, &response->ServiceCellParas.gsm.band);
    NextInt(&line, &response->ServiceCellParas.gsm.arfcn);
    NextInt(&line, &response->ServiceCellParas.gsm.bsic);
    NextIntFromHex(&line, &response->ServiceCellParas.gsm.cellId);
    NextIntFromHex(&line, &response->ServiceCellParas.gsm.lac);
    NextInt(&line, &response->ServiceCellParas.gsm.rxlev);
    NextInt(&line, &response->ServiceCellParas.gsm.rxQuality);
    NextInt(&line, &response->ServiceCellParas.gsm.ta);
    TELEPHONY_LOGI(
        "ParseGetGsmCellInfoLine band:%{public}d,arfcn:%{public}d,bsic:%{public}d,cellId:%{public}d"
        "lac:%{public}d,rxlev:%{public}d,rxQuality:%{public}d,ta:%{public}d",
        response->ServiceCellParas.gsm.band, response->ServiceCellParas.gsm.arfcn,
        response->ServiceCellParas.gsm.bsic, response->ServiceCellParas.gsm.cellId,
        response->ServiceCellParas.gsm.lac, response->ServiceCellParas.gsm.rxlev,
        response->ServiceCellParas.gsm.rxQuality, response->ServiceCellParas.gsm.ta);
}

static void ParseGetLteCellInfoLine(char *line, CurrentCellInfoVendor *response)
{
    if (line == NULL || response == NULL) {
        return;
    }
    NextInt(&line, &response->ServiceCellParas.lte.arfcn);
    NextIntFromHex(&line, &response->ServiceCellParas.lte.cellId);
    NextIntFromHex(&line, &response->ServiceCellParas.lte.pci);
    NextIntFromHex(&line, &response->ServiceCellParas.lte.tac);
    NextInt(&line, &response->ServiceCellParas.lte.rsrp);
    NextInt(&line, &response->ServiceCellParas.lte.rsrq);
    NextInt(&line, &response->ServiceCellParas.lte.rssi);
    TELEPHONY_LOGI(
        "ParseGetLteCellInfoLine arfcn:%{public}d,cellId:%{public}d,pci:%{public}d"
        "tac:%{public}d,rsrp:%{public}d,rsrq:%{public}d,rssi:%{public}d",
        response->ServiceCellParas.lte.arfcn, response->ServiceCellParas.lte.cellId,
        response->ServiceCellParas.lte.pci, response->ServiceCellParas.lte.tac, response->ServiceCellParas.lte.rsrp,
        response->ServiceCellParas.lte.rsrq, response->ServiceCellParas.lte.rssi);
}

static void ParseGetWcdmaCellInfoLine(char *line, CurrentCellInfoVendor *response)
{
    if (line == NULL || response == NULL) {
        return;
    }
    NextInt(&line, &response->ServiceCellParas.wcdma.arfcn);
    NextInt(&line, &response->ServiceCellParas.wcdma.psc);
    NextIntFromHex(&line, &response->ServiceCellParas.wcdma.cellId);
    NextIntFromHex(&line, &response->ServiceCellParas.wcdma.lac);
    NextInt(&line, &response->ServiceCellParas.wcdma.rscp);
    NextInt(&line, &response->ServiceCellParas.wcdma.rxlev);
    NextInt(&line, &response->ServiceCellParas.wcdma.ecno);
    NextInt(&line, &response->ServiceCellParas.wcdma.drx);
    NextInt(&line, &response->ServiceCellParas.wcdma.ura);
    TELEPHONY_LOGI(
        "ParseGetWcdmaCellInfoLine arfcn:%{public}d,psc:%{public}d,cellId:%{public}d,lac:%{public}d"
        "rscp:%{public}d,rxlev:%{public}d,ecno:%{public}d,drx:%{public}d,ura:%{public}d",
        response->ServiceCellParas.wcdma.arfcn, response->ServiceCellParas.wcdma.psc,
        response->ServiceCellParas.wcdma.cellId, response->ServiceCellParas.wcdma.lac,
        response->ServiceCellParas.wcdma.rscp, response->ServiceCellParas.wcdma.rxlev,
        response->ServiceCellParas.wcdma.ecno, response->ServiceCellParas.wcdma.drx,
        response->ServiceCellParas.wcdma.ura);
}

static void ParseGetCdmaCellInfoLine(char *line, CurrentCellInfoVendor *response)
{
    if (line == NULL || response == NULL) {
        return;
    }
    NextInt(&line, &response->ServiceCellParas.cdma.systemId);
    NextInt(&line, &response->ServiceCellParas.cdma.networkId);
    NextIntFromHex(&line, &response->ServiceCellParas.cdma.baseId);
    NextIntFromHex(&line, &response->ServiceCellParas.cdma.zoneId);
    NextInt(&line, &response->ServiceCellParas.cdma.pilotPn);
    NextInt(&line, &response->ServiceCellParas.cdma.pilotStrength);
    NextInt(&line, &response->ServiceCellParas.cdma.channel);
    NextInt(&line, &response->ServiceCellParas.cdma.longitude);
    NextInt(&line, &response->ServiceCellParas.cdma.latitude);
    TELEPHONY_LOGI(
        "ParseGetCdmaCellInfoLine systemId:%{public}d,networkId:%{public}d,baseId:%{public}d,zoneId:%{public}d"
        "pilotPn:%{public}d,pilotStrength:%{public}d,channel:%{public}d,longitude:%{public}d,latitude:%{public}d",
        response->ServiceCellParas.cdma.systemId, response->ServiceCellParas.cdma.networkId,
        response->ServiceCellParas.cdma.baseId, response->ServiceCellParas.cdma.zoneId,
        response->ServiceCellParas.cdma.pilotPn, response->ServiceCellParas.cdma.pilotStrength,
        response->ServiceCellParas.cdma.channel, response->ServiceCellParas.cdma.longitude,
        response->ServiceCellParas.cdma.latitude);
}

static void ParseGetTdscdmaCellInfoLine(char *line, CurrentCellInfoVendor *response)
{
    if (line == NULL || response == NULL) {
        return;
    }
    NextInt(&line, &response->ServiceCellParas.tdscdma.arfcn);
    NextInt(&line, &response->ServiceCellParas.tdscdma.syncId);
    NextInt(&line, &response->ServiceCellParas.tdscdma.sc);
    NextIntFromHex(&line, &response->ServiceCellParas.tdscdma.cellId);
    NextIntFromHex(&line, &response->ServiceCellParas.tdscdma.lac);
    NextInt(&line, &response->ServiceCellParas.tdscdma.rscp);
    NextInt(&line, &response->ServiceCellParas.tdscdma.drx);
    NextIntFromHex(&line, &response->ServiceCellParas.tdscdma.rac);
    NextInt(&line, &response->ServiceCellParas.tdscdma.cpid);
    TELEPHONY_LOGI(
        "ParseGetTdscdmaCellInfoLine arfcn:%{public}d,syncId:%{public}d,sc:%{public}d,cellId:%{public}d,"
        "lac:%{public}d,rscp:%{public}d,drx:%{public}d,rac:%{public}d,cpid:%{public}d,",
        response->ServiceCellParas.tdscdma.arfcn, response->ServiceCellParas.tdscdma.syncId,
        response->ServiceCellParas.tdscdma.sc, response->ServiceCellParas.tdscdma.cellId,
        response->ServiceCellParas.tdscdma.lac, response->ServiceCellParas.tdscdma.rscp,
        response->ServiceCellParas.tdscdma.drx, response->ServiceCellParas.tdscdma.rac,
        response->ServiceCellParas.tdscdma.cpid);
}

static void ParseGetNrCellInfoLine(char *line, CurrentCellInfoVendor *response)
{
    if (line == NULL || response == NULL) {
        return;
    }
    NextInt(&line, &response->ServiceCellParas.nr.nrArfcn);
    NextInt(&line, &response->ServiceCellParas.nr.pci);
    NextInt(&line, &response->ServiceCellParas.nr.tac);
    NextInt64(&line, &response->ServiceCellParas.nr.nci);
}

static int32_t ParseGetCurrentCellInfoResponseLineSwitch(const char *str, const CurrentCellInfoVendor *hrilResponse)
{
    char *line = (char *)str;
    CurrentCellInfoVendor *response = (CurrentCellInfoVendor *)hrilResponse;
    if ((line == NULL) || (response == NULL)) {
        TELEPHONY_LOGE("ParseGetCurrentCellInfoResponseLineSwitch line or response param is null");
        return HRIL_ERR_NULL_POINT;
    }
    switch (response->ratType) {
        case NETWORK_TYPE_GSM:
            ParseGetGsmCellInfoLine(line, response);
            break;
        case NETWORK_TYPE_LTE:
            ParseGetLteCellInfoLine(line, response);
            break;
        case NETWORK_TYPE_WCDMA:
            ParseGetWcdmaCellInfoLine(line, response);
            break;
        case NETWORK_TYPE_CDMA:
            ParseGetCdmaCellInfoLine(line, response);
            break;
        case NETWORK_TYPE_TDSCDMA:
            ParseGetTdscdmaCellInfoLine(line, response);
            break;
        case NETWORK_TYPE_NR:
            ParseGetNrCellInfoLine(line, response);
            break;
        default:
            TELEPHONY_LOGE("ParseGetCellInfoResponseLineSwitch is not support cell, line = %{public}s", line);
            break;
    }
    return HRIL_ERR_SUCCESS;
}

static int32_t ParseGetCurrentCellInfoResponseLine(char *line, CurrentCellInfoVendor *response)
{
    if (line == NULL || response == NULL) {
        return -1;
    }
    char *c = NULL;
    TELEPHONY_LOGI("ParseGetCurrentCellInfoResponseLine  line %{public}s", line);
    int32_t err = SkipATPrefix(&line);
    if (err < 0) {
        return err;
    }
    err = NextStr(&line, &c);
    if (err < 0) {
        return err;
    }
    if (!strcmp(c, "GSM")) {
        response->ratType = NETWORK_TYPE_GSM;
    } else if (!strcmp(c, "LTE")) {
        response->ratType = NETWORK_TYPE_LTE;
    } else if (!strcmp(c, "WCDMA")) {
        response->ratType = NETWORK_TYPE_WCDMA;
    } else if (!strcmp(c, "CDMA")) {
        response->ratType = NETWORK_TYPE_CDMA;
    } else if (!strcmp(c, "TDSCDMA")) {
        response->ratType = NETWORK_TYPE_TDSCDMA;
    } else if (!strcmp(c, "NR")) {
        response->ratType = NETWORK_TYPE_NR;
    } else if (!strcmp(c, "NONE")) {
        (void)memset_s(response, sizeof(CurrentCellInfoVendor), 0, sizeof(CurrentCellInfoVendor));
        response->ratType = NETWORK_TYPE_UNKNOWN;
        return HRIL_ERR_SUCCESS;
    } else {
        TELEPHONY_LOGI("ParseCurrentCellInfo unSupport ratType line %{public}s", line);
        return -1;
    }
    err = NextInt(&line, &response->mcc);
    if (err < 0) {
        return err;
    }
    err = NextInt(&line, &response->mnc);
    if (err < 0) {
        return err;
    }
    ParseGetCurrentCellInfoResponseLineSwitch(line, response);
    return HRIL_ERR_SUCCESS;
}

int32_t ProcessCurrentCellList(struct ReportInfo reportInfo, const char *s)
{
    char *str = (char *)s;
    if (str == NULL) {
        TELEPHONY_LOGE("ProcessCurrentCellList Str  is null.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t err = SkipATPrefix(&str);
    if (err < 0) {
        TELEPHONY_LOGE("ProcessCurrentCellList skip failed: [%{public}s]", str);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t semicolonNum = FindSemicolonCharNum(str);
    if (semicolonNum > 0) {
        CurrentCellInfoVendor *cciv = NULL;
        CurrentCellInfoList cellList = {0, NULL};
        cciv = (CurrentCellInfoVendor *)calloc(semicolonNum, sizeof(CurrentCellInfoVendor));
        if (cciv == NULL) {
            TELEPHONY_LOGE("^MONSC: notify CurrentCellInfoVendor alloc failed!");
            return HRIL_ERR_MEMORY_FULL;
        }
        for (int32_t i = 0; i < semicolonNum; i++) {
            char *pStr = strsep(&str, ";");
            if (ParseGetCurrentCellInfoResponseLine(pStr, &cciv[cellList.itemNum]) != 0) {
                continue;
            }
            cellList.itemNum++;
        }
        cellList.currentCellInfo = cciv;
        reportInfo.error = HRIL_ERR_SUCCESS;
        reportInfo.type = HRIL_NOTIFICATION;
        reportInfo.notifyId = HNOTI_NETWORK_CURRENT_CELL_UPDATED;
        OnNetworkReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&cellList,
            sizeof(CurrentCellInfoList));
        if (cellList.currentCellInfo != NULL) {
            free(cellList.currentCellInfo);
        }
        return HRIL_ERR_SUCCESS;
    } else {
        TELEPHONY_LOGW("^MONSC: notify str format  unexpected: %{public}s", str);
        return HRIL_ERR_GENERIC_FAILURE;
    }
}

void ReqGetCurrentCellInfo(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int32_t err = HRIL_ERR_SUCCESS;
    int32_t countCellInfo = 0;
    int32_t index = 0;
    ResponseInfo *responseInfo = NULL;
    Line *pLine = NULL;
    CurrentCellInfoVendor *currCellInfo = NULL;
    CurrentCellInfoList currCellInfoList = {0, NULL};
    int32_t ret = SendCommandLock("AT^MONSC", "^MONSC:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    respDataAck.respDataPointer = (uint8_t *)&currCellInfoList;
    respDataAck.respDataLen = sizeof(CurrentCellInfoList);
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("AT^MONSC responseInfo is nullptr");
        ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    if (ret != 0 || responseInfo->success == 0) {
        TELEPHONY_LOGE("AT^MONSC send AT CMD failed!");
        err = (ret != 0) ? HRIL_ERR_GENERIC_FAILURE : HRIL_ERR_CMD_SEND_FAILURE;
        ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        return;
    }
    for (countCellInfo = 0, pLine = responseInfo->head; pLine != NULL; pLine = pLine->next) {
        countCellInfo++;
    }
    if (countCellInfo == 0) {
        ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    currCellInfo = (CurrentCellInfoVendor *)calloc(countCellInfo, sizeof(CurrentCellInfoVendor));
    if (currCellInfo == NULL) {
        TELEPHONY_LOGE("currentCellList  countCellInfo:%{public}d,currCellInfo alloc failed!", countCellInfo);
        ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_MEMORY_FULL, &respDataAck);
        return;
    }
    for (pLine = responseInfo->head; pLine != NULL; pLine = pLine->next) {
        if (ParseGetCurrentCellInfoResponseLine(pLine->data, &currCellInfo[index]) != 0) {
            continue;
        }
        index++;
    }
    currCellInfoList.itemNum = index;
    currCellInfoList.currentCellInfo = currCellInfo;
    ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
    free(currCellInfo);
    return;
}

static int32_t OperListErrorHandler(AvailableOperInfo **ppOperInfo, AvailableOperInfo *pOperInfo)
{
    TELEPHONY_LOGE("ReqGetNetworkSearchInformation Failed");
    DealNetworkSearchInformation(0, ppOperInfo, pOperInfo);
    return HRIL_ERR_GENERIC_FAILURE;
}

int32_t ProcessOperListToUse(const char *list)
{
    const int32_t UNUSED_ITEM_COUNT = 2;
    AvailableOperInfo **ppOperInfo = NULL;
    AvailableOperInfo *pOperInfo = NULL;
    SetAtPauseFlag(false);
    alarm(0);
    int32_t item = ParseListItemNum(list);
    if (item <= UNUSED_ITEM_COUNT) {
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }
    char *line = (char *)list;
    int32_t ret = SkipATPrefix(&line);
    if (ret < 0) {
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }

    item = item - UNUSED_ITEM_COUNT;
    ppOperInfo = (AvailableOperInfo **)malloc(item * sizeof(size_t));
    if (!ppOperInfo) {
        TELEPHONY_LOGE("ppOperInfo malloc fail");
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }

    pOperInfo = (AvailableOperInfo *)malloc(item * sizeof(AvailableOperInfo));
    if (!pOperInfo) {
        TELEPHONY_LOGE("pOperInfo malloc fail");
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }
    (void)memset_s(pOperInfo, item * sizeof(AvailableOperInfo), 0, item * sizeof(AvailableOperInfo));
    for (int32_t j = 0; j < item; j++) {
        ppOperInfo[j] = &(pOperInfo[j]);
    }

    int32_t operCount = ParseOperListInfo(line, item, pOperInfo, ppOperInfo);
    if (operCount != 0) {
        DealNetworkSearchInformation(operCount, ppOperInfo, pOperInfo);
    } else {
        return OperListErrorHandler(ppOperInfo, pOperInfo);
    }
    return HRIL_ERR_SUCCESS;
}

static bool PrepareSetNetworkSelectionMode(char *cmd, const HRilSetNetworkModeInfo *setModeInfo)
{
    if (cmd == NULL || setModeInfo == NULL) {
        return false;
    }
    bool ret = true;
    TELEPHONY_LOGI("setModeInfo, serial123 = %{public}d", setModeInfo->selectMode);
    if (setModeInfo->selectMode == 0) {
        (void)sprintf_s(cmd, MAX_CMD_LENGTH, "%s", "AT+COPS=0");
    } else if (setModeInfo->selectMode == 1) {
        if (setModeInfo->oper == NULL) {
            ret = false;
        } else {
            (void)sprintf_s(cmd, MAX_CMD_LENGTH, "AT+COPS=1,2,%s", setModeInfo->oper);
        }
    } else {
        ret = false;
    }
    return ret;
}

void ReqSetNetworkSelectionMode(const ReqDataInfo *requestInfo, const HRilSetNetworkModeInfo *data)
{
    if (requestInfo == NULL) {
        return;
    }

    ResponseInfo *responseInfo = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char *cmdBuff = cmd;
    struct ReportInfo reportInfo;
    HRilSetNetworkModeInfo *setModeInfo = (HRilSetNetworkModeInfo *)data;
    if (setModeInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode HRIL_ERR_NULL_POINT");
        return;
    }
    if (!PrepareSetNetworkSelectionMode(cmdBuff, setModeInfo)) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode HRIL_ERR_INVALID_PARAMETER");
        return;
    }
    TELEPHONY_LOGI("requestSetAutomaticModeForNetworks, cmd = %{public}s", cmd);
    int32_t err = SendCommandLock(cmd, NULL, 0, &responseInfo);

    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 1);
        TELEPHONY_LOGE("SetAutomaticMode responseInfo == NULL");
        return;
    }
    TELEPHONY_LOGI("SetAutomaticModeForNetworks, responseInfo->success = %{public}d", responseInfo->success);
    if (err != 0 || responseInfo->success == 0) {
        TELEPHONY_LOGE("ret is not equal to HDF_SUCCESS!");
        err = GetResponseErrorCode(responseInfo);
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        TELEPHONY_LOGE("SetAutomaticMode HRIL_ERR_GENERIC_FAILURE");
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqGetNetworkSelectionMode(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    int32_t state = 0;
    struct ReportInfo reportInfo;
    int32_t ret = SendCommandLock("AT+COPS?", "+COPS:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("responseInfo is nullptr");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
        ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        return;
    }
    if (responseInfo->head == NULL) {
        TELEPHONY_LOGE("no data!");
        err = HRIL_ERR_NULL_POINT;
        ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        return;
    }
    char *line = responseInfo->head->data;
    ret = SkipATPrefix(&line);
    if (ret < 0) {
        err = HRIL_ERR_INVALID_RESPONSE;
        TELEPHONY_LOGE("Response is Invalid!");
        ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        return;
    }
    ret = NextInt(&line, &state);
    if (ret < 0) {
        err = HRIL_ERR_INVALID_RESPONSE;
        TELEPHONY_LOGE("Response is Invalid!");
        ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        return;
    }
    int32_t mode = (int32_t)state;
    respDataAck.respDataPointer = (uint8_t *)&mode;
    respDataAck.respDataLen = sizeof(int32_t);
    ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_SUCCESS, &respDataAck);
    NotifyNetWorkTime(requestInfo->slotId);
    return;
}

static int32_t IntToNetTypeCmd(int32_t value, char *dst, int32_t dstMaxSize)
{
    int32_t len = MAX_CMD_LENGTH - 1;
    (void)strcat_s(dst, len, "AT^SYSCFGEX=\"");
    const int32_t MAX_PREFERRED_NET_ENUM = 99;
    const int32_t MIN_CHAR_ARRAY_SIZE = 50;
    const int32_t CONVERT_FAIL = -1;
    const int32_t DECIMAL = 10;
    if ((value > MAX_PREFERRED_NET_ENUM) || (value < 0) || (dstMaxSize < MIN_CHAR_ARRAY_SIZE)) {
        return CONVERT_FAIL;
    }
    int32_t pos = strlen(dst);
    if (value < DECIMAL) {
        dst[pos++] = '0';
        dst[pos++] = (value % DECIMAL) + '0';
    } else {
        dst[pos++] = (value / DECIMAL) + '0';
        dst[pos++] = (value % DECIMAL) + '0';
    }
    dst[pos++] = '\"';
    dst[pos] = '\0';
    return 0;
}

static char *PrepareCommandByNetworkType(HRilPreferredNetworkType net)
{
    char *cmd = (char *)calloc(1, MAX_CMD_LENGTH);
    if (cmd != NULL) {
        TELEPHONY_LOGI("SetPreferredNetwork, net = %{public}d", net);
        if (net == HRIL_NETWORK_AUTO) {
            GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"00\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
        } else if (net == HRIL_NETWORK_GSM) {
            GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"01\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
        } else if (net == HRIL_NETWORK_WCDMA) {
            GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"02\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
        } else if (net == HRIL_NETWORK_LTE) {
            GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"03\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
        } else if (net == HRIL_NETWORK_LTE_WCDMA_GSM) {
            GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"030201\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
        } else if (net == HRIL_NETWORK_WCDMA_GSM) {
            GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"0201\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
        } else if (net == HRIL_NETWORK_LTE_WCDMA) {
            GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", "AT^SYSCFGEX=\"0302\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,0,0");
        } else {
            if (((net > HRIL_NETWORK_WCDMA_GSM) && (net <= HRIL_NETWORK_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA)) ||
                ((net >= HRIL_NETWORK_NR) && (net <= HRIL_NETWORK_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA))) {
                char dst[MAX_CMD_LENGTH] = {0};
                IntToNetTypeCmd(net, dst, MAX_CMD_LENGTH);
                GenerateCommand(cmd, MAX_CMD_LENGTH, "%s", dst);
            } else {
                free(cmd);
                cmd = NULL;
            }
        }
    }
    return cmd;
}

void ReqSetPreferredNetwork(const ReqDataInfo *requestInfo, const int32_t *data)
{
    if (requestInfo == NULL) {
        return;
    }
    ResponseInfo *responseInfo = NULL;
    struct ReportInfo reportInfo;
    if (data == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }
    int32_t net = *(int32_t *)data;
    int32_t err = HRIL_ERR_INVALID_PARAMETER;
    char *cmd = PrepareCommandByNetworkType(net);
    if (cmd == NULL) {
        TELEPHONY_LOGE("RequestReqSetPreferredNetwork  HRIL_ERR_INVALID_PARAMETER cmd NULL");
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }
    err = SendCommandLock(cmd, NULL, 0, &responseInfo);
    if (cmd != NULL) {
        free(cmd);
        cmd = NULL;
    }
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }
    if (err != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

static int32_t ParseNetTypeStr(const char *netType)
{
    TELEPHONY_LOGI("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    if (netType == NULL) {
        TELEPHONY_LOGE("ParseNetTypeStr netType is null");
        return -1;
    }
    TELEPHONY_LOGI("netType: [%{public}s]", netType);
    if (strcmp(netType, AUTO_TYPE) == 0) {
        return HRIL_NETWORK_AUTO;
    } else if (strcmp(netType, GSM_TYPE) == 0) {
        return HRIL_NETWORK_GSM;
    } else if (strcmp(netType, WCDMA_TYPE) == 0) {
        return HRIL_NETWORK_WCDMA;
    } else if (strcmp(netType, LTE_TYPE) == 0) {
        return HRIL_NETWORK_LTE;
    } else if (strcmp(netType, LTE_WCDMA_TYPE) == 0) {
        return HRIL_NETWORK_LTE_WCDMA;
    } else if (strcmp(netType, LTE_WCDMA_GSM_TYPE) == 0) {
        return HRIL_NETWORK_LTE_WCDMA_GSM;
    } else if (strcmp(netType, WCDMA_GSM_TYPE) == 0) {
        return HRIL_NETWORK_WCDMA_GSM;
    } else {
        int32_t net = ConvertCharToInt32(netType);
        if (((net > HRIL_NETWORK_WCDMA_GSM) && (net <= HRIL_NETWORK_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA)) ||
            ((net >= HRIL_NETWORK_NR) && (net <= HRIL_NETWORK_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA))) {
            return net;
        } else {
            return -1;
        }
    }
}

void ReqGetPreferredNetwork(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    char *netTypeStr = "";
    int32_t ret = SendCommandLock("AT^SYSCFGEX?", "^SYSCFGEX:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("responseInfo is null");
        ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_NULL_POINT, &respDataAck);
        return;
    }
    respDataAck.responseInfo = responseInfo;
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
        ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        return;
    }
    if (responseInfo->head == NULL) {
        ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    Line *pLine = responseInfo->head;
    char *line = pLine->data;
    if (SkipATPrefix(&line) < 0) {
        ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    if (NextStr(&line, &netTypeStr) < 0) {
        ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    int32_t netType = ParseNetTypeStr(netTypeStr);
    respDataAck.respDataPointer = (uint8_t *)&netType;
    respDataAck.respDataLen = sizeof(int32_t);
    ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_SUCCESS, &respDataAck);
}

static HRilRadioCapability *GetRadioCapability(void)
{
    static HRilRadioCapability hRilRadioCapability = {0};
    return &hRilRadioCapability;
}

void ReqGetRadioCapability(const ReqDataInfo *requestInfo)
{
    int32_t err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    TELEPHONY_LOGE("ReqGetRadioCapability before ratFamily:%{public}d", GetRadioCapability()->ratFamily);
    GetRadioCapability()->ratFamily = RAF_AUTO;
    TELEPHONY_LOGE("ReqGetRadioCapability OnNetworkReport ");
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(
        requestInfo->slotId, reportInfo, (const uint8_t *)GetRadioCapability(), sizeof(HRilRadioCapability));
}

static int32_t ExtractConfigInfo(const char *str, const HRilPhyChannelConfig *config)
{
    HRilPhyChannelConfig *cfg = (HRilPhyChannelConfig *)config;
    char *pStr = (char *)str;
    if (pStr == NULL || cfg == NULL) {
        TELEPHONY_LOGE("pStr or cfg is null.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (NextInt(&pStr, &cfg->cellConnStatus) < 0) {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    if (NextInt(&pStr, &cfg->cellBandwidth) < 0) {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    if (NextInt(&pStr, &cfg->ratType) < 0) {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    if (NextInt(&pStr, &cfg->freqRange) < 0) {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    if (NextInt(&pStr, &cfg->channelNum) < 0) {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    if (NextInt(&pStr, &cfg->physicalCellId) < 0) {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    if (NextInt(&pStr, &cfg->contextIdNum) < 0) {
        return HRIL_ERR_INVALID_RESPONSE;
    }
    int32_t *contextIds = NULL;
    if (cfg->contextIdNum > 0) {
        contextIds = (int32_t *)calloc(cfg->contextIdNum, sizeof(int32_t));
        if (contextIds == NULL) {
            TELEPHONY_LOGE("contextIds alloc failed!");
            return HRIL_ERR_MEMORY_FULL;
        }
        for (int32_t i = 0; i < cfg->contextIdNum; i++) {
            if (NextInt(&pStr, &(contextIds[i])) < 0) {
                free(contextIds);
                return HRIL_ERR_INVALID_RESPONSE;
            }
        }
    } else {
        cfg->contextIdNum = 0;
    }
    cfg->contextIds = contextIds;
    return HRIL_ERR_SUCCESS;
}

void ProcessPhyChnlCfgNotify(struct ReportInfo reportInfo, char *srcStr)
{
    char *str = (char *)srcStr;
    if (str == NULL) {
        TELEPHONY_LOGE("ProcessPhyChnlCfgNotify Str  is null.");
        return;
    }
    int32_t err = SkipATPrefix(&str);
    if (err < 0) {
        TELEPHONY_LOGE("ProcessPhyChnlCfgNotify skip failed: [%{public}s]", str);
        return;
    }
    int32_t semicolonNum = FindSemicolonCharNum(str);
    if (semicolonNum > 0) {
        HRilPhyChannelConfig *configInfo = NULL;
        HRilChannelConfigList configInfoList = {0, NULL};
        configInfo = (HRilPhyChannelConfig *)calloc(semicolonNum, sizeof(HRilPhyChannelConfig));
        if (configInfo == NULL) {
            TELEPHONY_LOGE("^PHYCHLCFG: notify configInfo alloc failed!");
            return;
        }
        for (int32_t i = 0; i < semicolonNum; i++) {
            char *pStr = strsep(&str, ";");
            int32_t ret = ExtractConfigInfo(pStr, &configInfo[configInfoList.itemNum]);
            if (ret != 0) {
                continue;
            }
            configInfoList.itemNum++;
        }
        configInfoList.channelConfigs = configInfo;
        reportInfo.error = HRIL_ERR_SUCCESS;
        reportInfo.type = HRIL_NOTIFICATION;
        reportInfo.notifyId = HNOTI_NETWORK_PHY_CHNL_CFG_UPDATED;
        OnNetworkReport(GetSlotId(NULL), reportInfo, (const uint8_t *)&configInfoList,
            sizeof(HRilChannelConfigList));
        for (int32_t i = 0; i < configInfoList.itemNum; i++) {
            if (configInfoList.channelConfigs[i].contextIds != NULL) {
                free(configInfoList.channelConfigs[i].contextIds);
            }
        }
        if (configInfoList.channelConfigs != NULL) {
            free(configInfoList.channelConfigs);
        }
    } else {
        TELEPHONY_LOGW("^PHYCHLCFG: notify str format  unexpected: %{public}s", str);
    }
}

static int32_t ParseConfigInfo(const char *str, const HRilPhyChannelConfig *config)
{
    HRilPhyChannelConfig *cfg = (HRilPhyChannelConfig *)config;
    char *pStr = (char *)str;
    if (pStr == NULL || cfg == NULL) {
        TELEPHONY_LOGE("pStr or cfg is null.");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (ReportStrWith(pStr, "^PHYCHLCFG:") == 0) {
        TELEPHONY_LOGE("skip failed: [%{public}s]", str);
        return HRIL_ERR_INVALID_RESPONSE;
    }
    int32_t err = SkipATPrefix(&pStr);
    if (err < 0) {
        TELEPHONY_LOGE("skip failed: [%{public}s]", str);
        return HRIL_ERR_INVALID_RESPONSE;
    }
    return ExtractConfigInfo(pStr, config);
}

void ReqGetPhysicalChannelConfig(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    const long TIME_OUT = DEFAULT_TIMEOUT;
    int32_t err = HRIL_ERR_SUCCESS;
    int32_t configCount = 0;
    ResponseInfo *responseInfo = NULL;
    Line *pLine = NULL;
    HRilPhyChannelConfig *configInfo = NULL;
    HRilChannelConfigList configInfoList = {0, NULL};
    int32_t ret = SendCommandLock("AT^PHYCHLCFG", "^PHYCHLCFG:", TIME_OUT, &responseInfo);
    struct ResponseAck respDataAck = {responseInfo, NULL, 0};
    respDataAck.respDataPointer = (uint8_t *)&configInfoList;
    respDataAck.respDataLen = sizeof(HRilChannelConfigList);
    if (ret != 0 || !responseInfo->success) {
        err = (responseInfo->success == 0) ? HRIL_ERR_CMD_SEND_FAILURE : HRIL_ERR_GENERIC_FAILURE;
        ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    for (configCount = 0, pLine = responseInfo->head; pLine != NULL; pLine = pLine->next) {
        configCount++;
    }
    TELEPHONY_LOGI("configCount:%{public}d", configCount);
    if (configCount > 0) {
        configInfo = (HRilPhyChannelConfig *)calloc(configCount, sizeof(HRilPhyChannelConfig));
        if (configInfo == NULL) {
            TELEPHONY_LOGE("configInfo alloc failed!");
            ResponseNetworkReport(requestInfo->slotId, requestInfo, HRIL_ERR_MEMORY_FULL, &respDataAck);
            return;
        }
        for (pLine = responseInfo->head; pLine != NULL; pLine = pLine->next) {
            ret = ParseConfigInfo(pLine->data, &configInfo[configInfoList.itemNum]);
            if (ret != 0) {
                continue;
            }
            configInfoList.itemNum++;
        }
        configInfoList.channelConfigs = configInfo;
    }
    ResponseNetworkReport(requestInfo->slotId, requestInfo, err, &respDataAck);
    for (int32_t i = 0; i < configInfoList.itemNum; i++) {
        if (configInfoList.channelConfigs[i].contextIds != NULL) {
            free(configInfoList.channelConfigs[i].contextIds);
        }
    }
    if (configInfoList.channelConfigs != NULL) {
        free(configInfoList.channelConfigs);
    }
    return;
}

void ReqSetLocateUpdates(const ReqDataInfo *requestInfo, HRilRegNotifyMode mode)
{
    if (requestInfo == NULL) {
        return;
    }
    const long TIME_OUT = DEFAULT_TIMEOUT;
    ResponseInfo *responseInfo = NULL;
    struct ReportInfo reportInfo = {};
    char *cmd = NULL;
    if (mode == REG_NOTIFY_STAT_LAC_CELLID) {
        cmd = "AT+CREG=2";
    } else if (mode == REG_NOTIFY_STAT_ONLY) {
        cmd = "AT+CREG=1";
    } else {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_PARAMETER, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        TELEPHONY_LOGE("ReqSetLocateUpdates:  locateUpdateMode > 1");
        return;
    }
    int32_t err = SendCommandLock(cmd, NULL, TIME_OUT, &responseInfo);
    if (responseInfo == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
        TELEPHONY_LOGE("ReqSetLocateUpdates responseInfo == NULL");
        return;
    }
    TELEPHONY_LOGI("ReqSetLocateUpdates, responseInfo->success = %{public}d", responseInfo->success);
    if (err != 0 || responseInfo->success == 0) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("ReqSetLocateUpdates errcode = %{public}d", err);
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnNetworkReport(requestInfo->slotId, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void NotifyNetWorkTime(int32_t slotId)
{
    const long TIME_OUT = DEFAULT_TIMEOUT;
    ResponseInfo *responseInfo = NULL;
    const int32_t TIME_VALUE_LEN = 30;
    const size_t TIME_VALUE_OFFSET = 2;
    char timeStr[TIME_VALUE_LEN] = {0};
    struct ReportInfo reportInfo = {0};

    int32_t ret = SendCommandLock("AT+CCLK?", "+CCLK:", TIME_OUT, &responseInfo);
    if (ret != 0 || responseInfo->success == 0) {
        TELEPHONY_LOGE("send AT CMD failed!");
        return;
    }
    if (responseInfo->head == NULL) {
        TELEPHONY_LOGE("no data!");
        return;
    }
    Line *pLine = responseInfo->head;
    if (pLine->data == NULL) {
        TELEPHONY_LOGE("no data!");
        return;
    }
    char *line = pLine->data;
    ret = SkipATPrefix(&line);
    if (ret < 0) {
        TELEPHONY_LOGE("format error!");
        return;
    }
    size_t len = strlen(line) - 1;
    if (len <= TIME_VALUE_OFFSET) {
        TELEPHONY_LOGE("invalid len!");
        return;
    }
    for (size_t i = TIME_VALUE_OFFSET; i < len; i++) {
        timeStr[i - TIME_VALUE_OFFSET] = *(line + i);
    }
    TELEPHONY_LOGI("netTime:%{public}s", timeStr);
    reportInfo.notifyId = HNOTI_NETWORK_TIME_UPDATED;
    reportInfo.type = HRIL_NOTIFICATION;
    reportInfo.error = HRIL_ERR_SUCCESS;
    OnNetworkReport(GetSlotId(NULL), reportInfo, (const uint8_t *)timeStr, TIME_VALUE_LEN);
    FreeResponseInfo(responseInfo);
}