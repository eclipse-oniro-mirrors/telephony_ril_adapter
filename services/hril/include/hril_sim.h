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
    HRilSim(int32_t slotId, IHRilReporter &hrilReporter);
    virtual ~HRilSim() = default;

    void ProcessSimRequest(int32_t code, struct HdfSBuf *data);
    void ProcessSimResponse(
        int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    void ProcessSimNotify(const struct ReportInfo *reportInfo, const void *response, size_t responseLen);
    bool IsSimRespOrNotify(uint32_t code);
    void RegisterSimFuncs(const HRilSimReq *simFuncs);

private:
    void AddHandlerToMap();
    void AddNotificationHandlerToMap();

    int32_t GetSimIO(struct HdfSBuf *data);
    int32_t GetSimStatus(struct HdfSBuf *data);
    int32_t GetImsi(struct HdfSBuf *data);
    int32_t GetSimLockStatus(struct HdfSBuf *data);
    int32_t SetSimLock(struct HdfSBuf *data);
    int32_t ChangeSimPassword(struct HdfSBuf *data);
    int32_t UnlockPin(struct HdfSBuf *data);
    int32_t UnlockPuk(struct HdfSBuf *data);
    int32_t UnlockPin2(struct HdfSBuf *data);
    int32_t UnlockPuk2(struct HdfSBuf *data);
    int32_t SetActiveSim(struct HdfSBuf *data);
    int32_t SimStkSendTerminalResponse(struct HdfSBuf *data);
    int32_t SimStkSendEnvelope(struct HdfSBuf *data);
    int32_t SimStkIsReady(struct HdfSBuf *data);
    int32_t SetRadioProtocol(struct HdfSBuf *data);
    int32_t SimOpenLogicalChannel(struct HdfSBuf *data);
    int32_t SimCloseLogicalChannel(struct HdfSBuf *data);
    int32_t SimTransmitApduLogicalChannel(struct HdfSBuf *data);
    int32_t SimTransmitApduBasicChannel(struct HdfSBuf *data);
    int32_t SimAuthentication(struct HdfSBuf *data);
    int32_t UnlockSimLock(struct HdfSBuf *data);

    int32_t GetSimIOResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetSimStatusResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetImsiResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetSimLockStatusResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetSimLockResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t ChangeSimPasswordResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t UnlockPinResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t UnlockPukResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t UnlockPin2Response(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t UnlockPuk2Response(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetActiveSimResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SimStkSendTerminalResponseResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SimStkSendEnvelopeResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SimStkIsReadyResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetRadioProtocolResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SimOpenLogicalChannelResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SimCloseLogicalChannelResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SimTransmitApduLogicalChannelResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SimTransmitApduBasicChannelResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SimAuthenticationResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t UnlockSimLockResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    IccIoResultInfo ProcessIccIoResponse(
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);

    int32_t SimStateUpdated(int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen);
    int32_t SimStkSessionEndNotify(
        int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen);
    int32_t SimStkProactiveNotify(int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen);
    int32_t SimStkAlphaNotify(int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen);
    int32_t SimRefreshNotify(int32_t notifyType, const HRilErrNumber e, const void *response, size_t responseLen);

private:
    bool IsSimResponse(uint32_t code);
    bool IsSimNotification(uint32_t code);

    const HRilSimReq *simFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_SIM_H
