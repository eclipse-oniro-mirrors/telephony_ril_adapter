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

#include "at_sim.h"

#include "hril_notification.h"
#include "securec.h"

#include "vendor_adapter.h"
#include "vendor_report.h"

static const int32_t ERR = -1;

static int32_t GetSimType(void)
{
    char *pLine = NULL;
    int32_t ret;
    int32_t simType = 0;
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("AT^CARDMODE", "^CARDMODE:", 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        TELEPHONY_LOGE("AT^CARDMODE send failed");
        return HRIL_SIM_TYPE_UNKNOWN;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = SkipATPrefix(&pLine);
    if (ret < 0) {
        return HRIL_SIM_TYPE_UNKNOWN;
    }
    ret = NextInt(&pLine, &simType);
    if (ret < 0) {
        return HRIL_SIM_TYPE_UNKNOWN;
    }
    FreeResponseInfo(pResponse);
    return simType;
}

static int32_t GetSimState(char *pLine, char *pResult, ResponseInfo *pResponse)
{
    int32_t status = HRIL_SIM_NOT_INSERTED;
    int32_t ret;
    ret = SkipATPrefix(&pLine);
    if (ret != 0) {
        return HRIL_SIM_NOT_READY;
    }
    ret = NextStr(&pLine, &pResult);
    if (ret != 0) {
        return HRIL_SIM_NOT_READY;
    }
    if (strcmp(pResult, "READY") == 0) {
        status = HRIL_SIM_READY;
    } else if (strcmp(pResult, "SIM PIN") == 0) {
        status = HRIL_SIM_PIN;
    } else if (strcmp(pResult, "SIM PUK") == 0) {
        status = HRIL_SIM_PUK;
    } else if (strcmp(pResult, "PH-NET PIN") == 0) {
        status = HRIL_PH_NET_PIN;
    } else if (strcmp(pResult, "PH-NET PUK") == 0) {
        status = HRIL_PH_NET_PIN;
    } else if (!ReportStrWith(pResponse->result, "+CME ERROR:")) {
        status = HRIL_SIM_NOT_READY;
    }
    return status;
}

static int32_t ParseSimResponseResult(char *pLine, HRilSimIOResponse *pSimResponse)
{
    if (pSimResponse == NULL) {
        return ERR;
    }
    int32_t err = SkipATPrefix(&pLine);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pSimResponse->sw1);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pSimResponse->sw2);
    if (err != 0) {
        return err;
    }

    if ((pLine != NULL && *pLine != '\0')) {
        err = NextStr(&pLine, &pSimResponse->response);
        if (err != 0) {
            return err;
        }
    }
    return 0;
}

static int32_t ParseSimPinInputTimesResult(char *pLine, HRilPinInputTimes *pinInputTimes)
{
    int32_t err = HRIL_ERR_GENERIC_FAILURE;
    if (pinInputTimes == NULL) {
        TELEPHONY_LOGE("pinInputTimes is null!!!");
        return err;
    }
    err = SkipATPrefix(&pLine);
    if (err != 0) {
        return err;
    }
    err = NextStr(&pLine, &pinInputTimes->code);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pinInputTimes->times);
    size_t atProlen = 0;
    if (err != 0 && strlen(pLine) == atProlen) {
        return err;
    }
    err = NextInt(&pLine, &pinInputTimes->pukTimes);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pinInputTimes->pinTimes);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pinInputTimes->puk2Times);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &pinInputTimes->pin2Times);
    if (err != 0) {
        return err;
    }
    return 0;
}

static int32_t ParseUnlockSimLockResult(char *pLine, HRilLockStatus *lockStatus)
{
    int32_t err = HRIL_ERR_GENERIC_FAILURE;
    if (lockStatus == NULL) {
        TELEPHONY_LOGE("lockStatus is null!!!");
        return err;
    }
    err = SkipATPrefix(&pLine);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &lockStatus->result);
    if (err != 0) {
        return err;
    }
    err = NextInt(&pLine, &lockStatus->remain);
    TELEPHONY_LOGD("ParseUnlockSimLockResult, lockStatus->result: %{public}d", lockStatus->result);
    TELEPHONY_LOGD("ParseUnlockSimLockResult, lockStatus->remain: %{public}d", lockStatus->remain);
    if (err != 0) {
        return err;
    }
    return 0;
}

void ReqGetSimStatus(const ReqDataInfo *requestInfo)
{
    char *pLine = NULL;
    char *pResult = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;
    HRilCardState cardState = {0};

    HRilRadioState radioState = GetRadioState();
    if (radioState == HRIL_RADIO_POWER_STATE_UNAVAILABLE || radioState == HRIL_RADIO_POWER_STATE_OFF) {
        cardState.simState = HRIL_SIM_NOT_READY;
    }
    cardState.simType = GetSimType();
    ret = SendCommandLock("AT+CPIN?", "+CPIN:", 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        TELEPHONY_LOGE("AT+CPIN? send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        if (ret == HRIL_ERR_NO_SIMCARD_INSERTED) {
            cardState.simState = HRIL_SIM_NOT_INSERTED;
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&cardState, sizeof(HRilCardState));
        } else {
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        }
        FreeResponseInfo(pResponse);
        return;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    cardState.simState = GetSimState(pLine, pResult, pResponse);
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&cardState, sizeof(HRilCardState));
    FreeResponseInfo(pResponse);
}

