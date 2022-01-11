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
HRilModem::HRilModem(IHRilReporter &hrilReporter)
    : HRilBase(hrilReporter)
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
}

void HRilModem::GetImei(int32_t slotId, struct HdfSBuf *data)
{
    if (modemFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetImei::modemFuncs_ is nullptr");
        return;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetImei::miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_MODEM_GET_IMEI);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetImei::Create Request is fail");
        return;
    }
    modemFuncs_->GetImei(requestInfo);
}

void HRilModem::GetMeid(int32_t slotId, struct HdfSBuf *data)
{
    if (modemFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetMeid::modemFuncs_ is nullptr");
        return;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetMeid::miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_MODEM_GET_MEID);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetMeid::Create Request is fail");
        return;
    }
    modemFuncs_->GetMeid(requestInfo);
}

void HRilModem::GetVoiceRadioTechnology(int32_t slotId, struct HdfSBuf *data)
{
    if (modemFuncs_ == nullptr) {
        TELEPHONY_LOGE("GetVoiceRadioTechnology::modemFuncs_ is nullptr");
        return;
    }
    int32_t serial = 0;
    if (!HdfSbufReadInt32(data, &serial)) {
        TELEPHONY_LOGE("GetVoiceRadioTechnology::miss serial parameter");
        return;
    }
    ReqDataInfo *requestInfo = CreateHRilRequest(serial, slotId, HREQ_MODEM_GET_VOICE_RADIO);
    if (requestInfo == nullptr) {
        TELEPHONY_LOGE("GetVoiceRadioTechnology::Create Request is fail");
        return;
    }
    modemFuncs_->GetVoiceRadioTechnology(requestInfo);
}

int32_t HRilModem::SetRadioStateResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("HRilModem::SetRadioStateResponse  HRIL_ERR_GENERIC_FAILURE ");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write unpad buffer failed");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("Dispatcher failed: ret:%d", ret);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
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
        TELEPHONY_LOGE("HRilModem::GetRadioStateResponse HRIL_ERR_GENERIC_FAILURE ");
        return HRIL_ERR_NULL_POINT;
    }

    if (!HdfSbufWriteInt32(dataSbuf, radioState)) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write data failed");
        return HRIL_ERR_GENERIC_FAILURE;
    }

    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write unpad buffer failed");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("dispatch failed: ret:%d,radioState:%d", ret, radioState);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilModem::GetImeiResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (response == nullptr) ? " " : (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in GetImeiResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetImeiResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);

    return HDF_SUCCESS;
}

