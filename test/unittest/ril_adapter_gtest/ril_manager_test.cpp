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

int32_t RilManagerTest::SetCellularRadioIndication()
{
    TELEPHONY_LOGI("RilManagerTest SetCellularRadioIndication start!");
    if (cellularRadio_ != nullptr) {
        std::unique_ptr<IPCObjectStub> callback = std::make_unique<RilRadioIndicationTest>(this);
        if (callback == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
        MessageParcel data;
        MessageParcel reply;
        sptr<IPCObjectStub> object = callback.release();
        data.WriteRemoteObject(object);
        MessageOption option;
        int32_t ret = cellularRadio_->SendRequest(RilManagerTest::HRIL_ADAPTER_RADIO_INDICATION, data, reply, option);
        TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HRIL_ADAPTER_RADIO_INDICATION, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::SetCellularRadioResponse()
{
    TELEPHONY_LOGI("RilManagerTest SetCellularRadioResponse start!");
    if (cellularRadio_ != nullptr) {
        std::unique_ptr<IPCObjectStub> callback = std::make_unique<RilRadioResponseTest>(this);
        if (callback == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
        MessageParcel data;
        MessageParcel reply;
        sptr<IPCObjectStub> object = callback.release();
        data.WriteRemoteObject(object);
        MessageOption option;
        int32_t ret = cellularRadio_->SendRequest(RilManagerTest::HRIL_ADAPTER_RADIO_RESPONSE, data, reply, option);
        TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HRIL_ADAPTER_RADIO_RESPONSE, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
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

int32_t RilManagerTest::GetCallList(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest GetCallList");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_GET_CALL_LIST, result);
        int32_t ret = SendInt32Event(HREQ_CALL_GET_CALL_LIST, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_CALL_GET_CALL_LIST, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::GetSimIO(SimIoRequestInfo data, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::GetSimIO --> ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_GET_SIM_IO, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("RilManagerTest::CreateRequest is null ");
            return HRIL_ERR_NULL_POINT;
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
            return HRIL_ERR_NULL_POINT;
        }
        TELEPHONY_LOGI("GetSimIO --> GetSimIO.Marshalling (wData) success");
        int32_t ret = SendBufferEvent(HREQ_SIM_GET_SIM_IO, wData);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_GET_SIM_IO, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("ERROR : GetSimIO --> cellularRadio_ == nullptr !!!");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::GetImsi(std::string aid, const AppExecFwk::InnerEvent::Pointer &result)
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
            return HRIL_ERR_NULL_POINT;
        }
        TELEPHONY_LOGI("GetImsi --> imsi.Marshalling(wData) success.");
        TELEPHONY_LOGI("GetImsi --> imsi.serial = %{public}d", imsi.serial);
        int32_t ret = SendBufferEvent(HREQ_SIM_GET_IMSI, wData);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_GET_IMSI, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("ERROR : GetImsi --> cellularRadio_ == nullptr !!!");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::GetRilCmSignalStrength(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int32_t what)
{
    TELEPHONY_LOGI("RilManagerTest::GetRilCmSignalStrength");
    if (cellularRadio_ != nullptr) {
        auto event = AppExecFwk::InnerEvent::Get(what);
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_SIGNAL_STRENGTH, event);
        int32_t ret = SendInt32Event(HREQ_NETWORK_GET_SIGNAL_STRENGTH, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_GET_SIGNAL_STRENGTH, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::GetSimStatus(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetSimStatus -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_GET_SIM_STATUS, result);
        int32_t ret = SendInt32Event(HREQ_SIM_GET_SIM_STATUS, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_SIM_GET_SIM_STATUS, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("ERROR : GetSimStatus --> cellularRadio_ == nullptr !!!");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::RilCmDial(std::string address, int32_t clirMode, const AppExecFwk::InnerEvent::Pointer &result)
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
            return HRIL_ERR_NULL_POINT;
        }
        int32_t ret = SendBufferEvent(HREQ_CALL_DIAL, wData);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_CALL_DIAL, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::Reject(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Reject  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_REJECT, result);
        int32_t ret = SendInt32Event(HREQ_CALL_REJECT, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_CALL_REJECT, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::Hangup(int32_t gsmIndex, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Hangup  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_HANGUP, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteInt32(gsmIndex);
        MessageOption option;
        int32_t ret = cellularRadio_->SendRequest(HREQ_CALL_HANGUP, data, reply, option);
        TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_CALL_HANGUP, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("Hangup  cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::HoldCall(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("HoldCall  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_HOLD_CALL, result);
        int32_t ret = SendInt32Event(HREQ_CALL_HOLD_CALL, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_CALL_HOLD_CALL, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::UnHoldCall(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("UnHoldCall  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_UNHOLD_CALL, result);
        int32_t ret = SendInt32Event(HREQ_CALL_UNHOLD_CALL, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_CALL_UNHOLD_CALL, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::SwitchCall(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("SwitchCall  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SWITCH_CALL, result);
        int32_t ret = SendInt32Event(HREQ_CALL_SWITCH_CALL, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_CALL_SWITCH_CALL, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::Answer(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Answer  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_ANSWER, result);
        int32_t ret = SendInt32Event(HREQ_CALL_ANSWER, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_CALL_ANSWER, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::RilCmJoin(int32_t callType, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilCmJoin  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_COMBINE_CONFERENCE, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteInt32(callType);
        MessageOption option;
        int32_t ret = cellularRadio_->SendRequest(HREQ_CALL_COMBINE_CONFERENCE, data, reply, option);
        TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_CALL_COMBINE_CONFERENCE, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("RilCmJoin  cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::RilCmSplit(int32_t callIndex, int32_t callType, const AppExecFwk::InnerEvent::Pointer &result)
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
        int32_t ret = cellularRadio_->SendRequest(HREQ_CALL_SEPARATE_CONFERENCE, data, reply, option);
        TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_CALL_SEPARATE_CONFERENCE, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("RilCmSplit  cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
}

void RilManagerTest::RejectRilCmCall(const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RejectRilCmCall");
}

int32_t RilManagerTest::GetCsRegStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest GetCsRegStatus");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_CS_REG_STATUS, response);
        int32_t ret = SendInt32Event(HREQ_NETWORK_GET_CS_REG_STATUS, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_GET_CS_REG_STATUS, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::GetPsRegStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_PS_REG_STATUS, response);
        int32_t ret = SendInt32Event(HREQ_NETWORK_GET_PS_REG_STATUS, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_GET_PS_REG_STATUS, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::GetOperatorInfo(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int32_t what)
{
    TELEPHONY_LOGI("RilManagerTest GetOperatorInfo");
    if (cellularRadio_ != nullptr) {
        auto event = AppExecFwk::InnerEvent::Get(what);
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_OPERATOR_INFO, event);
        int32_t ret = SendInt32Event(HREQ_NETWORK_GET_OPERATOR_INFO, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_GET_OPERATOR_INFO, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::SendSms(std::string smscPdu, std::string pdu,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler, const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_SEND_GSM_SMS, response);
        MessageParcel data;
        GsmSmsMessageInfo mGsmSmsMessageInfo = ConstructGsmSendSmsRilRequest(smscPdu, pdu);
        mGsmSmsMessageInfo.serial = request->serialId_;
        if (!mGsmSmsMessageInfo.Marshalling(data)) {
            TELEPHONY_LOGE("RilCmSendSMS Marshalling failed!!!");
            return HRIL_ERR_NULL_POINT;
        }
        int32_t ret = SendBufferEvent(HREQ_SMS_SEND_GSM_SMS, data);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SMS_SEND_GSM_SMS, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::SendSmsMoreMode(std::string smscPdu, std::string pdu,
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
            return HRIL_ERR_NULL_POINT;
        }
        int32_t ret = SendBufferEvent(HREQ_SMS_SEND_SMS_MORE_MODE, data);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SMS_SEND_SMS_MORE_MODE, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

GsmSmsMessageInfo RilManagerTest::ConstructGsmSendSmsRilRequest(string smscPdu, string pdu)
{
    GsmSmsMessageInfo msg;
    msg.smscPdu = smscPdu.empty() ? "" : smscPdu;
    msg.pdu = pdu.empty() ? "" : pdu;
    return msg;
}

int32_t RilManagerTest::SetRadioState(int32_t fan, int32_t rst, const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_SET_RADIO_STATUS, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteInt32(fan);
        data.WriteInt32(rst);
        MessageOption option;
        int32_t ret = cellularRadio_->SendRequest(HREQ_MODEM_SET_RADIO_STATUS, data, reply, option);
        TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_MODEM_SET_RADIO_STATUS, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("SetRadioState  cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::SendSmsAck(bool success, int32_t cause, const AppExecFwk::InnerEvent::Pointer &response)
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
            return HRIL_ERR_NULL_POINT;
        }
        int32_t ret = SendBufferEvent(HREQ_SMS_SEND_SMS_ACK, wData);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SMS_SEND_SMS_ACK, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::ActivatePdpContext(int32_t radioTechnology, RilDataProfileTest dataProfile, bool isRoaming,
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
            return HRIL_ERR_NULL_POINT;
        }
        int32_t ret = SendBufferEvent(HREQ_DATA_ACTIVATE_PDP_CONTEXT, wData);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_DATA_ACTIVATE_PDP_CONTEXT, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("ERROR : ActivatePdpContext --> cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::DeactivatePdpContext(
    int32_t ci, int32_t reason, const AppExecFwk::InnerEvent::Pointer &response)
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
            return HRIL_ERR_NULL_POINT;
        }
        int32_t ret = SendBufferEvent(HREQ_DATA_DEACTIVATE_PDP_CONTEXT, wData);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_DATA_DEACTIVATE_PDP_CONTEXT, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("ERROR : DeactivatePdpContext --> cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::RilSendAck()
{
    TELEPHONY_LOGI("RilManagerTest RilSendAck");
    if (cellularRadio_ != nullptr) {
        int32_t ret = SendInt32Event(HRIL_RESPONSE_ACKNOWLEDGEMENT, 0);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HRIL_RESPONSE_ACKNOWLEDGEMENT, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("RilSendAck Error trying to send ack, radioProxyCellularRadio_y = null");
        return HRIL_ERR_NULL_POINT;
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

int32_t RilManagerTest::SetUssd(std::string str, const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SET_USSD, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteCString(str.c_str());
        MessageOption option = {MessageOption::TF_ASYNC};
        int32_t ret = cellularRadio_->SendRequest(HREQ_CALL_SET_USSD, data, reply, option);
        TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_CALL_SET_USSD, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("SetUssd  cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::GetUssd(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetUssd -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_GET_USSD, result);
        int32_t ret = SendInt32Event(HREQ_CALL_GET_USSD, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_CALL_GET_USSD, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("ERROR : GetUssd --> cellularRadio_ == nullptr !!!");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::GetLinkBandwidthInfo(const int32_t cid, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetLinkBandwidthInfo -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, result);
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(request->serialId_);
        data.WriteInt32(cid);
        MessageOption option = {MessageOption::TF_ASYNC};
        int32_t ret = cellularRadio_->SendRequest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, data, reply, option);
        TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_DATA_GET_LINK_BANDWIDTH_INFO, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("ERROR : GetLinkBandwidthInfo --> cellularRadio_ == nullptr !!!");
        return HRIL_ERR_NULL_POINT;
    }
}
} // namespace Telephony
} // namespace OHOS