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

#include <string.h>

#include "hril_notification.h"
#include "vendor_report.h"

static int32_t g_cmdLength = 2;
char *g_commaStr = ",";
char *g_hyphenStr = "-";
#define MAX_LENGTH 100
#define MAX_DIMEN_LENGTH 20
#define CHNL_LEN 6

int32_t ProcessCellBroadcast(char *pBuff, HRilCBConfigReportInfo *response)
{
    char *tmp = NULL;
    int32_t ret;
    int32_t count = 0;

    if (pBuff == NULL || response == NULL) {
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

static void HandlerSmsResult(HRilSmsResponse *response, struct ReportInfo *reportInfo, const ReqDataInfo *requestInfo,
    int32_t *err, ResponseInfo *responseInfo)
{
    if (reportInfo == NULL || err == NULL) {
        return;
    }
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
    OnSmsReport(GetSlotId(requestInfo), *reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

static void HandleResult(int32_t *err, char *result, const ResponseInfo *responseInfo, HRilSmsResponse *response)
{
    if (response == NULL || err == NULL || responseInfo == NULL) {
        TELEPHONY_LOGE("response is NULL");
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
        response->msgRef = 1;
    }
}

void ReqSendGsmSms(const ReqDataInfo *requestInfo, const char *const *data, size_t dataLen)
{
    if (data == NULL || dataLen <= 1) {
        TELEPHONY_LOGE("data error");
        return;
    }
    char *smsc = NULL;
    char smscTemp[MAX_CMD_LENGTH] = {0};
    const char *pdu = NULL;
    char cmd[MAX_CMD_LENGTH] = {0};
    char smsPdu[MAX_CMD_LENGTH] = {0};
    char *result = NULL;
    int32_t err;
    struct ReportInfo reportInfo = {0};
    ResponseInfo *responseInfo = NULL;
    HRilSmsResponse response = {0};
    smsc = ((char **)data)[0];

    if (smsc == NULL) {
        err = GenerateCommand(smscTemp, MAX_CMD_LENGTH, "%s", "00");
        if (err < 0) {
            TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
            HandlerSmsResult(&response, &reportInfo, requestInfo, &err, responseInfo);
        }
        smsc = smscTemp;
    }
    pdu = ((const char **)data)[1];
    err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGS=%d", strlen(pdu) / g_cmdLength);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        HandlerSmsResult(&response, &reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = GenerateCommand(smsPdu, MAX_CMD_LENGTH, "%s%s", smsc, pdu);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        HandlerSmsResult(&response, &reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = SendCommandSmsLock(cmd, smsPdu, "+CMGS:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        HandlerSmsResult(&response, &reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    HandleResult(&err, result, responseInfo, &response);
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&response, sizeof(HRilSmsResponse));
    FreeResponseInfo(responseInfo);
}

void ReqSendSmsAck(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen)
{
    int32_t ackFlag;
    int32_t err;
    if (data == NULL || dataLen == 0) {
        TELEPHONY_LOGE("data error");
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_INVALID_RESPONSE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        return;
    }
    ackFlag = ((int32_t *)data)[0];
    if (ackFlag == 1) {
        err = SendCommandLock("AT+CNMA=1", NULL, 0, NULL);
    } else if (ackFlag == 0) {
        err = SendCommandLock("AT+CNMA=2", NULL, 0, NULL);
    } else {
        TELEPHONY_LOGE("unsupported arg to HREQ_SMS_SEND_SMS_ACKNOWLEDGE\n");
        err = HRIL_ERR_GENERIC_FAILURE;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}

void ReqSendCdmaSms(const ReqDataInfo *requestInfo, const char *data, size_t dataLen)
{
    char *result = NULL;
    int32_t err;
    ResponseInfo *responseInfo = NULL;
    struct ReportInfo reportInfo = {0};
    HRilSmsResponse response = {0};

    if (data == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = SendCommandLock("AT+COPS?", "COPS?", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        TELEPHONY_LOGE("AT+COPS? send failed");
        HandlerSmsResult(&response, &reportInfo, requestInfo, &err, responseInfo);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = SendCommandLock("AT$QCMGF=0", "$QCMGF=0", 0, &responseInfo);
    if (err != 0 || responseInfo == NULL || !responseInfo->success) {
        response.msgRef = HRIL_ERR_GENERIC_FAILURE;
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
        SkipATPrefix(&result);
        NextInt(&result, &response.msgRef);
    } else {
        response.msgRef = 1;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&response, sizeof(HRilSmsResponse));
    FreeResponseInfo(responseInfo);
}

void ReqSendCdmaSmsAck(const ReqDataInfo *requestInfo, const char *data, size_t dataLen)
{
    int32_t err = HRIL_ERR_SUCCESS;
    if (err > 0) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}

static void SimMessageError(
    struct ReportInfo *reportInfo, const ReqDataInfo *requestInfo, int32_t *err, ResponseInfo *responseInfo)
{
    if (reportInfo == NULL || err == NULL) {
        return;
    }
    *reportInfo = CreateReportInfo(requestInfo, *err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), *reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

static void WriteSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = { 0 };
    char smsPdu[MAX_CMD_LENGTH] = { 0 };
    int32_t err;
    HRilSmsWriteSms *msg = NULL;
    ResponseInfo *responseInfo = NULL;
    struct ReportInfo reportInfo = {0};
    if (data == NULL) {
        TELEPHONY_LOGE("data is nullptr");
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    msg = ((HRilSmsWriteSms *)data);
    if (msg->smsc == NULL || (strcmp(msg->smsc, "") == 0)) {
        if (msg->smsc != NULL) {
            free(msg->smsc);
        }
        msg->smsc = (char *)malloc(strlen("00") + 1);
        if (strcpy_s(msg->smsc, strlen("00") + 1, "00") != EOK) {
            TELEPHONY_LOGE("Set smsc failed");
            reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
            OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
            FreeResponseInfo(responseInfo);
            return;
        }
    }
    int32_t ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGW=%d,%d", strlen(msg->pdu) / g_cmdLength, msg->state);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", ret);
        SimMessageError(&reportInfo, requestInfo, &ret, responseInfo);
        return;
    }
    int32_t tmp = GenerateCommand(smsPdu, MAX_CMD_LENGTH, "%s%s", msg->smsc, msg->pdu);
    if (tmp < 0) {
        SimMessageError(&reportInfo, requestInfo, &tmp, responseInfo);
        return;
    }
    err = SendCommandSmsLock(cmd, smsPdu, "+CMGW:", 0, &responseInfo);
    ret = memset_s(cmd, MAX_CMD_LENGTH, 0, MAX_CMD_LENGTH);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success) || ret != EOK) {
        err = HRIL_ERR_GENERIC_FAILURE;
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

static void UpdateSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    char smsPdu[MAX_CMD_LENGTH] = { 0 };
    HRilSmsWriteSms *msg = NULL;
    ResponseInfo *responseInfo = NULL;
    struct ReportInfo reportInfo = {0};
    if (!CheckSimMessageValid(requestInfo, data, dataLen, msg)) {
        TELEPHONY_LOGE("sim message data error");
        return;
    }
    msg = ((HRilSmsWriteSms *)data);
    int32_t err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGW=%zu,%d", strlen(msg->pdu) / g_cmdLength, msg->state);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = GenerateCommand(smsPdu, MAX_CMD_LENGTH, "%s%s", msg->smsc, msg->pdu);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    err = SendCommandSmsLock(cmd, smsPdu, "+CMGW:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        TELEPHONY_LOGE("ExecuteCommand failed");
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
    int32_t ret = memset_s(cmd, MAX_CMD_LENGTH, 0, MAX_CMD_LENGTH);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success) || ret != EOK) {
        err = HRIL_ERR_GENERIC_FAILURE;
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

bool CheckSimMessageValid(
    const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen, HRilSmsWriteSms *msg)
{
    if (data == NULL) {
        TELEPHONY_LOGE("data is nullptr");
        ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        return false;
    }
    msg = ((HRilSmsWriteSms *)data);
    if (msg == NULL) {
        TELEPHONY_LOGE("msg is nullptr");
        return false;
    }
    if (msg->smsc == NULL || (strcmp(msg->smsc, "") == 0)) {
        if (msg->smsc != NULL) {
            free(msg->smsc);
        }
        msg->smsc = (char *)malloc(strlen("00") + 1);
        if (strcpy_s(msg->smsc, strlen("00") + 1, "00") != EOK) {
            TELEPHONY_LOGE("Set smsc failed");
            ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
            OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
            return false;
        }
    }
    return true;
}

void ReqWriteSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen)
{
    HRilSmsWriteSms *msg = NULL;
    ResponseInfo *responseInfo = NULL;

    if (data == NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
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
    ResponseInfo *responseInfo = NULL;
    if (data == NULL || dataLen == 0) {
        TELEPHONY_LOGE("data error");
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    int32_t index = ((int32_t *)data)[0] - 1;
    if (index < 0) {
        TELEPHONY_LOGE("invalid index:%d", index);
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    char cmd[MAX_CMD_LENGTH] = { 0 };
    int32_t err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGD=%d", index);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }

    err = SendCommandLock(cmd, "+CMGD:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqSetSmscAddr(const ReqDataInfo *requestInfo, const HRilServiceCenterAddress *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err;
    HRilServiceCenterAddress *address = NULL;
    ResponseInfo *responseInfo = NULL;

    if (data == NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
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
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = SendCommandLock(cmd, "+CSCA:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqGetSmscAddr(const ReqDataInfo *requestInfo)
{
    char *result = NULL;
    int32_t err;
    ResponseInfo *responseInfo = NULL;
    HRilServiceCenterAddress response;

    err = SendCommandLock("AT+CSCA?", "+CSCA:", 0, &responseInfo);
    if (err != 0 || responseInfo == NULL || !responseInfo->success) {
        err = HRIL_ERR_GENERIC_FAILURE;
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
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
            if (strcpy_s(response.address, strlen("") + 1, "") != EOK) {
                return;
            }
        }
        err = NextInt(&result, &response.tosca);
        if (err == -1) {
            TELEPHONY_LOGE("NextInt in ReqGetSmscAddr is failed!");
            response.tosca = 0;
        }
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&response, sizeof(HRilServiceCenterAddress));
    FreeResponseInfo(responseInfo);
}

void ReqGetCBConfigInfo(const ReqDataInfo *requestInfo, ResponseInfo *responseInfo, HRilCBConfigInfo *cellBroadcast,
    char *mids, char *dcss, int32_t mode)
{
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    bool midsEmpty = (mids == NULL || mids[0] == '\0');
    char *token;
    char copy[MAX_LENGTH] = { 0 };
    if (strcpy_s(copy, MAX_LENGTH, mids) != EOK) {
        FreeResponseInfo(responseInfo);
        return;
    }
    char delimiter[] = ",";
    char *p = NULL;
    token = strtok_r(copy, delimiter, &p);
    int32_t position = 0;
    char midsTotal[MAX_LENGTH][MAX_DIMEN_LENGTH] = { 0 };
    while (!midsEmpty && token != NULL && position < MAX_LENGTH) {
        size_t length = strlen(token);
        size_t place = 0;
        while (place < length && place < MAX_DIMEN_LENGTH && token[place] != '\0') {
            midsTotal[position][place] = token[place];
            place++;
        }
        position++;
        token = strtok_r(NULL, delimiter, &p);
    }
    int32_t index = 0;
    int32_t locate = 0;
    while (index < position) {
        locate = GetCBConfigData(midsTotal[index], dcss, mode, cellBroadcast, locate);
        index++;
    }
    OnSmsReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&cellBroadcast, sizeof(HRilCBConfigInfo) * locate);
    FreeResponseInfo(responseInfo);
}

void ReqGetCBConfig(const ReqDataInfo *requestInfo)
{
    ResponseInfo *responseInfo = NULL;
    HRilCBConfigInfo cellBroadcast[MAX_LENGTH] = { 0 };
    int32_t err = SendCommandLock("AT+CSCB?", "+CSCB:", 0, &responseInfo);
    if (err != 0 || responseInfo == NULL || !responseInfo->success || responseInfo->head == NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    char *line = responseInfo->head->data;
    SkipATPrefix(&line);
    int32_t mode;
    err = NextInt(&line, &mode);
    if (err > 0) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    char *mids;
    err = NextStr(&line, &mids);
    if (err > 0) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    char *dcss;
    err = NextStr(&line, &dcss);
    if (err > 0) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    ReqGetCBConfigInfo(requestInfo, responseInfo, cellBroadcast, mids, dcss, mode);
}

void SetCellBroadcastInfo(HRilCBConfigInfo *cellBroadcast, int32_t locate, char *startMid, char *endMid, char *startDcs,
    char *endDcs, bool dcssEmpty, int32_t mode)
{
    cellBroadcast[locate].startOfServiceId = atoi(startMid);
    cellBroadcast[locate].endOfServiceId = atoi(endMid);
    if (!dcssEmpty) {
        cellBroadcast[locate].startOfCodeScheme = atoi(startDcs);
        cellBroadcast[locate].endOfCodeScheme = atoi(endDcs);
    }
    cellBroadcast[locate].selected = mode;
}

int32_t GetCBConfigData(char *token, char *dcss, int32_t mode, HRilCBConfigInfo *cellBroadcast, int32_t locate)
{
    char *ttoken;
    char ddelimiter[] = "-";
    char ccopy[MAX_LENGTH] = { 0 };
    if (strcpy_s(ccopy, MAX_LENGTH, token) != EOK) {
        return MAX_LENGTH;
    }
    char *startMid = NULL;
    char *endMid = NULL;
    char *p = NULL;
    ttoken = strtok_r(ccopy, ddelimiter, &p);
    for (int32_t index = 0; ttoken != NULL; index++) {
        if (index == 0) {
            startMid = ttoken;
            endMid = ttoken;
        } else {
            endMid = ttoken;
            break;
        }
        ttoken = strtok_r(NULL, ddelimiter, &p);
    }
    char copyDcs[MAX_LENGTH] = { 0 };
    bool dcssEmpty = (dcss == NULL || dcss[0] == '\0');
    if (strcpy_s(copyDcs, MAX_LENGTH, dcss) != EOK) {
        return MAX_LENGTH;
    }
    char *startDcs = NULL;
    char *endDcs = NULL;
    char delimiterDcs[] = "-";
    char *tokenDcs;
    char *q = NULL;
    tokenDcs = strtok_r(copyDcs, delimiterDcs, &q);
    for (int32_t index = 0; !dcssEmpty && tokenDcs != NULL; index++) {
        if (index == 0) {
            startDcs = tokenDcs;
            endDcs = tokenDcs;
        } else {
            endDcs = tokenDcs;
            break;
        }
        tokenDcs = strtok_r(NULL, delimiterDcs, &q);
    }
    if (locate < MAX_LENGTH) {
        SetCellBroadcastInfo(cellBroadcast, locate, startMid, endMid, startDcs, endDcs, dcssEmpty, mode);
    }
    locate++;
    return locate;
}

bool UpdataMids(int32_t locate, char *mids, char *start, char *end)
{
    if (locate == 0) {
        if (strcpy_s(mids, MAX_LENGTH, start) != EOK) {
            return false;
        }
    } else {
        if (strcat_s(mids, MAX_LENGTH, g_commaStr) != EOK) {
            return false;
        }
        if (strcat_s(mids, MAX_LENGTH, start) != EOK) {
            return false;
        }
    }
    if (strcat_s(mids, MAX_LENGTH, g_hyphenStr) != EOK) {
        return false;
    }
    if (strcat_s(mids, MAX_LENGTH, end) != EOK) {
        return false;
    }
    return true;
}

bool UpdataMidValues(int32_t locate, char *mids, char *start)
{
    if (locate == 0) {
        if (strcpy_s(mids, MAX_LENGTH, start) != EOK) {
            return false;
        }
    } else {
        if (strcat_s(mids, MAX_LENGTH, g_commaStr) != EOK) {
            return false;
        }
        if (strcat_s(mids, MAX_LENGTH, start) != EOK) {
            return false;
        }
    }
    return true;
}

bool GetDcss(const HRilCBConfigInfo *data, int32_t len, char *dcss)
{
    for (int32_t locate = 0; locate < len && (locate + 1) * (CHNL_LEN + CHNL_LEN + 1) < MAX_LENGTH; locate++) {
        if (data[locate].startOfCodeScheme > data[locate].endOfCodeScheme) {
            TELEPHONY_LOGE("result.dcss is invalid");
            return false;
        } else if (data[locate].startOfCodeScheme < data[locate].endOfCodeScheme) {
            char start[CHNL_LEN] = { 0 };
            sprintf_s(start, CHNL_LEN, "%d", data[locate].startOfCodeScheme);
            char end[CHNL_LEN] = { 0 };
            sprintf_s(end, CHNL_LEN, "%d", data[locate].endOfCodeScheme);
            if (locate != 0) {
                break;
            }
            if (strcpy_s(dcss, MAX_LENGTH, start) != EOK) {
                return false;
            }
            if (strcat_s(dcss, MAX_LENGTH, g_hyphenStr) != EOK) {
                return false;
            }
            if (strcat_s(dcss, MAX_LENGTH, end) != EOK) {
                return false;
            }
        } else {
            char start[CHNL_LEN] = { 0 };
            sprintf_s(start, CHNL_LEN, "%d", data[locate].startOfCodeScheme);
            if (locate != 0) {
                break;
            }
            if (strcpy_s(dcss, MAX_LENGTH, start) != EOK) {
                return false;
            }
        }
    }
    return true;
}

bool GetMidsAndDcss(const HRilCBConfigInfo *data, int32_t len, char *mids, char *dcss)
{
    for (int32_t locate = 0; locate < len && (locate + 1) * (CHNL_LEN + CHNL_LEN + 1) < MAX_LENGTH; locate++) {
        if (data[locate].startOfServiceId < data[locate].endOfServiceId) {
            char start[CHNL_LEN] = { 0 };
            sprintf_s(start, CHNL_LEN, "%d", data[locate].startOfServiceId);
            char end[CHNL_LEN] = { 0 };
            sprintf_s(end, CHNL_LEN, "%d", data[locate].endOfServiceId);
            if (!UpdataMids(locate, mids, start, end)) {
                return false;
            }
        } else {
            char start[CHNL_LEN] = { 0 };
            sprintf_s(start, CHNL_LEN, "%d", data[locate].startOfServiceId);
            if (UpdataMidValues(locate, mids, start)) {
                return false;
            }
        }
        if (!GetDcss(data, len, dcss)) {
            return false;
        }
    }
    return true;
}

void ReqSetCBConfig(const ReqDataInfo *requestInfo, const HRilCBConfigInfo *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = { 0 };
    int32_t err;
    int32_t ret;
    ResponseInfo *responseInfo = NULL;
    int32_t len = (int32_t)(dataLen / sizeof(HRilCBConfigInfo));
    if (data == NULL || len == 0) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    int32_t mode = data[0].selected;
    char mids[MAX_LENGTH] = { 0 };
    char dcss[MAX_LENGTH] = { 0 };
    if (!GetMidsAndDcss(data, len, mids, dcss)) {
        return;
    }
    if (strcmp(mids, "") && !strcmp(dcss, "")) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCB=%d,\"%s\"", mode, mids);
    } else if (strcmp(mids, "") && strcmp(dcss, "")) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCB=%d,\"%s\",\"%s\"", mode, mids, dcss);
    } else if (strcmp(dcss, "") && !strcmp(mids, "")) {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCB=%d,,\"%s\"", mode, dcss);
    } else {
        ret = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CSCB=%d", mode);
    }
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", ret);
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = SendCommandLock(cmd, "+CSCB:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqGetCdmaCBConfig(const ReqDataInfo *requestInfo)
{
    char checked = '0';
    char lineBuff[MAX_CMD_LENGTH] = {0};
    char *line = lineBuff;
    int32_t err = HRIL_ERR_SUCCESS;
    int32_t service = 1;
    int32_t language = 1;
    ResponseInfo *responseInfo = NULL;
    HRilCdmaCBConfigInfo cdmaCBConfig = {0};

    int32_t ret = GenerateCommand(lineBuff, MAX_CMD_LENGTH, "AT:%d,%d,%c", service, language, checked);
    if (ret < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", ret);
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    SkipATPrefix(&line);
    err = NextInt(&line, &cdmaCBConfig.service);
    if (err > 0) {
        err = HRIL_ERR_GENERIC_FAILURE;
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = NextInt(&line, &cdmaCBConfig.language);
    if (err > 0) {
        err = HRIL_ERR_GENERIC_FAILURE;
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    char *tmp = NULL;
    err = NextStr(&line, &tmp);
    if (err > 0 || tmp == NULL || tmp[0] == '\0') {
        err = HRIL_ERR_GENERIC_FAILURE;
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    cdmaCBConfig.checked = tmp[0];
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, (const uint8_t *)&cdmaCBConfig, sizeof(HRilCdmaCBConfigInfo));
    FreeResponseInfo(responseInfo);
}

void ReqSetCdmaCBConfig(const ReqDataInfo *requestInfo, const HRilCdmaCBConfigInfo *data, size_t dataLen)
{
    int32_t err;
    ResponseInfo *responseInfo = NULL;

    if (data == NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    int32_t size = dataLen / sizeof(HRilCdmaCBConfigInfo);
    if (size <= 0) {
        err = HRIL_ERR_GENERIC_FAILURE;
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = HRIL_ERR_SUCCESS;
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqAddCdmaSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteCdmaSms *data, size_t dataLen)
{
    if (data == NULL) {
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        return;
    }
    HRilSmsWriteCdmaSms *cdmaSms = (HRilSmsWriteCdmaSms *)data;
    HRilCdmaSmsMessageInfo *cdmaMsg = &cdmaSms->cdmaMessageInfo;
    TELEPHONY_LOGD(
        "CreateCdmaPdu = %{public}x, %{public}x, %{public}x", cdmaMsg->serviceId, cdmaMsg->isExist, cdmaMsg->type);
    TELEPHONY_LOGD("CreateCdmaPdu2 = %{public}x, %{public}x, %{public}x", cdmaMsg->address.digitMode,
        cdmaMsg->address.mode, cdmaMsg->address.type);
    TELEPHONY_LOGD("CreateCdmaPdu3 = %{public}x, %{public}x, %{public}s", cdmaMsg->address.plan,
        cdmaMsg->address.number, cdmaMsg->address.bytes);
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
}

void ReqDelCdmaSimMessage(const ReqDataInfo *requestInfo, const int32_t *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err;
    int32_t index;
    ResponseInfo *responseInfo = NULL;
    if (data == NULL || dataLen == 0) {
        TELEPHONY_LOGE("data error");
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    index = ((int32_t *)data)[0] - 1;
    if (index < 0) {
        TELEPHONY_LOGE("invalid index:%d", index);
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CMGD=%d", index);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = SendCommandLock(cmd, "+CMGD:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        struct ReportInfo reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    struct ReportInfo reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqUpdateCdmaSimMessage(const ReqDataInfo *requestInfo, const HRilSmsWriteSms *data, size_t dataLen)
{
    char cmd[MAX_CMD_LENGTH] = {0};
    int32_t err;
    HRilSmsWriteSms *cdmaMsg = NULL;
    ResponseInfo *responseInfo = NULL;
    struct ReportInfo reportInfo = {0};
    if (data == NULL) {
        reportInfo = CreateReportInfo(requestInfo, HRIL_ERR_GENERIC_FAILURE, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    cdmaMsg = ((HRilSmsWriteSms *)data);
    err = GenerateCommand(cmd, MAX_CMD_LENGTH, "AT+CPMS=%d,%d%s", cdmaMsg->index, cdmaMsg->state, cdmaMsg->pdu);
    if (err < 0) {
        TELEPHONY_LOGE("GenerateCommand failed, err = %{public}d\n", err);
        reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
        OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
        FreeResponseInfo(responseInfo);
        return;
    }
    err = SendCommandLock(cmd, "+CPMS:", 0, &responseInfo);
    if (err != 0 || (responseInfo != NULL && !responseInfo->success)) {
        err = HRIL_ERR_GENERIC_FAILURE;
        SimMessageError(&reportInfo, requestInfo, &err, responseInfo);
        return;
    }
    reportInfo = CreateReportInfo(requestInfo, err, HRIL_RESPONSE, 0);
    OnSmsReport(GetSlotId(requestInfo), reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}