static int32_t ReqGetSimIOFDNWrite(HRilSimIO *pSim, ResponseInfo **ppResponse, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t tmp = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d,\"%s\"", "FD", 1, pSim->pin2);
    if (tmp < 0) {
        TELEPHONY_LOGE("GenerateCommand failed");
        return VENDOR_FAIL;
    }
    int32_t ret = SendCommandLock(cmd, "+CLCK", 0, ppResponse);
    if (ret != 0 || (ppResponse != NULL && !(*ppResponse)->success)) {
        TELEPHONY_LOGE("AT+CLCK failed");
        return HRIL_ERR_CMD_SEND_FAILURE;
    }
    tmp = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CRSM=%d,%d,%d,%d,%d,\"%s\",\"%s\"", pSim->command, pSim->fileid,
        pSim->p1, pSim->p2, pSim->p3, ((pSim->data == NULL) ? "" : (pSim->data)), pSim->pathid);
    if (tmp < 0) {
        TELEPHONY_LOGE("GenerateCommand failed");
        return VENDOR_FAIL;
    }
    ret = SendCommandLock(cmd, "+CRSM", 0, ppResponse);
    if (ret != 0 || (ppResponse != NULL && !(*ppResponse)->success)) {
        return HRIL_ERR_CMD_SEND_FAILURE;
    }
    tmp = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d,\"%s\"", "FD", 0, pSim->pin2);
    if (tmp < 0) {
        return VENDOR_FAIL;
    }
    ret = SendCommandLock(cmd, "+CLCK", 0, ppResponse);
    if (ret != 0 || !(*ppResponse)->success) {
        TELEPHONY_LOGE("AT+CLCK failed");
        return HRIL_ERR_CMD_SEND_FAILURE;
    }
    return HRIL_ERR_SUCCESS;
}

static int32_t ReqGetSimIOFDN(HRilSimIO *pSim, ResponseInfo **ppResponse, size_t dataLen)
{
    const char *queryCmd = "AT+CLCK=\"FD\",2";
    int32_t ret = SendCommandLock(queryCmd, "+CLCK", 0, ppResponse);
    if (ret != 0 || ppResponse == NULL || *ppResponse == NULL || !(*ppResponse)->success) {
        TELEPHONY_LOGE("AT+CLCK failed");
        return HRIL_ERR_CMD_SEND_FAILURE;
    }
    char *pLine = (*ppResponse)->last == NULL ? (*ppResponse)->result : (*ppResponse)->last->data;
    SkipATPrefix(&pLine);
    int32_t clckRes = VENDOR_FAIL;
    NextInt(&pLine, &clckRes);
    clckRes = (clckRes > 0) ? 1 : 0;
    TELEPHONY_LOGD("FDN had got FDN clck res:%{public}d", clckRes);
    int32_t writeRet = ReqGetSimIOFDNWrite(pSim, ppResponse, dataLen);
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t tmp = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d,\"%s\"", "FD", clckRes, pSim->pin2);
    if (tmp < 0) {
        TELEPHONY_LOGE("GenerateCommand failed");
        return VENDOR_FAIL;
    }
    ret = SendCommandLock(cmd, "+CLCK", 0, ppResponse);
    if (ret != 0 || (ppResponse != NULL && !(*ppResponse)->success)) {
        TELEPHONY_LOGE("AT+CLCK failed");
        return HRIL_ERR_CMD_SEND_FAILURE;
    }
    return writeRet;
}

static void HandlerSimIOResult(ResponseInfo *pResponse, HRilSimIOResponse *simResponse,
    const ReqDataInfo *requestInfo, char *pLine, int32_t *ret)
{
    if (pResponse == NULL || ret == NULL) {
        TELEPHONY_LOGE("pResponse is NULL");
        return;
    }

    if (*ret != HRIL_ERR_SUCCESS) {
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, ret);
        } else {
            *ret = HRIL_ERR_GENERIC_FAILURE;
        }
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, *ret, HRIL_RESPONSE, 0);
    if (simResponse == NULL) {
        TELEPHONY_LOGE("simResponse is NULL");
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    } else {
        OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)simResponse, sizeof(HRilSimIOResponse));
    }
    FreeResponseInfo(pResponse);
}

