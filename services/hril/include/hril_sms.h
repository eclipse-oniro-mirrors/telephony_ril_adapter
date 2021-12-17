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
    HRilSms();
    ~HRilSms() = default;

    void ProcessSmsResponse(
        int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo, const void *response, size_t responseLen);
    void ProcessSmsNotify(int32_t slotId, int32_t notifyType, const struct ReportInfo *reportInfo,
        const void *response, size_t responseLen);
    void ProcessSmsRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);
    bool IsSmsRespOrNotify(uint32_t code);
    void RegisterSmsFuncs(const HRilSmsReq *smsFuncs);

private:
    void AddHandlerToMap();
    void SendGsmSms(int32_t slotId, struct HdfSBuf *data);
    void SendCdmaSms(int32_t slotId, struct HdfSBuf *data);
    void AddSimMessage(int32_t slotId, struct HdfSBuf *data);
    void DelSimMessage(int32_t slotId, struct HdfSBuf *data);
    void UpdateSimMessage(int32_t slotId, struct HdfSBuf *data);
    void SetSmscAddr(int32_t slotId, struct HdfSBuf *data);
    void GetSmscAddr(int32_t slotId, struct HdfSBuf *data);
    void SetCBConfig(int32_t slotId, struct HdfSBuf *data);
    void GetCBConfig(int32_t slotId, struct HdfSBuf *data);
    void SetCdmaCBConfig(int32_t slotId, struct HdfSBuf *data);
    void GetCdmaCBConfig(int32_t slotId, struct HdfSBuf *data);
    void SendSmsMoreMode(int32_t slotId, struct HdfSBuf *data);
    void SendSmsAck(int32_t slotId, struct HdfSBuf *data);

    int32_t SendGsmSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SendCdmaSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t AddSimMessageResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t DelSimMessageResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t UpdateSimMessageResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetSmscAddrResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetSmscAddrResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SetCdmaCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t GetCdmaCBConfigResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SendSmsMoreModeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t SendSmsAckResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    int32_t DataSbuf(HdfSBuf *dataSbuf, int32_t indType);
    bool RequestWithInts(int **p, ReqDataInfo *requestInfo, int32_t argCount, ...);
    bool RequestWithStrings(int32_t serial, int32_t slotId, int32_t request, int32_t count, ...);
    int32_t SmsStatusReportNotify(
        int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    int32_t NewSmsStoredOnSimNotify(
        int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    int32_t NewSmsNotify(int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    int32_t NewCdmaSmsNotify(int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    int32_t CBConfigNotify(int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    CBConfigReportInfo MakeCBConfigResult(const void *response, const size_t responseLen);
    SendSmsResultInfo MakeSendSmsResult(
        HRilRadioResponseInfo &responseInfo, int32_t serial, const void *response, const size_t responseLen);
    void MakeCdmaSmsInfo(HRilCdmaSmsMessageInfo &msg, const CdmaSmsMessageInfo &message);
    void MakeCdmaSmsInfo(CdmaSmsMessageInfo &msg, const HRilCdmaSmsMessageInfo &message);
    bool IsSmsResponse(uint32_t code);
    bool IsSmsNotification(uint32_t code);

    using RespFunc = int32_t (HRilSms::*)(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    using NotiFunc = int32_t (HRilSms::*)(
        int32_t slotId, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    using ReqFunc = void (HRilSms::*)(int32_t slotId, struct HdfSBuf *data);
    std::map<uint32_t, ReqFunc> reqMemberFuncMap_;
    std::map<uint32_t, NotiFunc> notiMemberFuncMap_;
    std::map<uint32_t, RespFunc> respMemberFuncMap_;
    const HRilSmsReq *smsFuncs_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS

#endif // OHOS_HRIL_SMS_H