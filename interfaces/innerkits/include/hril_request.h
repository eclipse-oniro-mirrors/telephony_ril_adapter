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

#ifndef OHOS_RIL_REQUEST_H
#define OHOS_RIL_REQUEST_H

#define HRIL_RESPONSE_ACKNOWLEDGEMENT 800
typedef enum {
    HREQ_CALL_BASE = 0,

    /** Get call list. */
    HREQ_CALL_GET_CALL_LIST,

    /** Initiate voice call. */
    HREQ_CALL_DIAL,

    /** Hang up a call. */
    HREQ_CALL_HANGUP,

    /** Reject a specific call. */
    HREQ_CALL_REJECT,

    /** Answer incoming call. */
    HREQ_CALL_ANSWER,

    /** call hold value 6 */
    HREQ_CALL_HOLD_CALL,

    /** call active value 7 */
    HREQ_CALL_UNHOLD_CALL,

    /** Switch waiting or holding call and active call. */
    HREQ_CALL_SWITCH_CALL,

    /** Combine holding and active call. */
    HREQ_CALL_COMBINE_CONFERENCE,

    /** Keep all other calls except the xth call */
    HREQ_CALL_SEPARATE_CONFERENCE,

    /** Holds and resumes a call. */
    HREQ_CALL_CALL_SUPPLEMENT,

    /** Send a DTMF tone. */
    HREQ_CALL_SEND_DTMF,

    /** Start playing a DTMF tone. */
    HREQ_CALL_START_DTMF,

    /** Stop playing a currently playing DTMF tone. */
    HREQ_CALL_STOP_DTMF,

    /**
     * Called when set Calling line Identification Presentation Supplementary
     * Service(CLIP).
     */
    HREQ_CALL_SET_CLIP,

    /**
     * Called when get Calling line Identification Presentation Supplementary
     * Service(CLIP).
     */
    HREQ_CALL_GET_CLIP,

    /** Called when get call waiting. */
    HREQ_CALL_GET_CALL_WAITING,

    /** Called when set call waiting. */
    HREQ_CALL_SET_CALL_WAITING,

    /** Called when get call restriction. */
    HREQ_CALL_GET_CALL_RESTRICTION,

    /** Called when set call restriction. */
    HREQ_CALL_SET_CALL_RESTRICTION,

    /** Called when get call transfer info. */
    HREQ_CALL_GET_CALL_TRANSFER_INFO,

    /** Called when set call transfer info. */
    HREQ_CALL_SET_CALL_TRANSFER_INFO,

    /** Called when get Calling line identification restriction(CLIR). */
    HREQ_CALL_GET_CLIR,

    /** Called when set Calling line identification restriction(CLIR). */
    HREQ_CALL_SET_CLIR,

    /** Called when get call preference. */
    HREQ_CALL_GET_CALL_PREFERENCE,

    /** Called when set call preference. */
    HREQ_CALL_SET_CALL_PREFERENCE,

    /**
     * Called when set unstructured supplementary service data (USSD)
     * information.
     */
    HREQ_CALL_SET_USSD,

    /**
     * Called when get unstructured supplementary service data (USSD)
     * information.
     */
    HREQ_CALL_GET_USSD,

    /** Called when set mute mode. */
    HREQ_CALL_SET_MUTE,

    /** Called when get mute mode. */
    HREQ_CALL_GET_MUTE,

    /** Get emergency list. */
    HREQ_CALL_GET_EMERGENCY_LIST,

    /** Set emergency list. */
    HREQ_CALL_SET_EMERGENCY_LIST,

    /** Get the call failure cause. */
    HREQ_CALL_GET_FAIL_REASON,

    /** Set the call barring password. */
    HREQ_CALL_SET_BARRING_PASSWORD,

    /** Close unfinished unstructured supplementary service data (USSD). */
    HREQ_CALL_CLOSE_UNFINISHED_USSD,
    HREQ_EXPLICIT_CALL_TRANSFER_CONNECTION,
    HREQ_SET_VONR_SWITCH,

    /** Enable or disable supplementary service related notification from the network. */
    HREQ_CALL_SET_SUPP_SRV_NOTIFICATION,

    /** Set the TTY mode. */
    HREQ_CALL_GET_TTY_MODE,

    /** Get the TTY mode. */
    HREQ_CALL_SET_TTY_MODE,

    HREQ_SMS_BASE = 100,

    /** Send GSM SMS. */
    HREQ_SMS_SEND_GSM_SMS,

    /** Send CDMA SMS. */
    HREQ_SMS_SEND_CDMA_SMS,

    /** Add SIM message. */
    HREQ_SMS_ADD_SIM_MESSAGE,

    /** Delete SIM message. */
    HREQ_SMS_DEL_SIM_MESSAGE,

    /** Update SIM message. */
    HREQ_SMS_UPDATE_SIM_MESSAGE,

    /**
     * Send amd SMS message, and that more messages are expected to be sent
     * soon.
     */
    HREQ_SMS_SEND_SMS_MORE_MODE,

    /** Acknowledge successful or failed receipt of SMS. */
    HREQ_SMS_SEND_SMS_ACK,

    /** Sets the default Short Message Service Center address on the device. */
    HREQ_SMS_SET_SMSC_ADDR,

    /** Get the default Short Message Service Center address on the device. */
    HREQ_SMS_GET_SMSC_ADDR,

    /** Activates GSM cell broadcast. */
    HREQ_SMS_SET_CB_CONFIG,

    /** Get the GSM cell broadcast configuration. */
    HREQ_SMS_GET_CB_CONFIG,

    /** Get the CDMA cell broadcast configuration. */
    HREQ_SMS_GET_CDMA_CB_CONFIG,

    /** Set the CDMA cell broadcast configuration. */
    HREQ_SMS_SET_CDMA_CB_CONFIG,

    /** Writes CDMA SMS messages to the SIM card. */
    HREQ_SMS_ADD_CDMA_SIM_MESSAGE,

    /** Deletes CDMA SMS messages from the SIM card. */
    HREQ_SMS_DEL_CDMA_SIM_MESSAGE,

    /** Updates CDMA SMS messages from SIM card. */
    HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE,

    /** Set whether memory is available for new SMS messages. */
    HREQ_SMS_SET_MEMORY_STATUS,

    HREQ_SIM_BASE = 200,

    /** Gets the SIM card status. */
    HREQ_SIM_GET_SIM_STATUS,

    /**
     * Gets the international mobile subscriber identity (IMSI) of the SIM
     * card.
     */
    HREQ_SIM_GET_IMSI,

    /** Gets SIM card data. */
    HREQ_SIM_GET_SIM_IO,

    /** Gets SIM card lock status. */
    HREQ_SIM_GET_SIM_LOCK_STATUS,

    /** Sets the SIM card lock. */
    HREQ_SIM_SET_SIM_LOCK,

    /** Changes the SIM card password. */
    HREQ_SIM_CHANGE_SIM_PASSWORD,

    /** Sets the PIN for unlocking. */
    HREQ_SIM_UNLOCK_PIN,

    /** Sets the PUK for unlocking. */
    HREQ_SIM_UNLOCK_PUK,

    /** Gets SIM PIN input times from sim card. */
    HREQ_SIM_GET_SIM_PIN_INPUT_TIMES,

    /** Sets the PIN2 for unlocking. */
    HREQ_SIM_UNLOCK_PIN2,

    /** Sets the PUK2 for unlocking. */
    HREQ_SIM_UNLOCK_PUK2,

    /** Gets SIM PIN2 input times from sim card. */
    HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES,

    /** Specifies whether to activate a SIM card. */
    HREQ_SIM_SET_ACTIVE_SIM,

    /** Gets the protocol stack of the primary and secondary SIM cards. */
    HREQ_SIM_GET_RADIO_PROTOCOL,

    /** Sets the protocol stack for the primary and secondary SIM cards. */
    HREQ_SIM_SET_RADIO_PROTOCOL,

    /**
     * Sends the TerminalResponse instruction of the SIM application toolkit
     * (STK).
     */
    HREQ_SIM_STK_SEND_TERMINAL_RESPONSE,

    /** Sends the Envelope instruction of the STK. */
    HREQ_SIM_STK_SEND_ENVELOPE,

    /** Sned envelope response contain status of STK. */
    HREQ_SIM_STK_SEND_ENVELOPE_RESPONSE_CONTAIN_STATUS,

    /** Sends the CallSetup instruction of the STK. */
    HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT,

    /** Checks whether the STK is in the Ready state. */
    HREQ_SIM_STK_IS_READY,

    /**
     * Opens the logical channel of the application protocol data unit (APDU).
     */
    HREQ_SIM_OPEN_LOGICAL_CHANNEL,

    /** Closes the logical channel of the APDU. */
    HREQ_SIM_CLOSE_LOGICAL_CHANNEL,

    /**
     * Transmits APDU data over the logical channel. The logical channel is
     * opened and closed by the application.
     */
    HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL,

    /**
     * Transmits APDU data over the basic channel. The basic channel is the
     * channel opened by default.
     */
    HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL,

    /** Performs SIM card authentication. */
    HREQ_SIM_AUTHENTICATION,

    /** Unlocks a SIM card. */
    HREQ_SIM_UNLOCK_SIM_LOCK,

    /** Send SIM matched OperatorInfo. */
    HREQ_SIM_SEND_NCFG_OPER_INFO,

    /** Set SIM card power state. */
    HREQ_SIM_SET_SIM_POWER_STATE,

    HREQ_DATA_BASE = 300,

    /** Sets an apn to initial attach network. */
    HREQ_DATA_SET_INIT_APN_INFO,

    /** Deactivates PDP context. */
    HREQ_DATA_DEACTIVATE_PDP_CONTEXT,

    /** Activates PDP context. */
    HREQ_DATA_ACTIVATE_PDP_CONTEXT,

    /** Gets PDP context list. */
    HREQ_DATA_GET_PDP_CONTEXT_LIST,

    /** Gets current link information. */
    HREQ_DATA_GET_LINK_BANDWIDTH_INFO,

    /** Sets the rule for reporting the data link bandwidth information. */
    HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE,

    /** Sets the PDP context information for the data service. */
    HREQ_DATA_SET_DATA_PROFILE_INFO,

    /** Sends the data service performance mode. */
    HREQ_DATA_SEND_DATA_PERFORMANCE_MODE,

    /** Sends the data service sleep mode. */
    HREQ_DATA_SEND_DATA_SLEEP_MODE,

    /** Enables the data service of a SIM card slot. */
    HREQ_DATA_SET_DATA_PERMITTED,

    /** Get link capability. */
    HREQ_DATA_GET_LINK_CAPABILITY,

    /** Clean all data connections. */
    HREQ_DATA_CLEAN_ALL_CONNECTIONS,

    HREQ_NETWORK_BASE = 400,

    /** Gets the signal strength. */
    HREQ_NETWORK_GET_SIGNAL_STRENGTH,

    /** Gets the registration status of the CS domain. */
    HREQ_NETWORK_GET_CS_REG_STATUS,

    /** Gets the registration status of the PS domain. */
    HREQ_NETWORK_GET_PS_REG_STATUS,

    /** Gets the opreator info. */
    HREQ_NETWORK_GET_OPERATOR_INFO,

    /** Gets information about available networks. */
    HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION,

    /** Gets the network selection mode. */
    HREQ_NETWORK_GET_NETWORK_SELECTION_MODE,

    /** Sets the network selection mode. */
    HREQ_NETWORK_SET_NETWORK_SELECTION_MODE,

    /** Gets information about neighboring cells. */
    HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST,

    /** Gets current cell info. */
    HREQ_NETWORK_GET_CURRENT_CELL_INFO,

    /** Sets the preferred network type. */
    HREQ_NETWORK_SET_PREFERRED_NETWORK,

    /** Gets the preferred network type. */
    HREQ_NETWORK_GET_PREFERRED_NETWORK,

    /** Get radio capability. */
    HREQ_NETWORK_GET_RADIO_CAPABILITY,

    /** Gets the physical channel configuration. */
    HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG,

    /** Sets the cell location update notification mode. */
    HREQ_NETWORK_SET_LOCATE_UPDATES,

    /** Sets the filter of notifications reported by the modem. */
    HREQ_NETWORK_SET_NOTIFICATION_FILTER,

    /** Sets the device status. */
    HREQ_NETWORK_SET_DEVICE_STATE,

    /** Get rrc connection state. */
    HREQ_NETWORK_GET_RRC_CONNECTION_STATE,

    /** Set nr option mode. */
    HREQ_NETWORK_SET_NR_OPTION_MODE,

    /** Get nr option mode. */
    HREQ_NETWORK_GET_NR_OPTION_MODE,

    /** Get nr ssbid info. */
    HREQ_NETWORK_GET_NR_SSBID_INFO,

    HREQ_COMMON_BASE = 500,

    /** Sends a mobile phone power-off message to the modem. */
    HREQ_MODEM_SHUT_DOWN,

    /** Sets the modem status. */
    HREQ_MODEM_SET_RADIO_STATUS,

    /** Gets the modem status. */
    HREQ_MODEM_GET_RADIO_STATUS,

    /** Gets the international mobile equipment identity (IMEI). */
    HREQ_MODEM_GET_IMEI,

    /** Gets the international mobile equipment identity (IMEISV). */
    HREQ_MODEM_GET_IMEISV,

    /** Gets the mobile equipment identifier (MEID). */
    HREQ_MODEM_GET_MEID,

    /** Gets the baseband version. */
    HREQ_MODEM_GET_BASEBAND_VERSION,

    /** Gets the radio access technology of the CS domain. */
    HREQ_MODEM_GET_VOICE_RADIO,

    HREQ_MODEM_EXIT = 1000
} HRilRequest;
#endif // OHOS_RIL_REQUEST_H