void ReqGetSimIO(const ReqDataInfo *requestInfo, const HRilSimIO *data, size_t dataLen)
{
    char *pLine = NULL;
    int32_t ret;
    const int32_t FILEID = 0x6F3B;
    ResponseInfo *pResponse = NULL;
    HRilSimIOResponse simResponse = {0};
    HRilSimIO *pSim = (HRilSimIO *)data;
    if (pSim == NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    if (pSim->pin2 != NULL && strcmp(pSim->pin2, "") != 0 && pSim->fileid == FILEID) {
        ret = ReqGetSimIOFDN(pSim, &pResponse, dataLen);
        TELEPHONY_LOGE("ReqGetSimIOFDN call over");
        if (ret != HRIL_ERR_SUCCESS || !pResponse->success) {
            TELEPHONY_LOGE("FDN is failed");
            HandlerSimIOResult(pResponse, NULL, requestInfo, pLine, &ret);
            return;
        }
        TELEPHONY_LOGE("FDN is success");
        HandlerSimIOResult(pResponse, &simResponse, requestInfo, pLine, &ret);
        return;
    }
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CRSM=%d,%d,%d,%d,%d,\"%s\",\"%s\"", pSim->command,
        pSim->fileid, pSim->p1, pSim->p2, pSim->p3, ((pSim->data == NULL) ? "" : (pSim->data)), pSim->pathid);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CRSM", 0, &pResponse);
    if (ret != HRIL_ERR_SUCCESS || pResponse == NULL || !pResponse->success) {
        TELEPHONY_LOGE("send failed dataLen:%{public}zu", dataLen);
        HandlerSimIOResult(pResponse, NULL, requestInfo, pLine, &ret);
        return;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = ParseSimResponseResult(pLine, &simResponse);
    if (ret != HRIL_ERR_SUCCESS) {
        HandlerSimIOResult(pResponse, NULL, requestInfo, pLine, &ret);
        return;
    }
    if (GetSimType() == HRIL_SIM_TYPE_USIM && pSim->command == CMD_GET_RESPONSE) {
        ConvertUsimFcpToSimRsp((unsigned char **)&(simResponse.response));
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&simResponse, sizeof(HRilSimIOResponse));
    FreeResponseInfo(pResponse);
}

static void HandlerSimImsiResult(
    ResponseInfo *pResponse, struct ReportInfo reportInfo, const ReqDataInfo *requestInfo, char *pLine, int32_t *ret)
{
    if (pResponse == NULL || ret == NULL) {
        TELEPHONY_LOGE("pResponse is NULL");
        return;
    }
    if (pResponse && pResponse->result) {
        pLine = pResponse->result;
        SkipATPrefix(&pLine);
        NextInt(&pLine, ret);
    } else {
        *ret = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, *ret, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetSimImsi(const ReqDataInfo *requestInfo)
{
    char *pLine = NULL;
    char *result = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo = {0};

    ret = SendCommandLock("AT+CIMI", NULL, 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        TELEPHONY_LOGE("AT+CIMI send failed");
        HandlerSimImsiResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }
    if (pResponse->head) {
        result = pResponse->head->data;
    } else {
        HandlerSimImsiResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)result, 0);
    FreeResponseInfo(pResponse);
}

static void HandlerSimLockStatusResult(
    ResponseInfo *pResponse, struct ReportInfo reportInfo, const ReqDataInfo *requestInfo, char *pLine, int32_t *ret)
{
    if (pResponse == NULL || ret == NULL) {
        TELEPHONY_LOGE("pResponse is NULL");
        return;
    }
    if (pResponse && pResponse->result) {
        pLine = pResponse->result;
        SkipATPrefix(&pLine);
        NextInt(&pLine, ret);
    } else {
        *ret = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, *ret, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetSimLockStatus(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t ret;
    char *pLine = NULL;
    int32_t status = 0;
    HRilSimClock *pSimClck = NULL;
    ResponseInfo *pResponse = NULL;
    struct ReportInfo reportInfo = {0};

    pSimClck = (HRilSimClock *)data;
    if (pSimClck == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d", pSimClck->fac, pSimClck->mode);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CLCK", 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        TELEPHONY_LOGE("AT+CLCK send failed dataLen:%{public}zu", dataLen);
        HandlerSimLockStatusResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = SkipATPrefix(&pLine);
    if (ret != 0) {
        HandlerSimLockStatusResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }

    ret = NextInt(&pLine, &status);
    if (ret != 0) {
        HandlerSimLockStatusResult(pResponse, reportInfo, requestInfo, pLine, &ret);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&status, sizeof(int32_t));
    FreeResponseInfo(pResponse);
}

void ReqSetSimLock(const ReqDataInfo *requestInfo, const HRilSimClock *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    HRilSimClock *pSimClck = NULL;
    ResponseInfo *pResponse = NULL;

    pSimClck = (HRilSimClock *)data;
    if (pSimClck == NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    int32_t result = GenerateCommand(
        cmd, MAX_CMD_LENGTH, "AT+CLCK=\"%s\",%d,\"%s\"", pSimClck->fac, pSimClck->mode, pSimClck->passwd);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CLCK", 0, &pResponse);
    HRilLockStatus lockStatus = {0};
    lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
    lockStatus.remain = -1;
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CLCK send failed dataLen:%{public}zu", dataLen);
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
            HRilPinInputTimes pinInputTimes = { 0 };
            if (ret == AT_RESPONSE_INCORRECT_PASSWORD) {
                ReqGetSimPinInputTimesRemain(requestInfo, &pinInputTimes);
                lockStatus.result = HRIL_UNLOCK_PASSWORD_ERR;
                lockStatus.remain = pinInputTimes.pinTimes;
            }
            if (strcmp(pSimClck->fac, "FD") == 0) {
                lockStatus.remain = pinInputTimes.pin2Times;
            }
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;

            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
        }
        return;
    }
    lockStatus.result = HRIL_UNLOCK_SUCCESS;
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
    FreeResponseInfo(pResponse);
}

void ReqChangeSimPassword(const ReqDataInfo *requestInfo, const HRilSimPassword *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    HRilSimPassword *pSimPassword = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    pSimPassword = (HRilSimPassword *)data;
    if (pSimPassword == NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CPWD=\"%s\",\"%s\",\"%s\"", pSimPassword->fac,
        pSimPassword->oldPassword, pSimPassword->newPassword);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CPWD", 0, &pResponse);
    HRilLockStatus lockStatus = {0};
    lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
    lockStatus.remain = -1;
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CPWD send failed dataLen:%{public}zu", dataLen);
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
            HRilPinInputTimes pinInputTimes = {0};
            if (ret == AT_RESPONSE_INCORRECT_PASSWORD) {
                ReqGetSimPinInputTimesRemain(requestInfo, &pinInputTimes);
                lockStatus.result = HRIL_UNLOCK_PASSWORD_ERR;
                lockStatus.remain = pinInputTimes.pinTimes;
            }
            if (strcmp(pSimPassword->fac, "P2") == 0) {
                lockStatus.remain = pinInputTimes.pin2Times;
            }
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
        }
        return;
    }
    lockStatus.result = HRIL_UNLOCK_SUCCESS;
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
    FreeResponseInfo(pResponse);
}

void ReqUnlockPin(const ReqDataInfo *requestInfo, const char *pin)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CPIN=\"%s\"", pin);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CPIN", 0, &pResponse);
    HRilLockStatus lockStatus = {0};
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CPIN send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            TELEPHONY_LOGD("AT+CPIN send failed pLine1:%{public}s", pLine);
            SkipATPrefix(&pLine);
            TELEPHONY_LOGD("AT+CPIN send failed pLine2:%{public}s", pLine);
            NextInt(&pLine, &ret);
            TELEPHONY_LOGD("AT+CPIN send failed ret:%{public}d", ret);
            if (ret == AT_RESPONSE_INCORRECT_PASSWORD) {
                HRilPinInputTimes pinInputTimes = {0};
                ReqGetSimPinInputTimesRemain(requestInfo, &pinInputTimes);
                lockStatus.result = HRIL_UNLOCK_PASSWORD_ERR;
                lockStatus.remain = pinInputTimes.pinTimes;
            } else {
                lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
                lockStatus.remain = -1;
                TELEPHONY_LOGE("AT+CPWD send failed");
            }
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
            return;
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
            lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
            lockStatus.remain = -1;
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
            return;
        }
    }

    lockStatus.result = HRIL_UNLOCK_SUCCESS;
    lockStatus.remain = -1;
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
    FreeResponseInfo(pResponse);
}

