/*
 *	Copyright (C) 2018  Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     USER
 * @{
 *
 * @file		uart.c
 * @brief       STM8S-IOT Loop
 *
 * @author       Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru/</a> 
 * @}
 */

#include "stm8s.h"
#include "../stm8s_iot_config.h"
//#include "interrupts.h"
#include "onewire.h"
#include "uart.h"
#include "common.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

uint8_t UART_rx[UART_BUF_LEN]; // cycle buffer for received data
uint8_t UART_rx_start_i = 0;   // started index of received data (from which reading starts)
uint8_t UART_rx_cur_i = 0;     // index of current first byte in rx array (to which data will be written)

uint8_t UART_tx[UART_BUF_LEN];
uint8_t UART_tx_start_i;
uint8_t UART_tx_cur_i;

/**
 * Init UART
 * @param byte - data to send
 */
void UART_init(){
	#if UART1_SET
		/* Configure the UART1 */
		UART1_Config();
	#endif
}

/**
 * Send one byte through UART
 * @param byte - data to send
 */
void UART_send_byte(uint8_t byte){
#if 0 //viv-	
	while(!(UART1_SR & UART_SR_TXE)); // wait until previous byte transmitted
	UART1_DR = byte;
#endif //viv-		
}

void UART_write(char *str){
#if 0 //viv-	
	while(*str){
		while(!(UART1_SR & UART_SR_TXE));
		UART1_CR2 |= UART_CR2_TEN;
		UART1_DR = *str++;
	}
#endif //viv-		
}

/**
 * Read one byte from Rx buffer
 * @param byte - where to store readed data
 * @return 1 in case of non-empty buffer
 */
uint8_t UART_read_byte(uint8_t *byte){
#if 0 //viv-	
	if(UART_rx_start_i == UART_rx_cur_i) // buffer is empty
		return 0;
	*byte = UART_rx[UART_rx_start_i++];
	check_UART_pointer(UART_rx_start_i);
	return 1;
#else
		return 0;
#endif //viv-		
}

//viv+ new code 
void UART1_Config(void) {
/* Deinitializes the UART1 and UART3 peripheral */
    UART1_DeInit();
    /* UART1 configuration -------------------------------------------------*/
    /* UART1 configured as follow:
          - BaudRate = 57600 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Receive and transmit enabled
          - UART1 Clock disabled
     */
    /* Configure the UART1 */
    UART1_Init((uint32_t)57600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
                UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE); 

	/* Enable UART1 Transmit interrupt*/
    UART1_ITConfig(UART1_IT_TXE, ENABLE); 
    
    /* Enable general interrupts */
    //viv- it in main  enableInterrupts();                
}

/* EOF uart.c */
