/*
 * Copyright (C) 2018  Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     main
 * @{
 *
 * @file		interrupts.c
 * @brief       STM8S-IOT Loop
 *
 * @author       Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru/</a> 
 * @}
 */
 
#include <stdint.h>
#include "stm8s.h"
#include "uart.h"
#include "onewire.h"

#if 0 //viv- old 

// Top Level Interrupt
INTERRUPT_HANDLER(TLI_IRQHandler, 0){}

// Auto Wake Up Interrupt
INTERRUPT_HANDLER(AWU_IRQHandler, 1){}

// Clock Controller Interrupt
INTERRUPT_HANDLER(CLK_IRQHandler, 2){}

// External Interrupt PORTA
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3){}

// External Interrupt PORTB
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4){}

// External Interrupt PORTC
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5){
}

// External Interrupt PORTD
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6){
}

// External Interrupt PORTE
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7){}

#ifdef STM8S903
// External Interrupt PORTF
INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8){}
#endif // STM8S903

#if defined (STM8S208) || defined (STM8AF52Ax)
// CAN RX Interrupt routine.
INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8){}

// CAN TX Interrupt routine.
INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9){}
#endif // STM8S208 || STM8AF52Ax

// SPI Interrupt routine.
INTERRUPT_HANDLER(SPI_IRQHandler, 10){}

// Timer1 Update/Overflow/Trigger/Break Interrupt
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11){
}

// Timer1 Capture/Compare Interrupt routine.
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12){}

#ifdef STM8S903
// Timer5 Update/Overflow/Break/Trigger Interrupt
INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13){}

// Timer5 Capture/Compare Interrupt
INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14){}

#else // STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x

// Timer2 Update/Overflow/Break Interrupt
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13){
}

// Timer2 Capture/Compare Interrupt
// manage with sending/receiving
INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14){
#if 0 //viv-
	TIM2_SR1 &= ~TIM_SR1_CC1IF;
	onewire_gotlen = TIM2_CCR1H << 8;
	onewire_gotlen |= TIM2_CCR1L;
	if(onewire_tick_ctr){ // there's some more data to transmit / receive
		--onewire_tick_ctr;
		if(is_receiver){// receive bits
			ow_data >>= 1;
			if(onewire_gotlen < ONE_ZERO_BARRIER){ // this is 1
				ow_data |= 0x80; // LSbit first!
			}
			// in receiver mode we don't need to send byte after ctr is zero!
			if(onewire_tick_ctr == 0){
				TIM2_CR1 &= ~TIM_CR1_CEN;
			}
		}else{// transmit bits
			// update CCR2 registers with new values
			if(ow_data & 1){ // transmit 1
				TIM2REG(CCR2, BIT_ONE_P);
			}else{ // transmit 0
				TIM2REG(CCR2, BIT_ZERO_P);
			}
			ow_data >>= 1;
		}
	}else{ // end: turn off timer
		TIM2_CR1 &= ~TIM_CR1_CEN;
	}
#endif  //viv-			
}
#endif // STM8S903

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
// Timer3 Update/Overflow/Break Interrupt
INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15){}

// Timer3 Capture/Compare Interrupt
INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16){}
#endif // STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
// UART1 TX Interrupt
INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17){}

// UART1 RX Interrupt
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18){
#if 0 //viv-
	uint8_t rb;
	if(UART1_SR & UART_SR_RXNE){ // data received
		rb = UART1_DR; // read received byte & clear RXNE flag
		while(!(UART1_SR & UART_SR_TXE));
		UART1_DR = rb; // echo received symbol
		UART_rx[UART_rx_cur_i++] = rb; // put received byte into cycled buffer
		if(UART_rx_cur_i == UART_rx_start_i){ // Oops: buffer overflow! Just forget old data
			UART_rx_start_i++;
			check_UART_pointer(UART_rx_start_i);
		}
		check_UART_pointer(UART_rx_cur_i);
	}
#endif  //viv-	
}
#endif // STM8S208 or STM8S207 or STM8S103 or STM8S903 or STM8AF62Ax or STM8AF52Ax

// I2C Interrupt
INTERRUPT_HANDLER(I2C_IRQHandler, 19){}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
// UART2 TX interrupt
INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20){}