void ReqUnlockPuk(const ReqDataInfo *requestInfo, const char *puk, const char *pin)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CPIN=\"%s\",\"%s\"", puk, pin);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CPIN", 0, &pResponse);
    HRilLockStatus lockStatus = {0};
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CPIN send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            TELEPHONY_LOGD("AT+CPIN send failed pLine1:%{public}s", pLine);
            SkipATPrefix(&pLine);
            TELEPHONY_LOGD("AT+CPIN send failed pLine2:%{public}s", pLine);
            NextInt(&pLine, &ret);
            TELEPHONY_LOGD("AT+CPIN send failed ret:%{public}d", ret);
            if (ret == AT_RESPONSE_INCORRECT_PASSWORD) {
                HRilPinInputTimes pinInputTimes = {0};
                ReqGetSimPinInputTimesRemain(requestInfo, &pinInputTimes);
                lockStatus.result = HRIL_UNLOCK_PASSWORD_ERR;
                lockStatus.remain = pinInputTimes.pukTimes;
            } else {
                lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
                lockStatus.remain = -1;
                TELEPHONY_LOGE("AT+CPIN send failed");
            }
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
            return;
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
            lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
            lockStatus.remain = -1;
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
            return;
        }
    }

    lockStatus.result = HRIL_UNLOCK_SUCCESS;
    lockStatus.remain = -1;
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
    FreeResponseInfo(pResponse);
}

