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
    AddNotificationHandlerToMap();
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
    // response
    respMemberFuncMap_[HREQ_SIM_GET_SIM_IO] = &HRilSim::GetSimIOResponse;
    respMemberFuncMap_[HREQ_SIM_GET_SIM_STATUS] = &HRilSim::GetSimStatusResponse;
    respMemberFuncMap_[HREQ_SIM_GET_IMSI] = &HRilSim::GetImsiResponse;
    respMemberFuncMap_[HREQ_SIM_GET_SIM_LOCK_STATUS] = &HRilSim::GetSimLockStatusResponse;
    respMemberFuncMap_[HREQ_SIM_SET_SIM_LOCK] = &HRilSim::SetSimLockResponse;
    respMemberFuncMap_[HREQ_SIM_CHANGE_SIM_PASSWORD] = &HRilSim::ChangeSimPasswordResponse;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_PIN] = &HRilSim::UnlockPinResponse;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_PUK] = &HRilSim::UnlockPukResponse;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_PIN2] = &HRilSim::UnlockPin2Response;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_PUK2] = &HRilSim::UnlockPuk2Response;
    respMemberFuncMap_[HREQ_SIM_SET_ACTIVE_SIM] = &HRilSim::SetActiveSimResponse;
    respMemberFuncMap_[HREQ_SIM_STK_SEND_TERMINAL_RESPONSE] = &HRilSim::SimStkSendTerminalResponseResponse;
    respMemberFuncMap_[HREQ_SIM_STK_SEND_ENVELOPE] = &HRilSim::SimStkSendEnvelopeResponse;
    respMemberFuncMap_[HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT] =
        &HRilSim::SimStkSendCallSetupRequestResultResponse;
    respMemberFuncMap_[HREQ_SIM_STK_IS_READY] = &HRilSim::SimStkIsReadyResponse;
    respMemberFuncMap_[HREQ_SIM_GET_RADIO_PROTOCOL] = &HRilSim::GetRadioProtocolResponse;
    respMemberFuncMap_[HREQ_SIM_SET_RADIO_PROTOCOL] = &HRilSim::SetRadioProtocolResponse;
    respMemberFuncMap_[HREQ_SIM_OPEN_LOGICAL_CHANNEL] = &HRilSim::SimOpenLogicalChannelResponse;
    respMemberFuncMap_[HREQ_SIM_CLOSE_LOGICAL_CHANNEL] = &HRilSim::SimCloseLogicalChannelResponse;
    respMemberFuncMap_[HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL] = &HRilSim::SimTransmitApduLogicalChannelResponse;
    respMemberFuncMap_[HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL] = &HRilSim::SimTransmitApduBasicChannelResponse;
    respMemberFuncMap_[HREQ_SIM_AUTHENTICATION] = &HRilSim::SimAuthenticationResponse;
    respMemberFuncMap_[HREQ_SIM_UNLOCK_SIM_LOCK] = &HRilSim::UnlockSimLockResponse;
}

void HRilSim::AddNotificationHandlerToMap()
{
    // Notification
    notiMemberFuncMap_[HNOTI_SIM_STATUS_CHANGED] = &HRilSim::SimStateUpdated;
    notiMemberFuncMap_[HNOTI_SIM_STK_SESSION_END_NOTIFY] = &HRilSim::SimStkSessionEndNotify;
    notiMemberFuncMap_[HNOTI_SIM_STK_PROACTIVE_NOTIFY] = &HRilSim::SimStkProactiveNotify;
    notiMemberFuncMap_[HNOTI_SIM_STK_ALPHA_NOTIFY] = &HRilSim::SimStkAlphaNotify;
    notiMemberFuncMap_[HNOTI_SIM_STK_EVENT_NOTIFY] = &HRilSim::SimStkEventNotify;
    notiMemberFuncMap_[HNOTI_SIM_STK_CALL_SETUP_NOTIFY] = &HRilSim::SimStkCallSetupNotify;
    notiMemberFuncMap_[HNOTI_SIM_REFRESH_NOTIFY] = &HRilSim::SimRefreshNotify;
    notiMemberFuncMap_[HNOTI_SIM_RADIO_PROTOCOL_UPDATED] = &HRilSim::SimRadioProtocolUpdated;
}

