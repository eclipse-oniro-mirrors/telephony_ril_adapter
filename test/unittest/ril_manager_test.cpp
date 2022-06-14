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

#include "hril_request.h"
#include "ril_radio_indication_test.h"
#include "ril_radio_response_test.h"
#include "telephony_log_wrapper.h"

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
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(value);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
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
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteCString(value);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        status = cellularRadio_->SendRequest(dispatchId, data, reply, option);
    }
    return status;
}

int32_t RilManagerTest::SendBufferEvent(int32_t dispatchId, OHOS::MessageParcel &eventData)
{
    int32_t status = 0;
    if (cellularRadio_ != nullptr) {
        OHOS::MessageParcel reply;
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        status = cellularRadio_->SendRequest(dispatchId, eventData, reply, option);
    }
    return status;
}

int32_t RilManagerTest::SendCommonBufferEvent(int32_t dispatchId, const void *eventData, size_t dataLength)
{
    int32_t status = 0;
    const int32_t INCREMENT_VALUE = 1;
    if (cellularRadio_ != nullptr) {
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteBuffer(eventData, (dataLength + INCREMENT_VALUE));
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        status = cellularRadio_->SendRequest(dispatchId, data, reply, option);
    }
    return status;
}

void RilManagerTest::SetCellularRadioIndication()
{
    TELEPHONY_LOGI("RilManagerTest SetCellularRadioIndication start!");
    if (cellularRadio_ != nullptr) {
        std::unique_ptr<OHOS::IPCObjectStub> callback = std::make_unique<RilRadioIndicationTest>(this);
        if (callback == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        sptr<IPCObjectStub> object = callback.release();
        data.WriteRemoteObject(object);
        OHOS::MessageOption option;
        cellularRadio_->SendRequest(RilManagerTest::HRIL_ADAPTER_RADIO_INDICATION, data, reply, option);
    }
}

void RilManagerTest::SetCellularRadioResponse()
{
    TELEPHONY_LOGI("RilManagerTest SetCellularRadioResponse start!");
    if (cellularRadio_ != nullptr) {
        std::unique_ptr<OHOS::IPCObjectStub> callback = std::make_unique<RilRadioResponseTest>(this);
        if (callback == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        sptr<IPCObjectStub> object = callback.release();
        data.WriteRemoteObject(object);
        OHOS::MessageOption option;
        cellularRadio_->SendRequest(RilManagerTest::HRIL_ADAPTER_RADIO_RESPONSE, data, reply, option);
    }
}

RilManagerTest::RilManagerTest(int32_t preferredNetworkType, int32_t cdmaSubscription)
{
    TELEPHONY_LOGI("RilManagerTest init");
    auto servMgr_ = OHOS::HDI::ServiceManager::V1_0::IServiceManager::Get();
    cellularRadio_ = servMgr_->GetService(RIL_ADAPTER_SERVICE_NAME);
    if (cellularRadio_ == nullptr) {
        TELEPHONY_LOGE("RilManagerTest bind hdf error!");
        return;
    }
    TELEPHONY_LOGI("RilManagerTest bind hdf success!");
    slotId_ = HRIL_SIM_SLOT_0;
}

void RilManagerTest::OnInit()
{
    TELEPHONY_LOGI("OnInit start");
    SetCellularRadioIndication();
    SetCellularRadioResponse();
}

void RilManagerTest::SetSlotId(int32_t slotId)
{
    if ((slotId >= HRIL_SIM_SLOT_0) && (slotId <= HRIL_SIM_SLOT_NUM)) {
        slotId_ = slotId;
    }
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
    dataProfileDataInfo.password = dpi.password;
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
    TELEPHONY_LOGI("GetCallList");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_GET_CALL_LIST, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_GET_CALL_LIST, request->serialId_);
    } else {
        TELEPHONY_LOGE("GetCallList  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::RilCmDial(std::string address, int32_t clirMode, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilCmDial");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_DIAL, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel wData;
        DialInfo dialInfo;
        dialInfo.address = ConvertNullToEmptyString(address);
        dialInfo.clir = clirMode;
        dialInfo.serial = request->serialId_;

        wData.WriteInt32(slotId_);
        if (!dialInfo.Marshalling(wData)) {
            TELEPHONY_LOGE("RilCmDial -> RilManagerTest Marshalling");
            return;
        }
        SendBufferEvent(HREQ_CALL_DIAL, wData);
    } else {
        TELEPHONY_LOGE("RilCmDial  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::Hangup(int32_t gsmIndex, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Hangup");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_HANGUP, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(gsmIndex);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_CALL_HANGUP, data, reply, option);
    } else {
        TELEPHONY_LOGE("Hangup  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::Reject(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Reject");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_REJECT, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_REJECT, request->serialId_);
    } else {
        TELEPHONY_LOGE("Reject  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::Answer(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Answer");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_ANSWER, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_ANSWER, request->serialId_);
    } else {
        TELEPHONY_LOGE("Answer  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::HoldCall(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Hold");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_HOLD_CALL, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_HOLD_CALL, request->serialId_);
    } else {
        TELEPHONY_LOGE("Hold  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::UnHoldCall(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Unhold");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_UNHOLD_CALL, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_UNHOLD_CALL, request->serialId_);
    } else {
        TELEPHONY_LOGE("Unhold  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SwitchCall(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("Switch Call");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SWITCH_CALL, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_SWITCH_CALL, request->serialId_);
    } else {
        TELEPHONY_LOGE("Switch Call  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::RilCmJoin(int32_t callType, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilCmJoin");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_COMBINE_CONFERENCE, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(callType);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_CALL_COMBINE_CONFERENCE, data, reply, option);
    } else {
        TELEPHONY_LOGE("RilCmJoin  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::RilCmSplit(int32_t callIndex, int32_t callType, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilCmSplit");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SEPARATE_CONFERENCE, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(callIndex);
        data.WriteInt32(callType);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_CALL_SEPARATE_CONFERENCE, data, reply, option);
    } else {
        TELEPHONY_LOGE("RilCmSplit  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::CallSupplement(int32_t hangupType, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("CallSupplement");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_CALL_SUPPLEMENT, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(hangupType);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_CALL_CALL_SUPPLEMENT, data, reply, option);
    } else {
        TELEPHONY_LOGE("CallSupplement  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SendDtmf(char dtmfKey, int32_t index, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::SendDtmf -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SEND_DTMF, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);

        char temp[2];
        temp[0] = dtmfKey;
        temp[1] = '\0';
        const int32_t DTMF_ON_LEN = 300;
        data.WriteInt32(index);
        data.WriteInt32(DTMF_ON_LEN);
        data.WriteInt32(0);
        data.WriteInt32(1); // stringLength
        data.WriteCString(temp);

        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_CALL_SEND_DTMF, data, reply, option);
    } else {
        TELEPHONY_LOGE("SendDtmf  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::StartDtmf(char dtmfKey, int32_t index, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::StartDtmf -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_START_DTMF, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);

        char temp[2] = {};
        temp[0] = dtmfKey;
        data.WriteInt32(index);
        data.WriteCString(temp);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_CALL_START_DTMF, data, reply, option);
    } else {
        TELEPHONY_LOGE("StartDtmf  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::StopDtmf(int32_t index, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::StopDtmf -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_STOP_DTMF, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);

        char temp[2] = {};
        temp[0] = 'A';
        data.WriteInt32(index);
        data.WriteCString(temp);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_CALL_STOP_DTMF, data, reply, option);
    } else {
        TELEPHONY_LOGE("StopDtmf  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SetUssd(std::string str, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::SetUssdCusd -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SET_USSD, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteCString(str.c_str());
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_CALL_SET_USSD, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetUssdCusd  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::GetUssd(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetUssdCusd -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_GET_USSD, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_GET_USSD, request->serialId_);
    } else {
        TELEPHONY_LOGE("GetUssdCusd  cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::SetMute(int32_t mute, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::SetMute -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_SET_MUTE, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(mute);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_CALL_SET_MUTE, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetMute  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::GetMute(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetMute -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_GET_MUTE, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_GET_MUTE, request->serialId_);
    } else {
        TELEPHONY_LOGE("GetMute  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::GetEmergencyList(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetEmergencyList -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_GET_EMERGENCY_LIST, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_GET_EMERGENCY_LIST, request->serialId_);
    } else {
        TELEPHONY_LOGE("GetEmergencyList  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::GetFailReason(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetFailReason -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_CALL_GET_FAIL_REASON, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_CALL_GET_FAIL_REASON, request->serialId_);
    } else {
        TELEPHONY_LOGE("GetFailReason  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::GetSimIO(int32_t command, int32_t fileid, int32_t p1, int32_t p2, int32_t p3, std::string data,
    std::string path, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::GetSimIO --> ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_GET_SIM_IO, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("RilManagerTest::CreateRequest is null ");
            return;
        }
        OHOS::MessageParcel wData;
        wData.WriteInt32(slotId_);

        SimIoRequestInfo iccIoRequestInfo;
        iccIoRequestInfo.serial = request->serialId_;
        iccIoRequestInfo.command = command;
        iccIoRequestInfo.fileId = fileid;
        iccIoRequestInfo.path = ConvertNullToEmptyString(path);
        iccIoRequestInfo.p1 = p1;
        iccIoRequestInfo.p2 = p2;
        iccIoRequestInfo.p3 = p3;
        iccIoRequestInfo.data = ConvertNullToEmptyString(data);
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
        if (request == nullptr) {
            return;
        }
        UniInfo imsi;
        imsi.serial = request->serialId_;
        imsi.strTmp = ConvertNullToEmptyString(aid);
        OHOS::MessageParcel wData;
        wData.WriteInt32(slotId_);
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
    TELEPHONY_LOGI("RilManagerTest GetRilCmSignalStrength");
    if (cellularRadio_ != nullptr) {
        auto event = AppExecFwk::InnerEvent::Get(what);
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_SIGNAL_STRENGTH, event);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_SIGNAL_STRENGTH, request->serialId_);
    }
}

void RilManagerTest::GetSimStatus(const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetSimStatus -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_GET_SIM_STATUS, result);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_SIM_GET_SIM_STATUS, request->serialId_);
    } else {
        TELEPHONY_LOGE("ERROR : GetSimStatus --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::GetCsRegStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest GetCsRegStatus");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_CS_REG_STATUS, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_CS_REG_STATUS, request->serialId_);
    }
}

