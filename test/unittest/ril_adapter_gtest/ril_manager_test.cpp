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

#include "ril_manager_test.h"

#include "ril_radio_indication_test.h"
#include "ril_radio_response_test.h"
#include "telephony_log_wrapper.h"

#include "hril_request.h"

namespace OHOS {
namespace Telephony {
using namespace std;
using OHOS::IRemoteObject;
using OHOS::sptr;

struct EventPackage {
    uint32_t serial;
};

std::atomic_int32_t RilManagerTest::nextSerialId_(1);
std::unordered_map<int32_t, std::shared_ptr<HRilRequestTest>> RilManagerTest::requestMap_;

int32_t RilManagerTest::SendInt32Event(int32_t dispatchId, int32_t value)
{
    int32_t status = 0;
    if (cellularRadio_ != nullptr) {
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(value);
        MessageOption option = {MessageOption::TF_ASYNC};
        status = cellularRadio_->SendRequest(dispatchId, data, reply, option);
    }
    return status;
}

int32_t RilManagerTest::SendStringEvent(int32_t dispatchId, const char *value)
{
    int32_t status = 0;
    if (value == nullptr) {
        TELEPHONY_LOGE("value is nullptr");
        return status;
    }
    if (cellularRadio_ != nullptr) {
        MessageParcel data;
        MessageParcel reply;
        data.WriteCString(value);
        MessageOption option = {MessageOption::TF_ASYNC};
        status = cellularRadio_->SendRequest(dispatchId, data, reply, option);
    }
    return status;
}

int32_t RilManagerTest::SendBufferEvent(int32_t dispatchId, MessageParcel &eventData)
{
    int32_t status = 0;
    if (cellularRadio_ != nullptr) {
        MessageParcel reply;
        MessageOption option = {MessageOption::TF_ASYNC};
        status = cellularRadio_->SendRequest(dispatchId, eventData, reply, option);
    }
    return status;
}

int32_t RilManagerTest::SendCommonBufferEvent(int32_t dispatchId, const void *eventData, size_t dataLength)
{
    int32_t status = 0;
    const int32_t INCREMENT_VALUE = 1;
    if (cellularRadio_ != nullptr) {
        MessageParcel data;
        MessageParcel reply;
        data.WriteBuffer(eventData, (dataLength + INCREMENT_VALUE));
        MessageOption option = {MessageOption::TF_ASYNC};
        status = cellularRadio_->SendRequest(dispatchId, data, reply, option);
    }
    return status;
}

void RilManagerTest::SetCellularRadioIndication()
{
    TELEPHONY_LOGI("RilManagerTest SetCellularRadioIndication start!");
    if (cellularRadio_ != nullptr) {
        std::unique_ptr<IPCObjectStub> callback = std::make_unique<RilRadioResponseTest>(this);
        if (callback == nullptr) {
            return;
        }
        MessageParcel data;
        MessageParcel reply;
        sptr<IPCObjectStub> object = callback.release();
        data.WriteRemoteObject(object);
        MessageOption option;
        cellularRadio_->SendRequest(RilManagerTest::HRIL_ADAPTER_RADIO_INDICATION, data, reply, option);
    }
}

void RilManagerTest::SetCellularRadioResponse()
{
    TELEPHONY_LOGI("RilManagerTest SetCellularRadioResponse start!");
    if (cellularRadio_ != nullptr) {
        std::unique_ptr<IPCObjectStub> callback = std::make_unique<RilRadioResponseTest>(this);
        if (callback == nullptr) {
            return;
        }
        MessageParcel data;
        MessageParcel reply;
        sptr<IPCObjectStub> object = callback.release();
        data.WriteRemoteObject(object);
        MessageOption option;
        cellularRadio_->SendRequest(RilManagerTest::HRIL_ADAPTER_RADIO_RESPONSE, data, reply, option);
    }
}

RilManagerTest::RilManagerTest(int32_t preferredNetworkType, int32_t cdmaSubscription)
{
    TELEPHONY_LOGI("RilManagerTest init");
    auto servMgr_ = HDI::ServiceManager::V1_0::IServiceManager::Get();
    cellularRadio_ = servMgr_->GetService(RIL_ADAPTER_SERVICE_NAME);
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest bind hdf error!");
        return;
    }
    TELEPHONY_LOGI("RilManagerTest bind hdf success!");
}

void RilManagerTest::OnInit()
{
    TELEPHONY_LOGI("OnInit start");
    SetCellularRadioIndication();
    SetCellularRadioResponse();
}

RilManagerTest::~RilManagerTest()
{
    TELEPHONY_LOGI("~RilManagerTest!");
}

DataProfileDataInfo RilManagerTest::ConvertToHalDataProfile(RilDataProfileTest dpi)
{
    DataProfileDataInfo dataProfileDataInfo;
    dataProfileDataInfo.roamingProtocol = dpi.roamingProtocol;
    dataProfileDataInfo.verType = dpi.verType;
    dataProfileDataInfo.userName = dpi.userName;
    dataProfileDataInfo.password = dpi.pwdCode;
    dataProfileDataInfo.profileId = dpi.profileId;
    dataProfileDataInfo.apn = dpi.apn;
    dataProfileDataInfo.protocol = dpi.protocol;
    return dataProfileDataInfo;
}

std::shared_ptr<HRilRequestTest> RilManagerTest::CreateRequest(
    int32_t request, const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest =
        std::make_shared<HRilRequestTest>(GetNextSerialId(), request, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("TelRilBase telRilRequest is nullptr");
        return nullptr;
    }

    RilManagerTest::requestMap_.insert(std::make_pair(telRilRequest->serialId_, telRilRequest));
    return telRilRequest;
}

static std::string ConvertNullToEmptyString(std::string str)
{
    return (!str.empty()) ? (str) : ("");
}

void RilManagerTest::GetCallList(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest GetCallList");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_GET_CALL_LIST, result);
        SendInt32Event(HREQ_CALL_GET_CALL_LIST, request->serialId_);
    }
}