void ReqGetSimPinInputTimes(const ReqDataInfo *requestInfo)
{
    char *pLine = NULL;
    int32_t ret;
    HRilPinInputTimes pinInputTimes = {0};
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("AT^CPIN?", "^CPIN", 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        TELEPHONY_LOGE("AT^CPIN? send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = ParseSimPinInputTimesResult(pLine, &pinInputTimes);
    if (ret != 0) {
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&pinInputTimes, sizeof(HRilPinInputTimes));
    FreeResponseInfo(pResponse);
}

void ReqGetSimPinInputTimesRemain(const ReqDataInfo *requestInfo, HRilPinInputTimes *pinInputTimes)
{
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("AT^CPIN?", "^CPIN", 0, &pResponse);
    TELEPHONY_LOGI("AT+^CPIN send failed ret:%{public}d", ret);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        TELEPHONY_LOGE("AT^CPIN? send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        FreeResponseInfo(pResponse);
        return;
    }
    if (pResponse && pResponse->head) {
        pLine = pResponse->head->data;
    }
    TELEPHONY_LOGD("ReqGetSimPinInputTimesRemain pLine:%{public}s, result:%{public}s, success:%{public}d", pLine,
        pResponse->result, pResponse->success);
    ret = ParseSimPinInputTimesResult(pLine, pinInputTimes);
    TELEPHONY_LOGD("code:%{public}s, times:%{public}d, puk:%{public}d,"
        " pin:%{public}d, puk2:%{public}d, pin2:%{public}d",
        pinInputTimes->code, pinInputTimes->times, pinInputTimes->pukTimes,
        pinInputTimes->pinTimes, pinInputTimes->puk2Times, pinInputTimes->pin2Times);
    if (ret != 0) {
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        TELEPHONY_LOGE("AT+^CPIN send failed ret3:%{public}d", ret);
        return;
    }
    return;
}

void ReqUnlockPin2(const ReqDataInfo *requestInfo, const char *pin2)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^CPIN2=\"%s\"", pin2);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "^CPIN2", 0, &pResponse);
    HRilLockStatus lockStatus = {0};
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^CPIN2 send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            TELEPHONY_LOGD("AT^CPIN2 send failed pLine1:%{public}s", pLine);
            SkipATPrefix(&pLine);
            TELEPHONY_LOGD("AT^CPIN2 send failed pLine2:%{public}s", pLine);
            NextInt(&pLine, &ret);
            TELEPHONY_LOGD("AT^CPIN2 send failed ret:%{public}d", ret);
            if (ret == AT_RESPONSE_INCORRECT_PASSWORD) {
                HRilPinInputTimes pinInputTimes = {0};
                ReqGetSimPinInputTimesRemain(requestInfo, &pinInputTimes);
                lockStatus.result = HRIL_UNLOCK_PASSWORD_ERR;
                lockStatus.remain = pinInputTimes.pin2Times;
            } else {
                lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
                lockStatus.remain = -1;
                TELEPHONY_LOGE("AT+^CPIN2 send failed");
            }
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
            return;
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
            lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
            lockStatus.remain = -1;
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
            return;
        }
    }

    lockStatus.result = HRIL_UNLOCK_SUCCESS;
    lockStatus.remain = -1;
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
    FreeResponseInfo(pResponse);
}

void ReqUnlockPuk2(const ReqDataInfo *requestInfo, const char *puk2, const char *pin2)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^CPIN2=\"%s\",\"%s\"", puk2, pin2);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "^CPIN2", 0, &pResponse);
    HRilLockStatus lockStatus = {0};
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^CPIN2 send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            TELEPHONY_LOGD("AT^CPIN2 send failed pLine1:%{public}s", pLine);
            SkipATPrefix(&pLine);
            TELEPHONY_LOGD("AT^CPIN2 send failed pLine2:%{public}s", pLine);
            NextInt(&pLine, &ret);
            TELEPHONY_LOGD("AT^CPIN2 send failed ret:%{public}d", ret);
            if (ret == AT_RESPONSE_INCORRECT_PASSWORD) {
                HRilPinInputTimes pinInputTimes = {0};
                ReqGetSimPinInputTimesRemain(requestInfo, &pinInputTimes);
                lockStatus.result = HRIL_UNLOCK_PASSWORD_ERR;
                lockStatus.remain = pinInputTimes.puk2Times;
            } else {
                lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
                lockStatus.remain = -1;
                TELEPHONY_LOGE("AT+^CPIN2 send failed");
            }
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
            return;
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
            lockStatus.result = HRIL_UNLOCK_OTHER_ERR;
            lockStatus.remain = -1;
            struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
            OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
            FreeResponseInfo(pResponse);
            return;
        }
    }

    lockStatus.result = HRIL_UNLOCK_SUCCESS;
    lockStatus.remain = -1;
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(lockStatus));
    FreeResponseInfo(pResponse);
}

void ReqGetSimPin2InputTimes(const ReqDataInfo *requestInfo)
{
    char *pLine = NULL;
    int32_t ret;
    HRilPinInputTimes pin2InputTimes = {0};
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("AT^CPIN2?", "^CPIN2", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^CPIN2? send failed");
    }
    if (pResponse != NULL && pResponse->head != NULL) {
        pLine = pResponse->head->data;
    }
    ret = ParseSimPinInputTimesResult(pLine, &pin2InputTimes);
    if (ret != 0) {
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&pin2InputTimes, 0);
    FreeResponseInfo(pResponse);
}