int32_t HRilSim::GetSimIO(int32_t serialId, const OHOS::HDI::Ril::V1_0::SimIoRequestInfo &simIO)
{
    std::unique_ptr<HRilSimIO> rilSimIO = std::make_unique<HRilSimIO>();
    CopyToHRilSimIO(rilSimIO, simIO);
    int32_t ret = RequestVendor(
        serialId, HREQ_SIM_GET_SIM_IO, simFuncs_, &HRilSimReq::GetSimIO, rilSimIO.get(), sizeof(*rilSimIO));
    SafeFrees(rilSimIO->data, rilSimIO->pathid, rilSimIO->pin2);
    return ret;
}

int32_t HRilSim::GetSimStatus(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_SIM_GET_SIM_STATUS, simFuncs_, &HRilSimReq::GetSimStatus);
}

int32_t HRilSim::GetImsi(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_SIM_GET_IMSI, simFuncs_, &HRilSimReq::GetSimImsi);
}

int32_t HRilSim::GetSimLockStatus(int32_t serialId, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo)
{
    std::unique_ptr<HRilSimClock> rilSimLock = std::make_unique<HRilSimClock>();
    CopyToHRilSimLock(rilSimLock, simLockInfo);
    int32_t ret = RequestVendor(serialId, HREQ_SIM_GET_SIM_LOCK_STATUS, simFuncs_, &HRilSimReq::GetSimLockStatus,
        rilSimLock.get(), sizeof(*rilSimLock));
    SafeFrees(rilSimLock->fac, rilSimLock->passwd);
    return ret;
}

int32_t HRilSim::SetSimLock(int32_t serialId, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo)
{
    std::unique_ptr<HRilSimClock> rilSimLock = std::make_unique<HRilSimClock>();
    CopyToHRilSimLock(rilSimLock, simLockInfo);
    int32_t ret = RequestVendor(
        serialId, HREQ_SIM_SET_SIM_LOCK, simFuncs_, &HRilSimReq::SetSimLock, rilSimLock.get(), sizeof(*rilSimLock));
    SafeFrees(rilSimLock->fac, rilSimLock->passwd);
    return ret;
}

int32_t HRilSim::ChangeSimPassword(int32_t serialId, const OHOS::HDI::Ril::V1_0::SimPasswordInfo &simPassword)
{
    std::unique_ptr<HRilSimPassword> rilSimPassword = std::make_unique<HRilSimPassword>();
    CopyToHRilSimPassword(rilSimPassword, simPassword);
    int32_t ret = RequestVendor(serialId, HREQ_SIM_CHANGE_SIM_PASSWORD, simFuncs_, &HRilSimReq::ChangeSimPassword,
        rilSimPassword.get(), sizeof(*rilSimPassword));
    SafeFrees(rilSimPassword->fac, rilSimPassword->oldPassword, rilSimPassword->newPassword);
    return ret;
}

int32_t HRilSim::UnlockPin(int32_t serialId, const std::string &pin)
{
    char *pinPoint = nullptr;
    CopyToCharPoint(&pinPoint, pin);
    int32_t ret = RequestVendor(serialId, HREQ_SIM_UNLOCK_PIN, simFuncs_, &HRilSimReq::UnlockPin, pinPoint);
    SafeFrees(pinPoint);
    return ret;
}

int32_t HRilSim::UnlockPuk(int32_t serialId, const std::string &puk, const std::string &pin)
{
    char *pukPoint = nullptr;
    CopyToCharPoint(&pukPoint, puk);
    char *pinPoint = nullptr;
    CopyToCharPoint(&pinPoint, pin);
    int32_t ret = RequestVendor(serialId, HREQ_SIM_UNLOCK_PUK, simFuncs_, &HRilSimReq::UnlockPuk, pukPoint, pinPoint);
    SafeFrees(pukPoint, pinPoint);
    return ret;
}

