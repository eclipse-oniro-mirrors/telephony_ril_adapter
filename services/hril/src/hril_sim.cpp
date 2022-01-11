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
HRilSim::HRilSim(IHRilReporter &hrilReporter)
    : HRilBase(hrilReporter)
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

void HRilSim::ProcessSimNotify(int32_t slotId, const struct ReportInfo *reportInfo,
    const void *response, size_t responseLen)
{
    int code;
    HRilErrNumber e;
    code = reportInfo->notifyId;
    e = (HRilErrNumber)reportInfo->error;
    auto itFunc = notiMemberFuncMap_.find(code);
    if (itFunc != notiMemberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, (int32_t)reportInfo->type, e, response, responseLen);
        }
    }
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
    notiMemberFuncMap_[HNOTI_SIM_STK_SESSION_END_NOTIFY] = &HRilSim::StkSessionEndNotify;
    notiMemberFuncMap_[HNOTI_SIM_STK_PROACTIVE_CMD_NOTIFY] = &HRilSim::StkProactiveCommandNotify;
    notiMemberFuncMap_[HNOTI_SIM_STK_ALPHA_NOTIFY] = &HRilSim::StkAlphaNotify;

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
    respMemberFuncMap_[HREQ_SIM_SEND_TERMINAL_RESPONSE_CMD] = &HRilSim::SendTerminalResponseCmdResponse;
    respMemberFuncMap_[HREQ_SIM_SEND_ENVELOPE_CMD] = &HRilSim::SendEnvelopeCmdResponse;
    respMemberFuncMap_[HREQ_SIM_STK_CONTROLLER_IS_READY] = &HRilSim::StkControllerIsReadyResponse;
    respMemberFuncMap_[HREQ_SIM_STK_CMD_CALL_SETUP] = &HRilSim::StkCmdCallSetupResponse;
    respMemberFuncMap_[HREQ_SIM_RADIO_PROTOCOL] = &HRilSim::SetRadioProtocolResponse;
    respMemberFuncMap_[HREQ_SIM_OPEN_LOGICAL_SIM_IO] = &HRilSim::OpenLogicalSimIOResponse;
    respMemberFuncMap_[HREQ_SIM_CLOSE_LOGICAL_SIM_IO] = &HRilSim::CloseLogicalSimIOResponse;
    respMemberFuncMap_[HREQ_SIM_TRANSMIT_APDU_SIM_IO] = &HRilSim::TransmitApduSimIOResponse;
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
    reqMemberFuncMap_[HREQ_SIM_SEND_TERMINAL_RESPONSE_CMD] = &HRilSim::SendTerminalResponseCmd;
    reqMemberFuncMap_[HREQ_SIM_SEND_ENVELOPE_CMD] = &HRilSim::SendEnvelopeCmd;
    reqMemberFuncMap_[HREQ_SIM_STK_CONTROLLER_IS_READY] = &HRilSim::StkControllerIsReady;
    reqMemberFuncMap_[HREQ_SIM_STK_CMD_CALL_SETUP] = &HRilSim::StkCmdCallSetup;
    reqMemberFuncMap_[HREQ_SIM_RADIO_PROTOCOL] = &HRilSim::SetRadioProtocol;
    reqMemberFuncMap_[HREQ_SIM_OPEN_LOGICAL_SIM_IO] = &HRilSim::OpenLogicalSimIO;
    reqMemberFuncMap_[HREQ_SIM_CLOSE_LOGICAL_SIM_IO] = &HRilSim::CloseLogicalSimIO;
    reqMemberFuncMap_[HREQ_SIM_TRANSMIT_APDU_SIM_IO] = &HRilSim::TransmitApduSimIO;
    reqMemberFuncMap_[HREQ_SIM_UNLOCK_SIM_LOCK] = &HRilSim::UnlockSimLock;
}

