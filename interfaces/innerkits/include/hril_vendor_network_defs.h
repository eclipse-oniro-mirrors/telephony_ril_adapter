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

/**
 * @brief Defines the GSM received signal strength indicator.
 *
 * From 3GPP TS 27.007 8.5
 */
typedef struct {
    /**
     * Integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value
     * 0~99
     */
    int32_t rxlev;
    /**
     * Bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max
     */
    int32_t ber;
} HRilGsmRssi;

/**
 * @brief Defines the WCDMA received signal strength indicator.
 *
 * From 3GPP TS 27.007 8.5
 */
typedef struct {
    /**
     * Integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value
     * 0~99
     */
    int32_t rxlev;
    /**
     * Integer type, ratio of the received energy per PN chip to the total received power
     * spectral density (see 3GPP TS 25.133 [95] sub-clause).
     */
    int32_t ecio;
    /**
     * Integer type, received signal code power (see 3GPP TS 25.133 [95] sub-clause 9.1.1.3 and
     * 3GPP TS 25.123 [96] sub-clause 9.1.1.1.3).
     * range  value 0 ~ 96, 255 not known or not detectable
     */
    int32_t rscp;
    /**
     * Bit error rate, value range 0 ~ 7, max is 99, if unknown then set to max
     */
    int32_t ber;
} HRilWcdmaRssi;

/**
 * @brief Defines the CDMA received signal strength indicator.
 *
 * From 3GPP TS 27.007 8.69
 */
typedef struct {
    /**
     * Absolute value of signal strength.  This value is the actual Rssi value
     * multiplied by -1.
     * e.g: Rssi is -75, then this response value will be 75
     */
    int32_t absoluteRssi;
    /**
     * Ratio of the received energy per PN chip to the total received power spectral density,
     * e.g: If the actual Ec/Io is -12.5 dB, then this response value will be 125.
     * from 3GPP TS 25.133[95]
     */
    int32_t ecno;
} HRilCdmaRssi;

/**
 * @brief Defines the LTE signal strength.
 */
typedef struct {
    /**
     * Integer type, received signal strength level (see 3GPP TS 45.008 [20] sub-clause 8.1.4) value
     * 0~99
     */
    int32_t rxlev;
    /**
     * Integer type, reference signal received quality (see 3GPP TS 36.133 [96] sub-clause 9.1.7)
     * value range 0~33, 255 not known or not detectable
     */
    int32_t rsrq;
    /**
     * Integer type, reference signal received power (see 3GPP TS 36.133 [96] sub-clause 9.1.4)
     * value range 0~97, 255 not known or not detectable
     */
    int32_t rsrp;
    /**
     * Integer type, representing the signal-to-interference plus noise ratio, suitable for LTE mode
     * value range 0~251, 255 not known or not detectable
     */
    int32_t snr;
} HRilLteRssi;

/**
 * @brief Defines the TD-SCDMA signal strength.
 */
typedef struct {
    /**
     * Received signal code power
     */
    int32_t rscp;
} HRilTdScdmaRssi;

/**
 * @brief Defines the NR signal strength.
 */
typedef struct {
    /**
     * Reference signal received power
     */
    int32_t rsrp;
    /**
     * Received signal quality
     */
    int32_t rsrq;
    /**
     * Signal to interference plus noise ratio
     */
    int32_t sinr;
} HRilNrRssi;

/**
 * @brief Defines the received signal strength information.
 */
typedef struct {
    /**
     * GSM received signal strength indicator
     */
    HRilGsmRssi gsmRssi;
    /**
     * CDMA received signal strength indicator
     */
    HRilCdmaRssi cdmaRssi;
    /**
     * WCDMA received signal strength indicator
     */
    HRilWcdmaRssi wcdmaRssi;
    /**
     * LTE received signal strength indicator
     */
    HRilLteRssi lteRssi;
    /**
     * TD-SCDMA received signal strength indicator
     */
    HRilTdScdmaRssi tdScdmaRssi;
    /**
     * NR received signal strength indicator
     */
    HRilNrRssi nrRssi;
} HRilRssi;