void RilManagerTest::GetPsRegStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_PS_REG_STATUS, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_PS_REG_STATUS, request->serialId_);
    }
}

void RilManagerTest::GetNetworkSearchInfo(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION, request->serialId_);
    }
}

void RilManagerTest::GetNetworkSelectionMode(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_NETWORK_SELECTION_MODE, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_NETWORK_SELECTION_MODE, request->serialId_);
    }
}

void RilManagerTest::GetPreferredNetwork(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_PREFERRED_NETWORK, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_PREFERRED_NETWORK, request->serialId_);
    }
}

void RilManagerTest::GetImsRegStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_IMS_REG_STATUS, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_IMS_REG_STATUS, request->serialId_);
    }
}

void RilManagerTest::GetVoiceRadio(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_GET_VOICE_RADIO, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_MODEM_GET_VOICE_RADIO, request->serialId_);
    }
}

void RilManagerTest::GetPsAttachStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_PS_ATTACH_STATUS, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_PS_ATTACH_STATUS, request->serialId_);
    }
}

void RilManagerTest::SetPreferredNetwrok(int32_t preferredNetwork, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("SetRilPreferredNetwrok  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_SET_PREFERRED_NETWORK, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(preferredNetwork);
        MessageOption option;
        cellularRadio_->SendRequest(HREQ_NETWORK_SET_PREFERRED_NETWORK, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetRilPreferredNetwrok  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SetLocationUpdate(int32_t enableFlg, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("SetRilLocationUpdate  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_SET_LOCATE_UPDATES, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(enableFlg);
        MessageOption option;
        cellularRadio_->SendRequest(HREQ_NETWORK_SET_LOCATE_UPDATES, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetRilLocationUpdate  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SetNotificationFilter(int32_t filter, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("SetRilNotificationFilter Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_SET_NOTIFICATION_FILTER, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(filter);
        MessageOption option;
        cellularRadio_->SendRequest(HREQ_NETWORK_SET_NOTIFICATION_FILTER, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetRilNotificationFilter cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SetDeviceState(
    int32_t deviceStateType, bool deviceStateOn, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("SetRilDeviceState Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_SET_DEVICE_STATE, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(deviceStateType);
        data.WriteInt32(deviceStateOn);
        MessageOption option;
        cellularRadio_->SendRequest(HREQ_NETWORK_SET_DEVICE_STATE, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetRilDeviceState cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SetPsAttachStatus(int32_t attachFlg, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("SetRilPsAttachStatus  Request ");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_SET_PS_ATTACH_STATUS, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(attachFlg);
        MessageOption option;
        cellularRadio_->SendRequest(HREQ_NETWORK_SET_PS_ATTACH_STATUS, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetRilPsAttachStatus  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SetNetworkSelectionMode(
    int32_t mode, std::string plmn, const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_SET_NETWORK_SELECTION_MODE, result);
        if (request == nullptr) {
            TELEPHONY_LOGE("RilManagerTest::CreateRequest is null ");
            return;
        }
        OHOS::MessageParcel wData;
        SetNetworkModeInfo networkMode;
        networkMode.serial = request->serialId_;
        networkMode.selectMode = mode;
        networkMode.oper = ConvertNullToEmptyString(plmn);
        wData.WriteInt32(slotId_);
        if (!networkMode.Marshalling(wData)) {
            TELEPHONY_LOGE("ERROR : SetRilNetworkSelectionMode --> Marshalling (wData) failed !!!");
            return;
        }
        TELEPHONY_LOGI("SetRilNetworkSelectionMode --> SetRilNetworkSelectionMode.Marshalling (wData) success");
        SendBufferEvent(HREQ_NETWORK_SET_NETWORK_SELECTION_MODE, wData);
    } else {
        TELEPHONY_LOGE("ERROR : SetRilNetworkSelectionMode --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::GetCellInfoList(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST, request->serialId_);
    }
}

void RilManagerTest::GetCurrentCellInfo(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_CURRENT_CELL_INFO, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_CURRENT_CELL_INFO, request->serialId_);
    }
}

void RilManagerTest::GetOperatorInfo(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int32_t what)
{
    TELEPHONY_LOGI("RilManagerTest GetOperatorInfo");
    if (cellularRadio_ != nullptr) {
        auto event = AppExecFwk::InnerEvent::Get(what);
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_NETWORK_GET_OPERATOR_INFO, event);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_NETWORK_GET_OPERATOR_INFO, request->serialId_);
    }
}

void RilManagerTest::SendSms(std::string smscPdu, std::string pdu,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler, const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_SEND_GSM_SMS, response);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        GsmSmsMessageInfo mGsmSmsMessageInfo = ConstructGsmSendSmsRilRequest(smscPdu, pdu);
        mGsmSmsMessageInfo.serial = request->serialId_;
        data.WriteInt32(slotId_);
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
        if (request == nullptr) {
            return;
        }
        /* Do not log function arg for privacy */
        OHOS::MessageParcel data;
        GsmSmsMessageInfo mGsmSmsMessageInfo = ConstructGsmSendSmsRilRequest(smscPdu, pdu);
        mGsmSmsMessageInfo.serial = request->serialId_;
        data.WriteInt32(slotId_);
        if (!mGsmSmsMessageInfo.Marshalling(data)) {
            TELEPHONY_LOGE("RilCmSendSMS Marshalling failed!!!");
            return;
        }
        SendBufferEvent(HREQ_SMS_SEND_SMS_MORE_MODE, data);
    }
}

void RilManagerTest::SendSmsAck(bool success, int32_t cause, const AppExecFwk::InnerEvent::Pointer &response)
{
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_SEND_SMS_ACK, response);
    if (request == nullptr) {
        return;
    }
    /* Do not log function arg for privacy */
    OHOS::MessageParcel data;
    data.WriteInt32(slotId_);
    ModeData mModeData;
    mModeData.serial = request->serialId_;
    mModeData.result = success;
    mModeData.mode = cause;
    if (!mModeData.Marshalling(data)) {
        TELEPHONY_LOGE("SendSmsAck Marshalling failed!!!");
        return;
    }
    SendBufferEvent(HREQ_SMS_SEND_SMS_ACK, data);
}

void RilManagerTest::GetSmscAddr(const AppExecFwk::InnerEvent::Pointer &response)
{
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_GET_SMSC_ADDR, response);
    if (request == nullptr) {
        return;
    }
    /* Do not log function arg for privacy */
    OHOS::MessageParcel data;
    data.WriteInt32(slotId_);
    data.WriteInt32(request->serialId_);
    SendBufferEvent(HREQ_SMS_GET_SMSC_ADDR, data);
}

void RilManagerTest::SetSmscAddr(int32_t tosca, std::string address, const AppExecFwk::InnerEvent::Pointer &response)
{
    std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SMS_SET_SMSC_ADDR, response);
    if (request == nullptr) {
        return;
    }
    /* Do not log function arg for privacy */
    OHOS::MessageParcel data;
    data.WriteInt32(slotId_);
    ServiceCenterAddress serCenterAddress;
    serCenterAddress.serial = request->serialId_;
    serCenterAddress.address = address.empty() ? "" : address;
    serCenterAddress.tosca = tosca;
    if (!serCenterAddress.Marshalling(data)) {
        TELEPHONY_LOGE("SetSmscAddr Marshalling failed!!!");
        return;
    }
    SendBufferEvent(HREQ_SMS_SET_SMSC_ADDR, data);
}

GsmSmsMessageInfo RilManagerTest::ConstructGsmSendSmsRilRequest(string smscPdu, string pdu)
{
    GsmSmsMessageInfo msg;
    msg.smscPdu = smscPdu.empty() ? "" : smscPdu;
    msg.pdu = pdu.empty() ? "" : pdu;
    return msg;
}

void RilManagerTest::SetRadioState(int32_t fan, int32_t rst, const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_SET_RADIO_STATUS, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(fan);
        data.WriteInt32(rst);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_MODEM_SET_RADIO_STATUS, data, reply, option);
    } else {
        TELEPHONY_LOGE("SetRadioState  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::GetRadioState(const AppExecFwk::InnerEvent::Pointer &result)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_GET_RADIO_STATUS, result);
        if (request == nullptr) {
            return;
        }
        OHOS::MessageParcel data;
        OHOS::MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_MODEM_GET_RADIO_STATUS, data, reply, option);
    } else {
        TELEPHONY_LOGE("GetRadioState  cellularRadio_ == nullptr");
    }
}

void RilManagerTest::GetImei(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_MODEM_GET_IMEI, response);
        if (request == nullptr) {
            return;
        }
        SendInt32Event(HREQ_MODEM_GET_IMEI, request->serialId_);
    }
}

