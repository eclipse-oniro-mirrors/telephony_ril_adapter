/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_RIL_CALLBACK_TEST_H
#define OHOS_RIL_CALLBACK_TEST_H

#include <iostream>

#include "condition_variable"
#include "map"
#include "mutex"
#include "v1_0/iril.h"

namespace OHOS {
namespace Telephony {
class RilCallbackTest : public HDI::Ril::V1_0::IRilCallback {
public:
    void PrintResponseInfo(const std::string &method, const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo);

    // Call
    int32_t CallStateUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t CallRingbackVoiceNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::RingbackVoice &ringbackVoice) override;
    int32_t CallSrvccStatusNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SrvccStatus &srvccStatus) override;
    int32_t CallRsrvccStatusNotify(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t CallUssdNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::UssdNoticeInfo &ussdNoticeInfo) override;
    int32_t CallSsNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SsNoticeInfo &ssNoticeInfo) override;
    int32_t CallEmergencyNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::EmergencyInfoList &emergencyInfoList) override;
    int32_t SetEmergencyCallListResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetEmergencyCallListResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::EmergencyInfoList &emergencyInfoList) override;
    int32_t GetCallListResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CallInfoList &callList) override;
    int32_t DialResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t HangupResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t RejectResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t AnswerResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t HoldCallResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t UnHoldCallResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SwitchCallResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetClipResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::GetClipResult &getClipResult) override;
    int32_t SetClipResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t CombineConferenceResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SeparateConferenceResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t CallSupplementResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetCallWaitingResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CallWaitResult &callWaitResult) override;
    int32_t SetCallWaitingResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetCallTransferInfoResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CallForwardQueryInfoList &cFQueryList) override;
    int32_t SetCallTransferInfoResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetCallRestrictionResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CallRestrictionResult &result) override;
    int32_t SetCallRestrictionResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetClirResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::GetClirResult &getClirResult) override;
    int32_t SetClirResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t StartDtmfResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SendDtmfResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t StopDtmfResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetCallPreferenceModeResponse(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, int32_t mode) override;
    int32_t SetCallPreferenceModeResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SetUssdResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetUssdResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, int32_t cusd) override;
    int32_t SetMuteResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetMuteResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, int32_t mute) override;
    int32_t GetCallFailReasonResponse(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, int32_t callFail) override;
    int32_t SetBarringPasswordResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;

    // Data
    int32_t PdpContextListUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::DataCallResultList &dataCallResultList) override;
    int32_t ActivatePdpContextResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SetupDataCallResultInfo &setupDataCallResultInfo) override;
    int32_t DeactivatePdpContextResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetPdpContextListResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::DataCallResultList &dataCallResultList) override;
    int32_t SetInitApnInfoResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SetLinkBandwidthReportingRuleResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetLinkBandwidthInfoResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::DataLinkBandwidthInfo &dataLinkBandwidthInfo) override;
    int32_t SetDataPermittedResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SetDataProfileInfoResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo);

    // Modem
    int32_t RadioStateUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, int32_t state) override;
    int32_t VoiceRadioTechUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::VoiceRadioTechnology &voiceRadioTechnology) override;
    int32_t ShutDownResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SetRadioStateResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetRadioStateResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, int32_t state) override;
    int32_t GetImeiResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const std::string &imei) override;
    int32_t GetMeidResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const std::string &meid) override;
    int32_t GetVoiceRadioTechnologyResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::VoiceRadioTechnology &voiceRadioTechnology) override;
    int32_t GetBasebandVersionResponse(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const std::string &basebandVersion) override;

    // Sim notice
    int32_t SimStateUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSessionEndNotify(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkProactiveNotify(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const std::string &response) override;
    int32_t SimStkAlphaNotify(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const std::string &response) override;
    int32_t SimStkEventNotify(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const std::string &response) override;
    int32_t SimStkCallSetupNotify(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SimRefreshNotify(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SimRadioProtocolUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::RadioProtocol &radioProtocol) override
    {
        return 0;
    }
    // Sim response
    int32_t GetSimIOResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::IccIoResultInfo &result) override;
    int32_t GetSimStatusResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CardStatusInfo &result) override;
    int32_t GetImsiResponse(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const std::string &response) override;
    int32_t GetSimLockStatusResponse(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, int32_t simLockStatus) override;
    int32_t SetSimLockResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::LockStatusResp &lockStatus) override;
    int32_t ChangeSimPasswordResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::LockStatusResp &lockStatus) override;
    int32_t UnlockPinResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::LockStatusResp &lockStatus) override;
    int32_t UnlockPukResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::LockStatusResp &lockStatus) override;
    int32_t UnlockPin2Response(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::LockStatusResp &lockStatus) override;
    int32_t UnlockPuk2Response(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::LockStatusResp &lockStatus) override;
    int32_t SetActiveSimResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSendTerminalResponseResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSendEnvelopeResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSendCallSetupRequestResultResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkIsReadyResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SetRadioProtocolResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::RadioProtocol &radioProtocol) override;
    int32_t GetRadioProtocolResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::RadioProtocol &radioProtocol) override
    {
        return 0;
    }
    int32_t SimOpenLogicalChannelResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::OpenLogicalChannelResponse &pOpenLogicalChannelResponse) override;
    int32_t SimCloseLogicalChannelResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SimTransmitApduLogicalChannelResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::IccIoResultInfo &result) override;
    int32_t SimTransmitApduBasicChannelResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::IccIoResultInfo &result) override;
    int32_t SimAuthenticationResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::IccIoResultInfo &result) override;
    int32_t UnlockSimLockResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::LockStatusResp &lockStatus) override;

    // Network
    int32_t NetworkCsRegStatusUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CsRegStatusInfo &csRegStatusInfo) override;
    int32_t NetworkPsRegStatusUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::PsRegStatusInfo &psRegStatusInfo) override;
    int32_t SignalStrengthUpdated(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_0::Rssi &rssi) override;
    int32_t NetworkTimeZoneUpdated(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const std::string &timeZoneStr) override;
    int32_t NetworkTimeUpdated(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const std::string &timeStr) override;
    int32_t NetworkPhyChnlCfgUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::ChannelConfigInfoList &channelConfigInfoList) override;
    int32_t NetworkCurrentCellUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CellListCurrentInfo &cellListCurrentInfo) override;
    int32_t GetSignalStrengthResponse(
        const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_0::Rssi &rssi) override;
    int32_t GetCsRegStatusResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CsRegStatusInfo &csRegStatusInfo) override;
    int32_t GetPsRegStatusResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::PsRegStatusInfo &psRegStatusInfo) override;
    int32_t GetOperatorInfoResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::OperatorInfo &operatorInfo) override;
    int32_t GetNetworkSearchInformationResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::AvailableNetworkList &availableNetworkList) override;
    int32_t GetNetworkSelectionModeResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SetNetworkModeInfo &setNetworkModeInfo) override;
    int32_t SetNetworkSelectionModeResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetNeighboringCellInfoListResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CellListNearbyInfo &cellInfoList) override;
    int32_t GetCurrentCellInfoResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CellListCurrentInfo &cellInfoList) override;
    int32_t SetPreferredNetworkResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetPreferredNetworkResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::PreferredNetworkTypeInfo &preferredNetworkTypeInfo) override;
    int32_t GetPhysicalChannelConfigResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::ChannelConfigInfoList &channelConfigInfoList) override;
    int32_t SetLocateUpdatesResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SetNotificationFilterResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SetDeviceStateResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;

    // Sms
    int32_t NewSmsNotify(const HDI::Ril::V1_0::HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SmsMessageInfo &smsMessageInfo) override;
    int32_t NewCdmaSmsNotify(const HDI::Ril::V1_0::HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SmsMessageInfo &smsMessageInfo) override;
    int32_t SmsStatusReportNotify(const HDI::Ril::V1_0::HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SmsMessageInfo &smsMessageInfo) override;
    int32_t NewSmsStoredOnSimNotify(const HDI::Ril::V1_0::HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        int32_t recordNumber, int32_t indicationType) override;
    int32_t CBConfigNotify(const HDI::Ril::V1_0::HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CBConfigReportInfo &cellBroadConfigReportInfo) override;
    int32_t SendGsmSmsResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SendSmsResultInfo &sendSmsResultInfo) override;
    int32_t SendCdmaSmsResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SendSmsResultInfo &sendSmsResultInfo) override;
    int32_t AddSimMessageResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t DelSimMessageResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t UpdateSimMessageResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t AddCdmaSimMessageResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t DelCdmaSimMessageResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t UpdateCdmaSimMessageResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t SetSmscAddrResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetSmscAddrResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::ServiceCenterAddress &serviceCenterAddress) override;
    int32_t SetCBConfigResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetCBConfigResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CBConfigInfo &cellBroadcastInfo) override;
    int32_t SetCdmaCBConfigResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t GetCdmaCBConfigResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::CdmaCBConfigInfo &cdmaCBConfigInfo) override;
    int32_t SendSmsMoreModeResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SendSmsResultInfo &sendSmsResultInfo) override;
    int32_t SendSmsAckResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;

    int32_t CommonErrorResponse(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_CALLBACK_TEST_H