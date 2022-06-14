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

#include "ril_radio_response_test.h"

#include <iostream>

#include "hril_request.h"
#include "telephony_log_wrapper.h"

using namespace std;
using namespace OHOS::Telephony;
RilRadioResponseTest::RilRadioResponseTest(RilManagerTest *mRilManager) : IPCObjectStub(std::u16string(u""))
{
    mRilManager_ = mRilManager;
}

RilRadioResponseTest::~RilRadioResponseTest() {}

int32_t RilRadioResponseTest::OnRemoteRequest(
    uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply, OHOS::MessageOption &option)
{
    auto reqToken = data.ReadInterfaceToken();
    HRilResponseTypes responseType = HRIL_RESPONSE_NOTICE;
    int32_t slotId = 0;
    const uint8_t *spBuffer = data.ReadUnpadBuffer(sizeof(HRilResponseHeadInfo));
    const HRilResponseHeadInfo *headInfo = reinterpret_cast<const HRilResponseHeadInfo *>(spBuffer);
    if (headInfo != nullptr) {
        slotId = headInfo->slotId;
        responseType = headInfo->type;
    } else {
        TELEPHONY_LOGW("code:%{public}d, headInfo parsed is failed.", code);
    }
    TELEPHONY_LOGI("TOnRemoteRequest code:%{public}d, slotId:%{public}d, type:%{public}d", code, slotId, responseType);
    if (slotId != HRIL_SIM_SLOT_0) {
        TELEPHONY_LOGE("RilAdapterTest rsp slotid abnormal");
    }
    switch (code) {
        case HREQ_CALL_GET_CALL_LIST:
            OnResponseGetCallList(data);
            break;
        case HREQ_CALL_DIAL:
            OnResponseNullPara("Dial Result", data);
            break;
        case HREQ_CALL_HANGUP:
            OnResponseNullPara("Hangup Result", data);
            break;
        case HREQ_CALL_REJECT:
            OnResponseNullPara("Reject Result", data);
            break;
        case HREQ_CALL_ANSWER:
            OnResponseNullPara("Answer Result", data);
            break;
        case HREQ_CALL_HOLD_CALL:
            OnResponseNullPara("Hold Call Result", data);
            break;
        case HREQ_CALL_UNHOLD_CALL:
            OnResponseNullPara("Unhold Call Result", data);
            break;
        case HREQ_CALL_SWITCH_CALL:
            OnResponseNullPara("Switch Call Result", data);
            break;
        case HREQ_CALL_CALL_SUPPLEMENT:
            OnResponseNullPara("Call Supplement Result", data);
            break;
        case HREQ_CALL_START_DTMF:
            OnResponseNullPara("Start Dtmf Result", data);
            break;
        case HREQ_CALL_SEND_DTMF:
            OnResponseNullPara("Send Dtmf Result", data);
            break;
        case HREQ_CALL_STOP_DTMF:
            OnResponseNullPara("Stop Dtmf Result", data);
            break;
        case HREQ_CALL_SET_MUTE:
            OnResponseNullPara("Set Mute Result", data);
            break;
        case HREQ_CALL_GET_MUTE:
            OnResponseGetMute(data);
            break;
        case HREQ_SIM_CLOSE_LOGICAL_CHANNEL:
            OnResponseNullPara("Set Sim Close Logical Channel Result", data);
            break;
        case HREQ_SIM_OPEN_LOGICAL_CHANNEL:
            OnResponseOpenLogicalChannel(data);
            break;
        case HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL:
            OnResponseNullPara("Set Sim Transmit apdu basic channel Result", data);
            break;
        case HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL:
            OnResponseNullPara("Set sim transmit apdu logical channel Result", data);
            break;
        case HREQ_SIM_AUTHENTICATION:
            OnResponseNullPara("Set Sim Authentication Result", data);
            break;
        case HREQ_CALL_GET_EMERGENCY_LIST:
            OnResponseGetEmergencyList(data);
            break;
        case HREQ_CALL_GET_FAIL_REASON:
            OnResponseGetFailReason(data);
            break;
        case HREQ_SIM_GET_SIM_STATUS:
            OnResponseGetSimState(data);
            break;
        case HREQ_MODEM_SET_RADIO_STATUS:
            OnResponseNullPara("Set radio Result", data);
            break;
        case HREQ_MODEM_SHUT_DOWN:
            OnResponseNullPara("shut down", data);
            break;
        case HREQ_MODEM_GET_RADIO_STATUS:
            OnResponseGetRadioState(data);
            break;
        case HREQ_MODEM_GET_IMEI:
            OnResponseGetImei(data);
            break;
        case HREQ_DATA_ACTIVATE_PDP_CONTEXT:
            OnResponseActivateDataCall(data);
            break;
        case HREQ_DATA_DEACTIVATE_PDP_CONTEXT:
            OnResponseDeactivateDataCall(data);
            break;
        case HREQ_DATA_GET_PDP_CONTEXT_LIST:
            OnResponseGetPdpContextList(data);
            break;
        case HREQ_DATA_SET_INIT_APN_INFO:
            OnResponseSetInitialApn(data);
            break;
        case HREQ_NETWORK_GET_SIGNAL_STRENGTH:
            OnResponseGetSignalQuality(data);
            break;
        case HREQ_NETWORK_GET_CS_REG_STATUS:
            OnResponseGetCsRegStatus(data);
            break;
        case HREQ_NETWORK_GET_PS_REG_STATUS:
            OnResponseGetNetworkRegistrationState(data);
            break;
        case HREQ_NETWORK_GET_OPERATOR_INFO:
            OnResponseGetCurrentOperator(data);
            break;
        case HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION:
            OnRequestGetNetworkSearchInformationTest(data);
            break;
        case HREQ_NETWORK_GET_NETWORK_SELECTION_MODE:
            OnRequestGetNetworkSelectionModeTest(data);
            break;
        case HREQ_NETWORK_SET_NETWORK_SELECTION_MODE:
            OnRequestSetNetworkSelectionModeTest(data);
            break;
        case HREQ_NETWORK_GET_IMS_REG_STATUS:
            OnResponseGetImsRegistrationState(data);
            break;
        case HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST:
            OnResponseGetRilNeighboringCellInfoList(data);
            break;
        case HREQ_NETWORK_SET_PREFERRED_NETWORK:
            OnRequestSetNetworkPreferredNetworkModeTest(data);
            break;
        case HREQ_NETWORK_GET_PREFERRED_NETWORK:
            OnRequestGetNetworkPreferredNetworkModeTest(data);
            break;
        case HREQ_NETWORK_SET_PS_ATTACH_STATUS:
            OnRequestSetNetworkPsAttachStatusTest(data);
            break;
        case HREQ_NETWORK_SET_LOCATE_UPDATES:
            OnRequestSetLocationUpdatesTest(data);
            break;
        case HREQ_NETWORK_SET_NOTIFICATION_FILTER:
            OnRequestSetNotificationFilterTest(data);
            break;
        case HREQ_NETWORK_SET_DEVICE_STATE:
            OnRequestSetDeviceStateTest(data);
            break;
        case HREQ_MODEM_GET_MEID:
            OnRequestGetMeidTest(data);
            break;
        case HREQ_MODEM_GET_VOICE_RADIO:
            OnRequestGetModemVoiceRadioTest(data);
            break;
        case HREQ_MODEM_GET_BASEBAND_VERSION:
            OnRequestGetBasebandVersionTest(data);
            break;
        case HREQ_SIM_GET_IMSI:
            OnResponseGetImsi(data);
            break;
        case HREQ_SIM_GET_SIM_IO:
            OnResponseGetSimIo(data);
            break;
        case HREQ_NETWORK_GET_CURRENT_CELL_INFO:
            OnResponseGetRilCurrentCellInfo(data);
            break;
        case HREQ_SMS_SEND_GSM_SMS:
            OnResponseSendRilCmSms(data);
            break;
        case HREQ_SMS_SEND_SMS_MORE_MODE:
            OnResponseSendRilCmSmsMoreMode(data);
            break;
        case HREQ_SMS_SEND_SMS_ACK:
            OnResponseSendSmsAck(data);
            break;
        case HREQ_SMS_GET_SMSC_ADDR:
            OnResponseGetSmscAddr(data);
            break;
        case HREQ_SMS_SET_SMSC_ADDR:
            OnResponseSetSmscAddr(data);
            break;
        case HREQ_SIM_CHANGE_SIM_PASSWORD:
            OnResponseLockStatus(data);
            break;
        case HREQ_SIM_SET_SIM_LOCK:
            OnResponseLockStatus(data);
            break;
        case HREQ_SIM_UNLOCK_SIM_LOCK:
            OnResponseLockStatus(data);
            break;
        case HREQ_SIM_SET_ACTIVE_SIM:
            OnResponseNullPara("Set active sim Result", data);
            break;
        case HREQ_SIM_GET_SIM_LOCK_STATUS:
            OnResponseGetSimLockStatus(data);
            break;
        case HREQ_SIM_UNLOCK_PIN:
            OnResponseLockStatus(data);
            break;
        case HREQ_SIM_UNLOCK_PUK:
            OnResponseLockStatus(data);
            break;
        case HREQ_SIM_UNLOCK_PIN2:
            OnResponseLockStatus(data);
            break;
        case HREQ_SIM_UNLOCK_PUK2:
            OnResponseLockStatus(data);
            break;
        default:
            break;
    }
    return 0;
}

