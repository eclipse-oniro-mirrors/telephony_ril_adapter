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
#include "vendor_util.h"
#include "vendor_report.h"

static const int MAX_BUFF_SIZE = 500;
static const int DATA_LENGTH = 1;
static const int CMD_LENGTH = 2;

int ProcessCellBroadcast(char *pBuff, HRilCellBroadcastReportInfo *response)
{
    int ret = 0;
    char *tmp = NULL;
    int count = 0;
    const int TMP_INDEX = 1;
    const int BUFF_IS_NULL = -1;
    if (pBuff == NULL || response == NULL) {
        return BUFF_IS_NULL;
    }
    tmp = pBuff;
    while (*tmp != '\0' && tmp != NULL) {
        if (*tmp == ',') {
            count++;
        }
        tmp++;
    }
    SkipATPrefix(&pBuff);
    if (count > TMP_INDEX) {
        ret = NextInt(&pBuff, &response->sn);
        if (ret == BUFF_IS_NULL) {
            TELEPHONY_LOGE("sn is null");
        }
        ret = NextInt(&pBuff, &response->mid);
        if (ret == BUFF_IS_NULL) {
            TELEPHONY_LOGE("mid is null");
        }
        ret = NextInt(&pBuff, &response->page);
        if (ret == BUFF_IS_NULL) {
            TELEPHONY_LOGE("page is null");
        }
        ret = NextInt(&pBuff, &response->pages);
        if (ret == BUFF_IS_NULL) {
            TELEPHONY_LOGE("pages is null");
        }
        ret = NextStr(&pBuff, &response->dcs);
        if (ret == BUFF_IS_NULL) {
            TELEPHONY_LOGE("dcs is null");
        }
    } else {
        ret = NextInt(&pBuff, &response->length);
        if (ret == BUFF_IS_NULL) {
            TELEPHONY_LOGE("length is null");
        }
    }
    return count;
}

void ReqSendSms(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int err = 0;
    char *smsc = NULL;
    char smscTemp[MAX_BUFF_SIZE] = {0};
    const char *pdu = NULL;
    char cmd[MAX_BUFF_SIZE] = {0};
    char smsPdu[MAX_BUFF_SIZE] = {0};
    char *result = NULL;
    const int RESPONSE_SUCCESS = 1;
    const int RESPONSE_FAIL = -2;
    struct ReportInfo reportInfo = {};
    ResponseInfo *responseInfo = NULL;
    HRilSmsResponse response = {};
    smsc = ((char **)data)[0];

    if (smsc == NULL) {
        (void)sprintf_s(smscTemp, MAX_BUFF_SIZE, "00");
        smsc = smscTemp;
    }
    pdu = ((const char **)data)[DATA_LENGTH];
    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CMGS=%zu", strlen(pdu) / CMD_LENGTH);
    (void)sprintf_s(smsPdu, MAX_BUFF_SIZE, "%s%s", smsc, pdu);

    err = SendCommandSmsLock(cmd, smsPdu, "+CMGS:", 0, &responseInfo);
    if (err != 0 || !responseInfo->success) {
        goto ERROR;
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
        TELEPHONY_LOGD("%{public}s enter, result:%{public}s", __func__, result);
        SkipATPrefix(&result);
        err = NextInt(&result, &response.msgRef);
        if (err != 0) {
            TELEPHONY_LOGE("NextInt in ReqSendSms is failed!");
        }
        TELEPHONY_LOGD("%{public}s enter, msgRef:%{public}d", __func__, response.msgRef);
    } else {
        response.msgRef = RESPONSE_SUCCESS;
    }
    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, &response, sizeof(HRilSmsResponse));
    FreeResponseInfo(responseInfo);
    return;
ERROR:
    response.msgRef = RESPONSE_FAIL;
    reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, &response, sizeof(HRilSmsResponse));
    FreeResponseInfo(responseInfo);
    return;
}

void ReqSendSmsAck(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int ackFlag = 0;
    int err = 0;
    const int ACK_SUCCESS = 1;
    ackFlag = ((int *)data)[0];
    struct ReportInfo reportInfo = {};
    if (ackFlag == ACK_SUCCESS) {
        err = SendCommandLock("AT+CNMA=1", NULL, 0, NULL);
    } else if (ackFlag == 0) {
        err = SendCommandLock("AT+CNMA=2", NULL, 0, NULL);
    } else {
        TELEPHONY_LOGE("unsupported arg to HREQ_SMS_SEND_SMS_ACKNOWLEDGE\n");
        goto ERROR;
    }
    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    return;
ERROR:
    reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
}

