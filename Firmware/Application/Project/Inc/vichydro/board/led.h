/*
 *    ____  _  _   __   ____   __    ___    ____  _  _  ____  ____  ____  _  _  ____
 *   (  __)( \/ ) /  \ (_  _) (  )  / __)  / ___)( \/ )/ ___)(_  _)(  __)( \/ )/ ___)
 *    ) _)  )  ( (  O )  )(    )(  ( (__   \___ \ )  / \___ \  )(   ) _) / \/ \\___ \     
 *   (____)(_/\_) \__/  (__)  (__)  \___)  (____/(__/  (____/ (__) (____)\_)(_/(____/
 *
 * Copyright (c) 2020 EXOTIC SYSTEMS. All Rights Reserved.
 *
 * Licensees are granted free, non-transferable use of the information. NO WARRANTY 
 * of ANY KIND is provided. This heading must NOT be removed from the file.
 *
 * led.h
 * Date:          9 juil. 2020
 * Author:        Aurélien Jacquot / Martin Cornu
 * Project:       super_tracker_poc
 * Description:   Led management.
 *
 */
#ifndef APPLICATION_BOARD_LED_H_
#define APPLICATION_BOARD_LED_H_

/****************************************************************************************
 * Include Files
 ****************************************************************************************/ 
#include "gpio.h"
#include "vichydro/config/boardConfig.h"

/****************************************************************************************
 * Defines
 ****************************************************************************************/ 
#if (USE_DEBUG == 0u)
#define LED_GREEN_Pin					GPIO_PIN_14		/* define here because PA14 is for SWCLK in CubeMX */
#define LED_GREEN_GPIO_Port			GPIOA
#endif

#if defined(LED_RED_Pin) && defined(LED_GREEN_Pin)
#define LED_RED_ON() 			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET)
#define LED_RED_OFF() 			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET)
#define LED_RED_TOG() 			HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin)

#define LED_GREEN_ON() 			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET)
#define LED_GREEN_OFF() 		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET)
#define LED_GREEN_TOG() 		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin)
#else
#define LED_RED_ON()
#define LED_RED_OFF()
#define LED_RED_TOG()

#define LED_GREEN_ON()
#define LED_GREEN_OFF()
#define LED_GREEN_TOG()
#endif

/****************************************************************************************
 * Type declarations
 ****************************************************************************************/

/****************************************************************************************
 * Variable declarations
 ****************************************************************************************/  

/****************************************************************************************
 * Public functions
 ****************************************************************************************/  

/****************************************************************************************
 * End Of File
 ****************************************************************************************/
#endif /* APPLICATION_BOARD_LED_H_ */
