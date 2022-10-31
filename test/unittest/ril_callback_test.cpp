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
using namespace std;

enum class RatType {
    NETWORK_TYPE_UNKNOWN = 0,
    NETWORK_TYPE_GSM,
    NETWORK_TYPE_CDMA,
    NETWORK_TYPE_WCDMA,
    NETWORK_TYPE_TDSCDMA,
    NETWORK_TYPE_LTE,
    NETWORK_TYPE_NR
};

void RilCallbackTest::PrintResponseInfo(const std::string &method, const RilRadioResponseInfo &responseInfo)
{
    cout << "-------------" << method << "-------------" << endl
         << "[responseInfo] ----> [slotId]: " << responseInfo.slotId << " ----> [serial]: " << responseInfo.serial
         << " ----> [error]: " << static_cast<int>(responseInfo.error) << endl;
}

// SIM
int32_t RilCallbackTest::SimStateUpdated(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SimStateUpdated", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SimStkSessionEndNotify(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SimStkSessionEndNotify", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SimStkProactiveNotify(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    PrintResponseInfo("SimStkProactiveNotify", responseInfo);
    cout << "[response]: " << response;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SimStkAlphaNotify(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    PrintResponseInfo("SimStkAlphaNotify", responseInfo);
    cout << "[response]: " << response;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SimStkEventNotify(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    PrintResponseInfo("SimStkEventNotify", responseInfo);
    cout << "[response]: " << response;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SimStkCallSetupNotify(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SimStkCallSetupNotify", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SimRefreshNotify(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SimRefreshNotify", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetSimStatusResponse(const RilRadioResponseInfo &responseInfo, const CardStatusInfo &result)
{
    PrintResponseInfo("GetSimStatusResponse", responseInfo);
    cout << "[CardStatusInfo] -->[index] : " << result.index << " -->[simType] : " << result.simType
         << " -->[simState] : " << result.simState;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::GetSimIOResponse(const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    PrintResponseInfo("GetSimIOResponse", responseInfo);
    cout << "[IccIoResultInfo] -->[sw1] : " << result.sw1 << " -->[sw2] : " << result.sw2
         << " -->[response] : " << result.response;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::GetImsiResponse(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    PrintResponseInfo("GetImsiResponse", responseInfo);
    cout << "[response]: " << response;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::GetSimLockStatusResponse(const RilRadioResponseInfo &responseInfo, int32_t simLockStatus)
{
    PrintResponseInfo("GetSimLockStatusResponse", responseInfo);
    cout << "[simLockStatus]: " << simLockStatus;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SetSimLockResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    PrintResponseInfo("SetSimLockResponse", responseInfo);
    cout << "[LockStatusResp] -->[result] : " << lockStatus.result << " -->[remain] : " << lockStatus.remain;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::ChangeSimPasswordResponse(
    const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    PrintResponseInfo("ChangeSimPasswordResponse", responseInfo);
    cout << "[LockStatusResp] -->[result] : " << lockStatus.result << " -->[remain] : " << lockStatus.remain;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::UnlockPinResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    PrintResponseInfo("UnlockPinResponse", responseInfo);
    cout << "[LockStatusResp] -->[result] : " << lockStatus.result << " -->[remain] : " << lockStatus.remain;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::UnlockPukResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    PrintResponseInfo("UnlockPukResponse", responseInfo);
    cout << "[LockStatusResp] -->[result] : " << lockStatus.result << " -->[remain] : " << lockStatus.remain;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::UnlockPin2Response(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    PrintResponseInfo("UnlockPin2Response", responseInfo);
    cout << "[LockStatusResp] -->[result] : " << lockStatus.result << " -->[remain] : " << lockStatus.remain;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::UnlockPuk2Response(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    PrintResponseInfo("UnlockPuk2Response", responseInfo);
    cout << "[LockStatusResp] -->[result] : " << lockStatus.result << " -->[remain] : " << lockStatus.remain;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SetActiveSimResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetActiveSimResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SimStkSendTerminalResponseResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SimStkSendTerminalResponseResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SimStkSendEnvelopeResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SimStkSendEnvelopeResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SimStkSendCallSetupRequestResultResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SimStkSendCallSetupRequestResultResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SimStkIsReadyResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SimStkIsReadyResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SetRadioProtocolResponse(
    const RilRadioResponseInfo &responseInfo, const RadioProtocol &radioProtocol)
{
    PrintResponseInfo("SetRadioProtocolResponse", responseInfo);
    cout << "[RadioProtocol] -->[slotId] : " << radioProtocol.slotId << " -->[sessionId] : " << radioProtocol.sessionId
         << " -->[phase] : " << radioProtocol.phase << " -->[technology] : " << radioProtocol.technology
         << " -->[modemId] : " << radioProtocol.modemId << " -->[status] : " << radioProtocol.status;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SimOpenLogicalChannelResponse(
    const RilRadioResponseInfo &responseInfo, const OpenLogicalChannelResponse &pOpenLogicalChannelResponse)
{
    PrintResponseInfo("SimOpenLogicalChannelResponse", responseInfo);
    cout << "[OpenLogicalChannelResponse] -->[sw1] : " << pOpenLogicalChannelResponse.sw1
         << " -->[sw2] : " << pOpenLogicalChannelResponse.sw2
         << " -->[channelId] : " << pOpenLogicalChannelResponse.channelId
         << " -->[response] : " << pOpenLogicalChannelResponse.response;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SimCloseLogicalChannelResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SimCloseLogicalChannelResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SimTransmitApduLogicalChannelResponse(
    const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    PrintResponseInfo("SimTransmitApduLogicalChannelResponse", responseInfo);
    cout << "[IccIoResultInfo] -->[sw1] : " << result.sw1 << " -->[sw2] : " << result.sw2
         << " -->[channelId] : " << result.response;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SimTransmitApduBasicChannelResponse(
    const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    PrintResponseInfo("SimTransmitApduBasicChannelResponse", responseInfo);
    cout << "[IccIoResultInfo] -->[sw1] : " << result.sw1 << " -->[sw2] : " << result.sw2
         << " -->[channelId] : " << result.response;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SimAuthenticationResponse(
    const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    PrintResponseInfo("SimAuthenticationResponse", responseInfo);
    cout << "[IccIoResultInfo] -->[sw1] : " << result.sw1 << " -->[sw2] : " << result.sw2
         << " -->[channelId] : " << result.response;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::UnlockSimLockResponse(
    const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    PrintResponseInfo("UnlockPuk2Response", responseInfo);
    cout << "[LockStatusResp] -->[result] : " << lockStatus.result << " -->[remain] : " << lockStatus.remain;
    cout << endl << endl;
    return 0;
}

// Network
int32_t RilCallbackTest::NetworkCsRegStatusUpdated(
    const RilRadioResponseInfo &responseInfo, const CsRegStatusInfo &csRegStatusInfo)
{
    PrintResponseInfo("NetworkCsRegStatusUpdated", responseInfo);
    cout << "[csRegStatusInfo] -->[notifyType] : " << csRegStatusInfo.notifyType << endl;
    cout << "[csRegStatusInfo] -->[regStatus] : " << csRegStatusInfo.regStatus << endl;
    cout << "[csRegStatusInfo] -->[radioTechnology] : " << csRegStatusInfo.radioTechnology << endl;
    cout << "[csRegStatusInfo] -->[flag] : " << csRegStatusInfo.flag << endl;
    cout << "NetworkCsRegStatusUpdated finish." << endl;

    return 0;
}

int32_t RilCallbackTest::NetworkPsRegStatusUpdated(
    const RilRadioResponseInfo &responseInfo, const PsRegStatusInfo &psRegStatusInfo)
{
    PrintResponseInfo("NetworkPsRegStatusUpdated", responseInfo);
    cout << "[psRegStatusInfo] -->[notifyType] : " << psRegStatusInfo.notifyType << endl;
    cout << "[psRegStatusInfo] -->[regStatus] : " << psRegStatusInfo.regStatus << endl;
    cout << "[psRegStatusInfo] -->[radioTechnology] : " << psRegStatusInfo.radioTechnology << endl;
    cout << "[psRegStatusInfo] -->[isDcNrRestricted] : " << psRegStatusInfo.isDcNrRestricted << endl;
    cout << "[psRegStatusInfo] -->[isNrAvailable] : " << psRegStatusInfo.isNrAvailable << endl;
    cout << "[psRegStatusInfo] -->[isEnDcAvailable] : " << psRegStatusInfo.isEnDcAvailable << endl;
    cout << "NetworkPsRegStatusUpdated finish." << endl;

    return 0;
}

int32_t RilCallbackTest::SignalStrengthUpdated(const RilRadioResponseInfo &responseInfo, const Rssi &rssi)
{
    PrintResponseInfo("SignalStrengthUpdated", responseInfo);
    cout << "[rssi] -->[lte] -->[rxlev] : " << rssi.lte.rxlev << endl;
    cout << "[rssi] -->[lte] -->[rsrq] : " << rssi.lte.rsrq << endl;
    cout << "[rssi] -->[lte] -->[rsrp] : " << rssi.lte.rsrp << endl;
    cout << "[rssi] -->[lte] -->[snr] : " << rssi.lte.snr << endl;
    cout << "SignalStrengthUpdated finish." << endl;

    return 0;
}

int32_t RilCallbackTest::NetworkTimeZoneUpdated(
    const RilRadioResponseInfo &responseInfo, const std::string &timeZoneStr)
{
    PrintResponseInfo("NetworkTimeZoneUpdated", responseInfo);
    cout << "NetworkTimeZoneUpdated timeZoneStr : " << timeZoneStr << endl;
    cout << "NetworkTimeZoneUpdated finish." << endl;

    return 0;
}

int32_t RilCallbackTest::NetworkTimeUpdated(const RilRadioResponseInfo &responseInfo, const std::string &timeStr)
{
    PrintResponseInfo("NetworkTimeUpdated", responseInfo);
    cout << "NetworkTimeUpdated timeStr : " << timeStr << endl;
    cout << "NetworkTimeUpdated finish." << endl;

    return 0;
}

int32_t RilCallbackTest::NetworkPhyChnlCfgUpdated(
    const RilRadioResponseInfo &responseInfo, const ChannelConfigInfoList &channelConfigInfoList)
{
    PrintResponseInfo("NetworkPhyChnlCfgUpdated", responseInfo);

    cout << "[channelConfigInfoList] -->[itemNum] : " << channelConfigInfoList.itemNum << endl;
    cout << "[channelConfigInfoList] -->[flag] : " << channelConfigInfoList.flag << endl;

    for (auto phyChnlCfg : channelConfigInfoList.channelConfigInfos) {
        cout << "[phyChnlCfg] -->[cellConnStatus] : " << phyChnlCfg.cellConnStatus << endl;
        cout << "[phyChnlCfg] -->[ratType] : " << phyChnlCfg.ratType << endl;
        cout << "[phyChnlCfg] -->[cellBandwidthDownlinkKhz] : " << phyChnlCfg.cellBandwidthDownlinkKhz << endl;
        cout << "[phyChnlCfg] -->[cellBandwidthUplinkKhz] : " << phyChnlCfg.cellBandwidthUplinkKhz << endl;
        cout << "[phyChnlCfg] -->[freqRange] : " << phyChnlCfg.freqRange << endl;
        cout << "[phyChnlCfg] -->[downlinkChannelNum] : " << phyChnlCfg.downlinkChannelNum << endl;
        cout << "[phyChnlCfg] -->[uplinkChannelNum] : " << phyChnlCfg.uplinkChannelNum << endl;
        cout << "[phyChnlCfg] -->[contextIdNum] : " << phyChnlCfg.contextIdNum << endl;

        for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
            cout << "[phyChnlCfg] -->[contextIds] : " << j << " " << phyChnlCfg.contextIds[j] << endl;
        }
    }

    return 0;
}

int32_t RilCallbackTest::NetworkCurrentCellUpdated(
    const RilRadioResponseInfo &responseInfo, const CellListCurrentInfo &cellListCurrentInfo)
{
    PrintResponseInfo("NetworkCurrentCellUpdated", responseInfo);

    cout << "[cellListCurrentInfo] -->[itemNum] : " << cellListCurrentInfo.itemNum << endl;
    for (auto cellInfo : cellListCurrentInfo.cellCurrentInfo) {
        cout << "[cellInfo] -->[ratType] : " << cellInfo.ratType << endl;
        cout << "[cellInfo] -->[mcc] : " << cellInfo.mcc << endl;
        cout << "[cellInfo] -->[mnc] : " << cellInfo.mnc << endl;
        switch (static_cast<RatType>(cellInfo.ratType)) {
            case RatType::NETWORK_TYPE_LTE:
                cout << "[cellInfo] -->[arfcn] : " << cellInfo.serviceCells.lte.arfcn << endl;
                cout << "[cellInfo] -->[rsrp] : " << cellInfo.serviceCells.lte.rsrp << endl;
                cout << "[cellInfo] -->[rsrq] : " << cellInfo.serviceCells.lte.rsrq << endl;
                cout << "[cellInfo] -->[rssi] : " << cellInfo.serviceCells.lte.rssi << endl;
                break;
            case RatType::NETWORK_TYPE_GSM:
                cout << "[cellInfo] -->[band] : " << cellInfo.serviceCells.gsm.band << endl;
                cout << "[cellInfo] -->[arfcn] : " << cellInfo.serviceCells.gsm.arfcn << endl;
                cout << "[cellInfo] -->[rxlev] : " << cellInfo.serviceCells.gsm.rxlev << endl;
                cout << "[cellInfo] -->[rxQuality] : " << cellInfo.serviceCells.gsm.rxQuality << endl;
                break;
            case RatType::NETWORK_TYPE_WCDMA:
                cout << "[cellInfo] -->[arfcn] : " << cellInfo.serviceCells.wcdma.arfcn << endl;
                cout << "[cellInfo] -->[rscp] : " << cellInfo.serviceCells.wcdma.rscp << endl;
                cout << "[cellInfo] -->[rxlev] : " << cellInfo.serviceCells.wcdma.rxlev << endl;
                cout << "[cellInfo] -->[ecno] : " << cellInfo.serviceCells.wcdma.ecno << endl;
                break;
            case RatType::NETWORK_TYPE_NR:
                cout << "[cellInfo] -->[nrArfcn] : " << cellInfo.serviceCells.nr.nrArfcn << endl;
                cout << "[cellInfo] -->[tac] : " << cellInfo.serviceCells.nr.tac << endl;
                break;
            default:
                cout << "NetworkCurrentCellUpdated fail, invalid ratType" << endl;
                break;
        }
    }

    return 0;
}

int32_t RilCallbackTest::GetSignalStrengthResponse(const RilRadioResponseInfo &responseInfo, const Rssi &rssi)
{
    PrintResponseInfo("GetSignalStrengthResponse", responseInfo);
    cout << "[rssi] -->[lte] -->[rxlev] : " << rssi.lte.rxlev << endl;
    cout << "[rssi] -->[lte] -->[rsrq] : " << rssi.lte.rsrq << endl;
    cout << "[rssi] -->[lte] -->[rsrp] : " << rssi.lte.rsrp << endl;
    cout << "[rssi] -->[lte] -->[snr] : " << rssi.lte.snr << endl;

    return 0;
}

int32_t RilCallbackTest::GetCsRegStatusResponse(
    const RilRadioResponseInfo &responseInfo, const CsRegStatusInfo &csRegStatusInfo)
{
    PrintResponseInfo("GetCsRegStatusResponse", responseInfo);
    cout << "[csRegStatusInfo] -->[notifyType] : " << csRegStatusInfo.notifyType << endl;
    cout << "[csRegStatusInfo] -->[regStatus] : " << csRegStatusInfo.regStatus << endl;
    cout << "[csRegStatusInfo] -->[radioTechnology] : " << csRegStatusInfo.radioTechnology << endl;
    cout << "[csRegStatusInfo] -->[flag] : " << csRegStatusInfo.flag << endl;

    return 0;
}

int32_t RilCallbackTest::GetPsRegStatusResponse(
    const RilRadioResponseInfo &responseInfo, const PsRegStatusInfo &psRegStatusInfo)
{
    PrintResponseInfo("GetPsRegStatusResponse", responseInfo);
    cout << "[psRegStatusInfo] -->[notifyType] : " << psRegStatusInfo.notifyType << endl;
    cout << "[psRegStatusInfo] -->[regStatus] : " << psRegStatusInfo.regStatus << endl;
    cout << "[psRegStatusInfo] -->[radioTechnology] : " << psRegStatusInfo.radioTechnology << endl;
    cout << "[psRegStatusInfo] -->[isDcNrRestricted] : " << psRegStatusInfo.isDcNrRestricted << endl;
    cout << "[psRegStatusInfo] -->[isNrAvailable] : " << psRegStatusInfo.isNrAvailable << endl;
    cout << "[psRegStatusInfo] -->[isEnDcAvailable] : " << psRegStatusInfo.isEnDcAvailable << endl;

    return 0;
}

int32_t RilCallbackTest::GetOperatorInfoResponse(
    const RilRadioResponseInfo &responseInfo, const OperatorInfo &operatorInfo)
{
    PrintResponseInfo("GetOperatorInfoResponse", responseInfo);
    cout << "[operatorInfo] -->[longName] : " << operatorInfo.longName << endl;
    cout << "[operatorInfo] -->[shortName] : " << operatorInfo.shortName << endl;
    cout << "[operatorInfo] -->[numeric] : " << operatorInfo.numeric << endl;

    return 0;
}

int32_t RilCallbackTest::GetNetworkSearchInformationResponse(
    const RilRadioResponseInfo &responseInfo, const AvailableNetworkList &availableNetworkList)
{
    PrintResponseInfo("GetNetworkSearchInformationResponse", responseInfo);

    cout << "[availableNetworkList] -->[itemNum] : " << availableNetworkList.itemNum << endl;
    cout << "[availableNetworkList] -->[flag] : " << availableNetworkList.flag << endl;
    for (auto availableInfo : availableNetworkList.availableNetworkInfo) {
        cout << "[availableInfo] -->[status] : " << availableInfo.status << endl;
        cout << "[availableInfo] -->[rat] : " << availableInfo.rat << endl;
        cout << "[availableInfo] -->[longName] : " << availableInfo.longName << endl;
        cout << "[availableInfo] -->[shortName] : " << availableInfo.shortName << endl;
        cout << "[availableInfo] -->[numeric] : " << availableInfo.numeric << endl;
    }

    return 0;
}

int32_t RilCallbackTest::GetNetworkSelectionModeResponse(
    const RilRadioResponseInfo &responseInfo, const SetNetworkModeInfo &setNetworkModeInfo)
{
    PrintResponseInfo("GetNetworkSelectionModeResponse", responseInfo);
    cout << "preferredNetworkType: " << setNetworkModeInfo.selectMode << endl;
    cout << "operator: " << setNetworkModeInfo.oper << endl;
    cout << "flag: " << setNetworkModeInfo.flag << endl;
    return 0;
}

int32_t RilCallbackTest::SetNetworkSelectionModeResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetNetworkSelectionModeResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetNeighboringCellInfoListResponse(
    const RilRadioResponseInfo &responseInfo, const CellListNearbyInfo &cellInfoList)
{
    PrintResponseInfo("GetNeighboringCellInfoListResponse", responseInfo);

    cout << "[cellInfoList] -->[itemNum] : " << cellInfoList.itemNum << endl;
    for (auto cellInfo : cellInfoList.cellNearbyInfo) {
        cout << "[cellInfo] -->[ratType] : " << cellInfo.ratType << endl;
        switch (static_cast<RatType>(cellInfo.ratType)) {
            case RatType::NETWORK_TYPE_LTE:
                cout << "[cellInfo] -->[arfcn] : " << cellInfo.serviceCells.lte.arfcn << endl;
                cout << "[cellInfo] -->[pci] : " << cellInfo.serviceCells.lte.pci << endl;
                cout << "[cellInfo] -->[rsrp] : " << cellInfo.serviceCells.lte.rsrp << endl;
                cout << "[cellInfo] -->[rsrq] : " << cellInfo.serviceCells.lte.rsrq << endl;
                cout << "[cellInfo] -->[rxlev] : " << cellInfo.serviceCells.lte.rxlev << endl;
                break;
            case RatType::NETWORK_TYPE_GSM:
                cout << "[cellInfo] -->[band] : " << cellInfo.serviceCells.gsm.band << endl;
                cout << "[cellInfo] -->[arfcn] : " << cellInfo.serviceCells.gsm.arfcn << endl;
                cout << "[cellInfo] -->[bsic] : " << cellInfo.serviceCells.gsm.bsic << endl;
                cout << "[cellInfo] -->[rxlev] : " << cellInfo.serviceCells.gsm.rxlev << endl;
                break;
            case RatType::NETWORK_TYPE_WCDMA:
                cout << "[cellInfo] -->[arfcn] : " << cellInfo.serviceCells.wcdma.arfcn << endl;
                cout << "[cellInfo] -->[psc] : " << cellInfo.serviceCells.wcdma.psc << endl;
                cout << "[cellInfo] -->[rscp] : " << cellInfo.serviceCells.wcdma.rscp << endl;
                cout << "[cellInfo] -->[ecno] : " << cellInfo.serviceCells.wcdma.ecno << endl;
                break;
            case RatType::NETWORK_TYPE_NR:
                cout << "[cellInfo] -->[nrArfcn] : " << cellInfo.serviceCells.nr.nrArfcn << endl;
                cout << "[cellInfo] -->[tac] : " << cellInfo.serviceCells.nr.tac << endl;
                break;
            default:
                cout << "RilCallbackTest::GetNeighboringCellInfoListResponse invalid ratType" << endl;
                break;
        }
    }

    return 0;
}

int32_t RilCallbackTest::GetCurrentCellInfoResponse(
    const RilRadioResponseInfo &responseInfo, const CellListCurrentInfo &cellInfoList)
{
    PrintResponseInfo("GetCurrentCellInfoResponse", responseInfo);

    cout << "[cellInfoList] -->[itemNum] : " << cellInfoList.itemNum << endl;
    for (auto cellInfo : cellInfoList.cellCurrentInfo) {
        cout << "[cellInfo] -->[ratType] : " << cellInfo.ratType << endl;
        cout << "[cellInfo] -->[mcc] : " << cellInfo.mcc << endl;
        cout << "[cellInfo] -->[mnc] : " << cellInfo.mnc << endl;
        switch (static_cast<RatType>(cellInfo.ratType)) {
            case RatType::NETWORK_TYPE_LTE:
                cout << "[cellInfo] -->[arfcn] : " << cellInfo.serviceCells.lte.arfcn << endl;
                cout << "[cellInfo] -->[rsrp] : " << cellInfo.serviceCells.lte.rsrp << endl;
                cout << "[cellInfo] -->[rsrq] : " << cellInfo.serviceCells.lte.rsrq << endl;
                cout << "[cellInfo] -->[rssi] : " << cellInfo.serviceCells.lte.rssi << endl;
                break;
            case RatType::NETWORK_TYPE_GSM:
                cout << "[cellInfo] -->[band] : " << cellInfo.serviceCells.gsm.band << endl;
                cout << "[cellInfo] -->[arfcn] : " << cellInfo.serviceCells.gsm.arfcn << endl;
                cout << "[cellInfo] -->[bsic] : " << cellInfo.serviceCells.gsm.bsic << endl;
                cout << "[cellInfo] -->[cellId] : " << cellInfo.serviceCells.gsm.cellId << endl;
                cout << "[cellInfo] -->[rxlev] : " << cellInfo.serviceCells.gsm.rxlev << endl;
                break;
            case RatType::NETWORK_TYPE_WCDMA:
                cout << "[cellInfo] -->[arfcn] : " << cellInfo.serviceCells.wcdma.arfcn << endl;
                cout << "[cellInfo] -->[psc] : " << cellInfo.serviceCells.wcdma.psc << endl;
                cout << "[cellInfo] -->[rscp] : " << cellInfo.serviceCells.wcdma.rscp << endl;
                cout << "[cellInfo] -->[ecno] : " << cellInfo.serviceCells.wcdma.ecno << endl;
                break;
            case RatType::NETWORK_TYPE_NR:
                cout << "[cellInfo] -->[nrArfcn] : " << cellInfo.serviceCells.nr.nrArfcn << endl;
                cout << "[cellInfo] -->[tac] : " << cellInfo.serviceCells.nr.tac << endl;
                cout << "[cellInfo] -->[nci] : " << cellInfo.serviceCells.nr.nci << endl;
                break;
            default:
                cout << "RilCallbackTest::GetCurrentCellInfoResponse invalid ratType" << endl;
                break;
        }
    }

    return 0;
}

int32_t RilCallbackTest::SetPreferredNetworkResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetPreferredNetworkResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetPreferredNetworkResponse(
    const RilRadioResponseInfo &responseInfo, const PreferredNetworkTypeInfo &preferredNetworkTypeInfo)
{
    PrintResponseInfo("GetPreferredNetworkResponse", responseInfo);
    cout << "preferredNetworkType: " << preferredNetworkTypeInfo.preferredNetworkType << endl;
    cout << "flag: " << preferredNetworkTypeInfo.flag << endl;
    return 0;
}

int32_t RilCallbackTest::GetPhysicalChannelConfigResponse(
    const RilRadioResponseInfo &responseInfo, const ChannelConfigInfoList &channelConfigInfoList)
{
    PrintResponseInfo("GetPhysicalChannelConfigResponse", responseInfo);

    cout << "[channelConfigInfoList] -->[itemNum] : " << channelConfigInfoList.itemNum << endl;
    cout << "[channelConfigInfoList] -->[flag] : " << channelConfigInfoList.flag << endl;

    for (auto phyChnlCfg : channelConfigInfoList.channelConfigInfos) {
        cout << "[phyChnlCfg] -->[cellConnStatus] : " << phyChnlCfg.cellConnStatus << endl;
        cout << "[phyChnlCfg] -->[ratType] : " << phyChnlCfg.ratType << endl;
        cout << "[phyChnlCfg] -->[cellBandwidthDownlinkKhz] : " << phyChnlCfg.cellBandwidthDownlinkKhz << endl;
        cout << "[phyChnlCfg] -->[cellBandwidthUplinkKhz] : " << phyChnlCfg.cellBandwidthUplinkKhz << endl;
        cout << "[phyChnlCfg] -->[freqRange] : " << phyChnlCfg.freqRange << endl;
        cout << "[phyChnlCfg] -->[downlinkChannelNum] : " << phyChnlCfg.downlinkChannelNum << endl;
        cout << "[phyChnlCfg] -->[uplinkChannelNum] : " << phyChnlCfg.uplinkChannelNum << endl;
        cout << "[phyChnlCfg] -->[contextIdNum] : " << phyChnlCfg.contextIdNum << endl;

        for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
            cout << "[phyChnlCfg] -->[contextIds] : " << j << " " << phyChnlCfg.contextIds[j] << endl;
        }
    }

    return 0;
}

int32_t RilCallbackTest::SetLocateUpdatesResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetLocateUpdatesResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SetNotificationFilterResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetNotificationFilterResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SetDeviceStateResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetDeviceStateResponse", responseInfo);
    return 0;
}

// Call
int32_t RilCallbackTest::CallEmergencyNotice(
    const RilRadioResponseInfo &responseInfo, const EmergencyInfoList &emergencyInfoList)
{
    PrintResponseInfo("CallEmergencyNotice", responseInfo);
    cout << "[emergencyInfoList] -->[callSize] : " << emergencyInfoList.callSize << endl;
    for (auto emergencyInfo : emergencyInfoList.calls) {
        cout << "[emergencyInfo] -->[mcc] : " << emergencyInfo.mcc << " -->[ecc] : " << emergencyInfo.eccNum << endl;
    }
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::CallStateUpdated(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("CallStateUpdated", responseInfo);
    return 0;
}

int32_t RilCallbackTest::CallRingbackVoiceNotice(
    const RilRadioResponseInfo &responseInfo, const RingbackVoice &ringbackVoice)
{
    PrintResponseInfo("CallRingbackVoiceNotice", responseInfo);
    cout << "[ringbackVoice] -->[status] : " << ringbackVoice.status << endl;
    return 0;
}

int32_t RilCallbackTest::CallSrvccStatusNotice(const RilRadioResponseInfo &responseInfo, const SrvccStatus &srvccStatus)
{
    PrintResponseInfo("CallSrvccStatusNotice", responseInfo);
    cout << "[srvccStatus] -->[status] : " << srvccStatus.status << endl;
    return 0;
}

int32_t RilCallbackTest::CallRsrvccStatusNotify(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("CallRsrvccStatusNotify", responseInfo);
    return 0;
}

int32_t RilCallbackTest::CallUssdNotice(const RilRadioResponseInfo &responseInfo, const UssdNoticeInfo &ussdNoticeInfo)
{
    PrintResponseInfo("CallUssdNotice", responseInfo);
    cout << "[ussdNoticeInfo] -->[type] : " << ussdNoticeInfo.type << endl;
    cout << "[ussdNoticeInfo] -->[message] : " << ussdNoticeInfo.message << endl;
    return 0;
}

int32_t RilCallbackTest::CallSsNotice(const RilRadioResponseInfo &responseInfo, const SsNoticeInfo &ssNoticeInfo)
{
    PrintResponseInfo("CallSsNotice", responseInfo);
    cout << "[ssNoticeInfo] -->[serviceType] : " << ssNoticeInfo.serviceType << endl;
    cout << "[ssNoticeInfo] -->[requestType] : " << ssNoticeInfo.requestType << endl;
    cout << "[ssNoticeInfo] -->[serviceClass] : " << ssNoticeInfo.serviceClass << endl;
    return 0;
}

int32_t RilCallbackTest::GetEmergencyCallListResponse(
    const RilRadioResponseInfo &responseInfo, const EmergencyInfoList &emergencyInfoList)
{
    PrintResponseInfo("GetEmergencyCallListResponse", responseInfo);
    cout << "[emergencyInfoList] -->[callSize] : " << emergencyInfoList.callSize << endl;
    for (auto emergencyInfo : emergencyInfoList.calls) {
        cout << "[emergencyInfo] -->[mcc] : " << emergencyInfo.mcc << " -->[ecc] : " << emergencyInfo.eccNum << endl;
    }
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::SetEmergencyCallListResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetEmergencyCallListResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetCallListResponse(const RilRadioResponseInfo &responseInfo, const CallInfoList &callList)
{
    PrintResponseInfo("GetCallListResponse", responseInfo);
    cout << "[CallInfoList] -->[callSize] : " << callList.callSize << endl;
    return 0;
}

int32_t RilCallbackTest::DialResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("DialResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::HangupResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("HangupResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::RejectResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("RejectResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::AnswerResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("AnswerResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::HoldCallResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("HoldCallResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::UnHoldCallResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("UnHoldCallResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SwitchCallResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SwitchCallResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetClipResponse(const RilRadioResponseInfo &responseInfo, const GetClipResult &getClipResult)
{
    PrintResponseInfo("GetClipResponse", responseInfo);
    cout << "[getClipResult] -->[result] : " << getClipResult.result << endl;
    cout << "[getClipResult] -->[action] : " << getClipResult.action << endl;
    cout << "[getClipResult] -->[clipStat] : " << getClipResult.clipStat << endl;
    return 0;
}

int32_t RilCallbackTest::SetClipResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("GetClipResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::CombineConferenceResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("CombineConferenceResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SeparateConferenceResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SeparateConferenceResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::CallSupplementResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("CallSupplementResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetCallWaitingResponse(
    const RilRadioResponseInfo &responseInfo, const CallWaitResult &callWaitResult)
{
    PrintResponseInfo("GetCallWaitingResponse", responseInfo);
    cout << "[callWaitResult] -->[result] : " << callWaitResult.result << endl;
    cout << "[callWaitResult] -->[status] : " << callWaitResult.status << endl;
    cout << "[callWaitResult] -->[classCw] : " << callWaitResult.classCw << endl;
    return 0;
}

int32_t RilCallbackTest::SetCallWaitingResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetCallWaitingResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetCallTransferInfoResponse(
    const RilRadioResponseInfo &responseInfo, const CallForwardQueryInfoList &cFQueryList)
{
    PrintResponseInfo("GetCallTransferInfoResponse", responseInfo);
    cout << "[cFQueryList] -->[callSize] : " << cFQueryList.callSize << endl;
    return 0;
}

int32_t RilCallbackTest::SetCallTransferInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetCallTransferInfoResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetCallRestrictionResponse(
    const RilRadioResponseInfo &responseInfo, const CallRestrictionResult &result)
{
    PrintResponseInfo("GetCallRestrictionResponse", responseInfo);
    cout << "[result] -->[result] : " << result.result << endl;
    cout << "[result] -->[status] : " << result.status << endl;
    cout << "[result] -->[classCw] : " << result.classCw << endl;
    return 0;
}

int32_t RilCallbackTest::SetCallRestrictionResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetCallRestrictionResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetClirResponse(const RilRadioResponseInfo &responseInfo, const GetClirResult &getClirResult)
{
    PrintResponseInfo("GetClirResponse", responseInfo);
    cout << "[getClirResult] -->[result] : " << getClirResult.result << endl;
    cout << "[getClirResult] -->[action] : " << getClirResult.action << endl;
    cout << "[getClirResult] -->[clirStat] : " << getClirResult.clirStat << endl;
    return 0;
}

int32_t RilCallbackTest::SetClirResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetClirResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::StartDtmfResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("StartDtmfResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SendDtmfResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SendDtmfResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::StopDtmfResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("StopDtmfResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetCallPreferenceModeResponse(const RilRadioResponseInfo &responseInfo, int32_t mode)
{
    PrintResponseInfo("GetCallPreferenceModeResponse", responseInfo);
    cout << "mode : " << mode << endl;
    return 0;
}

int32_t RilCallbackTest::SetCallPreferenceModeResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetCallPreferenceModeResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SetUssdResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetUssdResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetUssdResponse(const RilRadioResponseInfo &responseInfo, int32_t cusd)
{
    PrintResponseInfo("GetUssdResponse", responseInfo);
    cout << "cusd : " << cusd << endl;
    return 0;
}

int32_t RilCallbackTest::SetMuteResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetMuteResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetMuteResponse(const RilRadioResponseInfo &responseInfo, int32_t mute)
{
    PrintResponseInfo("GetUssdResponse", responseInfo);
    cout << "mute : " << mute << endl;
    return 0;
}

int32_t RilCallbackTest::GetCallFailReasonResponse(const RilRadioResponseInfo &responseInfo, int32_t callFail)
{
    PrintResponseInfo("GetCallFailReasonResponse", responseInfo);
    cout << "callFail : " << callFail << endl;
    return 0;
}

int32_t RilCallbackTest::SetBarringPasswordResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetBarringPasswordResponse", responseInfo);
    return 0;
}

/**
 * modem
 **/
int32_t RilCallbackTest::RadioStateUpdated(const RilRadioResponseInfo &responseInfo, int32_t state)
{
    PrintResponseInfo("RadioStateUpdated", responseInfo);
    cout << "[state] : " << state << endl;
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::VoiceRadioTechUpdated(
    const RilRadioResponseInfo &responseInfo, const VoiceRadioTechnology &voiceRadioTechnology)
{
    PrintResponseInfo("VoiceRadioTechUpdated", responseInfo);
    cout << "[voiceRadioTechnology] -->[srvStatus] : " << voiceRadioTechnology.srvStatus
         << " -->[srvDomain] : " << voiceRadioTechnology.srvDomain
         << " -->[roamStatus] : " << voiceRadioTechnology.roamStatus
         << " -->[simStatus] : " << voiceRadioTechnology.simStatus
         << " -->[lockStatus] : " << voiceRadioTechnology.lockStatus
         << " -->[sysModeName] : " << voiceRadioTechnology.sysModeName
         << " -->[sysMode] : " << voiceRadioTechnology.sysMode
         << " -->[actName] : " << voiceRadioTechnology.actName
         << " -->[actType] : " << voiceRadioTechnology.actType;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::ShutDownResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("ShutDownResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SetRadioStateResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetRadioStateResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetRadioStateResponse(const RilRadioResponseInfo &responseInfo, int32_t state)
{
    PrintResponseInfo("GetRadioStateResponse", responseInfo);
    cout << "[state] : " << state << endl;
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::GetImeiResponse(const RilRadioResponseInfo &responseInfo, const std::string &imei)
{
    PrintResponseInfo("GetImeiResponse", responseInfo);
    cout << "[imei] : " << imei << endl;
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::GetMeidResponse(const RilRadioResponseInfo &responseInfo, const std::string &meid)
{
    PrintResponseInfo("GetMeidResponse", responseInfo);
    cout << "[meid] : " << meid << endl;
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::GetVoiceRadioTechnologyResponse(
    const RilRadioResponseInfo &responseInfo, const VoiceRadioTechnology &voiceRadioTechnology)
{
    PrintResponseInfo("GetVoiceRadioTechnologyResponse", responseInfo);
    cout << "[voiceRadioTechnology] -->[srvStatus] : " << voiceRadioTechnology.srvStatus
         << " -->[srvDomain] : " << voiceRadioTechnology.srvDomain
         << " -->[roamStatus] : " << voiceRadioTechnology.roamStatus
         << " -->[simStatus] : " << voiceRadioTechnology.simStatus
         << " -->[lockStatus] : " << voiceRadioTechnology.lockStatus
         << " -->[sysModeName] : " << voiceRadioTechnology.sysModeName
         << " -->[sysMode] : " << voiceRadioTechnology.sysMode << " -->[actName] : " << voiceRadioTechnology.actName
         << " -->[actType] : " << voiceRadioTechnology.actType;
    cout << endl << endl;
    return 0;
}

int32_t RilCallbackTest::GetBasebandVersionResponse(
    const RilRadioResponseInfo &responseInfo, const std::string &basebandVersion)
{
    PrintResponseInfo("GetBasebandVersionResponse", responseInfo);
    cout << "[basebandVersion] : " << basebandVersion << endl;
    cout << endl;
    return 0;
}

// Data
int32_t RilCallbackTest::PdpContextListUpdated(
    const RilRadioResponseInfo &responseInfo, const DataCallResultList &dataCallResultList)
{
    PrintResponseInfo("PdpContextListUpdated", responseInfo);
    cout << "[dataCallResultList] -->[size] : " << dataCallResultList.size << endl;
    for (const auto &setupDataCallResultInfo : dataCallResultList.dcList) {
        cout << "[setupDataCallResultInfo] -->[flag] : " << setupDataCallResultInfo.flag << endl;
        cout << "[setupDataCallResultInfo] -->[reason] : " << setupDataCallResultInfo.reason << endl;
        cout << "[setupDataCallResultInfo] -->[retryTime] : " << setupDataCallResultInfo.retryTime << endl;
        cout << "[setupDataCallResultInfo] -->[cid] : " << setupDataCallResultInfo.cid << endl;
        cout << "[setupDataCallResultInfo] -->[active] : " << setupDataCallResultInfo.active << endl;
        cout << "[setupDataCallResultInfo] -->[type] : " << setupDataCallResultInfo.type << endl;
        cout << "[setupDataCallResultInfo] -->[netPortName] : " << setupDataCallResultInfo.netPortName << endl;
        cout << "[setupDataCallResultInfo] -->[address] : " << setupDataCallResultInfo.address << endl;
        cout << "[setupDataCallResultInfo] -->[dns] : " << setupDataCallResultInfo.dns << endl;
        cout << "[setupDataCallResultInfo] -->[dnsSec] : " << setupDataCallResultInfo.dnsSec << endl;
        cout << "[setupDataCallResultInfo] -->[gateway] : " << setupDataCallResultInfo.gateway << endl;
        cout << "[setupDataCallResultInfo] -->[maxTransferUnit] : " << setupDataCallResultInfo.maxTransferUnit << endl;
        cout << "[setupDataCallResultInfo] -->[pCscfPrimAddr] : " << setupDataCallResultInfo.pCscfPrimAddr << endl;
        cout << "[setupDataCallResultInfo] -->[pCscfSecAddr] : " << setupDataCallResultInfo.pCscfSecAddr << endl;
        cout << "[setupDataCallResultInfo] -->[pduSessionId] : " << setupDataCallResultInfo.pduSessionId << endl;
    }
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::ActivatePdpContextResponse(
    const RilRadioResponseInfo &responseInfo, const SetupDataCallResultInfo &setupDataCallResultInfo)
{
    PrintResponseInfo("ActivatePdpContextResponse", responseInfo);
    cout << "[setupDataCallResultInfo] -->[flag] : " << setupDataCallResultInfo.flag << endl;
    cout << "[setupDataCallResultInfo] -->[reason] : " << setupDataCallResultInfo.reason << endl;
    cout << "[setupDataCallResultInfo] -->[retryTime] : " << setupDataCallResultInfo.retryTime << endl;
    cout << "[setupDataCallResultInfo] -->[cid] : " << setupDataCallResultInfo.cid << endl;
    cout << "[setupDataCallResultInfo] -->[active] : " << setupDataCallResultInfo.active << endl;
    cout << "[setupDataCallResultInfo] -->[type] : " << setupDataCallResultInfo.type << endl;
    cout << "[setupDataCallResultInfo] -->[netPortName] : " << setupDataCallResultInfo.netPortName << endl;
    cout << "[setupDataCallResultInfo] -->[address] : " << setupDataCallResultInfo.address << endl;
    cout << "[setupDataCallResultInfo] -->[dns] : " << setupDataCallResultInfo.dns << endl;
    cout << "[setupDataCallResultInfo] -->[dnsSec] : " << setupDataCallResultInfo.dnsSec << endl;
    cout << "[setupDataCallResultInfo] -->[gateway] : " << setupDataCallResultInfo.gateway << endl;
    cout << "[setupDataCallResultInfo] -->[maxTransferUnit] : " << setupDataCallResultInfo.maxTransferUnit << endl;
    cout << "[setupDataCallResultInfo] -->[pCscfPrimAddr] : " << setupDataCallResultInfo.pCscfPrimAddr << endl;
    cout << "[setupDataCallResultInfo] -->[pCscfSecAddr] : " << setupDataCallResultInfo.pCscfSecAddr << endl;
    cout << "[setupDataCallResultInfo] -->[pduSessionId] : " << setupDataCallResultInfo.pduSessionId << endl;
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::DeactivatePdpContextResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("DeactivatePdpContextResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetPdpContextListResponse(
    const RilRadioResponseInfo &responseInfo, const DataCallResultList &dataCallResultList)
{
    PrintResponseInfo("GetPdpContextListResponse", responseInfo);
    cout << "[dataCallResultList] -->[size] : " << dataCallResultList.size << endl;
    for (const auto &setupDataCallResultInfo : dataCallResultList.dcList) {
        cout << "[setupDataCallResultInfo] -->[flag] : " << setupDataCallResultInfo.flag << endl;
        cout << "[setupDataCallResultInfo] -->[reason] : " << setupDataCallResultInfo.reason << endl;
        cout << "[setupDataCallResultInfo] -->[retryTime] : " << setupDataCallResultInfo.retryTime << endl;
        cout << "[setupDataCallResultInfo] -->[cid] : " << setupDataCallResultInfo.cid << endl;
        cout << "[setupDataCallResultInfo] -->[active] : " << setupDataCallResultInfo.active << endl;
        cout << "[setupDataCallResultInfo] -->[type] : " << setupDataCallResultInfo.type << endl;
        cout << "[setupDataCallResultInfo] -->[netPortName] : " << setupDataCallResultInfo.netPortName << endl;
        cout << "[setupDataCallResultInfo] -->[address] : " << setupDataCallResultInfo.address << endl;
        cout << "[setupDataCallResultInfo] -->[dns] : " << setupDataCallResultInfo.dns << endl;
        cout << "[setupDataCallResultInfo] -->[dnsSec] : " << setupDataCallResultInfo.dnsSec << endl;
        cout << "[setupDataCallResultInfo] -->[gateway] : " << setupDataCallResultInfo.gateway << endl;
        cout << "[setupDataCallResultInfo] -->[maxTransferUnit] : " << setupDataCallResultInfo.maxTransferUnit << endl;
        cout << "[setupDataCallResultInfo] -->[pCscfPrimAddr] : " << setupDataCallResultInfo.pCscfPrimAddr << endl;
        cout << "[setupDataCallResultInfo] -->[pCscfSecAddr] : " << setupDataCallResultInfo.pCscfSecAddr << endl;
        cout << "[setupDataCallResultInfo] -->[pduSessionId] : " << setupDataCallResultInfo.pduSessionId << endl;
    }
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::SetInitApnInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetInitApnInfoResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SetLinkBandwidthReportingRuleResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetLinkBandwidthReportingRuleResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetLinkBandwidthInfoResponse(
    const RilRadioResponseInfo &responseInfo, const DataLinkBandwidthInfo &dataLinkBandwidthInfo)
{
    PrintResponseInfo("GetLinkBandwidthInfoResponse", responseInfo);
    cout << "[dataLinkBandwidthInfo] -->[serial] : " << dataLinkBandwidthInfo.serial << endl;
    cout << "[dataLinkBandwidthInfo] -->[cid] : " << dataLinkBandwidthInfo.cid << endl;
    cout << "[dataLinkBandwidthInfo] -->[qi] : " << dataLinkBandwidthInfo.qi << endl;
    cout << "[dataLinkBandwidthInfo] -->[dlGfbr] : " << dataLinkBandwidthInfo.dlGfbr << endl;
    cout << "[dataLinkBandwidthInfo] -->[ulGfbr] : " << dataLinkBandwidthInfo.ulGfbr << endl;
    cout << "[dataLinkBandwidthInfo] -->[dlMfbr] : " << dataLinkBandwidthInfo.dlMfbr << endl;
    cout << "[dataLinkBandwidthInfo] -->[ulMfbr] : " << dataLinkBandwidthInfo.ulMfbr << endl;
    cout << "[dataLinkBandwidthInfo] -->[ulSambr] : " << dataLinkBandwidthInfo.ulSambr << endl;
    cout << "[dataLinkBandwidthInfo] -->[dlSambr] : " << dataLinkBandwidthInfo.dlSambr << endl;
    cout << "[dataLinkBandwidthInfo] -->[averagingWindow] : " << dataLinkBandwidthInfo.averagingWindow << endl;
    cout << endl;
    return 0;
}

int32_t RilCallbackTest::SetDataPermittedResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetDataPermittedResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SetDataProfileInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetDataProfileInfoResponse", responseInfo);
    return 0;
}

// Sms
int32_t RilCallbackTest::NewSmsNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const SmsMessageInfo &smsMessageInfo)
{
    PrintResponseInfo("NewSmsNotify", responseInfo);
    cout << "[smsMessageInfo] -->[size] : " << smsMessageInfo.size << endl << endl;
    return 0;
}

int32_t RilCallbackTest::NewCdmaSmsNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const SmsMessageInfo &smsMessageInfo)
{
    PrintResponseInfo("NewCdmaSmsNotify", responseInfo);
    cout << "[smsMessageInfo] -->[size] : " << smsMessageInfo.size << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SmsStatusReportNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const SmsMessageInfo &smsMessageInfo)
{
    PrintResponseInfo("SmsStatusReportNotify", responseInfo);
    cout << "[smsMessageInfo] -->[size] : " << smsMessageInfo.size << endl << endl;
    return 0;
}

int32_t RilCallbackTest::NewSmsStoredOnSimNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, int32_t recordNumber, int32_t indicationType)
{
    PrintResponseInfo("NewSmsStoredOnSimNotify", responseInfo);
    cout << " [recordNumber] : " << recordNumber << endl;
    cout << " [indicationType] : " << indicationType << endl << endl;
    return 0;
}

int32_t RilCallbackTest::CBConfigNotify(
    const HDI::Ril::V1_0::RilRadioResponseInfo &responseInfo, const CBConfigReportInfo &cellBroadConfigReportInfo)
{
    PrintResponseInfo("CBConfigNotify", responseInfo);
    cout << "[cellBroadConfigReportInfo] -->[dcs] : " << cellBroadConfigReportInfo.dcs.c_str() << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SendGsmSmsResponse(
    const RilRadioResponseInfo &responseInfo, const SendSmsResultInfo &sendSmsResultInfo)
{
    PrintResponseInfo("SendGsmSmsResponse", responseInfo);
    cout << "[sendSmsResultInfo] -->[pdu] : " << sendSmsResultInfo.pdu.c_str() << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SendCdmaSmsResponse(
    const RilRadioResponseInfo &responseInfo, const SendSmsResultInfo &sendSmsResultInfo)
{
    PrintResponseInfo("SendCdmaSmsResponse", responseInfo);
    cout << "[sendSmsResultInfo] -->[pdu] : " << sendSmsResultInfo.pdu.c_str() << endl << endl;
    return 0;
}

int32_t RilCallbackTest::AddSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("AddSimMessageResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::DelSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("DelSimMessageResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::UpdateSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("UpdateSimMessageResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::AddCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("AddCdmaSimMessageResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::DelCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("DelCdmaSimMessageResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::UpdateCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("UpdateCdmaSimMessageResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::SetSmscAddrResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetSmscAddrResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetSmscAddrResponse(
    const RilRadioResponseInfo &responseInfo, const ServiceCenterAddress &serviceCenterAddress)
{
    PrintResponseInfo("GetSmscAddrResponse", responseInfo);
    cout << " [serviceCenterAddress] -->[tosca] : " << serviceCenterAddress.tosca << endl;
    cout << " [serviceCenterAddress] -->[address] : " << serviceCenterAddress.address.c_str() << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SetCBConfigResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetCBConfigResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetCBConfigResponse(
    const RilRadioResponseInfo &responseInfo, const CBConfigInfo &cellBroadcastInfo)
{
    PrintResponseInfo("GetCBConfigResponse", responseInfo);
    cout << " [cellBroadcastInfo] --> [mids] : " << cellBroadcastInfo.mids.c_str() << endl;
    cout << " [cellBroadcastInfo] --> [dcss] : " << cellBroadcastInfo.dcss.c_str() << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SetCdmaCBConfigResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SetCdmaCBConfigResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::GetCdmaCBConfigResponse(
    const RilRadioResponseInfo &responseInfo, const CdmaCBConfigInfo &cdmaCBConfigInfo)
{
    PrintResponseInfo("GetCdmaCBConfigResponse", responseInfo);
    cout << " [cdmaCBConfigInfo] --> [service] : " << cdmaCBConfigInfo.service << endl;
    cout << " [cdmaCBConfigInfo] --> [language] : " << cdmaCBConfigInfo.language << endl;
    cout << " [cdmaCBConfigInfo] --> [checked] : " << cdmaCBConfigInfo.checked << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SendSmsMoreModeResponse(
    const RilRadioResponseInfo &responseInfo, const SendSmsResultInfo &sendSmsResultInfo)
{
    PrintResponseInfo("SendSmsMoreModeResponse", responseInfo);
    cout << "[pdu] : " << sendSmsResultInfo.pdu.c_str() << endl << endl;
    return 0;
}

int32_t RilCallbackTest::SendSmsAckResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SendSmsAckResponse", responseInfo);
    return 0;
}

int32_t RilCallbackTest::CommonErrorResponse(const RilRadioResponseInfo &responseInfo)
{
    PrintResponseInfo("SendSmsAckResponse", responseInfo);
    return 0;
}
} // namespace Telephony
} // namespace OHOS
