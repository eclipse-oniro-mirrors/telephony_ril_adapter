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

namespace OHOS {
namespace Telephony {
static int32_t g_facPointerNum = 1;
static int32_t g_passwordPointerNum = 2;

HRilSim::HRilSim()
{
    AddHandlerToMap();
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

void HRilSim::ProcessSimNotify(
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

    // Response
    respMemberFuncMap_[HREQ_SIM_IO] = &HRilSim::RequestSimIOResponse;
    respMemberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &HRilSim::GetSimStatusResponse;
    respMemberFuncMap_[HREQ_SIM_GET_IMSI] = &HRilSim::GetImsiResponse;
    respMemberFuncMap_[HREQ_SIM_GET_ICCID] = &HRilSim::GetIccIDResponse;
    respMemberFuncMap_[HREQ_SIM_GET_LOCK_STATUS] = &HRilSim::GetSimLockStatusResponse;
    respMemberFuncMap_[HREQ_SIM_SET_LOCK] = &HRilSim::SetSimLockResponse;
    respMemberFuncMap_[HREQ_SIM_CHANGE_PASSWD] = &HRilSim::ChangeSimPasswordResponse;
    respMemberFuncMap_[HREQ_SIM_ENTER_PIN] = &HRilSim::EnterSimPinResponse;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_PIN] = &HRilSim::UnlockSimPinResponse;
    respMemberFuncMap_[HREQ_SIM_GET_PIN_INPUT_TIMES] = &HRilSim::GetSimPinInputTimesResponse;

    // Request
    reqMemberFuncMap_[HREQ_SIM_IO] = &HRilSim::RequestSimIO;
    reqMemberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &HRilSim::GetSimStatus;
    reqMemberFuncMap_[HREQ_SIM_GET_IMSI] = &HRilSim::GetImsi;
    reqMemberFuncMap_[HREQ_SIM_GET_ICCID] = &HRilSim::GetIccID;
    reqMemberFuncMap_[HREQ_SIM_GET_LOCK_STATUS] = &HRilSim::GetSimLockStatus;
    reqMemberFuncMap_[HREQ_SIM_SET_LOCK] = &HRilSim::SetSimLock;
    reqMemberFuncMap_[HREQ_SIM_CHANGE_PASSWD] = &HRilSim::ChangeSimPassword;
    reqMemberFuncMap_[HREQ_SIM_ENTER_PIN] = &HRilSim::EnterSimPin;
    reqMemberFuncMap_[HREQ_SIM_UNLOCK_PIN] = &HRilSim::UnlockSimPin;
    reqMemberFuncMap_[HREQ_SIM_GET_PIN_INPUT_TIMES] = &HRilSim::GetSimPinInputTimes;
}

IccIoResultInfo HRilSim::ProcessIccIoResponse(
    HRilRadioResponseInfo &responseInfo, int32_t serial, const void *response, size_t responseLen)
{
    IccIoResultInfo result = {};
    if (response == nullptr || responseLen != sizeof(HRilSimIOResponse)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
        result.response = std::string("");
    } else {
        HRilSimIOResponse *resp = (HRilSimIOResponse *)response;
        result.sw1 = resp->sw1;
        result.sw2 = resp->sw2;
        if (resp->response == nullptr) {
            result.response = "";
        } else {
            result.response = std::string(resp->response);
        }
    }
    return result;
}

void HRilSim::GetSimStatus(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetSimStatus simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_SIM_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimStatus HRilRequest!");
        return;
    }
    simFuncs_->GetSimStatus(requestInfo);
    free(requestInfo);
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
        HRilCardState *curPtr = ((HRilCardState *)response);
        rilCardStatus.index = curPtr->index;
        rilCardStatus.simType = curPtr->simType;
        rilCardStatus.simState = curPtr->simState;
    }
    return ResponseMessageParcel(responseInfo, rilCardStatus, requestNum);
}

void HRilSim::RequestSimIO(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::RequestSimIO simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    const int32_t DATA_POINTER_NUM = 1;
    const int32_t PATH_POINTER_NUM = 2;
    SimIoRequestInfo SimIO = SimIoRequestInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in RequestSimIO is nullptr!");
        return;
    }
    if (!SimIO.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    serial = SimIO.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_IO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create RequestSimIO HRilRequest!");
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
        FreeStrings(DATA_POINTER_NUM, rilSimIO.data);
        free(requestInfo);
        return;
    }
    simFuncs_->GetSimIO(requestInfo, &rilSimIO, sizeof(HRilSimIO));
    FreeStrings(PATH_POINTER_NUM, rilSimIO.data, rilSimIO.pathid);
    free(requestInfo);
}

int32_t HRilSim::RequestSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    IccIoResultInfo result = ProcessIccIoResponse(responseInfo, responseInfo.serial, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

void HRilSim::GetImsi(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetImsi simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_IMSI);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetImsi HRilRequest!");
        return;
    }
    simFuncs_->GetSimImsi(requestInfo);
    free(requestInfo);
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
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(HRilRadioResponseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetImsiResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallback_->dispatcher->Dispatch(serviceCallback_, requestNum, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in GetImsiResponse is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

void HRilSim::GetIccID(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetIccID simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_ICCID);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create  GetIccID HRilRequest!");
        return;
    }
    simFuncs_->GetSimIccID(requestInfo);
    free(requestInfo);
}