void PrintResponseInfo(const struct HRilRadioResponseInfo *rspInfo)
{
    if (rspInfo == nullptr) {
        return;
    }
    cout << endl
         // << "----> [flag]: " << rspInfo->flag << endl
         << "----> [serial]: " << rspInfo->serial << endl
         << "----> [error]: " << static_cast<int>(rspInfo->error) << endl
         << endl;
}

void RilRadioResponseTest::OnResponseNullPara(std::string command, OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseNullPara -->");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseNullPara -->data.ReadBuffer(readSpSize) failed");
        return;
    }

    cout << endl << "---->" << command << " :";
    TELEPHONY_LOGI("%{public}s", command.c_str());
    PrintResponseInfo(reinterpret_cast<const struct HRilRadioResponseInfo *>(spBuffer));

    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseNullPara --> finished");
}

void RilRadioResponseTest::OnResponseLockStatus(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseLockStatus -->");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseLockStatus -->data.ReadBuffer(readSpSize) failed");
        return;
    }
    const struct HRilRadioResponseInfo *responseInfo = reinterpret_cast<const struct HRilRadioResponseInfo *>(spBuffer);

    const size_t readLockStatusSize = sizeof(HRilLockStatus);
    const uint8_t *lockStatusBuffer = data.ReadBuffer(readLockStatusSize);
    if (lockStatusBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseLockStatus -->data.ReadBuffer(readLockStatusSize) failed");
        return;
    }
    const HRilLockStatus *resp = reinterpret_cast<const HRilLockStatus *>(lockStatusBuffer);
    cout << endl << "====>result: " << resp->result << endl;
    cout << endl << "====>remain: " << resp->remain << endl;
    cout << endl << "---->OnResponseLockStatus";
    PrintResponseInfo(responseInfo);
}

void RilRadioResponseTest::OnResponseOpenLogicalChannel(MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseOpenLogicalChannel -->");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseOpenLogicalChannel -->data.ReadBuffer(readSpSize) failed");
        return;
    }
    const struct HRilRadioResponseInfo *responseInfo =
        reinterpret_cast<const struct HRilRadioResponseInfo *>(spBuffer);

    std::shared_ptr<OpenLogicalChannelResponse> resp = std::make_shared<OpenLogicalChannelResponse>();
    resp.get()->ReadFromParcel(data);
    cout << endl << "====>channelId: " << resp->channelId << endl;
    cout << endl << "---->OnResponseOpenLogicalChannel";
    PrintResponseInfo(responseInfo);
}

