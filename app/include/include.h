#ifndef __INCLUDE__H
#define __INCLUDE__H


#include   <stdio.h>
#include   <string.h>
#include   <stdlib.h>
#include   <stdarg.h>


#include   "stm32f10x.h"
#include   "stm32f10x_conf.h"
#include   "stm32f10x_it.h"

#include   "os_cpu.h"
#include   "os_cfg.h"
#include   "ucos_ii.h"


#include   "bsp.h"
#include   "hal_com.h"
#include   "globals.h"
#include   "hal_rs485.h"
#include   "ring_buf.h"
#include   "app_instrument.h"
#include   "crc8_16.h"
#include   "hal_tim.h"

#define     APP_DEBUG                                  1
#if         APP_DEBUG  > 0
#define     APP_DBG_USED
#ifdef      APP_DBG_USED
void        App_Printf(char *format, ...);
#endif
#define 	  TRACE  App_Printf
#else      
#define 	  TRACE(...)
#endif 




/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/
#define  Task_INSTRUMENT_PRIO					3
#define  APP_TASK_START_PRIO 	        10
#define  MUTEX_PRIO						        1
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE		      128
#define  Task_INSTRUMENT_STK_SIZE				  1024


void  Delay (uint32_t nCount);
#endif //__INCLUDE__H
