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

#include "ril_callback_test.h"

#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::HDI::Ril::V1_0;
void RilCallbackTest::NotifyAll()
{
    std::unique_lock<std::mutex> callbackLock(callbackMutex_);
    if (resultInfo_.serial != currentSerialId_) {
        hdiId_ = HdiId::HREQ_NONE;
        TELEPHONY_LOGI("NotifyAll currentSerialId_ : %{public}d, serial: %{public}d not equal", currentSerialId_,
            resultInfo_.serial);
        return;
    }
    cv_.notify_all();
}

void RilCallbackTest::WaitFor(int32_t timeoutSecond)
{
    Clean();
    std::unique_lock<std::mutex> callbackLock(callbackMutex_);
    cv_.wait_for(callbackLock, std::chrono::seconds(timeoutSecond));
}

void RilCallbackTest::Clean()
{
    hdiId_ = HdiId::HREQ_NONE;
}

bool RilCallbackTest::GetBoolResult(HdiId hdiId)
{
    TELEPHONY_LOGI("GetBoolResult hdiId: %{public}d, error: %{public}d", hdiId, (int32_t)resultInfo_.error);
    bool ret = false;
    if (hdiId_ == HdiId::HREQ_NONE) {
        TELEPHONY_LOGE(
            "response timeout, not implemented hdiId: %{public}d, current hdiId_: %{public}d", (int32_t)hdiId, hdiId_);
        ret = true;
        Clean();
        return ret;
    }
    if (hdiId_ != hdiId) {
        ret = false;
        TELEPHONY_LOGE(
            "GetBoolResult hdiId does not match. hdiId: %{public}d, current hdiId: %{public}d", (int32_t)hdiId, hdiId_);
        Clean();
        return ret;
    }
    ret = true;
    Clean();
    return ret;
}

// SIM
int32_t RilCallbackTest::SimStateUpdated(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("SimStateUpdated notice : slotId = %{public}d", responseInfo.slotId);
    auto g_rilInterface = IRil::Get();
    if (g_rilInterface != nullptr) {
        g_rilInterface->GetSimStatus(GetSerialId(), responseInfo.slotId);
    }
    return 0;
}

int32_t RilCallbackTest::SimStkSessionEndNotify(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("SimStkSessionEndNotify notice : slotId = %{public}d", responseInfo.slotId);
    return 0;
}

int32_t RilCallbackTest::SimStkProactiveNotify(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    TELEPHONY_LOGI("SimStkProactiveNotify notice : slotId = %{public}d, response = %{public}s", responseInfo.slotId,
        response.c_str());
    return 0;
}

int32_t RilCallbackTest::SimStkAlphaNotify(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    TELEPHONY_LOGI(
        "SimStkAlphaNotify notice : slotId = %{public}d, response = %{public}s", responseInfo.slotId, response.c_str());
    return 0;
}

int32_t RilCallbackTest::SimStkEventNotify(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    TELEPHONY_LOGI(
        "SimStkEventNotify notice : slotId = %{public}d, response = %{public}s", responseInfo.slotId, response.c_str());
    return 0;
}

int32_t RilCallbackTest::SimStkCallSetupNotify(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("SimStkCallSetupNotify notice : slotId = %{public}d", responseInfo.slotId);
    return 0;
}

int32_t RilCallbackTest::SimRefreshNotify(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("SimRefreshNotify notice : slotId = %{public}d", responseInfo.slotId);
    return 0;
}

