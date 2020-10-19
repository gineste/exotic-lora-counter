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
 */
#ifndef MACHINE_H_
#define MACHINE_H_

/****************************************************************************************
 * Include Files
 ****************************************************************************************/
#include <it_sdk/itsdk.h>
#include <it_sdk/statemachine/statemachine.h>

/****************************************************************************************
 * Defines
 ****************************************************************************************/
#define VICHYDRO_ST_SETUP 			0
#define VICHYDRO_ST_WAIT4CONF 	1
#define VICHYDRO_ST_RUN 			2
#define VICHYDRO_ST_SEND			3
#define VICHYDRO_ST_SENDBOOT		4
#define VICHYDRO_ST_JOIN			5

#define FRAME_SENSOR			((void *)0)
#define FRAME_BOOT			((void *)1)

/****************************************************************************************
 * Type definitions
 ****************************************************************************************/
typedef enum __attribute__ ((__packed__)) {
	VICHYDRO_CONNEXION_INIT = 0,
	VICHYDRO_CONNEXION_JOINED = 1,
	VICHYDRO_CONNEXION_DISCONNECTED = 2,
} vichydro_connection_state_e;

typedef struct {
	uint64_t	lastTimeUpdateMs;			// in ms => last time we updated the timings
	uint32_t	lastMeasureS;				// time in S since last measure
	uint32_t	lastAckTestS;				// time in S since last test
	uint32_t	lastConnectTryS;			// time in S since last connection try
	uint8_t		ackFailed;					// Number of ack tried failed
	uint16_t	lastResetCause;				// Last reset reason
	uint8_t		bootFrameSent:1;			// The reboot message has been sent

	uint16_t	nbPress;			/* Number of press since the last reset */
	uint32_t	nbPressTot;		/* Number of press since the beginning of the device life */

	vichydro_connection_state_e		connection;		// State of the LoRaWan connection
	uint16_t	connectionFailed;			// Failed consecutive connection counter

} vichydro_state_t;

/****************************************************************************************
 * Public variable declarations
 ****************************************************************************************/
extern vichydro_state_t vichydro_state;
extern machine_t vichydro_stm;

/****************************************************************************************
 * Public function declarations
 ****************************************************************************************/
void vichydro_stm_updateTiming();
uint16_t vichydro_stm_stSetup(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop);
uint16_t vichydro_stm_stRun(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop);
uint16_t vichydro_stm_stWaitC(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop);
uint16_t vichydro_stm_stSend(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop);
uint16_t vichydro_stm_stJoin(void * p, uint8_t cState, uint16_t cLoop, uint32_t tLoop);

void vichydro_setup();
void vITflagsProcess(void);

#endif /* MACHINE_H_ */
