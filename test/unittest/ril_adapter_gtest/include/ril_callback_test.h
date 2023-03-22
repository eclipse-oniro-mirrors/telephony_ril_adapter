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

#include "condition_variable"
#include "map"
#include "mutex"
#include "v1_0/iril.h"

namespace OHOS {
namespace Telephony {
constexpr static int32_t SLOTID_1 = 0;
constexpr static int32_t SLOTID_2 = 1;
constexpr static int32_t SUCCESS = 0;
constexpr static int32_t WAIT_TIME_SECOND = 20;
constexpr static int32_t WAIT_TIME_SECOND_LONG = 40;
constexpr static int32_t CID = 1;
constexpr static int32_t REASON = 2;
constexpr int32_t TEST_TOSCA = 0;
const std::string TEST_STORAGE_PDU = "1234";
const std::string TEST_SEND_PDU = "A10305810180F6000004F4F29C0E";
const std::string TEST_SMSC_PDU = "00";
const std::string TEST_CDMA_PDU = "pdu";
const std::string TEST_ID_LIST = "0,1,5,320-478,922";
const std::string TEST_DCS_LIST = "0-3,5";
const int32_t TEST_MODE = 0;
const bool TEST_RESULT = true;

enum class HdiId {
    HREQ_NONE = -1,
    HREQ_CALL_BASE = 0,
    HREQ_CALL_GET_CALL_LIST,
    HREQ_CALL_DIAL,
    HREQ_CALL_HANGUP,
    HREQ_CALL_REJECT,
    HREQ_CALL_ANSWER,
    HREQ_CALL_HOLD_CALL, // call hold value 6
    HREQ_CALL_UNHOLD_CALL, // call active value 6
    HREQ_CALL_SWITCH_CALL,
    HREQ_CALL_COMBINE_CONFERENCE,
    HREQ_CALL_SEPARATE_CONFERENCE, // Keep all other calls except the xth call
    HREQ_CALL_CALL_SUPPLEMENT,
    HREQ_CALL_SEND_DTMF,
    HREQ_CALL_START_DTMF,
    HREQ_CALL_STOP_DTMF,
    HREQ_CALL_SET_CLIP,
    HREQ_CALL_GET_CLIP,
    HREQ_CALL_GET_CALL_WAITING,
    HREQ_CALL_SET_CALL_WAITING,
    HREQ_CALL_GET_CALL_RESTRICTION,
    HREQ_CALL_SET_CALL_RESTRICTION,
    HREQ_CALL_GET_CALL_TRANSFER_INFO,
    HREQ_CALL_SET_CALL_TRANSFER_INFO,
    HREQ_CALL_GET_CLIR,
    HREQ_CALL_SET_CLIR,
    HREQ_CALL_GET_CALL_PREFERENCE,
    HREQ_CALL_SET_CALL_PREFERENCE,
    HREQ_CALL_SET_USSD,
    HREQ_CALL_GET_USSD,
    HREQ_CALL_SET_MUTE,
    HREQ_CALL_GET_MUTE,
    HREQ_CALL_GET_EMERGENCY_LIST,
    HREQ_CALL_SET_EMERGENCY_LIST,
    HREQ_CALL_GET_FAIL_REASON,
    HREQ_CALL_SET_BARRING_PASSWORD,

    HREQ_SMS_BASE = 100,
    HREQ_SMS_SEND_GSM_SMS,
    HREQ_SMS_SEND_CDMA_SMS,
    HREQ_SMS_ADD_SIM_MESSAGE,
    HREQ_SMS_DEL_SIM_MESSAGE,
    HREQ_SMS_UPDATE_SIM_MESSAGE,
    HREQ_SMS_SEND_SMS_MORE_MODE,
    HREQ_SMS_SEND_SMS_ACK,
    HREQ_SMS_SET_SMSC_ADDR,
    HREQ_SMS_GET_SMSC_ADDR,
    HREQ_SMS_SET_CB_CONFIG,
    HREQ_SMS_GET_CB_CONFIG,
    HREQ_SMS_GET_CDMA_CB_CONFIG,
    HREQ_SMS_SET_CDMA_CB_CONFIG,
    HREQ_SMS_ADD_CDMA_SIM_MESSAGE,
    HREQ_SMS_DEL_CDMA_SIM_MESSAGE,
    HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE,

