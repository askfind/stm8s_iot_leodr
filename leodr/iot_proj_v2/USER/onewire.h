/*
 * onewire.h
 *
 * Copyright 2015 Edward V. Emelianoff <eddy@sao.ru, edward.emelianoff@gmail.com>
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


#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

#include <stdint.h>
#include "stm8s.h"

/* functions */
/*viv-
#define enableInterrupts()    {__asm__("rim\n");}    // enable interrupts
#define disableInterrupts()   {__asm__("sim\n");}    // disable interrupts
#define iret()                {__asm__("iret\n");}   // Interrupt routine return
#define pop_ccr()             {__asm__("pop cc\n");} // Pop CCR from the stack
#define push_ccr()            {__asm__("push cc\n");}// Push CCR on the stack
#define rim()                 {__asm__("rim\n");}    // enable interrupts
#define sim()                 {__asm__("sim\n");}    // disable interrupts
#define nop()                 {__asm__("nop\n");}    // No Operation
#define trap()                {__asm__("trap\n");}   // Trap (soft IT)
#define wfi()                 {__asm__("wfi\n");}    // Wait For Interrupt
#define halt()                {__asm__("halt\n");}   // Halt
*/

#define NULL (void *)0

#define GPIO_PIN3		(1 << 3)
#define GPIO_PIN5		(1 << 5)

/* ------------------- swim control ------------------- */
#define CFG_GCR			*(unsigned char*)0x7F60
#define SWIM_CSR		*(unsigned char*)0x7F80

/* UART_SR bits */
#define UART_SR_TXE (1 << 7)
#define UART_SR_TC (1 << 6)
#define UART_SR_RXNE (1 << 5)
#define UART_SR_IDLE (1 << 4)
#define UART_SR_OR (1 << 3)
#define UART_SR_NF (1 << 2)
#define UART_SR_FE (1 << 1)
#define UART_SR_PE (1 << 0)

/* UART_CR2 bits */
#define UART_CR2_TIEN (1 << 7)
#define UART_CR2_TCIEN (1 << 6)
#define UART_CR2_RIEN (1 << 5)
#define UART_CR2_ILIEN (1 << 4)
#define UART_CR2_TEN (1 << 3)
#define UART_CR2_REN (1 << 2)
#define UART_CR2_RWU (1 << 1)
#define UART_CR2_SBK (1 << 0)

/* TIM_IER bits */
#define TIM_IER_BIE (1 << 7)
#define TIM_IER_TIE (1 << 6)
#define TIM_IER_COMIE (1 << 5)
#define TIM_IER_CC4IE (1 << 4)
#define TIM_IER_CC3IE (1 << 3)
#define TIM_IER_CC2IE (1 << 2)
#define TIM_IER_CC1IE (1 << 1)
#define TIM_IER_UIE (1 << 0)

/* TIM_CR1 bits */
#define TIM_CR1_APRE (1 << 7)
#define TIM_CR1_CMSH (1 << 6)
#define TIM_CR1_CMSL (1 << 5)
#define TIM_CR1_DIR (1 << 4)
#define TIM_CR1_OPM (1 << 3)
#define TIM_CR1_URS (1 << 2)
#define TIM_CR1_UDIS (1 << 1)
#define TIM_CR1_CEN (1 << 0)

/* TIM_SR1 bits */
#define TIM_SR1_BIF (1 << 7)
#define TIM_SR1_TIF (1 << 6)
#define TIM_SR1_COMIF (1 << 5)
#define TIM_SR1_CC4IF (1 << 4)
#define TIM_SR1_CC3IF (1 << 3)
#define TIM_SR1_CC2IF (1 << 2)
#define TIM_SR1_CC1IF (1 << 1)
#define TIM_SR1_UIF (1 << 0)

/* TIM_EGR bits */
#define TIM_EGR_BG (1 << 7)
#define TIM_EGR_TG (1 << 6)
#define TIM_EGR_COMG (1 << 5)
#define TIM_EGR_CC4G (1 << 4)
#define TIM_EGR_CC3G (1 << 3)
#define TIM_EGR_CC2G (1 << 2)
#define TIM_EGR_CC1G (1 << 1)
#define TIM_EGR_UG (1 << 0)

#define EEPROM_KEY1		0xAE  // keys to manage EEPROM's write access
#define EEPROM_KEY2		0x56
#define EEPROM_START_ADDR  (unsigned char*)0x4000


#define ERR_TEMP_VAL  200000

