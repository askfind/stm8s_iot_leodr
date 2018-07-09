/*
 * Copyright (C) 2018	Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     ADC  
 * @{
 *
 * @file		adc.h
 * @brief   	ADC
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru</a> 
 */
#ifndef ADC_H
#define ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

extern void ADC_init();
extern uint16_t ADC_read();

#ifdef __cplusplus
}
#endif

#endif /* ADC_H */
/** @} */
