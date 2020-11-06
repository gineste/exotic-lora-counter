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
#ifndef CONFIG_GLOBALDEFS_H_
#define CONFIG_GLOBALDEFS_H_

/****************************************************************************************
 * Include Files
 ****************************************************************************************/
#include <stdint.h>

/* it-sdk */
#include "it_sdk/logger/logger.h"
#include "it_sdk/wrappers.h"

/* config */
#include "vichydro/config/boardConfig.h"

/****************************************************************************************
 * Defines
 ****************************************************************************************/
#if (USE_DEBUG == 1u)
	#define SPTK_PRINT(lvl, msg)
	#define RESET_STYLE()
	#define PRINT_FAST(p_data)						log_info(p_data)
	#define PRINT_CUSTOM(p_type, ...)			log_info(p_type, __VA_ARGS__)
   #define PRINT_DEBUG(p_type, p_data)			log_debug(p_type, p_data)
   #define PRINT_INFO(p_type, p_data)			log_info(p_type, p_data)
   #define PRINT_STATE(p_type, p_data)
   #define PRINT_WARNING(p_type, p_data)		log_warn(p_type, p_data)
   #define PRINT_ERROR(p_type, p_data)			log_error(p_type, p_data)
#else
	#define SPTK_PRINT(lvl, msg)
	#define RESET_STYLE()
	#define PRINT_FAST(p_data)
	#define PRINT_CUSTOM(p_type, ...)
   #define PRINT_DEBUG(p_type, p_data)
   #define PRINT_INFO(p_type, p_data)
   #define PRINT_STATE(p_type, p_data)
   #define PRINT_WARNING(p_type, p_data)
   #define PRINT_ERROR(p_type, p_data)
#endif

/****************************************************************************************
 * Type definitions
 ****************************************************************************************/

/****************************************************************************************
 * Public function declarations
 ****************************************************************************************/

#endif /* CONFIG_GLOBALDEFS_H_ */
