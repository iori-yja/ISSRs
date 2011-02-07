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

/*-----------------------------------------
				includes
-------------------------------------------*/
#include "misc.h"
/* check header file version for fool proof */
#if __MISC_H != 0x0003
#error "header file version is not correspond!"
#endif

/*-----------------------------------------
		   Common Basis Functions
-------------------------------------------*/
inline void __NOP(void)
{
	__asm__ __volatile__("nop"); 
}

void time_waste(volatile uint32_t dv)
{
	volatile uint32_t cnt;
	for (cnt=0; cnt<dv ; cnt++ ) { ; }
}

void _delay_us(volatile uint32_t us)
{
	while(--us) { __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();}
}

/*-----------------------------------------
		Bare-Metal Only Functions
-------------------------------------------*/
#ifndef INC_FREERTOS_H
void _delay_ms(volatile uint32_t dv)
{
	waiter = dv;
	/* reset_timer(TIMER_1); */
	enable_timer(TIMER_1);
	while(waiter);
	disable_timer(TIMER_1);
}

void init_delay()
{
	init_timer(TIMER_1,DELAY_MSEC);
}
#endif

/*-----------------------------------------
		Support Functions via STDIO
-------------------------------------------*/
void dump(uint32_t adr,uint32_t size)
{
	uint8_t *ad,data[16];
	int	i,j,k;

	(size%16)?(k=size/16+1):(k=size/16);
	DBG_print("\n");
	for(j=0,ad=(uint8_t*)adr;j<k;j++){
		DBG_print("%08X",(unsigned int)ad);
		for(i=0;i<16;i++,ad++)
			DBG_print(" %02X",data[i]=*ad);
		DBG_putch(' ');
		for(i=0;i<16;i++){
			(data[i]>=0x20 && data[i]<0x80)?DBG_putch(data[i]):DBG_putch('.');
		}
        DBG_print("\n");
	}
}