void HRilSim::GetSimIO(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
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
    int32_t serial = SimIO.serial;
    HRilSimIO rilSimIO = {};
    rilSimIO.command = SimIO.command;
    rilSimIO.fileid = SimIO.fileId;
    rilSimIO.p1 = SimIO.p1;
    rilSimIO.p2 = SimIO.p2;
    rilSimIO.p3 = SimIO.p3;
    rilSimIO.data = static_cast<char*>(const_cast<char*>(SimIO.data.c_str()));
    rilSimIO.pathid = static_cast<char*>(const_cast<char*>(SimIO.path.c_str()));
    rilSimIO.pin2 = static_cast<char*>(const_cast<char*>(SimIO.pin2.c_str()));
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_SIM_IO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimIO HRilRequest!");
        return;
    }
    simFuncs_->GetSimIO(requestInfo, &rilSimIO, sizeof(rilSimIO));
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
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimStatus HRilRequest!");
        return;
    }
    simFuncs_->GetSimStatus(requestInfo);
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
        TELEPHONY_LOGE("RilAdapter failed to do Create GetImsi HRilRequest!");
        return;
    }
    simFuncs_->GetSimImsi(requestInfo);
}

void HRilSim::GetSimLockStatus(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
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
    HRilSimClock rilSimClock = {};
    rilSimClock.mode = simClock.mode;
    rilSimClock.status = simClock.status;
    rilSimClock.classx = simClock.classx;
    rilSimClock.fac = static_cast<char*>(const_cast<char*>(simClock.fac.c_str()));
    rilSimClock.passwd = static_cast<char*>(const_cast<char*>(simClock.passwd.c_str()));
    int32_t serial = simClock.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_GET_SIM_LOCK_STATUS);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimLockStatus HRilRequest!");
        return;
    }
    simFuncs_->GetSimLockStatus(requestInfo, &rilSimClock, sizeof(rilSimClock));
}

void HRilSim::SetSimLock(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
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
    HRilSimClock rilSimClock = {};
    rilSimClock.fac = static_cast<char*>(const_cast<char*>(simClock.fac.c_str()));
    rilSimClock.mode = simClock.mode;
    rilSimClock.status = simClock.status;
    rilSimClock.passwd = static_cast<char*>(const_cast<char*>(simClock.passwd.c_str()));
    rilSimClock.classx = simClock.classx;
    int32_t serial = simClock.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_SET_SIM_LOCK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SetSimLock HRilRequest!");
        return;
    }
    simFuncs_->SetSimLock(requestInfo, &rilSimClock, sizeof(rilSimClock));
}

void HRilSim::ChangeSimPassword(int32_t slotId, struct HdfSBuf *data)
{
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
    HRilSimPassword rilSimPassword = {};
    rilSimPassword.fac = static_cast<char*>(const_cast<char*>(simPassword.fac.c_str()));
    rilSimPassword.oldPassword = static_cast<char*>(const_cast<char*>(simPassword.oldPassword.c_str()));
    rilSimPassword.newPassword = static_cast<char*>(const_cast<char*>(simPassword.newPassword.c_str()));
    rilSimPassword.passwordLength = simPassword.passwordLength;
    int32_t serial = simPassword.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_CHANGE_SIM_PASSWORD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create ChangeSimPassword HRilRequest!");
        return;
    }
    simFuncs_->ChangeSimPassword(requestInfo, &rilSimPassword, sizeof(rilSimPassword));
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
        TELEPHONY_LOGE("RilAdapter failed to do Create EnterSimPin HRilRequest!");
        return;
    }
    simFuncs_->UnlockPin(requestInfo, pin);
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
        TELEPHONY_LOGE("RilAdapter failed to do Create UnlockSimPin HRilRequest!");
        return;
    }
    simFuncs_->UnlockPuk(requestInfo, puk, pin);
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
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimPinInputTimes HRilRequest!");
        return;
    }
    simFuncs_->GetSimPinInputTimes(requestInfo);
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
        TELEPHONY_LOGE("RilAdapter failed to do Create EnterSimPin HRilRequest!");
        return;
    }
    simFuncs_->UnlockPin2(requestInfo, pin2);
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
        TELEPHONY_LOGE("RilAdapter failed to do Create UnlockSimPin2 HRilRequest!");
        return;
    }
    simFuncs_->UnlockPuk2(requestInfo, puk2, pin2);
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
        TELEPHONY_LOGE("RilAdapter failed to do Create GetSimPin2InputTimes HRilRequest!");
        return;
    }
    simFuncs_->GetSimPin2InputTimes(requestInfo);
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
        TELEPHONY_LOGE("RilAdapter failed to do Create EnterSimPin HRilRequest!");
        return;
    }
    simFuncs_->SetActiveSim(requestInfo, index, enable);
}

