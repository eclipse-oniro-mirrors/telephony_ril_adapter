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
    int32_t GetSimIO(int32_t serialId, const OHOS::HDI::Ril::V1_0::SimIoRequestInfo &simIO);
    int32_t GetSimStatus(int32_t serialId);
    int32_t GetImsi(int32_t serialId);
    int32_t GetSimLockStatus(int32_t serialId, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo);
    int32_t SetSimLock(int32_t serialId, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo);
    int32_t ChangeSimPassword(int32_t serialId, const OHOS::HDI::Ril::V1_0::SimPasswordInfo &simPassword);
    int32_t UnlockPin(int32_t serialId, const std::string &pin);
    int32_t UnlockPuk(int32_t serialId, const std::string &puk, const std::string &pin);
    int32_t UnlockPin2(int32_t serialId, const std::string &pin2);
    int32_t UnlockPuk2(int32_t serialId, const std::string &puk2, const std::string &pin2);
    int32_t SetActiveSim(int32_t serialId, int32_t index, int32_t enable);
    int32_t SimStkSendTerminalResponse(int32_t serialId, const std::string &strCmd);
    int32_t SimStkSendEnvelope(int32_t serialId, const std::string &strCmd);
    int32_t SimStkSendCallSetupRequestResult(int32_t serialId, int32_t accept);
    int32_t SimStkIsReady(int32_t serialId);
    int32_t GetRadioProtocol(int32_t serialId);
    int32_t SetRadioProtocol(int32_t serialId, const HDI::Ril::V1_0::RadioProtocol &radioProtocol);
    int32_t SimOpenLogicalChannel(int32_t serialId, const std::string &appID, int32_t p2);
    int32_t SimCloseLogicalChannel(int32_t serialId, int32_t channelId);
    int32_t SimTransmitApduLogicalChannel(
        int32_t serialId, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO);
    int32_t SimTransmitApduBasicChannel(int32_t serialId, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO);
    int32_t SimAuthentication(int32_t serialId, const OHOS::HDI::Ril::V1_0::SimAuthenticationRequestInfo &simAuthInfo);
    int32_t UnlockSimLock(int32_t serialId, int32_t lockType, const std::string &key);

private:
    void AddHandlerToMap();
    void AddNotificationHandlerToMap();
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
    int32_t SimStkSendCallSetupRequestResultResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SimStkIsReadyResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetRadioProtocolResponse(
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
    HDI::Ril::V1_0::IccIoResultInfo ProcessIccIoResponse(
        HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);

    int32_t SimStateUpdated(int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t SimStkSessionEndNotify(
        int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t SimStkProactiveNotify(
        int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t SimStkAlphaNotify(int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t SimStkEventNotify(int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t SimStkCallSetupNotify(
        int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t SimRefreshNotify(int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);
    int32_t SimRadioProtocolUpdated(
        int32_t notifyType, const HRilErrNumber error, const void *response, size_t responseLen);

private:
    bool IsSimResponse(uint32_t code);
    bool IsSimNotification(uint32_t code);
    void CopyToHRilSimIO(std::unique_ptr<HRilSimIO> &rilSimIO, const OHOS::HDI::Ril::V1_0::SimIoRequestInfo &simIO);
    void CopyToHRilSimLock(
        std::unique_ptr<HRilSimClock> &rilSimLock, const OHOS::HDI::Ril::V1_0::SimLockInfo &simLockInfo);
    void CopyToHRilSimPassword(
        std::unique_ptr<HRilSimPassword> &rilSimPassword, const OHOS::HDI::Ril::V1_0::SimPasswordInfo &simPassword);
    void CopyToHRilApduSimIO(
        std::unique_ptr<HRilApduSimIO> &rilApduSimIO, const OHOS::HDI::Ril::V1_0::ApduSimIORequestInfo &apduSimIO);
    void CopyToHRilSimAuthentication(std::unique_ptr<HRilSimAuthenticationRequestInfo> &rilSimAuthInfo,
        const OHOS::HDI::Ril::V1_0::SimAuthenticationRequestInfo &simAuthInfo);
    bool BuildLockStatusResp(const void *response, size_t responseLen, HDI::Ril::V1_0::LockStatusResp &lockStatus);
    void ResponseLockStatus(HDI::Ril::V1_0::LockStatusResp &lockStatus, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t BuildSimIOResp(HDI::Ril::V1_0::IccIoResultInfo &result, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t CheckCharData(const void *response, size_t responseLen);
    void BuildRadioProtocol(HDI::Ril::V1_0::RadioProtocol &radioProtocol, const void *response);
    int32_t ConvertRadioProtocolTech(int32_t tech);

private:
    const HRilSimReq *simFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_SIM_H
