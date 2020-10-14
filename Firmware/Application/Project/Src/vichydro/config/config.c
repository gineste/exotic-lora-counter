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
 * Description:   Configuration.
 *
 */

/****************************************************************************************
 * Include Files
 ****************************************************************************************/
#include <it_sdk/config.h>
#include <it_sdk/eeprom/sdk_config.h>
#include <vichydro/statem/machine.h>

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
// State
vichydro_state_t vichydro_state;

/****************************************************************************************
 * Public functions
 ****************************************************************************************/ 
/**
 * Setup the vichydro software & boards
 */
void vichydro_setup() {
	// Init State
	vichydro_state.lastTimeUpdateMs = 0;
	vichydro_state.lastAckTestS = 0;
	vichydro_state.lastConnectTryS = 0;
	vichydro_state.lastMeasureS = 0;
	vichydro_state.ackFailed = 0;
	vichydro_state.connection = VICHYDRO_CONNEXION_INIT;
	vichydro_state.connectionFailed = 0;
	vichydro_state.bootFrameSent = 0;
}

// OVERRIDE THE CONFIG FUNCTIONS

itsdk_config_ret_e itsdk_config_app_resetToFactory() {
	itsdk_config.app.version = ITSDK_USER_VERSION_BYTE;
	itsdk_config.app.sendDuty = VICHYDRO_CONFIG_SENDDUTY;
	itsdk_config.app.ackDuty = VICHYDRO_CONFIG_ACKDUTY;
	itsdk_config.app.ackRetry = VICHYDRO_CONFIG_ACKRETRY;
	itsdk_config.app.sleepDuty = VICHYDRO_CONFIG_SLEEPDUTY;
	return CONFIG_RESTORED_FROM_FACTORY;
}

/****************************************************************************************
 * Private functions
 ****************************************************************************************/

/****************************************************************************************
 * End Of File
 ****************************************************************************************/

