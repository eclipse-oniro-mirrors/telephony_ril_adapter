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
HRilModem::HRilModem(int32_t slotId, IHRilReporter &hrilReporter) : HRilBase(slotId, hrilReporter)
{
    AddHandlerToMap();
}

int32_t HRilModem::ShutDown(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_MODEM_SHUT_DOWN, modemFuncs_, &HRilModemReq::ShutDown);
}

int32_t HRilModem::SetRadioState(int32_t serialId, int32_t fun, int32_t rst)
{
    return RequestVendor(serialId, HREQ_MODEM_SET_RADIO_STATUS, modemFuncs_, &HRilModemReq::SetRadioState, fun, rst);
}

int32_t HRilModem::GetRadioState(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_MODEM_GET_RADIO_STATUS, modemFuncs_, &HRilModemReq::GetRadioState);
}

int32_t HRilModem::GetImei(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_MODEM_GET_IMEI, modemFuncs_, &HRilModemReq::GetImei);
}

int32_t HRilModem::GetMeid(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_MODEM_GET_MEID, modemFuncs_, &HRilModemReq::GetMeid);
}

int32_t HRilModem::GetVoiceRadioTechnology(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_MODEM_GET_VOICE_RADIO, modemFuncs_, &HRilModemReq::GetVoiceRadioTechnology);
}

int32_t HRilModem::GetBasebandVersion(int32_t serialId)
{
    return RequestVendor(serialId, HREQ_MODEM_GET_BASEBAND_VERSION, modemFuncs_, &HRilModemReq::GetBasebandVersion);
}

int32_t HRilModem::RadioStateUpdated(
    const int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(int32_t)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (response == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HRIL_ERR_NULL_POINT;
    }
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::RadioStateUpdated, *(const int32_t *)response);
}

int32_t HRilModem::VoiceRadioTechUpdated(
    const int32_t indType, const HRilErrNumber error, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(HRilVoiceRadioInfo)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (response == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HRIL_ERR_NULL_POINT;
    }
    HDI::Ril::V1_0::VoiceRadioTechnology voiceRadioTech = {};
    BuildIVoiceRadioTechnology(voiceRadioTech, *(const HRilVoiceRadioInfo *)response);
    return Notify(indType, error, &HDI::Ril::V1_0::IRilCallback::VoiceRadioTechUpdated, voiceRadioTech);
}

int32_t HRilModem::ShutDownResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::ShutDownResponse);
}

int32_t HRilModem::SetRadioStateResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::SetRadioStateResponse);
}

int32_t HRilModem::GetRadioStateResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(int32_t)) != 0) {
        TELEPHONY_LOGE("Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (response == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HRIL_ERR_NULL_POINT;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetRadioStateResponse, *(const int32_t *)response);
}

int32_t HRilModem::GetImeiResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(char)) != 0) {
        TELEPHONY_LOGE("GetImeiResponse:Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (response == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HRIL_ERR_NULL_POINT;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetImeiResponse, std::string((const char *)response));
}

int32_t HRilModem::GetMeidResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(char)) != 0) {
        TELEPHONY_LOGE("GetMeidResponse:Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (response == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HRIL_ERR_NULL_POINT;
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetMeidResponse, std::string((const char *)response));
}

int32_t HRilModem::GetVoiceRadioTechnologyResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    HDI::Ril::V1_0::VoiceRadioTechnology voiceRadioTech = {};
    if (response == nullptr) {
        TELEPHONY_LOGE("GetVoiceRadioTechnologyResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        BuildIVoiceRadioTechnology(voiceRadioTech, *(const HRilVoiceRadioInfo *)response);
    }
    return Response(responseInfo, &HDI::Ril::V1_0::IRilCallback::GetVoiceRadioTechnologyResponse, voiceRadioTech);
}

void HRilModem::BuildIVoiceRadioTechnology(
    HDI::Ril::V1_0::VoiceRadioTechnology &voiceRadioTech, const HRilVoiceRadioInfo &hRiadioInfo)
{
    voiceRadioTech.srvStatus = static_cast<OHOS::HDI::Ril::V1_0::RilSrvStatus>(hRiadioInfo.srvStatus);
    voiceRadioTech.srvDomain = static_cast<OHOS::HDI::Ril::V1_0::RilSrvDomain>(hRiadioInfo.srvDomain);
    voiceRadioTech.roamStatus = static_cast<OHOS::HDI::Ril::V1_0::RilRoamStatus>(hRiadioInfo.roamStatus);
    voiceRadioTech.simStatus = static_cast<OHOS::HDI::Ril::V1_0::RilSimStatus>(hRiadioInfo.simStatus);
    voiceRadioTech.lockStatus = static_cast<OHOS::HDI::Ril::V1_0::RilSimLockStatus>(hRiadioInfo.lockStatus);
    voiceRadioTech.sysMode = static_cast<OHOS::HDI::Ril::V1_0::RilSysMode>(hRiadioInfo.sysMode);
    voiceRadioTech.sysModeName = (hRiadioInfo.sysModeName == nullptr) ? "" : hRiadioInfo.sysModeName;
    voiceRadioTech.actType = static_cast<OHOS::HDI::Ril::V1_0::RilRadioTech>(hRiadioInfo.actType);
    voiceRadioTech.actName = (hRiadioInfo.actName == nullptr) ? "" : hRiadioInfo.actName;
}

int32_t HRilModem::GetBasebandVersionResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    if ((response == nullptr && responseLen != 0) || (responseLen % sizeof(char)) != 0) {
        TELEPHONY_LOGE("GetBasebandVersionResponse:Invalid parameter, responseLen:%{public}zu", responseLen);
        return HRIL_ERR_INVALID_PARAMETER;
    }
    if (response == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HRIL_ERR_NULL_POINT;
    }
    return Response(
        responseInfo, &HDI::Ril::V1_0::IRilCallback::GetBasebandVersionResponse, std::string((const char *)response));
}

bool HRilModem::IsModemResponse(uint32_t code)
{
    return ((code >= HREQ_COMMON_BASE) && (code <= HREQ_MODEM_GET_VOICE_RADIO));
}

bool HRilModem::IsModemNotification(uint32_t code)
{
    return ((code >= HNOTI_COMMON_BASE) && (code <= HNOTI_MODEM_VOICE_TECH_UPDATED));
}

bool HRilModem::IsModemRespOrNotify(uint32_t code)
{
    return IsModemResponse(code) || IsModemNotification(code);
}

void HRilModem::AddHandlerToMap()
{
    // indication
    notiMemberFuncMap_[HNOTI_MODEM_RADIO_STATE_UPDATED] = &HRilModem::RadioStateUpdated;
    notiMemberFuncMap_[HNOTI_MODEM_VOICE_TECH_UPDATED] = &HRilModem::VoiceRadioTechUpdated;
    // response
    respMemberFuncMap_[HREQ_MODEM_SHUT_DOWN] = &HRilModem::ShutDownResponse;
    respMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &HRilModem::SetRadioStateResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &HRilModem::GetRadioStateResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_IMEI] = &HRilModem::GetImeiResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_MEID] = &HRilModem::GetMeidResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_VOICE_RADIO] = &HRilModem::GetVoiceRadioTechnologyResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_BASEBAND_VERSION] = &HRilModem::GetBasebandVersionResponse;
}

void HRilModem::RegisterModemFuncs(const HRilModemReq *modemFuncs)
{
    modemFuncs_ = modemFuncs;
}
} // namespace Telephony
} // namespace OHOS