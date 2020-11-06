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
#ifndef BQ35100_H_
#define BQ35100_H_

/****************************************************************************************
 * Include Files
 ****************************************************************************************/
#include <stdint.h>

/****************************************************************************************
 * Defines
 ****************************************************************************************/

/****************************************************************************************
 * Type definitions
 ****************************************************************************************/
typedef enum _BQ35100_ERROR_ {
   BQ35100_ERROR_NONE,
   BQ35100_ERROR_NOT_FOUND,
   BQ35100_ERROR_COMM,
   BQ35100_ERROR_BUSY,
   BQ35100_ERROR_PARAM,
   BQ35100_ERROR_INIT
}e_BQ35100_ErrorCode_t;

/****************************************************************************************
 * Public function declarations
 ****************************************************************************************/
e_BQ35100_ErrorCode_t eBQ35100_Init(void);
e_BQ35100_ErrorCode_t eBQ35100_VoltageGet(uint16_t * p_pu16mV);
e_BQ35100_ErrorCode_t eBQ35100_DeviceTypeGet(uint16_t * p_pu16DeviceType);

#endif /* BQ35100_H_ */
