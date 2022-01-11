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

#ifndef OHOS_RIL_VENDOR_NETWORK_DEFS_H
#define OHOS_RIL_VENDOR_NETWORK_DEFS_H

#define MAX_CHAR_LEN 100

#include <stdint.h>

#include "hril_public_struct.h"

/* From 3GPP TS 27.007 8.5 */
typedef struct {
    int rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99 */
    int ber; /* bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max */
} HRilGsmRssi;

/* From 3GPP TS 27.007 8.5 */
typedef struct {
    int rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99 */
    int ecio; /* integer type, ratio of the received energy per PN chip to the total received power
               * spectral density (see 3GPP TS 25.133 [95] sub-clause). */
    int rscp; /* integer type, received signal code power (see 3GPP TS 25.133 [95] sub-clause 9.1.1.3 and
               * 3GPP TS 25.123 [96] sub-clause 9.1.1.1.3).
               * range  value 0 ~ 96, 255 not known or not detectable */
    int ber; /* bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max */
} HRilWcdmaRssi;

/* From 3GPP TS 27.007 8.69 */
typedef struct {
    int absoluteRssi; /* Absolute value of signal strength.  This value is the actual Rssi value
                       * multiplied by -1.
                       * e.g: Rssi is -75, then this response value will be 75 */
    int ecno; /* ratio of the received energy per PN chip to the total received power spectral density,
               * e.g: If the actual Ec/Io is -12.5 dB, then this response value will be 125.
               * from 3GPP TS 25.133[95] */
} HRilCdmaRssi;

typedef struct {
    int rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value 0~99 */
    int rsrq; /* integer type, reference signal received quality (see 3GPP TS 36.133 [96] sub-clause 9.1.7)
               * value range 0~33, 255 not known or not detectable */
    int rsrp; /* integer type, reference signal received power (see 3GPP TS 36.133 [96] sub-clause 9.1.4)
               * value range 0~97, 255 not known or not detectable */
    int snr; /* integer type, representing the signal-to-interference plus noise ratio, suitable for LTE mode
              * value range 0~251, 255 not known or not detectable */
} HRilLteRssi;

typedef struct {
    int rscp;
} HRilTdScdmaRssi;

typedef struct {
    int rsrp;
    int rsrq;
    int sinr;
}HRilNrRssi;

typedef struct {
    HRilGsmRssi gsmRssi;
    HRilCdmaRssi cdmaRssi;
    HRilWcdmaRssi wcdmaRssi;
    HRilLteRssi lteRssi;
    HRilTdScdmaRssi tdScdmaRssi;
    HRilNrRssi nrRssi;
} HRilRssi;

typedef struct {
    int serial;
    int selectMode;
    char *oper;
} HRilSetNetworkModeInfo;

typedef enum {
    CS_REG_TYPE = 1, /* Corresponds to AT command: AT+CREG */
    PS_REG_TYPE, /* Corresponds to AT command: AT+CGREG */
    PS_5GREG_TYPE, /* Corresponds to AT command: AT+C5GREG */
    EPS_REG_TYPE,  /* Corresponds to AT command: AT+CEREG */
} HRilRegType;

typedef enum {
    REG_STAT_POS = 0, /* AT command: +CREG,+CGREG,+CEREG,+C5GREG,regState feild position index in response */
    REG_LAC_POS, /* Lac feild position index in response */
    REG_CELL_ID_POS, /* CellId feild position index in response */
    REG_ACT_POS, /* Acttype feild position index in response */
    REG_NR_AVAILABLE_POS, /* AT command: +C5GREG,isNrAvailable feild position index in response */
    REG_EN_DC_AVAILABLE_POS, /* AT command: +C5GREG,isEnDcAvailable feild position index in response */
    REG_DC_NR_RESTRICTED_POS, /* AT command: +C5GREG,isDcNrAvailable feild position index in response */
} HRilRegPosIndex;

typedef enum {
    REG_NOTIFY_TYPE = 0, /* AT command: +CREG,+CGREG,+CEREG,+C5GREG, data Mesage notify type */
    REG_RESPONSE_TYPE, /* AT command: +CREG,+CGREG,+CEREG,+C5GREG, data Mesage response type */
} HRilRegMsgType;

typedef struct {
    HRilRegType regType;
    HRilRegMsgType regMsgType;
    HRilRegNotifyMode notifyMode;
    HRilRegStatus regStatus;
    int32_t lacCode;
    int32_t cellId;
    HRilRadioTech actType;
    int32_t isNrAvailable;
    int32_t isEnDcAvailable;
    int32_t isDcNrRestricted;
} HRilRegStatusInfo;

