/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_RIL_NETWORK_PARCEL_H
#define OHOS_RIL_NETWORK_PARCEL_H

#include "hril_base_parcel.h"

namespace OHOS {
namespace Telephony {
struct OperatorInfoResult : public HrilBaseParcel {
    std::string longName;
    std::string shortName;
    std::string numeric;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<OperatorInfoResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct AvailableNetworkInfo : public HrilBaseParcel {
    std::string longName;
    std::string shortName;
    std::string numeric;
    int32_t status;
    int32_t rat;
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<AvailableNetworkInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct AvailableNetworkList : public HrilBaseParcel {
    int32_t itemNum;
    std::vector<AvailableNetworkInfo> availableNetworkInfo;
    int64_t flag;
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<AvailableNetworkList> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct SetNetworkModeInfo : public HrilBaseParcel {
    int32_t selectMode;
    std::string oper; /* Operator information */
    int64_t flag;
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<SetNetworkModeInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

/* Voice registration status results */
struct CsRegStatusInfo : public HrilBaseParcel {
    int32_t notifyType; /* The notifyType,Indicate the content of the notification */
    HRilRegStatus regStatus; /* The corresponding valid registration states are NOT_REG_MT_NOT_SEARCHING_OP,
                              * "REG_MT_HOME, NOT_REG_MT_SEARCHING_OP, REG_DENIED,  UNKNOWN, REG_ROAMING". */
    int32_t lacCode;
    int32_t cellId;
    HRilRadioTech radioTechnology; /* Available voice radio technology, RMS defined by radio technology */
    int64_t flag; /* flag, Used by search network manager in response */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CsRegStatusInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct PsRegStatusResultInfo : public HrilBaseParcel {
    int32_t notifyType; /* The notifyType,Indicate the content of the notification */
    HRilRegStatus regStatus; /* valid when are is ITE UNKNOWN REG = REG, otherwise it defined in RegStatus */
    int32_t lacCode;
    int32_t cellId;
    HRilRadioTech radioTechnology;
    bool isNrAvailable;
    bool isEnDcAvailable;
    bool isDcNrRestricted;
    int64_t flag; /* flag, Used by search network manager in response */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<PsRegStatusResultInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct PhysicalChannelConfig : public HrilBaseParcel {
    HRilCellConnectionStatus cellConnStatus;
    HRilRadioTech ratType;
    int32_t cellBandwidthDownlinkKhz;
    int32_t cellBandwidthUplinkKhz;
    int32_t freqRange;
    int32_t downlinkChannelNum;
    int32_t uplinkChannelNum;
    int32_t physicalCellId;
    int32_t contextIdNum;
    std::vector<int32_t> contextIds;
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<PhysicalChannelConfig> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct ChannelConfigInfoList : public HrilBaseParcel {
    int32_t itemNum;
    std::vector<PhysicalChannelConfig> channelConfigInfos;
    int64_t flag; /* flag, Used by search network manager in response */
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<ChannelConfigInfoList> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

typedef struct {
    int32_t band; /* value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900 */
    int32_t arfcn; /* Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int32_t bsic; /* cell sit code 0~63 */
    int32_t cellId;
    int32_t lac; /* Location area code 0~FFFF */
    int32_t rxlev; /* <RXLEV> dbm -120~37 */
    int32_t rxQuality; /* 0~7 */
    int32_t ta; /* 0~63 */
} CellRatGsm;

typedef struct {
    int32_t arfcn; /* Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int32_t cellId;
    int32_t pci;
    int32_t tac; /* Tracking Area Code 0~FFFF */
    int32_t rsrp; /* Reference Signal Received Power -140~-44, dBm */
    int32_t rsrq; /* Reference Signal Received Quality -19.5~-3 */
    int32_t rssi; /* Receiving signal strength in dbm 90~-25 */
} CellRatLte;

typedef struct {
    int32_t arfcn; /* Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int32_t psc; /* Primary Scrambling Code. 0~511 */
    int32_t cellId;
    int32_t lac; /* Tracking Area Code 0~FFFF */
    int32_t rscp; /* Reference Signal Received Power -140~-44, dBm */
    int32_t rxlev; /* Reference Signal Received Quality -19.5~-3 */
    int32_t ecno; /* Receiving signal strength in dbm 90~-25 */
    int32_t drx; /* Discontinuous reception cycle length. 6~9 */
    int32_t ura; /* UTRAN Registration Area Identity. 0~65535 */
} CellRatWcdma;

typedef struct {
    int32_t systemId; /* integer type and range is 0-65535 */
    int32_t networkId; /* integer type and range is 0-65535 */
    int32_t baseId; /* integer type and range is 0-65535 */
    int32_t zoneId; /* integer type and range is 0-65535 */
    int32_t pilotPn; /* integer type and range is 0-65535 */
    int32_t pilotStrength; /* integer type and range is 0-65535 */
    int32_t channel; /* integer type and range is 0-65535 */
    int32_t longitude; /* integer type and range is -648000 -- 648000, unit: second */
    int32_t latitude; /* integer type and range is -648000 -- 648000, unit: second */
} CellRatCdma;

typedef struct {
    int32_t arfcn;
    int32_t syncId;
    int32_t sc;
    int32_t cellId;
    int32_t lac;
    int32_t rscp;
    int32_t drx;
    int32_t rac;
    int32_t cpid;
} CellRatTdscdma;

typedef struct {
    int32_t nrArfcn;
    int32_t pci;
    int32_t tac;
    int64_t nci;
} CellRatNr;

struct CurrentCellInfo : public HrilBaseParcel {
    int32_t ratType;
    int32_t mcc;
    int32_t mnc;

    union {
        CellRatGsm gsm;
        CellRatLte lte;
        CellRatWcdma wcdma;
        CellRatCdma cdma;
        CellRatTdscdma tdscdma;
        CellRatNr nr;
    } ServiceCellParas;

    bool ReadRayTypeGsmParcel(Parcel &parcel);
    bool ReadRayTypeLteParcel(Parcel &parcel);
    bool ReadRayTypeWcdmaParcel(Parcel &parcel);
    bool ReadRayTypeCdmaParcel(Parcel &parcel);
    bool ReadRayTypeTdscdmaParcel(Parcel &parcel);
    bool ReadRayTypeNrParcel(Parcel &parcel);
    bool WriteRayTypeGsmParcel(Parcel &parcel) const;
    bool WriteRayTypeLteParcel(Parcel &parcel) const;
    bool WriteRayTypeWcdmaParcel(Parcel &parcel) const;
    bool WriteRayTypeCdmaParcel(Parcel &parcel) const;
    bool WriteRayTypeTdscdmaParcel(Parcel &parcel) const;
    bool WriteRayTypeNrParcel(Parcel &parcel) const;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CurrentCellInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CellListCurrentInfo : public HrilBaseParcel {
    int32_t itemNum;
    std::vector<CurrentCellInfo> cellCurrentInfo;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CellListCurrentInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

typedef struct {
    int32_t band; /* value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900 */
    int32_t arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~1023 */
    int32_t bsic; /* cell sit code 0~63 */
    int32_t cellId;
    int32_t lac; /* Location area code 0~FFFF */
    int32_t rxlev; /* <RXLEV> dbm -120~37 */
} CellListRatGsm;

typedef struct {
    int32_t arfcn; /* Absolute Radio FreListquency Channel Number of the BCCHcarrier 0~1023 */
    int32_t pci;
    int32_t rsrp; /* Reference Signal Received Power -140~-44, dBm */
    int32_t rsrq; /* Reference Signal Received Quality -19.5~-3 */
    int32_t rxlev; /* <RXLEV> dbm -120~37 */
} CellListRatLte;

typedef struct {
    int32_t arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~16383 */
    int32_t psc; /* Primary Scrambling Code. 0~511 */
    int32_t rscp; /* Received Signal Code Power in dBm -120~25, dBm */
    int32_t ecno; /* The ratio of power per modulation bit to noise spectral density. -25~0 */
} CellListRatWcdma;

typedef struct {
    int32_t systemId; /* integer type and range is 0-65535 */
    int32_t networkId; /* integer type and range is 0-65535 */
    int32_t baseId; /* integer type and range is 0-65535 */
    int32_t zoneId; /* integer type and range is 0-65535 */
    int32_t pilotPn; /* integer type and range is 0-65535 */
    int32_t pilotStrength; /* integer type and range is 0-65535 */
    int32_t channel; /* integer type and range is 0-65535 */
    int32_t longitude; /* integer type and range is -648000 -- 648000, unit: second */
    int32_t latitude; /* integer type and range is -648000 -- 648000, unit: second */
} CellListRatCdma;

typedef struct {
    int32_t arfcn;
    int32_t syncId;
    int32_t sc;
    int32_t cellId;
    int32_t lac;
    int32_t rscp;
    int32_t drx;
    int32_t rac;
    int32_t cpid; /* 8-bit Cell Parameters ID described in TS 25.331, 0..127, INT_MAX if unknown */
} CellListRatTdscdma;

typedef struct {
    int32_t nrArfcn;
    int32_t pci;
    int32_t tac;
    int64_t nci;
} CellListRatNr;

struct CellNearbyInfo : public HrilBaseParcel {
    int32_t ratType;

    union {
        CellListRatGsm gsm;
        CellListRatLte lte;
        CellListRatWcdma wcdma;
        CellListRatCdma cdma;
        CellListRatTdscdma tdscdma;
        CellListRatNr nr;
    } ServiceCellParas;

    bool ReadRayTypeGsmListParcel(Parcel &parcel);
    bool ReadRayTypeLteListParcel(Parcel &parcel);
    bool ReadRayTypeWcdmaListParcel(Parcel &parcel);
    bool ReadRayTypeCdmaListParcel(Parcel &parcel);
    bool ReadRayTypeTdscdmaListParcel(Parcel &parcel);
    bool ReadRayTypeNrListParcel(Parcel &parcel);
    bool WriteRayTypeGsmListParcel(Parcel &parcel) const;
    bool WriteRayTypeLteListParcel(Parcel &parcel) const;
    bool WriteRayTypeWcdmaListParcel(Parcel &parcel) const;
    bool WriteRayTypeCdmaListParcel(Parcel &parcel) const;
    bool WriteRayTypeTdscdmaListParcel(Parcel &parcel) const;
    bool WriteRayTypeNrListParcel(Parcel &parcel) const;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CellNearbyInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CellListNearbyInfo : public HrilBaseParcel {
    int32_t itemNum;
    std::vector<CellNearbyInfo> cellNearbyInfo;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CellListNearbyInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct PreferredNetworkTypeInfo : public HrilBaseParcel {
    int32_t preferredNetworkType;
    int64_t flag;
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<PreferredNetworkTypeInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_NETWORK_PARCEL_H