// UART2 RX interrupt
INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21){
#if 0 //viv-	
	uint8_t rb;
	if(UART2_SR & UART_SR_RXNE){ // data received
		rb = UART2_DR; // read received byte & clear RXNE flag
		while(!(UART2_SR & UART_SR_TXE));
		UART2_DR = rb; // echo received symbol
		UART_rx[UART_rx_cur_i++] = rb; // put received byte into cycled buffer
		if(UART_rx_cur_i == UART_rx_start_i){ // Oops: buffer overflow! Just forget old data
			UART_rx_start_i++;
			check_UART_pointer(UART_rx_start_i);
		}
		check_UART_pointer(UART_rx_cur_i);
	}
#endif //viv-	
}
#endif // STM8S105 or STM8AF626x

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
// UART3 TX interrupt
INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20){}

// UART3 RX interrupt
INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21){}
#endif // STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
// ADC2 interrupt
INTERRUPT_HANDLER(ADC2_IRQHandler, 22){}
#else
// ADC1 interrupt
INTERRUPT_HANDLER(ADC1_IRQHandler, 22){
}
#endif // STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax

#ifdef STM8S903
// Timer6 Update/Overflow/Trigger Interrupt
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23){}
#else // STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x
// Timer4 Update/Overflow Interrupt
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23){
#if 0 //viv-	
	if(TIM4_SR & TIM_SR1_UIF){ // update interrupt
		Global_time++; // increase timer
	}
	TIM4_SR = 0; // clear all interrupt flags
#endif	
}
#endif // STM8S903

// Eeprom EEC Interrupt
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24){}

#endif //viv- old

// viv+ new  +++++++++++++++++++++++++++++++++++++++

/** @addtogroup TIM2_PWM_DutyCycleConfiguration
  * @{
  */
#ifdef _COSMIC_
/**
  * @brief  Dummy interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief  TRAP interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  Top Level Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Auto Wake Up Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Clock Controller Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTA Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTB Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTC Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTD Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External Interrupt PORTE Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#ifdef STM8S903
/**
  * @brief  External Interrupt PORTF Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  CAN TX Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208 || STM8AF52Ax */

/**
  * @brief  SPI Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Timer1 Update/Overflow/Trigger/Break Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Timer1 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#ifdef STM8S903
/**
  * @brief  Timer5 Update/Overflow/Break/Trigger Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  Timer5 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8S001 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
/**
  * @brief  Timer2 Update/Overflow/Break Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Timer2 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) || defined(STM8AF62Ax) || defined(STM8AF52Ax) || defined(STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  Timer3 Capture/Compare Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) || defined(STM8S001) || defined(STM8AF62Ax) || defined(STM8AF52Ax) || defined(STM8S903)
/**
  * @brief  UART1 TX Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    /* Write one byte to the transmit data register */
    //viv- UART1_SendData8(TxBuffer1[IncrementVar_TxCounter1()]);

    //vivif (GetVar_TxCounter1() == GetVar_NbrOfDataToTransfer1())
    {
        /* Disable the UART1 Transmit interrupt */
        UART1_ITConfig(UART1_IT_TXE, DISABLE);
    }  
}

/**
  * @brief  UART1 RX Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
   /* Read one byte from the receive data register */
   //viv- RxBuffer1[IncrementVar_RxCounter1()] = UART1_ReceiveData8();

    //viv- if (GetVar_RxCounter1() == GetVar_NbrOfDataToRead1())
    {
        /* Disable the UART1 Receive interrupt */
        UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
    }  
}
#endif /*STM8S105 || STM8S001 */

/**
  * @brief  I2C Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief  UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }

/**
  * @brief  UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }
#endif /* STM8S105*/

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief  UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }

/**
  * @brief  UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  }
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief  ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
{

    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    return;

}
#else /*STM8S105, STM8S103 or STM8S903 or STM8AF626x */
/**
  * @brief  ADC1 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{

    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    return;

}
#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#ifdef STM8S903
/**
  * @brief  Timer6 Update/Overflow/Trigger Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8S001 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
/**
  * @brief  Timer4 Update/Overflow Interrupt routine
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*STM8S903*/

/**
  * @brief  Eeprom EEC Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */



/* EOF interrupts.c */
