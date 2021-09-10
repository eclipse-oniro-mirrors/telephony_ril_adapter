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

#ifndef OHOS_RIL_NETWORK_PARCEL_H
#define OHOS_RIL_NETWORK_PARCEL_H

#include "parcel.h"
#include "hril_types.h"

namespace OHOS {
namespace Telephony {
struct OperatorInfoResult : public Parcelable {
    std::string longName;
    std::string shortName;
    std::string numeric;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<OperatorInfoResult> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct AvailableNetworkInfo : public Parcelable {
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

struct AvailableNetworkList : public Parcelable {
    int32_t itemNum;
    std::vector<AvailableNetworkInfo> availableNetworkInfo;
    int64_t flag;
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<AvailableNetworkList> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct SetNetworkModeInfo : public Parcelable {
    int32_t serial;
    int32_t selectMode;
    std::string oper; /* Operator information */
    int64_t flag;
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<SetNetworkModeInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

/* Voice registration status results */
struct CsRegStatusInfo : public Parcelable {
    int32_t regStatus; /* The corresponding valid registration states are NOT_REG_MT_NOT_SEARCHING_OP,
                        * "REG_HOME, NOT_REG_MT_SEARCHING_OP, REG_DENIED,  UNKNOWN, REG_ROAMING". */
    int32_t radioTechnology; /* Available voice radio technology, RMS defined by radio technology */

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CsRegStatusInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct PsRegStatusResultInfo : public Parcelable {
    int32_t regStatus; /* valid when are is ITE UNKNOWN REG = REG, otherwise it defined in RegStatus */
    int32_t radioTechnology;
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<PsRegStatusResultInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

typedef enum {
    RAT_TYPE_NONE = 0, /* indicates no cell information */
    RAT_TYPE_GSM,
    RAT_TYPE_LTE,
    RAT_TYPE_WCDMA,
    RAT_TYPE_TD_SCDMA
} RatType; // Radio Access Technology

typedef struct {
    int band; /* value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900 */
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int bsic; /* cell sit code 0~63 */
    int cellId;
    int lac; /* Location area code 0~FFFF */
    int rxlev; /* <RXLEV> dbm -120~37 */
    int rxQuality; /* 0~7 */
    int ta; /* 0~63 */
} CellRatGsm;

typedef struct {
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int cellId;
    int pci;
    int tac; /* Tracking Area Code 0~FFFF */
    int rsrp; /* Reference Signal Received Power -140~-44, dBm */
    int rsrq; /* Reference Signal Received Quality -19.5~-3 */
    int rssi; /* Receiving signal strength in dbm 90~-25 */
} CellRatLte;

typedef struct {
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int psc; /* Primary Scrambling Code. 0~511 */
    int cellId;
    int lac; /* Tracking Area Code 0~FFFF */
    int rscp; /* Reference Signal Received Power -140~-44, dBm */
    int rxlev; /* Reference Signal Received Quality -19.5~-3 */
    int ecno; /* Receiving signal strength in dbm 90~-25 */
    int drx; /* Discontinuous reception cycle length. 6~9 */
    int ura; /* UTRAN Registration Area Identity. 0~65535 */
} CellRatWcdma;

struct CurrentCellInfo : public Parcelable {
    int ratType;
    int mcc;
    int mnc;

    union {
        CellRatGsm gsm;
        CellRatLte lte;
        CellRatWcdma wcdma;
    } ServiceCellParas;

    bool ReadRayTypeGsmParcel(Parcel &parcel);
    bool ReadRayTypeLteParcel(Parcel &parcel);
    bool WriteRayTypeGsmParcel(Parcel &parcel) const;
    bool WriteRayTypeLteParcel(Parcel &parcel) const;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CurrentCellInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct LocationUpdateResultInfo : public Parcelable {
    int32_t regState;
    std::string lac;
    std::string ci;
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<LocationUpdateResultInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

typedef struct {
    int band; /* value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900 */
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~1023 */
    int bsic; /* cell sit code 0~63 */
    int cellId;
    int lac; /* Location area code 0~FFFF */
    int rxlev; /* <RXLEV> dbm -120~37 */
} CellListRatGsm;

typedef struct {
    int arfcn; /* Absolute Radio FreListquency Channel Number of the BCCHcarrier 0~1023 */
    int pci;
    int rsrp; /* Reference Signal Received Power -140~-44, dBm */
    int rsrq; /* Reference Signal Received Quality -19.5~-3 */
    int rxlev; /* <RXLEV> dbm -120~37 */
} CellListRatLte;

typedef struct {
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~16383 */
    int psc; /* Primary Scrambling Code. 0~511 */
    int rscp; /* Received Signal Code Power in dBm -120~25, dBm */
    int ecno; /* The ratio of power per modulation bit to noise spectral density. -25~0 */
} CellListRatWcdma;

struct CellNearbyInfo : public Parcelable {
    int ratType;

    union {
        CellListRatGsm gsm;
        CellListRatLte lte;
        CellListRatWcdma wcdma;
    } ServiceCellParas;

    bool ReadRayTypeGsmListParcel(Parcel &parcel);
    bool ReadRayTypeLteListParcel(Parcel &parcel);
    bool WriteRayTypeGsmListParcel(Parcel &parcel) const;
    bool WriteRayTypeLteListParcel(Parcel &parcel) const;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CellNearbyInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};

struct CellListNearbyInfo : public Parcelable {
    int32_t itemNum;
    std::vector<CellNearbyInfo> cellNearbyInfo;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    std::shared_ptr<CellListNearbyInfo> UnMarshalling(Parcel &parcel);
    void Dump(std::string, int32_t);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_RIL_NETWORK_PARCEL_H