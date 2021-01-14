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
 * Date:          Oct 12, 2020
 * Author:        Martin Cornu.
 * Description:   Project main.
 *
 */

/****************************************************************************************
 * Include Files
 ****************************************************************************************/
/* it-sdk */
#include <it_sdk/itsdk.h>
#include <it_sdk/time/time.h>
#include <it_sdk/logger/logger.h>
#include <it_sdk/lorawan/lorawan.h>
#include <it_sdk/encrypt/encrypt.h>
#include <it_sdk/eeprom/securestore.h>
#include <it_sdk/lowpower/lowpower.h>
#include <drivers/sx1276/sx1276.h>
#include <it_sdk/configNvm.h>
#include <it_sdk/statemachine/statemachine.h>
#include <it_sdk/sched/scheduler.h>

#include "vichydro/config/boardConfig.h"
#include <vichydro/statem/machine.h>

/****************************************************************************************
 * Variable Declarations
 ****************************************************************************************/
extern machine_t vichydro_stm;

/****************************************************************************************
 * Public functions
 ****************************************************************************************/
/**
 * Process the state machine on regular basis
 */
void task() {
	statem(&vichydro_stm);
}

/**
 * On Reset setup
 */
void project_setup ()
{
	log_info ("Booting !!\r\n");
	vichydro_state.lastResetCause = itsdk_getResetCause();
	SX1276InitLowPower();
	itdt_sched_registerSched(VICHYDRO_CONFIG_TIME_BASE_S*1000,ITSDK_SCHED_CONF_IMMEDIATE, &task);

#if (USE_DEBUG == 1u)
	lowPower_disable();
#endif
}

/**
 * LowPower loop
 */
void project_loop ()
{
	itsdk_lorawan_loop();
   vITflagsProcess();
}

