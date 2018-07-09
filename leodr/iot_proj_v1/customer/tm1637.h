/*
 * Copyright (C) 2018	Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     Protocol 
 * @{
 *
 * @file		mmsg.h
 * @brief   	Protocol mmsg
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru</a> 
 */
// indigo6alpha's TM1637 library for STM8SF103F3 MCU
// Project started 3/17/2018
// written by indigo6alpha (indigosixalpha164@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>

#ifndef TM1637_H
#define TM1637_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <time.h>

/* ------------------- TYPES  ------------------- */
typedef unsigned char* 	gpio_port_t;
typedef unsigned char	gpio_pin_t;

/* ------------------- REGISTERS -------------- */
#define GPIO_OFST_ODR			0x0000
#define GPIO_OFST_IDR			0x0001
#define GPIO_OFST_DDR			0x0002
#define GPIO_OFST_CR1			0x0003
#define GPIO_OFST_CR2			0x0004

#define GPIO_PORT_A			(gpio_port_t)0x5000

#define GPIO_PORT_B			(gpio_port_t)0x5005

#define GPIO_PORT_C			(gpio_port_t)0x500A

#define GPIO_PORT_D			(gpio_port_t)0x500F

#define GPIO_PORT_E 			(gpio_port_t)0x5014

#define GPIO_PORT_F 			(gpio_port_t)0x5019

/* ------------------- PINS ------------------- */
#define GPIO_PIN_0		1
#define GPIO_PIN_1		2
#define GPIO_PIN_2		4
#define GPIO_PIN_3		8
#define GPIO_PIN_4		16
#define GPIO_PIN_5		32
#define GPIO_PIN_6		64
#define GPIO_PIN_7		128
#define GPIO_PIN_ALL	255

/// <summary>
/// Sleep for the specified time (in ms). It internally just calls the CLK delay function defined in clock.h
/// </summary>
void usleep(int milliseconds);

/// <summary>
/// Initialize tm1637 with the clock and data pins
/// </summary>
int tm1637Init(gpio_port_t bClock, gpio_pin_t bClockP, gpio_port_t bData, gpio_pin_t bDataP);

/// <summary>
/// Start wire transaction
/// </summary>
static void tm1637Start(void);

/// <summary>
/// Stop wire transaction
/// </summary>
static void tm1637Stop(void);

/// <summary>
/// Get data acknowledgement
/// </summary>
static unsigned char tm1637GetAck(void);

/// <summary>
/// Write a unsigned char to the controller
/// </summary>
static void tm1637WriteByte(unsigned char b);

/// <summary>
/// Write a sequence of unsigned chars to the controller
/// </summary>
static void tm1637Write(unsigned char *pData, unsigned char bLen);

/// <summary>
/// Set brightness (0-8)
/// </summary>
void tm1637SetBrightness(unsigned char b);

/// <summary>
/// Display a string of 4 digits and optional colon
/// by passing a string such as "12:34" or "45 67"
/// </summary>
void tm1637ShowDigits(char *pString);

void TM1637_init() ;
void TM1637_process();

#ifdef __cplusplus
}
#endif

#endif /* TM1637_H */
/** @} */