void RilManagerTest::ActivatePdpContext(int32_t radioTechnology, RilDataProfileTest dataProfile, bool isRoaming,
    bool allowRoaming, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::ActivatePdpContext -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_ACTIVATE_PDP_CONTEXT, response);
        if (request == nullptr) {
            return;
        }
        DataCallInfo dataCallInfo;
        dataCallInfo.serial = request->serialId_;
        dataCallInfo.radioTechnology = radioTechnology;
        dataCallInfo.dataProfileInfo = ConvertToHalDataProfile(dataProfile);
        dataCallInfo.roamingAllowed = allowRoaming;
        dataCallInfo.isRoaming = isRoaming;
        OHOS::MessageParcel wData;
        wData.WriteInt32(slotId_);
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
        if (request == nullptr) {
            return;
        }
        UniInfo uniInfo;
        uniInfo.serial = request->serialId_;
        uniInfo.gsmIndex = ci;
        uniInfo.arg1 = reason;
        OHOS::MessageParcel wData;
        wData.WriteInt32(slotId_);
        if (!uniInfo.Marshalling(wData)) {
            TELEPHONY_LOGE("ERROR : DeactivatePdpContext --> uniInfo.Marshalling(wData) failed !!!");
            return;
        }
        SendBufferEvent(HREQ_DATA_DEACTIVATE_PDP_CONTEXT, wData);
    } else {
        TELEPHONY_LOGE("ERROR : DeactivatePdpContext --> cellularRadio_ == nullptr");
    }
}

