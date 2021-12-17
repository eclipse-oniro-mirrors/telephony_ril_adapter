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

#include "hril_modem.h"

#include "hril_notification.h"
#include "hril_request.h"

namespace OHOS {
namespace Telephony {
HRilModem::HRilModem()
{
    AddHandlerToMap();
}

void HRilModem::SetRadioState(int32_t slotId, struct HdfSBuf *data)
{
    if (modemFuncs_ == nullptr) {
        TELEPHONY_LOGE("it is null for modem-func set");
        return;
    }
    int32_t serial = -1;
    int32_t fun = -1;
    int32_t rst = -1;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &fun)) {
        TELEPHONY_LOGE("miss fun parameter");
        return;
    }

    if (!HdfSbufReadInt32(data, &rst)) {
        TELEPHONY_LOGE("miss rst parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_MODEM_SET_RADIO_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("requestInfo == nullptr Request is fail");
        return;
    }
    modemFuncs_->SetRadioState(requestInfo, fun, rst);
    free(requestInfo);
}

int32_t HRilModem::SetRadioStateResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("HRilModem::SetRadioStateResponse  HDF_FAILURE ");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write unpad buffer failed");
        return HDF_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("Dispatcher failed: ret:%d", ret);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

void HRilModem::GetRadioState(int32_t slotId, struct HdfSBuf *data)
{
    if (modemFuncs_ == nullptr) {
        TELEPHONY_LOGE("it is null for modem-func set");
        return;
    }
    int32_t serial = -1;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_MODEM_GET_RADIO_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("requestInfo == nullptr Request is fail");
        return;
    }
    modemFuncs_->GetRadioState(requestInfo);
    free(requestInfo);
}

int32_t HRilModem::GetRadioStateResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    int32_t radioState = 0;
    if (response != nullptr) {
        radioState = *(int *)response;
    }
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("HRilModem::GetRadioStateResponse HDF_FAILURE ");
        return HDF_FAILURE;
    }

    if (!HdfSbufWriteInt32(dataSbuf, radioState)) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write data failed");
        return HDF_FAILURE;
    }

    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write unpad buffer failed");
        return HDF_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("dispatch failed: ret:%d,radioState:%d", ret, radioState);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

bool HRilModem::IsModemResponse(uint32_t code)
{
    return ((code >= HREQ_COMMON_BASE) && (code <= HREQ_MODEM_GET_RADIO_STATUS));
}

bool HRilModem::IsModemNotification(uint32_t code)
{
    return ((code >= HNOTI_COMMON_BASE) && (code <= HNOTI_MODEM_RADIO_STATE_UPDATED));
}

bool HRilModem::IsModemRespOrNotify(uint32_t code)
{
    return IsModemResponse(code) || IsModemNotification(code);
}

int32_t HRilModem::RadioStateUpdated(
    const int32_t slotId, int32_t indType, const HRilErrno e, const void *response, size_t responselen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HDF_FAILURE;
    }
    HRilRadioState radioState = *((HRilRadioState *)response);
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("get hdf handler failed");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, static_cast<int32_t>(radioState))) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write data failed");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write data failed");
        return HDF_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_MODEM_RADIO_STATE_UPDATED, dataSbuf);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("dispatch failed: ret:%d", ret);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

void HRilModem::AddHandlerToMap()
{
    // indication
    notiMemberFuncMap_[HNOTI_MODEM_RADIO_STATE_UPDATED] = &HRilModem::RadioStateUpdated;
    // response
    respMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &HRilModem::SetRadioStateResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &HRilModem::GetRadioStateResponse;
    // request
    reqMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &HRilModem::SetRadioState;
    reqMemberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &HRilModem::GetRadioState;
}

void HRilModem::ProcessModemResponse(
    int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    auto itFunc = respMemberFuncMap_.find(code);
    if (itFunc != respMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, code, responseInfo, response, responseLen);
        }
    } else {
        TELEPHONY_LOGE("not find response func: %{public}d", code);
    }
}

void HRilModem::ProcessModemNotify(int32_t slotId, int32_t notifyType, const struct ReportInfo *reportInfo,
    const void *response, size_t responseLen)
{
    int code;
    HRilErrno e;
    code = reportInfo->notifyId;
    e = (HRilErrno)reportInfo->error;
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, notifyType, e, response, responseLen);
        }
    } else {
        TELEPHONY_LOGE("not find notify func: %{public}d", code);
    }
}

void HRilModem::ProcessCommonRequest(int32_t slotId, int32_t code, struct HdfSBuf *data)
{
    auto itFunc = reqMemberFuncMap_.find(code);
    if (itFunc != reqMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, data);
        }
    } else {
        TELEPHONY_LOGE("not find request func: %{public}d", code);
    }
}
void HRilModem::RegisterModemFuncs(const HRilModemReq *modemFuncs)
{
    modemFuncs_ = modemFuncs;
}
} // namespace Telephony
} // namespace OHOS