/**
 * @brief Defines the network mode information.
 */
typedef struct {
    /**
     * Request SN
     */
    int32_t serial;
    /**
     * Network mode
     */
    int32_t selectMode;
    /**
     * Carrier
     */
    char *oper;
} HRilSetNetworkModeInfo;

/**
 * @brief Defines the registration type
 */
typedef enum {
    /**
     * Corresponds to AT command: AT+CREG
     */
    CS_REG_TYPE = 1,
    /**
     * Corresponds to AT command: AT+CGREG
     */
    PS_REG_TYPE,
} HRilRegType;

/**
 * @brief Defines the registration position index in response
 */
typedef enum {
    /**
     * AT command: +CREG,+CGREG,regState field position index in response
     */
    REG_STAT_POS = 0,
    /**
     * Lac field position index in response
     */
    REG_LAC_POS,
    /**
     * CellId field position index in response
     */
    REG_CELL_ID_POS,
    /**
     * ActType field position index in response
     */
    REG_ACT_POS,
    /**
     * AT command: +C5GREG,isNrAvailable field position index in response
     */
    REG_NR_AVAILABLE_POS,
    /**
     * AT command: +C5GREG,isEnDcAvailable field position index in response
     */
    REG_EN_DC_AVAILABLE_POS,
    /**
     * AT command: +C5GREG,isDcNrAvailable field position index in response
     */
    REG_DC_NR_RESTRICTED_POS,
} HRilRegPosIndex;

/**
 * @brief Defines the registration message type
 */
typedef enum {
    /**
     * AT command: +CREG,+CGREG data Message notify type
     */
    REG_NOTIFY_TYPE = 0,
    /**
     * AT command: +CREG,+CGREG data Message response type
     */
    REG_RESPONSE_TYPE,
} HRilRegMsgType;

/**
 * @brief Defines the registration status information.
 */
typedef struct {
    /**
     * registration type
     */
    HRilRegType regType;
    /**
     * registration message type
     */
    HRilRegMsgType regMsgType;
    /**
     * Notification type
     */
    HRilRegNotifyMode notifyMode;
    /**
     * Registration status
     */
    HRilRegStatus regStatus;
    /**
     * Location area code
     */
    int32_t lacCode;
    /**
     * Cell ID
     */
    int32_t cellId;
    /**
     * Radio access technology type
     */
    HRilRadioTech actType;
    /**
     * Whether the NR mode is available
     */
    int32_t isNrAvailable;
    /**
     * Whether ENDC is available
     */
    int32_t isEnDcAvailable;
    /**
     * Whether DCNR is restricted
     */
    int32_t isDcNrRestricted;
} HRilRegStatusInfo;

/**
 * @brief Defines the physical channel configuration.
 */
typedef struct {
    /**
     * Connection status
     */
    HRilCellConnectionStatus cellConnStatus;
    /**
     * Radio access technology type
     */
    HRilRadioTech ratType;
    /**
     * Downlink bandwidth in kHz
     */
    int32_t cellBandwidthDownlinkKhz;
    /**
     * Uplink bandwidth in kHz.
     */
    int32_t cellBandwidthUplinkKhz;
    /**
     * Frequency range
     */
    int32_t freqRange;
    /**
     * Downlink channel ID
     */
    int32_t downlinkChannelNum;
    /**
     * Uplink channel ID
     */
    int32_t uplinkChannelNum;
    /**
     * Physical cell ID
     */
    int32_t physicalCellId;
    /**
     * Logical device ID
     */
    int32_t contextIdNum;
    /**
     * Uplink channel ID
     */
    int32_t *contextIds;
} HRilPhyChannelConfig;

/**
 * @brief Defines the channel configuration information list.
 */
