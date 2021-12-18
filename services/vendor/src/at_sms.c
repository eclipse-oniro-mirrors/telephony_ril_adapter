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

#include "at_sms.h"

#include "vendor_report.h"

static int32_t g_responseSuccess = 1;
static int32_t g_responseFail = -2;
static int32_t g_cmdLength = 2;

int32_t ProcessCellBroadcast(char *pBuff, HRilCBConfigReportInfo *response)
{
    int32_t ret;
    char *tmp = NULL;
    int32_t count = 0;
    if (pBuff == NULL || response == NULL) {
        TELEPHONY_LOGE("pBuff or response is null");
        return -1;
    }
    tmp = pBuff;
    while (*tmp != '\0' && tmp != NULL) {
        if (*tmp == ',') {
            count++;
        }
        tmp++;
    }
    SkipATPrefix(&pBuff);
    if (count > 1) {
        ret = NextInt(&pBuff, &response->sn);
        if (ret == -1) {
            TELEPHONY_LOGE("sn is null");
        }
        ret = NextInt(&pBuff, &response->mid);
        if (ret == -1) {
            TELEPHONY_LOGE("mid is null");
        }
        ret = NextInt(&pBuff, &response->page);
        if (ret == -1) {
            TELEPHONY_LOGE("page is null");
        }
        ret = NextInt(&pBuff, &response->pages);
        if (ret == -1) {
            TELEPHONY_LOGE("pages is null");
        }
        ret = NextStr(&pBuff, &response->dcs);
        if (ret == -1) {
            TELEPHONY_LOGE("dcs is null");
        }
    } else {
        ret = NextInt(&pBuff, &response->length);
        if (ret == -1) {
            TELEPHONY_LOGE("mode is null");
        }
    }
    return count;
}

