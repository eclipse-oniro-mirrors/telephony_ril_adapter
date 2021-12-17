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

#include "hril_sim.h"

#include "hril_notification.h"
#include "hril_request.h"

namespace OHOS {
namespace Telephony {
HRilSim::HRilSim()
{
    AddHandlerToMap();
}

void HRilSim::ProcessSimRequest(int32_t slotId, int32_t code, struct HdfSBuf *data)
{
    auto itFunc = reqMemberFuncMap_.find(code);
    if (itFunc != reqMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, data);
        }
    }
}

void HRilSim::ProcessSimResponse(
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

void HRilSim::ProcessSimNotify(int32_t slotId, int32_t notifyType, const struct ReportInfo *reportInfo,
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
    }
}

bool HRilSim::IsSimResponse(uint32_t code)
{
    return ((code >= HREQ_SIM_BASE) && (code < HREQ_DATA_BASE));
}

bool HRilSim::IsSimNotification(uint32_t code)
{
    return ((code >= HNOTI_SIM_BASE) && (code < HNOTI_DATA_BASE));
}

bool HRilSim::IsSimRespOrNotify(uint32_t code)
{
    return IsSimResponse(code) || IsSimNotification(code);
}

void HRilSim::AddHandlerToMap()
{
    // Notification
    notiMemberFuncMap_[HNOTI_SIM_STATUS_CHANGED] = &HRilSim::SimStateUpdated;

    // response
    respMemberFuncMap_[HREQ_SIM_GET_SIM_IO] = &HRilSim::GetSimIOResponse;
    respMemberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &HRilSim::GetSimStatusResponse;
    respMemberFuncMap_[HREQ_SIM_GET_IMSI] = &HRilSim::GetImsiResponse;
    respMemberFuncMap_[HREQ_SIM_GET_SIM_LOCK_STATUS] = &HRilSim::GetSimLockStatusResponse;
    respMemberFuncMap_[HREQ_SIM_SET_SIM_LOCK] = &HRilSim::SetSimLockResponse;
    respMemberFuncMap_[HREQ_SIM_CHANGE_SIM_PASSWORD] = &HRilSim::ChangeSimPasswordResponse;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_PIN] = &HRilSim::UnlockPinResponse;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_PUK] = &HRilSim::UnlockPukResponse;
    respMemberFuncMap_[HREQ_SIM_GET_SIM_PIN_INPUT_TIMES] = &HRilSim::GetSimPinInputTimesResponse;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_PIN2] = &HRilSim::UnlockPin2Response;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_PUK2] = &HRilSim::UnlockPuk2Response;
    respMemberFuncMap_[HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES] = &HRilSim::GetSimPin2InputTimesResponse;
    respMemberFuncMap_[HREQ_SIM_SET_ACTIVE_SIM] = &HRilSim::SetActiveSimResponse;

    // request
    reqMemberFuncMap_[HREQ_SIM_GET_SIM_IO] = &HRilSim::GetSimIO;
    reqMemberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &HRilSim::GetSimStatus;
    reqMemberFuncMap_[HREQ_SIM_GET_IMSI] = &HRilSim::GetImsi;
    reqMemberFuncMap_[HREQ_SIM_GET_SIM_LOCK_STATUS] = &HRilSim::GetSimLockStatus;
    reqMemberFuncMap_[HREQ_SIM_SET_SIM_LOCK] = &HRilSim::SetSimLock;
    reqMemberFuncMap_[HREQ_SIM_CHANGE_SIM_PASSWORD] = &HRilSim::ChangeSimPassword;
    reqMemberFuncMap_[HREQ_SIM_UNLOCK_PIN] = &HRilSim::UnlockPin;
    reqMemberFuncMap_[HREQ_SIM_UNLOCK_PUK] = &HRilSim::UnlockPuk;
    reqMemberFuncMap_[HREQ_SIM_GET_SIM_PIN_INPUT_TIMES] = &HRilSim::GetSimPinInputTimes;
    reqMemberFuncMap_[HREQ_SIM_UNLOCK_PIN2] = &HRilSim::UnlockPin2;
    reqMemberFuncMap_[HREQ_SIM_UNLOCK_PUK2] = &HRilSim::UnlockPuk2;
    reqMemberFuncMap_[HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES] = &HRilSim::GetSimPin2InputTimes;
    reqMemberFuncMap_[HREQ_SIM_SET_ACTIVE_SIM] = &HRilSim::SetActiveSim;
}