typedef struct {
    /**
     * Number
     */
    int32_t itemNum;
    /**
     * Physical channel configuration
     */
    HRilPhyChannelConfig *channelConfigs;
} HRilChannelConfigList;

/**
 * @brief Defines the registration status information of the IMS.
 */
typedef struct {
    /**
     * Notification type
     */
    int32_t notifyType;
    /**
     * Registration information
     */
    int32_t regInfo;
    /**
     * Extension information
     */
    int32_t extInfo;
} HRilImsRegStatusInfo;

/**
 * @brief Defines the available network information.
 */
typedef struct {
    /**
     * Long name of the registered network in alphanumeric format
     */
    char *longName;
    /**
     * Short name of the registered network in alphanumeric format
     */
    char *shortName;
    /**
     * Available network ID
     */
    char *numeric;
    /**
     * Network status
     */
    int32_t status;
    /**
     * Radio access technology type
     */
    int32_t rat;
} AvailableOperInfo;

/**
 * @brief Defines the CDMA cellular information.
 */
typedef struct {
    /**
     * System ID range is 0-65535
     */
    int32_t systemId;
    /**
     * Network ID range is 0-65535
     */
    int32_t networkId;
    /**
     * Basic ID range is 0-65535
     */
    int32_t baseId;
    /**
     * Zone ID range is 0-65535
     */
    int32_t zoneId;
    /**
     * PN pilot sequence range is 0-65535
     */
    int32_t pilotPn;
    /**
     * Pilot signal strength range is 0-65535
     */
    int32_t pilotStrength;
    /**
     * Channel range is 0-65535
     */
    int32_t channel;
    /**
     * Longitude range is -648000 -- 648000, unit: second
     */
    int32_t longitude;
    /**
     * Latitude range is -648000 -- 648000, unit: second
     */
    int32_t latitude;
} CellRatCdma;

/**
 * @brief Defines the GSM cell information.
 */
typedef struct {
    /**
     * Cell band information, value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900
     */
    int32_t band;
    /**
     * Absolute RF channel number of the broadcast control channel (BCCH) carrier, which ranges from 0 to 1023
     */
    int32_t arfcn;
    /**
     * Base transceiver station identity code 0~63
     */
    int32_t bsic;
    /**
     * Cell information ID
     */
    int32_t cellId;
    /**
     * Location area code, which ranges from 0 to 0xFFFF
     */
    int32_t lac;
    /**
     * Signal received strength, which ranges from -120 to 37
     */
    int32_t rxlev;
} CellListRatGsm;

/**
 * @brief Defines the LTE cell information.
 */
typedef struct {
    /**
     * Absolute RF channel number of the BCCH carrier, which ranges from 0 to 1023
     */
    int32_t arfcn;
    /**
     * Physical cell ID. 0~503
     */
    int32_t pci;
    /**
     * Signal received power, which ranges from -140 to -44
     */
    int32_t rsrp;
    /**
     * Signal received quality, which ranges from -19 to -3
     */
    int32_t rsrq;
    /**
     * Signal received strength, which ranges from -120 to 37
     */
    int32_t rxlev;
} CellListRatLte;

/**
 * @brief Defines the WCDMA cell information.
 */
typedef struct {
    /**
     * Absolute RF channel number of the BCCH carrier, which ranges from 0 to 16383
     */
    int32_t arfcn;
    /**
     * Primary Scrambling Code. 0~511
     */
    int32_t psc;
    /**
     * Received Signal Code Power in dBm -120~25, dBm
     */
    int32_t rscp;
    /**
     * The ratio of power per modulation bit to noise spectral density. -25~0
     */
    int32_t ecno;
} CellListRatWcdma;

/**
 * @brief Defines the TD-SCDMA cellular information.
 */
