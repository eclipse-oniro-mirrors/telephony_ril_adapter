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

#include "at_modem.h"
#include "vendor_adapter.h"
#include "vendor_report.h"
#include "vendor_util.h"

struct ResponseAck {
    ResponseInfo *responseInfo;
    uint8_t *respDataPointer;
    int32_t respDataLen;
};

static int32_t ResponseModemReport(
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
    OnModemReport(slotId, reportInfo, (const uint8_t *)(respDataAck->respDataPointer), respDataAck->respDataLen);
    if (respDataAck->responseInfo != NULL) {
        FreeResponseInfo(respDataAck->responseInfo);
    }
    return err;
}

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
    TELEPHONY_LOGD("modem response error code: %{public}d", ret);
    return ret;
}

void ReqSetRadioState(const ReqDataInfo *requestInfo, int32_t function, int32_t reset)
{
    if (requestInfo == NULL) {
        return;
    }
    struct ReportInfo reportInfo;
    int32_t ret = SetRadioState(function, reset);
    if (ret == HRIL_ERR_SUCCESS) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    } else {
        TELEPHONY_LOGE("ReqSetRadioState failed");
        if (ret == HRIL_ERR_REPEAT_STATUS) {
            reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_REPEAT_STATUS, HRIL_RESPONSE, 0);
        } else {
            reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        }
    }
    OnModemReport(requestInfo->slotId, reportInfo, NULL, 0);
}