void ReqSetActiveSim(const ReqDataInfo *requestInfo, int32_t index, int32_t enable)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^HVSST=%d,%d", index, enable);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "^HVSST", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^HVSST send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSimStkSendTerminalResponse(const ReqDataInfo *requestInfo, const char *strCmd)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+SPUSATTERMINAL=\"%s\"", strCmd);
    if (result < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", result);
    }
    ret = SendCommandLock(cmd, "+SPUSATTERMINAL", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+SPUSATTERMINAL send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSimStkSendEnvelope(const ReqDataInfo *requestInfo, const char *strCmd)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+SPUSATENVECMD=\"%s\"", strCmd);
    if (result < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", result);
    }
    ret = SendCommandLock(cmd, "+SPUSATENVECMD", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+SPUSATENVECMD send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSimStkSendCallSetupRequestResult(const ReqDataInfo *requestInfo, int32_t accept)
{
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}

void ReqSimStkIsReady(const ReqDataInfo *requestInfo)
{
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    ret = SendCommandLock("AT+SPUSATPROFILE?", "+SPUSATPROFILE", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+SPUSATPROFILE send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqGetRadioProtocol(const ReqDataInfo *requestInfo)
{
    TELEPHONY_LOGD("ReqGetRadioProtocol");
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    HRilRadioProtocol radioProtocol = {};
    radioProtocol.sessionId = 0;
    radioProtocol.phase = HRIL_RADIO_PROTOCOL_PHASE_INITIAL;
    radioProtocol.technology = HRIL_RADIO_PROTOCOL_TECH_GSM | HRIL_RADIO_PROTOCOL_TECH_WCDMA |
                               HRIL_RADIO_PROTOCOL_TECH_HSPA | HRIL_RADIO_PROTOCOL_TECH_HSPAP |
                               HRIL_RADIO_PROTOCOL_TECH_LTE | HRIL_RADIO_PROTOCOL_TECH_LTE_CA;
    radioProtocol.modemId = 0;
    radioProtocol.status = HRIL_RADIO_PROTOCOL_STATUS_NONE;
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&radioProtocol, sizeof(HRilRadioProtocol));
}

void ReqSetRadioProtocol(const ReqDataInfo *requestInfo, const HRilRadioProtocol *data)
{
    HRilRadioProtocol *radioProtocol = (HRilRadioProtocol *)data;
    struct ReportInfo reportInfo = { 0 };
    if (radioProtocol == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        return;
    }

    if (radioProtocol->phase != HRIL_RADIO_PROTOCOL_PHASE_UPDATE) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)radioProtocol, sizeof(HRilRadioProtocol));
        return;
    }
    radioProtocol->phase = HRIL_RADIO_PROTOCOL_PHASE_NOTIFY;
    radioProtocol->status = HRIL_RADIO_PROTOCOL_STATUS_SUCCESS;
    reportInfo.error = HRIL_ERR_SUCCESS;
    reportInfo.type = HRIL_NOTIFICATION;
    reportInfo.notifyId = HNOTI_SIM_RADIO_PROTOCOL_UPDATED;
    OnSimReport(GetSlotId(NULL), reportInfo, (const uint8_t *)radioProtocol, sizeof(HRilRadioProtocol));
}

void ReqSimOpenLogicalChannel(const ReqDataInfo *requestInfo, const char *appID, int32_t p2)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+SPCCHO=\"%s\",%d", appID, p2);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    int32_t ret = SendCommandLock(cmd, "+SPCCHO", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+SPCCHO send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSimCloseLogicalChannel(const ReqDataInfo *requestInfo, int32_t channelId)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CCHC=%d", channelId);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CCHC", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT+CCHC send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(pResponse);
}

void ReqSimTransmitApduLogicalChannel(const ReqDataInfo *requestInfo, HRilApduSimIO *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;
    HRilSimIOResponse apduSimResponse = {0};
    HRilApduSimIO *pApduSimIO = (HRilApduSimIO *)data;

    if (pApduSimIO == NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CGLA=%d,%d,%d,%d,%d,%d,\"%s\"", pApduSimIO->channelId,
        pApduSimIO->type, pApduSimIO->instruction, pApduSimIO->p1, pApduSimIO->p2, pApduSimIO->p3, pApduSimIO->data);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "+CGLA", 0, &pResponse);
    if (ret != 0 || pResponse == NULL || !pResponse->success) {
        TELEPHONY_LOGE("AT+CGLA send failed  dataLen:%{public}zu", dataLen);
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    if (pResponse->head) {
        pLine = pResponse->head->data;
    }
    ret = ParseSimResponseResult(pLine, &apduSimResponse);
    if (ret != HRIL_ERR_SUCCESS) {
        HandlerSimIOResult(pResponse, NULL, requestInfo, pLine, &ret);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&apduSimResponse, sizeof(HRilSimIOResponse));
    FreeResponseInfo(pResponse);
}

void ReqUnlockSimLock(const ReqDataInfo *requestInfo, int32_t lockType, const char *password)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char *pLine = NULL;
    int32_t ret;
    ResponseInfo *pResponse = NULL;

    int32_t result = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT^UNLOCKSIMLOCK=\"%d\",%s", lockType, password);
    if (result <= 0) {
        TELEPHONY_LOGE("GenerateCommand is failed");
    }
    ret = SendCommandLock(cmd, "^UNLOCKSIMLOCK", 0, &pResponse);
    if (ret != 0 || (pResponse != NULL && !pResponse->success)) {
        TELEPHONY_LOGE("AT^UNLOCKSIMLOCK send failed");
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    if (pResponse && pResponse->head) {
        pLine = pResponse->head->data;
    }
    HRilLockStatus lockStatus = {0};
    ret = ParseUnlockSimLockResult(pLine, &lockStatus);
    if (ret != 0) {
        if (pResponse && pResponse->result) {
            pLine = pResponse->result;
            SkipATPrefix(&pLine);
            NextInt(&pLine, &ret);
        } else {
            ret = HRIL_ERR_GENERIC_FAILURE;
        }
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, ret, HRIL_RESPONSE, 0);
        OnSimReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(pResponse);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSimReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&lockStatus, sizeof(HRilLockStatus));
    FreeResponseInfo(pResponse);
}