int32_t HRilSim::UnlockPin2(int32_t serialId, const std::string &pin2)
{
    char *pin2Point = nullptr;
    CopyToCharPoint(&pin2Point, pin2);
    int32_t ret = RequestVendor(serialId, HREQ_SIM_UNLOCK_PIN2, simFuncs_, &HRilSimReq::UnlockPin2, pin2Point);
    SafeFrees(pin2Point);
    return ret;
}

int32_t HRilSim::UnlockPuk2(int32_t serialId, const std::string &puk2, const std::string &pin2)
{
    char *puk2Point = nullptr;
    CopyToCharPoint(&puk2Point, puk2);
    char *pin2Point = nullptr;
    CopyToCharPoint(&pin2Point, pin2);
    int32_t ret =
        RequestVendor(serialId, HREQ_SIM_UNLOCK_PUK2, simFuncs_, &HRilSimReq::UnlockPuk2, puk2Point, pin2Point);
    SafeFrees(puk2Point, pin2Point);
    return ret;
}

int32_t HRilSim::SetActiveSim(int32_t serialId, int32_t index, int32_t enable)
{
    return RequestVendor(serialId, HREQ_SIM_SET_ACTIVE_SIM, simFuncs_, &HRilSimReq::SetActiveSim, index, enable);
}

int32_t HRilSim::SimStkSendTerminalResponse(int32_t serialId, const std::string &strCmd)
{
    char *strCmdPoint = nullptr;
    CopyToCharPoint(&strCmdPoint, strCmd);
    int32_t ret = RequestVendor(
        serialId, HREQ_SIM_STK_SEND_TERMINAL_RESPONSE, simFuncs_, &HRilSimReq::SimStkSendTerminalResponse, strCmdPoint);
    SafeFrees(strCmdPoint);
    return ret;
}

int32_t HRilSim::SimStkSendEnvelope(int32_t serialId, const std::string &strCmd)
{
    char *strCmdPoint = nullptr;
    CopyToCharPoint(&strCmdPoint, strCmd);
    int32_t ret =
        RequestVendor(serialId, HREQ_SIM_STK_SEND_ENVELOPE, simFuncs_, &HRilSimReq::SimStkSendEnvelope, strCmdPoint);
    SafeFrees(strCmdPoint);
    return ret;
}

int32_t HRilSim::SimStkSendCallSetupRequestResult(int32_t serialId, int32_t accept)
{
    return RequestVendor(serialId, HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT, simFuncs_,
        &HRilSimReq::SimStkSendCallSetupRequestResult, accept);
}

int32_t HRilSim::SimStkIsReady(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_SIM_STK_IS_READY, simFuncs_, &HRilSimReq::SimStkIsReady);
}

int32_t HRilSim::GetRadioProtocol(int32_t serialId)
{
    TELEPHONY_LOGI("HRilSim::GetRadioProtocol slotId:%{public}d", GetSlotId());
    return RequestVendor(serialId, HREQ_SIM_GET_RADIO_PROTOCOL, simFuncs_, &HRilSimReq::GetRadioProtocol);
}

int32_t HRilSim::SetRadioProtocol(int32_t serialId, const HDI::Ril::V1_0::RadioProtocol &radioProtocol)
{
    HRilRadioProtocol hrilRadioProtocol = {};
    hrilRadioProtocol.sessionId = radioProtocol.sessionId;
    hrilRadioProtocol.phase = static_cast<HRilRadioProtocolPhase>(radioProtocol.phase);
    hrilRadioProtocol.technology = radioProtocol.technology;
    hrilRadioProtocol.modemId = radioProtocol.modemId;
    hrilRadioProtocol.status = static_cast<HRilRadioProtocolStatus>(radioProtocol.status);
    TELEPHONY_LOGI("HRilSim::SetRadioProtocol slotId:%{public}d, serialId:%{public}d"
        "sessionId:%{public}d, phase:%{public}d, technology:%{public}d, modemId:%{public}d, status:%{public}d",
        GetSlotId(), serialId, hrilRadioProtocol.sessionId, hrilRadioProtocol.phase, hrilRadioProtocol.technology,
        hrilRadioProtocol.modemId, hrilRadioProtocol.status);
    return RequestVendor(serialId, HREQ_SIM_SET_RADIO_PROTOCOL, simFuncs_,
        &HRilSimReq::SetRadioProtocol, &hrilRadioProtocol);
}