void HRilSim::GetSimIO(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial;
    SimIoRequestInfo SimIO = SimIoRequestInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in GetSimIO is nullptr!");
        return;
    }
    if (!SimIO.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    serial = SimIO.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_SIM_IO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    HRilSimIO rilSimIO = {};
    rilSimIO.command = SimIO.command;
    rilSimIO.fileid = SimIO.fileId;
    rilSimIO.p1 = SimIO.p1;
    rilSimIO.p2 = SimIO.p2;
    rilSimIO.p3 = SimIO.p3;
    if (!ConvertToString(&rilSimIO.data, SimIO.data, requestInfo)) {
        free(requestInfo);
        return;
    }
    if (!ConvertToString(&rilSimIO.pathid, SimIO.path, requestInfo)) {
        SafeFrees(rilSimIO.data);
        free(requestInfo);
        return;
    }
    if (!ConvertToString(&rilSimIO.pin2, SimIO.pin2, requestInfo)) {
        SafeFrees(rilSimIO.data, rilSimIO.pathid);
        free(requestInfo);
        return;
    }
    simFuncs_->GetSimIO(requestInfo, &rilSimIO, sizeof(rilSimIO));
    SafeFrees(rilSimIO.data, rilSimIO.pathid, rilSimIO.pin2);
    free(requestInfo);
}

void HRilSim::GetSimStatus(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_SIM_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    simFuncs_->GetSimStatus(requestInfo);
    free(requestInfo);
}

void HRilSim::GetImsi(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_IMSI);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    simFuncs_->GetSimImsi(requestInfo);
    free(requestInfo);
}

