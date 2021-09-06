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

#ifndef OHOS_HRIL_COMMON_H
#define OHOS_HRIL_COMMON_H

#include "hril_base.h"
#include "hril_modem_parcel.h"

namespace OHOS {
namespace Telephony {
class HRilModem : public HRilBase {
public:
    HRilModem();

    ~HRilModem() = default;

    void SetRadioStatus(int32_t slotId, struct HdfSBuf *data);
    int32_t SetRadioStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    void GetRadioStatus(int32_t slotId, struct HdfSBuf *data);
    int32_t GetRadioStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t RadioStateUpdated(
        int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    void AddHandlerToMap();
    void ProcessModemResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    void ProcessModemNotify(
        int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    void ProcessCommonRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);
    bool IsModemRespOrNotify(uint32_t code);
    void RegisterModemFuncs(const HRilModemReq *modemFuncs);

private:
    bool IsModemResponse(uint32_t code);
    bool IsModemNotification(uint32_t code);
    using RespFunc = int32_t (HRilModem::*)(int32_t slotId, int32_t requestNum,
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    using NotiFunc = int32_t (HRilModem::*)(
        int32_t slotId, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    using ReqFunc = void (HRilModem::*)(int32_t slotId, struct HdfSBuf *data);
    std::map<uint32_t, ReqFunc> reqMemberFuncMap_;
    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    const HRilModemReq *modemFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_COMMON_H
