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

#ifndef OHOS_RIL_VENDOR_NETWORK_DEFS_H
#define OHOS_RIL_VENDOR_NETWORK_DEFS_H

#define MAX_CHAR_LEN 100

#include "hril_public_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/* From 3GPP TS 27.007 8.5 */
typedef struct {
    int32_t rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value
                      0~99 */
    int32_t ber; /* bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max */
} HRilGsmRssi;

/* From 3GPP TS 27.007 8.5 */
typedef struct {
    int32_t rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value
                      0~99 */
    int32_t ecio; /* integer type, ratio of the received energy per PN chip to the total received power
                   * spectral density (see 3GPP TS 25.133 [95] sub-clause). */
    int32_t rscp; /* integer type, received signal code power (see 3GPP TS 25.133 [95] sub-clause 9.1.1.3 and
                   * 3GPP TS 25.123 [96] sub-clause 9.1.1.1.3).
                   * range  value 0 ~ 96, 255 not known or not detectable */
    int32_t ber; /* bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max */
} HRilWcdmaRssi;

/* From 3GPP TS 27.007 8.69 */
typedef struct {
    int32_t absoluteRssi; /* Absolute value of signal strength.  This value is the actual Rssi value
                           * multiplied by -1.
                           * e.g: Rssi is -75, then this response value will be 75 */
    int32_t ecno; /* ratio of the received energy per PN chip to the total received power spectral density,
                   * e.g: If the actual Ec/Io is -12.5 dB, then this response value will be 125.
                   * from 3GPP TS 25.133[95] */
} HRilCdmaRssi;

typedef struct {
    int32_t rxlev; /* integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value
                      0~99 */
    int32_t rsrq; /* integer type, reference signal received quality (see 3GPP TS 36.133 [96] sub-clause 9.1.7)
                   * value range 0~33, 255 not known or not detectable */
    int32_t rsrp; /* integer type, reference signal received power (see 3GPP TS 36.133 [96] sub-clause 9.1.4)
                   * value range 0~97, 255 not known or not detectable */
    int32_t snr; /* integer type, representing the signal-to-interference plus noise ratio, suitable for LTE mode
                  * value range 0~251, 255 not known or not detectable */
} HRilLteRssi;

typedef struct {
    int32_t rscp;
} HRilTdScdmaRssi;

typedef struct {
    int32_t rsrp;
    int32_t rsrq;
    int32_t sinr;
} HRilNrRssi;

typedef struct {
    HRilGsmRssi gsmRssi;
    HRilCdmaRssi cdmaRssi;
    HRilWcdmaRssi wcdmaRssi;
    HRilLteRssi lteRssi;
    HRilTdScdmaRssi tdScdmaRssi;
    HRilNrRssi nrRssi;
} HRilRssi;

typedef struct {
    int32_t serial;
    int32_t selectMode;
    char *oper;
} HRilSetNetworkModeInfo;

typedef enum {
    CS_REG_TYPE = 1, /* Corresponds to AT command: AT+CREG */
    PS_REG_TYPE, /* Corresponds to AT command: AT+CGREG */
} HRilRegType;

typedef enum {
    REG_STAT_POS = 0, /* AT command: +CREG,+CGREG,regState field position index in response */
    REG_LAC_POS, /* Lac field position index in response */
    REG_CELL_ID_POS, /* CellId field position index in response */
    REG_ACT_POS, /* ActType field position index in response */
    REG_NR_AVAILABLE_POS, /* AT command: +C5GREG,isNrAvailable field position index in response */
    REG_EN_DC_AVAILABLE_POS, /* AT command: +C5GREG,isEnDcAvailable field position index in response */
    REG_DC_NR_RESTRICTED_POS, /* AT command: +C5GREG,isDcNrAvailable field position index in response */
} HRilRegPosIndex;

typedef enum {
    REG_NOTIFY_TYPE = 0, /* AT command: +CREG,+CGREG data Message notify type */
    REG_RESPONSE_TYPE, /* AT command: +CREG,+CGREG data Message response type */
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
    HRilCellConnectionStatus cellConnStatus;
    HRilRadioTech ratType;
    int32_t cellBandwidthDownlinkKhz;
    int32_t cellBandwidthUplinkKhz;
    int32_t freqRange;
    int32_t downlinkChannelNum;
    int32_t uplinkChannelNum;
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
    int32_t status;
    int32_t rat;
} AvailableOperInfo;

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

// cellList
typedef struct {
    int32_t band; /* value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900 */
    int32_t arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~1023 */
    int32_t bsic; /* cell sit code 0~63 */
    int32_t cellId;
    int32_t lac; /* Location area code 0~FFFF */
    int32_t rxlev; /* <RXLEV> dbm -120~37 */
} CellListRatGsm;

typedef struct {
    int32_t arfcn; /* Absolute Radio Frequency Channel Number of the BCCHcarrier 0~39649 */
    int32_t pci; /* Physical cell ID. 0~503 */
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

typedef struct {
    int32_t ratType;

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
    int32_t pci; /* Physical cell ID. 0~503 */
    int32_t tac; /* Tracking Area Code 0~FFFF */
    int32_t rsrp; /* Reference Signal Received Power -140~-44， dBm */
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
} CurrentCellInfoVendor;

typedef struct {
    int32_t itemNum;
    CurrentCellInfoVendor *currentCellInfo;
} CurrentCellInfoList;

typedef struct {
    void (*GetImsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetSignalStrength)(const ReqDataInfo *requestInfo);
    void (*GetCsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetPsRegStatus)(const ReqDataInfo *requestInfo);
    void (*GetOperatorInfo)(const ReqDataInfo *requestInfo);
    void (*GetNeighboringCellInfoList)(const ReqDataInfo *requestInfo);
    void (*GetCurrentCellInfo)(const ReqDataInfo *requestInfo);
    void (*GetNetworkSearchInformation)(const ReqDataInfo *requestInfo);
    void (*GetNetworkSelectionMode)(const ReqDataInfo *requestInfo);
    void (*SetNetworkSelectionMode)(const ReqDataInfo *requestInfo, const HRilSetNetworkModeInfo *data);
    void (*GetPreferredNetwork)(const ReqDataInfo *requestInfo);
    void (*SetPreferredNetwork)(const ReqDataInfo *requestInfo, const int32_t *data);
    void (*GetPhysicalChannelConfig)(const ReqDataInfo *requestInfo);
    void (*SetLocateUpdates)(const ReqDataInfo *requestInfo, HRilRegNotifyMode mode);
    void (*SetNotificationFilter)(const ReqDataInfo *requestInfo, const int32_t *newFilter);
    void (*SetDeviceState)(const ReqDataInfo *requestInfo, const int32_t *deviceStateType, const int32_t *deviceState);
} HRilNetworkReq;
#ifdef __cplusplus
}
#endif
#endif // OHOS_RIL_VENDOR_NETWORK_DEFS_H