int32_t HRilSim::SimOpenLogicalChannel(int32_t serialId, const std::string &appID, int32_t p2)
{
    char *appIDPoint = nullptr;
    CopyToCharPoint(&appIDPoint, appID);
    int32_t ret = RequestVendor(
        serialId, HREQ_SIM_OPEN_LOGICAL_CHANNEL, simFuncs_, &HRilSimReq::SimOpenLogicalChannel, appIDPoint, p2);
    SafeFrees(appIDPoint);
    return ret;
}

int32_t HRilSim::SimCloseLogicalChannel(int32_t serialId, int32_t channelId)
{
    return RequestVendor(
        serialId, HREQ_SIM_CLOSE_LOGICAL_CHANNEL, simFuncs_, &HRilSimReq::SimCloseLogicalChannel, channelId);
}

int32_t HRilSim::SimTransmitApduLogicalChannel(
    int32_t serialId, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO)
{
    std::unique_ptr<HRilApduSimIO> rilApduSimIO = std::make_unique<HRilApduSimIO>();
    CopyToHRilApduSimIO(rilApduSimIO, apduSimIO);
    int32_t ret = RequestVendor(serialId, HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL, simFuncs_,
        &HRilSimReq::SimTransmitApduLogicalChannel, rilApduSimIO.get(), sizeof(*rilApduSimIO));
    SafeFrees(rilApduSimIO->data);
    return ret;
}

int32_t HRilSim::SimTransmitApduBasicChannel(
    int32_t serialId, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO)
{
    std::unique_ptr<HRilApduSimIO> rilApduSimIO = std::make_unique<HRilApduSimIO>();
    CopyToHRilApduSimIO(rilApduSimIO, apduSimIO);
    int32_t ret = RequestVendor(serialId, HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL, simFuncs_,
        &HRilSimReq::SimTransmitApduBasicChannel, rilApduSimIO.get(), sizeof(*rilApduSimIO));
    SafeFrees(rilApduSimIO->data);
    return ret;
}

int32_t HRilSim::SimAuthentication(
    int32_t serialId, const OHOS::HDI::Ril::V1_0::SimAuthenticationRequestInfo &simAuthInfo)
{
    std::unique_ptr<HRilSimAuthenticationRequestInfo> rilSimAuthInfo =
        std::make_unique<HRilSimAuthenticationRequestInfo>();
    CopyToHRilSimAuthentication(rilSimAuthInfo, simAuthInfo);
    int32_t ret = RequestVendor(serialId, HREQ_SIM_AUTHENTICATION, simFuncs_, &HRilSimReq::SimAuthentication,
        rilSimAuthInfo.get(), sizeof(*rilSimAuthInfo));
    SafeFrees(rilSimAuthInfo->aid, rilSimAuthInfo->data);
    return ret;
}

int32_t HRilSim::UnlockSimLock(int32_t serialId, int32_t lockType, const std::string &key)
{
    char *keyPoint = nullptr;
    CopyToCharPoint(&keyPoint, key);
    int32_t ret =
        RequestVendor(serialId, HREQ_SIM_UNLOCK_SIM_LOCK, simFuncs_, &HRilSimReq::UnlockSimLock, lockType, keyPoint);
    SafeFrees(keyPoint);
    return ret;
}

int32_t HRilSim::GetSimIOResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::IccIoResultInfo result = {};
    int32_t ret = BuildSimIOResp(result, responseInfo, response, responseLen);
    if (ret != HRIL_ERR_SUCCESS) {
        return ret;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetSimIOResponse, result);
}