static void ErrorHandler(const ReqDataInfo *requestInfo, ResponseInfo *pResponse)
{
    if (requestInfo == NULL) {
        return;
    }
    struct ReportInfo reportInfo;
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
    OnModemReport(requestInfo->slotId, reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetRadioState(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    const long long timeOut = DEFAULT_TIMEOUT;
    char *pLine = NULL;
    int32_t radioState = -1;
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo;

    int32_t ret = SendCommandLock("AT+CFUN?", "+CFUN", timeOut, &pResponse);
    if (ret != 0 || !pResponse->success) {
        TELEPHONY_LOGE("AT+CFUN send failed");
        return ErrorHandler(requestInfo, pResponse);
    }
    if (pResponse->head != NULL) {
        pLine = pResponse->head->data;
    }

    ret = SkipATPrefix(&pLine);
    if (ret != 0) {
        return ErrorHandler(requestInfo, pResponse);
    }
    ret = NextInt(&pLine, &radioState);
    if (ret != 0) {
        return ErrorHandler(requestInfo, pResponse);
    }

    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnModemReport(requestInfo->slotId, reportInfo, (const uint8_t *)&radioState, sizeof(int32_t));
    FreeResponseInfo(pResponse);
    return;
}

void ReqGetImei(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    TELEPHONY_LOGD("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    int32_t ret = SendCommandLock("AT+CGSN", NULL, DEFAULT_TIMEOUT, &responseInfo);
    struct ResponseAck respDataAck = { responseInfo, NULL, 0 };
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("responseInfo is null");
        ResponseModemReport(requestInfo->slotId, requestInfo, HRIL_ERR_NULL_POINT, &respDataAck);
        return;
    }
    respDataAck.responseInfo = responseInfo;
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
        ResponseModemReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        return;
    }
    if (responseInfo->head == NULL) {
        TELEPHONY_LOGE("no data!");
        ResponseModemReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    char *imeiSn = responseInfo->head->data;
    if ((imeiSn == NULL) || (strlen(imeiSn) == 0)) {
        TELEPHONY_LOGE("ReqGetImei: ResponseInfo is Invalid!");
        ResponseModemReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    respDataAck.respDataPointer = (uint8_t *)imeiSn;
    respDataAck.respDataLen = strlen(imeiSn);
    ResponseModemReport(requestInfo->slotId, requestInfo, HRIL_ERR_SUCCESS, &respDataAck);
}

void ReqGetMeid(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    int32_t err = HRIL_ERR_SUCCESS;
    ResponseInfo *responseInfo = NULL;
    TELEPHONY_LOGD("enter to [%{public}s]:%{public}d", __func__, __LINE__);
    int32_t ret = SendCommandLock("AT+MEID", NULL, DEFAULT_TIMEOUT, &responseInfo);
    struct ResponseAck respDataAck = { responseInfo, NULL, 0 };
    if (responseInfo == NULL) {
        ResponseModemReport(requestInfo->slotId, requestInfo, HRIL_ERR_NULL_POINT, &respDataAck);
        return;
    }
    respDataAck.responseInfo = responseInfo;
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
        ResponseModemReport(requestInfo->slotId, requestInfo, err, &respDataAck);
        return;
    }
    if (responseInfo->head == NULL) {
        TELEPHONY_LOGE("no data!");
        ResponseModemReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    char *meidSn = responseInfo->head->data;
    if ((meidSn == NULL) || (strlen(meidSn) == 0)) {
        ResponseModemReport(requestInfo->slotId, requestInfo, HRIL_ERR_INVALID_RESPONSE, &respDataAck);
        return;
    }
    respDataAck.respDataPointer = (uint8_t *)meidSn;
    respDataAck.respDataLen = strlen(meidSn);
    ResponseModemReport(requestInfo->slotId, requestInfo, HRIL_ERR_SUCCESS, &respDataAck);
}

static HRilRadioTech ConvertVoiceTechToRadioTech(HRilVoiceSubMode subMode)
{
    switch (subMode) {
        case HRIL_ACT_GSM:
        case HRIL_ACT_GPRS:
        case HRIL_ACT_EDGE:
            return RADIO_TECHNOLOGY_GSM;
        case HRIL_ACT_HSPA:
        case HRIL_ACT_HSDPA:
        case HRIL_ACT_HSUPA:
            return RADIO_TECHNOLOGY_HSPA;
        case HRIL_ACT_HSPAP:
        case HRIL_ACT_DC_HSPAP:
            return RADIO_TECHNOLOGY_HSPAP;
        case HRIL_ACT_WCDMA:
            return RADIO_TECHNOLOGY_WCDMA;
        case HRIL_ACT_LTE:
            return RADIO_TECHNOLOGY_LTE;
        case HRIL_ACT_IS95A:
        case HRIL_ACT_IS95B:
        case HRIL_ACT_CDMA2000_1X:
        case HRIL_ACT_HYBRID_CDMA2000_1X:
            return RADIO_TECHNOLOGY_1XRTT;
        case HRIL_ACT_EVDO_REL0:
        case HRIL_ACT_EVDO_RELA:
        case HRIL_ACT_EVDO_RELB:
        case HRIL_ACT_HYBRID_EVDO_REL0:
        case HRIL_ACT_HYBRID_EVDO_RELA:
        case HRIL_ACT_HYBRID_EVDO_RELB:
            return RADIO_TECHNOLOGY_EVDO;
        case HRIL_ACT_TDSCDMA:
            return RADIO_TECHNOLOGY_TD_SCDMA;
        case HRIL_ACT_LTE_CA:
            return RADIO_TECHNOLOGY_LTE_CA;
        case HRIL_ACT_802_16E:
            return RADIO_TECHNOLOGY_IWLAN;
        case HRIL_ACT_NR:
            return RADIO_TECHNOLOGY_NR;
        default:
            return RADIO_TECHNOLOGY_UNKNOWN;
    }
}

int32_t ProcessVoiceRadioInfo(const char *s, const HRilVoiceRadioInfo *hrilVoiceRadioInfo)
{
    int32_t srvStatus = 0;
    int32_t srvDomain = 0;
    int32_t roamStatus = 0;
    int32_t simStatus = 0;
    int32_t lockStatus = 0;
    int32_t sysMode = 0;
    int32_t actType = 0;
    char *str = (char *)s;
    HRilVoiceRadioInfo *voiceRadioInfo = (HRilVoiceRadioInfo *)hrilVoiceRadioInfo;
    if ((str == NULL) || (voiceRadioInfo == NULL)) {
        TELEPHONY_LOGE("ProcessVoiceRadioInfo s or voiceRadioInfo param is null");
        return HRIL_ERR_NULL_POINT;
    } else {
        (void)memset_s(voiceRadioInfo, sizeof(HRilVoiceRadioInfo), 0, sizeof(HRilVoiceRadioInfo));
        TELEPHONY_LOGD("result: %{public}s", str);
        int32_t err = SkipATPrefix(&str);
        if (err < 0) {
            TELEPHONY_LOGE("skip failed: [%{public}s]", str);
            return HRIL_ERR_INVALID_RESPONSE;
        }
        int32_t commaNum = FindCommaCharNum(str);
        const int32_t VOICE_COMMA_NUM = 8;
        if (commaNum != VOICE_COMMA_NUM) {
            TELEPHONY_LOGE("ProcessVoiceRadioInfo failed commaNum: [%{public}d]", commaNum);
            return HRIL_ERR_INVALID_RESPONSE;
        }
        NextIntNotSkipNextComma(&str, &srvStatus);
        voiceRadioInfo->srvStatus = srvStatus;
        NextIntNotSkipNextComma(&str, &srvDomain);
        voiceRadioInfo->srvDomain = srvDomain;
        NextIntNotSkipNextComma(&str, &roamStatus);
        voiceRadioInfo->roamStatus = roamStatus;
        NextIntNotSkipNextComma(&str, &simStatus);
        voiceRadioInfo->simStatus = simStatus;
        if (NextIntNotSkipNextComma(&str, &lockStatus) < 0) {
            voiceRadioInfo->lockStatus = 0;
        } else {
            voiceRadioInfo->lockStatus = lockStatus;
        }
        NextIntNotSkipNextComma(&str, &sysMode);
        voiceRadioInfo->sysMode = sysMode;
        NextStr(&str, &(voiceRadioInfo->sysModeName));
        NextIntNotSkipNextComma(&str, &actType);
        voiceRadioInfo->actType = ConvertVoiceTechToRadioTech((HRilVoiceSubMode)actType);
        NextStr(&str, &(voiceRadioInfo->actName));
        return HRIL_ERR_SUCCESS;
    }
}

void ReqGetVoiceRadioTechnology(const ReqDataInfo *requestInfo)
{
    if (requestInfo == NULL) {
        return;
    }
    int32_t err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo;
    ResponseInfo *responseInfo = NULL;
    char *result = NULL;
    int32_t ret = SendCommandLock("AT^SYSINFOEX", "^SYSINFOEX:", DEFAULT_TIMEOUT, &responseInfo);
    if (responseInfo == NULL) {
        TELEPHONY_LOGE("responseInfo is nullptr!");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_NULL_POINT, HRIL_RESPONSE, 0);
        OnModemReport(requestInfo->slotId, reportInfo, NULL, 0);
        return;
    }
    if (ret != 0 || !responseInfo->success) {
        err = GetResponseErrorCode(responseInfo);
        TELEPHONY_LOGE("send AT CMD failed!");
    }
    HRilVoiceRadioInfo voiceRadioInfo = {0};
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    ret = ProcessVoiceRadioInfo(result, &voiceRadioInfo);
    if (ret != 0) {
        TELEPHONY_LOGE("ProcessVoiceRadioInfo format  unexpected: %{public}s", result);
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnModemReport(requestInfo->slotId, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnModemReport(requestInfo->slotId, reportInfo, (const uint8_t *)&voiceRadioInfo, sizeof(HRilVoiceRadioInfo));
    FreeResponseInfo(responseInfo);
}
