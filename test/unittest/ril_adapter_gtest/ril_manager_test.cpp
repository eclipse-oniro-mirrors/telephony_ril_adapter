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

int32_t RilManagerTest::SendInt32sEvent(int32_t dispatchId, int32_t argCount, ...)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("cellularRadio_ is nullptr!!!");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    MessageParcel data;
    MessageParcel reply;
    va_list list;
    va_start(list, argCount);
    int32_t i = 0;

    while (i < argCount) {
        int32_t value = va_arg(list, int32_t);
        data.WriteInt32(value);
        i++;
    }
    va_end(list);
    OHOS::MessageOption option = {OHOS::MessageOption::TF_ASYNC};
    TELEPHONY_LOGI("Send Event %{public}d", dispatchId);
    return cellularRadio_->SendRequest(dispatchId, data, reply, option);
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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

int32_t RilManagerTest::SetEmergencyCallList(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("SetEmergencyInfoList");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SET_EMERGENCY_LIST, result);
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
        int32_t total = 1;
        int32_t category = 2;
        int32_t serviceType = 0;
        EmergencyInfoList emergencyInfoList;
        emergencyInfoList.callSize = total;
        emergencyInfoList.flag = request->serialId_;
        EmergencyInfo emergencyInfo = {};
        emergencyInfo.index = 1;
        emergencyInfo.total = total;
        emergencyInfo.eccNum = "120";
        emergencyInfo.category = category;
        emergencyInfo.simpresent = serviceType;
        emergencyInfo.mcc = "460";
        emergencyInfo.abnormalService = 0;
        emergencyInfoList.calls.push_back(emergencyInfo);
        MessageParcel wData;
        if (!emergencyInfoList.Marshalling(wData)) {
            TELEPHONY_LOGE("ERROR : ActivatePdpContext --> emergencyInfoList.Marshalling(wData) failed !!!");
            return HRIL_ERR_NULL_POINT;
        }
        int32_t ret = SendBufferEvent(HREQ_CALL_SET_EMERGENCY_LIST, wData);
        TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_CALL_SET_EMERGENCY_LIST, ret);
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
        int32_t ret = SendInt32Event(HREQ_NETWORK_GET_OPERATOR_INFO, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_GET_OPERATOR_INFO, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::SetNotificationFilter(int32_t filter, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest SetNotificationFilter");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("SetRilNotificationFilter cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_SET_NOTIFICATION_FILTER, result);
    if (request == nullptr) {
        TELEPHONY_LOGE("SetRilNotificationFilter request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    MessageParcel reply;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    data.WriteInt32(filter);
    MessageOption option;
    int32_t ret = cellularRadio_->SendRequest(HREQ_NETWORK_SET_NOTIFICATION_FILTER, data, reply, option);
    TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_SET_NOTIFICATION_FILTER, ret);
    return ret;
}

int32_t RilManagerTest::SetDeviceState(int32_t deviceStateType, bool deviceStateOn,
    const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest SetDeviceState");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("SetDeviceState cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_SET_DEVICE_STATE, result);
    if (request == nullptr) {
        TELEPHONY_LOGE("SetDeviceState request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    MessageParcel reply;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    data.WriteInt32(deviceStateType);
    data.WriteInt32(deviceStateOn);
    MessageOption option;
    int32_t ret = cellularRadio_->SendRequest(HREQ_NETWORK_SET_DEVICE_STATE, data, reply, option);
    TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_SET_DEVICE_STATE, ret);
    return ret;
}

int32_t RilManagerTest::GetNetworkSearchInfo(const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest GetNetworkSearchInfo");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetNetworkSearchInfo cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION, response);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetNetworkSearchInfo request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendInt32Event(HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION, request->serialId_);
    TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d",
        HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION, ret);
    return ret;
}

int32_t RilManagerTest::GetNetworkSelectionMode(const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest GetNetworkSelectionMode");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetNetworkSelectionMode cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_NETWORK_SELECTION_MODE, response);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetNetworkSelectionMode request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendInt32Event(HREQ_NETWORK_GET_NETWORK_SELECTION_MODE, request->serialId_);
    TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_GET_NETWORK_SELECTION_MODE, ret);
    return ret;
}

