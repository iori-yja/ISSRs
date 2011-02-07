/*****************************************************************************
 *   uart.h:  Header file for NXP LPC23xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
 ******************************************************************************/

/* modified by Martin Thomas (check for __irq makro) */

#ifndef __UART_H 
#define __UART_H

#include "LPC23xx.h"

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80


#define UART_BUFSIZE		256
/* XBee test uses UART_1 */
/* CQ-FRK-NXPARM  UART_0 via CP2102 */
/* printf function uses UART_NUM PORT! */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef UART_NUM
	#define UART_NUM	0
	#define MON_NUM 	1
#endif
#define UART_BAUD	115200UL
//#define gets(x,y)  UARTGets(UART_NUM,x,y)
//#define BUFFERING_UART

DWORD UARTInit( DWORD portNum, DWORD Baudrate );
#ifdef __irq
void UART0Handler( void ) __irq;
void UART1Handler( void ) __irq;
#else
void UART0Handler( void );
void UART1Handler( void );
#endif
void UARTSend( DWORD portNum, const BYTE *BufferPtr, DWORD Length );
void UARTPutch( DWORD portNum, BYTE charactor );
char UARTGetch( DWORD portNum );
char UARTGets(DWORD portNum,char *buf,DWORD bufsize);

#ifdef BUFFERING_UART
extern volatile DWORD UART0Count;
extern volatile BYTE  UART0Buffer[UART_BUFSIZE];
extern volatile DWORD UART1Count;
extern volatile BYTE  UART1Buffer[UART_BUFSIZE];
#endif

#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
