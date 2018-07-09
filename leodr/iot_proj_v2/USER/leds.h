/*
 * Copyright (C) 2018	Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     LEDs 
 * @{
 *
 * @file		leds.h
 * @brief   	Control Leds
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru</a> 
 */
#ifndef LEDS_H
#define LEDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm8s.h"

/* 
 * LED
 */
#define LED_GPIO_PORT  			(GPIOB)
#define LED_GPIO_PINS  			(GPIO_PIN_5)
 
void LED_Conf(void);
void LED_On(void);
void LED_Off(void);
void LED_Togle(void);
void LED_Display(void);
void LED_Display_Bit();

#ifdef __cplusplus
}
#endif

#endif /* LEDS_H */
/** @} */
