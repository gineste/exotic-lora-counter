/*
 *    ____  _  _   __   ____   __    ___    ____  _  _  ____  ____  ____  _  _  ____
 *   (  __)( \/ ) /  \ (_  _) (  )  / __)  / ___)( \/ )/ ___)(_  _)(  __)( \/ )/ ___)
 *    ) _)  )  ( (  O )  )(    )(  ( (__   \___ \ )  / \___ \  )(   ) _) / \/ \\___ \     
 *   (____)(_/\_) \__/  (__)  (__)  \___)  (____/(__/  (____/ (__) (____)\_)(_/(____/
 *
 * Copyright (c) 2017 EXOTIC SYSTEMS. All Rights Reserved.
 *
 * Licensees are granted free, non-transferable use of the information. NO WARRANTY 
 * of ANY KIND is provided. This heading must NOT be removed from the file.
 *
 * Date:          Oct 14, 2020
 * Author:        Martin Cornu.
 * Description:   State machine.
 *
 */

/****************************************************************************************
 * Include Files
 ****************************************************************************************/
#include <it_sdk/itsdk.h>
#include <it_sdk/time/time.h>
#include <it_sdk/logger/logger.h>
#include <it_sdk/logger/error.h>
#include <it_sdk/statemachine/statemachine.h>
#include <it_sdk/lorawan/lorawan.h>
#include <it_sdk/encrypt/encrypt.h>
#include <it_sdk/eeprom/eeprom.h>
#include <it_sdk/eeprom/sdk_config.h>
#include <it_sdk/lorawan/cayenne.h>
#include <it_sdk/configError.h>

#include "vichydro/statem/machine.h"

/****************************************************************************************
 * Defines
 ****************************************************************************************/

/****************************************************************************************
 * Private type declarations
 ****************************************************************************************/

/****************************************************************************************
 * Private function declarations
 ****************************************************************************************/

/****************************************************************************************
 * Variable declarations
 ****************************************************************************************/
machine_t vichydro_stm = {
	VICHYDRO_ST_SETUP,			// currentState
    LOOP_INIT_VALUE, 			// loopCounter
    STATE_UNKNOWN,				// lastState
    TOTAL_LOOP_INIT_VALUE,		// totalLoop
    &vichydro_stm_updateTiming,	// Update age each time the machine is running - on each cycles.
    {
      //       UID                RESET       PROCE      			 P0      		Name:XXXXXXX
        { VICHYDRO_ST_SETUP, 	  NULL,	    &vichydro_stm_stSetup,    NULL,   			"Setup"    },
		  { VICHYDRO_ST_WAIT4CONF,   NULL,     &vichydro_stm_stWaitC,    NULL,   			"Wait"     },
        { VICHYDRO_ST_RUN, 	 	  NULL,     &vichydro_stm_stRun,   	 NULL,   			"Run"      },
        { VICHYDRO_ST_SEND, 	 	  NULL,     &vichydro_stm_stSend,   	 FRAME_SENSOR,	   	"SendS"    },
        { VICHYDRO_ST_SENDBOOT, 	  NULL,     &vichydro_stm_stSend,   	 FRAME_BOOT,   		"SendBOOT" },
        { VICHYDRO_ST_JOIN, 	 	  NULL,     &vichydro_stm_stJoin,   	 NULL,   			"Join"     },
        { STATE_LAST,	 		  NULL,     NULL,  					 NULL,      		"Error"    }

    }
};

/****************************************************************************************
 * Public functions
 ****************************************************************************************/ 
/**
 * On every machine cycle, call the timing update;
 * The precision is 1S so the scheduler period should not be lower than 1s
 */
void vichydro_stm_updateTiming() {
	uint64_t now = itsdk_time_get_ms();
	uint32_t durationS =(uint32_t)((now - vichydro_state.lastTimeUpdateMs)/1000);
	vichydro_state.lastTimeUpdateMs += (durationS*1000);
	vichydro_state.lastAckTestS += durationS;
	vichydro_state.lastConnectTryS += durationS;
	vichydro_state.lastMeasureS += durationS;
}

/** ***************************************************************************************
 * Setup step, here we are configuring the hardware and software stuff
 */
