/*
 * Copyright (C) 2018  Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     USER Library
 * @{
 *
 * @file		clk.c
 * @brief       
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru/</a> 
 * @}
 */

#include "stm8s.h"
#include "../stm8s_iot_config.h"
#include "clk.h"
#include "common.h"

/**
  * @brief  Configure system clock to run at Mhz
  * @param  None
  * @retval None
  */
void CLK_Config(void)
{
  CLK_DeInit();
  
#if CLK_CONFIG_SET

	#if 	HSI_2_MHZ_CLOCK_SOURCE
		/* Clock divider to HSI/8 */
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);
	#elif 	HSI_4_MHZ_CLOCK_SOURCE
		/* Clock divider to HSI/4 */
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV4);
	#elif 	HSI_8_MHZ_CLOCK_SOURCE
		/* Clock divider to HSI/2 */
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
	#elif 	HSI_16_MHZ_CLOCK_SOURCE
		/* Clock divider to HSI/1 */
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	#else
		#warning "Default HSI clock"
	#endif /* HSI CLOCK_SOURCE */

#endif /* CLK_CONFIG_SET */
}

/**
  * @brief  Output the system clock on
  *         CCO pin
  * @param  None
  * @retval None
  */
void CLK_CCO_Output_Config(void)
{
#if CLK_CCO_OUTPUT_SET
  /* Output Fcpu on CLK_CCO pin */
  CLK_CCOConfig(CLK_OUTPUT_MASTER);
#endif /* CLK_CCO_OUTPUT_SET */	
}

/* EOF clk.c */
