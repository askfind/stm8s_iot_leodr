/*
 * Copyright (C) 2018	Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     Config STM8S-IoT
 * @{
 *
 * @file		stm8s_iot_config.h
 * @brief   	Сhoice of optionsn
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru</a> 
 */

#ifndef STM8S_IOT_CONFIG_H
#define STM8S_IOT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Config flags
 * @{
 */
#define LED_SET					(1) 


/**
 *  General purpose I/O ports (GPIO)
 */


/**
 *  System clock to use HSI clock source
 */
#define CLK_CONFIG_SET			(1) 
#define HSI_2_MHZ_CLOCK_SOURCE 	(0)  
#define HSI_4_MHZ_CLOCK_SOURCE 	(0) 
#define HSI_8_MHZ_CLOCK_SOURCE 	(0)
#define HSI_16_MHZ_CLOCK_SOURCE (1)

#define  CLK_CCO_OUTPUT_SET		(1)

/**
 *  (UART)
 */
#define  UART1_SET				(1)

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* STM8S_IOT_CONFIG_H */
/** @} */