void ConvertUsimFcpToSimRsp(uint8_t **simResponse)
{
    uint16_t fcpLen = strlen((char *)*simResponse) / HALF_LEN;
    uint8_t *fcpByte = malloc(fcpLen);
    UsimFileDescriptor fDescriptor = { 0 };
    UsimFileIdentifier fId = { 0 };
    uint8_t simRspByte[GET_RESPONSE_EF_SIZE_BYTES] = { 0 };
    if (fcpByte == NULL) {
        TELEPHONY_LOGE("fcpByte is NULL");
        free(fcpByte);
        return;
    }
    if (memset_s(fcpByte, fcpLen, 0, fcpLen) != EOK) {
        TELEPHONY_LOGE("ConvertUsimFcpToSimRsp memset_s failed");
        free(fcpByte);
        return;
    }
    ConvertHexStringToByteArray(*simResponse, strlen((char *)*simResponse), fcpByte, fcpLen);
    if (FcpFileDescriptorQuery(fcpByte, fcpLen, (UsimFileDescriptor *)&fDescriptor) == FALSE) {
        TELEPHONY_LOGE("FcpFileDescriptorQuery failed");
        free(fcpByte);
        return;
    }
    if (FcpFileIdentifierQuery(fcpByte, fcpLen, (UsimFileIdentifier *)&fId) == FALSE) {
        TELEPHONY_LOGE("FcpFileIdentifierQuery failed");
        free(fcpByte);
        return;
    }
    if (IsDedicatedFile(fDescriptor.fd)) {
        simRspByte[RESPONSE_DATA_FILE_TYPE] = TYPE_DF;
        *simResponse = ConvertByteArrayToHexString(simRspByte, fcpLen);
        free(fcpByte);
        return;
    }
    CreateSimRspByte(simRspByte, GET_RESPONSE_EF_SIZE_BYTES, &fId, &fDescriptor);
    *simResponse = ConvertByteArrayToHexString(simRspByte, GET_RESPONSE_EF_SIZE_BYTES);
    free(fcpByte);
}

void CreateSimRspByte(uint8_t simRspByte[], int responseLen, UsimFileIdentifier *fId, UsimFileDescriptor *fDescriptor)
{
    if (responseLen < RESPONSE_DATA_RECORD_LENGTH + 1) {
        TELEPHONY_LOGE("simRspByte size error");
        return;
    }
    if (fId == NULL || fDescriptor == NULL) {
        TELEPHONY_LOGE("fId or  fDescriptor is null");
        return;
    }
    simRspByte[RESPONSE_DATA_FILE_TYPE] = TYPE_EF;
    simRspByte[RESPONSE_DATA_FILE_ID_1] = (fId->fileId & BYTE_NUM_1) >> ADDR_OFFSET_8BIT;
    simRspByte[RESPONSE_DATA_FILE_ID_2] = fId->fileId & BYTE_NUM_2;
    simRspByte[RESPONSE_DATA_LENGTH] = (GET_RESPONSE_EF_SIZE_BYTES - RESPONSE_DATA_LENGTH - 1);
    if (IsLinearFixedFile(fDescriptor->fd)) {
        simRspByte[RESPONSE_DATA_STRUCTURE] = EF_TYPE_LINEAR_FIXED;
        simRspByte[RESPONSE_DATA_RECORD_LENGTH] = fDescriptor->recLen;
        fDescriptor->dataSize = (fDescriptor->numRec & BYTE_NUM_0) * (fDescriptor->recLen);
        simRspByte[RESPONSE_DATA_FILE_SIZE_1] = (fDescriptor->dataSize & BYTE_NUM_1) >> ADDR_OFFSET_8BIT;
        simRspByte[RESPONSE_DATA_FILE_SIZE_2] = fDescriptor->dataSize & BYTE_NUM_2;
        simRspByte[RESPONSE_DATA_FILE_STATUS] = VALID_FILE_STATUS;
        return;
    }
    if (IsTransparentFile(fDescriptor->fd)) {
        simRspByte[RESPONSE_DATA_STRUCTURE] = EF_TYPE_TRANSPARENT;
        return;
    }
    if (IsCyclicFile(fDescriptor->fd)) {
        simRspByte[RESPONSE_DATA_STRUCTURE] = EF_TYPE_CYCLIC;
        simRspByte[RESPONSE_DATA_RECORD_LENGTH] = fDescriptor->recLen;
        return;
    }
}

uint8_t FcpTlvSearchTag(uint8_t *dataPtr, uint16_t len, UsimFcpTag tag, uint8_t **outPtr)
{
    uint8_t tagLen = 0;
    uint16_t lenVar = len;
    for (*outPtr = dataPtr; lenVar > 0; *outPtr += tagLen) {
        tagLen = (*(*outPtr + 1) + HALF_LEN);
        if (**outPtr == (uint8_t)tag) {
            *outPtr += HALF_LEN;
            return *(*outPtr - 1);
        }
        lenVar -= tagLen;
    }
    *outPtr = NULL;
    return FALSE;
}

