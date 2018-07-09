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
 * @file		main.c
 * @brief       STM8S-IOT Loop
 *
 * @author       Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru/</a> 
 * @}
 */

#include "stm8s.h"

#include "stm8s_iot_config.h"
#include "clk.h"
#include "leds.h"
#include "uart.h"
#include "common.h"

void assert_failed(u8* file, u32 line);

static uint8_t a = 0;

static void delay(uint32_t t)
{
  while(t--);
}

/* 	-------------------------------
 *	STM8S-IOT Loop
 *	-------------------------------  
 */
main()
{
	uint8_t a;
	uint8_t state = 0x00;	
  
	
    if( a & 0x01 ) {
		__asm__("NOP");
	}
	else {
		__asm__("NOP");
	}	 
	//TestOptim();
  
	/*
	* Initialize LED
	*/
	LED_Conf();	
	LED_Off();
	
	/* 
	* Clock control (CLK)
	*/
	CLK_Config();  
	CLK_CCO_Output_Config();	
  
	/* 
	* UART1 init
	*/
	UART1_Config();

	/* 
	* 1-WIRE init
	*/
	
	/**
	 *  Loop
	 */    
	while (1)
	{
		LED_Togle();	/*  Togle LED */
		delay(20000);
	}
}

void assert_failed(u8* file, u32 line) {
  file += 0;
  line += 0;
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }  
}

/* End main() ---------------------- */
