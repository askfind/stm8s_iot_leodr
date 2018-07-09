/*
 * Copyright (C) 2009-2018  Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     ADC
 * @{
 *
 * @file		adc.c
 * @brief       ADC
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru/</a> 
 * @}
 */

#include <stdint.h>
#include <stdio.h>
#include <stm8s.h>
#include <uart.h>
#include <delay.h>

#include <adc.h>

uint16_t ADC_read() {
    uint8_t adcH, adcL;
    ADC1_CR1 |= (1 << ADC1_CR1_ADON);
    while (!(ADC1_CSR & (1 << ADC1_CSR_EOC)));
    adcL = ADC1_DRL;
    adcH = ADC1_DRH;
    ADC1_CSR &= ~(1 << ADC1_CSR_EOC); // clear EOC flag
    return (adcL | (adcH << 8));
}

void ADC_init() {

    /* configure ADC channel 3 (PD2)  */
    ADC1_CSR |= (1 << 1);

    /* right-align data */
    ADC1_CR2 |= (1 << ADC1_CR2_ALIGN);

    /* wake ADC from power down */
    ADC1_CR1 |= (1 << ADC1_CR1_ADON);
}

/* EOF adc.c */
