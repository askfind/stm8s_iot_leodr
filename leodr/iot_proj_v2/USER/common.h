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
 * @file		common.h
 * @brief       
 *
 * @author       Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru/</a> 
 * @}
 */

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define  Global_time (100) 

/**
 * @name    
 * @{
 */
void uli2a(uint32_t num, uint16_t base, int16_t uc, uint8_t * bf);
 
void printUint(uint8_t *val, uint8_t len);
void print_long(long Number);
void error_msg(char *msg);
void printUHEX(uint8_t val);

void TestOptim(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */
/** @} */
