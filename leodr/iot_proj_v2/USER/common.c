/*
 * Copyright (C) 2018  Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     USER Library
 * @{
 *
 * @file		common.c
 * @brief       
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru/</a> 
 * @}
 */

#include "stm8s.h"
#include "uart.h"
#include "common.h"

//viv+ new
/* ------------------------------------------------------------------
*  code 207  uli2a
*  ------------------------------------------------------------------
*/
void uli2a(uint32_t num, uint16_t base, int16_t uc, uint8_t * bf)
{
	int16_t n=0;
	uint32_t d=1;
	while (num/d >= base)
		d*=base;		 
	while (d!=0) {
		int16_t dgt = num / d;
		num%=d;
		d/=base;
		if (n || dgt>0|| d==0) {
			*bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
			++n;
			}
		}
	*bf=0;
}

//viv~ old 
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
	UART_write((char*)&decimal_buff[ch+1]);
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
	UART_write(&decimal_buff[ch]);
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
	UART_write("\nERROR: ");
	UART_write(msg);
	UART_send_byte('\n');
}

uint8_t uint8_ttoHEX(uint8_t val){
	val &= 0x0f;
	if(val < 10) val += '0';
	else val += 'a' - 10;
	return val;
}

void printUHEX(uint8_t val){
	UART_write("0x");
	UART_send_byte(uint8_ttoHEX(val>>4)); // MSB
	UART_send_byte(uint8_ttoHEX(val));    // LSB
}

void TestOptim(void) {

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
#if 0 //viv+ оптимизация
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

/* EOF common.c */