void ReqStorageSms(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int err = 0;
    HRilSmsWriteSms *msg = NULL;
    char cmd[MAX_BUFF_SIZE] = {0};
    char smsPdu[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    ResponseInfo *responseInfo = NULL;
    msg = ((HRilSmsWriteSms *)data);
    TELEPHONY_LOGD("%{public}s enter, pdu:%{public}s, smsc:%{public}s, status:%{public}d", __func__, msg->pdu,
        msg->smsc, msg->state);
    if (msg->smsc == NULL || (strcmp(msg->smsc, "") == 0)) {
        strcpy_s(msg->smsc, strlen("00"), "00");
    }

    err = sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CMGW=%zu,%d", strlen(msg->pdu) / CMD_LENGTH, msg->state);
    if (err < 0) {
        TELEPHONY_LOGE("sprintf_s failed, err = %{public}d\n", err);
        goto ERROR;
    }
    err = sprintf_s(smsPdu, MAX_BUFF_SIZE, "%s%s", msg->smsc, msg->pdu);
    if (err < 0) {
        TELEPHONY_LOGE("sprintf_s failed, err = %{public}d\n", err);
        goto ERROR;
    }

    err = SendCommandSmsLock(cmd, smsPdu, "+CMGW:", 0, &responseInfo);
    if (err != 0 || responseInfo->success == 0) {
        goto ERROR;
    }

    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
    return;
ERROR:
    reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
    return;
}

void ReqDeleteSms(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int err = 0;
    int index = 0;
    char cmd[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    ResponseInfo *responseInfo = NULL;
    if (data == NULL) {
        err = HRIL_ERR_GENERIC_FAILURE;
        goto ERROR;
    }
    index = ((int *)data)[0];
    TELEPHONY_LOGD("%{public}s enter, index:%{public}d", __func__, index);
    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CMGD=%d", index);

    err = SendCommandLock(cmd, "+CMGD:", 0, &responseInfo);
    if (err != 0 || !responseInfo->success) {
        goto ERROR;
    }

    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
    return;
ERROR:
    reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqUpdateSms(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int err = 0;
    HRilSmsWriteSms *msg = NULL;
    char cmd[MAX_BUFF_SIZE] = {0};
    char smsPdu[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    ResponseInfo *responseInfo = NULL;
    if (data == NULL) {
        err = HRIL_ERR_GENERIC_FAILURE;
        goto ERROR;
    }
    msg = ((HRilSmsWriteSms *)data);

    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CMGW=%zu,%d", strlen(msg->pdu) / CMD_LENGTH, msg->state);
    (void)sprintf_s(smsPdu, MAX_BUFF_SIZE, "%s", msg->pdu);

    err = SendCommandSmsLock(cmd, smsPdu, "+CMGW:", 0, &responseInfo);
    if (err != 0 || !responseInfo->success) {
        err = HRIL_ERR_GENERIC_FAILURE;
        goto ERROR;
    }
    (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CMGD=%d", msg->index);
    err = SendCommandLock(cmd, "+CMGD:", 0, &responseInfo);
    if (err != 0 || !responseInfo->success) {
        goto ERROR;
    }

    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
    return;
ERROR:
    reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqSetSmsCenterAddress(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int err = 0;
    HRilServiceCenterAddress *address = NULL;
    char cmd[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    ResponseInfo *responseInfo = NULL;
    if (data == NULL) {
        err = HRIL_ERR_GENERIC_FAILURE;
        goto ERROR;
    }
    address = ((HRilServiceCenterAddress *)data);
    if (address->tosca == 0) {
        (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CSCA=\"%s\"", address->address);
    } else {
        (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CSCA=\"%s\",%d", address->address, address->tosca);
    }
    err = SendCommandLock(cmd, "+CSCA:", 0, &responseInfo);
    if (err != 0 || !responseInfo->success) {
        goto ERROR;
    }

    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
    return;
ERROR:
    reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqGetSmsCenterAddress(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int err = 0;
    const int NEXTSTRERR = -1;
    char *result = NULL;
    HRilServiceCenterAddress response = {};
    struct ReportInfo reportInfo = {};
    ResponseInfo *responseInfo = NULL;
    err = SendCommandLock("AT+CSCA?", "+CSCA:", 0, &responseInfo);
    if (err != 0 || !responseInfo->success) {
        goto ERROR;
    }
    if (responseInfo->head != NULL) {
        result = responseInfo->head->data;
    }
    if (result == NULL) {
        TELEPHONY_LOGE("ReqGetSmsCenterAddress result is null");
    } else {
        SkipATPrefix(&result);
        err = NextStr(&result, &response.address);
        if (err == NEXTSTRERR) {
            (void)strcpy_s(response.address, strlen("") + DATA_LENGTH, "");
        }
        err = NextInt(&result, &response.tosca);
        if (err == NEXTSTRERR) {
            response.tosca = 0;
        }
    }
    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, &response, sizeof(HRilServiceCenterAddress *));
    FreeResponseInfo(responseInfo);
    return;
ERROR:
    reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}

void ReqSetCellBroadcast(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int err = 0;
    HRilCellBroadcastInfo *cellBroadcast = NULL;
    char cmd[MAX_BUFF_SIZE] = {0};
    struct ReportInfo reportInfo = {};
    ResponseInfo *responseInfo = NULL;
    if (data == NULL) {
        err = HRIL_ERR_GENERIC_FAILURE;
        goto ERROR;
    }
    cellBroadcast = ((HRilCellBroadcastInfo *)data);
    TELEPHONY_LOGD("%{public}s enter, mode:%{public}d, dcss:%{public}s, mids:%{public}s", __func__,
        cellBroadcast->mode, cellBroadcast->dcss, cellBroadcast->mids);
    if (strcmp(cellBroadcast->mids, "") && !strcmp(cellBroadcast->dcss, "")) {
        (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CSCB=%d,\"%s\"", cellBroadcast->mode, cellBroadcast->mids);
    } else if (strcmp(cellBroadcast->dcss, "") && strcmp(cellBroadcast->dcss, "")) {
        (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CSCB=%d,\"%s\",\"%s\"", cellBroadcast->mode, cellBroadcast->mids,
            cellBroadcast->dcss);
    } else if (strcmp(cellBroadcast->dcss, "") && !strcmp(cellBroadcast->mids, "")) {
        (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CSCB=%d,,\"%s\"", cellBroadcast->mode, cellBroadcast->dcss);
    } else {
        (void)sprintf_s(cmd, MAX_BUFF_SIZE, "AT+CSCB=%d", cellBroadcast->mode);
    }

    err = SendCommandLock(cmd, "+CSCB:", 0, &responseInfo);
    if (err != 0 || !responseInfo->success) {
        goto ERROR;
    }

    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
    return;
ERROR:
    reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, NULL, 0);
    FreeResponseInfo(responseInfo);
}
