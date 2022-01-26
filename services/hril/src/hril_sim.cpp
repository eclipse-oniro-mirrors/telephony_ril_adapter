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
HRilSim::HRilSim(int32_t slotId, IHRilReporter &hrilReporter) : HRilBase(slotId, hrilReporter)
{
    AddHandlerToMap();
}

bool HRilSim::IsSimRespOrNotify(uint32_t code)
{
    return IsSimResponse(code) || IsSimNotification(code);
}

void HRilSim::RegisterSimFuncs(const HRilSimReq *simFuncs)
{
    simFuncs_ = simFuncs;
}

void HRilSim::AddHandlerToMap()
{
    // Notification
    notiMemberFuncMap_[HNOTI_SIM_STATUS_CHANGED] = &HRilSim::SimStateUpdated;
    notiMemberFuncMap_[HNOTI_SIM_STK_SESSION_END_NOTIFY] = &HRilSim::SimStkSessionEndNotify;
    notiMemberFuncMap_[HNOTI_SIM_STK_PROACTIVE_NOTIFY] = &HRilSim::SimStkProactiveNotify;
    notiMemberFuncMap_[HNOTI_SIM_STK_ALPHA_NOTIFY] = &HRilSim::SimStkAlphaNotify;

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
    respMemberFuncMap_[HREQ_SIM_STK_SEND_TERMINAL_RESPONSE] = &HRilSim::SimStkSendTerminalResponseResponse;
    respMemberFuncMap_[HREQ_SIM_STK_SEND_ENVELOPE] = &HRilSim::SimStkSendEnvelopeResponse;
    respMemberFuncMap_[HREQ_SIM_STK_IS_READY] = &HRilSim::SimStkIsReadyResponse;
    respMemberFuncMap_[HREQ_SIM_RADIO_PROTOCOL] = &HRilSim::SetRadioProtocolResponse;
    respMemberFuncMap_[HREQ_SIM_OPEN_LOGICAL_CHANNEL] = &HRilSim::SimOpenLogicalChannelResponse;
    respMemberFuncMap_[HREQ_SIM_CLOSE_LOGICAL_CHANNEL] = &HRilSim::SimCloseLogicalChannelResponse;
    respMemberFuncMap_[HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL] = &HRilSim::SimTransmitApduLogicalChannelResponse;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_SIM_LOCK] = &HRilSim::UnlockSimLockResponse;

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
    reqMemberFuncMap_[HREQ_SIM_STK_SEND_TERMINAL_RESPONSE] = &HRilSim::SimStkSendTerminalResponse;
    reqMemberFuncMap_[HREQ_SIM_STK_SEND_ENVELOPE] = &HRilSim::SimStkSendEnvelope;
    reqMemberFuncMap_[HREQ_SIM_STK_IS_READY] = &HRilSim::SimStkIsReady;
    reqMemberFuncMap_[HREQ_SIM_RADIO_PROTOCOL] = &HRilSim::SetRadioProtocol;
    reqMemberFuncMap_[HREQ_SIM_OPEN_LOGICAL_CHANNEL] = &HRilSim::SimOpenLogicalChannel;
    reqMemberFuncMap_[HREQ_SIM_CLOSE_LOGICAL_CHANNEL] = &HRilSim::SimCloseLogicalChannel;
    reqMemberFuncMap_[HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL] = &HRilSim::SimTransmitApduLogicalChannel;
    reqMemberFuncMap_[HREQ_SIM_UNLOCK_SIM_LOCK] = &HRilSim::UnlockSimLock;
}