void RilManagerTest::GetSimIO(SimIoRequestInfo data, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::GetSimIO --> ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_GET_SIM_IO, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("RilManagerTest::CreateRequest is null ");
            return;
        }
        MessageParcel wData;
        SimIoRequestInfo iccIoRequestInfo;
        iccIoRequestInfo.serial = request->serialId_;
        iccIoRequestInfo.command = data.command;
        iccIoRequestInfo.fileId = data.fileId;
        iccIoRequestInfo.path = ConvertNullToEmptyString(data.path);
        iccIoRequestInfo.p1 = data.p1;
        iccIoRequestInfo.p2 = data.p2;
        iccIoRequestInfo.p3 = data.p3;
        iccIoRequestInfo.data = ConvertNullToEmptyString(data.data);
        if (!iccIoRequestInfo.Marshalling(wData)) {
            TELEPHONY_LOGE("ERROR : GetSimIO --> iccIo.Marshalling (wData) failed !!!");
            return;
        }
        TELEPHONY_LOGI("GetSimIO --> GetSimIO.Marshalling (wData) success");
        SendBufferEvent(HREQ_SIM_GET_SIM_IO, wData);
    } else {
        TELEPHONY_LOGE("ERROR : GetSimIO --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::GetImsi(std::string aid, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetImsi -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_GET_IMSI, result);
        UniInfo imsi;
        imsi.serial = request->serialId_;
        imsi.strTmp = ConvertNullToEmptyString(aid);
        MessageParcel wData;
        if (!imsi.Marshalling(wData)) {
            TELEPHONY_LOGE("ERROR : GetImsi --> imsi.Marshalling(wData) failed !!!");
            return;
        }
        TELEPHONY_LOGI("GetImsi --> imsi.Marshalling(wData) success.");
        TELEPHONY_LOGI("GetImsi --> imsi.serial = %{public}d", imsi.serial);
        SendBufferEvent(HREQ_SIM_GET_IMSI, wData);
    } else {
        TELEPHONY_LOGE("ERROR : GetImsi --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::GetRilCmSignalStrength(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int32_t what)
{
    TELEPHONY_LOGI("RilManagerTest::GetRilCmSignalStrength");
    if (cellularRadio_ != nullptr) {
        auto event = AppExecFwk::InnerEvent::Get(what);
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_SIGNAL_STRENGTH, event);
        SendInt32Event(HREQ_NETWORK_GET_SIGNAL_STRENGTH, request->serialId_);
    }
}

void RilManagerTest::GetSimStatus(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetSimStatus -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_GET_SIM_STATUS, result);
        SendInt32Event(HREQ_SIM_GET_SIM_STATUS, request->serialId_);
    } else {
        TELEPHONY_LOGE("ERROR : GetSimStatus --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::RilCmDial(std::string address, int32_t clirMode, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilCmDial ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_DIAL, result);
        MessageParcel wData;
        DialInfo dialInfo;
        dialInfo.address = ConvertNullToEmptyString(address);
        dialInfo.clir = clirMode;
        dialInfo.serial = request->serialId_;

        if (!dialInfo.Marshalling(wData)) {
            TELEPHONY_LOGE("RilCmDial -> RilManagerTest Marshalling");
            return;
        }
        SendBufferEvent(HREQ_CALL_DIAL, wData);
    }
}

void RilManagerTest::Reject(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Reject  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_REJECT, result);
        SendInt32Event(HREQ_CALL_REJECT, request->serialId_);
    }
}

