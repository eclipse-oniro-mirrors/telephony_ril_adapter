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

#ifndef OHOS_RIL_CALL_PARCEL_H
#define OHOS_RIL_CALL_PARCEL_H

#include "hril_base_parcel.h"

namespace OHOS {
namespace Telephony {
/* From 3GPP TS 27.007 V4.3.0 (2001-12) 7.7, AT + ATD */
struct DialInfo : public HrilBaseParcel {
    int32_t serial;
    std::string address;
    int32_t clir; /* Calling Line Identification Restriction. From TS 27.007 V3.4.0 (2000-03) */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<DialInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct UusData : public HrilBaseParcel {
    int32_t uusDcs; /* Cell Broadcast Data Coding Scheme(default 0). */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<UusData> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct GetClipResult : public HrilBaseParcel {
    int32_t result; /* query results */
    int32_t action; /* parameter sets/shows the result code presentation status in the TA */
    int32_t clipStat; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<GetClipResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct GetClirResult : public HrilBaseParcel {
    int32_t result; /* query results */
    int32_t action; /* parameter sets/shows the result code presentation status in the TA */
    int32_t clirStat; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<GetClirResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallWaitResult : public HrilBaseParcel {
    int32_t result; /* query results */
    int32_t status; /* parameter sets/shows the result code presentation status in the TA */
    int32_t classCw; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallWaitResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallRestrictionResult : public HrilBaseParcel {
    int32_t result; /* query results */
    int32_t status; /* parameter sets/shows the result code presentation status in the TA */
    int32_t classCw; /* parameter shows the subscriber CLIP service status in the network, <0-4> */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallRestrictionResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallInfo : public HrilBaseParcel {
    int32_t index;
    int32_t dir;
    int32_t state;
    int32_t mode;
    int32_t mpty;
    int32_t voiceDomain;
    int32_t callType;
    std::string number;
    int32_t type;
    std::string alpha;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallInfoList : public HrilBaseParcel {
    int32_t callSize;
    int32_t flag;
    std::vector<CallInfo> calls;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallInfoList> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct EmergencyInfo : public HrilBaseParcel {
    int32_t index;
    int32_t total;
    std::string eccNum;
    int32_t category;
    int32_t simpresent;
    std::string mcc;
    int32_t abnormalService;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<EmergencyInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct EmergencyInfoList : public HrilBaseParcel {
    int32_t callSize;
    int32_t flag;
    std::vector<EmergencyInfo> calls;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<EmergencyInfoList> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallForwardSetInfo : public HrilBaseParcel {
    int32_t serial;
    int32_t reason;
    int32_t mode;
    std::string number;
    int32_t classx;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallForwardSetInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallForwardQueryResult : public HrilBaseParcel {
    int32_t serial;
    int32_t result; /* query results */
    int32_t status;
    int32_t classx;
    std::string number;
    int32_t type;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallForwardQueryResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallCringInfo : public HrilBaseParcel {
    std::string type;
    std::string pdpType;
    std::string pdpAddress;
    std::string l2p;
    std::string apn;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallCringInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallWaitInfo : public HrilBaseParcel {
    std::string number;
    int32_t type;
    int32_t businessClass;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallWaitInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallConnectInfo : public HrilBaseParcel {
    int32_t callId;
    int32_t type;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallConnectInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallEndInfo : public HrilBaseParcel {
    int32_t callId;
    int32_t duration;
    int32_t noCliCause;
    int32_t ccCause;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallEndInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallStatusInfo : public HrilBaseParcel {
    int32_t callId;
    int32_t status;
    int32_t voiceDomain;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallStatusInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CallImsServiceStatus : public HrilBaseParcel {
    int32_t smsSrvStatus;
    int32_t smsSrvRat;
    int32_t voipSrvStatus;
    int32_t voipSrvRat;
    int32_t vtSrvStatus;
    int32_t vtSrvRat;
    int32_t vsSrvStatus;
    int32_t vsSrvRat;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CallImsServiceStatus> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct UssdInfo : public HrilBaseParcel {
    int32_t n; /* Integer value, the control result code is reported, the default value is 0.
                            0: Disable the reporting of result codes;
                            1: Enable result code reporting;
                            2: Exit the session. */
    std::string str; /* USSD string, the maximum length is 160 characters. */
    int32_t dcs; /* Integer value, encoding method.
                                15: Not specified (default 7bit encoding);
                                68: 8bit encoding (not supported temporarily);
                                72: UCS2 encoding (not supported temporarily). */

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<UssdInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct UssdNoticeInfo : public HrilBaseParcel {
    int32_t m; /* Integer value.
                            0: The network does not require a TE reply (USSD-Notify initiated by the network or TE
                            The network does not need further information after starting operation);
                            1: The network needs a TE reply (USSD-Request initiated by the network, or TE sent
                            After starting the operation, the network needs further information);
                            2: The USSD session is released by the network;
                            3: Other local clients have responded;
                            4: The operation is not supported;
                            5: The network timed out. */
    std::string str; /* USSD string, the maximum length is 160 characters. */
    int32_t dcs; /* Integer value, encoding method.
                                15: Not specified (default 7bit encoding);
                                68: 8bit encoding (not supported temporarily);
                                72: UCS2 encoding (not supported temporarily). */

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<UssdNoticeInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

/*
 * Active reporting of SRVCC status is controlled by the +CIREP command.
 * This command complies with the 3GPP TS 27.007 protocol.
 */
struct SrvccStatus : public HrilBaseParcel {
    /*
     * SRVCC status.
     *  1: SRVCC starts;
     *   2: SRVCC is successful;
     *   3: SRVCC is cancelled;
     *   4: SRVCC failed.
     */
    int32_t status;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<SrvccStatus> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

/*
 * The ringback voice event reported by the modem during dialing.
 * Note: Modem private commands, not a reported field specified by the 3gpp protocol.
 */
struct RingbackVoice : public HrilBaseParcel {
    /* 0 network alerting; 1 local alerting */
    int32_t status;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<RingbackVoice> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_CALL_PARCEL_H