int32_t RilManagerTest::SetNetworkSelectionMode(
    int32_t mode, std::string plmn, const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::SetNetworkSelectionMode cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_SET_NETWORK_SELECTION_MODE, result);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::SetNetworkSelectionMode request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    OHOS::MessageParcel data;
    SetNetworkModeInfo networkMode;
    networkMode.serial = request->serialId_;
    networkMode.selectMode = mode;
    networkMode.oper = ConvertNullToEmptyString(plmn);
    int slotId = 0;
    data.WriteInt32(slotId);
    if (!networkMode.Marshalling(data)) {
        TELEPHONY_LOGE("RilManagerTest::SetNetworkSelectionMode --> Marshalling (data) failed !!!");
        return HRIL_ERR_INVALID_PARAMETER;
    }
    int32_t ret = SendBufferEvent(HREQ_NETWORK_SET_NETWORK_SELECTION_MODE, data);
    TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_SET_NETWORK_SELECTION_MODE, ret);
    return ret;
}

int32_t RilManagerTest::GetRadioCapability(const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest GetRadioCapability");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetRadioCapability cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_RADIO_CAPABILITY, response);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetRadioCapability request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendInt32Event(HREQ_NETWORK_GET_RADIO_CAPABILITY, request->serialId_);
    TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_GET_RADIO_CAPABILITY, ret);
    return ret;
}

int32_t RilManagerTest::GetPhysicalChannelConfig(const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest GetPhysicalChannelConfig");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetPhysicalChannelConfig cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG, response);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetPhysicalChannelConfig request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendInt32Event(HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG, request->serialId_);
    TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG, ret);
    return ret;
}

int32_t RilManagerTest::SetDataProfileInfo(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR : SetDataProfileInfo --> cellularRadio_ is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_SET_DATA_PROFILE_INFO, response);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = request->serialId_;
    dataProfilesInfo.profilesSize = 1;
    DataProfileDataInfo dataInfo;
    dataInfo.serial = 0;
    dataInfo.profileId = 0;
    dataInfo.apn = "111";
    dataInfo.protocol = "222";
    dataInfo.roamingProtocol = "0";
    dataInfo.verType = 0;
    dataInfo.userName = "3333";
    dataInfo.password = "3333";
    dataProfilesInfo.profiles.push_back(dataInfo);
    OHOS::MessageParcel wData;
    if (!dataProfilesInfo.Marshalling(wData)) {
        TELEPHONY_LOGE("ERROR: SetDataProfileInfo --> dataInfo.Marshalling(wData) failed!!!");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendBufferEvent(HREQ_DATA_SET_DATA_PROFILE_INFO, wData);
    TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_DATA_SET_DATA_PROFILE_INFO, ret);
    return ret;
}

int32_t RilManagerTest::SendDataPerformanceMode(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR : SendDataPerformanceMode --> cellularRadio_ is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_SEND_DATA_PERFORMANCE_MODE, response);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 1;
    dataPerformanceInfo.enforce = 1;
    OHOS::MessageParcel wData;
    if (!dataPerformanceInfo.Marshalling(wData)) {
        TELEPHONY_LOGE("ERROR: SendDataPerformanceMode --> dataInfo.Marshalling(wData) failed!!!");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendBufferEvent(HREQ_DATA_SEND_DATA_PERFORMANCE_MODE, wData);
    TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_DATA_SEND_DATA_PERFORMANCE_MODE, ret);
    return ret;
}

int32_t RilManagerTest::SendDataSleepMode(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR : SendDataSleepMode --> cellularRadio_ is nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_SEND_DATA_SLEEP_MODE, response);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    OHOS::MessageParcel wData;
    if (!dataSleepInfo.Marshalling(wData)) {
        TELEPHONY_LOGE("ERROR: SendDataSleepMode --> dataInfo.Marshalling(wData) failed!!!");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendBufferEvent(HREQ_DATA_SEND_DATA_SLEEP_MODE, wData);
    TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_DATA_SEND_DATA_SLEEP_MODE, ret);
    return ret;
}

int32_t RilManagerTest::SetBarringPassword(const std::string &fac, const std::string &oldPwd, const std::string &newPwd,
    const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::SetBarringPassword -->");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_SET_BARRING_PASSWORD, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    MessageParcel data;
    data.WriteInt32(telRilRequest->serialId_);
    data.WriteCString(fac.c_str());
    data.WriteCString(oldPwd.c_str());
    data.WriteCString(newPwd.c_str());

    int32_t ret = SendBufferEvent(HREQ_CALL_SET_BARRING_PASSWORD, data);
    TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_CALL_SET_BARRING_PASSWORD, ret);
    return ret;
}