uint8_t FcpFileDescriptorQuery(uint8_t *fcpByte, uint16_t fcpLen, UsimFileDescriptor *filledStructPtr)
{
    if (fcpByte == NULL || fcpLen < HALF_LEN + 1) {
        TELEPHONY_LOGE("fcpByte size error");
        return FALSE;
    }
    uint8_t valueLen = fcpByte[1];
    uint8_t *dataPtr = &fcpByte[HALF_LEN];
    if (fcpByte[0] != FCP_TEMP_T) {
        TELEPHONY_LOGE("fcpByte data error");
        return FALSE;
    }
    uint8_t resultLen = 0;
    uint8_t *outPtr = NULL;
    UsimFileDescriptor *queryPtr = filledStructPtr;
    resultLen = FcpTlvSearchTag(dataPtr, valueLen, FCP_FILE_DES_T, &outPtr);
    if (!((outPtr != NULL) && ((resultLen == HALF_LEN) || (resultLen == FIVE_LEN)))) {
        TELEPHONY_LOGE("resultLen value error");
        return FALSE;
    }
    queryPtr->fd = outPtr[0];
    queryPtr->dataCoding = outPtr[1];
    if (resultLen == FIVE_LEN) {
        queryPtr->recLen = (short)((outPtr[HALF_LEN] << ADDR_OFFSET_8BIT) | outPtr[THIRD_INDEX]);
        queryPtr->numRec = outPtr[HALF_BYTE_LEN];
        return TRUE;
    }
    queryPtr->recLen = 0;
    queryPtr->numRec = 0;
    return TRUE;
}

uint8_t FcpFileIdentifierQuery(uint8_t *fcpByte, uint16_t fcpLen, UsimFileIdentifier *filledStructPtr)
{
    if (fcpByte == NULL || fcpLen < HALF_LEN + 1) {
        TELEPHONY_LOGE("fcpByte size error");
        return FALSE;
    }
    uint8_t valueLen = fcpByte[1];
    uint8_t *dataPtr = &fcpByte[HALF_LEN];
    if (fcpByte[0] != FCP_TEMP_T) {
        TELEPHONY_LOGE("fcpByte data error");
        return FALSE;
    }
    uint8_t resultLen = 0;
    uint8_t *outPtr = NULL;
    UsimFileIdentifier *queryPtr = (UsimFileIdentifier *)filledStructPtr;
    resultLen = FcpTlvSearchTag(dataPtr, valueLen, FCP_FILE_ID_T, &outPtr);
    if (outPtr == NULL) {
        queryPtr->fileId = 0;
        return FALSE;
    }
    if (resultLen != HALF_LEN) {
        TELEPHONY_LOGE("resultLen size error");
        return FALSE;
    }
    queryPtr->fileId = (short)((outPtr[0] << ADDR_OFFSET_8BIT) | outPtr[1]);
    return TRUE;
}

uint8_t IsCyclicFile(uint8_t fd)
{
    return (0x07 & (fd)) == 0x06;
}

uint8_t IsDedicatedFile(uint8_t fd)
{
    return (0x38 & (fd)) == 0x38;
}

uint8_t IsLinearFixedFile(uint8_t fd)
{
    return (0x07 & (fd)) == 0x02;
}

uint8_t IsTransparentFile(uint8_t fd)
{
    return (0x07 & (fd)) == 0x01;
}

void ConvertHexStringToByteArray(uint8_t *originHexString, int responseLen, uint8_t *byteArray, int fcpLen)
{
    if (responseLen <= 0 || fcpLen <= 0) {
        TELEPHONY_LOGE("originHexString is error, size=%{public}d", responseLen);
        return;
    }
    for (int i = 0; i < fcpLen; i++) {
        int hexIndex = i * HALF_LEN;
        if (hexIndex + 1 >= responseLen) {
            break;
        }
        byteArray[i] = (ToByte(originHexString[hexIndex]) << HALF_BYTE_LEN) | ToByte(originHexString[hexIndex + 1]);
    }
}

uint8_t *ConvertByteArrayToHexString(uint8_t *byteArray, int byteArrayLen)
{
    uint8_t *buf = malloc(byteArrayLen * HALF_LEN + 1);
    if (buf == NULL) {
        TELEPHONY_LOGE("buf is NULL");
        return NULL;
    }
    int bufIndex = 0;
    const char HEX_DIGITS[HEX_DIGITS_LEN] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E',
        'F' };
    for (int i = 0; i < byteArrayLen; i++) {
        uint8_t b = byteArray[i];
        buf[bufIndex++] = HEX_DIGITS[(b >> HALF_BYTE_LEN) & BYTE_NUM_4];
        buf[bufIndex++] = HEX_DIGITS[b & BYTE_NUM_4];
    }
    buf[bufIndex] = '\0';
    return buf;
}

uint8_t ToByte(char c)
{
    if (c >= '0' && c <= '9') {
        return (c - '0');
    }
    if (c >= 'A' && c <= 'F') {
        return (c - 'A' + DECIMAL_MAX);
    }
    if (c >= 'a' && c <= 'f') {
        return (c - 'a' + DECIMAL_MAX);
    }
    TELEPHONY_LOGE("ToByte Error: %{public}c", c);
    return 0;
}
