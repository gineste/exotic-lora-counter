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

#include "vichydro/config/GlobalDefs.h"
#include "vichydro/libraries/timeout.h"

#include "vichydro/drivers/bq35100.h"

/****************************************************************************************
 * Defines
 ****************************************************************************************/
/* communication commands */
#define BQ35100_NOT_SHIFTED_ADDR             	0x55
#define BQ35100_WRITE_ADDR                  	0xAA
#define BQ35100_READ_ADDR                   	0xAB

/* commands */
#define	BQ35100_MAC								0x3E	/* Manufacturer access control (MAC) for sub commands */
#define	BQ35100_MAC_DATA						0x40	/* Read data from MAC */

/* control sub commands */
#define BQ35100_DEVICE_TYPE_REG					0x0001
#define BQ35100_HW_VERSION_REG					0x0003

/* values */
#define	BQ35100_DEVICE_TYPE						0x100

/****************************************************************************************
 * Private type declarations
 ****************************************************************************************/

/****************************************************************************************
 * Private function declarations
 ****************************************************************************************/
static e_BQ35100_ErrorCode_t eWriteRegister(uint16_t p_u16RegAdr, uint16_t p_u16Value);
static e_BQ35100_ErrorCode_t eReadRegister(uint16_t p_u16RegAdr, uint16_t * p_pu16Value);

/****************************************************************************************
 * Variable declarations
 ****************************************************************************************/ 
static uint8_t g_u8Initialized = 0u;

/****************************************************************************************
 * Public functions
 ****************************************************************************************/ 
e_BQ35100_ErrorCode_t eBQ35100_Init(void)
{
	e_BQ35100_ErrorCode_t l_eCode = BQ35100_ERROR_NONE;
	uint16_t l_u16DeviceType = 0x0000;

	if(0u == g_u8Initialized)
	{
		HAL_GPIO_WritePin(BQ_GE_GPIO_Port, BQ_GE_Pin, GPIO_PIN_SET);	/* gauge enable */
		vTime_WaitMs(400u);												/* wait device to power up */
		eBQ35100_DeviceTypeGet(&l_u16DeviceType);						/* check device type */

		if(BQ35100_DEVICE_TYPE != l_u16DeviceType)
		{
			l_eCode = BQ35100_ERROR_INIT;
		}
		else
		{
			 l_eCode = BQ35100_ERROR_NONE;
		}
	}

	return l_eCode;
}

e_BQ35100_ErrorCode_t eBQ35100_VoltageGet(uint16_t * p_pu16mV)
{
	e_BQ35100_ErrorCode_t l_eCode = BQ35100_ERROR_NONE;
	uint16_t l_u16Data = 0u;

	if(NULL != p_pu16mV)
	{
		l_eCode = eReadRegister(0x08, &l_u16Data);

		if(BQ35100_ERROR_NONE == l_eCode)
		{
			*p_pu16mV = l_u16Data;
		}
	}

	return l_eCode;
}

e_BQ35100_ErrorCode_t eBQ35100_DeviceTypeGet(uint16_t * p_pu16DeviceType)
{
	e_BQ35100_ErrorCode_t l_eCode = BQ35100_ERROR_NONE;
	uint16_t l_u16Data = 0u;

	if(NULL != p_pu16DeviceType)
	{
		l_eCode = eWriteRegister(BQ35100_MAC, BQ35100_DEVICE_TYPE_REG);

		if(BQ35100_ERROR_NONE == l_eCode)
		{
			l_eCode = eReadRegister(BQ35100_MAC_DATA, &l_u16Data);

			if(BQ35100_ERROR_NONE == l_eCode)
			{
				*p_pu16DeviceType = l_u16Data;
			}
		}
	}

	return l_eCode;
}

/****************************************************************************************
 * Private functions
 ****************************************************************************************/
static e_BQ35100_ErrorCode_t eWriteRegister(uint16_t p_u16RegAdr, uint16_t p_u16Value)
{
	e_BQ35100_ErrorCode_t l_eCode = BQ35100_ERROR_NONE;
	_I2C_Status l_eI2cCode = I2C_OK;
	uint8_t l_au8Bytes[2u];

	l_au8Bytes[0] = p_u16Value & 0xFF;
	l_au8Bytes[1] = p_u16Value >> 8;

	l_eI2cCode = i2c_memWrite(&hi2c1, BQ35100_NOT_SHIFTED_ADDR, p_u16RegAdr, 8u, l_au8Bytes, 2u);
	vTime_WaitMs(5u);

	if(I2C_OK != l_eI2cCode)
	{
		l_eCode = BQ35100_ERROR_COMM;
	}
	else
	{
		l_eCode = BQ35100_ERROR_NONE;
	}

	return l_eCode;
}

static e_BQ35100_ErrorCode_t eReadRegister(uint16_t p_u16RegAdr, uint16_t * p_pu16Value)
{
	e_BQ35100_ErrorCode_t l_eCode = BQ35100_ERROR_NONE;
	_I2C_Status l_eI2cCode = I2C_OK;
	uint8_t l_au8Bytes[2u];

	l_eI2cCode = i2c_memRead(&hi2c1, BQ35100_NOT_SHIFTED_ADDR, p_u16RegAdr, 8u, l_au8Bytes, 2u);
	vTime_WaitMs(5u);

	if(I2C_OK != l_eI2cCode)
	{
		l_eCode = BQ35100_ERROR_COMM;
	}
	else
	{
		*p_pu16Value = l_au8Bytes[0u] + (uint16_t)l_au8Bytes[1u]*256u;
		l_eCode = BQ35100_ERROR_NONE;
	}

	return l_eCode;
}

/****************************************************************************************
 * End Of File
 ****************************************************************************************/