int32_t HRilSim::GetIccIDResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in GetIccIDResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(HRilRadioResponseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetIccIDResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallback_->dispatcher->Dispatch(serviceCallback_, requestNum, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

void HRilSim::GetSimLockStatus(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetSimLockStatus simFuncs_ is nullptr!");
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
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_LOCK_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimLockStatus HRilRequest!");
        return;
    }
    HRilSimClock rilSimClock = {};
    rilSimClock.mode = simClock.mode;
    rilSimClock.status = simClock.status;
    rilSimClock.classx = simClock.classx;

    if (!ConvertToString(&rilSimClock.fac, simClock.fac, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in GetSimLockStatus is failed!");
        free(requestInfo);
        return;
    }
    if (!ConvertToString(&rilSimClock.passwd, simClock.passwd, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in GetSimLockStatus is failed!");
        FreeStrings(g_facPointerNum, rilSimClock.fac);
        free(requestInfo);
        return;
    }

    simFuncs_->GetSimLockStatus(requestInfo, &rilSimClock, sizeof(HRilSimClock));
    FreeStrings(g_passwordPointerNum, rilSimClock.fac, rilSimClock.passwd);
    free(requestInfo);
}

int32_t HRilSim::GetSimLockStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    int32_t simLockStatus = *(int32_t *)response;
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetSimLockStatusResponse dataSbuf is NULL.");
        return HDF_FAILURE;
    }

    if (!HdfSbufWriteInt32(dataSbuf, simLockStatus)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in GetSimLockStatusResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }

    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(HRilRadioResponseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetSimLockStatusResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }

    int32_t ret = serviceCallback_->dispatcher->Dispatch(serviceCallback_, requestNum, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in GetSimLockStatusResponse is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HDF_SUCCESS;
}

void HRilSim::SetSimLock(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::SetSimLock simFuncs_ is nullptr!");
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
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_SET_LOCK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetSimLock HRilRequest!");
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
        FreeStrings(g_facPointerNum, rilSimClock.fac);
        free(requestInfo);
        return;
    }
    rilSimClock.classx = simClock.classx;
    simFuncs_->SetSimLock(requestInfo, &rilSimClock, sizeof(HRilSimClock));
    FreeStrings(g_passwordPointerNum, rilSimClock.fac, rilSimClock.passwd);
    free(requestInfo);
}

int32_t HRilSim::SetSimLockResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

void HRilSim::ChangeSimPassword(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::ChangeSimPassword simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    const int32_t NEW_PASSWORD_POINTER_NUM = 2;
    SimPasswordInfo simPassword = SimPasswordInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel) || parcel == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel or Parcel in ChangeSimPassword is failed! ");
        return;
    }
    if (!simPassword.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    serial = simPassword.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_CHANGE_PASSWD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create ChangeSimPassword HRilRequest!");
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
        FreeStrings(g_facPointerNum, rilSimPassword.fac);
        free(requestInfo);
        return;
    }
    if (!ConvertToString(&rilSimPassword.newPassword, simPassword.newPassword, requestInfo)) {
        TELEPHONY_LOGE("ConvertToString in ChangeSimPassword is failed!");
        FreeStrings(g_passwordPointerNum, rilSimPassword.fac, rilSimPassword.oldPassword);
        free(requestInfo);
        return;
    }
    rilSimPassword.passwordLength = simPassword.passwordLength;
    simFuncs_->ChangeSimPassword(requestInfo, &rilSimPassword, sizeof(HRilSimPassword));
    FreeStrings(
        NEW_PASSWORD_POINTER_NUM, rilSimPassword.fac, rilSimPassword.oldPassword, rilSimPassword.newPassword);
    free(requestInfo);
}

int32_t HRilSim::ChangeSimPasswordResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

void HRilSim::EnterSimPin(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::EnterSimPin simFuncs_ is nullptr!");
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

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_ENTER_PIN);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create EnterSimPin HRilRequest!");
        return;
    }
    simFuncs_->EnterSimPin(requestInfo, pin);
    free(requestInfo);
}

int32_t HRilSim::EnterSimPinResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

void HRilSim::UnlockSimPin(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::UnlockSimPin simFuncs_ is nullptr!");
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

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_UNLOCK_PIN);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create UnlockSimPin HRilRequest!");
        return;
    }
    simFuncs_->UnlockSimPin(requestInfo, puk, pin);
    free(requestInfo);
}

int32_t HRilSim::UnlockSimPinResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(HRilRadioResponseInfo));
}

void HRilSim::GetSimPinInputTimes(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("HRilCall::GetSimPinInputTimes simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;

    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_PIN_INPUT_TIMES);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimPinInputTimes HRilRequest!");
        return;
    }
    simFuncs_->GetSimPinInputTimes(requestInfo);
    free(requestInfo);
}

int32_t HRilSim::GetSimPinInputTimesResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HRilPinInputTimes *pPinInputTimes = (HRilPinInputTimes *)response;
    if (pPinInputTimes == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HDF_FAILURE;
    }
    SimPinInputTimes pinInputTimesResult = {};
    pinInputTimesResult.code = pPinInputTimes->code;
    pinInputTimesResult.times = pPinInputTimes->times;
    pinInputTimesResult.pukTimes = pPinInputTimes->pukTimes;
    pinInputTimesResult.pinTimes = pPinInputTimes->pinTimes;
    pinInputTimesResult.puk2Times = pPinInputTimes->puk2Times;
    pinInputTimesResult.pin2Times = pPinInputTimes->pin2Times;
    return ResponseMessageParcel(responseInfo, pinInputTimesResult, requestNum);
}

int32_t HRilSim::SimStateUpdated(
    int32_t slotId, int32_t notifyType, const HRilErrno e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);

    if (serviceCallbackNotify_ == nullptr) {
        TELEPHONY_LOGE("RilAdapter serviceCallbackNotify_ is null");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = serviceCallbackNotify_->dispatcher->Dispatch(
        serviceCallbackNotify_, HNOTI_SIM_STATUS_CHANGED, dataSbuf, nullptr);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret in SimStateUpdated is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (dataSbuf != nullptr) {
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