void RilManagerTest::SetInitApnInfo(RilDataProfileTest dataProfile, const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_SET_INIT_APN_INFO, response);
        if (request == nullptr) {
            return;
        }

        DataProfileDataInfo dataInfo = ConvertToHalDataProfile(dataProfile);
        dataInfo.serial = request->serialId_;
        OHOS::MessageParcel wData;
        wData.WriteInt32(slotId_);
        if (!dataInfo.Marshalling(wData)) {
            TELEPHONY_LOGE("ERROR : SetInitApnInfo --> dataInfo.Marshalling(wData) failed !!!");
            return;
        }

        SendBufferEvent(HREQ_DATA_SET_INIT_APN_INFO, wData);
    } else {
        TELEPHONY_LOGE("ERROR : SetInitApnInfo --> cellularRadio_ == nullptr");
    }
}

void RilManagerTest::GetPdpContext(const AppExecFwk::InnerEvent::Pointer &response)
{
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_GET_PDP_CONTEXT_LIST, response);
        if (request == nullptr) {
            return;
        }
        UniInfo uniInfo;
        uniInfo.serial = request->serialId_;
        OHOS::MessageParcel wData;
        wData.WriteInt32(slotId_);
        if (!uniInfo.Marshalling(wData)) {
            TELEPHONY_LOGE("ERROR : GetPdpContext --> uniInfo.Marshalling(wData) failed !!!");
            return;
        }
        SendBufferEvent(HREQ_DATA_GET_PDP_CONTEXT_LIST, wData);
    } else {
        TELEPHONY_LOGE("ERROR : GetPdpContext --> cellularRadio_ == nullptr");
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

void RilManagerTest::SimOpenLogicalChannel(
    std::string appID, int32_t p2, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SimOpenLogicalChannel -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_OPEN_LOGICAL_CHANNEL, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteCString(appID.c_str());
        data.WriteInt32(p2);
        if (SendBufferEvent(HREQ_SIM_OPEN_LOGICAL_CHANNEL, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : SimOpenLogicalChannel --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::SimCloseLogicalChannel(int32_t channelId, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SimCloseLogicalChannel -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_CLOSE_LOGICAL_CHANNEL, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(channelId);
        if (SendBufferEvent(HREQ_SIM_CLOSE_LOGICAL_CHANNEL, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : SimCloseLogicalChannel --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::SimTransmitApduLogicalChannel(
    ApduSimIORequestInfo reqInfo, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SimTransmitApduLogicalChannel -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return;
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

        data.WriteInt32(slotId_);
        ApduRequestInfo.Marshalling(data);

        if (SendBufferEvent(HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : SimTransmitApduLogicalChannel --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::SimTransmitApduBasicChannel(
    ApduSimIORequestInfo reqInfo, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SimTransmitApduBasicChannel -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return;
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

        data.WriteInt32(slotId_);
        ApduRequestInfo.Marshalling(data);

        if (SendBufferEvent(HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : SimTransmitApduBasicChannel --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::SimAuthentication(
    SimAuthenticationRequestInfo reqInfo, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SimAuthentication -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_AUTHENTICATION, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return;
        }

        MessageParcel data;
        SimAuthenticationRequestInfo simAuthinfo;
        simAuthinfo.serial = request->serialId_;
        simAuthinfo.aid = reqInfo.aid;
        simAuthinfo.authData = reqInfo.authData;
        data.WriteInt32(slotId_);
        simAuthinfo.Marshalling(data);

        if (SendBufferEvent(HREQ_SIM_AUTHENTICATION, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : SimAuthentication --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::SetActiveSim(int32_t slotId, int32_t enable, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::SetActiveSim -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_SET_ACTIVE_SIM, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return;
        }

        int32_t index = 1;
        MessageParcel data;
        data.WriteInt32(slotId);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(index);
        data.WriteInt32(enable);

        if (SendBufferEvent(HREQ_SIM_SET_ACTIVE_SIM, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : SetActiveSim --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::UnlockSimLock(
    int32_t lockType, std::string password, const AppExecFwk::InnerEvent::Pointer &response)
{
    TELEPHONY_LOGI("RilManagerTest::UnlockSimLock -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_UNLOCK_SIM_LOCK, response);
        if (request == nullptr) {
            TELEPHONY_LOGE("request is nullptr");
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(lockType);
        data.WriteCString(password.c_str());
        MessageParcel reply;
        OHOS::MessageOption option = { OHOS::MessageOption::TF_ASYNC };
        if (cellularRadio_->SendRequest(HREQ_SIM_UNLOCK_SIM_LOCK, data, reply, option) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : UnlockSimLock --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::GetLinkBandwidthInfo(const int32_t cid, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetLinkBandwidthInfo -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        MessageParcel reply;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteInt32(cid);
        MessageOption option = { MessageOption::TF_ASYNC };
        cellularRadio_->SendRequest(HREQ_DATA_GET_LINK_BANDWIDTH_INFO, data, reply, option);
    } else {
        TELEPHONY_LOGE("ERROR : GetLinkBandwidthInfo --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::UnLockPin(const std::string &pin, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::UnLockPin -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_UNLOCK_PIN, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteCString(pin.c_str());

        if (SendBufferEvent(HREQ_SIM_UNLOCK_PIN, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : UnLockPin --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::UnLockPin2(const std::string &pin2, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::UnLockPin2 -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_UNLOCK_PIN2, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteCString(pin2.c_str());

        if (SendBufferEvent(HREQ_SIM_UNLOCK_PIN2, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : UnLockPin2 --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::UnLockPuk(
    const std::string &puk, const std::string &pin, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::UnLockPuk -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_UNLOCK_PUK, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteCString(puk.c_str());
        data.WriteCString(pin.c_str());

        if (SendBufferEvent(HREQ_SIM_UNLOCK_PUK, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : UnLockPuk --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::UnLockPuk2(
    const std::string &puk2, const std::string &pin2, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::UnLockPuk2 -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_UNLOCK_PUK2, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        data.WriteInt32(request->serialId_);
        data.WriteCString(puk2.c_str());
        data.WriteCString(pin2.c_str());

        if (SendBufferEvent(HREQ_SIM_UNLOCK_PUK2, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : UnLockPuk2 --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::ChangeSimPassword(const std::string &fac, const std::string &oldPassword,
    const std::string &newPassword, int32_t passwordLength, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::ChangeSimPassword -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_CHANGE_SIM_PASSWORD, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        SimPasswordInfo simPwdInfo;
        simPwdInfo.serial = request->serialId_;
        simPwdInfo.fac = fac;
        simPwdInfo.oldPassword = oldPassword;
        simPwdInfo.newPassword = newPassword;
        simPwdInfo.passwordLength = passwordLength;

        if (!simPwdInfo.Marshalling(data)) {
            TELEPHONY_LOGE("RilManagerTest simlockinfo Marshalling failed!!!");
            return;
        }
        if (SendBufferEvent(HREQ_SIM_CHANGE_SIM_PASSWORD, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : ChangeSimPassword --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::SetSimLock(
    const std::string &fac, int32_t mode, const std::string &password, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::SetSimLock -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_SET_SIM_LOCK, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        SimLockInfo simlockinfo;
        simlockinfo.serial = request->serialId_;
        simlockinfo.fac = fac;
        simlockinfo.mode = mode;
        simlockinfo.passwd = password;
        simLockInfo.classx = 0;

        if (!simlockinfo.Marshalling(data)) {
            TELEPHONY_LOGE("RilManagerTest simlockinfo Marshalling failed!!!");
            return;
        }
        if (SendBufferEvent(HREQ_SIM_SET_SIM_LOCK, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : SetSimLock --> cellularRadio_ == nullptr !!!");
    }
}

void RilManagerTest::GetSimLockStatus(
    const std::string &fac, int32_t mode, const AppExecFwk::InnerEvent::Pointer &result)
{
    TELEPHONY_LOGI("RilManagerTest::GetSimLockStatus -->");
    if (cellularRadio_ != nullptr) {
        std::shared_ptr<HRilRequestTest> request = CreateRequest(HREQ_SIM_GET_SIM_LOCK_STATUS, result);
        if (request == nullptr) {
            return;
        }
        MessageParcel data;
        data.WriteInt32(slotId_);
        SimLockInfo simlockinfo;
        simlockinfo.serial = request->serialId_;
        simlockinfo.fac = fac;
        simlockinfo.mode = mode;
        simLockInfo.classx = 0;

        if (!simlockinfo.Marshalling(data)) {
            TELEPHONY_LOGE("RilManagerTest simlockinfo Marshalling failed!!!");
            return;
        }
        if (SendBufferEvent(HREQ_SIM_GET_SIM_LOCK_STATUS, data) < 0) {
            TELEPHONY_LOGE("cellularRadio_->SendRequest fail");
        }
    } else {
        TELEPHONY_LOGE("ERROR : GetSimLockStatus --> cellularRadio_ == nullptr !!!");
    }
}
} // namespace Telephony
} // namespace OHOS