int32_t RilManagerTest::SendSms(std::string smscPdu, std::string pdu,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler, const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_SEND_GSM_SMS, response);
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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

int32_t RilManagerTest::ShutDown(const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::ShutDown cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_SHUT_DOWN, result);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::ShutDown request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    MessageParcel reply;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    MessageOption option;
    int32_t ret = cellularRadio_->SendRequest(HREQ_MODEM_SHUT_DOWN, data, reply, option);
    TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_MODEM_SHUT_DOWN, ret);
    return ret;
}

int32_t RilManagerTest::GetRadioState(const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetRadioState cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_GET_RADIO_STATUS, result);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetRadioState request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    MessageParcel reply;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    MessageOption option;
    int32_t ret = cellularRadio_->SendRequest(HREQ_MODEM_GET_RADIO_STATUS, data, reply, option);
    TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_MODEM_GET_RADIO_STATUS, ret);
    return ret;
}

int32_t RilManagerTest::GetImei(const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetImei cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_GET_IMEI, result);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetImei request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    MessageParcel reply;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    MessageOption option;
    int32_t ret = cellularRadio_->SendRequest(HREQ_MODEM_GET_IMEI, data, reply, option);
    TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_MODEM_GET_IMEI, ret);
    return ret;
}

int32_t RilManagerTest::GetMeid(const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetMeid cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_GET_MEID, result);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetMeid request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    MessageParcel reply;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    MessageOption option;
    int32_t ret = cellularRadio_->SendRequest(HREQ_MODEM_GET_MEID, data, reply, option);
    TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_MODEM_GET_MEID, ret);
    return ret;
}

int32_t RilManagerTest::GetVoiceRadioTechnology(const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetVoiceRadioTechnology cellularRadio_ == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_GET_VOICE_RADIO, result);
    if (request == nullptr) {
        TELEPHONY_LOGE("RilManagerTest::GetVoiceRadioTechnology request == nullptr");
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(request->serialId_);
    MessageOption option;
    int32_t ret = cellularRadio_->SendRequest(HREQ_MODEM_GET_VOICE_RADIO, data, reply, option);
    TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_MODEM_GET_VOICE_RADIO, ret);
    return ret;
}

int32_t RilManagerTest::SendSmsAck(bool success, int32_t cause, const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_SEND_SMS_ACK, response);
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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

int32_t RilManagerTest::SimOpenLogicalChannel(
    std::string appID, int32_t p2, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SimOpenLogicalChannel -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_OPEN_LOGICAL_CHANNEL, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return HRIL_ERR_NULL_POINT;
        }
        MessageParcel data;
        data.WriteInt32(0);
        data.WriteInt32(request->serialId_);
        data.WriteCString(appID.c_str());
        data.WriteInt32(p2);
        int32_t ret = SendBufferEvent(HREQ_SIM_OPEN_LOGICAL_CHANNEL, data);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_OPEN_LOGICAL_CHANNEL, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::SimCloseLogicalChannel(int32_t channelId, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SimCloseLogicalChannel -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_CLOSE_LOGICAL_CHANNEL, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return HRIL_ERR_NULL_POINT;
        }
        MessageParcel data;
        data.WriteInt32(0);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(channelId);
        int32_t ret = SendBufferEvent(HREQ_SIM_CLOSE_LOGICAL_CHANNEL, data);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_CLOSE_LOGICAL_CHANNEL, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::SimTransmitApduLogicalChannel(
    ApduSimIORequestInfo reqInfo, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SimTransmitApduLogicalChannel -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return HRIL_ERR_NULL_POINT;
        }

        MessageParcel data;
        ApduSimIORequestInfo ApduRequestInfo;
        ApduRequestInfo.serial = request->serialId_;
        ApduRequestInfo.channelId = reqInfo.channelId;
        ApduRequestInfo.type = reqInfo.type;
        ApduRequestInfo.instruction = reqInfo.instruction;
        ApduRequestInfo.p1 = reqInfo.p1;
        ApduRequestInfo.p2 = reqInfo.p2;
        ApduRequestInfo.p3 = reqInfo.p3;
        ApduRequestInfo.data = reqInfo.data;

        data.WriteInt32(0);
        ApduRequestInfo.Marshalling(data);
        int32_t ret = SendBufferEvent(HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL, data);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d",
            HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::SimTransmitApduBasicChannel(
    ApduSimIORequestInfo reqInfo, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SimTransmitApduBasicChannel -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return HRIL_ERR_NULL_POINT;
        }

        MessageParcel data;
        ApduSimIORequestInfo ApduRequestInfo;
        ApduRequestInfo.serial = request->serialId_;
        ApduRequestInfo.type = reqInfo.type;
        ApduRequestInfo.instruction = reqInfo.instruction;
        ApduRequestInfo.p1 = reqInfo.p1;
        ApduRequestInfo.p2 = reqInfo.p2;
        ApduRequestInfo.p3 = reqInfo.p3;
        ApduRequestInfo.data = reqInfo.data;

        data.WriteInt32(0);
        ApduRequestInfo.Marshalling(data);
        int32_t ret = SendBufferEvent(HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL, data);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d",
            HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL, ret);
        return ret;
    }
    return HRIL_ERR_NULL_POINT;
}

