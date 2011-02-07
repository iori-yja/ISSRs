/*****************************************************************************
 *   target.c:  Target C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *   2009.04.10  ver 2.00    Adopted to LPC2388 CQ-FRK-NXPARM JUNK Board
 *
*****************************************************************************/
#include "LPC23xx.h"
#include "target.h"


/******************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a necessary 
**						place, change it as needed
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TargetInit(void)
{
    /* Add your codes here */
    i2enable();
    PCONP |= 0x00001000;
    return;
}

/******************************************************************************
** Function name:		GPIOResetInit
**
** Descriptions:		Initialize the target board before running the main() 
**				function; User may change it as needed, but may not 
**				deleted it.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void GPIOResetInit( void )
{
	/* Reset all GPIO pins to default: primary function */
    PINSEL0 = 0x0FA00050;
    PINSEL1 = 0x00154000;
    PINSEL2 = 0x00000000;
    PINSEL3 = 0xF0000000;
    PINSEL4 = 0x00000000;
    PINSEL5 = 0x00000000;
    PINSEL6 = 0x00000000;
    PINSEL7 = 0x00000000;
    PINSEL8 = 0x00000000;
    PINSEL9 = 0x00000000;
   PINSEL10 = 0x00000000;
    
     IODIR0 = 0x00000000;
     IODIR1 = 0x00000000;
     IOSET0 = 0x00000000;
     IOSET1 = 0x00000000;
    
    FIO0DIR = 0x00000000;
    FIO1DIR = 0x00000000;
    FIO2DIR = 0x00000201;
    FIO3DIR = 0x00000000;
    FIO4DIR = 0x0000FFFF;
    
	FIO0MASK = 0x00000000;
	FIO0PIN = 0x00000000;
	FIO0SET = 0x00000000;
	FIO0CLR = 0x00000000;
	
	FIO1MASK = 0x00000000;
	FIO1PIN = 0x00000000;
	FIO1SET = 0x00000000;
	FIO1CLR = 0x00000000;
	
	FIO2MASK = 0x00000000;
	FIO2PIN = 0x00000000;
	FIO2SET = 0x00000000;
	FIO2CLR = 0x00000000;

	FIO3MASK = 0x00000000;
	FIO3PIN = 0x00000000;
	FIO3SET = 0x00000000;
	FIO3CLR = 0x00000000;
	
	FIO4MASK = 0x00000000;
	FIO4PIN = 0x00000000;
	FIO4SET = 0x00000000;
	FIO4CLR = 0x00000000;
	
    return;        
}

/******************************************************************************
** Function name:		ConfigurePLL
**
** Descriptions:		Configure PLL switching to main OSC instead of IRC
**						at power up and wake up from power down. 
**						This routine is used in TargetResetInit() and those
**						examples using power down and wake up such as
**						USB suspend to resume, ethernet WOL, and power management
**						example
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ConfigurePLL ( void )
{
	DWORD MValue, NValue;

	if ( PLLSTAT & (1 << 25) )
    {
		PLLCON = 1;			/* Enable PLL, disconnected */
	 	PLLFEED = 0xaa;
		PLLFEED = 0x55;
    }

    PLLCON = 0;				/* Disable PLL, disconnected */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
    
	SCS |= 0x20;			/* Enable main OSC */
    while( !(SCS & 0x40) );	/* Wait until main OSC is usable */

    CLKSRCSEL = 0x1;		/* select main OSC, 12MHz, as the PLL clock source */
	
		  
	PLLCFG = (PLL_NValue << 16) | PLL_MValue ;
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	PLLCON = 1;				/* Enable PLL */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	

    CCLKCFG = CCLKDivValue;	        /* Fcore = 72 MHz */
#if USE_USB
    USBCLKCFG = USBCLKDivValue;		/* usbclk = 48 MHz */
#endif

    while ( ((PLLSTAT & (1 << 26)) == 0) );	/* Check lock bit status */
    
    MValue = PLLSTAT & 0x00007FFF;
    NValue = (PLLSTAT & 0x00FF0000) >> 16;
    while ((MValue != PLL_MValue) && ( NValue != PLL_NValue) );

    PLLCON = 3;				/* enable and connect */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
	while ( ((PLLSTAT & (1 << 25)) == 0) );	/* Check connect bit status */


	return;
}

/******************************************************************************
** Function name:		TargetResetInit
**
** Descriptions:		Initialize the target board before running the main() 
**						function; User may change it as needed, but may not 
**						deleted it.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TargetResetInit(void)
{

// mthomas
#if 0
#ifdef __DEBUG_RAM    
    MEMMAP = 0x2;			/* remap to internal RAM */
#endif

#ifdef __DEBUG_FLASH    
    MEMMAP = 0x1;			/* remap to internal flash */
#endif
#endif

#ifdef __DEBUG_RAM    
    MEMMAP = 0x2;			/* remap to internal RAM */
#else
    MEMMAP = 0x1;			/* remap to internal flash */
#endif 


#if USE_USB
	PCONP |= 0x80000000;		/* Turn On USB PCLK */
#endif
	/* Configure PLL, switch from IRC to Main OSC */
	ConfigurePLL();

  /* Set system timers for each component */
#if (Fpclk / (Fcclk / 4)) == 1
    PCLKSEL0 = 0x00000000;	/* PCLK is 1/4 CCLK */
    PCLKSEL1 = 0x00000000;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
    PCLKSEL0 = 0xAAAAAAAA;	/* PCLK is 1/2 CCLK */
    PCLKSEL1 = 0xAAAAAAAA;	 
#endif
#if (Fpclk / (Fcclk / 4)) == 4
    PCLKSEL0 = 0x55555555;	/* PCLK is the same as CCLK */
    PCLKSEL1 = 0x55555555;	
#endif

    /* Set memory accelerater module*/
    MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
#if Fcclk < 40000000
    MAMTIM = 2;
#else
    MAMTIM = 3;
#endif
#endif
    MAMCR = 2;
	
	init_VIC();
	
	TargetInit();
	
    return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