void RilRadioResponseTest::OnResponseGetCallList(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetCallList -->");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetCallList -->data.ReadBuffer(readSpSize) failed");
        return;
    }

    const struct HRilRadioResponseInfo *responseInfo = reinterpret_cast<const struct HRilRadioResponseInfo *>(spBuffer);
    if (responseInfo->error == HRilErrType::NONE) {
        std::shared_ptr<CallInfoList> callInfos = std::make_shared<CallInfoList>();
        if (callInfos == nullptr) {
            TELEPHONY_LOGE("ERROR : callInfo == nullptr !!!");
            return;
        }
        callInfos.get()->ReadFromParcel(data);
        cout << endl << "====>Call List Num: " << callInfos->callSize << endl;
        for (int32_t i = 0; i < callInfos->callSize; i++) {
            cout << "====> [index]: " << callInfos->calls[i].index << "\tdir: " << callInfos->calls[i].dir
                 << "\tstate: " << callInfos->calls[i].state << "\tnumber: " << callInfos->calls[i].number << endl;
        }
    }

    cout << endl << "---->Get Call list Result:";
    PrintResponseInfo(responseInfo);
}

void RilRadioResponseTest::OnResponseGetMute(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetMute -->");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetMute -->data.ReadBuffer(readSpSize) failed");
        return;
    }

    const struct HRilRadioResponseInfo *responseInfo = reinterpret_cast<const struct HRilRadioResponseInfo *>(spBuffer);
    if (responseInfo->error == HRilErrType::NONE) {
        int32_t mute = data.ReadInt32();
        cout << endl << "====>Mute State: " << mute << endl;
    }

    cout << endl << "---->Get Mute Result :";
    PrintResponseInfo(responseInfo);
}

void RilRadioResponseTest::OnResponseGetSimLockStatus(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetSimLockStatus -->");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetSimLockStatus -->data.ReadBuffer(readSpSize) failed");
        return;
    }

    const struct HRilRadioResponseInfo *responseInfo = reinterpret_cast<const struct HRilRadioResponseInfo *>(spBuffer);
    if (responseInfo->error == HRilErrType::NONE) {
        int32_t simLockStatus = data.ReadInt32();
        cout << endl << "====>Sim Lock Status: " << simLockStatus << endl;
    }

    cout << endl << "---->Get Sim Lock Status :";
    PrintResponseInfo(responseInfo);
}
void RilRadioResponseTest::OnResponseGetEmergencyList(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetEmergencyList -->");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetEmergencyList -->data.ReadBuffer(readSpSize) failed");
        return;
    }

    const struct HRilRadioResponseInfo *responseInfo = reinterpret_cast<const struct HRilRadioResponseInfo *>(spBuffer);
    if (responseInfo->error == HRilErrType::NONE) {
        std::shared_ptr<EmergencyInfoList> emergencyCallList = std::make_shared<EmergencyInfoList>();
        if (emergencyCallList == nullptr) {
            TELEPHONY_LOGE("ERROR : callInfo == nullptr !!!");
            return;
        }
        emergencyCallList->ReadFromParcel(data);

        cout << endl << "====>Emergency List Num: " << emergencyCallList->callSize << endl;
        for (int32_t i = 0; i < emergencyCallList->callSize; i++) {
            cout << "====> [index]: " << emergencyCallList->calls[i].index
                 << "\tcategory: " << emergencyCallList->calls[i].category
                 << "\tmcc: " << emergencyCallList->calls[i].mcc << "\teccNum: " << emergencyCallList->calls[i].eccNum
                 << endl;
        }
    }

    cout << endl << "---->Get Emergency list Result:";
    PrintResponseInfo(responseInfo);
}

void RilRadioResponseTest::OnResponseGetFailReason(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest OnResponseGetFailReason -->");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetFailReason -->data.ReadBuffer(readSpSize) failed");
        return;
    }

    const struct HRilRadioResponseInfo *responseInfo = reinterpret_cast<const struct HRilRadioResponseInfo *>(spBuffer);
    if (responseInfo->error == HRilErrType::NONE) {
        int32_t reason = data.ReadInt32();
        cout << endl << "====>Fail Reason: " << reason << endl;
    }

    cout << endl << "---->Get Call Fail Reason Result :";
    PrintResponseInfo(responseInfo);
}

