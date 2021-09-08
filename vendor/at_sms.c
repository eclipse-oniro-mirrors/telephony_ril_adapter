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
#include "vendor_adapter.h"

int ProcessCellBroadcast(char *s, HRilCellBroadcastReportInfo *response)
{
    int ret = 0;
    char *p = NULL;
    int count = 0;
    if (s == NULL || response == NULL) {
        return -1;
    }
    p = s;
    while (*p != '\0' && p != NULL) {
        if (*p == ',') {
            count++;
        }
        p++;
    }
    SkipATPrefix(&s);
    if (count > 1) {
        ret = NextInt(&s, &response->sn);
        if (ret == -1) {
            TELEPHONY_LOGE("sn is null");
        }
        ret = NextInt(&s, &response->mid);
        if (ret == -1) {
            TELEPHONY_LOGE("mid is null");
        }
        ret = NextInt(&s, &response->page);
        if (ret == -1) {
            TELEPHONY_LOGE("page is null");
        }
        ret = NextInt(&s, &response->pages);
        if (ret == -1) {
            TELEPHONY_LOGE("pages is null");
        }
        ret = NextStr(&s, &response->dcs);
        if (ret == -1) {
            TELEPHONY_LOGE("dcs is null");
        }
    } else {
        ret = NextInt(&s, &response->length);
        if (ret == -1) {
            TELEPHONY_LOGE("mode is null");
        }
    }
    return count;
}

void ReqSendSms(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    int err;
    char *smsc = NULL;
    const char *pdu = NULL;
    char *cmd = NULL;
    char *smsPdu = NULL;
    char *result = NULL;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    HRilSmsResponse response;
    (void)memset_s(&response, sizeof(HRilSmsResponse), 0, sizeof(HRilSmsResponse));
    TELEPHONY_LOGD("%{public}s enter", __func__);

    smsc = ((char **)data)[0];
    if (smsc == NULL) {
        asprintf(&smsc, "00");
    }
    pdu = ((const char **)data)[1];
    (void)memset_s(&response, sizeof(response), 0, sizeof(response));
    TELEPHONY_LOGD("%{public}s enter, smsc:%{public}s, pdu:%{public}s", __func__, smsc, pdu);
    asprintf(&cmd, "AT+CMGS=%zu", strlen(pdu) / 2);
    asprintf(&smsPdu, "%s%s", smsc, pdu);

    err = SendCommandSmsLock(cmd, smsPdu, "+CMGS:", 0, &responseInfo);
    free(cmd);
    free(smsPdu);
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
        response.msgRef = 1;
    }
    reportInfo = CreateReportInfo(requestInfo, VENDOR_SUCCESS, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, &response, sizeof(HRilSmsResponse));
    FreeResponseInfo(responseInfo);
    return;
ERROR:
    response.msgRef = -2;
    reportInfo = CreateReportInfo(requestInfo, AT_ERR_GENERIC, HRIL_RESPONSE, 0);
    OnSmsReport(reportInfo, &response, sizeof(HRilSmsResponse));
    FreeResponseInfo(responseInfo);
    return;
}

