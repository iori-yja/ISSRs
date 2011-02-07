/*****************************************************************************
 *   target.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.20  ver 1.00    Prelimnary version, first Release
 *   2009.08.04  ver 2.00    Nemui Added Some
 *
******************************************************************************/
#ifndef __TARGET_H 
#define __TARGET_H

#ifdef __cplusplus
   extern "C" {
#endif

#include <stdio.h>			/* stdio   */
#include <stdlib.h>			/* strings */
#include <string.h>			/* memcpy  */
#include <inttypes.h>		/* types   */
#include <stdint.h>
#include "LPC23xx.h"		/* system */
#include "type.h"			/* system */
#include "irq.h"			/* system */
//#include "swi.h"			/* system */
//#include "timer.h"			/* system */ //due to webserver task.
#include "fio.h"			/* system */
#include "uart.h"			/* system */
#include "rtc.h"			/* system */
#include "misc.h"			/* system */
#include "i2c.h"
#include "adc.h"
/* Board Dependent ! */
#define KEIL_BOARD_LPC23XX         0
#define EA_BOARD_LPC24XX           0
#define OLIMEX_BOARD_LPC2378_EK    1

/* LED1 (LPC2388 CQ-FRK-NXPARM JUNK Board LED as P1.18) */
#define LED1_FIOPIN  FIO1PIN
#define LED1_FIOSET  FIO1SET
#define LED1_FIOCLR  FIO1CLR
#define LED1_MASK    (1UL<<18)
/* Board Dependent ! */

/* If USB device is used, the CCLK setting needs to be 57.6Mhz, CCO will be 288Mhz
to get precise USB clock 48Mhz. If USB is not used, you set any clock you want
based on the table below. If you want to use USB, change "define USE_USB" from 0 to 1 */
 
#define	USE_USB					1

//This segment should not be modified
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/* PLL Setting Table Matrix */
/* 	
	Main Osc.	CCLKCFG	Fcco		Fcclk 		M 	N
	12Mhz		29		300Mhz		10Mhz		24	1
	12Mhz		35		360Mhz		10Mhz		14	0					
	12Mhz		27		336Mhz		12Mhz		13	0
	12Mhz		14		300Mhz		20Mhz		24	1		
	12Mhz		17		360Mhz		20Mhz		14	0
	12Mhz		13		336Mhz		24Mhz		13	0
	12Mhz		11		300Mhz		25Mhz		24	1   
	12Mhz		9		300Mhz		30Mhz		24	1
	12Mhz		11		360Mhz		30Mhz		14	0
	12Mhz		9		320Mhz		32Mhz		39	2
	12Mhz		9		350Mhz		35Mhz		174	11
	12Mhz		7		312Mhz		39Mhz		12	0
 	12Mhz		8		360Mhz		40Mhz		14	0 
	12Mhz		7		360Mhz		45Mhz		14	0
	12Mhz		6		336Mhz		48Mhz		13	0  
	12Mhz		5		300Mhz		50Mhz		24	1
	12Mhz		5		312Mhz		52Mhz		12	0
	12Mhz		5		336Mhz		56Mhz		13	0		
 	12Mhz		4		300Mhz		60Mhz		24	1		
  	12Mhz		4		320Mhz		64Mhz		39	2
	12Mhz		4		350Mhz		70Mhz		174	11
	12Mhz		4		360Mhz		72Mhz		14	0		
	12Mhz		3		300Mhz		75Mhz		24	1
	12Mhz		3		312Mhz		78Mhz		12	0  
	12Mhz		3		320Mhz		80Mhz		39	2
	12Mhz		3		336Mhz		84Mhz		13	0 
*/


#if USE_USB		/* 1 is USB, 0 is non-USB related */
/* Fcck = 72Mhz, Fosc = 288Mhz, and USB 48Mhz */
#define PLL_MValue			11UL   	/* MSEL value "12" but should stored PLLCFG is (12 - 1)	*/
#define PLL_NValue			0UL		/* NSEL value "1"  but should stored PLLCFG is (1 - 1)	*/
#define CCLKDivValue		3UL     /* Fosc is divides by CCLK_DIV+1 to make72MHz			*/
#define USBCLKDivValue		5UL     /* Fosc is divides by USBCLKDivValue+1 to make48MHz		*/

/* Fcck = 72Mhz, Fosc = 432Mhz, and USB 48Mhz */
/*
#define PLL_MValue			17UL  //MSEL value "18" but should stored PLLCFG is (18 - 1)
#define PLL_NValue			0UL   //NSEL value "1" but  should stored PLLCFG is (1 - 1).
#define CCLKDivValue		5     //Fosc is divides by CCLK_DIV+1 to make72MHz
#define USBCLKDivValue		8     //Fosc is divides by USBCLKDivValue+1 to make48MHz
*/
/* Fcck = 57.6Mhz, Fosc = 288Mhz, and USB 48Mhz */
/*
#define PLL_MValue			11
#define PLL_NValue			0
#define CCLKDivValue		4
#define USBCLKDivValue		5
*/

/* System configuration: Fosc, Fcclk, Fcco, Fpclk must be defined */
/* PLL input Crystal frequence range 4KHz~20MHz. */
#define Fosc	12000000
/* System frequence,should be less than 80MHz. */
#define Fcclk	72000000
#define Fcco	288000000
/* #define Fcco	432000000 */
/* #define Fcclk	57600000 */
#else

/* Fcck = 50Mhz, Fosc = 300Mhz, and USB 48Mhz */
#define PLL_MValue			24
#define PLL_NValue			1
#define CCLKDivValue		5
#define USBCLKDivValue		6

/* System configuration: Fosc, Fcclk, Fcco, Fpclk must be defined */
/* PLL input Crystal frequence range 4KHz~20MHz. */
#define Fosc	12000000
/* System frequence,should be less than 80MHz. */
#define Fcclk	50000000
#define Fcco	300000000

#endif

/* APB clock frequence , must be 1/2/4 multiples of ( Fcclk/4 ). */
/* If USB is enabled, the minimum APB must be greater than 16Mhz */ 
#if USE_USB
#define Fpclk	(Fcclk / 2)
#else
#define Fpclk	(Fcclk / 4)
#endif

/******************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a 
**				necessary place, change it as needed
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern void TargetInit(void);
extern void ConfigurePLL( void );
extern void TargetResetInit(void);
extern void GPIOResetInit(void);
#ifdef __cplusplus
   }
#endif
 
#endif /* end __TARGET_H */
/******************************************************************************
**                            End Of File
******************************************************************************/
