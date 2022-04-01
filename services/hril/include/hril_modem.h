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

    int32_t SetRadioState(struct HdfSBuf *data);
    int32_t GetRadioState(struct HdfSBuf *data);
    int32_t GetImei(struct HdfSBuf *data);
    int32_t GetMeid(struct HdfSBuf *data);
    int32_t GetVoiceRadioTechnology(struct HdfSBuf *data);
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
    int32_t RadioStateUpdated(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t VoiceRadioTechUpdated(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    void AddHandlerToMap();
    bool IsModemRespOrNotify(uint32_t code);
    void RegisterModemFuncs(const HRilModemReq *modemFuncs);

private:
    bool IsModemResponse(uint32_t code);
    bool IsModemNotification(uint32_t code);

    const HRilModemReq *modemFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_COMMON_H