int32_t HRilModem::GetMeidResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
    const void *response, size_t responseLen)
{
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("dataSbuf is nullptr!");
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteString(dataSbuf, (response == nullptr) ? " " : (const char *)response)) {
        TELEPHONY_LOGE("HdfSbufWriteString in GetMeidResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    if (!HdfSbufWriteUnpadBuffer(dataSbuf, (const uint8_t *)&responseInfo, sizeof(responseInfo))) {
        TELEPHONY_LOGE("HdfSbufWriteUnpadBuffer in GetMeidResponse is failed!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    int32_t ret = ServiceDispatcher(requestNum, dataSbuf);
    if (ret != HDF_SUCCESS) {
        TELEPHONY_LOGE("ret is not equal to HDF_SUCCESS!");
        HdfSBufRecycle(dataSbuf);
        return HDF_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);

    return HDF_SUCCESS;
}

int32_t HRilModem::GetVoiceRadioTechnologyResponse(int32_t slotId, int32_t requestNum,
    HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen)
{
    VoiceRadioTechnology voiceRadioTech;
    voiceRadioTech.srvStatus = HRIL_NO_SRV_SERVICE;
    voiceRadioTech.srvDomain = HRIL_NO_DOMAIN_SERVICE;
    voiceRadioTech.roamStatus = HRIL_ROAM_UNKOWN;
    voiceRadioTech.simStatus = HRIL_USIM_INVALID;
    voiceRadioTech.lockStatus = HRIL_SIM_CARD_UNLOCK;
    voiceRadioTech.sysMode = HRIL_NO_SYSMODE_SERVICE;
    voiceRadioTech.sysModeName = "";
    voiceRadioTech.actType = RADIO_TECHNOLOGY_UNKNOWN;
    voiceRadioTech.actName = "";
    if (response == nullptr) {
        TELEPHONY_LOGE("GetVoiceRadioTechnologyResponse Invalid response: nullptr");
        if (responseInfo.error == HRilErrType::NONE) {
            responseInfo.error = HRilErrType::HRIL_ERR_INVALID_RESPONSE;
        }
    } else {
        const HRilVoiceRadioInfo *hrilVoiceRadioInfo = static_cast<const HRilVoiceRadioInfo *>(response);
        voiceRadioTech.srvStatus = hrilVoiceRadioInfo->srvStatus;
        voiceRadioTech.srvDomain = hrilVoiceRadioInfo->srvDomain;
        voiceRadioTech.roamStatus = hrilVoiceRadioInfo->roamStatus;
        voiceRadioTech.simStatus = hrilVoiceRadioInfo->simStatus;
        voiceRadioTech.lockStatus = hrilVoiceRadioInfo->lockStatus;
        voiceRadioTech.sysMode = hrilVoiceRadioInfo->sysMode;
        voiceRadioTech.sysModeName = hrilVoiceRadioInfo->sysModeName;
        voiceRadioTech.actType = hrilVoiceRadioInfo->actType;
        voiceRadioTech.actName = hrilVoiceRadioInfo->actName;
        TELEPHONY_LOGI(
            "GetVoiceRadioTechnologyResponse srvStatus:%{public}d, srvDomain:%{public}d, "
            "roamStatus:%{public}d, simStatus:%{public}d, lockStatus:%{public}d,"
            "sysMode:%{public}d, sysModeName:%{public}s, actType:%{public}d, actName:%{public}s",
            voiceRadioTech.srvStatus, voiceRadioTech.srvDomain, voiceRadioTech.roamStatus, voiceRadioTech.simStatus,
            voiceRadioTech.lockStatus, voiceRadioTech.sysMode, voiceRadioTech.sysModeName.c_str(),
            voiceRadioTech.actType, voiceRadioTech.actName.c_str());
    }
    return ResponseMessageParcel(responseInfo, voiceRadioTech, requestNum);
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

int32_t HRilModem::RadioStateUpdated(
    const int32_t slotId, int32_t indType, const HRilErrNumber e, const void *response, size_t responselen)
{
    if (response == nullptr) {
        TELEPHONY_LOGE("response is null");
        return HRIL_ERR_NULL_POINT;
    }
    HRilRadioState radioState = *((HRilRadioState *)response);
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
    struct HdfSBuf *dataSbuf = HdfSBufTypedObtain(SBUF_IPC);
    if (dataSbuf == nullptr) {
        TELEPHONY_LOGE("get hdf handler failed");
        return HRIL_ERR_NULL_POINT;
    }
    if (!HdfSbufWriteInt32(dataSbuf, static_cast<int32_t>(radioState))) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write data failed");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    if (!HdfSbufWriteInt32(dataSbuf, indType)) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("hdf write data failed");
        return HRIL_ERR_GENERIC_FAILURE;
    }
    int32_t ret = ServiceNotifyDispatcher(HNOTI_MODEM_RADIO_STATE_UPDATED, dataSbuf);
    if (ret != HRIL_ERR_SUCCESS) {
        HdfSBufRecycle(dataSbuf);
        TELEPHONY_LOGE("dispatch failed: ret:%{public}d", ret);
        return HRIL_ERR_GENERIC_FAILURE;
    }
    HdfSBufRecycle(dataSbuf);
    return HRIL_ERR_SUCCESS;
}

int32_t HRilModem::VoiceRadioTechUpdated(
    const int32_t slotId, int32_t indType, const HRilErrNumber e, const void *response, size_t responselen)
{
    if (response == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    HRilVoiceRadioInfo *voiceInfo = ((HRilVoiceRadioInfo *)response);
    VoiceRadioTechnology voiceTech;
    indType = (int32_t)ConvertIntToRadioNoticeType(indType);
    voiceTech.srvStatus = voiceInfo->srvStatus;
    voiceTech.srvDomain = voiceInfo->srvDomain;
    voiceTech.roamStatus = voiceInfo->roamStatus;
    voiceTech.simStatus = voiceInfo->simStatus;
    voiceTech.lockStatus = voiceInfo->lockStatus;
    voiceTech.sysMode = voiceInfo->sysMode;
    voiceTech.sysModeName = (voiceInfo->sysModeName == nullptr) ? "" : voiceInfo->sysModeName;
    voiceTech.actType = voiceInfo->actType;
    voiceTech.actName = (voiceInfo->actName == nullptr) ? "" : voiceInfo->actName;
    TELEPHONY_LOGI("VoiceRadioTechUpdated indType%{public}d,srvStatus%{public}d,srvDomain:%{public}d,"
        "roamStatus:%{public}d,simStatus:%{public}d,lockStatus:%{public}d,sysMode:%{public}d,"
        "sysModeName:%{public}s,actType:%{public}d,actName:%{public}s", indType, voiceInfo->srvStatus,
        voiceInfo->srvDomain, voiceInfo->roamStatus, voiceInfo->simStatus, voiceInfo->lockStatus,
        voiceInfo->sysMode, voiceInfo->sysModeName, voiceInfo->actType, voiceInfo->actName);
    struct HdfSBuf *dataSbuf;
    std::unique_ptr<MessageParcel> parcel = std::make_unique<MessageParcel>();
    if (parcel == nullptr) {
        return HDF_FAILURE;
    }
    dataSbuf = ParcelToSbuf(parcel.get());
    if (dataSbuf == nullptr) {
        return HDF_FAILURE;
    }
    voiceTech.Marshalling(*parcel.get());
    int32_t ret = ServiceNotifyDispatcher(HNOTI_MODEM_VOICE_TECH_UPDATED, dataSbuf);
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
    notiMemberFuncMap_[HNOTI_MODEM_VOICE_TECH_UPDATED] = &HRilModem::VoiceRadioTechUpdated;
    // response
    respMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &HRilModem::SetRadioStateResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &HRilModem::GetRadioStateResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_IMEI] = &HRilModem::GetImeiResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_MEID] = &HRilModem::GetMeidResponse;
    respMemberFuncMap_[HREQ_MODEM_GET_VOICE_RADIO] = &HRilModem::GetVoiceRadioTechnologyResponse;
    // request
    reqMemberFuncMap_[HREQ_MODEM_SET_RADIO_STATUS] = &HRilModem::SetRadioState;
    reqMemberFuncMap_[HREQ_MODEM_GET_RADIO_STATUS] = &HRilModem::GetRadioState;
    reqMemberFuncMap_[HREQ_MODEM_GET_IMEI] = &HRilModem::GetImei;
    reqMemberFuncMap_[HREQ_MODEM_GET_MEID] = &HRilModem::GetMeid;
    reqMemberFuncMap_[HREQ_MODEM_GET_VOICE_RADIO] = &HRilModem::GetVoiceRadioTechnology;
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

void HRilModem::ProcessModemNotify(int32_t slotId, const struct ReportInfo *reportInfo,
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