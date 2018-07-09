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
#include <stdint.h>
#include "ports_definition.h"
#include "interrupts.h"
#include "uart.h"
#include "onewire.h"


volatile unsigned long Global_time = 0L; // global time in ms
uint16_t paused_val = 500; // interval between LED flashing
volatile uint8_t waitforread = 0;
uint8_t matchROM = 0,         // scan all stored ROMs
starting_val = 0,        // starting ROM number for next portion of scan
delete_notexistant = 0;  // delete all ROMs that not exists

static void TestOptim(void) {

//----------------------------------
#if 0 //viv+ оптимизация uint32_t v
    uint32_t u = 0;

    uint16_t cnt_h = 0;
    uint16_t cnt_l = 0;

    //ver1
    u++;

    //ver2
    if( ++cnt_l == 0) cnt_h++;
    __asm__("NOP");

#endif

//----------------------
#if 1 //viv+ оптимизация
  { //viv+ test

union {
    struct {
      uint8_t m1:1 ;
      uint8_t m0:1 ;
    };
    uint8_t q;
} u;

struct {
  uint8_t b1:1 ;
  uint8_t b0:1 ;
} bit;


    uint8_t q = 0;

    uint32_t a = 0;
    uint32_t * pa = &a;

    uint8_t b = 0;
    uint16_t hw = 0;

    uint16_t c= 0;
    uint16_t * pc = &c;

    // q
    q = 1; // 2

    if( q & (1<<1) ) { //5
      __asm__("NOP");
    }


    // struct
    bit.b0 = 1; //3
    bit.b1 = 0; //3

    if(bit.b0) { //5
      __asm__("NOP");
    }

    if(!bit.b1) { //4
      __asm__("NOP");
    }

    a++; // 97 ccstep
    a += 1; // 97 ccstep
    *pa++; // 6 ccstep

    c++; // 6 ccstep
    c += 1; // 6 ccstep
    *pc++; //6 ccstep


    u.q = 2; // 2 ccstep

    u.m0 = 0; // 3 ccstep

    u.m0 = 1; // 3 ccstep

    if( u.m0 ) // 5 ccstep
    {
      __asm__("NOP");
    }

    if( b & (1<<1)) //// 5 ccstep
    {
      __asm__("NOP");
    }

    if( !(b & (1<<1))) // 4 ccstep
    {
      __asm__("NOP");
    }

    u.q = 2;    // 2 ccstep
    if( u.q == 2 ) // 3 ccstep
    {
      __asm__("NOP");
    }

    if( u.m0  ) // 5 ccstep
    {
      __asm__("NOP");
    }
    else {
      __asm__("nop");
    }

    if( u.m1  ) // 5 ccstep
    {
      __asm__("NOP");
    }
    else {
      __asm__("nop");
    }


    if( u.q == 2 ) // 3 ccstep
    {
      __asm__("NOP");
    }


    b |= (1<<1); // 3 ccstep
    hw |= (1<<2); // 7 ccstep

    __asm__("NOP");

    b &= (1<<1); // 3 ccstep
    hw &= (1<<2); // 7 ccstep

    __asm__("NOP");

    if( b & (1<<1)) // 4 ccstep
    {
      __asm__("NOP");
    }

    if( (b & (1<<1)) > 0 ) // 11 ccstep
    {
      __asm__("NOP");
    }

    __asm__("NOP");


    if( hw & (1<<1)) // 7 ccstep
    {
      __asm__("NOP");
    }

    if( (hw & (1<<1)) > 0 ) // 7 ccstep
    {
      __asm__("NOP");
    }

  } //test end
#endif
}