typedef struct {
    int32_t cellConnStatus;
    int32_t cellBandwidth;
    int32_t ratType;
    int32_t freqRange;
    int32_t channelNum;
    int32_t physicalCellId;
    int32_t contextIdNum;
    int32_t *contextIds;
} HRilPhyChannelConfig;

typedef struct {
    int32_t itemNum;
    HRilPhyChannelConfig *channelConfigs;
} HRilChannelConfigList;

typedef struct {
    int32_t notifyType;
    int32_t regInfo;
    int32_t extInfo;
} HRilImsRegStatusInfo;

typedef struct {
    char *longName;
    char *shortName;
    char *numeric;
    int status;
    int rat;
} AvailableOperInfo;

typedef struct {
    int systemId; /* integer type and range is 0-65535 */
    int networkId; /* integer type and range is 0-65535 */
    int baseId; /* integer type and range is 0-65535 */
    int zoneId; /* integer type and range is 0-65535 */
    int pilotPn; /* integer type and range is 0-65535 */
    int pilotStrength; /* integer type and range is 0-65535 */
    int channel; /* integer type and range is 0-65535 */
    int longitude; /* integer typeand range is -648000 -- 648000, unit: second */
    int latitude; /* integer typeand range is -648000 -- 648000, unit: second */
} CellRatCdma;

// cellList
typedef struct {
    int band; /* value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900 */
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~1023 */
    int bsic; /* cell sit code 0~63 */
    int cellId;
    int lac; /* Location area code 0~FFFF */
    int rxlev; /* <RXLEV> dbm -120~37 */
} CellListRatGsm;

typedef struct {
    int arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~39649 */
    int pci; /* Physical cell ID. 0~503 */
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

typedef struct {
    int arfcn;
    int syncId;
    int sc;
    int cellId;
    int lac;
    int rscp;
    int drx;
    int rac;
    int cpid;
} CellRatTdscdma;

typedef struct {
    int nrArfcn;
    int pci;
    int tac;
    int nci;
}CellRatNr;

typedef struct {
    int ratType;

    union {
        CellListRatGsm gsm;
        CellListRatLte lte;
        CellListRatWcdma wcdma;
        CellRatCdma cdma;
        CellRatTdscdma tdscdma;
        CellRatNr nr;
    } ServiceCellParas;
} CellInfo;

typedef struct {
    int32_t itemNum;
    CellInfo *cellNearbyInfo;
} CellInfoList;

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
    int pci; /* Physical cell ID. 0~503 */
    int tac; /* Tracking Area Code 0~FFFF */
    int rsrp; /* Reference Signal Received Power -140~-44， dBm */
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

typedef struct {
    int ratType;
    int mcc;
    int mnc;

    union {
        CellRatGsm gsm;
        CellRatLte lte;
        CellRatWcdma wcdma;
        CellRatCdma cdma;
        CellRatTdscdma tdscdma;
        CellRatNr nr;
    } ServiceCellParas;
} CurrentCellInfoVendor;

typedef struct {
    int32_t ratfamily;
    char modemId[MAX_CHAR_LEN];
} HRilRadioCapability;

typedef struct {
    void (*GetImsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetSignalStrength)(const ReqDataInfo *requestInfo);
    void (*GetCsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetPsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetOperatorInfo)(const ReqDataInfo *requestInfo);
    void (*GetCellInfoList)(const ReqDataInfo *requestInfo);
    void (*GetCurrentCellInfo)(const ReqDataInfo *requestInfo);
    void (*GetNetworkSearchInformation)(const ReqDataInfo *requestInfo);
    void (*GetNetworkSelectionMode)(const ReqDataInfo *requestInfo);
    void (*SetNetworkSelectionMode)(const ReqDataInfo *requestInfo, const HRilSetNetworkModeInfo *data);
    void (*GetPreferredNetwork)(const ReqDataInfo *requestInfo);
    void (*SetPreferredNetwork)(const ReqDataInfo *requestInfo, const int32_t *data);
    void (*SetPsAttachStatus)(const ReqDataInfo *requestInfo, const int32_t *data);
    void (*GetPsAttachStatus)(const ReqDataInfo *requestInfo);
    void (*GetRadioCapability)(const ReqDataInfo *requestInfo);
    void (*SetRadioCapability)(const ReqDataInfo *requestInfo, const HRilRadioCapability *data);
    void (*GetPhysicalChannelConfig)(const ReqDataInfo *requestInfo);
    void (*SetLocateUpdates)(const ReqDataInfo *requestInfo, HRilRegNotifyMode mode);
} HRilNetworkReq;
#endif // OHOS_RIL_VENDOR_NETWORK_DEFS_H
