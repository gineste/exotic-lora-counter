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
 * Date:          20 oct. 2020
 * Author:        Martin Cornu.
 * Description:   File description.
 *
 */

/****************************************************************************************
 * Include Files
 ****************************************************************************************/
#include <i2c.h>
#include <it_sdk/wrappers.h>

#include "vichydro/drivers/bq35100.h"

/****************************************************************************************
 * Defines
 ****************************************************************************************/
/* communication commands */
#define BQ35100_NOT_SHIFTED_ADDR             0x55
#define BQ35100_WRITE_ADDR                  	0xAA
#define BQ35100_READ_ADDR                   	0xAB

/* control functions */
#define BQ35100_DEVICE_TYPE_REG					0x0001
#define BQ35100_HW_VERSION_REG					0x0003
/****************************************************************************************
 * Private type declarations
 ****************************************************************************************/

/****************************************************************************************
 * Private function declarations
 ****************************************************************************************/

/****************************************************************************************
 * Variable declarations
 ****************************************************************************************/ 

/****************************************************************************************
 * Public functions
 ****************************************************************************************/ 
e_BQ35100_ErrorCode_t eBQ35100_DeviceTypeGet(uint8_t * p_pu8DeviceType)
{
	e_BQ35100_ErrorCode_t l_eCode = BQ35100_ERROR_NONE;
	_I2C_Status l_eI2cCode = I2C_OK;
	uint16_t l_u16Data = 0u;

	if(NULL != p_pu8DeviceType)
	{
		l_eI2cCode = i2c_write16BRegister(&hi2c1,BQ35100_NOT_SHIFTED_ADDR, 0x3E, BQ35100_DEVICE_TYPE_REG, 1);
		if(I2C_OK == l_eI2cCode)
		{
			l_eI2cCode = i2c_read16BRegister(&hi2c1, BQ35100_NOT_SHIFTED_ADDR, 0x40, &l_u16Data, 1);

			if(I2C_OK == l_eI2cCode)
			{
				/**p_pu8DeviceType = l_u8Data;*/
			}
		}
	}

	return l_eCode;
}

/****************************************************************************************
 * Private functions
 ****************************************************************************************/

/****************************************************************************************
 * End Of File
 ****************************************************************************************/