static void HandlerGsmSmsResult(HRilSmsResponse *response, struct ReportInfo *reportInfo,
    const ReqDataInfo *requestInfo, int32_t *err, ResponseInfo *responseInfo)
{
    char *pLine = NULL;
    *err = HRIL_ERR_GENERIC_FAILURE;
    if (responseInfo && responseInfo->result) {
        pLine = responseInfo->result;
        SkipATPrefix(&pLine);
        NextInt(&pLine, err);
    }
    if (*err == -1) {
        *err = HRIL_ERR_INVALID_RESPONSE;
    }
    *reportInfo = CreateReportInfo(requestInfo, *err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, *reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

static void HandleResult(int32_t *err, char *result, const ResponseInfo *responseInfo, HRilSmsResponse *response)
{
    if (responseInfo == NULL || response == NULL) {
        TELEPHONY_LOGE("responseInfo or response is NULL");
        return;
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
        SkipATPrefix(&result);
        *err = NextInt(&result, &response->msgRef);
        if (*err != 0) {
            TELEPHONY_LOGE("NextInt in ReqSendGsmSms is failed!");
        }
    } else {
        response->msgRef = g_responseSuccess;
    }
}

void ReqSendGsmSms(const ReqDataInfo *requestInfo, const char *const *data, size_t dataLen)
{
    int32_t err;
    char *smsc = NULL;
    char smscTemp[MAX_CMD_LENGTH] = {0};
    const char *pdu = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char smsPdu[MAX_CMD_LENGTH] = {0};
    char *result = NULL;
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    HRilSmsResponse response = {0};
    smsc = ((char **)data)[0];
    if (smsc == NULL) {
        err = GenerateCommand(smscTemp, MAX_CMD_LENGTH, "%s", "00");
        if (err < 0) {
            TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
            HandlerGsmSmsResult(&response, &reportInfo, requestInfo, &err, responseInfo);
        }
        smsc = smscTemp;
    }
    pdu = ((const char **)data)[1];
    err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGS=%d", strlen(pdu) / g_cmdLength);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        HandlerGsmSmsResult(&response, &reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = GenerateCommand(smsPdu, MAX_CMD_LENGTH, "%s%s", smsc, pdu);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        HandlerGsmSmsResult(&response, &reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = SendCommandSmsLock(cmd, smsPdu, "+CMGS:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        TELEPHONY_LOGE("responseInfo fail!");
        HandlerGsmSmsResult(&response, &reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    HandleResult(&err, result, responseInfo, &response);
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&response, sizeof(HRilSmsResponse));
    FreeResponseInfo(responseInfo);
}

void ReqSendSmsAck(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen)
{
    int32_t ackFlag;
    int32_t err;
    ackFlag = ((int32_t *)data)[0];
    struct ReportInfo reportInfo = {0};
    if (ackFlag == 1) {
        err = SendCommandLock("AT+CNMA=1", NULL, 0, NULL);
    } else if (ackFlag == 0) {
        err = SendCommandLock("AT+CNMA=2", NULL, 0, NULL);
    } else {
        TELEPHONY_LOGE("unsupported arg to HREQ_SMS_SEND_SMS_ACKNOWLEDGE\n");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
}

void ReqSendCdmaSms(const ReqDataInfo *requestInfo, const HRilCdmaSmsMessageInfo *data, size_t dataLen)
{
    int32_t err;
    char *result = NULL;
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    HRilSmsResponse response = {0};
    if (data == NULL) {
        TELEPHONY_LOGE("data is  null!");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    responseInfo = (ResponseInfo *)calloc(1, sizeof(ResponseInfo));
    if (responseInfo == NULL) {
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        response.msgRef = g_responseFail;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&response, sizeof(HRilSmsResponse));
        FreeResponseInfo(responseInfo);
        return;
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
        SkipATPrefix(&result);
        NextInt(&result, &response.msgRef);
    } else {
        response.msgRef = g_responseSuccess;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&response, sizeof(HRilSmsResponse));
    FreeResponseInfo(responseInfo);
}

void ReqSendCdmaSmsAck(const ReqDataInfo *requestInfo, const char *data, size_t dataLen)
{
    int32_t err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo = {0};
    if (err > 0) {
        TELEPHONY_LOGE("vendor config is fail!");
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
}

static void SimMessageError(
    struct ReportInfo *reportInfo, const ReqDataInfo *requestInfo, int32_t *err, ResponseInfo *responseInfo)
{
    *reportInfo = CreateReportInfo(requestInfo, *err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, *reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

static void WriteSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen)
{
    int32_t err;
    HRilSmsWriteSms *msg = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char smsPdu[MAX_CMD_LENGTH] = {0};
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    msg = ((HRilSmsWriteSms *)data);
    if (msg->smsc == NULL || (strcmp(msg->smsc, "") == 0)) {
        strcpy_s(msg->smsc, strlen("00"), "00");
    }
    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGW=%d,%d", strlen(msg->pdu) / g_cmdLength, msg->state);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", ret);
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    int32_t tmp = GenerateCommand(smsPdu, MAX_CMD_LENGTH, "%s%s", msg->smsc, msg->pdu);
    if (tmp < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", tmp);
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = SendCommandSmsLock(cmd, smsPdu, "+CMGW:", 0, &responseInfo);
    memset_s(cmd, MAX_CMD_LENGTH, 0, MAX_CMD_LENGTH);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        TELEPHONY_LOGE("responseInfo fail!");
        err = HRIL_ERR_GENERIC_FAILURE;
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

static void UpdateSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen)
{
    int32_t err;
    HRilSmsWriteSms *msg = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char smsPdu[MAX_CMD_LENGTH] = {0};
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    msg = ((HRilSmsWriteSms *)data);
    err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGW=%zu,%d", strlen(msg->pdu) / g_cmdLength, msg->state);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = GenerateCommand(smsPdu, MAX_CMD_LENGTH, "%s", msg->pdu);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = SendCommandSmsLock(cmd, smsPdu, "+CMGW:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    int32_t res = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGD=%d", msg->index);
    if (res < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", res);
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = SendCommandLock(cmd, "+CMGD:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqWriteSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen)
{
    int32_t err;
    HRilSmsWriteSms *msg = NULL;
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    if (data == NULL) {
        TELEPHONY_LOGE("data is  null!");
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    msg = ((HRilSmsWriteSms *)data);
    if (msg->index < 0) {
        WriteSimMessage(requestInfo, data, dataLen);
    } else {
        UpdateSimMessage(requestInfo, data, dataLen);
    }
}

void ReqDelSimMessage(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen)
{
    int32_t err;
    int32_t index;
    char cmd[MAX_CMD_LENGTH] = {0};
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    if (data == NULL) {
        TELEPHONY_LOGE("data is  null!");
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    index = ((int32_t *)data)[0];
    err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGD=%d", index);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }

    err = SendCommandLock(cmd, "+CMGD:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqSetSmscAddr(const ReqDataInfo *requestInfo, const HRilServiceCenterAddress *data, size_t dataLen)
{
    int32_t err;
    HRilServiceCenterAddress *address = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    if (data == NULL) {
        TELEPHONY_LOGE("data is  null!");
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    address = ((HRilServiceCenterAddress *)data);

    if (address->tosca == 0) {
        err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCA=\"%s\"", address->address);
    } else {
        err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCA=\"%s\",%d", address->address, address->tosca);
    }
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = SendCommandLock(cmd, "+CSCA:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqGetSmscAddr(const ReqDataInfo *requestInfo)
{
    int32_t err;
    char *result = NULL;
    HRilServiceCenterAddress response;
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    err = SendCommandLock("AT+CSCA?", "+CSCA:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    if (result == NULL) {
        TELEPHONY_LOGE("ReqGetSmscAddr result is null");
    } else {
        SkipATPrefix(&result);
        err = NextStr(&result, &response.address);
        if (err == -1) {
            TELEPHONY_LOGE("NextStr in ReqGetSmscAddr is failed!");
            (void)strcpy_s(response.address, strlen("") + 1, "");
        }
        err = NextInt(&result, &response.tosca);
        if (err == -1) {
            TELEPHONY_LOGE("NextInt in ReqGetSmscAddr is failed!");
            response.tosca = 0;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&response, sizeof(HRilServiceCenterAddress));
    FreeResponseInfo(responseInfo);
}

void ReqGetCBConfig(const ReqDataInfo *requestInfo)
{
    int32_t err;
    HRilCBConfigInfo cellBroadcast = {0};
    struct ReportInfo reportInfo = {0};
    char *line = NULL;
    ResponseInfo *responseInfo = NULL;
    err = SendCommandLock("AT+CSCB?", "+CSCB:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    if (responseInfo->head == NULL) {
        reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    line = responseInfo->head->data;
    SkipATPrefix(&line);
    err = NextInt(&line, &cellBroadcast.mode);
    if (err > 0) {
        TELEPHONY_LOGE("vendor config is fail!");
        reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = NextStr(&line, &cellBroadcast.mids);
    if (err > 0) {
        TELEPHONY_LOGE("vendor config is fail!");
        reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = NextStr(&line, &cellBroadcast.dcss);
    if (err > 0) {
        TELEPHONY_LOGE("vendor config is fail!");
        reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&cellBroadcast, sizeof(HRilCBConfigInfo));
    FreeResponseInfo(responseInfo);
}

void ReqSetCBConfig(const ReqDataInfo *requestInfo, const HRilCBConfigInfo *data, size_t dataLen)
{
    int32_t err;
    HRilCBConfigInfo *cellBroadcast = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    if (data == NULL) {
        TELEPHONY_LOGE("data is  null!");
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    cellBroadcast = ((HRilCBConfigInfo *)data);
    int ret;

    if (strcmp(cellBroadcast->mids, "") && !strcmp(cellBroadcast->dcss, "")) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCB=%d,\"%s\"", cellBroadcast->mode, cellBroadcast->mids);
    } else if (strcmp(cellBroadcast->mids, "") && strcmp(cellBroadcast->dcss, "")) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCB=%d,\"%s\",\"%s\"", cellBroadcast->mode,
            cellBroadcast->mids, cellBroadcast->dcss);
    } else if (strcmp(cellBroadcast->dcss, "") && !strcmp(cellBroadcast->mids, "")) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCB=%d,,\"%s\"", cellBroadcast->mode, cellBroadcast->dcss);
    } else {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCB=%d", cellBroadcast->mode);
    }
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", ret);
        reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }

    err = SendCommandLock(cmd, "+CSCB:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }

    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqGetCdmaCBConfig(const ReqDataInfo *requestInfo)
{
    int32_t err;
    HRilCdmaCBConfigInfo cdmaCBConfig = {0};
    struct ReportInfo reportInfo = {0};
    char lineBuff[MAX_CMD_LENGTH] = {0};
    char *line = lineBuff;
    ResponseInfo *responseInfo = NULL;
    err = HRIL_ERR_SUCCESS;
    int32_t service = 1;
    int32_t language = 1;
    char checked = '0';
    int32_t ret = GenerateCommand(lineBuff, MAX_CMD_LENGTH, "test:%d,%d,%c", service, language, checked);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", ret);
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    SkipATPrefix(&line);
    err = NextInt(&line, &cdmaCBConfig.service);
    if (err > 0) {
        TELEPHONY_LOGE("vendor config is fail!");
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = NextInt(&line, &cdmaCBConfig.language);
    if (err > 0) {
        TELEPHONY_LOGE("vendor config is fail!");
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    char *tmp = NULL;
    err = NextStr(&line, &tmp);
    if (err > 0 || tmp == NULL || tmp[0] == '\0') {
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    cdmaCBConfig.checked = tmp[0];
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, (const uint8_t *)&cdmaCBConfig, sizeof(HRilCdmaCBConfigInfo));
    FreeResponseInfo(responseInfo);
}

void ReqSetCdmaCBConfig(const ReqDataInfo *requestInfo, const HRilCdmaCBConfigInfo *data, size_t dataLen)
{
    int32_t err;
    HRilCdmaCBConfigInfo *cdmaCBConfig = NULL;
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    cdmaCBConfig = ((HRilCdmaCBConfigInfo *)data);
    int32_t size = dataLen / sizeof(HRilCdmaCBConfigInfo);
    if (size <= 0) {
        err = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = HRIL_ERR_SUCCESS;
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(HRIL_SIM_SLOT_1, reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}