int32_t RilCallbackTest::GetSimStatusResponse(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_0::CardStatusInfo &result)
{
    TELEPHONY_LOGI(
        "GetBoolResult GetSimStatus result : slotId = %{public}d, simType = %{public}d, simState = %{public}d",
        responseInfo.slotId, result.simType, result.simState);
    simState_[responseInfo.slotId] = result.simState;
    TELEPHONY_LOGI("IsReady %{public}d %{public}d", responseInfo.slotId, simState_[responseInfo.slotId]);
    hdiId_ = HdiId::HREQ_SIM_GET_SIM_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetSimIOResponse(const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    TELEPHONY_LOGI("GetBoolResult GetSimIO result : sw1 = %{public}d, sw2 = %{public}d, response = %{public}s",
        result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_GET_SIM_IO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetImsiResponse(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    TELEPHONY_LOGI("GetBoolResult GetImsi result : response = %{public}s", response.c_str());
    hdiId_ = HdiId::HREQ_SIM_GET_IMSI;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetSimLockStatusResponse(const RilRadioResponseInfo &responseInfo, int32_t simLockStatus)
{
    TELEPHONY_LOGI("GetBoolResult GetSimLockStatus result : simLockStatus = %{public}d", simLockStatus);
    hdiId_ = HdiId::HREQ_SIM_GET_SIM_LOCK_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetSimLockResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    TELEPHONY_LOGI("GetBoolResult SetSimLock result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_SET_SIM_LOCK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::ChangeSimPasswordResponse(
    const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    TELEPHONY_LOGI("GetBoolResult ChangeSimPassword result : result = %{public}d, remain = %{public}d",
        lockStatus.result, lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_CHANGE_SIM_PASSWORD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::UnlockPinResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    TELEPHONY_LOGI("GetBoolResult UnlockPin result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PIN;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::UnlockPukResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    TELEPHONY_LOGI("GetBoolResult UnlockPuk result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PUK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::UnlockPin2Response(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    TELEPHONY_LOGI("GetBoolResult UnlockPin2 result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PIN2;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::UnlockPuk2Response(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    TELEPHONY_LOGI("GetBoolResult UnlockPuk2 result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PUK2;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetActiveSimResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetActiveSim result");
    hdiId_ = HdiId::HREQ_SIM_SET_ACTIVE_SIM;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SimStkSendTerminalResponseResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SimStkSendTerminalResponse result");
    hdiId_ = HdiId::HREQ_SIM_STK_SEND_TERMINAL_RESPONSE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SimStkSendEnvelopeResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SimStkSendEnvelope result");
    hdiId_ = HdiId::HREQ_SIM_STK_SEND_ENVELOPE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SimStkSendCallSetupRequestResultResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SimStkSendCallSetupRequestResult result");
    hdiId_ = HdiId::HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SimStkIsReadyResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SimStkIsReady result");
    hdiId_ = HdiId::HREQ_SIM_STK_IS_READY;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetRadioProtocolResponse(
    const RilRadioResponseInfo &responseInfo, const RadioProtocol &radioProtocol)
{
    TELEPHONY_LOGI("GetBoolResult SetRadioProtocol result : phase = %{public}d, slotId = %{public}d",
        radioProtocol.phase, radioProtocol.slotId);
    hdiId_ = HdiId::HREQ_SIM_RADIO_PROTOCOL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SimOpenLogicalChannelResponse(
    const RilRadioResponseInfo &responseInfo, const OpenLogicalChannelResponse &pOpenLogicalChannelResponse)
{
    TELEPHONY_LOGI(
        "GetBoolResult SimOpenLogicalChannel result : sw1 = %{public}d, sw2 = %{public}d, channelId = %{public}d, "
        "response = %{public}s",
        pOpenLogicalChannelResponse.sw1, pOpenLogicalChannelResponse.sw2, pOpenLogicalChannelResponse.channelId,
        pOpenLogicalChannelResponse.response.c_str());
    currentChannelId_ = pOpenLogicalChannelResponse.channelId;
    hdiId_ = HdiId::HREQ_SIM_OPEN_LOGICAL_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SimCloseLogicalChannelResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SimCloseLogicalChannel result");
    hdiId_ = HdiId::HREQ_SIM_CLOSE_LOGICAL_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SimTransmitApduLogicalChannelResponse(
    const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    TELEPHONY_LOGI(
        "GetBoolResult SimTransmitApduLogicalChannel result : sw1 = %{public}d, sw2 = %{public}d, response = "
        "%{public}s",
        result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SimTransmitApduBasicChannelResponse(
    const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    TELEPHONY_LOGI(
        "GetBoolResult SimTransmitApduBasicChannel result : sw1 = %{public}d, sw2 = %{public}d, response = %{public}s",
        result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SimAuthenticationResponse(
    const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    TELEPHONY_LOGI("GetBoolResult SimAuthentication result : sw1 = %{public}d, sw2 = %{public}d, response = %{public}s",
        result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_AUTHENTICATION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::UnlockSimLockResponse(
    const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    TELEPHONY_LOGI("GetBoolResult UnlockSimLock result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_SIM_LOCK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

// Network
int32_t RilCallbackTest::NetworkCsRegStatusUpdated(
    const RilRadioResponseInfo &responseInfo, const CsRegStatusInfo &csRegStatusInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::NetworkCsRegStatusUpdated notifyType:%{public}d, regStatus:%{public}d, "
        "lacCode:%{public}d, cellId:%{public}d, radioTechnology:%{public}d",
        csRegStatusInfo.notifyType, csRegStatusInfo.regStatus, csRegStatusInfo.lacCode, csRegStatusInfo.cellId,
        csRegStatusInfo.radioTechnology);
    return 0;
}

int32_t RilCallbackTest::NetworkPsRegStatusUpdated(
    const RilRadioResponseInfo &responseInfo, const PsRegStatusInfo &psRegStatusInfo)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::NetworkPsRegStatusUpdated notifyType:%{public}d, regStatus:%{public}d, lacCode:%{public}d, "
        "cellId:%{public}d, tech:%{public}d, nrRestricted:%{public}d, nrAvailable:%{public}d, enDcAvailable:%{public}d",
        psRegStatusInfo.notifyType, psRegStatusInfo.regStatus, psRegStatusInfo.lacCode, psRegStatusInfo.cellId,
        psRegStatusInfo.radioTechnology, psRegStatusInfo.isDcNrRestricted, psRegStatusInfo.isNrAvailable,
        psRegStatusInfo.isEnDcAvailable);
    return 0;
}

int32_t RilCallbackTest::SignalStrengthUpdated(const RilRadioResponseInfo &responseInfo, const Rssi &rssi)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::SignalStrengthUpdated rxlev:%{public}d rsrp:%{public}d", rssi.lte.rxlev, rssi.lte.rsrp);
    return 0;
}

int32_t RilCallbackTest::NetworkTimeZoneUpdated(
    const RilRadioResponseInfo &responseInfo, const std::string &timeZoneStr)
{
    TELEPHONY_LOGI("RilCallbackTest::NetworkTimeZoneUpdated timeZone:%{public}s", timeZoneStr.c_str());
    return 0;
}

int32_t RilCallbackTest::NetworkTimeUpdated(const RilRadioResponseInfo &responseInfo, const std::string &timeStr)
{
    TELEPHONY_LOGI("RilCallbackTest::NetworkTimeZoneUpdated time:%{public}s", timeStr.c_str());
    return 0;
}

int32_t RilCallbackTest::NetworkPhyChnlCfgUpdated(
    const RilRadioResponseInfo &responseInfo, const ChannelConfigInfoList &channelConfigInfoList)
{
    TELEPHONY_LOGI("RilCallbackTest::NetworkPhyChnlCfgUpdated itemNum:%{public}d", channelConfigInfoList.itemNum);
    for (PhysicalChannelConfig phyChnlCfg : channelConfigInfoList.channelConfigInfos) {
        TELEPHONY_LOGI(
            "RilCallbackTest::NetworkPhyChnlCfgUpdated cellConnStatus:%{public}d, "
            "cellBandwidthDownlinkKhz:%{public}d, cellBandwidthUplinkKhz:%{public}d, physicalCellId:%{public}d, "
            "ratType:%{public}d, freqRange:%{public}d, downlinkChannelNum:%{public}d, "
            "uplinkChannelNum:%{public}d, contextIdNum:%{public}d",
            phyChnlCfg.cellConnStatus, phyChnlCfg.cellBandwidthDownlinkKhz, phyChnlCfg.cellBandwidthUplinkKhz,
            phyChnlCfg.ratType, phyChnlCfg.freqRange, phyChnlCfg.downlinkChannelNum, phyChnlCfg.uplinkChannelNum,
            phyChnlCfg.physicalCellId, phyChnlCfg.contextIdNum);
        for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
            TELEPHONY_LOGI("contextIds[%{public}d]:%{public}d", j, phyChnlCfg.contextIds[j]);
        }
    }
    return 0;
}

int32_t RilCallbackTest::NetworkCurrentCellUpdated(
    const RilRadioResponseInfo &responseInfo, const CellListCurrentInfo &cellListCurrentInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::NetworkCurrentCellUpdated itemNum:%{public}d", cellListCurrentInfo.itemNum);
    for (auto info : cellListCurrentInfo.cellCurrentInfo) {
        TELEPHONY_LOGI("RilCallbackTest::NetworkCurrentCellUpdated ratType:%{public}d, mcc:%{public}d, mnc:%{public}d",
            info.ratType, info.mcc, info.mnc);
        switch (static_cast<RatType>(info.ratType)) {
            case RatType::NETWORK_TYPE_LTE:
                TELEPHONY_LOGI("cellId:%{public}d", info.serviceCells.lte.cellId);
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.lte.arfcn);
                TELEPHONY_LOGI("pci:%{public}d", info.serviceCells.lte.pci);
                TELEPHONY_LOGI("rsrp:%{public}d", info.serviceCells.lte.rsrp);
                TELEPHONY_LOGI("rsrq:%{public}d", info.serviceCells.lte.rsrq);
                TELEPHONY_LOGI("rxlev:%{public}d", info.serviceCells.lte.rssi);
                break;
            case RatType::NETWORK_TYPE_GSM:
                TELEPHONY_LOGI("band:%{public}d", info.serviceCells.gsm.band);
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.gsm.arfcn);
                TELEPHONY_LOGI("bsic:%{public}d", info.serviceCells.gsm.bsic);
                TELEPHONY_LOGI("cellId:%{public}d", info.serviceCells.gsm.cellId);
                TELEPHONY_LOGI("rxlev:%{public}d", info.serviceCells.gsm.rxlev);
                TELEPHONY_LOGI("lac:%{public}d", info.serviceCells.gsm.lac);
                break;
            case RatType::NETWORK_TYPE_WCDMA:
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.wcdma.arfcn);
                TELEPHONY_LOGI("psc:%{public}d", info.serviceCells.wcdma.psc);
                TELEPHONY_LOGI("rscp:%{public}d", info.serviceCells.wcdma.rscp);
                TELEPHONY_LOGI("ecno:%{public}d", info.serviceCells.wcdma.ecno);
                break;
            case RatType::NETWORK_TYPE_NR:
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.nr.nrArfcn);
                TELEPHONY_LOGI("psc:%{public}d", info.serviceCells.nr.pci);
                TELEPHONY_LOGI("rscp:%{public}d", info.serviceCells.nr.tac);
                TELEPHONY_LOGI("ecno:%{public}d", info.serviceCells.nr.nci);
                break;
            default:
                TELEPHONY_LOGE("RilCallbackTest::NetworkCurrentCellUpdated invalid ratType");
        }
    }
    return 0;
}

int32_t RilCallbackTest::GetSignalStrengthResponse(const RilRadioResponseInfo &responseInfo, const Rssi &rssi)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::GetSignalStrengthResponse rxlev:%{public}d rsrp:%{public}d", rssi.lte.rxlev, rssi.lte.rsrp);
    hdiId_ = HdiId::HREQ_NETWORK_GET_SIGNAL_STRENGTH;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCsRegStatusResponse(
    const RilRadioResponseInfo &responseInfo, const CsRegStatusInfo &csRegStatusInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::GetCsRegStatusResponse notifyType:%{public}d, regStatus:%{public}d, "
                   "lacCode:%{public}d, cellId:%{public}d, radioTechnology:%{public}d",
        csRegStatusInfo.notifyType, csRegStatusInfo.regStatus, csRegStatusInfo.lacCode, csRegStatusInfo.cellId,
        csRegStatusInfo.radioTechnology);
    hdiId_ = HdiId::HREQ_NETWORK_GET_CS_REG_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetPsRegStatusResponse(
    const RilRadioResponseInfo &responseInfo, const PsRegStatusInfo &psRegStatusInfo)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::GetPsRegStatusResponse notifyType:%{public}d, regStatus:%{public}d, lacCode:%{public}d, "
        "cellId:%{public}d, tech:%{public}d, nrRestricted:%{public}d, nrAvailable:%{public}d, enDcAvailable:%{public}d",
        psRegStatusInfo.notifyType, psRegStatusInfo.regStatus, psRegStatusInfo.lacCode, psRegStatusInfo.cellId,
        psRegStatusInfo.radioTechnology, psRegStatusInfo.isDcNrRestricted, psRegStatusInfo.isNrAvailable,
        psRegStatusInfo.isEnDcAvailable);
    hdiId_ = HdiId::HREQ_NETWORK_GET_PS_REG_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetOperatorInfoResponse(
    const RilRadioResponseInfo &responseInfo, const OperatorInfo &operatorInfo)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::GetOperatorInfoResponse longName:%{public}s, shortName:%{public}s, numeric:%{public}s",
        operatorInfo.longName.c_str(), operatorInfo.shortName.c_str(), operatorInfo.numeric.c_str());
    hdiId_ = HdiId::HREQ_NETWORK_GET_OPERATOR_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetNetworkSearchInformationResponse(
    const RilRadioResponseInfo &responseInfo, const AvailableNetworkList &availableNetworkList)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::GetNetworkSearchInformationResponse itemNum:%{public}d", availableNetworkList.itemNum);
    for (auto availableInfo : availableNetworkList.availableNetworkInfo) {
        TELEPHONY_LOGI("status:%{public}d", availableInfo.status);
        TELEPHONY_LOGI("numeric:%{public}s", availableInfo.numeric.c_str());
        TELEPHONY_LOGI("shortName:%{public}s", availableInfo.shortName.c_str());
        TELEPHONY_LOGI("longName:%{public}s", availableInfo.longName.c_str());
        TELEPHONY_LOGI("rat:%{public}d", availableInfo.rat);
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetNetworkSelectionModeResponse(
    const RilRadioResponseInfo &responseInfo, const SetNetworkModeInfo &setNetworkModeInfo)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::GetNetworkSelectionModeResponse selectMode:%{public}d", setNetworkModeInfo.selectMode);
    hdiId_ = HdiId::HREQ_NETWORK_GET_NETWORK_SELECTION_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetNetworkSelectionModeResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetNetworkSelectionModeResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_NETWORK_SELECTION_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetNeighboringCellInfoListResponse(
    const RilRadioResponseInfo &responseInfo, const CellListNearbyInfo &cellInfoList)
{
    TELEPHONY_LOGI("RilCallbackTest::GetNeighboringCellInfoListResponse itemNum:%{public}d", cellInfoList.itemNum);
    for (auto info : cellInfoList.cellNearbyInfo) {
        TELEPHONY_LOGI("RilCallbackTest::GetNeighboringCellInfoListResponse ratType:%{public}d", info.ratType);
        switch (static_cast<RatType>(info.ratType)) {
            case RatType::NETWORK_TYPE_LTE:
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.lte.arfcn);
                TELEPHONY_LOGI("pci:%{public}d", info.serviceCells.lte.pci);
                TELEPHONY_LOGI("rsrp:%{public}d", info.serviceCells.lte.rsrp);
                TELEPHONY_LOGI("rsrq:%{public}d", info.serviceCells.lte.rsrq);
                TELEPHONY_LOGI("rxlev:%{public}d", info.serviceCells.lte.rxlev);
                break;
            case RatType::NETWORK_TYPE_GSM:
                TELEPHONY_LOGI("band:%{public}d", info.serviceCells.gsm.band);
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.gsm.arfcn);
                TELEPHONY_LOGI("bsic:%{public}d", info.serviceCells.gsm.bsic);
                TELEPHONY_LOGI("cellId:%{public}d", info.serviceCells.gsm.cellId);
                TELEPHONY_LOGI("rxlev:%{public}d", info.serviceCells.gsm.rxlev);
                TELEPHONY_LOGI("lac:%{public}d", info.serviceCells.gsm.lac);
                break;
            case RatType::NETWORK_TYPE_WCDMA:
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.wcdma.arfcn);
                TELEPHONY_LOGI("psc:%{public}d", info.serviceCells.wcdma.psc);
                TELEPHONY_LOGI("rscp:%{public}d", info.serviceCells.wcdma.rscp);
                TELEPHONY_LOGI("ecno:%{public}d", info.serviceCells.wcdma.ecno);
                break;
            case RatType::NETWORK_TYPE_NR:
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.nr.nrArfcn);
                TELEPHONY_LOGI("psc:%{public}d", info.serviceCells.nr.pci);
                TELEPHONY_LOGI("rscp:%{public}d", info.serviceCells.nr.tac);
                TELEPHONY_LOGI("ecno:%{public}d", info.serviceCells.nr.nci);
                break;
            default:
                TELEPHONY_LOGE("RilCallbackTest::GetNeighboringCellInfoListResponse invalid ratType");
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCurrentCellInfoResponse(
    const RilRadioResponseInfo &responseInfo, const CellListCurrentInfo &cellInfoList)
{
    TELEPHONY_LOGI("RilCallbackTest::GetCurrentCellInfoResponse itemNum:%{public}d", cellInfoList.itemNum);
    for (auto info : cellInfoList.cellCurrentInfo) {
        TELEPHONY_LOGI("RilCallbackTest::GetCurrentCellInfoResponse ratType:%{public}d, mcc:%{public}d, mnc:%{public}d",
            info.ratType, info.mcc, info.mnc);
        switch (static_cast<RatType>(info.ratType)) {
            case RatType::NETWORK_TYPE_LTE:
                TELEPHONY_LOGI("cellId:%{public}d", info.serviceCells.lte.cellId);
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.lte.arfcn);
                TELEPHONY_LOGI("pci:%{public}d", info.serviceCells.lte.pci);
                TELEPHONY_LOGI("rsrp:%{public}d", info.serviceCells.lte.rsrp);
                TELEPHONY_LOGI("rsrq:%{public}d", info.serviceCells.lte.rsrq);
                TELEPHONY_LOGI("rxlev:%{public}d", info.serviceCells.lte.rssi);
                break;
            case RatType::NETWORK_TYPE_GSM:
                TELEPHONY_LOGI("band:%{public}d", info.serviceCells.gsm.band);
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.gsm.arfcn);
                TELEPHONY_LOGI("bsic:%{public}d", info.serviceCells.gsm.bsic);
                TELEPHONY_LOGI("cellId:%{public}d", info.serviceCells.gsm.cellId);
                TELEPHONY_LOGI("rxlev:%{public}d", info.serviceCells.gsm.rxlev);
                TELEPHONY_LOGI("lac:%{public}d", info.serviceCells.gsm.lac);
                break;
            case RatType::NETWORK_TYPE_WCDMA:
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.wcdma.arfcn);
                TELEPHONY_LOGI("psc:%{public}d", info.serviceCells.wcdma.psc);
                TELEPHONY_LOGI("rscp:%{public}d", info.serviceCells.wcdma.rscp);
                TELEPHONY_LOGI("ecno:%{public}d", info.serviceCells.wcdma.ecno);
                break;
            case RatType::NETWORK_TYPE_NR:
                TELEPHONY_LOGI("arfcn:%{public}d", info.serviceCells.nr.nrArfcn);
                TELEPHONY_LOGI("psc:%{public}d", info.serviceCells.nr.pci);
                TELEPHONY_LOGI("rscp:%{public}d", info.serviceCells.nr.tac);
                TELEPHONY_LOGI("ecno:%{public}d", info.serviceCells.nr.nci);
                break;
            default:
                TELEPHONY_LOGE("RilCallbackTest::GetCurrentCellInfoResponse invalid ratType");
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetPreferredNetworkResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetPreferredNetworkResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_PREFERRED_NETWORK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetPreferredNetworkResponse(
    const RilRadioResponseInfo &responseInfo, const PreferredNetworkTypeInfo &preferredNetworkTypeInfo)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::GetPreferredNetworkResponse type:%{public}d", preferredNetworkTypeInfo.preferredNetworkType);
    hdiId_ = HdiId::HREQ_NETWORK_GET_PREFERRED_NETWORK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetPhysicalChannelConfigResponse(
    const RilRadioResponseInfo &responseInfo, const ChannelConfigInfoList &channelConfigInfoList)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::GetPhysicalChannelConfigResponse itemNum:%{public}d", channelConfigInfoList.itemNum);
    for (PhysicalChannelConfig phyChnlCfg : channelConfigInfoList.channelConfigInfos) {
        TELEPHONY_LOGI(
            "RilCallbackTest::GetPhysicalChannelConfigResponse cellConnStatus:%{public}d, "
            "cellBandwidthDownlinkKhz:%{public}d, cellBandwidthUplinkKhz:%{public}d, physicalCellId:%{public}d, "
            "ratType:%{public}d, freqRange:%{public}d, downlinkChannelNum:%{public}d, "
            "uplinkChannelNum:%{public}d, contextIdNum:%{public}d",
            phyChnlCfg.cellConnStatus, phyChnlCfg.cellBandwidthDownlinkKhz, phyChnlCfg.cellBandwidthUplinkKhz,
            phyChnlCfg.ratType, phyChnlCfg.freqRange, phyChnlCfg.downlinkChannelNum, phyChnlCfg.uplinkChannelNum,
            phyChnlCfg.physicalCellId, phyChnlCfg.contextIdNum);
        for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
            TELEPHONY_LOGI("contextIds[%{public}d]:%{public}d", j, phyChnlCfg.contextIds[j]);
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetLocateUpdatesResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetLocateUpdatesResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_LOCATE_UPDATES;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetNotificationFilterResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetNotificationFilterResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_NOTIFICATION_FILTER;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetDeviceStateResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetDeviceStateResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_DEVICE_STATE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

// Call
int32_t RilCallbackTest::CallEmergencyNotice(
    const RilRadioResponseInfo &responseInfo, const EmergencyInfoList &emergencyInfoList)
{
    TELEPHONY_LOGI("GetBoolResult CallEmergencyNotice callSize : %{public}d", emergencyInfoList.callSize);
    return 0;
}

int32_t RilCallbackTest::CallStateUpdated(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI(
        "GetBoolResult CallStateUpdated slotId : %{public}d, type: %{public}d", responseInfo.slotId, responseInfo.type);
    return 0;
}

int32_t RilCallbackTest::CallRingbackVoiceNotice(
    const RilRadioResponseInfo &responseInfo, const RingbackVoice &ringbackVoice)
{
    TELEPHONY_LOGI("GetBoolResult CallRingbackVoiceNotice slotId : %{public}d, ringbackVoice: %{public}d",
        responseInfo.slotId, ringbackVoice.status);
    return 0;
}

int32_t RilCallbackTest::CallSrvccStatusNotice(const RilRadioResponseInfo &responseInfo, const SrvccStatus &srvccStatus)
{
    TELEPHONY_LOGI("GetBoolResult CallSrvccStatusNotice slotId : %{public}d, srvccStatus: %{public}d",
        responseInfo.slotId, srvccStatus.status);
    return 0;
}

int32_t RilCallbackTest::CallUssdNotice(const RilRadioResponseInfo &responseInfo, const UssdNoticeInfo &ussdNoticeInfo)
{
    TELEPHONY_LOGI("GetBoolResult CallUssdNotice slotId : %{public}d, ussdNoticeInfo m: %{public}d, str: %{public}s",
        responseInfo.slotId, ussdNoticeInfo.type, ussdNoticeInfo.message.c_str());
    return 0;
}

int32_t RilCallbackTest::CallSsNotice(const RilRadioResponseInfo &responseInfo, const SsNoticeInfo &ssNoticeInfo)
{
    TELEPHONY_LOGI("GetBoolResult CallSsNotice slotId : %{public}d, result: %{public}d, serviceType: %{public}d,"
                   "requestType: %{public}d, serviceClass: %{public}d",
        responseInfo.slotId, ssNoticeInfo.result, ssNoticeInfo.serviceType, ssNoticeInfo.requestType,
        ssNoticeInfo.serviceClass);
    return 0;
}

int32_t RilCallbackTest::CallRsrvccStatusNotify(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI(
        "CallRsrvccStatusNotify slotId : %{public}d, type: %{public}d", responseInfo.slotId, responseInfo.type);
    return 0;
}

int32_t RilCallbackTest::GetEmergencyCallListResponse(
    const RilRadioResponseInfo &responseInfo, const EmergencyInfoList &emergencyInfoList)
{
    TELEPHONY_LOGI("GetBoolResult emergencyInfoList callSize : %{public}d", emergencyInfoList.callSize);
    hdiId_ = HdiId::HREQ_CALL_GET_EMERGENCY_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetEmergencyCallListResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetEmergencyCallListResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_EMERGENCY_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCallListResponse(const RilRadioResponseInfo &responseInfo, const CallInfoList &callList)
{
    TELEPHONY_LOGI("GetBoolResult CallInfoList callSize : %{public}d", callList.callSize);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::DialResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult DialResponse");
    hdiId_ = HdiId::HREQ_CALL_DIAL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::HangupResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult HangupResponse");
    hdiId_ = HdiId::HREQ_CALL_HANGUP;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::RejectResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult RejectResponse");
    hdiId_ = HdiId::HREQ_CALL_REJECT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::AnswerResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult AnswerResponse");
    hdiId_ = HdiId::HREQ_CALL_ANSWER;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::HoldCallResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult HoldCallResponse");
    hdiId_ = HdiId::HREQ_CALL_HOLD_CALL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::UnHoldCallResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult UnHoldCallResponse");
    hdiId_ = HdiId::HREQ_CALL_UNHOLD_CALL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SwitchCallResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SwitchCallResponse");
    hdiId_ = HdiId::HREQ_CALL_SWITCH_CALL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetClipResponse(const RilRadioResponseInfo &responseInfo, const GetClipResult &getClipResult)
{
    TELEPHONY_LOGI("GetBoolResult result: %{public}d, action: %{public}d, clipStat: %{public}d", getClipResult.result,
        getClipResult.action, getClipResult.clipStat);
    hdiId_ = HdiId::HREQ_CALL_GET_CLIP;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetClipResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetClipResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CLIP;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::CombineConferenceResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult CombineConferenceResponse");
    hdiId_ = HdiId::HREQ_CALL_COMBINE_CONFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SeparateConferenceResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SeparateConferenceResponse");
    hdiId_ = HdiId::HREQ_CALL_SEPARATE_CONFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::CallSupplementResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult CallSupplementResponse");
    hdiId_ = HdiId::HREQ_CALL_CALL_SUPPLEMENT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCallWaitingResponse(
    const RilRadioResponseInfo &responseInfo, const CallWaitResult &callWaitResult)
{
    TELEPHONY_LOGI("GetBoolResult GetCallWaitingResponse result: %{public}d, status: %{public}d, classCw: %{public}d",
        callWaitResult.result, callWaitResult.status, callWaitResult.classCw);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_WAITING;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetCallWaitingResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetCallWaitingResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_WAITING;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCallTransferInfoResponse(
    const RilRadioResponseInfo &responseInfo, const CallForwardQueryInfoList &cFQueryList)
{
    TELEPHONY_LOGI("GetBoolResult GetCallTransferInfoResponse cFQueryList: %{public}d", cFQueryList.callSize);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_TRANSFER_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetCallTransferInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetCallTransferInfoResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_TRANSFER_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCallRestrictionResponse(
    const RilRadioResponseInfo &responseInfo, const CallRestrictionResult &result)
{
    TELEPHONY_LOGI("GetBoolResult result: %{public}d, status: %{public}d, classCw: %{public}d", result.result,
        result.status, result.classCw);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_RESTRICTION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetCallRestrictionResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetCallRestrictionResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_RESTRICTION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetClirResponse(const RilRadioResponseInfo &responseInfo, const GetClirResult &getClirResult)
{
    TELEPHONY_LOGI("GetBoolResult result: %{public}d, action: %{public}d, clirStat: %{public}d", getClirResult.result,
        getClirResult.action, getClirResult.clirStat);
    hdiId_ = HdiId::HREQ_CALL_GET_CLIR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetClirResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetClirResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CLIR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::StartDtmfResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult StartDtmfResponse");
    hdiId_ = HdiId::HREQ_CALL_START_DTMF;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SendDtmfResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SendDtmfResponse");
    hdiId_ = HdiId::HREQ_CALL_SEND_DTMF;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::StopDtmfResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult StopDtmfResponse");
    hdiId_ = HdiId::HREQ_CALL_STOP_DTMF;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCallPreferenceModeResponse(const RilRadioResponseInfo &responseInfo, int32_t mode)
{
    TELEPHONY_LOGI("GetBoolResult GetCallPreferenceModeResponse mode: %{public}d", mode);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_PREFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetCallPreferenceModeResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetCallPreferenceModeResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_PREFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetUssdResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetUssdResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_USSD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetUssdResponse(const RilRadioResponseInfo &responseInfo, int32_t cusd)
{
    TELEPHONY_LOGI("GetBoolResult GetUssdResponse cusd: %{public}d", cusd);
    hdiId_ = HdiId::HREQ_CALL_GET_USSD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetMuteResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetMuteResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_MUTE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetMuteResponse(const RilRadioResponseInfo &responseInfo, int32_t mute)
{
    TELEPHONY_LOGI("GetBoolResult GetMuteResponse mute: %{public}d", mute);
    hdiId_ = HdiId::HREQ_CALL_GET_MUTE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCallFailReasonResponse(const RilRadioResponseInfo &responseInfo, int32_t callFail)
{
    TELEPHONY_LOGI("GetBoolResult GetCallFailReasonResponse callFail: %{public}d", callFail);
    hdiId_ = HdiId::HREQ_CALL_GET_FAIL_REASON;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetBarringPasswordResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("GetBoolResult SetBarringPasswordResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_BARRING_PASSWORD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

/**
 * modem
 **/
int32_t RilCallbackTest::RadioStateUpdated(const RilRadioResponseInfo &responseInfo, int32_t state)
{
    TELEPHONY_LOGI("RadioStateUpdated state : %{public}d", state);
    return 0;
}

int32_t RilCallbackTest::VoiceRadioTechUpdated(
    const RilRadioResponseInfo &responseInfo, const VoiceRadioTechnology &voiceRadioTechnology)
{
    TELEPHONY_LOGI(
        "GetVoiceRadioTechnologyResponse srvStatus : %{public}d srvDomain : %{public}d roamStatus: %{public}d "
        "simStatus : %{public}d lockStatus : %{public}d sysMode : %{public}d actType : %{public}d sysModeName : "
        "%{public}s actName : %{public}s",
        voiceRadioTechnology.srvStatus, voiceRadioTechnology.srvDomain, voiceRadioTechnology.roamStatus,
        voiceRadioTechnology.simStatus, voiceRadioTechnology.lockStatus, voiceRadioTechnology.sysMode,
        voiceRadioTechnology.actType, voiceRadioTechnology.sysModeName.c_str(), voiceRadioTechnology.actName.c_str());
    return 0;
}

int32_t RilCallbackTest::ShutDownResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("ShutDownResponse");
    hdiId_ = HdiId::HREQ_MODEM_SHUT_DOWN;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetRadioStateResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("SetRadioStateResponse");
    hdiId_ = HdiId::HREQ_MODEM_SET_RADIO_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetRadioStateResponse(const RilRadioResponseInfo &responseInfo, int32_t state)
{
    TELEPHONY_LOGI("GetRadioStateResponse state : %{public}d", state);
    hdiId_ = HdiId::HREQ_MODEM_GET_RADIO_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetImeiResponse(const RilRadioResponseInfo &responseInfo, const std::string &imei)
{
    TELEPHONY_LOGI("GetImeiResponse imei : %{public}s", imei.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_IMEI;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetMeidResponse(const RilRadioResponseInfo &responseInfo, const std::string &meid)
{
    TELEPHONY_LOGI("GetMeidResponse meid : %{public}s", meid.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_MEID;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetVoiceRadioTechnologyResponse(
    const RilRadioResponseInfo &responseInfo, const VoiceRadioTechnology &voiceRadioTechnology)
{
    TELEPHONY_LOGI(
        "GetVoiceRadioTechnologyResponse srvStatus : %{public}d srvDomain : %{public}d roamStatus: %{public}d "
        "simStatus : %{public}d lockStatus : %{public}d sysMode : %{public}d actType : %{public}d sysModeName : "
        "%{public}s actName : %{public}s",
        voiceRadioTechnology.srvStatus, voiceRadioTechnology.srvDomain, voiceRadioTechnology.roamStatus,
        voiceRadioTechnology.simStatus, voiceRadioTechnology.lockStatus, voiceRadioTechnology.sysMode,
        voiceRadioTechnology.actType, voiceRadioTechnology.sysModeName.c_str(), voiceRadioTechnology.actName.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_VOICE_RADIO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetBasebandVersionResponse(
    const RilRadioResponseInfo &responseInfo, const std::string &basebandVersion)
{
    TELEPHONY_LOGI("GetBasebandVersionResponse basebandVersion : %{public}s", basebandVersion.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_BASEBAND_VERSION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

// Data
int32_t RilCallbackTest::PdpContextListUpdated(
    const RilRadioResponseInfo &responseInfo, const DataCallResultList &dataCallResultList)
{
    TELEPHONY_LOGI("RilCallbackTest::PdpContextListUpdated size:%{public}d", dataCallResultList.size);
    for (auto &setupDataCallResultInfo : dataCallResultList.dcList) {
        TELEPHONY_LOGI(
            "RilCallbackTest::PdpContextListUpdated flag:%{public}d reason:%{public}d retryTime:%{public}d "
            "cid:%{public}d active:%{public}d type:%{public}s netPortName:%{public}s address:%{public}s "
            "dns:%{public}s dnsSec:%{public}s gateway:%{public}s maxTransferUnit:%{public}d pCscfPrimAddr:%{public}s "
            "pCscfSecAddr:%{public}s pduSessionId:%{public}d",
            setupDataCallResultInfo.flag, setupDataCallResultInfo.reason, setupDataCallResultInfo.retryTime,
            setupDataCallResultInfo.cid, setupDataCallResultInfo.active, setupDataCallResultInfo.type.c_str(),
            setupDataCallResultInfo.netPortName.c_str(), setupDataCallResultInfo.address.c_str(),
            setupDataCallResultInfo.dns.c_str(), setupDataCallResultInfo.dnsSec.c_str(),
            setupDataCallResultInfo.gateway.c_str(), setupDataCallResultInfo.maxTransferUnit,
            setupDataCallResultInfo.pCscfPrimAddr.c_str(), setupDataCallResultInfo.pCscfSecAddr.c_str(),
            setupDataCallResultInfo.pduSessionId);
    }
    return 0;
}

int32_t RilCallbackTest::ActivatePdpContextResponse(
    const RilRadioResponseInfo &responseInfo, const SetupDataCallResultInfo &setupDataCallResultInfo)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::ActivatePdpContextResponse flag:%{public}d reason:%{public}d retryTime:%{public}d "
        "cid:%{public}d active:%{public}d type:%{public}s netPortName:%{public}s address:%{public}s "
        "dns:%{public}s dnsSec:%{public}s gateway:%{public}s maxTransferUnit:%{public}d pCscfPrimAddr:%{public}s "
        "pCscfSecAddr:%{public}s pduSessionId:%{public}d",
        setupDataCallResultInfo.flag, setupDataCallResultInfo.reason, setupDataCallResultInfo.retryTime,
        setupDataCallResultInfo.cid, setupDataCallResultInfo.active, setupDataCallResultInfo.type.c_str(),
        setupDataCallResultInfo.netPortName.c_str(), setupDataCallResultInfo.address.c_str(),
        setupDataCallResultInfo.dns.c_str(), setupDataCallResultInfo.dnsSec.c_str(),
        setupDataCallResultInfo.gateway.c_str(), setupDataCallResultInfo.maxTransferUnit,
        setupDataCallResultInfo.pCscfPrimAddr.c_str(), setupDataCallResultInfo.pCscfSecAddr.c_str(),
        setupDataCallResultInfo.pduSessionId);
    hdiId_ = HdiId::HREQ_DATA_ACTIVATE_PDP_CONTEXT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::DeactivatePdpContextResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::DeactivatePdpContextResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_DEACTIVATE_PDP_CONTEXT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetPdpContextListResponse(
    const RilRadioResponseInfo &responseInfo, const DataCallResultList &dataCallResultList)
{
    TELEPHONY_LOGI("RilCallbackTest::GetPdpContextListResponse size:%{public}d", dataCallResultList.size);
    for (auto &setupDataCallResultInfo : dataCallResultList.dcList) {
        TELEPHONY_LOGI(
            "RilCallbackTest::GetPdpContextListResponse flag:%{public}d reason:%{public}d retryTime:%{public}d "
            "cid:%{public}d active:%{public}d type:%{public}s netPortName:%{public}s address:%{public}s "
            "dns:%{public}s dnsSec:%{public}s gateway:%{public}s maxTransferUnit:%{public}d pCscfPrimAddr:%{public}s "
            "pCscfSecAddr:%{public}s pduSessionId:%{public}d",
            setupDataCallResultInfo.flag, setupDataCallResultInfo.reason, setupDataCallResultInfo.retryTime,
            setupDataCallResultInfo.cid, setupDataCallResultInfo.active, setupDataCallResultInfo.type.c_str(),
            setupDataCallResultInfo.netPortName.c_str(), setupDataCallResultInfo.address.c_str(),
            setupDataCallResultInfo.dns.c_str(), setupDataCallResultInfo.dnsSec.c_str(),
            setupDataCallResultInfo.gateway.c_str(), setupDataCallResultInfo.maxTransferUnit,
            setupDataCallResultInfo.pCscfPrimAddr.c_str(), setupDataCallResultInfo.pCscfSecAddr.c_str(),
            setupDataCallResultInfo.pduSessionId);
    }
    hdiId_ = HdiId::HREQ_DATA_GET_PDP_CONTEXT_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetInitApnInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetInitApnInfoResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_INIT_APN_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetLinkBandwidthReportingRuleResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetLinkBandwidthReportingRuleResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetLinkBandwidthInfoResponse(
    const RilRadioResponseInfo &responseInfo, const DataLinkBandwidthInfo &dataLinkBandwidthInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::GetLinkBandwidthInfoResponse serial:%{public}d cid:%{public}d qi:%{public}d "
                   "dlGfbr:%{public}d "
                   "ulGfbr:%{public}d dlMfbr:%{public}d ulMfbr:%{public}d ulSambr:%{public}d dlSambr:%{public}d "
                   "averagingWindow:%{public}d",
        dataLinkBandwidthInfo.serial, dataLinkBandwidthInfo.cid, dataLinkBandwidthInfo.qi, dataLinkBandwidthInfo.dlGfbr,
        dataLinkBandwidthInfo.ulGfbr, dataLinkBandwidthInfo.dlMfbr, dataLinkBandwidthInfo.ulMfbr,
        dataLinkBandwidthInfo.ulSambr, dataLinkBandwidthInfo.dlSambr, dataLinkBandwidthInfo.averagingWindow);
    hdiId_ = HdiId::HREQ_DATA_GET_LINK_BANDWIDTH_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetDataPermittedResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetDataPermittedResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_DATA_PERMITTED;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetDataProfileInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetDataProfileInfoResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_DATA_PROFILE_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

// Sms
int32_t RilCallbackTest::NewSmsNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const SmsMessageInfo &smsMessageInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::NewSmsNotify smsMessageInfo size : %{public}d", smsMessageInfo.size);
    return 0;
}

int32_t RilCallbackTest::NewCdmaSmsNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const SmsMessageInfo &smsMessageInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::NewCdmaSmsNotify smsMessageInfo size : %{public}d", smsMessageInfo.size);
    return 0;
}

int32_t RilCallbackTest::SmsStatusReportNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const SmsMessageInfo &smsMessageInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SmsStatusReportNotify smsMessageInfo size : %{public}d", smsMessageInfo.size);
    return 0;
}

int32_t RilCallbackTest::NewSmsStoredOnSimNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, int32_t recordNumber, int32_t indicationType)
{
    TELEPHONY_LOGI("RilCallbackTest::NewSmsStoredOnSimNotify recordNumber : %{public}d, indicationType : %{public}d",
        recordNumber, indicationType);
    return 0;
}

int32_t RilCallbackTest::CBConfigNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const CBConfigReportInfo &cellBroadConfigReportInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::CBConfigNotify cellBroadConfigReportInfo dcs : %{public}s",
        cellBroadConfigReportInfo.dcs.c_str());
    return 0;
}

int32_t RilCallbackTest::SendGsmSmsResponse(
    const RilRadioResponseInfo &responseInfo, const SendSmsResultInfo &sendSmsResultInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SendGsmSmsResponse sendSmsResultInfo pdu : %{public}s, error : %{public}d",
        sendSmsResultInfo.pdu.c_str(), sendSmsResultInfo.errCode);
    hdiId_ = HdiId::HREQ_SMS_SEND_GSM_SMS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SendCdmaSmsResponse(
    const RilRadioResponseInfo &responseInfo, const SendSmsResultInfo &sendSmsResultInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SendCdmaSmsResponse sendSmsResultInfo pdu : %{public}s, error : %{public}d",
        sendSmsResultInfo.pdu.c_str(), sendSmsResultInfo.errCode);
    hdiId_ = HdiId::HREQ_SMS_SEND_CDMA_SMS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::AddSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::AddSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_ADD_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::DelSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::DelSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_DEL_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::UpdateSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::UpdateSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_UPDATE_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::AddCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::AddCdmaSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_ADD_CDMA_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::DelCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::DelCdmaSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_DEL_CDMA_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::UpdateCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::UpdateCdmaSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetSmscAddrResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetSmscAddrResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SET_SMSC_ADDR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetSmscAddrResponse(
    const RilRadioResponseInfo &responseInfo, const ServiceCenterAddress &serviceCenterAddress)
{
    TELEPHONY_LOGI("RilCallbackTest::GetSmscAddrResponse serviceCenterAddress tosca : %{public}d, address : %{public}s",
        serviceCenterAddress.tosca, serviceCenterAddress.address.c_str());
    smscAddr_ = serviceCenterAddress.address;
    hdiId_ = HdiId::HREQ_SMS_GET_SMSC_ADDR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetCBConfigResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetCBConfigResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SET_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCBConfigResponse(
    const RilRadioResponseInfo &responseInfo, const CBConfigInfo &cellBroadcastInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::GetCBConfigResponse cellBroadcastInfo mids : %{public}s, dcss: %{public}s",
        cellBroadcastInfo.mids.c_str(), cellBroadcastInfo.dcss.c_str());
    hdiId_ = HdiId::HREQ_SMS_GET_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SetCdmaCBConfigResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SetCdmaCBConfigResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SET_CDMA_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::GetCdmaCBConfigResponse(
    const RilRadioResponseInfo &responseInfo, const CdmaCBConfigInfo &cdmaCBConfigInfo)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::GetCdmaCBConfigResponse cdmaCBConfigInfo service : %{public}d, language : %{public}d, "
        "checked: %{public}d",
        cdmaCBConfigInfo.service, cdmaCBConfigInfo.language, cdmaCBConfigInfo.checked);
    hdiId_ = HdiId::HREQ_SMS_GET_CDMA_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SendSmsMoreModeResponse(
    const RilRadioResponseInfo &responseInfo, const SendSmsResultInfo &sendSmsResultInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SendSmsMoreModeResponse sendSmsResultInfo pdu : %{public}s, error : %{public}d",
        sendSmsResultInfo.pdu.c_str(), sendSmsResultInfo.errCode);
    hdiId_ = HdiId::HREQ_SMS_SEND_SMS_MORE_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::SendSmsAckResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI("RilCallbackTest::SendSmsAckResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SEND_SMS_ACK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallbackTest::CommonErrorResponse(const RilRadioResponseInfo &responseInfo)
{
    TELEPHONY_LOGI(
        "RilCallbackTest::CommonErrorResponse type:%{public}d error:%{public}d", responseInfo.type, responseInfo.error);
    return 0;
}
} // namespace Telephony
} // namespace OHOS
