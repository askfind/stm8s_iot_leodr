/*
 * Copyright (C) 2018	Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     Driver  
 * @{
 *
 * @file		ds18b20.h
 * @brief   	Temperature
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru</a> 
 */
#ifndef DS18B20_H
#define DS18B20_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

void show_ROM(uint8_t *ptr);
void show_last_ROM();
void show_stored_ROMs();
void send_read_seq();
void read_next_sensor();
void show_received_data();
void wait_reading();
void start_temp_reading();

void DS18B20_init();
void DS18B20_process() ;

#ifdef __cplusplus
}
#endif

#endif /* DS18B20_H */
/** @} */
