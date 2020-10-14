/* ==========================================================
 * configNvm.h - Application specific configuration file
 * Project : Disk91 SDK
 * ----------------------------------------------------------
 * Created on: 21 f�vr. 2019
 *     Author: Paul Pinault aka Disk91
 * ----------------------------------------------------------
 * Copyright (C) 2019 Disk91
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LESSER General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * ----------------------------------------------------------
 * 
 *
 * ==========================================================
 */

#ifndef INC_IT_SDK_CONFIGNVM_H_
#define INC_IT_SDK_CONFIGNVM_H_

// The configuration version
#define ITSDK_CONFIGURATION_APP_VERSION		ITSDK_USER_VERSION_BYTE


#define VICHYDRO_CONFIG_TIME_BASE_S 			10		// Base of time in Second, use as unit for 	VICHYDRO_CONFIG_SENDDUTY and others

#define VICHYDRO_CONFIG_SENDDUTY		3		// By default send new data every 3xVICHYDRO_CONFIG_TIME_BASE_Ss
#define VICHYDRO_CONFIG_ACKDUTY	   10		// Every 10x*(VICHYDRO_CONFIG_ACKDUTY) => 5m ack is performed
#define VICHYDRO_CONFIG_ACKRETRY		6		// 6 ACK retry ( 30m ) before failing (this is not LoRaWan retry)
#define VICHYDRO_CONFIG_SLEEPDUTY   20		// 20*VICHYDRO_CONFIG_SENDDUTY (10 min) time before trying to reconnect
#define VICHYDRO_CONFIG_BOOTFRM_S  (5*60)	// Boot frame after 5 minutes
/**
 * The application configuration specific fields are stored in this structure
 * The structure size must be 32b aligned.
 */
typedef struct {
	uint8_t							version;		// configuration version
	uint8_t							sendDuty;		// Period between two communications x10s
	uint8_t							ackDuty;		// Number of sendDuty period before requesting a ACK to verify Network connectivity x sendDuty
	uint8_t							ackRetry;		// Number of ackDuty retry before considering reconnection x ackDuty
	uint8_t							sleepDuty;		// Period between two connection try when disconnected x sendDuty

	uint8_t							alignment[3];	// For 32bits size alignment
} itsdk_configuration_app_t;


#endif /* INC_IT_SDK_CONFIGNVM_H_ */
