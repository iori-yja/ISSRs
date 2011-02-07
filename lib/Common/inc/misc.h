/********************************************************************************
* File Name          : misc.h
* Author             : Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
* Version            : V3.00
* Since              : 2009.05.14
* Description        : miscellaneous routine for LPC23xx uC.
*
*                    : 2009.05.14  V1.00    Prelimnary version, first Release.
*                    : 2009.06.29  V2.00    added systick timer using timer1.
*					 : 2009.07.22  V3.00    added FreeRTOS support.
* License			 : BSD License. See Copyright.txt
********************************************************************************/
#ifndef __MISC_H 
#define __MISC_H	0x0003

/* debug output definitiion */
#define DBG_print printf
#define DBG_putch putchar

/* Basis Inclusion */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

/* LPC23xx/24xx Peripheral Registers */
#include "LPC23xx.h"
/* Target Board Setting */
#include "target.h"

#ifdef USE_FreeRTOS_SUPPORT
/* FreeRTOS compatibility */
 #include "FreeRTOS.h"
 #include "task.h"
 #define _delay_ms(x)   	vTaskDelay(x/portTICK_RATE_MS)
 
#else
#include "timer.h"
/* Bare-Metal running */
 extern void _delay_ms(uint32_t ms);
 extern void init_delay(void);
 
#endif
extern void _delay_us(uint32_t us);
extern void __NOP(void);
extern void time_waste(uint32_t dv);
extern void dump(uint32_t adr,uint32_t size);

#endif /*__MISC_H */
