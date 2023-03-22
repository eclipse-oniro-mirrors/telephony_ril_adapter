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

#ifndef OHOS_HRIL_SMS_H
#define OHOS_HRIL_SMS_H

#include "hril_base.h"
#include "hril_sms_parcel.h"

namespace OHOS {
namespace Telephony {
class HRilSms : public HRilBase {
public:
    HRilSms(int32_t slotId, IHRilReporter &hrilReporter);
    virtual ~HRilSms() = default;

    bool IsSmsRespOrNotify(uint32_t code);
    void RegisterSmsFuncs(const HRilSmsReq *smsFuncs);

    int32_t SendGsmSms(int32_t serialId, const OHOS::HDI::Ril::V1_0::GsmSmsMessageInfo &gsmSmsMessageInfo);
    int32_t SendCdmaSms(int32_t serialId, const OHOS::HDI::Ril::V1_0::SendCdmaSmsMessageInfo &cdmaSmsMessageInfo);
    int32_t AddSimMessage(int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo);
    int32_t DelSimMessage(int32_t serialId, int32_t index);
    int32_t UpdateSimMessage(int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo);
    int32_t SetSmscAddr(int32_t serialId, const OHOS::HDI::Ril::V1_0::ServiceCenterAddress &serviceCenterAddress);
    int32_t GetSmscAddr(int32_t serialId);
    int32_t SetCBConfig(int32_t serialId, const OHOS::HDI::Ril::V1_0::CBConfigInfo &cellBroadcastInfo);
    int32_t GetCBConfig(int32_t serialId);
    int32_t SetCdmaCBConfig(int32_t serialId, const OHOS::HDI::Ril::V1_0::CdmaCBConfigInfoList &cdmaCBConfigInfoList);
    int32_t GetCdmaCBConfig(int32_t serialId);
    int32_t SendSmsMoreMode(int32_t serialId, const OHOS::HDI::Ril::V1_0::GsmSmsMessageInfo &gsmSmsMessageInfo);
    int32_t SendSmsAck(int32_t serialId, const OHOS::HDI::Ril::V1_0::ModeData &modeData);
    int32_t AddCdmaSimMessage(int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo);
    int32_t DelCdmaSimMessage(int32_t serialId, int32_t index);
    int32_t UpdateCdmaSimMessage(int32_t serialId, const OHOS::HDI::Ril::V1_0::SmsMessageIOInfo &smsMessageIOInfo);

    int32_t SendGsmSmsResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SendCdmaSmsResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t AddSimMessageResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t DelSimMessageResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t UpdateSimMessageResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetSmscAddrResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetSmscAddrResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetCBConfigResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCBConfigResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SetCdmaCBConfigResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t GetCdmaCBConfigResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SendSmsMoreModeResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t SendSmsAckResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t AddCdmaSimMessageResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t DelCdmaSimMessageResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    int32_t UpdateCdmaSimMessageResponse(
        int32_t requestNum, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);

    int32_t SmsStatusReportNotify(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NewSmsStoredOnSimNotify(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NewSmsNotify(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t NewCdmaSmsNotify(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);
    int32_t CBConfigNotify(int32_t indType, HRilErrNumber error, const void *response, size_t responseLen);

private:
    void AddHandlerToMap();
    bool IsSmsResponse(uint32_t code);
    bool IsSmsNotification(uint32_t code);
    int32_t RequestWithInts(int32_t **p, ReqDataInfo *requestInfo, int32_t argCount, ...);
    int32_t RequestWithStrings(int32_t serial, int32_t request, int32_t count, ...);
    HDI::Ril::V1_0::CBConfigReportInfo MakeCBConfigResult(const void *response, const size_t responseLen);
    HDI::Ril::V1_0::SendSmsResultInfo MakeSendSmsResult(
        HRilRadioResponseInfo &responseInfo, int32_t serial, const void *response, const size_t responseLen);
    void CopyToHRilCdmaCBConfigInfo(
        HRilCdmaCBConfigInfo *list, OHOS::HDI::Ril::V1_0::CdmaCBConfigInfoList cellBroadcastInfoList);

    const HRilSmsReq *smsFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_SMS_H