    HREQ_SIM_BASE = 200,
    HREQ_SIM_GET_SIM_STATUS,
    HREQ_SIM_GET_IMSI,
    HREQ_SIM_GET_SIM_IO,
    HREQ_SIM_GET_SIM_LOCK_STATUS,
    HREQ_SIM_SET_SIM_LOCK,
    HREQ_SIM_CHANGE_SIM_PASSWORD,
    HREQ_SIM_UNLOCK_PIN,
    HREQ_SIM_UNLOCK_PUK,
    HREQ_SIM_GET_SIM_PIN_INPUT_TIMES,
    HREQ_SIM_UNLOCK_PIN2,
    HREQ_SIM_UNLOCK_PUK2,
    HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES,
    HREQ_SIM_SET_ACTIVE_SIM,
    HREQ_SIM_RADIO_PROTOCOL,
    HREQ_SIM_STK_SEND_TERMINAL_RESPONSE,
    HREQ_SIM_STK_SEND_ENVELOPE,
    HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT,
    HREQ_SIM_STK_IS_READY,
    HREQ_SIM_OPEN_LOGICAL_CHANNEL,
    HREQ_SIM_CLOSE_LOGICAL_CHANNEL,
    HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL,
    HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL,
    HREQ_SIM_AUTHENTICATION,
    HREQ_SIM_UNLOCK_SIM_LOCK,

    HREQ_DATA_BASE = 300,
    HREQ_DATA_SET_INIT_APN_INFO,
    HREQ_DATA_DEACTIVATE_PDP_CONTEXT,
    HREQ_DATA_ACTIVATE_PDP_CONTEXT,
    HREQ_DATA_GET_PDP_CONTEXT_LIST,
    HREQ_DATA_GET_LINK_BANDWIDTH_INFO,
    HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE,
    HREQ_DATA_SET_DATA_PROFILE_INFO,
    HREQ_DATA_SET_DATA_PERMITTED,

    HREQ_NETWORK_BASE = 400,
    HREQ_NETWORK_GET_SIGNAL_STRENGTH,
    HREQ_NETWORK_GET_CS_REG_STATUS,
    HREQ_NETWORK_GET_PS_REG_STATUS,
    HREQ_NETWORK_GET_OPERATOR_INFO,
    HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION,
    HREQ_NETWORK_GET_NETWORK_SELECTION_MODE,
    HREQ_NETWORK_SET_NETWORK_SELECTION_MODE,
    HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST,
    HREQ_NETWORK_GET_CURRENT_CELL_INFO,
    HREQ_NETWORK_SET_PREFERRED_NETWORK,
    HREQ_NETWORK_GET_PREFERRED_NETWORK,
    HREQ_NETWORK_GET_RADIO_CAPABILITY,
    HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG,
    HREQ_NETWORK_SET_LOCATE_UPDATES,
    HREQ_NETWORK_SET_NOTIFICATION_FILTER,
    HREQ_NETWORK_SET_DEVICE_STATE,

