/*
 * Copyright (C) 2018	Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     USER 
 * @{
 *
 * @file		uart.h
 * @brief   	
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru</a> 
 */

#ifndef UART_H
#define UART_H

//extern volatile unsigned long Global_time; // global time in ms

#define UART_BUF_LEN 32			// max 7 bytes transmited in on operation

extern uint8_t UART_rx[];
extern uint8_t UART_rx_start_i;
extern uint8_t UART_rx_cur_i;

extern uint8_t UART_tx[];
extern uint8_t UART_tx_start_i;
extern uint8_t UART_tx_cur_i;

void UART_init();
void UART_send_byte(uint8_t byte);
void UART_write(char *str);
uint8_t UART_read_byte(uint8_t *byte);

//viv+ new
void UART1_Config(void);


#define check_UART_pointer(x)  do{if(x == UART_BUF_LEN) x = 0;}while(0)

#endif /* UART_H_*/
