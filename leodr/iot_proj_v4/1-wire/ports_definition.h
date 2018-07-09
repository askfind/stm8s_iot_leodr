/*
 * ports_definition.h - definition of ports pins & so on
 *
 * Copyright 2014 Edward V. Emelianov <eddy@sao.ru, edward.emelianoff@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#pragma once
#ifndef __PORTS_DEFINITION_H__
#define __PORTS_DEFINITION_H__

#include <stdint.h>
//#include "stm8l.h"
#include "stm8s.h"


// macro for using in port constructions like PORT(LED_PORT, ODR) = xx
#define CONCAT(a, b)	a ## _ ## b
#define PORT(a, b)		CONCAT(a , b)

// on-board LED
#define LED_PORT		PB
#define LED_PIN			GPIO_PIN5

// control LED
#define LED_OFF() PORT(LED_PORT, ODR) = LED_PIN;
#define LED_ON() PORT(LED_PORT, ODR) &= (~LED_PIN);
#define LED_TGL() PORT(LED_PORT, ODR) ^= LED_PIN;

// control PD5_OUT
#define OUT_PORT		PD
#define OUT_PIN			GPIO_PIN3

// UART1_TX
#define UART_PORT		PD
#define UART_TX_PIN		GPIO_PIN5

#endif // __PORTS_DEFINITION_H__