void RilRadioResponseTest::GetImsiResponse(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::GetImsiResponse --> ");
    const char *buffer = data.ReadCString();
    if (buffer == nullptr) {
        TELEPHONY_LOGE("ERROR : GetImsiResponse --> data.ReadCString() failed !!!");
        return;
    }
    std::unique_ptr<std::string> imsi = std::make_unique<std::string>(buffer);
    if (imsi == nullptr) {
        TELEPHONY_LOGE("ERROR : GetImsiResponse --> imsi is null!!!");
        return;
    }
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : GetImsiResponse --> read spBuffer(HRilRadioResponseInfo) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetSimState(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetSimState --> ");
    std::unique_ptr<CardStatusInfo> cardStatusInfo = std::make_unique<CardStatusInfo>();
    if (cardStatusInfo == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetSimState --> cardStatusInfo null !!!");
        return;
    }
    if (!cardStatusInfo.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("ERROR : OnResponseGetSimState --> ReadFromParcel(data) failed !!!");
        return;
    }

    cout << "OnResponseGetSimState: index = " << cardStatusInfo->index << endl;
    cout << "OnResponseGetSimState: simType = " << cardStatusInfo->simType << endl;
    cout << "OnResponseGetSimState: simState = " << cardStatusInfo->simState << endl;

    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetSimState --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetImsi(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetImsi --> ");
    const char *buffer = data.ReadCString();
    std::unique_ptr<std::string> imsi = std::make_unique<std::string>(buffer);
    if (buffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetImsi --> buffer null !!!");
        return;
    }

    cout << "OnResponseGetImsi: imsi = " << *imsi << endl;

    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetImsi --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetSimIo(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetSimIo --> ");
    std::unique_ptr<IccIoResultInfo> simIoResult = std::make_unique<IccIoResultInfo>();
    if (simIoResult == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetSimIo --> simIoResult null !!!");
        return;
    }
    if (!simIoResult.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("ERROR : OnResponseGetSimIo --> ReadFromParcel(data) failed !!!");
        return;
    }

    cout << "OnResponseGetSimIo: sw1 = " << simIoResult->sw1 << endl;
    cout << "OnResponseGetSimIo: sw2 = " << simIoResult->sw2 << endl;
    cout << "OnResponseGetSimIo: response = " << simIoResult->response << endl;

    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetSimIo --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetRilCurrentCellInfo(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetRilCurrentCellInfo --> ");
    std::unique_ptr<CurrentCellInfo> curCellInfo = std::make_unique<CurrentCellInfo>();
    if (curCellInfo == nullptr) {
        return;
    }
    if (!curCellInfo.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("ERROR : OnResponseGetRilCurrentCellInfo --> CurrentCellInfo.ReadFromParcel(data) failed !!!");
        return;
    }
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetRilCurrentCellInfo --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }

    cout << "OnResponseGetRilCurrentCellInfo: mcc = " << curCellInfo->mcc << endl;
    cout << "OnResponseGetRilCurrentCellInfo: mnc = " << curCellInfo->mnc << endl;
    cout << "OnResponseGetRilCurrentCellInfo: ratType = " << curCellInfo->ratType << endl;

    if (curCellInfo->ratType == NETWORK_TYPE_WCDMA) {
        cout << "OnResponseGetRilCurrentCellInfo: arfcn = " << curCellInfo->ServiceCellParas.wcdma.arfcn << endl;
        cout << "OnResponseGetRilCurrentCellInfo: psc = " << curCellInfo->ServiceCellParas.wcdma.psc << endl;
        cout << "OnResponseGetRilCurrentCellInfo: cellId = " << curCellInfo->ServiceCellParas.wcdma.cellId << endl;
        cout << "OnResponseGetRilCurrentCellInfo: lac = " << curCellInfo->ServiceCellParas.wcdma.lac << endl;
        cout << "OnResponseGetRilCurrentCellInfo: rscp = " << curCellInfo->ServiceCellParas.wcdma.rscp << endl;
        cout << "OnResponseGetRilCurrentCellInfo: rxlev = " << curCellInfo->ServiceCellParas.wcdma.rxlev << endl;
        cout << "OnResponseGetRilCurrentCellInfo: ecno = " << curCellInfo->ServiceCellParas.wcdma.ecno << endl;
        cout << "OnResponseGetRilCurrentCellInfo: drx = " << curCellInfo->ServiceCellParas.wcdma.drx << endl;
        cout << "OnResponseGetRilCurrentCellInfo: ura = " << curCellInfo->ServiceCellParas.wcdma.ura << endl;
    } else if (curCellInfo->ratType == NETWORK_TYPE_LTE) {
        cout << "OnResponseGetRilCurrentCellInfo: arfcn = " << curCellInfo->ServiceCellParas.lte.arfcn << endl;
        cout << "OnResponseGetRilCurrentCellInfo: cellId = " << curCellInfo->ServiceCellParas.lte.cellId << endl;
        cout << "OnResponseGetRilCurrentCellInfo: pci = " << curCellInfo->ServiceCellParas.lte.pci << endl;
        cout << "OnResponseGetRilCurrentCellInfo: tac = " << curCellInfo->ServiceCellParas.lte.tac << endl;
        cout << "OnResponseGetRilCurrentCellInfo: rsrp = " << curCellInfo->ServiceCellParas.lte.rsrp << endl;
        cout << "OnResponseGetRilCurrentCellInfo: rsrq = " << curCellInfo->ServiceCellParas.lte.rsrq << endl;
        cout << "OnResponseGetRilCurrentCellInfo: rssi = " << curCellInfo->ServiceCellParas.lte.rssi << endl;
    } else if (curCellInfo->ratType == NETWORK_TYPE_GSM) {
        cout << "OnResponseGetRilCurrentCellInfo: band = " << curCellInfo->ServiceCellParas.gsm.band << endl;
        cout << "OnResponseGetRilCurrentCellInfo: arfcn = " << curCellInfo->ServiceCellParas.gsm.arfcn << endl;
        cout << "OnResponseGetRilCurrentCellInfo: bsic = " << curCellInfo->ServiceCellParas.gsm.bsic << endl;
        cout << "OnResponseGetRilCurrentCellInfo: cellId = " << curCellInfo->ServiceCellParas.gsm.cellId << endl;
        cout << "OnResponseGetRilCurrentCellInfo: lac = " << curCellInfo->ServiceCellParas.gsm.lac << endl;
        cout << "OnResponseGetRilCurrentCellInfo: rxlev = " << curCellInfo->ServiceCellParas.gsm.rxlev << endl;
        cout << "OnResponseGetRilCurrentCellInfo: rxQuality = " << curCellInfo->ServiceCellParas.gsm.rxQuality << endl;
        cout << "OnResponseGetRilCurrentCellInfo: ta = " << curCellInfo->ServiceCellParas.gsm.ta << endl;
    } else if (curCellInfo->ratType == NETWORK_TYPE_NR) {
        cout << "OnResponseGetRilCurrentCellInfo: nrArfcn = " << curCellInfo->ServiceCellParas.nr.nrArfcn << endl;
        cout << "OnResponseGetRilCurrentCellInfo: pci = " << curCellInfo->ServiceCellParas.nr.pci << endl;
        cout << "OnResponseGetRilCurrentCellInfo: tac = " << curCellInfo->ServiceCellParas.nr.tac << endl;
        cout << "OnResponseGetRilCurrentCellInfo: nci = " << curCellInfo->ServiceCellParas.nr.nci << endl;
    } else {
        cout << "OnResponseGetRilCurrentCellInfo: invalid rat " << endl;
    }
}

void RilRadioResponseTest::OnResponseGetRilNeighboringCellInfoList(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetRilNeighboringCellInfoList --> ");
    std::unique_ptr<CellListNearbyInfo> neighboringCellList = std::make_unique<CellListNearbyInfo>();
    if (neighboringCellList == nullptr) {
        return;
    }
    if (!neighboringCellList.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE(
            "ERROR : OnResponseGetRilNeighboringCellInfoList --> CurrentCellInfo.ReadFromParcel(data) failed !!!");
        return;
    }
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetRilCurrentCellInfo --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }

    cout << "OnResponseGetRilNeighboringCellInfoList: itemNum = " << neighboringCellList->itemNum << endl;
    for (auto &info : neighboringCellList->cellNearbyInfo) {
        cout << "OnResponseGetRilNeighboringCellInfoList: ratType = " << info.ratType << endl;
        if (info.ratType == NETWORK_TYPE_LTE) {
            cout << "OnResponseGetRilNeighboringCellInfoList: arfcn = " << info.ServiceCellParas.lte.arfcn << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: pci = " << info.ServiceCellParas.lte.pci << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: rsrp = " << info.ServiceCellParas.lte.rsrp << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: rsrq = " << info.ServiceCellParas.lte.rsrq << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: rxlev = " << info.ServiceCellParas.lte.rxlev << endl;
        } else if (info.ratType == NETWORK_TYPE_GSM) {
            cout << "OnResponseGetRilNeighboringCellInfoList: band = " << info.ServiceCellParas.gsm.band << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: arfcn = " << info.ServiceCellParas.gsm.arfcn << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: bsic = " << info.ServiceCellParas.gsm.bsic << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: cellId = " << info.ServiceCellParas.gsm.cellId << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: lac = " << info.ServiceCellParas.gsm.lac << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: rxlev = " << info.ServiceCellParas.gsm.rxlev << endl;
        } else if (info.ratType == NETWORK_TYPE_WCDMA) {
            cout << "OnResponseGetRilNeighboringCellInfoList: arfcn = " << info.ServiceCellParas.wcdma.arfcn << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: psc = " << info.ServiceCellParas.wcdma.psc << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: rscp = " << info.ServiceCellParas.wcdma.rscp << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: ecno = " << info.ServiceCellParas.wcdma.ecno << endl;
        } else if (info.ratType == NETWORK_TYPE_NR) {
            cout << "OnResponseGetRilNeighboringCellInfoList: nrArfcn = " << info.ServiceCellParas.nr.nrArfcn << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: pci = " << info.ServiceCellParas.nr.pci << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: tac = " << info.ServiceCellParas.nr.tac << endl;
            cout << "OnResponseGetRilNeighboringCellInfoList: nci = " << info.ServiceCellParas.nr.nci << endl;
        } else {
            cout << "OnResponseGetRilNeighboringCellInfoList: invalid ratTyep" << endl;
        }
    }
}

void RilRadioResponseTest::OnResponseGetSignalQuality(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest OnResponseGetSignalQuality ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest::OnResponseGetSignalQuality read spBuffer failed");
        return;
    }

    cout << "OnResponseGetSignalQuality: out " << endl;
}

void RilRadioResponseTest::OnResponseGetIccIOState(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetIccIOState --> ");
    std::unique_ptr<IccIoResultInfo> iccIoResult = std::make_unique<IccIoResultInfo>();
    if (iccIoResult == nullptr) {
        return;
    }
    if (!iccIoResult.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("OnResponseGetIccIOState --> iccIoResult.ReadFromParcel(data) failed !!!");
        return;
    }
    TELEPHONY_LOGI("OnResponseGetIccIOState --> iccIoResult.ReadFromParcel(data) success.");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetIccIOState --> read spBuffer(HRilRadioResponseInfo) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetCsRegStatus(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetCsRegStatus start ");
    cout << "OnResponseGetCsRegStatus in" << endl;

    std::unique_ptr<CsRegStatusInfo> mCsRegStatusInfo = std::make_unique<CsRegStatusInfo>();
    if (mCsRegStatusInfo == nullptr) {
        TELEPHONY_LOGE("RilAdapter %{public}s: failed to obtain CsRegStatusInfo", __func__);
        return;
    }
    mCsRegStatusInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetIccIOState --> read spBuffer failed");
        return;
    }

    cout << "OnResponseGetCsRegStatus: notifyType = " << mCsRegStatusInfo->notifyType << endl;
    cout << "OnResponseGetCsRegStatus: regStatus = " << mCsRegStatusInfo->regStatus << endl;
    cout << "OnResponseGetCsRegStatus: lacCode = " << mCsRegStatusInfo->lacCode << endl;
    cout << "OnResponseGetCsRegStatus: cellId = " << mCsRegStatusInfo->cellId << endl;
    cout << "OnResponseGetCsRegStatus: radioTechnology = " << mCsRegStatusInfo->radioTechnology << endl;
}

void RilRadioResponseTest::OnResponseGetNetworkRegistrationState(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetNetworkRegistrationState start, but need struct");
    std::unique_ptr<PsRegStatusResultInfo> psRegStateResult = std::make_unique<PsRegStatusResultInfo>();
    if (psRegStateResult == nullptr) {
        return;
    }
    psRegStateResult.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseGetNetworkRegistrationState --> read spBuffer failed");
        return;
    }

    cout << "OnResponseGetNetworkRegistrationState: notifyType = " << psRegStateResult->notifyType << endl;
    cout << "OnResponseGetNetworkRegistrationState: regStatus = " << psRegStateResult->regStatus << endl;
    cout << "OnResponseGetNetworkRegistrationState: lacCode = " << psRegStateResult->lacCode << endl;
    cout << "OnResponseGetNetworkRegistrationState: cellId = " << psRegStateResult->cellId << endl;
    cout << "OnResponseGetNetworkRegistrationState: radioTechnology = " << psRegStateResult->radioTechnology << endl;
    cout << "OnResponseGetNetworkRegistrationState: isNrAvailable = " << psRegStateResult->isNrAvailable << endl;
    cout << "OnResponseGetNetworkRegistrationState: isEnDcAvailable = " << psRegStateResult->isEnDcAvailable << endl;
    cout << "OnResponseGetNetworkRegistrationState: isDcNrRestricted = " << psRegStateResult->isDcNrRestricted << endl;
}