int32_t HRilSim::GetSimIO(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->GetSimIO == nullptr)) {
        TELEPHONY_LOGE("GetSimIO::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    SimIoRequestInfo SimIO = SimIoRequestInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in GetSimIO is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!SimIO.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t serial = SimIO.serial;
    HRilSimIO rilSimIO = {};
    rilSimIO.command = SimIO.command;
    rilSimIO.fileid = SimIO.fileId;
    rilSimIO.p1 = SimIO.p1;
    rilSimIO.p2 = SimIO.p2;
    rilSimIO.p3 = SimIO.p3;
    rilSimIO.data = static_cast<char *>(const_cast<char *>(SimIO.data.c_str()));
    rilSimIO.pathid = static_cast<char *>(const_cast<char *>(SimIO.path.c_str()));
    rilSimIO.pin2 = static_cast<char *>(const_cast<char *>(SimIO.pin2.c_str()));
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_GET_SIM_IO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimIO HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->GetSimIO(requestInfo, &rilSimIO, sizeof(rilSimIO));
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::GetSimStatus(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->GetSimStatus == nullptr)) {
        TELEPHONY_LOGE("GetSimStatus::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_GET_SIM_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimStatus HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->GetSimStatus(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::GetImsi(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->GetSimImsi == nullptr)) {
        TELEPHONY_LOGE("GetSimImsi::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_GET_IMSI);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetImsi HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->GetSimImsi(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::GetSimLockStatus(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->GetSimLockStatus == nullptr)) {
        TELEPHONY_LOGE("GetSimLockStatus::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    SimLockInfo simClock = SimLockInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in GetSimLockStatus is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!simClock.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilSimClock rilSimClock = {};
    rilSimClock.mode = simClock.mode;
    rilSimClock.status = simClock.status;
    rilSimClock.classx = simClock.classx;
    rilSimClock.fac = static_cast<char *>(const_cast<char *>(simClock.fac.c_str()));
    rilSimClock.passwd = static_cast<char *>(const_cast<char *>(simClock.passwd.c_str()));
    int32_t serial = simClock.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_GET_SIM_LOCK_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimLockStatus HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->GetSimLockStatus(requestInfo, &rilSimClock, sizeof(rilSimClock));
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SetSimLock(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->SetSimLock == nullptr)) {
        TELEPHONY_LOGE("SetSimLock::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    SimLockInfo simClock = SimLockInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SetSimLock is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!simClock.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilSimClock rilSimClock = {};
    rilSimClock.fac = static_cast<char *>(const_cast<char *>(simClock.fac.c_str()));
    rilSimClock.mode = simClock.mode;
    rilSimClock.status = simClock.status;
    rilSimClock.passwd = static_cast<char *>(const_cast<char *>(simClock.passwd.c_str()));
    rilSimClock.classx = simClock.classx;
    int32_t serial = simClock.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_SET_SIM_LOCK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetSimLock HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->SetSimLock(requestInfo, &rilSimClock, sizeof(rilSimClock));
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::ChangeSimPassword(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->ChangeSimPassword == nullptr)) {
        TELEPHONY_LOGE("ChangeSimPassword::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    SimPasswordInfo simPassword = SimPasswordInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr || simFuncs_ == nullptr) {
        TELEPHONY_LOGE("ChangeSimPassword failed : nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!simPassword.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilSimPassword rilSimPassword = {};
    rilSimPassword.fac = static_cast<char *>(const_cast<char *>(simPassword.fac.c_str()));
    rilSimPassword.oldPassword = static_cast<char *>(const_cast<char *>(simPassword.oldPassword.c_str()));
    rilSimPassword.newPassword = static_cast<char *>(const_cast<char *>(simPassword.newPassword.c_str()));
    rilSimPassword.passwordLength = simPassword.passwordLength;
    int32_t serial = simPassword.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_CHANGE_SIM_PASSWORD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create ChangeSimPassword HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->ChangeSimPassword(requestInfo, &rilSimPassword, sizeof(rilSimPassword));
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::UnlockPin(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->UnlockPin == nullptr)) {
        TELEPHONY_LOGE("UnlockPin::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    const char *pin = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in EnterSimPin is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    pin = parcel->ReadCString();
    if (pin == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_UNLOCK_PIN);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create EnterSimPin HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->UnlockPin(requestInfo, pin);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::UnlockPuk(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->UnlockPuk == nullptr)) {
        TELEPHONY_LOGE("UnlockPuk::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    const char *puk = nullptr;
    const char *pin = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    puk = HdfSbufReadString(data);
    if (puk == nullptr) {
        TELEPHONY_LOGE("miss puk parameter");
        return HRIL_ERR_NULL_POINT;
    }
    pin = HdfSbufReadString(data);
    if (pin == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_UNLOCK_PUK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create UnlockSimPin HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->UnlockPuk(requestInfo, puk, pin);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::GetSimPinInputTimes(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->GetSimPinInputTimes == nullptr)) {
        TELEPHONY_LOGE("GetSimPinInputTimes::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_GET_SIM_PIN_INPUT_TIMES);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimPinInputTimes HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->GetSimPinInputTimes(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::UnlockPin2(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->UnlockPin2 == nullptr)) {
        TELEPHONY_LOGE("UnlockPin2::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    const char *pin2 = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    pin2 = parcel->ReadCString();
    if (pin2 == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_UNLOCK_PIN2);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create EnterSimPin HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->UnlockPin2(requestInfo, pin2);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::UnlockPuk2(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->UnlockPuk2 == nullptr)) {
        TELEPHONY_LOGE("UnlockPuk2::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    const char *puk2 = nullptr;
    const char *pin2 = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    puk2 = HdfSbufReadString(data);
    if (puk2 == nullptr) {
        TELEPHONY_LOGE("miss puk2 parameter");
        return HRIL_ERR_NULL_POINT;
    }
    pin2 = HdfSbufReadString(data);
    if (pin2 == nullptr) {
        TELEPHONY_LOGE("miss pin2 parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_UNLOCK_PUK2);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create UnlockSimPin2 HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->UnlockPuk2(requestInfo, puk2, pin2);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::GetSimPin2InputTimes(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->GetSimPin2InputTimes == nullptr)) {
        TELEPHONY_LOGE("GetSimPin2InputTimes::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimPin2InputTimes HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->GetSimPin2InputTimes(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SetActiveSim(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->SetActiveSim == nullptr)) {
        TELEPHONY_LOGE("SetActiveSim::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    int32_t index = 0;
    int32_t enable = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &index)) {
        TELEPHONY_LOGE("miss index parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &enable)) {
        TELEPHONY_LOGE("miss enable parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_SET_ACTIVE_SIM);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create EnterSimPin HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->SetActiveSim(requestInfo, index, enable);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SimStkSendTerminalResponse(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->SimStkSendTerminalResponse == nullptr)) {
        TELEPHONY_LOGE("SimStkSendTerminalResponse::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    const char *strCmd = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    strCmd = parcel->ReadCString();
    if (strCmd == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_STK_SEND_TERMINAL_RESPONSE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SimStkSendTerminalResponse HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->SimStkSendTerminalResponse(requestInfo, strCmd);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SimStkSendEnvelope(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->SimStkSendEnvelope == nullptr)) {
        TELEPHONY_LOGE("SimStkSendEnvelope::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    const char *strCmd = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    strCmd = parcel->ReadCString();
    if (strCmd == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_STK_SEND_ENVELOPE);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SimStkSendEnvelope HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->SimStkSendEnvelope(requestInfo, strCmd);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SimStkIsReady(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->SimStkIsReady == nullptr)) {
        TELEPHONY_LOGE("SimStkIsReady::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_STK_IS_READY);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SimStkIsReady HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->SimStkIsReady(requestInfo);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SetRadioProtocol(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->SetRadioProtocol == nullptr)) {
        TELEPHONY_LOGE("SetRadioProtocol::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    SimProtocolRequest protocol = SimProtocolRequest();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in GetSimIO is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!protocol.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t serial = protocol.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_RADIO_PROTOCOL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetRadioProtocol HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    HRilSimProtocolRequest hRilSimProtocolRequest;
    hRilSimProtocolRequest.phase = protocol.phase;
    hRilSimProtocolRequest.protocol = protocol.protocol;
    hRilSimProtocolRequest.slotId = protocol.slotId;
    simFuncs_->SetRadioProtocol(requestInfo, &hRilSimProtocolRequest, sizeof(hRilSimProtocolRequest));
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SimOpenLogicalChannel(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->SimOpenLogicalChannel == nullptr)) {
        TELEPHONY_LOGE("SimOpenLogicalChannel::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    const char *appID = nullptr;
    int32_t p2 = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    appID = HdfSbufReadString(data);
    if (appID == nullptr) {
        TELEPHONY_LOGE("miss appID parameter");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufReadInt32(data, &p2)) {
        TELEPHONY_LOGE("miss p2 parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_OPEN_LOGICAL_CHANNEL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SimOpenLogicalChannel HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }

    simFuncs_->SimOpenLogicalChannel(requestInfo, appID, p2);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SimCloseLogicalChannel(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->SimCloseLogicalChannel == nullptr)) {
        TELEPHONY_LOGE("SimCloseLogicalChannel::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    int32_t channelId = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &channelId)) {
        TELEPHONY_LOGE("miss channelId parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_CLOSE_LOGICAL_CHANNEL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SimCloseLogicalChannel HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }

    simFuncs_->SimCloseLogicalChannel(requestInfo, channelId);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SimTransmitApduLogicalChannel(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->SimTransmitApduLogicalChannel == nullptr)) {
        TELEPHONY_LOGE("SimTransmitApduLogicalChannel::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    ApduSimIORequestInfo apduSimIO = ApduSimIORequestInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in SimTransmitApduLogicalChannel is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!apduSimIO.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HRilApduSimIO rilApduSimIO = {};
    rilApduSimIO.channelId = apduSimIO.channelId;
    rilApduSimIO.type = apduSimIO.type;
    rilApduSimIO.instruction = apduSimIO.instruction;
    rilApduSimIO.p1 = apduSimIO.p1;
    rilApduSimIO.p2 = apduSimIO.p2;
    rilApduSimIO.p3 = apduSimIO.p3;
    rilApduSimIO.data = static_cast<char *>(const_cast<char *>(apduSimIO.data.c_str()));

    int32_t serial = apduSimIO.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SimTransmitApduLogicalChannel HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->SimTransmitApduLogicalChannel(requestInfo, &rilApduSimIO, sizeof(rilApduSimIO));
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::UnlockSimLock(struct HdfSBuf *data)
{
    if ((simFuncs_ == nullptr) || (simFuncs_->UnlockSimLock == nullptr)) {
        TELEPHONY_LOGE("UnlockSimLock::simFuncs_:%{public}p", simFuncs_);
        return HRIL_ERR_NULL_POINT;
    }
    int32_t serial = 0;
    int32_t lockType = 0;
    const char *key = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (!HdfSbufReadInt32(data, &lockType)) {
        TELEPHONY_LOGE("miss lockType parameter");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    key = HdfSbufReadString(data);
    if (key == nullptr) {
        TELEPHONY_LOGE("miss key parameter");
        return HRIL_ERR_NULL_POINT;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, HREQ_SIM_UNLOCK_SIM_LOCK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create UnlockSimLock HRilRequest!");
        return HRIL_ERR_NULL_POINT;
    }
    simFuncs_->UnlockSimLock(requestInfo, lockType, key);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::GetSimIOResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) ||
        (response != nullptr && responseLen != sizeof(HRilSimIOResponse))) {
        TELEPHONY_LOGE("Invalid response: Vendor exception!");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if (response == nullptr && responseLen == 0) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    }
    IccIoResultInfo result = ProcessIccIoResponse(responseInfo, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSim::GetSimStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    CardStatusInfo rilCardStatus = {};
    if ((response == nullptr && responseLen != 0) ||
        (response != nullptr && responseLen != sizeof(HRilCardState))) {
        TELEPHONY_LOGE("Invalid response: Vendor exception!");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if (response == nullptr && responseLen == 0) {
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

int32_t HRilSim::GetImsiResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if (response == nullptr) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    }
    struct HdfSBuf *dataSbuf = HdfSbufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("Error : dataSbuf in GetImsiResponse is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in GetImsiResponse is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetImsiResponse is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in GetImsiResponse is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSbufRecycle(dataSbuf);

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::GetSimLockStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    int32_t simLockStatus = 0;
    if (response == nullptr || responseLen != sizeof(int32_t)) {
        TELEPHONY_LOGE("GetSimStatusResponse: Invalid response");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        simLockStatus = *(static_cast<const int32_t *>(response));
    }
    struct HdfSBuf *dataSbuf = HdfSbufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("GetSimLockStatusResponse dataSbuf is NULL.");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, simLockStatus)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in GetSimLockStatusResponse is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetSimLockStatusResponse is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in GetSimLockStatusResponse is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSbufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SetSimLockResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::ChangeSimPasswordResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::UnlockPinResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::UnlockPukResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::GetSimPinInputTimesResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    SimPinInputTimes pinInputTimesResult = {};
    if ((response == nullptr && responseLen != 0) ||
        (response != nullptr && responseLen != sizeof(HRilPinInputTimes))) {
        TELEPHONY_LOGE("Invalid response: Vendor exception!");
        return HRIL_ERR_GENERIC_FAILURE;
    } else if (response == nullptr && responseLen == 0) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilPinInputTimes *pPinInputTimes = static_cast<const HRilPinInputTimes *>(response);
        pinInputTimesResult.code = pPinInputTimes->code;
        pinInputTimesResult.times = pPinInputTimes->times;
        pinInputTimesResult.pukTimes = pPinInputTimes->pukTimes;
        pinInputTimesResult.pinTimes = pPinInputTimes->pinTimes;
        pinInputTimesResult.puk2Times = pPinInputTimes->puk2Times;
        pinInputTimesResult.pin2Times = pPinInputTimes->pin2Times;
    }

    return ResponseMessageParcel(responseInfo, pinInputTimesResult, requestNum);
}

int32_t HRilSim::UnlockPin2Response(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::UnlockPuk2Response(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::GetSimPin2InputTimesResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    SimPinInputTimes pin2InputTimesResult = {};
    if (response == nullptr) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilPinInputTimes *pPin2InputTimes = static_cast<const HRilPinInputTimes *>(response);
        pin2InputTimesResult.code = pPin2InputTimes->code;
        pin2InputTimesResult.times = pPin2InputTimes->times;
        pin2InputTimesResult.pukTimes = pPin2InputTimes->pukTimes;
        pin2InputTimesResult.pinTimes = pPin2InputTimes->pinTimes;
        pin2InputTimesResult.puk2Times = pPin2InputTimes->puk2Times;
        pin2InputTimesResult.pin2Times = pPin2InputTimes->pin2Times;
    }

    return ResponseMessageParcel(responseInfo, pin2InputTimesResult, requestNum);
}

int32_t HRilSim::SetActiveSimResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::SimStkSendTerminalResponseResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::SimStkSendEnvelopeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::SimStkIsReadyResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::SetRadioProtocolResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    SimProtocolResponse pSimProtocol = {};
    if (response == nullptr) {
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilSimProtocolResponse *pSimProtocolResponse = static_cast<const HRilSimProtocolResponse *>(response);
        pSimProtocol.phase = pSimProtocolResponse->phase;
        pSimProtocol.result = pSimProtocolResponse->result;
        pSimProtocol.slotId = pSimProtocolResponse->slotId;
    }

    return ResponseMessageParcel(responseInfo, pSimProtocol, requestNum);
}

int32_t HRilSim::SimOpenLogicalChannelResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::SimCloseLogicalChannelResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::SimTransmitApduLogicalChannelResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    IccIoResultInfo result = ProcessIccIoResponse(responseInfo, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSim::UnlockSimLockResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    LockStatusResp lockStatus = {};

    if (response == nullptr || responseLen != sizeof(HRilLockStatus)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilLockStatus *resp = static_cast<const HRilLockStatus *>(response);
        lockStatus.result = resp->result;
        lockStatus.remain = resp->remain;
    }
    return ResponseMessageParcel(responseInfo, lockStatus, requestNum);
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
            result.response = "";
        } else {
            result.response = std::string(resp->response);
        }
    }
    return result;
}

int32_t HRilSim::SimStateUpdated(int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSbufTypedObtain(SBUF_IPC);
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SIM_STATUS_CHANGED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in SimStateUpdated is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SimStkSessionEndNotify(
    int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSbufTypedObtain(SBUF_IPC);
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SIM_STK_SESSION_END_NOTIFY, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in SimStkSessionEndNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SimStkProactiveNotify(
    int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSbufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("Error : dataSbuf in SimStkProactiveNotify is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in SimStkProactiveNotify is failed!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SIM_STK_PROACTIVE_NOTIFY, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in SimStkProactiveNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSbufRecycle(dataSbuf);

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SimStkAlphaNotify(int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSbufTypedObtain(SBUF_IPC);
    if (!HdfSbufWriteInt32(dataSbuf, GetSlotId())) {
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SIM_STK_ALPHA_NOTIFY, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in SimStkAlphaNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSbufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSbufRecycle(dataSbuf);
    }

    return HRIL_ERR_SUCCESS;
}

bool HRilSim::IsSimResponse(uint32_t code)
{
    return ((code >= HREQ_SIM_BASE) && (code < HREQ_DATA_BASE));
}

bool HRilSim::IsSimNotification(uint32_t code)
{
    return ((code >= HNOTI_SIM_BASE) && (code < HNOTI_DATA_BASE));
}
} // namespace Telephony
} // namespace OHOS