#define TIM2REGH(reg)  TIM2_##reg##H
#define TIM2REGL(reg)  TIM2_##reg##L
#define TIM2REG(reg, val)  do{TIM2REGH(reg) = val >> 8; TIM2REGL(reg) = val & 0xff;}while(0)


// ARR values: 1000 for reset, 100 for data in/out
// CCR2 values: 500 for reset, 60 for sending 0 or reading, <15 for sending 1
// CCR1 values: >550 if there's devices on line (on reset), >12 (typ.15) - read 0, < 12 (typ.1) - read 1
#define RESET_LEN         ((uint16_t)1000)
#define BIT_LEN           ((uint16_t)100)
#define RESET_P           ((uint16_t)500)
#define BIT_ONE_P         ((uint16_t)10)
#define BIT_ZERO_P        ((uint16_t)60)
#define BIT_READ_P        ((uint16_t)5)
#define RESET_BARRIER     ((uint16_t)550)
#define ONE_ZERO_BARRIER  ((uint16_t)10)

#define OW_BUSY  ((TIM2_CR1 & TIM_CR1_CEN))


/*
 * thermometer commands
 * send them with bus reset!
 */
// find devices
#define OW_SEARCH_ROM       (0xf0)
// read device (when it is alone on the bus)
#define OW_READ_ROM         (0x33)
// send device ID (after this command - 8 bytes of ID)
#define OW_MATCH_ROM        (0x55)
// broadcast command
#define OW_SKIP_ROM         (0xcc)
// find devices with critical conditions
#define OW_ALARM_SEARCH     (0xec)
/*
 * thermometer functions
 * send them without bus reset!
 */
// start themperature reading
#define OW_CONVERT_T         (0x44)
// write critical temperature to device's RAM
#define OW_SCRATCHPAD        (0x4e)
// read whole device flash
#define OW_READ_SCRATCHPAD   (0xbe)
// copy critical themperature from device's RAM to its EEPROM
#define OW_COPY_SCRATCHPAD   (0x48)
// copy critical themperature from EEPROM to RAM (when power on this operation runs automatically)
#define OW_RECALL_E2         (0xb8)
// check whether there is devices wich power up from bus
#define OW_READ_POWER_SUPPLY (0xb4)

/*
 * RAM register:
 * 0 - themperature LSB
 * 1 - themperature MSB (all higher bits are sign)
 * 2 - T_H
 * 3 - T_L
 * 4 - B20: Configuration register (only bits 6/5 valid: 9..12 bits resolution); 0xff for S20
 * 5 - 0xff (reserved)
 * 6 - (reserved for B20); S20: COUNT_REMAIN (0x0c)
 * 7 - COUNT PER DEGR (0x10)
 * 8 - CRC
 *
 * To identify S20/B20 use value of confuguration register: its MSbit is 0
 */

typedef enum{
	OW_MODE_OFF,        // sleeping
	OW_MODE_TRANSMIT_N, // transmit N bytes
	OW_MODE_RECEIVE_N,  // receive N bytes
	OW_MODE_RESET       // reset bus
} OW_modes;

typedef struct{
	uint8_t is_free;
	uint8_t ROM_bytes[8];
} ow_ROM;

#define EEPROM_MAGICK  (0x1234)

// there's only 128 bytes of EEPROM on STM8S003!!!
// so we have not more than 14 sensors!
#define MAX_SENSORS (14)

typedef struct{
	uint16_t magick;
	ow_ROM ROMs[MAX_SENSORS];
} eeprom_data;

extern uint8_t ROM[];
extern eeprom_data *saved_data;

extern volatile uint8_t  ow_data;          // byte to send/receive
extern volatile uint8_t  onewire_tick_ctr; // tick counter
extern volatile uint8_t  is_receiver;
extern volatile uint16_t onewire_gotlen;   // last captured value
extern uint8_t ow_data_array[];
extern uint8_t ow_byte_ctr;
extern volatile OW_modes ow_mode;

extern void (*ow_process_resdata)();

void onewire_setup();
uint8_t onewire_reset();
void onewire_send_byte(uint8_t byte);
void onewire_wait_for_receive();
void process_onewire();
void onewire_receive_bytes(uint8_t N);
void onewire_send_bytes(uint8_t N);

long gettemp();

void eeprom_default_setup();
uint8_t erase_saved_ROM(uint8_t num);
uint8_t store_ROM();

// ++++++++++++++++++++++++++++++++++
void TIM2_Config(void);

#endif // __ONEWIRE_H__