void HRilSim::SendTerminalResponseCmd(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    const char *strCmd = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    strCmd = parcel->ReadCString();
    if (strCmd == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_SEND_TERMINAL_RESPONSE_CMD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SendTerminalResponseCmd HRilRequest!");
        return;
    }
    simFuncs_->SendTerminalResponseCmd(requestInfo, strCmd);
}

void HRilSim::SendEnvelopeCmd(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    const char *strCmd = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    strCmd = parcel->ReadCString();
    if (strCmd == nullptr) {
        TELEPHONY_LOGE("miss pin parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_SEND_ENVELOPE_CMD);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create SendEnvelopeCmd HRilRequest!");
        return;
    }
    simFuncs_->SendEnvelopeCmd(requestInfo, strCmd);
}

void HRilSim::StkControllerIsReady(int32_t slotId, struct HdfSBuf *data)
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

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_STK_CONTROLLER_IS_READY);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create StkControllerIsReady HRilRequest!");
        return;
    }
    simFuncs_->StkControllerIsReady(requestInfo);
}

void HRilSim::StkCmdCallSetup(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int serial = 0;
    int flagAccept = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &flagAccept)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_STK_CMD_CALL_SETUP);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create StkCmdCallSetup HRilRequest!");
        return;
    }
    simFuncs_->StkCmdCallSetup(requestInfo, flagAccept);
}

void HRilSim::SetRadioProtocol(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    SimProtocolRequest protocol = SimProtocolRequest();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in GetSimIO is nullptr!");
        return;
    }
    if (!protocol.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    int32_t serial = protocol.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_RADIO_PROTOCOL);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create StkCmdCallSetup HRilRequest!");
        return;
    }
    HRilSimProtocolRequest hRilSimProtocolRequest;
    hRilSimProtocolRequest.phase = protocol.phase;
    hRilSimProtocolRequest.protocol = protocol.protocol;
    hRilSimProtocolRequest.slotId = protocol.slotId;
    simFuncs_->SetRadioProtocol(requestInfo, &hRilSimProtocolRequest, sizeof(hRilSimProtocolRequest));
}

void HRilSim::OpenLogicalSimIO(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    const char *appID = nullptr;
    int32_t p2 = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    appID = HdfSbufReadString(data);
    if (appID == nullptr) {
        TELEPHONY_LOGE("miss appID parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &p2)) {
        TELEPHONY_LOGE("miss p2 parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_OPEN_LOGICAL_SIM_IO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create OpenLogicalSimIO HRilRequest!");
        return;
    }

    simFuncs_->OpenLogicalSimIO(requestInfo, appID, p2);
}

void HRilSim::CloseLogicalSimIO(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    int32_t chanID = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &chanID)) {
        TELEPHONY_LOGE("miss chanID parameter");
        return;
    }

    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_CLOSE_LOGICAL_SIM_IO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create CloseLogicalSimIO HRilRequest!");
        return;
    }

    simFuncs_->CloseLogicalSimIO(requestInfo, chanID);
}

void HRilSim::TransmitApduSimIO(int32_t slotId, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    ApduSimIORequestInfo apduSimIO = ApduSimIORequestInfo();
    MessageParcel *parcel = nullptr;
    if (SbufToParcel(data, &parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do SbufToParcel");
        return;
    }
    if (parcel == nullptr) {
        TELEPHONY_LOGE("parcel in TransmitApduSimIO is nullptr!");
        return;
    }
    if (!apduSimIO.ReadFromParcel(*parcel)) {
        TELEPHONY_LOGE("RilAdapter failed to do ReadFromParcel!");
        return;
    }
    HRilApduSimIO rilApduSimIO = {};
    rilApduSimIO.chanId = apduSimIO.chanId;
    rilApduSimIO.type = apduSimIO.type;
    rilApduSimIO.instruction = apduSimIO.instruction;
    rilApduSimIO.p1 = apduSimIO.p1;
    rilApduSimIO.p2 = apduSimIO.p2;
    rilApduSimIO.p3 = apduSimIO.p3;
    rilApduSimIO.data = static_cast<char*>(const_cast<char*>(apduSimIO.data.c_str()));

    int32_t serial = apduSimIO.serial;
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_SIM_TRANSMIT_APDU_SIM_IO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create TransmitApduSimIO HRilRequest!");
        return;
    }
    simFuncs_->TransmitApduSimIO(requestInfo, &rilApduSimIO, sizeof(rilApduSimIO));
}

