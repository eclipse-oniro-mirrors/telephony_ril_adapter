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

#ifndef OHOS_HRIL_MODEM_H
#define OHOS_HRIL_MODEM_H

#include "hril_base.h"
#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
class HRilModem : public HRilBase {
public:
    HRilModem(int32_t slotId, IHRilReporter &hrilReporter);
    virtual ~HRilModem() = default;

    int32_t ShutDown(int32_t serialId);
    int32_t SetRadioState(int32_t serialId, int32_t fun, int32_t rst);
    int32_t GetRadioState(int32_t serialId);
    int32_t GetImei(int32_t serialId);
    int32_t GetMeid(int32_t serialId);
    int32_t GetVoiceRadioTechnology(int32_t serialId);
    int32_t GetBasebandVersion(int32_t serialId);
    int32_t ShutDownResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetRadioStateResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetRadioStateResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetImeiResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetMeidResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetVoiceRadioTechnologyResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetBasebandVersionResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t RadioStateUpdated(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t VoiceRadioTechUpdated(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    void AddHandlerToMap();
    bool IsModemRespOrNotify(uint32_t code);
    void RegisterModemFuncs(const HRilModemReq *modemFuncs);

private:
    bool IsModemResponse(uint32_t code);
    bool IsModemNotification(uint32_t code);
    void BuildIVoiceRadioTechnology(
        HDI::Ril::V1_0::VoiceRadioTechnology &voiceRadioTech, const HRilVoiceRadioInfo &hRiadioInfo);
    const HRilModemReq *modemFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_COMMON_H
