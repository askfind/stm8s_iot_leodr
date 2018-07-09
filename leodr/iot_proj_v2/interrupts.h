/*
 * Copyright (C) 2018	Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     main 
 * @{
 *
 * @file		interrupts.h
 * @brief   	
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru</a> 
 */

#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "stm8s.h"

// Top Level Interrupt
INTERRUPT_DEFINITION(TLI_IRQHandler, 0);

// Auto Wake Up Interrupt
INTERRUPT_DEFINITION(AWU_IRQHandler, 1);

// Clock Controller Interrupt
INTERRUPT_DEFINITION(CLK_IRQHandler, 2);

// External Interrupt PORTA
INTERRUPT_DEFINITION(EXTI_PORTA_IRQHandler, 3);

// External Interrupt PORTB
INTERRUPT_DEFINITION(EXTI_PORTB_IRQHandler, 4);

// External Interrupt PORTC
INTERRUPT_DEFINITION(EXTI_PORTC_IRQHandler, 5);

// External Interrupt PORTD
INTERRUPT_DEFINITION(EXTI_PORTD_IRQHandler, 6);

// External Interrupt PORTE
INTERRUPT_DEFINITION(EXTI_PORTE_IRQHandler, 7);

#ifdef STM8S903
// External Interrupt PORTF
INTERRUPT_DEFINITION(EXTI_PORTF_IRQHandler, 8);
#endif // STM8S903

#if defined (STM8S208) || defined (STM8AF52Ax)
// CAN RX Interrupt routine.
INTERRUPT_DEFINITION(CAN_RX_IRQHandler, 8);

// CAN TX Interrupt routine.
INTERRUPT_DEFINITION(CAN_TX_IRQHandler, 9);
#endif // STM8S208 || STM8AF52Ax

// SPI Interrupt routine.
INTERRUPT_DEFINITION(SPI_IRQHandler, 10);

// Timer1 Update/Overflow/Trigger/Break Interrupt
INTERRUPT_DEFINITION(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11);

// Timer1 Capture/Compare Interrupt routine.
INTERRUPT_DEFINITION(TIM1_CAP_COM_IRQHandler, 12);

#ifdef STM8S903
// Timer5 Update/Overflow/Break/Trigger Interrupt
INTERRUPT_DEFINITION(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13);

// Timer5 Capture/Compare Interrupt
INTERRUPT_DEFINITION(TIM5_CAP_COM_IRQHandler, 14);

#else // STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x
// Timer2 Update/Overflow/Break Interrupt
INTERRUPT_DEFINITION(TIM2_UPD_OVF_BRK_IRQHandler, 13);

// Timer2 Capture/Compare Interrupt
INTERRUPT_DEFINITION(TIM2_CAP_COM_IRQHandler, 14);
#endif // STM8S903

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
// Timer3 Update/Overflow/Break Interrupt
INTERRUPT_DEFINITION(TIM3_UPD_OVF_BRK_IRQHandler, 15);

// Timer3 Capture/Compare Interrupt
INTERRUPT_DEFINITION(TIM3_CAP_COM_IRQHandler, 16);
#endif // STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
// UART1 TX Interrupt
INTERRUPT_DEFINITION(UART1_TX_IRQHandler, 17);

// UART1 RX Interrupt
INTERRUPT_DEFINITION(UART1_RX_IRQHandler, 18);
#endif // STM8S208 or STM8S207 or STM8S103 or STM8S903 or STM8AF62Ax or STM8AF52Ax

// I2C Interrupt
INTERRUPT_DEFINITION(I2C_IRQHandler, 19);

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
// UART2 TX interrupt
INTERRUPT_DEFINITION(UART2_TX_IRQHandler, 20);

// UART2 RX interrupt
INTERRUPT_DEFINITION(UART2_RX_IRQHandler, 21);
#endif // STM8S105 or STM8AF626x

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
// UART3 TX interrupt
INTERRUPT_DEFINITION(UART3_TX_IRQHandler, 20);

// UART3 RX interrupt
INTERRUPT_DEFINITION(UART3_RX_IRQHandler, 21);
#endif // STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
// ADC2 interrupt
INTERRUPT_DEFINITION(ADC2_IRQHandler, 22);
#else // STM8S105, STM8S103 or STM8S903 or STM8AF626x
// ADC1 interrupt
INTERRUPT_DEFINITION(ADC1_IRQHandler, 22);
#endif // STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax

#ifdef STM8S903
// Timer6 Update/Overflow/Trigger Interrupt
INTERRUPT_DEFINITION(TIM6_UPD_OVF_TRG_IRQHandler, 23);
#else // STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x
// Timer4 Update/Overflow Interrupt
INTERRUPT_DEFINITION(TIM4_UPD_OVF_IRQHandler, 23);
#endif // STM8S903

// Eeprom EEC Interrupt
INTERRUPT_DEFINITION(EEPROM_EEC_IRQHandler, 24);

#endif // __INTERRUPTS_H__
