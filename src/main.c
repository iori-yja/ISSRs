/*
    FreeRTOS V6.0.1 - Copyright (C) 2009 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS eBook                                  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public 
    License and the FreeRTOS license exception along with FreeRTOS; if not it 
    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the standard demo application tasks.
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
*
 */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Demo app includes. */
#include "BlockQ.h"
#include "death.h"
#include "blocktim.h"
#include "flash.h"
#include "partest.h"
#include "GenQTest.h"
#include "QPeek.h"
#include "dynamic.h"
#include <string.h>
#include <stdio.h>
#include "LPC23xx.h"
#include "target.h"
#include "i2cErr.h"
#include "adc.h"
static void UARTint		( void );
static int  modechecker		( void );
static void vLedTask 		( void *pvParameters );
static void vLed2Task 		( void *pvParameters );
static void vi2c		( void );
static void ISSR 		( void *pvParameters );
static void vHPYTask		( void *pvParameters );
static void vADcTask		( void *pvParameters );
static void vrightwheelTask	( void *pvParameters );
static void vleftwheelTask	( void *pvParameters );
void vValueTask			( void *pvParameters );
void vtrsTask			( void *pvParameters );

/* Demo application definitions. */
#define mainQUEUE_SIZE						( 3 )
#define mainCHECK_DELAY						( ( portTickType ) 5000 / portTICK_RATE_MS )
#define mainBASIC_WEB_STACK_SIZE            ( configMINIMAL_STACK_SIZE * 6 )

/* Task priorities. */
#define mainQUEUE_POLL_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainFLASH_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainCREATOR_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainGEN_QUEUE_TASK_PRIORITY			( tskIDLE_PRIORITY ) 
#define HIGHpriority						( tskIDLE_PRIORITY + 4 )
/* Configure the hardware as required by the demo. */
static void prvSetupHardware( void );
//int rightCycle;
//int leftCycle;
int i2cErr;
//int verti,horizoni;
int i2cbuff[6];
/* The queue used to send messages to the LCD task. */
//xQueueHandle xLCDQueue;
int left;
int right;
xSemaphoreHandle Mutex;

unsigned int ADC_Read (unsigned int ch )
{
  unsigned int i;
    AD0CR  = (0x00200400 | ( 1 << ch ));	// Init ADC (Pclk = 18MHz) and select channel. Sampling rate = 4.5MHz
    AD0CR |= 0x01000000;		// Start A/D Conversion
    do
    {
    switch(ch)
    {
    case 0:
        i = AD0DR0;	// Read A/D Data Register
 	break;
    case 1:
    	i = AD0DR1;
	break;
    case 2:
    	i = AD0DR2;
	break;
    case 3:
    	i = AD0DR3;
	break;
    case 4:
    	i = AD0DR4;
	break;
    case 5:
    	i = AD0DR5;
	break;
    case 6:
    	i = AD0DR6;
	break;
    case 7:
    	i = AD0DR7;
    	break;
    	}
    	
       } while ((i & 0x80000000) == 0);	// Wait for end of A/D Conversion
    return (i >> 6) & 0x03FF;		// bit 6:15 is 10 bit AD value
}