int32_t HRilSim::GetSimStatusResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::CardStatusInfo rilCardStatus = {};
    if ((response == nullptr && responseLen != 0) ||
        (response != nullptr && responseLen != sizeof(HRilCardState))) {
        TELEPHONY_LOGE("Invalid response: Vendor exception!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (response == nullptr && responseLen == 0) {
        TELEPHONY_LOGE("response is null");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
        return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetSimStatusResponse, rilCardStatus);
    }
    const HRilCardState *curPtr = static_cast<const HRilCardState *>(response);
    rilCardStatus.index = curPtr->index;
    rilCardStatus.simType = curPtr->simType;
    rilCardStatus.simState = curPtr->simState;
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetSimStatusResponse, rilCardStatus);
}

int32_t HRilSim::GetImsiResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    int32_t ret = CheckCharData(response, responseLen);
    if (ret != HRIL_ERR_SUCCESS) {
        return ret;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetImsiResponse, (const char *)response);
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
        return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetSimLockStatusResponse, simLockStatus);
    }
    simLockStatus = *(static_cast<const int32_t *>(response));
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetSimLockStatusResponse, simLockStatus);
}

int32_t HRilSim::SetSimLockResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::LockStatusResp result = {};
    ResponseLockStatus(result, responseInfo, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetSimLockResponse, result);
}

int32_t HRilSim::ChangeSimPasswordResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::LockStatusResp result = {};
    ResponseLockStatus(result, responseInfo, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::ChangeSimPasswordResponse, result);
}

int32_t HRilSim::UnlockPinResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::LockStatusResp result = {};
    ResponseLockStatus(result, responseInfo, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::UnlockPinResponse, result);
}

int32_t HRilSim::UnlockPukResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::LockStatusResp result = {};
    ResponseLockStatus(result, responseInfo, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::UnlockPukResponse, result);
}

int32_t HRilSim::UnlockPin2Response(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::LockStatusResp result = {};
    ResponseLockStatus(result, responseInfo, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::UnlockPin2Response, result);
}

int32_t HRilSim::UnlockPuk2Response(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::LockStatusResp result = {};
    ResponseLockStatus(result, responseInfo, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::UnlockPuk2Response, result);
}

int32_t HRilSim::SetActiveSimResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetActiveSimResponse);
}

int32_t HRilSim::SimStkSendTerminalResponseResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SimStkSendTerminalResponseResponse);
}

int32_t HRilSim::SimStkSendEnvelopeResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SimStkSendEnvelopeResponse);
}

int32_t HRilSim::SimStkSendCallSetupRequestResultResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SimStkSendCallSetupRequestResultResponse);
}

int32_t HRilSim::SimStkIsReadyResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SimStkIsReadyResponse);
}

int32_t HRilSim::GetRadioProtocolResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::RadioProtocol radioProtocol = {};
    if (response == nullptr || responseLen != sizeof(HRilRadioProtocol)) {
        TELEPHONY_LOGE("GetRadioProtocolResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        BuildRadioProtocol(radioProtocol, response);
        TELEPHONY_LOGI("HRilSim::GetRadioProtocolResponse slotId:%{public}d, serial:%{public}d"
            "sessionId:%{public}d, phase:%{public}d, technology:%{public}d, modemId:%{public}d, status:%{public}d",
            GetSlotId(), responseInfo.serial, radioProtocol.sessionId, radioProtocol.phase, radioProtocol.technology,
            radioProtocol.modemId, radioProtocol.status);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetRadioProtocolResponse, radioProtocol);
}

int32_t HRilSim::SetRadioProtocolResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::RadioProtocol radioProtocol = {};
    if (response == nullptr || responseLen != sizeof(HRilRadioProtocol)) {
        TELEPHONY_LOGE("SetRadioProtocolResponse response is invalid");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        BuildRadioProtocol(radioProtocol, response);
        TELEPHONY_LOGI("HRilSim::SetRadioProtocolResponse slotId:%{public}d, serial:%{public}d"
            "sessionId:%{public}d, phase:%{public}d, technology:%{public}d, modemId:%{public}d, status:%{public}d",
            GetSlotId(), responseInfo.serial, radioProtocol.sessionId, radioProtocol.phase, radioProtocol.technology,
            radioProtocol.modemId, radioProtocol.status);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetRadioProtocolResponse, radioProtocol);
}

