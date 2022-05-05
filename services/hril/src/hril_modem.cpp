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

int32_t HRilModem::ShutDown(struct HdfSBuf *data)
{
    return RequestVendor(data, HREQ_MODEM_SHUT_DOWN, modemFuncs_, &HRilModemReq::ShutDown);
}

int32_t HRilModem::SetRadioState(struct HdfSBuf *data)
{
    int32_t fun = -1;
    int32_t rst = -1;
    return RequestVendor(data, HREQ_MODEM_SET_RADIO_STATUS, modemFuncs_, &HRilModemReq::SetRadioState, fun, rst);
}

int32_t HRilModem::GetRadioState(struct HdfSBuf *data)
{
    return RequestVendor(data, HREQ_MODEM_GET_RADIO_STATUS, modemFuncs_, &HRilModemReq::GetRadioState);
}

int32_t HRilModem::GetImei(struct HdfSBuf *data)
{
    return RequestVendor(data, HREQ_MODEM_GET_IMEI, modemFuncs_, &HRilModemReq::GetImei);
}

int32_t HRilModem::GetMeid(struct HdfSBuf *data)
{
    return RequestVendor(data, HREQ_MODEM_GET_MEID, modemFuncs_, &HRilModemReq::GetMeid);
}

int32_t HRilModem::GetVoiceRadioTechnology(struct HdfSBuf *data)
{
    return RequestVendor(data, HREQ_MODEM_GET_VOICE_RADIO, modemFuncs_, &HRilModemReq::GetVoiceRadioTechnology);
}

int32_t HRilModem::GetBasebandVersion(struct HdfSBuf *data)
{
    return RequestVendor(data, HREQ_MODEM_GET_BASEBAND_VERSION, modemFuncs_, &HRilModemReq::GetBasebandVersion);
}

int32_t HRilModem::RadioStateUpdated(
    const int32_t indType, const HRilErrNumber e, const void *response, size_t responselen)
{
    return Notify<int32_t, HRilInt32Parcel>(
        indType, (const int32_t *)response, responselen, HNOTI_MODEM_RADIO_STATE_UPDATED);
}

int32_t HRilModem::VoiceRadioTechUpdated(
    const int32_t indType, const HRilErrNumber e, const void *response, size_t responselen)
{
    return Notify<HRilVoiceRadioInfo, VoiceRadioTechnology>(
        indType, (const HRilVoiceRadioInfo *)response, responselen, HNOTI_MODEM_VOICE_TECH_UPDATED);
}

int32_t HRilModem::ShutDownResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, HrilEmptyParcel(), requestNum);
}

int32_t HRilModem::SetRadioStateResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, HrilEmptyParcel(), requestNum);
}

int32_t HRilModem::GetRadioStateResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, HRilInt32Parcel((const uint8_t *)response, responseLen), requestNum);
}

int32_t HRilModem::GetImeiResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, HRilStringParcel((const char *)response), requestNum);
}

int32_t HRilModem::GetMeidResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, HRilStringParcel((const char *)response), requestNum);
}

int32_t HRilModem::GetVoiceRadioTechnologyResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    VoiceRadioTechnology voiceRadioTech = {};
    if (response == nullptr) {
        TELEPHONY_LOGE("GetVoiceRadioTechnologyResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        voiceRadioTech = *static_cast<const HRilVoiceRadioInfo *>(response);
    }
    return Response(responseInfo, voiceRadioTech, requestNum);
}

int32_t HRilModem::GetBasebandVersionResponse(
    int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    return Response(responseInfo, HRilStringParcel((const char *)response), requestNum);
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
    // request
    reqMemberFuncMap_[HREQ_MODEM_SHUT_DOWN] = &HRilModem::ShutDown;
    reqMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &HRilModem::SetRadioState;
    reqMemberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &HRilModem::GetRadioState;
    reqMemberFuncMap_[HREQ_MODEM_GET_IMEI] = &HRilModem::GetImei;
    reqMemberFuncMap_[HREQ_MODEM_GET_MEID] = &HRilModem::GetMeid;
    reqMemberFuncMap_[HREQ_MODEM_GET_VOICE_RADIO] = &HRilModem::GetVoiceRadioTechnology;
    reqMemberFuncMap_[HREQ_MODEM_GET_BASEBAND_VERSION] = &HRilModem::GetBasebandVersion;
}

void HRilModem::RegisterModemFuncs(const HRilModemReq *modemFuncs)
{
    modemFuncs_ = modemFuncs;
}
} // namespace Telephony
} // namespace OHOS