    HREQ_COMMON_BASE = 500,
    HREQ_MODEM_SHUT_DOWN,
    HREQ_MODEM_SET_RADIO_STATUS,
    HREQ_MODEM_GET_RADIO_STATUS,
    HREQ_MODEM_GET_IMEI,
    HREQ_MODEM_GET_MEID,
    HREQ_MODEM_GET_BASEBAND_VERSION,
    HREQ_MODEM_GET_VOICE_RADIO,
    HREQ_MODEM_EXIT = 1000
};

enum class DeviceStateType { POWER_SAVE_MODE, CHARGING_STATE, LOW_DATA_STATE };

enum class NotificationFilter {
    NOTIFICATION_FILTER_ALL = -1,
    NOTIFICATION_FILTER_NONE = 0,
    NOTIFICATION_FILTER_SIGNAL_STRENGTH = 1,
    NOTIFICATION_FILTER_NETWORK_STATE = 2,
    NOTIFICATION_FILTER_DATA_CALL = 4,
    NOTIFICATION_FILTER_LINK_CAPACITY = 8,
    NOTIFICATION_FILTER_PHYSICAL_CHANNEL_CONFIG = 16
};

enum class RatType {
    NETWORK_TYPE_UNKNOWN = 0,
    NETWORK_TYPE_GSM,
    NETWORK_TYPE_CDMA,
    NETWORK_TYPE_WCDMA,
    NETWORK_TYPE_TDSCDMA,
    NETWORK_TYPE_LTE,
    NETWORK_TYPE_NR
};

enum class SelectionMode { MODE_TYPE_UNKNOWN = -1, MODE_TYPE_AUTO = 0, MODE_TYPE_MANUAL = 1 };

enum class PreferredNetworkMode {
    CORE_NETWORK_MODE_AUTO = 0,
    CORE_NETWORK_MODE_GSM = 1,
    CORE_NETWORK_MODE_WCDMA = 2,
    CORE_NETWORK_MODE_LTE = 3,
    CORE_NETWORK_MODE_LTE_WCDMA = 4,
    CORE_NETWORK_MODE_LTE_WCDMA_GSM = 5,
    CORE_NETWORK_MODE_WCDMA_GSM = 6,
    CORE_NETWORK_MODE_CDMA = 7,
    CORE_NETWORK_MODE_EVDO = 8,
    CORE_NETWORK_MODE_EVDO_CDMA = 9,
    CORE_NETWORK_MODE_WCDMA_GSM_EVDO_CDMA = 10,
    CORE_NETWORK_MODE_LTE_EVDO_CDMA = 11,
    CORE_NETWORK_MODE_LTE_WCDMA_GSM_EVDO_CDMA = 12,
    CORE_NETWORK_MODE_TDSCDMA = 13,
    CORE_NETWORK_MODE_TDSCDMA_GSM = 14,
    CORE_NETWORK_MODE_TDSCDMA_WCDMA = 15,
    CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM = 16,
    CORE_NETWORK_MODE_LTE_TDSCDMA = 17,
    CORE_NETWORK_MODE_LTE_TDSCDMA_GSM = 18,
    CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA = 19,
    CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM = 20,
    CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM_EVDO_CDMA = 21,
    CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA = 22,
    CORE_NETWORK_MODE_NR = 31,
    CORE_NETWORK_MODE_NR_LTE = 32,
    CORE_NETWORK_MODE_NR_LTE_WCDMA = 33,
    CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM = 34,
    CORE_NETWORK_MODE_NR_LTE_EVDO_CDMA = 35,
    CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM_EVDO_CDMA = 36,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA = 37,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA_GSM = 38,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA = 39,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM = 40,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA = 41,
    CORE_NETWORK_MODE_MAX_VALUE = 99,
};

enum class SimMessageStatus {
    SIM_MESSAGE_STATUS_UNREAD = 0,
    SIM_MESSAGE_STATUS_READ = 1,
    SIM_MESSAGE_STATUS_UNSENT = 2,
    SIM_MESSAGE_STATUS_SENT = 3,
};

class RilCallbackTest : public HDI::Ril::V1_0::IRilCallback {
public:
    void NotifyAll();
    void WaitFor(int32_t timeoutSecond);
    bool GetBoolResult(HdiId hdiId_);
    void Clean();

    // Call
    int32_t CallStateUpdated(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
    int32_t CallRingbackVoiceNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::RingbackVoice &ringbackVoice) override;
    int32_t CallSrvccStatusNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SrvccStatus &srvccStatus) override;
    int32_t CallUssdNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::UssdNoticeInfo &ussdNoticeInfo) override;
    int32_t CallSsNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::SsNoticeInfo &ssNoticeInfo) override;
    int32_t CallEmergencyNotice(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo,
        const HDI::Ril::V1_0::EmergencyInfoList &emergencyInfoList) override;
    int32_t CallRsrvccStatusNotify(const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo) override;
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
    int32_t GetSerialId()
    {
        currentSerialId_ = rand() % RAND_MAX;
        return currentSerialId_;
    }
    bool IsReady(int32_t slotId)
    {
        return simState_[slotId] != 0;
    }
    int32_t GetCurrentChannelId()
    {
        return currentChannelId_;
    }

    std::string GetSmscAddr()
    {
        return smscAddr_;
    }

private:
    std::map<int32_t, int32_t> simState_;
    std::mutex callbackMutex_;
    std::condition_variable cv_;
    int32_t currentChannelId_ = 1;
    std::string smscAddr_ = "00";
    HdiId hdiId_;
    HDI::Ril::V1_0::RilRadioResponseInfo resultInfo_;
    int32_t currentSerialId_ = 0;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_CALLBACK_TEST_H