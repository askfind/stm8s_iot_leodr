
/*
 * Copyright (C) 2018  Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     LEDs
 * @{
 *
 * @file		leds.c
 * @brief       Control Leds
 *
 * @author       Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru/</a> 
 * @}
 */
 
#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"

#include "../stm8s_iot_config.h" 
#include "leds.h"

static void DelayForLed(u32 nCount);

void LED_Conf(void)
{
	#if LED_SET		
	/* Initialize I/Os in Output Mode */
	GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
    #endif /* LED_SET */
}

void LED_On(void)
{
	/*  On LEDs */
	GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
}

void LED_Off(void)
{
	/*  Off LEDs */
	GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
}

void LED_Togle(void)
{
	/*  Toggles LEDs */
	GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
}

static void DelayForLed(u32 nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }

}

/* EOF leds.c */
