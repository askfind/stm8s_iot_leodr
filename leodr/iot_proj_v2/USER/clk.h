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
 * @file		clk.h
 * @brief       
 *
 * @author       Vladimir Vasilyev <askfind@ya.ru>
 * 
 * <a href="Умный кремний, www.smartsilicon.ru">http://smartsilicon.ru/</a> 
 * @}
 */

#ifndef CLK_H
#define CLK_H

#ifdef __cplusplus
extern "C" {
#endif

#define  Global_time (100) 

/**
 * @name    
 * @{
 */
extern void CLK_Config(void);
extern void CLK_CCO_Output_Config(void);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CLK_H */
/** @} */
