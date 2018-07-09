/*
 * blinky.c
 *
 * Copyright 2014 Edward V. Emelianoff <eddy@sao.ru>
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


#include "ports_definition.h"
#include "onewire.h"
#include "uart.h"
#include "interrupts.h"

uint8_t UART_rx[UART_BUF_LEN]; // cycle buffer for received data
uint8_t UART_rx_start_i = 0;   // started index of received data (from which reading starts)
uint8_t UART_rx_cur_i = 0;     // index of current first byte in rx array (to which data will be written)

/**
 * Send one byte through UART
 * @param byte - data to send
 */
void UART_send_byte(uint8_t byte){
	while(!(UART1_SR & UART_SR_TXE)); // wait until previous byte transmitted
	UART1_DR = byte;
}

void uart_write(char *str){
	while(*str){
		while(!(UART1_SR & UART_SR_TXE));
		UART1_CR2 |= UART_CR2_TEN;
		UART1_DR = *str++;
	}
}

/**
 * Read one byte from Rx buffer
 * @param byte - where to store readed data
 * @return 1 in case of non-empty buffer
 */
uint8_t UART_read_byte(uint8_t *byte){
	if(UART_rx_start_i == UART_rx_cur_i) // buffer is empty
		return 0;
	*byte = UART_rx[UART_rx_start_i++];
	check_UART_pointer(UART_rx_start_i);
	return 1;
}

void printUint(uint8_t *val, uint8_t len){
	unsigned long Number = 0;
	uint8_t i = len;
	char ch;
	uint8_t decimal_buff[12]; // max len of U32 == 10 + \n + \0
	if(len > 4 || len == 3 || len == 0) return;
	for(i = 0; i < 12; i++)
		decimal_buff[i] = 0;
	decimal_buff[10] = '\n';
	ch = 9;
	switch(len){
		case 1:
			Number = *((uint8_t*)val);
			break;
		case 2:
			Number = *((uint16_t*)val);
		break;
		case 4:
			Number = *((unsigned long*)val);
		break;
	}
	do{
		i = Number % 10L;
		decimal_buff[ch--] = i + '0';
		Number /= 10L;
	}while(Number && ch > -1);
	uart_write((char*)&decimal_buff[ch+1]);
}

/**
 * print signed long onto terminal
 * max len = 10 symbols + 1 for "-" + 1 for '\n' + 1 for 0 = 13
 */
void print_long(long Number){
	uint8_t i, L = 0;
	char ch;
	char decimal_buff[12];
	decimal_buff[11] = 0;
	ch = 11;
	if(Number < 0){
		Number = -Number;
		L = 1;
	}
	do{
		i = Number % 10L;
		decimal_buff[--ch] = i + '0';
		Number /= 10L;
	}while(Number && ch > 0);
	if(ch > 0 && L) decimal_buff[--ch] = '-';
	uart_write(&decimal_buff[ch]);
}

uint8_t readInt(int *val){
	unsigned long T = Global_time;
	unsigned long R = 0;
	int readed;
	uint8_t sign = 0, rb, ret = 0, bad = 0;
	do{
		if(!UART_read_byte(&rb)) continue;
		if(rb == '-' && R == 0){ // negative number
			sign = 1;
			continue;
		}
		if(rb < '0' || rb > '9') break; // number ends with any non-digit symbol that will be omitted
		ret = 1; // there's at least one digit
		R = R * 10L + rb - '0';
		if(R > 0x7fff){ // bad value
			R = 0;
			bad = 0;
		}
	}while(Global_time - T < 10000); // wait no longer than 10s
	if(bad || !ret) return 0;
	readed = (int) R;
	if(sign) readed *= -1;
	*val = readed;
	return 1;
}

void error_msg(char *msg){
	uart_write("\nERROR: ");
	uart_write(msg);
	UART_send_byte('\n');
}

uint8_t uint8_ttoHEX(uint8_t val){
	val &= 0x0f;
	if(val < 10) val += '0';
	else val += 'a' - 10;
	return val;
}

void printUHEX(uint8_t val){
	uart_write("0x");
	UART_send_byte(uint8_ttoHEX(val>>4)); // MSB
	UART_send_byte(uint8_ttoHEX(val));    // LSB
}

void uart_init(){
	// PD5 - UART2_TX
	PORT(UART_PORT, DDR) |= UART_TX_PIN;
	PORT(UART_PORT, CR1) |= UART_TX_PIN;
// Configure UART
	// 8 bit, no parity, 1 stop (UART_CR1/3 = 0 - reset value)
	// 57600 on 16MHz: BRR1=0x11, BRR2=0x06
	UART1_BRR1 = 0x11; UART1_BRR2 = 0x06;
	UART1_CR2 = UART_CR2_TEN | UART_CR2_REN | UART_CR2_RIEN; // Allow RX/TX, generate ints on rx
}