typedef struct {
    /**
     * Absolute RF channel number of the BCCH carrier
     */
    int32_t arfcn;
    /**
     * Synchronization flag
     */
    int32_t syncId;
    /**
     * Super cell
     */
    int32_t sc;
    /**
     * Cell ID
     */
    int32_t cellId;
    /**
     * Location area code
     */
    int32_t lac;
    /**
     * Received signal code power
     */
    int32_t rscp;
    /**
     * Discontinuous reception cycle
     */
    int32_t drx;
    /**
     * Routing area code
     */
    int32_t rac;
    /**
     * Super cell ID
     */
    int32_t cpid;
} CellRatTdscdma;

/**
 * @brief Defines the NR cellular information.
 */
typedef struct {
    /**
     * Absolute RF channel number of the BCCH carrier
     */
    int32_t nrArfcn;
    /**
     * Physical cell ID
     */
    int32_t pci;
    /**
     * Type allocation code
     */
    int32_t tac;
    /**
     * NR cell ID
     */
    int64_t nci;
    /**
     * Signal received power, which ranges from -140 to -44
     */
    int32_t rsrp;
    /**
     * Signal received quality, which ranges from -19 to -3
     */
    int32_t rsrq;
} CellRatNr;

/**
 * @brief Defines the cell information.
 */
typedef struct {
    /**
     * Radio access technology type
     */
    int32_t ratType;

    union {
        /**
         * GSM cellular information
         */
        CellListRatGsm gsm;
        /**
         * LTE cellular information
         */
        CellListRatLte lte;
        /**
         * WCDMA signal strength
         */
        CellListRatWcdma wcdma;
        /**
         * CDMA signal strength
         */
        CellRatCdma cdma;
        /**
         * TD-SCDMA signal strength
         */
        CellRatTdscdma tdscdma;
        /**
         * NR signal strength
         */
        CellRatNr nr;
    } ServiceCellParas;
} CellInfo;

/**
 * @brief Defines the cell information list.
 */
typedef struct {
    /**
     * Number
     */
    int32_t itemNum;
    /**
     * Neighboring cell list
     */
    CellInfo *cellNearbyInfo;
} CellInfoList;

/**
 * @brief Defines the GSM cellular information.
 */
typedef struct {
    /**
     * Cell band information value:0~3 0:GSM850 1:GSM900 0:GSM1800 0:GSM1900
     */
    int32_t band;
    /**
     * Absolute RF channel number of the BCCH carrier, which ranges from 0 to 1023
     */
    int32_t arfcn;
    /**
     * Base transceiver station identity code
     */
    int32_t bsic;
    /**
     * Cell ID
     */
    int32_t cellId;
    /**
     * Location area code, which ranges from 0 to 0xFFFF
     */
    int32_t lac;
    /**
     * Signal received strength, which ranges from -120 to 37
     */
    int32_t rxlev;
    /**
     *  Signal received quality, which ranges from 0 to 7
     */
    int32_t rxQuality;
    /**
     *  Timing advance, which ranges from 0 to 63
     */
    int32_t ta;
} CellRatGsm;

/**
 * @brief Defines the LTE cellular information.
 */
typedef struct {
    /**
     * Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023
     */
    int32_t arfcn;
    /**
     * Cell ID
     */
    int32_t cellId;
    /**
     * Physical cell ID. 0~503
     */
    int32_t pci;
    /**
     * Tracking Area Code 0~FFFF
     */
    int32_t tac;
    /**
     * Reference Signal Received Power -140~-44， dBm
     */
    int32_t rsrp;
    /**
     * Reference Signal Received Quality -19.5~-3
     */
    int32_t rsrq;
    /**
     * Receiving signal strength in dbm 90~-25
     */
    int32_t rssi;
} CellRatLte;

/**
 * @brief Defines the WCDMA cellular information.
 */
