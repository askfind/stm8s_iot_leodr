/*
 * Copyright (C) 2009-2018  Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     Protocols
 * @{
 *
 * @file		mmsg.c
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

/*
 * Variables
 */
volatile uint8_t flag_new_meter_done;
meter_pack_t   measure_pack;
static uint8_t crc;
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
 * CRC8 clear
 */
void crc8_clear(void)
{
    crc = 0;
}

/*
 * CRC8 add byte
 */
void crc8_addbyte(uint8_t data)
{
    uint8_t	bit_counter;
    uint8_t	feedback_bit;

    bit_counter = 8;
    do {
        feedback_bit = (crc ^ data) & 0x01;
        if (feedback_bit == 0x01) {
            crc = crc ^ 0x18; /* 0X18 = X^8+X^5+X^4+X^0 */
        }
        crc = (crc >> 1) & 0x7F;
        if (feedback_bit == 0x01) {
            crc = crc | 0x80;
        }

        data = data >> 1;

        bit_counter--;

    } while (bit_counter > 0);
}

/*
 * CRC8 get
 */
uint8_t crc8_getcrc(void)
{
    return crc & 0xFF;
}

/*
*------------------------------------------------------------------------------
* Prints a packet meters 
*------------------------------------------------------------------------------
* Функция формирования и передачи сообщения с измерениями
*
*  Параметр:    Тип пакета
*
*  Результат:   Строка измерения с "XX:,,,#CR,LT"
*                                   MK - все измерения
*                                   FS - часота
*                                   TS - период
*                                   AD - аналоговые значения
*------------------------------------------------------------------------------
* Описание измерительных пакетов:
*
* MK - мультиметр:  "MK:Aa,Ab,Ac,F,T,N#CRC"+CR+LT
*                    Aa - первое аналогое значение
*                    Ab - второе аналогое значение
*                    Ac - третье аналогое значение
*                    F  - частота
*                    T  - период
*                    N  - номер измерение
*
*
* FS - период:      "FS:F#CRC"+CR+LT
*                   значение  цифры целого
*
* TS - период:      "TS:T#CRC"+CR+LT
*                   значение  цифры целого
*
* AD - аналоговые: "AD:A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11#CRC"+CR+LT
*                  если аналоговое значение не используется, то пусто.
*                  значние до четырех цифр ( от "0" или "1024")
*
*------------------------------------------------------------------------------
*/
void Send_Packet(uint8_t type_pack)
{
	printf("%c",'*'); /* заголовок */
	switch (type_pack) {
		case MK_PACKET: {
			/* Сформировать пакет для передачи*/
			printf("%s","MK:"); /* заголовок */
	#if 1        
			printf("%d",measure_pack.adcDVCC);
			printf("%c",',');
			printf("%d",measure_pack.adcDeg);
			printf("%c",',');
			printf("%d",measure_pack.adcExtVolt);
			printf("%c",',');
			printf("%d",measure_pack.adcExtVolt);
			printf("%c",',');
			printf("%d",measure_pack.Freque);  /* 32-бит */
			printf("%c",',');
			printf("%d",measure_pack.Tperiod); /* 32-бит */
			printf("%c",'#');
			printf("%X",crc8_getcrc()); /* CRC */
	#endif
			printf("\n");
			}
		break;
		case TS_PACKET: {
			/* Сформировать пакет для передачи*/
			printf("%s","TS:"); /* заголовок */
			printf("%d",(uint16_t)measure_pack.Tperiod);  /* 32-бит */			
			printf("#%x\n",crc8_getcrc());
			}
		break;	        
		case AD_PACKET: {
			/* Сформировать пакет для передачи*/
			printf("%s","AD:"); /* заголовок */
			printf("%d",measure_pack.adcExtVolt);		
			printf("#%x\n",crc8_getcrc());
			}
		break;	
		case FS_PACKET: {
			printf("%s","FS:");
			printf("%d",(uint16_t)measure_pack.Freque);  /* 32-бит */
			printf("#%x\n",crc8_getcrc());
			}
		default: 
			/* Нет измерений " EM:#0xD6 " */
			printf("EM:0x%x,#%x\n",measure_pack.flags_meter,0xD6);
		break;
	}
	return;
}

/*
 * Main loop Send packet 
 */
void main()
{
	static uint16_t cnt = 0;	
	/* Config UART */ 
	uart_init();
    
    /* Loop */ 
    while (1) {
        printf("cnt=%i; %s",cnt++,"Test protocol mmsg\n");
        
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