void show_ROM(uint8_t *ptr){
	char i;
	for(i = 7; i > -1; i--){
		if(i < 7) UART_send_byte(',');
		printUHEX(ptr[i]);
	}
	UART_send_byte('\n');
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

void show_received_data();
// ask to read N bytes after temper request
void send_read_seq(){
//	uart_write("send_read_seq()\n");
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
		//if(i) UART_send_byte(',');
		//printUHEX(rom[i]);
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
		if(i < 8) UART_send_byte(',');
		printUHEX(ow_data_array[i]);
	}
*/
	print_long((long)starting_val);
	uart_write(": ");
	ow_process_resdata = NULL;
	L = gettemp();
	if(L == ERR_TEMP_VAL){
		uart_write("no such device");
		if(delete_notexistant){
			uart_write(", ");
			if(!erase_saved_ROM(starting_val-1)) uart_write("can't ");
			uart_write("delete");
		}
	}else{
		print_long(gettemp());
		uart_write("/10 degr.C");
	}
	UART_send_byte('\n');
	if(matchROM) // read next value if we have several sensors
		read_next_sensor();
}

void wait_reading(){
//	uart_write("wait_reading()\n");
	if(ow_data_array[0] == 0xff){ // the conversion is done!
		waitforread = 1;
		ow_process_resdata = NULL;
	}else{
		onewire_receive_bytes(1); // send read seq waiting for end of conversion
	}
}

void start_temp_reading(){
	if(!onewire_reset()){
		uart_write("no devices found!");
		return;
	}
	ow_data_array[0] = OW_CONVERT_T;
	ow_data_array[1] = OW_SKIP_ROM;
	ow_process_resdata = wait_reading;
    uart_write("onewire_send_bytes\n");
	onewire_send_bytes(2);
}

int main() {
	unsigned long T = 0L, Tow = 0L;
//	int Ival;
	uint8_t rb;
	CFG_GCR |= 1; // disable SWIM
	// Configure clocking
	CLK_CKDIVR = 0; // F_HSI = 16MHz, f_CPU = 16MHz
// Test
       TestOptim();
// Timer 4 (8 bit) used as system tick timer
	// prescaler == 128 (2^7), Tfreq = 125kHz
	// period = 1ms, so ARR = 125
	TIM4_PSCR = 7;
	TIM4_ARR = 125;
	// interrupts: update
	TIM4_IER = TIM_IER_UIE;
	// auto-reload + interrupt on overflow + enable
	TIM4_CR1 = TIM_CR1_APRE | TIM_CR1_URS | TIM_CR1_CEN;

	// PC2 - PP output (on-board LED)
	PORT(LED_PORT, DDR)  |= LED_PIN;
	PORT(LED_PORT, CR1)  |= LED_PIN;

    // --------------
    
	uart_init();

	onewire_setup();

	eeprom_default_setup();

	enableInterrupts();


	
	// Loop
	do{
		if((Global_time - T > paused_val) || (T > Global_time)){
			T = Global_time;
			//PORT(LED_PORT, ODR) ^= LED_PIN; // blink on-board LED
		}
		if(Global_time != Tow){ // process every byte not frequently than once per 1ms
			Tow = Global_time;
			process_onewire();
		}
		if(UART_read_byte(&rb)){ // buffer isn't empty
			switch(rb){
				case 'h': // help
				case 'H':
					uart_write("\nPROTO:\n+/-\tLED period\nS/s\tset/get Mspeed\n"
					"r: reset 1-wire\n"
					"w: read temper\n"
					"D: delete not existant ROMs (only for next reading cycle)\n"
					"R: read ROM\n"
					//"M: match ROM / not match ROM\n"
					"S: store last readed ROM\n"
					"Z: show all stored ROMs\n"
					"W: read temperatures for all stored ROMs\n"
					);
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
					if(!onewire_reset()) uart_write("not ");
					uart_write("found 1-wire devices (echo len: ");
					print_long((long)onewire_gotlen);
					uart_write(")\n");
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
					uart_write("storing last ROM ");
					if(!store_ROM()) uart_write("fails\n");
					else uart_write("done\n");
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
			}else uart_write("error reseting!");
		}
	}while(1);
}