void RilRadioResponseTest::OnResponseGetCurrentOperator(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest OnResponseGetCurrentOperator ");
    std::unique_ptr<OperatorInfoResult> mOperatorInfo = std::make_unique<OperatorInfoResult>();
    if (mOperatorInfo == nullptr) {
        return;
    }
    mOperatorInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseGetCurrentOperator read spBuffer failed");
        return;
    }

    cout << "OnResponseGetCurrentOperator: longName = " << mOperatorInfo->longName << endl;
    cout << "OnResponseGetCurrentOperator: shortName = " << mOperatorInfo->shortName << endl;
    cout << "OnResponseGetCurrentOperator: numeric = " << mOperatorInfo->numeric << endl;
}

void RilRadioResponseTest::OnResponseGetRadioState(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseGetRadioStateResponse read spBuffer failed");
        return;
    }
    int32_t radioState = data.ReadInt32();
    TELEPHONY_LOGI("OnResponseGetRadioStateResponse back");

    cout << "---->OnResponseGetRadioStateResponse Result:" << endl << "----> [radioState]: " << radioState;
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnResponseGetImei(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseGetImeiResponse read spBuffer failed");
        return;
    }

    std::string imeiId = data.ReadString();

    TELEPHONY_LOGI("OnResponseGetImeiResponse back");

    cout << "---->OnResponseGetImeiResponse Result:" << endl << "----> [imeiId]: " << imeiId;
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::SendSmsResponse(OHOS::MessageParcel &data)
{
    std::unique_ptr<SendSmsResultInfo> mSendSmsResultInfo = std::make_unique<SendSmsResultInfo>();
    if (mSendSmsResultInfo == nullptr) {
        return;
    }
    mSendSmsResultInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("SendSmsResponse --> read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::SendSmsMoreModeResponse(OHOS::MessageParcel &data)
{
    std::unique_ptr<SendSmsResultInfo> mSendSmsResultInfo = std::make_unique<SendSmsResultInfo>();
    if (mSendSmsResultInfo == nullptr) {
        return;
    }
    mSendSmsResultInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("SendSmsMoreModeResponse --> read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseActivateDataCall(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::ActivatePdpContextResponse --> ");
    std::unique_ptr<SetupDataCallResultInfo> setupDataCallResultInfo = std::make_unique<SetupDataCallResultInfo>();
    if (setupDataCallResultInfo == nullptr) {
        return;
    }
    if (!setupDataCallResultInfo.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE(
            "ERROR : OnResponseActivateDataCall --> setupDataCallResultInfo.ReadFromParcel(data) failed !!!");
        return;
    }
    cout << "OnResponseActivateDataCall: flag = " << setupDataCallResultInfo->flag << endl;
    cout << "OnResponseActivateDataCall: reason = " << setupDataCallResultInfo->reason << endl;
    cout << "OnResponseActivateDataCall: retryTime = " << setupDataCallResultInfo->retryTime << endl;
    cout << "OnResponseActivateDataCall: cid = " << setupDataCallResultInfo->cid << endl;
    cout << "OnResponseActivateDataCall: active = " << setupDataCallResultInfo->active << endl;
    cout << "OnResponseActivateDataCall: type = " << setupDataCallResultInfo->type << endl;
    cout << "OnResponseActivateDataCall: netPortName = " << setupDataCallResultInfo->netPortName << endl;
    cout << "OnResponseActivateDataCall: address = " << setupDataCallResultInfo->address << endl;
    cout << "OnResponseActivateDataCall: dns = " << setupDataCallResultInfo->dns << endl;
    cout << "OnResponseActivateDataCall: dnsSec = " << setupDataCallResultInfo->dnsSec << endl;
    cout << "OnResponseActivateDataCall: gateway = " << setupDataCallResultInfo->gateway << endl;
    cout << "OnResponseActivateDataCall: pCscfPrimAddr = " << setupDataCallResultInfo->pCscfPrimAddr << endl;
    cout << "OnResponseActivateDataCall: pCscfSecAddr = " << setupDataCallResultInfo->pCscfSecAddr << endl;
    cout << "OnResponseActivateDataCall: maxTransferUnit = " << setupDataCallResultInfo->maxTransferUnit << endl;

    TELEPHONY_LOGI("OnResponseActivateDataCall --> setupDataCallResultInfo.ReadFromParcel(data) success");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseActivateDataCall --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseDeactivateDataCall(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseDeactivateDataCall --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseDeactivateDataCall --> ReadBuffer(data) failed !!!");
        return;
    }

    cout << "OnResponseDeactivateDataCall finished." << endl;
}

void RilRadioResponseTest::OnResponseGetPdpContextList(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetPdpContextList --> ");
    std::unique_ptr<DataCallResultList> dataCallList = std::make_unique<DataCallResultList>();
    if (dataCallList == nullptr) {
        TELEPHONY_LOGE("dataCallList is null ptr!");
        return;
    }
    if (!dataCallList.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("ERROR : OnResponseGetPdpContextList --> dataCallList.ReadFromParcel(data) failed !!!");
        return;
    }
    TELEPHONY_LOGI("OnResponseGetPdpContextList --> dataCallList.ReadFromParcel(data) success");

    for (auto &setupDataCallResultInfo : dataCallList->dcList) {
        cout << "OnResponseGetPdpContextList: flag = " << setupDataCallResultInfo.flag << endl;
        cout << "OnResponseGetPdpContextList: reason = " << setupDataCallResultInfo.reason << endl;
        cout << "OnResponseGetPdpContextList: retryTime = " << setupDataCallResultInfo.retryTime << endl;
        cout << "OnResponseGetPdpContextList: cid = " << setupDataCallResultInfo.cid << endl;
        cout << "OnResponseGetPdpContextList: active = " << setupDataCallResultInfo.active << endl;
        cout << "OnResponseGetPdpContextList: type = " << setupDataCallResultInfo.type << endl;
        cout << "OnResponseGetPdpContextList: netPortName = " << setupDataCallResultInfo.netPortName << endl;
        cout << "OnResponseGetPdpContextList: address = " << setupDataCallResultInfo.address << endl;
        cout << "OnResponseGetPdpContextList: dns = " << setupDataCallResultInfo.dns << endl;
        cout << "OnResponseGetPdpContextList: dnsSec = " << setupDataCallResultInfo.dnsSec << endl;
        cout << "OnResponseGetPdpContextList: gateway = " << setupDataCallResultInfo.gateway << endl;
        cout << "OnResponseGetPdpContextList: pCscfPrimAddr = " << setupDataCallResultInfo.pCscfPrimAddr << endl;
        cout << "OnResponseGetPdpContextList: pCscfSecAddr = " << setupDataCallResultInfo.pCscfSecAddr << endl;
        cout << "OnResponseGetPdpContextList: maxTransferUnit = " << setupDataCallResultInfo.maxTransferUnit << endl;
    }
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetPdpContextList --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseSendNewMessageAckCdma(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSendNewMessageAckCdma read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseSetInitialApn(OHOS::MessageParcel &data)
{
    cout << "OnResponseSetInitialApn finished." << endl;

    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSetInitialApn read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseGetImsRegistrationState(OHOS::MessageParcel &data)
{
    std::unique_ptr<ImsRegStatusInfo> imsRegStatus = std::make_unique<ImsRegStatusInfo>();
    if (imsRegStatus == nullptr) {
        return;
    }
    imsRegStatus.get()->ReadFromParcel(data);

    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseGetImsRegistrationState read spBuffer failed");
        return;
    }

    cout << "OnResponseGetImsRegistrationState: notifyType = " << imsRegStatus->notifyType << endl;
    cout << "OnResponseGetImsRegistrationState: regInfo = " << imsRegStatus->regInfo << endl;
    cout << "OnResponseGetImsRegistrationState: extInfo = " << imsRegStatus->extInfo << endl;
}

void RilRadioResponseTest::OnResponseSendImsSms(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest OnResponseSendImsSms ");
    std::unique_ptr<SendSmsResultInfo> mSendSmsResultInfo = std::make_unique<SendSmsResultInfo>();
    if (mSendSmsResultInfo == nullptr) {
        return;
    }
    mSendSmsResultInfo.get()->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnResponseSendImsSms read spBuffer failed");
        return;
    }
}

void RilRadioResponseTest::OnResponseSetDataCallAllowed(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseSetDataCallAllowed --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseSetDataCallAllowed --> ReadBuffer(data) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseSetModemDataProfile(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseSetModemDataProfile --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnResponseSetModemDataProfile --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponsePullRadioLceData(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnRequestGetNetworkSelectionModeTest(OHOS::MessageParcel &data)
{
    std::unique_ptr<SetNetworkModeInfo> networkMode = std::make_unique<SetNetworkModeInfo>();
    if (networkMode == nullptr) {
        return;
    }

    if (!networkMode.get()->ReadFromParcel(data)) {
        TELEPHONY_LOGE("ERROR : OnRequestGetNetworkSelectionModeTest --> ReadFromParcel(data) failed !!!");
        return;
    }

    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
    cout << "OnRequestGetNetworkSelectionModeTest: selectMode = " << networkMode->selectMode << endl;
}
void RilRadioResponseTest::OnRequestGetNetworkSearchInformationTest(OHOS::MessageParcel &data)
{
    std::unique_ptr<AvailableNetworkList> availableNetwork = std::make_unique<AvailableNetworkList>();
    if (availableNetwork == nullptr) {
        return;
    }
    availableNetwork.get()->ReadFromParcel(data);

    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }

    for (auto &sigleAvailableNetwork : availableNetwork->availableNetworkInfo) {
        cout << "OnRequestGetNetworkSearchInformationTest: longName = " << sigleAvailableNetwork.longName << endl;
        cout << "OnRequestGetNetworkSearchInformationTest: shortName = " << sigleAvailableNetwork.shortName << endl;
        cout << "OnRequestGetNetworkSearchInformationTest: numeric = " << sigleAvailableNetwork.numeric << endl;
        cout << "OnRequestGetNetworkSearchInformationTest: status = " << sigleAvailableNetwork.status << endl;
        cout << "OnRequestGetNetworkSearchInformationTest: rat = " << sigleAvailableNetwork.rat << endl;
    }
}

void RilRadioResponseTest::OnRequestSetNetworkSelectionModeTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnRequestSetNetworkSelectionModeTest read spBuffer failed");
        return;
    }

    cout << "---->SetNetworkSelectionMode Result:";
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnRequestSetNetworkPreferredNetworkModeTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnRequestSetNetworkPreferredNetworkModeTest read spBuffer failed");
        return;
    }

    cout << "---->SetNetworkPreferredNetworkMode Result:";
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnRequestGetNetworkPreferredNetworkModeTest(OHOS::MessageParcel &data)
{
    std::unique_ptr<PreferredNetworkTypeInfo> preferNetworkType = std::make_unique<PreferredNetworkTypeInfo>();
    if (preferNetworkType == nullptr) {
        return;
    }
    preferNetworkType.get()->ReadFromParcel(data);

    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
    cout << "OnRequestGetNetworkPreferredNetworkModeTest: preferredNetworkType = "
         << preferNetworkType->preferredNetworkType << endl;
}

void RilRadioResponseTest::OnRequestGetMeidTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnRequestGetMeidTest read spBuffer failed");
        return;
    }
    std::string meid = data.ReadString();
    cout << "---->OnRequestGetMeidTest Result:" << endl << "----> [imeid]: " << meid;
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnRequestGetModemVoiceRadioTest(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestGetModemVoiceRadioTest -->");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("OnRequestGetModemVoiceRadioTest -->data.ReadBuffer(readSpSize) failed");
        return;
    }

    const struct HRilRadioResponseInfo *responseInfo = reinterpret_cast<const struct HRilRadioResponseInfo *>(spBuffer);
    if (responseInfo->error == HRilErrType::NONE) {
        std::shared_ptr<VoiceRadioTechnology> voiceRadioTech = std::make_shared<VoiceRadioTechnology>();
        if (voiceRadioTech == nullptr) {
            TELEPHONY_LOGE("ERROR : voiceRadioTech == nullptr !!!");
            return;
        }
        voiceRadioTech.get()->ReadFromParcel(data);
        cout << "OnRequestGetModemVoiceRadioTest: srvStatus = " << voiceRadioTech->srvStatus << endl;
        cout << "OnRequestGetModemVoiceRadioTest: srvDomain = " << voiceRadioTech->srvDomain << endl;
        cout << "OnRequestGetModemVoiceRadioTest: roamStatus = " << voiceRadioTech->roamStatus << endl;
        cout << "OnRequestGetModemVoiceRadioTest: simStatus = " << voiceRadioTech->simStatus << endl;
        cout << "OnRequestGetModemVoiceRadioTest: sysMode = " << voiceRadioTech->sysMode << endl;
        cout << "OnRequestGetModemVoiceRadioTest: sysModeName = " << voiceRadioTech->sysModeName << endl;
        cout << "OnRequestGetModemVoiceRadioTest: actType = " << voiceRadioTech->actType << endl;
        cout << "OnRequestGetModemVoiceRadioTest: actName = " << voiceRadioTech->actName << endl;
    }

    cout << endl << "---->Get ModemVoiceRadioTest Result:";
    PrintResponseInfo(responseInfo);
}

void RilRadioResponseTest::OnRequestGetBasebandVersionTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnRequestGetBasebandVersionTest read spBuffer failed");
        return;
    }
    std::string bandVersion = data.ReadString();
    cout << "---->OnRequestGetBasebandVersionTest Result:" << endl << "----> [bandVersion]: " << bandVersion;
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnRequestSetNetworkPsAttachStatusTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnRequestSetNetworkPsAttachStatusTest read spBuffer failed");
        return;
    }

    cout << "---->SetNetworkPsAttachStatus Result:";
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnRequestSetLocationUpdatesTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnRequestSetLocationUpdatesTest read spBuffer failed");
        return;
    }

    cout << "---->SetLocationUpdates Result:";
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnRequestSetNotificationFilterTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnRequestSetNotificationFilterTest read spBuffer failed");
        return;
    }

    cout << "---->SetNotificationFilter Result:";
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnRequestSetDeviceStateTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("RilRadioResponseTest OnRequestSetDeviceStateTest read spBuffer failed");
        return;
    }

    cout << "---->SetDeviceState Result:";
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnRequestGetPreferredNetworkTypeInfoTest(OHOS::MessageParcel &data)
{
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponsePullRadioLceData --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }

    std::unique_ptr<PreferredNetworkTypeInfo> networkType = std::make_unique<PreferredNetworkTypeInfo>();
    if (networkType == nullptr) {
        return;
    }
    networkType.get()->ReadFromParcel(data);
    cout << "OnRequestGetPreferredNetworkTypeInfoTest: preferredNetworkType = " << networkType->preferredNetworkType
         << endl;
}