void RilManagerTest::Hangup(int32_t gsmIndex, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Hangup  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_HANGUP, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteInt32(gsmIndex);
        MessageOption option;
        cellularRadio_->SendRequest(HREQ_CALL_HANGUP, data, reply, option);
    } else {
        TELEPHONY_LOGE("Hangup  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::HoldCall(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Hold  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_HOLD_CALL, result);
        SendInt32Event(HREQ_CALL_HOLD_CALL, request->serialId_);
    }
}

void RilManagerTest::UnHoldCall(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Active  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_UNHOLD_CALL, result);
        SendInt32Event(HREQ_CALL_UNHOLD_CALL, request->serialId_);
    }
}

void RilManagerTest::SwitchCall(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Swap  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SWITCH_CALL, result);
        SendInt32Event(HREQ_CALL_SWITCH_CALL, request->serialId_);
    }
}

void RilManagerTest::Answer(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Answer  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_ANSWER, result);
        SendInt32Event(HREQ_CALL_ANSWER, request->serialId_);
    }
}

void RilManagerTest::RilCmJoin(int32_t callType, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilCmJoin  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_COMBINE_CONFERENCE, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteInt32(callType);
        MessageOption option;
        cellularRadio_->SendRequest(HREQ_CALL_COMBINE_CONFERENCE, data, reply, option);
    } else {
        TELEPHONY_LOGE("RilCmJoin  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::RilCmSplit(int32_t callIndex, int32_t callType, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilCmSplit  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SEPARATE_CONFERENCE, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteInt32(callIndex);
        data.WriteInt32(callType);
        MessageOption option;
        cellularRadio_->SendRequest(HREQ_CALL_SEPARATE_CONFERENCE, data, reply, option);
    } else {
        TELEPHONY_LOGE("RilCmSplit  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::RejectRilCmCall(const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RejectRilCmCall");
}

void RilManagerTest::GetCsRegStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest GetCsRegStatus");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_CS_REG_STATUS, response);
        SendInt32Event(HREQ_NETWORK_GET_CS_REG_STATUS, request->serialId_);
    }
}

void RilManagerTest::GetPsRegStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_PS_REG_STATUS, response);
        SendInt32Event(HREQ_NETWORK_GET_PS_REG_STATUS, request->serialId_);
    }
}

void RilManagerTest::GetOperatorInfo(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int32_t what)
{
    TELEPHONY_LOGI("RilManagerTest GetOperatorInfo");
    if (cellularRadio_ != nullptr) {
        auto event = AppExecFwk::InnerEvent::Get(what);
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_OPERATOR_INFO, event);
        SendInt32Event(HREQ_NETWORK_GET_OPERATOR_INFO, request->serialId_);
    }
}

void RilManagerTest::SendSms(std::string smscPdu, std::string pdu,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler, const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_SEND_GSM_SMS, response);
        MessageParcel data;
        GsmSmsMessageInfo mGsmSmsMessageInfo = ConstructGsmSendSmsRilRequest(smscPdu, pdu);
        mGsmSmsMessageInfo.serial = request->serialId_;
        if (!mGsmSmsMessageInfo.Marshalling(data)) {
            TELEPHONY_LOGE("RilCmSendSMS Marshalling failed!!!");
            return;
        }
        SendBufferEvent(HREQ_SMS_SEND_GSM_SMS, data);
    }
}

void RilManagerTest::SendSmsMoreMode(std::string smscPdu, std::string pdu,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler, const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_SEND_SMS_MORE_MODE, response);
        /* Do not log function arg for privacy */
        MessageParcel data;
        GsmSmsMessageInfo mGsmSmsMessageInfo = ConstructGsmSendSmsRilRequest(smscPdu, pdu);
        mGsmSmsMessageInfo.serial = request->serialId_;
        if (!mGsmSmsMessageInfo.Marshalling(data)) {
            TELEPHONY_LOGE("RilCmSendSMS Marshalling failed!!!");
            return;
        }
        SendBufferEvent(HREQ_SMS_SEND_SMS_MORE_MODE, data);
    }
}

GsmSmsMessageInfo RilManagerTest::ConstructGsmSendSmsRilRequest(string smscPdu, string pdu)
{
    GsmSmsMessageInfo msg;
    msg.smscPdu = smscPdu.empty() ? "" : smscPdu;
    msg.pdu = pdu.empty() ? "" : pdu;
    return msg;
}