/*-----------------------------------------------------------*/
int main( void )
{
	TargetResetInit();//
	GPIOResetInit();
	UARTint();
	FIO2PIN1 = 0x2;
	Mutex = xSemaphoreCreateMutex();
 	printf(" world!%d:%d\n",pdTRUE,pdFALSE);
	 if( Mutex != NULL ){
		xTaskCreate( vLedTask,  ( signed portCHAR * )"LED" , configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY-1, NULL );
		/* Start2csender the tasks defined within this file/specific to this demo. */
		xTaskCreate( vi2c,  ( signed portCHAR * )"LD" , configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
		xTaskCreate( ISSR,  ( signed portCHAR * )"ITS" , configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
		FIO2CLR1 = 0xFF;
		vTaskStartScheduler();
		}
	else printf("F");
	while(1);
	return 0;
}
//*******************************************************************************************************
//	-----------------------------In main initializer----------------------------^^
/*******UART initialize***************/
void UARTint( void )
{
	DWORD Fdiv,i=0;

    	U0LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
    	Fdiv = ( Fpclk / 16 ) / UART_BAUD ;	/*baud rate */
    	U0DLM = Fdiv / 256;
    	U0DLL = Fdiv % 256;
		U0LCR = 0x03;		/* DLAB = 0 */
    	U0FCR = 0x07;		/* Enable and reset TX and RX FIFO. */
	
	printf("Hello,");
	}

//---------------------------------------

/*-----------------------------------------------------------*/

//*******************************************************************************************************
//      ----------------------------------- Routines -------------------------------
void getISSI( void )
{
int bitshift=0;
int il=0;
int prv;
int crr;
vTaskDelay( 1 / portTICK_RATE_MS );
	printf("start ISSI connection\n");

	while(1){
		printf("HandShake");
		il++;
		FIO2SET0 = 1;
		FIO2SET1 = 0x2;
		prv=FIO2PIN0&3;
		printf("up\t%x\n",prv);
		bitshift = (bitshift<<1)+(prv&2)>>1;
		vTaskDelay(1 / portTICK_RATE_MS);
		FIO2CLR0 = 1;
		FIO2CLR1 = 0x2;
		printf("prv=%x",prv);
		crr = FIO0PIN&3;
		printf("Cr=%d",crr);
		if(crr&2!=prv&2){
			printf("-%d\t%d\n",crr,il);
			break;
			}
		vTaskDelay(1 / portTICK_RATE_MS);
		printf("Delaying%d",0.01 / portTICK_RATE_MS);
		}
	}

//*******************************************************************************************************
//	-----------------------------------T A S K s.-------------------------------^^

void vApplicationTickHook( void )
{

}
/*-----------------------------------------------------------*/
void vADcTask( void *pvParameters )
{
unsigned int a, t;
unsigned int res[5];
	for(;;){
		for(a = 0; a <= 4; a++){
			res[a] = ADC_Read(a);
			}
		vTaskDelay(30 / portTICK_RATE_MS);
		}
}
/*-----------------------------------------------------------*/
void ISSR( void *pvParameters )
{
unsigned int tmpData;
	vTaskDelay(30 / portTICK_RATE_MS);
	while(!xSemaphoreTake( Mutex, 301 / portTICK_RATE_MS ));
	vTaskDelay(30 / portTICK_RATE_MS);
	FIO2CLR1 = 0xFF;
	while(1){
		getISSI();
		vTaskDelay(300 / portTICK_RATE_MS);
		}
}
/*-----------------------------------------------------------*/
unsigned int res[5];
int white, black;
//-----------------
void vValueTask( void *pvParameters )
{
int i;
unsigned int a, t;
	vTaskDelay(30 / portTICK_RATE_MS);
	for(white = 0, black = 0, i = 1;i <= 20; i++){
	for(a = 0; a <= 7; a++){
		res[a] = ADC_Read(a);
		printf("  %x....%4x\t", a, res[a] );
	}
	printf("\n");
	white += ((res[1]+res[3]+res[4])/3);
	black += (res[2]);
	}
//	white = (white / 20);
//	black = (black / 20);
//	printf("white %x, black %x", white, black);
//	write_ROM(0x1A, 32, white);		//(int subadr, int size, int Data)
//	write_ROM(0x2A, 32, black);		//(int subadr, int size, int Data)
	while(1){
		for(a = 0; a <= 4; a++){
			res[a] = ADC_Read(a);
			printf("%d\t", res[a]);
			vTaskDelay(3 / portTICK_RATE_MS);
		}
		printf("\n");
	}
}

void vtrsTask( void *pvParameters ){
	vTaskDelay(300 / portTICK_RATE_MS);
	for(;;){
	if((2*res[6])<=(white+black)){
	printf("migi");
	left = 	0;
	right = 10;
	while((2*res[2])<=(white+black)||((2*res[3])<=(white+black))||((2*res[4])<=(white+black)));
	left = 10;
	right = 10;
}
	if((2*res[4])<=((white+black))){
	printf("tyuou");
	left = 10;
	right = 0;
	while(((2*res[2])<=(white+black))||((2*res[3])<=(white+black))||((2*res[4])<=(white+black)));
	left = 10;
	right = 10;
}
//		vTaskDelay(30 / portTICK_RATE_MS);
	
}
}
//**           **GPIO TASK**          **
/*-----------------------------------------------------------*/
void vLedTask( void *pvParameters )
{
	GPIOInit(1, FAST_PORT, DIR_OUT, LED1_MASK );
	
	for(;;)
	{
		FIO1PIN ^= LED1_MASK;
		vTaskDelay(207 / portTICK_RATE_MS);
			vTaskSuspend(NULL);

	}

}

//----------------------------------------------------------
void vLed2Task( void *pvParameters )
{	
	for(;;)
	{
		FIO2PIN1 ^= 0x02;
		vTaskDelay(300 / portTICK_RATE_MS);
	}

}
//TODO 衝突検知　
/*-----------------------------------------------------------
		for(a = 0; a <= 4; a++){
			res[a] = ADC_Read(a);
			printf("  %x....%4x\t", a, res[a] );
		}

void vHPYTask( void *pvParameters )
{	
int phase = 0;
unsigned int Step[4] = {0x6, 0xC, 0x9, 0x3}

for(;;)
{
	if(){ 
	FIO4CLR0 = 0x0F;
	}
}

}


-----------------------------------------------------------*/
void vrightwheelTask( void *pvParameters)
{
	int phase;
	FIO4DIR0 = 0x0F;
	FIO4CLR0 = 0x0F;
	while(1){
	if (0!=right){
	switch(phase){
		case 0:
		FIO4PIN0 = 0x03;
		phase++;
		break;
		case 1:
		FIO4PIN0 = 0x09;
		phase++;		
		break;
		case 2:
		FIO4PIN0 = 0x0C;
		phase++;
		break;
		case 3:
		FIO4PIN0 = 0x06;
		default:
		phase=0;
		break;
		}
	vTaskDelay( right / portTICK_RATE_MS );
	}
	else 	vTaskDelay( 10 / portTICK_RATE_MS );

}
}
//============================
void vleftwheelTask( void *pvPatameters){	
	int phase;
	FIO4DIR1 = 0x0F;
	FIO4CLR1 = 0x0F;
	while(1){
	if (0!=left){
	switch(phase){
		case 0:
		FIO4PIN1 = 0x03;
		phase++;		
		break;
		case 1:
		FIO4PIN1 = 0x09;
		phase++;
		break;
		case 2:
		FIO4PIN1 = 0x0C;
		phase++;
		break;
		case 3:
		FIO4PIN1 = 0x06;
		default:
		phase = 0;		
		break;
		}
		vTaskDelay( left / portTICK_RATE_MS);
		}else 		vTaskDelay( left / portTICK_RATE_MS);
	}
}