int32_t RilManagerTest::SetActiveSim(
    int32_t index, int32_t enable, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SetActiveSim -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_SET_ACTIVE_SIM, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return HRIL_ERR_NULL_POINT;
        }

        int32_t slotId = 0;
        MessageParcel data;
        data.WriteInt32(slotId);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(index);
        data.WriteInt32(enable);
        int32_t ret = SendBufferEvent(HREQ_SIM_SET_ACTIVE_SIM, data);
        TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_SET_ACTIVE_SIM, ret);
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
        int32_t ret = SendInt32Event(HREQ_CALL_GET_USSD, request->serialId_);
        TELEPHONY_LOGI("SendInt32Event(ID:%{public}d) return: %{public}d", HREQ_CALL_GET_USSD, ret);
        return ret;
    } else {
        TELEPHONY_LOGE("ERROR : GetUssd --> cellularRadio_ == nullptr !!!");
        return HRIL_ERR_NULL_POINT;
    }
}

int32_t RilManagerTest::SetClip(int32_t action, const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_SET_CLIP, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    int32_t ret = SendInt32sEvent(HREQ_CALL_SET_CLIP, HRIL_EVENT_COUNT_2, telRilRequest->serialId_, action);
    if (ret != TELEPHONY_ERR_SUCCESS) {
        TELEPHONY_LOGE("function is failed, error: %{public}d", ret);
    }
    return ret;
}

int32_t RilManagerTest::GetClip(const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_GET_CLIR, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    return SendInt32Event(HREQ_CALL_GET_CLIR, telRilRequest->serialId_);
}

int32_t RilManagerTest::SetCallWaiting(int32_t activate, const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_SET_CALL_WAITING, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    int32_t ret = SendInt32sEvent(HREQ_CALL_SET_CALL_WAITING, HRIL_EVENT_COUNT_2, telRilRequest->serialId_, activate);
    if (ret != TELEPHONY_ERR_SUCCESS) {
        TELEPHONY_LOGE("function is failed, error: %{public}d", ret);
    }
    return ret;
}

int32_t RilManagerTest::GetCallWaiting(const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_GET_CALL_WAITING, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    return SendInt32Event(HREQ_CALL_GET_CALL_WAITING, telRilRequest->serialId_);
}

int32_t RilManagerTest::SetCallRestriction(
    std::string fac, int32_t mode, std::string password, const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_SET_CALL_RESTRICTION, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    MessageParcel data = {};
    MessageParcel reply = {};
    data.WriteInt32(telRilRequest->serialId_);
    data.WriteInt32(mode);
    data.WriteCString(fac.c_str());
    data.WriteCString(password.c_str());
    OHOS::MessageOption option = {OHOS::MessageOption::TF_ASYNC};
    int32_t ret = cellularRadio_->SendRequest(HREQ_CALL_SET_CALL_RESTRICTION, data, reply, option);
    if (ret != TELEPHONY_ERR_SUCCESS) {
        TELEPHONY_LOGE("function is failed, error: %{public}d", ret);
    }
    return ret;
}