void HRilSim::UnlockSimLock(int32_t slotld, struct HdfSBuf *data)
{
    if (simFuncs_ == nullptr) {
        TELEPHONY_LOGE("simFuncs_ is nullptr!");
        return;
    }
    int32_t serial = 0;
    int32_t lockType = 0;
    const char *passward = nullptr;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("miss serial parameter");
        return;
    }
    if (!HdfSbufReadInt32(data, &lockType)) {
        TELEPHONY_LOGE("miss lockType parameter");
        return;
    }
    passward = HdfSbufReadString(data);
    if (passward == nullptr) {
        TELEPHONY_LOGE("miss pw parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotld, HREQ_SIM_UNLOCK_SIM_LOCK);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter failed to do Create UnlockSimLock HRilRequest!");
        return;
    }
    simFuncs_->UnlockSimLock(requestInfo, lockType, passward);
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
        const HRilCardState *curPtr = static_cast<const HRilCardState*>(response);
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
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in GetImsiResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetImsiResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in GetImsiResponse is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::GetSimLockStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
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
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("GetSimLockStatusResponse dataSbuf is NULL.");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, simLockStatus)) {
        TELEPHONY_LOGE("HdfSbufWriteInt32 in GetSimLockStatusResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetSimLockStatusResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in GetSimLockStatusResponse is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::SetSimLockResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
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
        return HRIL_ERR_NULL_POINT;
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
        return HRIL_ERR_NULL_POINT;
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

int32_t HRilSim::SendTerminalResponseCmdResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::SendEnvelopeCmdResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::StkControllerIsReadyResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::StkCmdCallSetupResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::SetRadioProtocolResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    const HRilSimProtocolResponse *pSimProtocolResponse = static_cast<const HRilSimProtocolResponse *>(response);
    if (pSimProtocolResponse == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HRIL_ERR_NULL_POINT;
    }

    SimProtocolResponse pSimProtocol;
    pSimProtocol.phase = pSimProtocolResponse->phase;
    pSimProtocol.result = pSimProtocolResponse->result;
    pSimProtocol.slotId = pSimProtocolResponse->slotId;
    return ResponseMessageParcel(responseInfo, pSimProtocol, requestNum);
}

int32_t HRilSim::OpenLogicalSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::CloseLogicalSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    return ResponseRequestInfo(requestNum, &responseInfo, sizeof(responseInfo));
}

int32_t HRilSim::TransmitApduSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    IccIoResultInfo result = ProcessIccIoResponse(responseInfo, response, responseLen);
    return ResponseMessageParcel(responseInfo, result, requestNum);
}

int32_t HRilSim::UnlockSimLockResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
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

int32_t HRilSim::SimStateUpdated(
    int32_t slotId, int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SIM_STATUS_CHANGED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in SimStateUpdated is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::StkSessionEndNotify(
    int32_t slotId, int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SIM_STK_SESSION_END_NOTIFY, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in StkSessionEndNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
    }

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::StkProactiveCommandNotify(
    int32_t slotId, int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("Error : dataSbuf in StkProactiveCommandNotify is nullptr!");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteString(dataSbuf, (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in StkProactiveCommandNotify is failed!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SIM_STK_PROACTIVE_CMD_NOTIFY, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in StkProactiveCommandNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);

    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::StkAlphaNotify(
    int32_t slotId, int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    int32_t ret = ServiceNotifyDispatcher(HNOTI_SIM_STK_ALPHA_NOTIFY, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        TELEPHONY_LOGE("ret in StkAlphaNotify is not equal to HRIL_ERR_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (dataSbuf != nullptr) {
        HdfSBufRecycle(dataSbuf);
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