void ReqSendSmsAck(ReqDataInfo *requestInfo, const void *data, size_t dataLen)
{
    TELEPHONY_LOGD("%{public}s enter", __func__);
    int ackFlag;
    int err;
    ackFlag = ((int *)data)[0];
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    if (ackFlag == 1) {
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
    int err;
    HRilSmsWriteSms *msg = NULL;
    char *cmd = NULL;
    char *smsPdu = NULL;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    TELEPHONY_LOGD("%{public}s enter", __func__);

    msg = ((HRilSmsWriteSms *)data);
    TELEPHONY_LOGD("%{public}s enter, pdu:%{public}s, smsc:%{public}s, status:%{public}d", __func__, msg->pdu,
        msg->smsc, msg->state);
    if (msg->smsc == NULL || (strcmp(msg->smsc, "") == 0)) {
        strcpy_s(msg->smsc, strlen("00"), "00");
    }
    asprintf(&cmd, "AT+CMGW=%zu,%d", strlen(msg->pdu) / 2, msg->state);
    asprintf(&smsPdu, "%s%s", msg->smsc, msg->pdu);

    err = SendCommandSmsLock(cmd, smsPdu, "+CMGW:", 0, &responseInfo);
    free(cmd);
    free(smsPdu);
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
    int err;
    int index;
    char *cmd = NULL;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    TELEPHONY_LOGD("%{public}s enter", __func__);
    if (data == NULL) {
        err = HRIL_ERR_GENERIC_FAILURE;
        goto ERROR;
    }
    index = ((int *)data)[0];
    TELEPHONY_LOGD("%{public}s enter, index:%{public}d", __func__, index);
    asprintf(&cmd, "AT+CMGD=%d", index);

    err = SendCommandLock(cmd, "+CMGD:", 0, &responseInfo);
    free(cmd);
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
    int err;
    HRilSmsWriteSms *msg = NULL;
    char *cmd = NULL;
    char *smsPdu = NULL;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    TELEPHONY_LOGD("%{public}s enter", __func__);
    if (data == NULL) {
        err = HRIL_ERR_GENERIC_FAILURE;
        goto ERROR;
    }
    msg = ((HRilSmsWriteSms *)data);
    TELEPHONY_LOGD("%{public}s enter, pdu:%{public}s, state:%{public}d", __func__, msg->pdu, msg->state);

    asprintf(&cmd, "AT+CMGD=%d", msg->index);

    err = SendCommandLock(cmd, "+CMGD:", 0, &responseInfo);
    free(cmd);
    cmd = NULL;
    if (err != 0 || !responseInfo->success) {
        goto ERROR;
    }

    asprintf(&cmd, "AT+CMGW=%zu,%d", strlen(msg->pdu) / 2, msg->state);
    asprintf(&smsPdu, "%s", msg->pdu);

    err = SendCommandSmsLock(cmd, smsPdu, "+CMGW:", 0, &responseInfo);
    free(cmd);
    free(smsPdu);
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
    int err;
    HRilServiceCenterAddress *address = NULL;
    char *cmd = NULL;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    TELEPHONY_LOGD("%{public}s enter", __func__);
    if (data == NULL) {
        err = HRIL_ERR_GENERIC_FAILURE;
        goto ERROR;
    }
    address = ((HRilServiceCenterAddress *)data);
    TELEPHONY_LOGD(
        "%{public}s enter, address:%{public}s, tosca:%{public}d", __func__, address->address, address->tosca);
    if (address->tosca == 0) {
        asprintf(&cmd, "AT+CSCA=\"%s\"", address->address);
    } else {
        asprintf(&cmd, "AT+CSCA=\"%s\",%d", address->address, address->tosca);
    }
    err = SendCommandLock(cmd, "+CSCA:", 0, &responseInfo);
    free(cmd);
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
    int err;
    char *result = NULL;
    HRilServiceCenterAddress response;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    TELEPHONY_LOGD("%{public}s enter", __func__);
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
        if (err == -1) {
            (void)strcpy_s(response.address, strlen("") + 1, "");
        }
        err = NextInt(&result, &response.tosca);
        if (err == -1) {
            response.tosca = 0;
        }
        TELEPHONY_LOGD("result[address:%{public}s, tosca:%{public}d]", response.address, response.tosca);
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
    int err;
    HRilCellBroadcastInfo *cellBroadcast = NULL;
    char *cmd = NULL;
    struct ReportInfo reportInfo;
    (void)memset_s(&reportInfo, sizeof(struct ReportInfo), 0, sizeof(struct ReportInfo));
    ResponseInfo *responseInfo = NULL;
    TELEPHONY_LOGD("%{public}s enter", __func__);
    if (data == NULL) {
        err = HRIL_ERR_GENERIC_FAILURE;
        goto ERROR;
    }
    cellBroadcast = ((HRilCellBroadcastInfo *)data);
    TELEPHONY_LOGD("%{public}s enter, mode:%{public}d, dcss:%{public}s, mids:%{public}s", __func__,
        cellBroadcast->mode, cellBroadcast->dcss, cellBroadcast->mids);
    if (strcmp(cellBroadcast->mids, "") && !strcmp(cellBroadcast->dcss, "")) {
        asprintf(&cmd, "AT+CSCB=%d,\"%s\"", cellBroadcast->mode, cellBroadcast->mids);
    } else if (strcmp(cellBroadcast->dcss, "") && strcmp(cellBroadcast->dcss, "")) {
        asprintf(&cmd, "AT+CSCB=%d,\"%s\",\"%s\"", cellBroadcast->mode, cellBroadcast->mids, cellBroadcast->dcss);
    } else if (strcmp(cellBroadcast->dcss, "") && !strcmp(cellBroadcast->mids, "")) {
        asprintf(&cmd, "AT+CSCB=%d,,\"%s\"", cellBroadcast->mode, cellBroadcast->dcss);
    } else {
        asprintf(&cmd, "AT+CSCB=%d", cellBroadcast->mode);
    }

    err = SendCommandLock(cmd, "+CSCB:", 0, &responseInfo);
    free(cmd);
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