int32_t RilManagerTest::GetCallRestriction(std::string fac, const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_GET_CALL_RESTRICTION, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    MessageParcel data = {};
    MessageParcel reply = {};
    data.WriteInt32(telRilRequest->serialId_);
    data.WriteCString(fac.c_str());
    OHOS::MessageOption option = {OHOS::MessageOption::TF_ASYNC};
    int32_t ret = cellularRadio_->SendRequest(HREQ_CALL_GET_CALL_RESTRICTION, data, reply, option);
    if (ret != TELEPHONY_ERR_SUCCESS) {
        TELEPHONY_LOGE("function is failed, error: %{public}d", ret);
    }
    return ret;
}

int32_t RilManagerTest::SetCallTransferInfo(
    int32_t reason, int32_t mode, std::string number, int32_t classx, const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_SET_CALL_TRANSFER_INFO, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("cellularRadio_ is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    CallForwardSetInfo callForwardSetInfo;
    callForwardSetInfo.reason = reason;
    callForwardSetInfo.mode = mode;
    callForwardSetInfo.classx = classx;
    callForwardSetInfo.number = number;
    callForwardSetInfo.serial = telRilRequest->serialId_;

    MessageParcel data = {};
    callForwardSetInfo.Marshalling(data);
    int32_t ret = SendBufferEvent(HREQ_CALL_SET_CALL_TRANSFER_INFO, data);
    TELEPHONY_LOGI("Send (ID:%{public}d) return: %{public}d", HREQ_CALL_SET_CALL_TRANSFER_INFO, ret);
    return ret;
}

int32_t RilManagerTest::GetCallTransferInfo(int32_t reason, const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_GET_CALL_TRANSFER_INFO, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    int32_t ret =
        SendInt32sEvent(HREQ_CALL_GET_CALL_TRANSFER_INFO, HRIL_EVENT_COUNT_2, telRilRequest->serialId_, reason);
    if (ret != TELEPHONY_ERR_SUCCESS) {
        TELEPHONY_LOGE("function is failed, error: %{public}d", ret);
    }
    return ret;
}

int32_t RilManagerTest::SetClir(int32_t action, const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_SET_CLIR, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    int32_t ret = SendInt32sEvent(HREQ_CALL_SET_CLIR, HRIL_EVENT_COUNT_2, telRilRequest->serialId_, action);
    if (ret != TELEPHONY_ERR_SUCCESS) {
        TELEPHONY_LOGE("function is failed, error: %{public}d", ret);
    }
    return ret;
}

int32_t RilManagerTest::GetClir(const AppExecFwk::InnerEvent::Pointer &result)
{
    std::shared_ptr<HRilRequestTest> telRilRequest = CreateRequest(HREQ_CALL_GET_CLIR, result);
    if (telRilRequest == nullptr) {
        TELEPHONY_LOGE("telRilRequest is nullptr");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("%{public}s  cellularRadio_ is nullptr", __func__);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }

    return SendInt32Event(HREQ_CALL_GET_CLIR, telRilRequest->serialId_);
}

int32_t RilManagerTest::GetLinkBandwidthInfo(const int32_t cid, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetLinkBandwidthInfo -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, result);
        if (request == nullptr) {
            return HRIL_ERR_NULL_POINT;
        }
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

int32_t RilManagerTest::SetDataPermitted(int32_t enabled, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::SetDataPermitted -->");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR : SetDataPermitted --> cellularRadio_ == nullptr !!!");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_SET_DATA_PERMITTED, result);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    MessageParcel reply;
    data.WriteInt32(request->serialId_);
    data.WriteInt32(enabled);
    MessageOption option = { MessageOption::TF_ASYNC };
    int32_t ret = cellularRadio_->SendRequest(HREQ_DATA_SET_DATA_PERMITTED, data, reply, option);
    TELEPHONY_LOGI("SendRequest(ID:%{public}d) return: %{public}d", HREQ_DATA_SET_DATA_PERMITTED, ret);
    return ret;
}

int32_t RilManagerTest::UnLockPin(const std::string &pin, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::UnLockPin -->");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR: UnLockPin --> cellularRadio_ is nullptr!!!");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_UNLOCK_PIN, result);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    data.WriteCString(pin.c_str());
    int32_t ret = SendBufferEvent(HREQ_SIM_UNLOCK_PIN, data);
    TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_UNLOCK_PIN, ret);
    return ret;
}

int32_t RilManagerTest::UnLockPin2(const std::string &pin2, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::UnLockPin -->");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR: UnLockPin2 --> cellularRadio_ is nullptr!!!");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_UNLOCK_PIN2, result);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    data.WriteCString(pin2.c_str());
    int32_t ret = SendBufferEvent(HREQ_SIM_UNLOCK_PIN2, data);
    TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_UNLOCK_PIN2, ret);
    return ret;
}