void RilManagerTest::SetRadioState(int fan, int rst, const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_SET_RADIO_STATUS, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteInt32(fan);
        data.WriteInt32(rst);
        MessageOption option;
        cellularRadio_->SendRequest(HREQ_MODEM_SET_RADIO_STATUS, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetRadioState  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SendSmsAck(bool success, int32_t cause, const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_SEND_SMS_ACK, response);
        MessageParcel wData;
        UniInfo mUniversalInfo;
        mUniversalInfo.serial = request->serialId_;
        mUniversalInfo.flag = success;
        mUniversalInfo.gsmIndex = cause;
        if (!mUniversalInfo.Marshalling(wData)) {
            TELEPHONY_LOGE("RilManagerTest UniversalInfo Marshalling failed!!!");
            return;
        }
        SendBufferEvent(HREQ_SMS_SEND_SMS_ACK, wData);
    }
}

void RilManagerTest::ActivatePdpContext(int32_t radioTechnology, RilDataProfileTest dataProfile, bool isRoaming,
    bool allowRoaming, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::ActivatePdpContext -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_ACTIVATE_PDP_CONTEXT, response);
        DataCallInfo dataCallInfo;
        dataCallInfo.serial = request->serialId_;
        dataCallInfo.radioTechnology = radioTechnology;
        dataCallInfo.dataProfileInfo = ConvertToHalDataProfile(dataProfile);
        dataCallInfo.roamingAllowed = allowRoaming;
        dataCallInfo.isRoaming = isRoaming;
        MessageParcel wData;
        if (!dataCallInfo.Marshalling(wData)) {
            TELEPHONY_LOGE("ERROR : ActivatePdpContext --> dataCallInfo.Marshalling(wData) failed !!!");
            return;
        }
        SendBufferEvent(HREQ_DATA_ACTIVATE_PDP_CONTEXT, wData);
    } else {
        TELEPHONY_LOGE("ERROR : ActivatePdpContext --> cellularRadio_ == nullptr");
    }
}

void RilManagerTest::DeactivatePdpContext(int32_t ci, int32_t reason, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilBaseCommands::DeactivatePdpContext -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_DEACTIVATE_PDP_CONTEXT, response);
        UniInfo uniInfo;
        uniInfo.serial = request->serialId_;
        uniInfo.gsmIndex = ci;
        uniInfo.arg1 = reason;
        MessageParcel wData;
        if (!uniInfo.Marshalling(wData)) {
            TELEPHONY_LOGE("ERROR : DeactivatePdpContext --> uniInfo.Marshalling(wData) failed !!!");
            return;
        }
        SendBufferEvent(HREQ_DATA_DEACTIVATE_PDP_CONTEXT, wData);
    } else {
        TELEPHONY_LOGE("ERROR : DeactivatePdpContext --> cellularRadio_ == nullptr");
    }
}

void RilManagerTest::RilSendAck()
{
    TELEPHONY_LOGI("RilManagerTest RilSendAck");
    if (cellularRadio_ != nullptr) {
        SendInt32Event(HRIL_RESPONSE_ACKNOWLEDGEMENT, 0);
    } else {
        TELEPHONY_LOGE("RilSendAck Error trying to send ack, radioProxyCellularRadio_y = null");
    }
}

void RilManagerTest::RilProcessResponseDone(
    std::shared_ptr<HRilRequestTest> request, HRilRadioResponseInfo &responseInfo)
{
    if (request == nullptr) {
        return;
    }
    if (responseInfo.error == HRilErrType::NONE) {
        TELEPHONY_LOGI("RilProcessResponseDone success");
    }
}

void RilManagerTest::SetUssdCusd(std::string str, const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SET_USSD, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteCString(str.c_str());
        MessageOption option = {MessageOption::TF_ASYNC};
        cellularRadio_->SendRequest(HREQ_CALL_SET_USSD, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetUssdCusd  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::GetUssdCusd(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetUssdCusd -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_GET_USSD, result);
        SendInt32Event(HREQ_CALL_GET_USSD, request->serialId_);
    } else {
        TELEPHONY_LOGE("ERROR : GetUssdCusd --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::GetLinkBandwidthInfo(const int32_t cid, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetLinkBandwidthInfo -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteInt32(cid);
        MessageOption option = {MessageOption::TF_ASYNC};
        cellularRadio_->SendRequest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, data, reply, option);
    } else {
        TELEPHONY_LOGE("ERROR : GetLinkBandwidthInfo --> cellularRadio_ == nullptr !!!");
    }
}
} // namespace Telephony
} // namespace OHOS