uint16_t vichydro_stm_stSetup(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop) {
	log_debug("[%d] SETUP\r\n", (uint32_t) itsdk_time_get_ms());
	static itsdk_lorawan_channelInit_t channels= ITSDK_LORAWAN_CHANNEL;
	#ifdef ITSDK_LORAWAN_CHANNEL
		itsdk_lorawan_setup(__LORAWAN_REGION_EU868,&channels);
	#else
		itsdk_lorawan_setup(__LORAWAN_REGION_EU868,NULL);
	#endif

	// Setup the state
	vichydro_setup();

	// Check is the device has been initialized of if still factory default (should be 00,00,00...)
	uint8_t devEui[8];
	itsdk_lorawan_getDeviceEUI(devEui);
	uint8_t i = 0;
	while ( i < 8 && devEui[i] == 0 ) i++;
	if  ( i == 8 ) {
		// The device is not yet initialized
		log_info("Lorawan EUI not init\r\n");
		return VICHYDRO_ST_WAIT4CONF;
	} else {
		// Ready to go
		return (VICHYDRO_ST_JOIN | STATE_IMMEDIATE_JUMP);
	}

}


/** ***************************************************************************************
 * Until the configuration of the deviceId is correct we stay in the stWait
 */
uint16_t vichydro_stm_stWaitC(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop) {
	log_debug("[%d] WAIT CONF\r\n", (uint32_t) itsdk_time_get_ms());
	// Assuming the more convinient way is to reset after configuration
	return VICHYDRO_ST_WAIT4CONF;
}



/** ***************************************************************************************
 * Central decision loop / capture data and manage transmission
 */
uint16_t vichydro_stm_stRun(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop) {
	log_debug("[%d] RUN\r\n", (uint32_t) itsdk_time_get_ms());

	if ( vichydro_state.connection != VICHYDRO_CONNEXION_JOINED ) {
  	   // open for discussion, it is immediate or not ?
	   return VICHYDRO_ST_JOIN;
	}

	// Send the reboot frame
	if ( vichydro_state.bootFrameSent == 0 && vichydro_state.lastTimeUpdateMs > (1000*VICHYDRO_CONFIG_BOOTFRM_S)) {
		vichydro_state.bootFrameSent = 1;
		return ( VICHYDRO_ST_SENDBOOT | STATE_IMMEDIATE_JUMP);
	}


	// Get values when sendDuty time has been reached
	if ( itsdk_config.app.sendDuty > 0 && vichydro_state.lastMeasureS >= itsdk_config.app.sendDuty*VICHYDRO_CONFIG_TIME_BASE_S ) {
		log_info("T%d\r\n",(uint32_t)itsdk_time_get_ms());

		vichydro_state.lastMeasureS = 0;

		if ( vichydro_state.connection == VICHYDRO_CONNEXION_JOINED )
		   return ( VICHYDRO_ST_SEND | STATE_IMMEDIATE_JUMP);
	}

	return VICHYDRO_ST_RUN;
}


/** ***************************************************************************************
 *  LoRaWan Communication
 */

/**
 * Data transmission
 */
