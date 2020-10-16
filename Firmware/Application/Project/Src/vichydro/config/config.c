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
#include <it_sdk/logger/logger.h>
#include <it_sdk/time/time.h>
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
static void __userBtn_interrupt(uint16_t GPIO_Pin);
static void __ilsBtn_interrupt(uint16_t GPIO_Pin);

/****************************************************************************************
 * Variable declarations
 ****************************************************************************************/ 
// State
vichydro_state_t vichydro_state;

static gpio_irq_chain_t __userBtn_gpio_irq = {
		__userBtn_interrupt,
		USER_BTN_Pin,
		NULL,
};

static gpio_irq_chain_t __ilsBtn_gpio_irq = {
		__ilsBtn_interrupt,
		ILS_Pin,
		NULL,
};

static uint8_t g_u8UserBtnITFlag = 0u;
static uint8_t g_u8IlsSensorITFlag = 0u;

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

	/* Config user button interrupt */
	if (USER_BTN_Pin != __LP_GPIO_NONE ) {
		gpio_configure(__BANK_B,USER_BTN_Pin,GPIO_INTERRUPT_FALLING);
		gpio_registerIrqAction(&__userBtn_gpio_irq);
	}

	/* Config ils sensor interrupt */
	if (ILS_Pin != __LP_GPIO_NONE ) {
		gpio_configure(__BANK_A,ILS_Pin,GPIO_INTERRUPT_FALLING);
		gpio_registerIrqAction(&__ilsBtn_gpio_irq);
	}
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

void vITflagsProcess(void)
{
    if(g_u8IlsSensorITFlag == 1u)
    {
        log_debug("Blink led\r\n");
        g_u8IlsSensorITFlag = 0u;
       HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
       while (itsdk_time_get_ms() % 400 == 0);
       HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
    }

    if(g_u8UserBtnITFlag == 1u)
    {
        log_debug("Blink led\r\n");
        g_u8UserBtnITFlag = 0u;
       HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
       while (itsdk_time_get_ms() % 400 == 0);
       HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
    }
}

/****************************************************************************************
 * Private functions
 ****************************************************************************************/
/* @brief	User button interrupt handler.
 * 			Button use for reset counter of press.
 * @param 	GPIO_Pin
 * @return 	none
 */
static void __userBtn_interrupt(uint16_t GPIO_Pin) {
	log_debug("user btn Int\r\n");
   g_u8UserBtnITFlag = 1u;
}

/* @brief	Ils sensor interrupt handler.
 * 			Sensor use to detect a press.
 * @param 	GPIO_Pin
 * @return 	none
 */
static void __ilsBtn_interrupt(uint16_t GPIO_Pin) {
	log_debug("ils Int\r\n");
   g_u8IlsSensorITFlag = 1u;
}

/****************************************************************************************
 * End Of File
 ****************************************************************************************/

