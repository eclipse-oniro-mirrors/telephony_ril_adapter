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

private:
    void AddHandlerToMap();
    int32_t SendGsmSms(struct HdfSBuf *data);
    int32_t SendCdmaSms(struct HdfSBuf *data);
    int32_t AddSimMessage(struct HdfSBuf *data);
    int32_t DelSimMessage(struct HdfSBuf *data);
    int32_t UpdateSimMessage(struct HdfSBuf *data);
    int32_t SetSmscAddr(struct HdfSBuf *data);
    int32_t GetSmscAddr(struct HdfSBuf *data);
    int32_t SetCBConfig(struct HdfSBuf *data);
    int32_t GetCBConfig(struct HdfSBuf *data);
    int32_t SetCdmaCBConfig(struct HdfSBuf *data);
    int32_t GetCdmaCBConfig(struct HdfSBuf *data);
    int32_t SendSmsMoreMode(struct HdfSBuf *data);
    int32_t SendSmsAck(struct HdfSBuf *data);
    int32_t AddCdmaSimMessage(struct HdfSBuf *data);
    int32_t DelCdmaSimMessage(struct HdfSBuf *data);
    int32_t UpdateCdmaSimMessage(struct HdfSBuf *data);

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

    int32_t SmsStatusReportNotify(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NewSmsStoredOnSimNotify(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NewSmsNotify(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t NewCdmaSmsNotify(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);
    int32_t CBConfigNotify(int32_t indType, HRilErrNumber e, const void *response, size_t responseLen);

    bool IsSmsResponse(uint32_t code);
    bool IsSmsNotification(uint32_t code);
    int32_t DataSbuf(HdfSBuf *dataSbuf, int32_t indType);
    int32_t RequestWithInts(int **p, ReqDataInfo *requestInfo, int32_t argCount, ...);
    int32_t RequestWithStrings(int32_t serial, int32_t request, int32_t count, ...);
    CBConfigReportInfo MakeCBConfigResult(const void *response, const size_t responseLen);
    SendSmsResultInfo MakeSendSmsResult(
        HRilRadioResponseInfo &responseInfo, int32_t serial, const void *response, const size_t responseLen);

    const HRilSmsReq *smsFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_HRIL_SMS_H