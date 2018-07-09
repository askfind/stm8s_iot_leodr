/*
 * Copyright (C) 2018	Vladimir Vasilyev
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     Protocol 
 * @{
 *
 * @file		mmsg.h
 * @brief   	Protocol mmsg
 *
 * @author      Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru</a> 
 */
#ifndef MMSG_H
#define MMSG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#define BUF_FIELD_SIZE 9

enum type_packet_enum {
     EM_PACKET	= 0,     
     FS_PACKET	= 1,
     TS_PACKET	= 2,
     AD_PACKET	= 3,
     MK_PACKET	= 4,     
};

typedef struct {
    // Flags meter
    volatile uint8_t	flags_meter;
    // Results meter
    volatile uint16_t	adcMeasu;
    uint16_t  			adcDVCC;
    uint16_t  			adcDeg;
    uint16_t  			adcExtVolt;
    uint32_t			IntDegC;
    volatile uint32_t 	Tperiod;
    volatile uint32_t 	Freque;
    uint16_t 			capLoTCnt_cur;
    uint16_t 			capHiTCnt_cur;
    uint16_t 			capLoTCnt_old;
    uint16_t 			capHiTCnt_old;
} meter_pack_t;

enum state_msg_e {
    ST_MSG_IDLE      = 0,
    ST_MSG_BEG       = 1,
    ST_MSG_END       = 2,
    ST_MSG_GGA       = 3,
    ST_MSG_VTG       = 4,
    ST_MSG_READ_CRC8 = 5,
};

enum state_chekcsum_e {
    ST_CHECKSUM_START = 0,
    ST_CHECKSUM_ADD   = 1,
    ST_CHECKSUM_END   = 2,
};

enum state_field_e {
    ST_FL_READ       = 0,
    ST_FL_PARSE      = 1,
    ST_FL_CLEAR      = 2,
};

/*
*********************************************************************************************************
*                        FUNCTIONS PROTOTYPES
*********************************************************************************************************
*/
void crc8_clear(void);
void crc8_addbyte(uint8_t data);
uint8_t crc8_getcrc(void);

void Sent_Packet(uint8_t type_pack);

#ifdef __cplusplus
}
#endif

#endif /* MMSG_H */
/** @} */