int32_t HRilSim::SimOpenLogicalChannelResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::OpenLogicalChannelResponse pOpenLogicalChannelResponse = {};
    if (response == nullptr || responseLen != sizeof(HRilOpenLogicalChannelResponse)) {
        TELEPHONY_LOGE("Invalid response: response is nullptr");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    const HRilOpenLogicalChannelResponse *pRilResponse = static_cast<const HRilOpenLogicalChannelResponse *>(response);
    pOpenLogicalChannelResponse.sw1 = pRilResponse->sw1;
    pOpenLogicalChannelResponse.sw2 = pRilResponse->sw2;
    pOpenLogicalChannelResponse.channelId = pRilResponse->channelId;
    pOpenLogicalChannelResponse.response = (pRilResponse->response == nullptr) ? "" :  pRilResponse->response;
    return Response(
        responseInfo, &HDI::Ril::V1_0::IRilCallback::SimOpenLogicalChannelResponse, pOpenLogicalChannelResponse);
}

int32_t HRilSim::SimCloseLogicalChannelResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SimCloseLogicalChannelResponse);
}

int32_t HRilSim::SimTransmitApduLogicalChannelResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::IccIoResultInfo result = {};
    int32_t ret = BuildSimIOResp(result, responseInfo, response, responseLen);
    if (ret != HRIL_ERR_SUCCESS) {
        return ret;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SimTransmitApduLogicalChannelResponse, result);
}

int32_t HRilSim::SimTransmitApduBasicChannelResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::IccIoResultInfo result = {};
    int32_t ret = BuildSimIOResp(result, responseInfo, response, responseLen);
    if (ret != HRIL_ERR_SUCCESS) {
        return ret;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SimTransmitApduBasicChannelResponse, result);
}

int32_t HRilSim::SimAuthenticationResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::IccIoResultInfo result = {};
    int32_t ret = BuildSimIOResp(result, responseInfo, response, responseLen);
    if (ret != HRIL_ERR_SUCCESS) {
        return ret;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SimAuthenticationResponse, result);
}

int32_t HRilSim::UnlockSimLockResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::LockStatusResp result = {};
    ResponseLockStatus(result, responseInfo, response, responseLen);
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::UnlockSimLockResponse, result);
}

HDI::Ril::V1_0::IccIoResultInfo HRilSim::ProcessIccIoResponse(
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::IccIoResultInfo result = {};
    if (response == nullptr || responseLen != sizeof(HRilSimIOResponse)) {
        TELEPHONY_LOGI("Invalid response: response is nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
        result.response = std::string("");
        return result;
    }
    const HRilSimIOResponse *resp = static_cast<const HRilSimIOResponse *>(response);
    result.sw1 = resp->sw1;
    result.sw2 = resp->sw2;
    result.response = (resp->response == nullptr) ? "" : std::string(resp->response);
    return result;
}

int32_t HRilSim::SimStateUpdated(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::SimStateUpdated);
}

int32_t HRilSim::SimStkSessionEndNotify(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::SimStkSessionEndNotify);
}

int32_t HRilSim::SimStkProactiveNotify(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    int32_t ret = CheckCharData(response, responseLen);
    if (ret != HRIL_ERR_SUCCESS) {
        return ret;
    }
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::SimStkProactiveNotify, (const char *)response);
}

int32_t HRilSim::SimStkAlphaNotify(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    int32_t ret = CheckCharData(response, responseLen);
    if (ret != HRIL_ERR_SUCCESS) {
        return ret;
    }
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::SimStkAlphaNotify, (const char *)response);
}