void HRilSim::SetSimLock(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    SimLockInfo simClock = SimLockInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SetSimLock is nullptr!");
        return;
    }
    if (!simClock.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    serial = simClock.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_SET_SIM_LOCK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    HRilSimClock rilSimClock = {};
    if (!ConvertToString(&rilSimClock.fac, simClock.fac, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in SetSimLock is failed!");
        free(requestInfo);
        return;
    }
    rilSimClock.mode = simClock.mode;
    rilSimClock.status = simClock.status;
    if (!ConvertToString(&rilSimClock.passwd, simClock.passwd, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in SetSimLock is failed!");
        SafeFrees(rilSimClock.fac);
        free(requestInfo);
        return;
    }
    rilSimClock.classx = simClock.classx;
    simFuncs_->SetSimLock(requestInfo, &rilSimClock, sizeof(rilSimClock));
    SafeFrees(rilSimClock.fac, rilSimClock.passwd);
    free(requestInfo);
}

void HRilSim::GetSimLockStatus(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    SimLockInfo simClock = SimLockInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in GetSimLockStatus is nullptr!");
        return;
    }
    if (!simClock.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    serial = simClock.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_SIM_LOCK_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    HRilSimClock rilSimClock = {};
    rilSimClock.fac = rilSimClock.fac;
    rilSimClock.mode = simClock.mode;
    rilSimClock.status = simClock.status;
    rilSimClock.passwd = rilSimClock.passwd;
    rilSimClock.classx = rilSimClock.classx;

    if (!ConvertToString(&rilSimClock.fac, simClock.fac, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in GetSimLockStatus is failed!");
        free(requestInfo);
        return;
    }
    if (!ConvertToString(&rilSimClock.passwd, simClock.passwd, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in GetSimLockStatus is failed!");
        SafeFrees(rilSimClock.fac);
        free(requestInfo);
        return;
    }

    simFuncs_->GetSimLockStatus(requestInfo, &rilSimClock, sizeof(rilSimClock));
    SafeFrees(rilSimClock.fac, rilSimClock.passwd);
    free(requestInfo);
}

void HRilSim::ChangeSimPassword(int32_t slotId, struct HdfSBuf *data)
{
    int32_t serial;
    SimPasswordInfo simPassword = SimPasswordInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr || simFuncs_ == nullptr) {
        TELEPHONY_LOGE("ChangeSimPassword failed : nullptr!");
        return;
    }
    if (!simPassword.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    serial = simPassword.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_CHANGE_SIM_PASSWORD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    HRilSimPassword rilSimPassword = {};
    if (!ConvertToString(&rilSimPassword.fac, simPassword.fac, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in ChangeSimPassword is failed!");
        free(requestInfo);
        return;
    }
    if (!ConvertToString(&rilSimPassword.oldPassword, simPassword.oldPassword, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in ChangeSimPassword is failed!");
        SafeFrees(rilSimPassword.fac);
        free(requestInfo);
        return;
    }
    if (!ConvertToString(&rilSimPassword.newPassword, simPassword.newPassword, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in ChangeSimPassword is failed!");
        SafeFrees(rilSimPassword.fac, rilSimPassword.oldPassword);
        free(requestInfo);
        return;
    }
    rilSimPassword.passwordLength = simPassword.passwordLength;
    simFuncs_->ChangeSimPassword(requestInfo, &rilSimPassword, sizeof(rilSimPassword));
    SafeFrees(rilSimPassword.fac, rilSimPassword.oldPassword, rilSimPassword.newPassword);
    free(requestInfo);
}

void HRilSim::UnlockPin(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    const char *pin = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in EnterSimPin is nullptr!");
        return;
    }
    pin = parcel->ReadCString();
    if (pin == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_UNLOCK_PIN);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    simFuncs_->UnlockPin(requestInfo, pin);
    free(requestInfo);
}

void HRilSim::UnlockPuk(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    const char *puk = nullptr;
    const char *pin = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    puk = HdfSbufReadString(data);
    if (puk == nullptr) {
        TELEPHONY_LOGE("miss puk parameter");
        return;
    }
    pin = HdfSbufReadString(data);
    if (pin == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_UNLOCK_PUK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    simFuncs_->UnlockPuk(requestInfo, puk, pin);
    free(requestInfo);
}

void HRilSim::GetSimPinInputTimes(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_SIM_PIN_INPUT_TIMES);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    simFuncs_->GetSimPinInputTimes(requestInfo);
    free(requestInfo);
}

void HRilSim::UnlockPin2(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    const char *pin2 = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    pin2 = parcel->ReadCString();
    if (pin2 == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_UNLOCK_PIN2);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    simFuncs_->UnlockPin2(requestInfo, pin2);
    free(requestInfo);
}

void HRilSim::UnlockPuk2(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    const char *puk2 = nullptr;
    const char *pin2 = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    puk2 = HdfSbufReadString(data);
    if (puk2 == nullptr) {
        TELEPHONY_LOGE("miss puk2 parameter");
        return;
    }
    pin2 = HdfSbufReadString(data);
    if (pin2 == nullptr) {
        TELEPHONY_LOGE("miss pin2 parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_UNLOCK_PUK2);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    simFuncs_->UnlockPuk2(requestInfo, puk2, pin2);
    free(requestInfo);
}

void HRilSim::GetSimPin2InputTimes(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    simFuncs_->GetSimPin2InputTimes(requestInfo);
    free(requestInfo);
}

void HRilSim::SetActiveSim(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    int index = 0;
    int enable = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("miss index parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &enable)) {
        TELEPHONY_LOGE("miss enable parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_SET_ACTIVE_SIM);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do CreateHRilRequest!");
        return;
    }
    simFuncs_->SetActiveSim(requestInfo, index, enable);
    free(requestInfo);
}

int32_t HRilSim::GetSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    IccIoResultInfo result = ProcessIccIoResponse(responseInfo, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSim::GetSimStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    CardStatusInfo rilCardStatus = {};
    if (response == nullptr || responseLen != sizeof(HRilCardState)) {
        TELEPHONY_LOGE("GetSimStatusResponse: Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilCardState *curPtr = static_cast<const HRilCardState *>(response);
        rilCardStatus.index = curPtr->index;
        rilCardStatus.simType = curPtr->simType;
        rilCardStatus.simState = curPtr->simState;
    }
    return ResponseMessageParcel(responseInfo, rilCardStatus, requestNum);
}

int32_t HRilSim::GetImsiResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("Error : dataSbuf in GetImsiResponse is nullptr!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in GetImsiResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetImsiResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in GetImsiResponse is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);

    return HDF_SUCCESS;
}

int32_t HRilSim::SetSimLockResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::GetSimLockStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    int32_t simLockStatus = 0;
    if (response == nullptr || responseLen != sizeof(int32_t)) {
        TELEPHONY_LOGE("Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        simLockStatus = *(static_cast<const int32_t *>(response));
    }
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("GetSimLockStatusResponse dataSbuf is NULL.");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, simLockStatus)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in GetSimLockStatusResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetSimLockStatusResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in GetSimLockStatusResponse is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

int32_t HRilSim::ChangeSimPasswordResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::UnlockPinResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::UnlockPukResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::GetSimPinInputTimesResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    const HRilPinInputTimes *pPinInputTimes = static_cast<const HRilPinInputTimes *>(response);
    if (pPinInputTimes == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HDF_FAILURE;
    }
    SimPinInputTimes pinInputTimesResult;
    pinInputTimesResult.code = pPinInputTimes->code;
    pinInputTimesResult.times = pPinInputTimes->times;
    pinInputTimesResult.pukTimes = pPinInputTimes->pukTimes;
    pinInputTimesResult.pinTimes = pPinInputTimes->pinTimes;
    pinInputTimesResult.puk2Times = pPinInputTimes->puk2Times;
    pinInputTimesResult.pin2Times = pPinInputTimes->pin2Times;
    return ResponseMessageParcel(responseInfo, pinInputTimesResult, requestNum);
}

int32_t HRilSim::UnlockPin2Response(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::UnlockPuk2Response(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::GetSimPin2InputTimesResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    const HRilPinInputTimes *pPin2InputTimes = static_cast<const HRilPinInputTimes *>(response);
    if (pPin2InputTimes == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HDF_FAILURE;
    }
    SimPinInputTimes pin2InputTimesResult;
    pin2InputTimesResult.code = pPin2InputTimes->code;
    pin2InputTimesResult.times = pPin2InputTimes->times;
    pin2InputTimesResult.pukTimes = pPin2InputTimes->pukTimes;
    pin2InputTimesResult.pinTimes = pPin2InputTimes->pinTimes;
    pin2InputTimesResult.puk2Times = pPin2InputTimes->puk2Times;
    pin2InputTimesResult.pin2Times = pPin2InputTimes->pin2Times;
    return ResponseMessageParcel(responseInfo, pin2InputTimesResult, requestNum);
}

int32_t HRilSim::SetActiveSimResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

IccIoResultInfo HRilSim::ProcessIccIoResponse(
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    IccIoResultInfo result = {};
    if (response == nullptr || responseLen != sizeof(HRilSimIOResponse)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
        result.response = std::string("");
    } else {
        const HRilSimIOResponse *resp = static_cast<const HRilSimIOResponse *>(response);
        result.sw1 = resp->sw1;
        result.sw2 = resp->sw2;
        if (resp->response == nullptr) {
            TELEPHONY_LOGE("resp->response is nullptr");
            result.response = "";
        } else {
            result.response = std::string(resp->response);
        }
    }
    return result;
}

int32_t HRilSim::SimStateUpdated(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SIM_STATUS_CHANGED, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in SimStateUpdated is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
        TELEPHONY_LOGE("dataSbuf is  null!");
        HdfSBufRecycle(dataSbuf);
    }
    return HDF_SUCCESS;
}

void HRilSim::RegisterSimFuncs(const HRilSimReq *simFuncs)
{
    simFuncs_ = simFuncs;
}
} // namespace Telephony
} // namespace OHOS
