/*
 * Copyright (C) 2009-2018  Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     Main
 * @{
 *
 * @file		main.c
 * @brief       Send packet measur
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

#include "mmsg.h"

#define V_REF 3.3
/*
 * Variables
 */
volatile uint8_t flag_new_meter_done;
meter_pack_t   measure_pack;
uint8_t result;


/*
 * Redirect stdout to UART
 */
int putchar(int c)
{
    uart_write(c);
    crc8_addbyte(c);
    return 0;
}


/*
 * Main loop Send packet 
 */
void main()
{
	static uint16_t cnt = 0;	
	/* Config UART */ 
	uart_init();
	
	/* Config ADC */ 
	ADC_init();
    
    /* Config TM1637 */
    TM1637_init();
    
    /* Config DS18B20 */
    DS18B20_init();
    
    /* Loop */ 
    while (1) {
        printf("cnt=%i; %s",cnt++,"Test protocol mmsg\n");

        {
        uint16_t val = ADC_read();
        float voltage = (V_REF / 1024.0) * val * 1000;
        printf("Channel3: %d mV\n", (uint16_t) voltage);
		}
        
        TM1637_process();
            
        measure_pack.Freque += 2;
        measure_pack.IntDegC += 1;
        measure_pack.adcDVCC += 11;
        measure_pack.adcDeg   += 33;
        measure_pack.adcExtVolt += 2;
        measure_pack.adcMeasu   += 4;
        measure_pack.Tperiod += 1;
        measure_pack.flags_meter = 0x55;
         
        Send_Packet(MK_PACKET);
        Send_Packet(AD_PACKET);
        Send_Packet(TS_PACKET);
        Send_Packet(FS_PACKET);
        Send_Packet(EM_PACKET);

        delay_ms(1000);
    }
}
