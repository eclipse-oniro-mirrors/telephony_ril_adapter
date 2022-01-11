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

#ifndef OHOS_HRIL_SIM_H
#define OHOS_HRIL_SIM_H

#include "hril_base.h"
#include "hril_sim_parcel.h"

namespace OHOS {
namespace Telephony {
class HRilSim : public HRilBase {
public:
    HRilSim(IHRilReporter &hrilReporter);
    virtual ~HRilSim() = default;

    void ProcessSimRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);
    void ProcessSimResponse(int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    void ProcessSimNotify(int32_t slotId, const struct ReportInfo *reportInfo,
        const void *response, size_t responseLen);
    bool IsSimRespOrNotify(uint32_t code);
    void RegisterSimFuncs(const HRilSimReq *simFuncs);

private:
    void AddHandlerToMap();

    void GetSimIO(int32_t slotId, struct HdfSBuf *data);
    void GetSimStatus(int32_t slotId, struct HdfSBuf *data);
    void GetImsi(int32_t slotId, struct HdfSBuf *data);
    void GetSimLockStatus(int32_t slotId, struct HdfSBuf *data);
    void SetSimLock(int32_t slotId, struct HdfSBuf *data);
    void ChangeSimPassword(int32_t slotId, struct HdfSBuf *data);
    void UnlockPin(int32_t slotId, struct HdfSBuf *data);
    void UnlockPuk(int32_t slotId, struct HdfSBuf *data);
    void GetSimPinInputTimes(int32_t slotId, struct HdfSBuf *data);
    void UnlockPin2(int32_t slotId, struct HdfSBuf *data);
    void UnlockPuk2(int32_t slotId, struct HdfSBuf *data);
    void GetSimPin2InputTimes(int32_t slotId, struct HdfSBuf *data);
    void SetActiveSim(int32_t slotId, struct HdfSBuf *data);
    void SendTerminalResponseCmd(int32_t slotId, struct HdfSBuf *data);
    void SendEnvelopeCmd(int32_t slotId, struct HdfSBuf *data);
    void StkControllerIsReady(int32_t slotId, struct HdfSBuf *data);
    void StkCmdCallSetup(int32_t slotId, struct HdfSBuf *data);
    void SetRadioProtocol(int32_t slotId, struct HdfSBuf *data);
    void OpenLogicalSimIO(int32_t slotId, struct HdfSBuf *data);
    void CloseLogicalSimIO(int32_t slotId, struct HdfSBuf *data);
    void TransmitApduSimIO(int32_t slotId, struct HdfSBuf *data);
    void UnlockSimLock(int32_t slotld, struct HdfSBuf *data);

    int32_t GetSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetSimStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetImsiResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetSimLockStatusResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetSimLockResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t ChangeSimPasswordResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t UnlockPinResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t UnlockPukResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetSimPinInputTimesResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t UnlockPin2Response(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t UnlockPuk2Response(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetSimPin2InputTimesResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetActiveSimResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SendTerminalResponseCmdResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SendEnvelopeCmdResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t StkControllerIsReadyResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t StkCmdCallSetupResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetRadioProtocolResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t OpenLogicalSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t CloseLogicalSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t TransmitApduSimIOResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t UnlockSimLockResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    IccIoResultInfo ProcessIccIoResponse(HRilRadioResponseInfo &responseInfo, const void *response,
        size_t responseLen);

    int32_t SimStateUpdated(
        int32_t slotId, int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen);
    int32_t StkSessionEndNotify(
        int32_t slotId, int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen);
    int32_t StkProactiveCommandNotify(
        int32_t slotId, int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen);
    int32_t StkAlphaNotify(
        int32_t slotId, int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen);

private:
    bool IsSimResponse(uint32_t code);
    bool IsSimNotification(uint32_t code);
    using ReqFunc = void (HRilSim::*)(int32_t slotId, struct HdfSBuf *data);
    using RespFunc = int32_t (HRilSim::*)(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    using NotiFunc = int32_t (HRilSim::*)(
        int32_t slotId, int32_t notifyType, HRilErrNumber e, const void *response, size_t responseLen);

    std::map<uint32_t, ReqFunc> reqMemberFuncMap_;
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    const HRilSimReq *simFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_SIM_H