int32_t RilManagerTest::UnLockPuk(
    const std::string &puk, const std::string &pin, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::UnLockPuk -->");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR: UnLockPuk --> cellularRadio_ is nullptr!!!");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_UNLOCK_PUK, result);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    data.WriteCString(puk.c_str());
    data.WriteCString(pin.c_str());
    int32_t ret = SendBufferEvent(HREQ_SIM_UNLOCK_PUK, data);
    TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_UNLOCK_PUK, ret);
    return ret;
}

int32_t RilManagerTest::UnLockPuk2(
    const std::string &puk2, const std::string &pin2, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::UnLockPuk2 -->");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR: UnLockPuk2 --> cellularRadio_ is nullptr!!!");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_UNLOCK_PUK2, result);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    int slotId = 0;
    data.WriteInt32(slotId);
    data.WriteInt32(request->serialId_);
    data.WriteCString(puk2.c_str());
    data.WriteCString(pin2.c_str());
    int32_t ret = SendBufferEvent(HREQ_SIM_UNLOCK_PUK2, data);
    TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_UNLOCK_PUK2, ret);
    return ret;
}

int32_t RilManagerTest::ChangeSimPassword(const std::string &fac, const std::string &oldPassword,
    const std::string &newPassword, int32_t passwordLength, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::ChangeSimPassword -->");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR: ChangeSimPassword --> cellularRadio_ is nullptr!!!");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_CHANGE_SIM_PASSWORD, result);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    int slotId = 0;
    data.WriteInt32(slotId);
    SimPasswordInfo simPwdInfo;
    simPwdInfo.serial = request->serialId_;
    simPwdInfo.fac = fac;
    simPwdInfo.oldPassword = oldPassword;
    simPwdInfo.newPassword = newPassword;
    simPwdInfo.passwordLength = passwordLength;
    if (!simPwdInfo.Marshalling(data)) {
        TELEPHONY_LOGE("RilManagerTest simlockinfo Marshalling failed!!!");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendBufferEvent(HREQ_SIM_CHANGE_SIM_PASSWORD, data);
    TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_CHANGE_SIM_PASSWORD, ret);
    return ret;
}

int32_t RilManagerTest::SetSimLock(
    const std::string &fac, int32_t mode, const std::string &password, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::SetSimLock -->");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR: SetSimLock --> cellularRadio_ is nullptr!!!");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_SET_SIM_LOCK, result);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    int slotId = 0;
    data.WriteInt32(slotId);
    SimLockInfo simlockinfo;
    simlockinfo.serial = request->serialId_;
    simlockinfo.fac = fac;
    simlockinfo.mode = mode;
    simlockinfo.passwd = password;
    if (!simlockinfo.Marshalling(data)) {
        TELEPHONY_LOGE("RilManagerTest simlockinfo Marshalling failed!!!");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendBufferEvent(HREQ_SIM_SET_SIM_LOCK, data);
    TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_SET_SIM_LOCK, ret);
    return ret;
}

int32_t RilManagerTest::GetSimLockStatus(
    const std::string &fac, int32_t mode, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetSimLockStatus -->");
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("ERROR: GetSimLockStatus --> cellularRadio_ is nullptr!!!");
        return HRIL_ERR_NULL_POINT;
    }
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_GET_SIM_LOCK_STATUS, result);
    if (request == nullptr) {
        return HRIL_ERR_NULL_POINT;
    }
    MessageParcel data;
    int slotId = 0;
    data.WriteInt32(slotId);
    SimLockInfo simlockinfo;
    simlockinfo.serial = request->serialId_;
    simlockinfo.fac = fac;
    simlockinfo.mode = mode;
    if (!simlockinfo.Marshalling(data)) {
        TELEPHONY_LOGE("RilManagerTest simlockinfo Marshalling failed!!!");
        return HRIL_ERR_NULL_POINT;
    }
    int32_t ret = SendBufferEvent(HREQ_SIM_GET_SIM_LOCK_STATUS, data);
    TELEPHONY_LOGI("SendBufferEvent(ID:%{public}d) return: %{public}d", HREQ_SIM_GET_SIM_LOCK_STATUS, ret);
    return ret;
}
} // namespace Telephony
} // namespace OHOS
