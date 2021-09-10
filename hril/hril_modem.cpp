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

namespace OHOS {
namespace Telephony {
HRilModem::HRilModem()
{
    AddHandlerToMap();
}

void HRilModem::SetRadioStatus(int32_t slotId, struct HdfSBuf *data)
{
    TELEPHONY_LOGD("HRilModem SetRadioStatus enter--> slotId: %{public}d", slotId);

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
    modemFuncs_->SetRadioStatus(requestInfo, fun, rst);
    free(requestInfo);
}

int32_t HRilModem::SetRadioStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    TELEPHONY_LOGD("HRilModem::SetRadioStatusResponse  enter --> ");
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("HRilModem::SetRadioStatusResponse  HDF_FAILURE ");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallback_->dispatcher->Dispatch(serviceCallback_, requestNum, dataSbuf, nullptr);
    TELEPHONY_LOGD("HRilModem::SetRadioStatusResponse  ret:%{public}d ", ret);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

void HRilModem::GetRadioStatus(int32_t slotId, struct HdfSBuf *data)
{
    TELEPHONY_LOGD("HRilModem GetRadioStatus enter--> slotId: %{public}d", slotId);

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
    modemFuncs_->GetRadioStatus(requestInfo);
    free(requestInfo);
}

int32_t HRilModem::GetRadioStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    TELEPHONY_LOGD("HRilModem::GetRadioStatusResponse  enter --> ");
    int32_t radioStatus = 0;
    if (response != nullptr) {
        radioStatus = *(int *)response;
    }
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("HRilModem::GetRadioStatusResponse HDF_FAILURE ");
        return HDF_FAILURE;
    }

    if (!HdfSbufWriteInt32(dataSbuf, radioStatus)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }

    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallback_->dispatcher->Dispatch(serviceCallback_, requestNum, dataSbuf, nullptr);
    TELEPHONY_LOGD("HRilModem::GetRadioStatusResponse ret:%{public}d radioStatus:%{public}d ", ret, radioStatus);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
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
    HRilRadioState radioState = *((HRilRadioState *)response);
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, static_cast<int32_t>(radioState))) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter serviceCallbackNotify_ is null");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallbackNotify_->dispatcher->Dispatch(
        serviceCallbackNotify_, HNOTI_MODEM_RADIO_STATE_UPDATED, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

void HRilModem::AddHandlerToMap()
{
    // indication
    notiMemberFuncMap_[HNOTI_MODEM_RADIO_STATE_UPDATED] = &HRilModem::RadioStateUpdated;
    // response
    respMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &HRilModem::SetRadioStatusResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &HRilModem::GetRadioStatusResponse;
    // request
    reqMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &HRilModem::SetRadioStatus;
    reqMemberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &HRilModem::GetRadioStatus;
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
    }
}

void HRilModem::ProcessModemNotify(
    int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen)
{
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(code, notifyType, e, response, responseLen);
        }
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
    }
}
void HRilModem::RegisterModemFuncs(const HRilModemReq *modemFuncs)
{
    modemFuncs_ = modemFuncs;
}
} // namespace Telephony
} // namespace OHOS