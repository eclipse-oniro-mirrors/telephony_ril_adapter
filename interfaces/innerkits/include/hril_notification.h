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

#ifndef OHOS_RIL_NOTIFICATION_H
#define OHOS_RIL_NOTIFICATION_H

typedef enum {
    HNOTI_CALL_BASE = 1000,

    /** Called when call state is updated. */
    HNOTI_CALL_STATE_UPDATED,

    /** Called when a new USSD message is received. */
    HNOTI_CALL_USSD_REPORT,

    /**
     * Called when nework doesn't have in-band information, need to play
     * out-band tone.
     */
    HNOTI_CALL_RINGBACK_VOICE_REPORT,

    /**
     * Called when Single Radio Voice Call Continuity(SRVCC) progress state has
     * changed.
     */
    HNOTI_CALL_SRVCC_STATUS_REPORT,

    /** Called when emergency number list is updated. */
    HNOTI_CALL_EMERGENCY_NUMBER_REPORT,

    /**
     * Called when SS response is received when DIAL/USSD/SS is changed to SS
     * by call control.
     */
    HNOTI_CALL_SS_REPORT,

    /** Called when RSRVCC status is updated. */
    HNOTI_CALL_RSRVCC_STATUS_REPORT,

    /** Called when supplementary service related notification is received from the network. */
    HNOTI_CALL_SUPP_SVC_NOTIFY,

    HNOTI_SMS_BASE = 1100,

    /** Called when new SMS is received. */
    HNOTI_SMS_NEW_SMS,

    /** Called when new CDMA SMS is received. */
    HNOTI_SMS_NEW_CDMA_SMS,

    /** Called when SMS status is received. */
    HNOTI_SMS_STATUS_REPORT,

    /** Called when SMS has been stored on SIM card. */
    HNOTI_SMS_NEW_SMS_STORED_ON_SIM,

    /** Called when new cell broadcast is received. */
    HNOTI_CB_CONFIG_REPORT,

    /** Called when sms storage on SIM card is full. */
    HNOTI_SMS_SIM_SMS_MEMORY_FULL,

    HNOTI_SIM_BASE = 1200,

    /** Called when sim status is changed. */
    HNOTI_SIM_STATUS_CHANGED,

    /** Called when STK session is terminated by SIM. */
    HNOTI_SIM_STK_SESSION_END_NOTIFY,

    /** Called when SIM issue a STK proactive command to applications. */
    HNOTI_SIM_STK_PROACTIVE_NOTIFY,

    /** Called when there is an ALPHA from UICC during Call Control. */
    HNOTI_SIM_STK_ALPHA_NOTIFY,

    /** Called when SIM notifies applcations some event happens. */
    HNOTI_SIM_STK_EVENT_NOTIFY,

    /** Called when SIM wants application to setup a voice call. */
    HNOTI_SIM_STK_CALL_SETUP_NOTIFY,

    /**
     * Called when that file(s) in the SIM have been updated, or the SIM has
     * been reinitialized.
     */
    HNOTI_SIM_REFRESH_NOTIFY,

    /** Called when radio protocol is updated. */
    HNOTI_SIM_RADIO_PROTOCOL_UPDATED,

    HNOTI_DATA_BASE = 1300,

    /** Called when pdp context list is updated. */
    HNOTI_DATA_PDP_CONTEXT_LIST_UPDATED,

    /** Reporting the data link capability.*/
    HNOTI_DATA_LINK_CAPABILITY_UPDATED,

    HNOTI_NETWORK_BASE = 1400,

    /** Called when CS registered status is updated. */
    HNOTI_NETWORK_CS_REG_STATUS_UPDATED,

    /** Called when signal strength is updated. */
    HNOTI_NETWORK_SIGNAL_STRENGTH_UPDATED,

    /** Called when radio has received a time message. */
    HNOTI_NETWORK_TIME_UPDATED,

    /** Called when time zone is updated. */
    HNOTI_NETWORK_TIME_ZONE_UPDATED,

    /** Called when IMS registration state is updated. */
    HNOTI_NETWORK_IMS_REG_STATUS_UPDATED,

    /** Called when PS registration state is updated. */
    HNOTI_NETWORK_PS_REG_STATUS_UPDATED,

    /** Called when update physical channel configuration. */
    HNOTI_NETWORK_PHY_CHNL_CFG_UPDATED,

    /** Called when current cell is updated. */
    HNOTI_NETWORK_CURRENT_CELL_UPDATED,

    /** Called when update rrc connection state.*/
    HNOTI_NETWORK_RRC_CONNECTION_STATE_UPDATED,

    /** Called when resident network is updated.*/
    HNOTI_NETWORK_RESIDENT_NETWORK_UPDATED,

    /** Called when network restricted state is updated. */
    HNOTI_NETWORK_RESTRICTED_STATE_UPDATED,

    HNOTI_COMMON_BASE = 1500,

    /** Called when radio state is updated. */
    HNOTI_MODEM_RADIO_STATE_UPDATED,

    /** Called when voice technology is updated. */
    HNOTI_MODEM_VOICE_TECH_UPDATED,

    /** Called when DSDS mode is updated.*/
    HNOTI_MODEM_DSDS_MODE_UPDATED,

    HNOTI_COMMON_END = 1599,
} HRilNotification;
#endif // OHOS_RIL_NOTIFICATION_H