typedef struct {
    /**
     * Absolute RF channel number of the BCCH carrier, which ranges from 0 to 16383
     */
    int32_t arfcn;
    /**
     * Primary Scrambling Code. 0~511
     */
    int32_t psc;
    /**
     * Cell ID
     */
    int32_t cellId;
    /**
     * Location area code, which ranges from 0 to 0xFFFF
     */
    int32_t lac;
    /**
     * Reference Signal Received Power -140~-44, dBm
     */
    int32_t rscp;
    /**
     * Signal received strength, which ranges from -19 to -3
     */
    int32_t rxlev;
    /**
     * Received signal strength in dbm, which ranges from -90 to -25
     */
    int32_t ecno;
    /**
     * Discontinuous reception cycle, which ranges from 6 to 9
     */
    int32_t drx;
    /**
     * UMTS Terrestrial Radio Access Network (UTRAN) registration zone ID
     */
    int32_t ura;
} CellRatWcdma;

/**
 * @brief Defines the current cell information vendor
 */
typedef struct {
    /**
     * Radio access technology type
     */
    int32_t ratType;
    /**
     * Mobile country code (MCC)
     */
    int32_t mcc;
    /**
     * Mobile network code (MNC)
     */
    int32_t mnc;

    union {
        /**
         * GSM cellular information
         */
        CellRatGsm gsm;
        /**
         * LTE cellular information
         */
        CellRatLte lte;
        /**
         * WCDMA cellular information
         */
        CellRatWcdma wcdma;
        /**
         * CDMA cellular information
         */
        CellRatCdma cdma;
        /**
         * TD-SCDMA cellular information
         */
        CellRatTdscdma tdscdma;
        /**
         * NR cellular information
         */
        CellRatNr nr;
    } ServiceCellParas;
} CurrentCellInfoVendor;

/**
 * @brief Defines the current cell information list
 */
typedef struct {
    /**
     * Number of cells
     */
    int32_t itemNum;
    /**
     * Current cell information
     */
    CurrentCellInfoVendor *currentCellInfo;
} CurrentCellInfoList;

typedef struct {
    /** SSB index. */
    int32_t ssbId;

    /** Reference Signal Received Power -140~-44, dBm */
    int32_t rsrp;
} SsbIdInfoVendor;

typedef struct {
    /** Physical cell ID. */
    int32_t pci;

    /** Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int32_t arfcn;

    /** Reference Signal Received Power -140~-44, dBm */
    int32_t rsrp;

    /** Signal To Interference Plus Noise Ratio. */
    int32_t sinr;

    /** Neighboring cell ssbId list, always size is 4 */
    SsbIdInfoVendor *ssbIdList;
} NeighboringCellSsbInfoVendor;

typedef struct {
    /** Absolute Radio Frequency Channel Number of the BCCH carrier 0~1023 */
    int32_t arfcn;

    /** Context Identifier. */
    int64_t cid;

    /** Physical cell ID. */
    int32_t pic;

    /** Reference Signal Received Power -140~-44, dBm */
    int32_t rsrp;

    /** Signal To Interference Plus Noise Ratio. */
    int32_t sinr;

    /** Time advance. */
    int32_t timeAdvance;

    /** Service cell ssbId list, always size is 8 */
    SsbIdInfoVendor *sCellSsbList;

    /** Neighboring cell ssb list count, max size is 4 */
    int32_t nbCellCount;

    /** Neighboring cell ssb info list, max size is 4 */
    NeighboringCellSsbInfoVendor *nbCellSsbList;
} NrCellSsbIdsVendor;

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
    void (*GetRrcConnectionState)(const ReqDataInfo *requestInfo);
    void (*SetNrOptionMode)(const ReqDataInfo *requestInfo, const int32_t *data);
    void (*GetNrOptionMode)(const ReqDataInfo *requestInfo);
    void (*GetNrSsbId)(const ReqDataInfo *requestInfo);
} HRilNetworkReq;
#ifdef __cplusplus
}
#endif
#endif // OHOS_RIL_VENDOR_NETWORK_DEFS_H