void RilRadioResponseTest::OnRequestGetLinkBandwidthInfoTest(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnRequestGetLinkBandwidthInfoTest --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnRequestGetLinkBandwidthInfoTest --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
}

void RilRadioResponseTest::OnResponseSendRilCmSms(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseSendRilCmSms --> ");
    std::shared_ptr<SendSmsResultInfo> sendSmsResultInfo = std::make_shared<SendSmsResultInfo>();
    sendSmsResultInfo->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseSendRilCmSms --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
    cout << "---->OnResponseSendRilCmSms Result:" << endl
         << "----> [msgRef]: " << sendSmsResultInfo->msgRef << endl
         << "----> [pdu]: " << sendSmsResultInfo->pdu.c_str() << endl
         << "----> [errCode]: " << sendSmsResultInfo->errCode << endl;
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnResponseSendRilCmSmsMoreMode(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseSendRilCmSmsMoreMode --> ");
    std::shared_ptr<SendSmsResultInfo> sendSmsResultInfo = std::make_shared<SendSmsResultInfo>();
    sendSmsResultInfo->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseSendRilCmSmsMoreMode --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
    cout << "---->OnResponseSendRilCmSmsMoreMode Result:" << endl
         << "----> [msgRef]: " << sendSmsResultInfo->msgRef << endl
         << "----> [pdu]: " << sendSmsResultInfo->pdu.c_str() << endl
         << "----> [errCode]: " << sendSmsResultInfo->errCode << endl;
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnResponseSendSmsAck(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseSendSmsAck --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseSendSmsAck --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
    cout << "---->OnResponseSendSmsAck Result:" << endl;
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnResponseGetSmscAddr(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseGetSmscAddr --> ");
    std::shared_ptr<ServiceCenterAddress> serCenterAddress = std::make_shared<ServiceCenterAddress>();
    serCenterAddress->ReadFromParcel(data);
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseGetSmscAddr --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
    cout << "---->OnResponseGetSmscAddr Result:" << endl
         << "----> [tosca]: " << serCenterAddress->tosca << endl
         << "----> [address]: " << serCenterAddress->address.c_str() << endl;
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}

void RilRadioResponseTest::OnResponseSetSmscAddr(OHOS::MessageParcel &data)
{
    TELEPHONY_LOGI("RilRadioResponseTest::OnResponseSetSmscAddr --> ");
    const size_t readSpSize = sizeof(struct HRilRadioResponseInfo);
    const uint8_t *spBuffer = data.ReadBuffer(readSpSize);
    if (spBuffer == nullptr) {
        TELEPHONY_LOGE("ERROR : OnResponseSetSmscAddr --> data.ReadBuffer(readSpSize) failed !!!");
        return;
    }
    cout << "---->OnResponseSetSmscAddr Result:" << endl;
    PrintResponseInfo((struct HRilRadioResponseInfo *)spBuffer);
}
