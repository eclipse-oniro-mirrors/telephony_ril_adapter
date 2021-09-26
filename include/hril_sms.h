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

    bool RequestWithStrings(int32_t serial, int32_t slotId, int32_t request, int32_t count, ...);

    /* Requests to send an SMS */
    void SendSms(int32_t slotId, struct HdfSBuf *data);

    int32_t SendSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void StorageSms(int32_t slotId, struct HdfSBuf *data);

    int32_t StorageSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void DeleteSms(int32_t slotId, struct HdfSBuf *data);

    int32_t DeleteSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void UpdateSms(int32_t slotId, struct HdfSBuf *data);

    int32_t UpdateSmsResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void GetSmsCenterAddress(int32_t slotId, struct HdfSBuf *data);

    int32_t GetSmsCenterAddressResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void SetSmsCenterAddress(int32_t slotId, struct HdfSBuf *data);

    int32_t SetSmsCenterAddressResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    void SetCellBroadcast(int32_t slotId, struct HdfSBuf *data);

    int32_t SetCellBroadcastResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    /* Requests to send more SMS.
     * More SMS is expected soon. If possible, Keep SMS relay protocol link open. (eg TS 27.005 AT+CMMS command) */
    void SendSmsMoreMode(int32_t slotId, struct HdfSBuf *data);

    int32_t SendSmsMoreModeResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);

    bool RequestWithInts(int32_t **p, ReqDataInfo *requestInfo, int32_t argCount, ...);

    void SendSmsAck(int32_t slotId, struct HdfSBuf *data);
    int32_t SendSmsAckResponse(int32_t slotId, int32_t requestNum, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    int32_t DataSbuf(HdfSBuf *dataSbuf, uint8_t *bytes, int32_t indType);
    int32_t SmsStatusReportNotify(
        int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    int32_t NewSmsStoredOnSimNotify(
        int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);
    int32_t NewSmsNotify(int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);

    int32_t CellBroadcastNotify(
        int32_t slotId, int32_t indType, HRilErrno e, const void *response, size_t responseLen);

    CellBroadcastReportInfo MakeCellBroadcastResult(const void *response, const size_t responseLen);

    void AddHandlerToMap();
    SendSmsResultInfo MakeSendSmsResult(
        HRilRadioResponseInfo &responseInfo, int32_t serial, const void *response, const size_t responseLen);
    void ProcessSmsResponse(int32_t slotId, int32_t code, HRilRadioResponseInfo &responseInfo,
        const void *response, size_t responseLen);
    void ProcessSmsNotify(int32_t code, int32_t notifyType, HRilErrno e, const void *response, size_t responseLen);
    void ProcessSmsRequest(int32_t slotId, int32_t code, struct HdfSBuf *data);

    bool IsSmsResponse(uint32_t code);

    bool IsSmsNotification(uint32_t code);

    bool IsSmsRespOrNotify(uint32_t code);
    void RegisterSmsFuncs(const HRilSmsReq *smsFuncs);

private:
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