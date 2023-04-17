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

#ifndef OHOS_RIL_SMS_PARCEL_H
#define OHOS_RIL_SMS_PARCEL_H

#include "hril_base_parcel.h"

namespace OHOS {
namespace Telephony {
struct GsmSmsMessageInfo {
    int32_t serial = 0;
    std::string smscPdu = ""; /* Short Message Service Center Protocol Data Unit see GSM 03.40 */
    std::string pdu = ""; /* Protocol Data Unit */
    int32_t state = 0;
};

struct SendCdmaSmsMessageInfo {
    int32_t serial = 0;
    std::string smscPdu = ""; /* Short Message Service Center Protocol Data Unit see GSM 03.40 */
    int32_t state = 0;
};

struct SmsMessageIOInfo {
    int32_t serial = 0;
    std::string smscPdu = ""; /* Short Message Service Center Protocol Data Unit see GSM 03.40 */
    std::string pdu = ""; /* Protocol Data Unit */
    int32_t state = 0;
    int32_t index = 0;
};

struct ServiceCenterAddress {
    int32_t serial = 0;
    int32_t tosca = 0;
    std::string address = ""; /* Protocol Data Unit */
};

/* From 3GPP TS 27.005  AT+CSCB = [<mode>[,<mids>[,<dcss>]]] */
struct CBConfigInfo {
    int32_t serial = 0;
    int32_t mode = 0;
    int32_t indicationType = 0;
    std::string mids = "";
    std::string dcss = "";
};

struct CdmaCBConfigInfo {
    int32_t service = 0;
    int32_t language = 0;
    int32_t checked = 0;
};

struct CdmaCBConfigInfoList {
    int32_t serial = 0;
    int32_t size = 0;
    std::vector<CdmaCBConfigInfo> list {};
};

/* From 3GPP TS 27.005   if text mode (+CMGF=1):
    <CR><LF>+CBM:
    <sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data><CR>
    <LF> if PDU mode
    (+CMGF=0):
    <CR><LF>+CBM: <length><CR><LF><pdu><CR><LF>
*/
struct CBConfigReportInfo {
    int32_t indicationType = 0;
    int32_t sn = 0;
    int32_t mid = 0;
    int32_t page = 0;
    int32_t pages = 0;
    std::string dcs = "";
    std::string data = "";
    int32_t length = 0;
    std::string pdu = "";
};

struct SmsMessageInfo {
    int32_t indicationType = 0;
    int32_t size = 0;
    std::vector<uint8_t> pdu {}; /* Protocol Data Unit */
};

struct ModeData {
    int32_t serial = 0;
    bool result = false;
    int32_t mode = 0;
    std::string pdu = ""; /* Protocol Data Unit */
};

struct SendSmsResultInfo : public HrilBaseParcel {
    int32_t msgRef = 0; /* TP-Message-Reference for GSM, and BearerData MessageId for CDMA
                         * from 3GPP2 C.S0015-B, v2.0, 4.5-1 */
    std::string pdu = ""; /* Protocol Data Unit */
    int32_t errCode = 0;
    int64_t flag = 0;
    bool ReadFromParcel(Parcel &parcel)
    {
        if (!ReadBaseInt32(parcel, msgRef)) {
            return false;
        }
        if (!ReadBaseString(parcel, pdu)) {
            return false;
        }
        if (!ReadBaseInt32(parcel, errCode)) {
            return false;
        }
        if (!ReadBaseInt64(parcel, flag)) {
            return false;
        }
        return true;
    }

    bool Marshalling(Parcel &parcel) const
    {
        if (!WriteBaseInt32(parcel, msgRef)) {
            return false;
        }
        if (!WriteBaseString(parcel, pdu)) {
            return false;
        }
        if (!WriteBaseInt32(parcel, errCode)) {
            return false;
        }
        if (!WriteBaseInt64(parcel, flag)) {
            return false;
        }
        return true;
    }
    std::shared_ptr<SendSmsResultInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CdmaSmsAddress {
    int32_t digitMode = 0;
    int32_t mode = 0;
    int32_t type = 0;
    int32_t plan = 0;
    unsigned char number = 0;
    unsigned char bytes[36] = { 0 };
};

struct CdmaSmsSubAddress {
    int32_t type = 0;
    unsigned char odd = 0;
    unsigned char number = 0;
    unsigned char bytes[36] = { 0 };
};

struct CdmaSmsMessageInfo {
    int32_t serial = 0;
    int32_t serviceId = 0;
    unsigned char isExist = 0;
    int32_t type = 0;
    CdmaSmsAddress address;
    CdmaSmsSubAddress subAddress;
    int32_t size = 0;
    unsigned char bytes[255] = { 0 };
};

struct CdmaSmsInfo {
    int32_t indicationType = 0;
    int32_t msgRef = 0; /* TP-Message-Reference for GSM, and BearerData MessageId for CDMA
                         * from 3GPP2 C.S0015-B, v2.0, 4.5-1 */
    std::string pdu = ""; /* Protocol Data Unit */
    int32_t errCode = 0; /* if unknown or not applicable, that is -1
                          * from 3GPP 27.005, 3.2.5 for GSM/UMTS,
                          * 3GPP2 N.S0005 (IS-41C) Table 171 for CDMA */
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_SMS_PARCEL_H
