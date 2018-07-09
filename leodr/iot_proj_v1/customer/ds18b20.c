/*
 * Copyright (C) 2009-2018  Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     Driver
 * @{
 *
 * @file		ds18b20.c
 * @brief       DS18B20
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

#include "onewire.h"
#include <ds18b20.h>

volatile unsigned long Global_time = 0L; // global time in ms
uint16_t paused_val = 500; // interval between LED flashing
volatile uint8_t waitforread = 0;
uint8_t matchROM = 0,         // scan all stored ROMs
starting_val = 0,        // starting ROM number for next portion of scan
delete_notexistant = 0;  // delete all ROMs that not exists

void show_ROM(uint8_t *ptr){
	char i;
	for(i = 7; i > -1; i--){
		if(i < 7) printf("%c",',');
		printf("%x",ptr[i]);
	}
	printf("\n");
	ow_process_resdata = NULL;
}

void show_last_ROM(){
	char i;
	for(i = 7; i > -1; i--)
		ROM[i] = ow_data_array[i];
	show_ROM(ROM);
}

void show_stored_ROMs(){
	uint8_t i;
	for(i = 0; i < MAX_SENSORS; i++)
		if((saved_data->ROMs[i]).is_free == 0)
			show_ROM((saved_data->ROMs[i]).ROM_bytes);
}

void send_read_seq(){
//	printf("send_read_seq()\n");
	onewire_receive_bytes(9);
	ow_process_resdata = show_received_data;
}

void read_next_sensor(){
	uint8_t i;
	uint8_t *rom = NULL;
	if(!onewire_reset()) return;
	for(;starting_val < MAX_SENSORS; starting_val++)
		if((saved_data->ROMs[starting_val]).is_free == 0){
			rom = (saved_data->ROMs[starting_val]).ROM_bytes;
			starting_val++;
			break;
		}
	if(!rom){
		ow_process_resdata = NULL;
		delete_notexistant = 0;
		return; // all done
	}
	ow_data_array[0] = OW_READ_SCRATCHPAD;
	ow_process_resdata = send_read_seq;
	for(i = 0; i < 8; i++){
		ow_data_array[i+1] = rom[i];
		//if(i) printf("%c",',');
		//printf("%x",rom[i]);
	}
	ow_data_array[9] = OW_MATCH_ROM;
	onewire_send_bytes(10);
}

// show received scrtchpad
void show_received_data(){
//	char i;
	long L;
/*
	for(i = 8; i > -1; i--){
		if(i < 8) printf("%c",',');
		printf("%x",ow_data_array[i]);
	}
*/
	//viv- print_long((long)starting_val);
	printf(": ");
	ow_process_resdata = NULL;
	L = gettemp();
	if(L == ERR_TEMP_VAL){
		printf("no such device");
		if(delete_notexistant){
			printf(", ");
			if(!erase_saved_ROM(starting_val-1)) printf("can't ");
			printf("delete");
		}
	}else{
		//viv- print_long(gettemp());
		printf("/10 degr.C");
	}
	printf("%c",'\n');
	if(matchROM) // read next value if we have several sensors
		read_next_sensor();
}

void wait_reading(){
//	printf("wait_reading()\n");
	if(ow_data_array[0] == 0xff){ // the conversion is done!
		waitforread = 1;
		ow_process_resdata = NULL;
	}else{
		onewire_receive_bytes(1); // send read seq waiting for end of conversion
	}
}

void start_temp_reading(){
	if(!onewire_reset()){
		printf("no devices found!");
		return;
	}
	ow_data_array[0] = OW_CONVERT_T;
	ow_data_array[1] = OW_SKIP_ROM;
	ow_process_resdata = wait_reading;
    printf("onewire_send_bytes\n");
	onewire_send_bytes(2);
}
	
void DS18B20_init() {
	
	onewire_setup();
	eeprom_default_setup();
	
}

void DS18B20_process() {
// Loop
	unsigned long T = 0L, Tow = 0L;
//	int Ival;
	uint8_t rb;
		
	do{
		if((Global_time - T > paused_val) || (T > Global_time)){
			T = Global_time;
			//PORT(LED_PORT, ODR) ^= LED_PIN; // blink on-board LED
		}
		if(Global_time != Tow){ // process every byte not frequently than once per 1ms
			Tow = Global_time;
			process_onewire();
		}
		//viv- if(UART_read_byte(&rb)){ // buffer isn't empty
		if(0){ // buffer isn't empty	
			
			switch(rb){
				case 'h': // help
				case 'H': 
				{
					printf(" \
					\nPROTO:\n+/-\tLED period\nS/s\tset/get Mspeed\n" \
					"r: reset 1-wire\n" \
					"w: read temper\n" \
					"D: delete not existant ROMs (only for next reading cycle)\n" \
					"R: read ROM\n" \
					//"M: match ROM / not match ROM\n" \
					"S: store last readed ROM\n" \
					"Z: show all stored ROMs\n" \
					"W: read temperatures for all stored ROMs\n"
					);
				}
				break;
				case '+':
					paused_val += 100;
					if(paused_val > 10000)
						paused_val = 500; // but not more than 10s
				break;
				case '-':
					paused_val -= 100;
					if(paused_val < 500)  // but not less than 0.5s
						paused_val = 500;
				break;
				case 'r':
					if(!onewire_reset()) printf("not ");
					printf("found 1-wire devices (echo len: ");
					//viv- print_long((long)onewire_gotlen);
					printf("\n");
				break;
				case 'w':
					start_temp_reading();
				break;
				case 'D':
					delete_notexistant = 1;
				break;
				case 'R':
					if(onewire_reset()){
						onewire_send_byte(OW_READ_ROM);
						while(OW_BUSY);
						ow_process_resdata = show_last_ROM;
						onewire_receive_bytes(8);
					}
				break;
			//	case 'M':
			//		matchROM = !matchROM;
			//	break;
				case 'S':
					printf("storing last ROM ");
					if(!store_ROM()) printf("fails\n");
					else printf("done\n");
				break;
				case 'Z':
					show_stored_ROMs();
				break;
				case 'W':
					matchROM = 1;
					start_temp_reading();
				break;
			}
		}
		if(waitforread){
			if(onewire_reset()){
				ow_process_resdata = send_read_seq;
				waitforread = 0;
				if(!matchROM){
						ow_data_array[0] = OW_READ_SCRATCHPAD;
					ow_data_array[1] = OW_SKIP_ROM;
					onewire_send_bytes(2);
				}else{
					starting_val = 0;
					read_next_sensor();
				}
			}else printf("error reseting!");
		}
	}while(1);

}	

/* EOF ds18b20.c */