uint16_t vichydro_stm_stSend(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop) {
	log_debug("[%d] SEND\r\n", (uint32_t) itsdk_time_get_ms());
	uint16_t next = VICHYDRO_ST_RUN;
	uint8_t frBuffer[128];
	int index = 0;
	uint8_t sPort=1;
	itsdk_cayenne_data_u cayenne;

	itsdk_lorawan_sendconf_t ack = LORAWAN_SEND_UNCONFIRMED;
	if ( vichydro_state.lastAckTestS > itsdk_config.app.ackDuty*itsdk_config.app.sendDuty*VICHYDRO_CONFIG_TIME_BASE_S) {
		vichydro_state.lastAckTestS = 0;
		ack = LORAWAN_SEND_CONFIRMED;
	}

	if ( p == FRAME_SENSOR ) {
		log_debug("Building frame sensor\r\n");
		sPort = 1;
	} else if ( p == FRAME_BOOT ) {
		log_debug("Building frame boot\r\n");
		// +------+------+------+----------+-----------+
		// | 0x01 | Reset Cause | SendDuty | SleepDuty |
		// +------+------+------+----------+-----------+
		// Frame boot frame, not cayenne style
		sPort = 5;	// admin frame
		ack = LORAWAN_SEND_CONFIRMED;	// override
		frBuffer[0] = 0x01;				// header
		frBuffer[1] = (vichydro_state.lastResetCause & 0xFF00) >> 8;
		frBuffer[2] = (vichydro_state.lastResetCause & 0xFF);
		frBuffer[3] = (itsdk_config.app.sendDuty);
		frBuffer[4] = (uint8_t)(itsdk_config.app.sleepDuty);
		index=5;
	}

	uint8_t rPort,rSize=32;
	uint8_t rData[32];
	itsdk_lorawan_send_t r = itsdk_lorawan_send_sync(
			frBuffer,
			index,
			sPort,
			__LORAWAN_DR_5,
			ack,
			itsdk_config.sdk.lorawan.retries,
			&rPort,
			&rSize,
			rData,
			/*PAYLOAD_ENCRYPT_AESCTR | PAYLOAD_ENCRYPT_SPECK |*/ PAYLOAD_ENCRYPT_NONE
	);
	switch ( r ) {
		case LORAWAN_SEND_ACKED_WITH_DOWNLINK:
			// case with downlink data
			//proceed_downlink(rPort,rSize,rData);		// missing break is normal
		case LORAWAN_SEND_ACKED_WITH_DOWNLINK_PENDING:
			// case with downlink data + pending downlink on server side.
			vichydro_state.ackFailed = 0;
			break;
		case LORAWAN_SEND_ACKED:
			vichydro_state.ackFailed = 0;
			break;
		case LORAWAN_SEND_SENT:
			if ( ack == LORAWAN_SEND_CONFIRMED ) {
				vichydro_state.ackFailed++;
			}
			if ( vichydro_state.ackFailed > VICHYDRO_CONFIG_ACKRETRY ) {
				vichydro_state.connection = VICHYDRO_CONNEXION_DISCONNECTED;
				ITSDK_ERROR_REPORT(APP_ERROR_LORA_DISCONNECT,0);
			}
			break;
		case LORAWAN_SEND_DUTYCYCLE:
			// retry
			next = VICHYDRO_ST_SEND;
			break;
		case LORAWAN_SEND_NOT_JOINED:
			vichydro_state.connection = VICHYDRO_CONNEXION_DISCONNECTED;
			ITSDK_ERROR_REPORT(APP_ERROR_LORA_DISCONNECT,0);
			break;

		default:
			break;
	}
	return next;
}

/** ***************************************************************************************
 *  LoRaWan Join procedure
 */

uint16_t vichydro_stm_stJoin(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop) {
	log_debug("[%d] JOIN\r\n", (uint32_t) itsdk_time_get_ms());
	uint16_t next = VICHYDRO_ST_JOIN;
	switch ( vichydro_state.connection ) {
	case VICHYDRO_CONNEXION_INIT:
		// first connection, no wait, loop until connection success
		if ( !itsdk_lorawan_hasjoined() ) {
			if ( itsdk_lorawan_join_sync() == LORAWAN_JOIN_SUCCESS ) {
				vichydro_state.connection = VICHYDRO_CONNEXION_JOINED;
				vichydro_state.connectionFailed = 0;
				next = VICHYDRO_ST_RUN;
			} else {
				log_debug("Try to join Lorawan network %d/20\r\n", vichydro_state.connectionFailed);
				vichydro_state.connectionFailed++;
				if ( vichydro_state.connectionFailed > 20 ) {
					log_debug("Fail to join lorawan network\r\n");
					// slow down the retry
					vichydro_state.connection = VICHYDRO_CONNEXION_DISCONNECTED;
				}
			}
		}
		break;
	case VICHYDRO_CONNEXION_DISCONNECTED:
		// reconnection try on every
		if (vichydro_state.lastConnectTryS > itsdk_config.app.sleepDuty*itsdk_config.app.sendDuty*VICHYDRO_CONFIG_TIME_BASE_S ) {
			log_debug("Try to reconnect lorawan network...\r\n");
			vichydro_state.lastConnectTryS = 0;
			if ( itsdk_lorawan_join_sync() == LORAWAN_JOIN_SUCCESS ) {
				vichydro_state.connection = VICHYDRO_CONNEXION_JOINED;
				vichydro_state.connectionFailed=0;
			} else {
				vichydro_state.connectionFailed++;
			}
			next = VICHYDRO_ST_RUN;
		}
		break;
	default:
	case VICHYDRO_CONNEXION_JOINED:
		// we should not be here
		next = VICHYDRO_ST_RUN;
	}
	return next;
}

/****************************************************************************************
 * Private functions
 ****************************************************************************************/

/****************************************************************************************
 * End Of File
 ****************************************************************************************/