int32_t HRilSim::SimStkEventNotify(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    int32_t ret = CheckCharData(response, responseLen);
    if (ret != HRIL_ERR_SUCCESS) {
        return ret;
    }
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::SimStkEventNotify, (const char *)response);
}

int32_t HRilSim::SimStkCallSetupNotify(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::SimStkCallSetupNotify);
}

int32_t HRilSim::SimRefreshNotify(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::SimRefreshNotify);
}

int32_t HRilSim::SimRadioProtocolUpdated(
    int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if (response == nullptr || responseLen != sizeof(HRilRadioProtocol)) {
        TELEPHONY_LOGE("SimRadioProtocolUpdated response is invalid");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    HDI::Ril::V1_0::RadioProtocol radioProtocol = {};
    BuildRadioProtocol(radioProtocol, response);
    TELEPHONY_LOGI("SimRadioProtocolUpdated slotId:%{public}d, sessionId:%{public}d, phase:%{public}d, "
        "technology:%{public}d, modemId:%{public}d, status:%{public}d",
        radioProtocol.slotId, radioProtocol.sessionId, static_cast<int32_t>(radioProtocol.phase),
        radioProtocol.technology, radioProtocol.modemId, static_cast<int32_t>(radioProtocol.status));
    return Notify(notifyType, error, &HDI::Ril::V1_0::IRilCallback::SimRadioProtocolUpdated, radioProtocol);
}

bool HRilSim::IsSimResponse(uint32_t code)
{
    return ((code >= HREQ_SIM_BASE) && (code < HREQ_DATA_BASE));
}

bool HRilSim::IsSimNotification(uint32_t code)
{
    return ((code >= HNOTI_SIM_BASE) && (code < HNOTI_DATA_BASE));
}

void HRilSim::CopyToHRilSimIO(std::unique_ptr<HRilSimIO> &rilSimIO, const OHOS::HDI::Ril::V1_0::SimIoRequestInfo &simIO)
{
    rilSimIO->command = simIO.command;
    rilSimIO->fileid = simIO.fileId;
    rilSimIO->p1 = simIO.p1;
    rilSimIO->p2 = simIO.p2;
    rilSimIO->p3 = simIO.p3;
    TELEPHONY_LOGD("CopyToHRilSimIO.data: %{public}s, path: %{public}s, pin2: %{public}s", simIO.data.c_str(),
        simIO.path.c_str(), simIO.pin2.c_str());
    CopyToCharPoint(&(rilSimIO->data), simIO.data);
    CopyToCharPoint(&(rilSimIO->pathid), simIO.path);
    CopyToCharPoint(&(rilSimIO->pin2), simIO.pin2);
    TELEPHONY_LOGD("CopyToHRilSimIO.result: %{public}s, pathid: %{public}s, pin2: %{public}s", rilSimIO->data,
        rilSimIO->pathid, rilSimIO->pin2);
}

void HRilSim::CopyToHRilSimLock(
    std::unique_ptr<HRilSimClock> &rilSimLock, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo)
{
    rilSimLock->mode = simLockInfo.mode;
    rilSimLock->status = simLockInfo.status;
    rilSimLock->classx = simLockInfo.classx;
    CopyToCharPoint(&(rilSimLock->fac), simLockInfo.fac);
    CopyToCharPoint(&(rilSimLock->passwd), simLockInfo.passwd);
}

void HRilSim::CopyToHRilSimPassword(
    std::unique_ptr<HRilSimPassword> &rilSimPassword, const OHOS::HDI::Ril::V1_0::SimPasswordInfo &simPassword)
{
    CopyToCharPoint(&(rilSimPassword->fac), simPassword.fac);
    CopyToCharPoint(&(rilSimPassword->oldPassword), simPassword.oldPassword);
    CopyToCharPoint(&(rilSimPassword->newPassword), simPassword.newPassword);
    rilSimPassword->passwordLength = simPassword.passwordLength;
}

void HRilSim::CopyToHRilApduSimIO(
    std::unique_ptr<HRilApduSimIO> &rilApduSimIO, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO)
{
    rilApduSimIO->channelId = apduSimIO.channelId;
    rilApduSimIO->type = apduSimIO.type;
    rilApduSimIO->instruction = apduSimIO.instruction;
    rilApduSimIO->p1 = apduSimIO.p1;
    rilApduSimIO->p2 = apduSimIO.p2;
    rilApduSimIO->p3 = apduSimIO.p3;
    CopyToCharPoint(&(rilApduSimIO->data), apduSimIO.data);
}

void HRilSim::CopyToHRilSimAuthentication(std::unique_ptr<HRilSimAuthenticationRequestInfo> &rilSimAuthInfo,
    const OHOS::HDI::Ril::V1_0::SimAuthenticationRequestInfo &simAuthInfo)
{
    rilSimAuthInfo->serial = simAuthInfo.serial;
    CopyToCharPoint(&(rilSimAuthInfo->aid), simAuthInfo.aid);
    CopyToCharPoint(&(rilSimAuthInfo->data), simAuthInfo.authData);
}

bool HRilSim::BuildLockStatusResp(const void *response, size_t responseLen, HDI::Ril::V1_0::LockStatusResp &lockStatus)
{
    if (response == nullptr || responseLen != sizeof(HRilLockStatus)) {
        return false;
    }
    const HRilLockStatus *resp = static_cast<const HRilLockStatus *>(response);
    lockStatus.result = resp->result;
    lockStatus.remain = resp->remain;
    return true;
}

void HRilSim::ResponseLockStatus(HDI::Ril::V1_0::LockStatusResp &lockStatus, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    if (!BuildLockStatusResp(response, responseLen, lockStatus)) {
        TELEPHONY_LOGE("Invalid ResponseLockStatus: response is error");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    }
}

int32_t HRilSim::BuildSimIOResp(HDI::Ril::V1_0::IccIoResultInfo &result, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) ||
        (response != nullptr && responseLen != sizeof(HRilSimIOResponse))) {
        TELEPHONY_LOGE("Invalid BuildSimIOResp: Vendor exception!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (response == nullptr && responseLen == 0) {
        TELEPHONY_LOGE("BuildSimIOResp response is null");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    }
    result = ProcessIccIoResponse(responseInfo, response, responseLen);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilSim::CheckCharData(const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(char)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (response == nullptr && responseLen == 0) {
        TELEPHONY_LOGE("response is null");
        return HRIL_ERR_NULL_POINT;
    }
    return HRIL_ERR_SUCCESS;
}

void HRilSim::BuildRadioProtocol(HDI::Ril::V1_0::RadioProtocol &radioProtocol, const void *response)
{
    const HRilRadioProtocol *hrilRadioProtocol = static_cast<const HRilRadioProtocol *>(response);
    radioProtocol.slotId = GetSlotId();
    radioProtocol.sessionId = hrilRadioProtocol->sessionId;
    radioProtocol.phase = static_cast<HDI::Ril::V1_0::RadioProtocolPhase>(hrilRadioProtocol->phase);
    radioProtocol.technology = ConvertRadioProtocolTech(hrilRadioProtocol->technology);
    radioProtocol.modemId = hrilRadioProtocol->modemId;
    radioProtocol.status = static_cast<HDI::Ril::V1_0::RadioProtocolStatus>(hrilRadioProtocol->status);
}

int32_t HRilSim::ConvertRadioProtocolTech(int32_t tech)
{
    uint32_t radioProtocolTech = HRIL_RADIO_PROTOCOL_TECH_UNKNOWN;
    for (uint32_t radioTech = RADIO_TECHNOLOGY_GSM; radioTech <= RADIO_TECHNOLOGY_NR; radioTech++) {
        uint32_t protocolTech = 1 << radioTech;
        if ((static_cast<uint32_t>(tech) & protocolTech) != 0) {
            radioProtocolTech |= protocolTech;
        }
    }
    return static_cast<int32_t>(radioProtocolTech);
}
} // namespace Telephony
} // namespace OHOS
