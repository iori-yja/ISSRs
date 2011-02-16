/****************************************************************************
This is i2c source for LPC23xx/24xx micro processor.
Author		:iorivr
History	:30th,Jan,2010 stared to write.
function	:i2c initialize, data sending, and data resieving.
NOTE		:It is NOT to eat.
		 distributed on LGPL license.
		 Also, I will adjust even for SCCB.

****************************************************************************
|\  | |""""| """|""" |"""" 
After 502page on user manual is very usefull. Please reference for it!!
| \ | |    |    |    |====	
|  \| |____|    |    |____
******************************************************************************/
#include"lpc23xx.h"
#include"target.h"
#include"i2c.h"
#include"i2cErr.h"
#include"semphr.h"
#include"OV7670userconfig.h"
extern xSemaphoreHandle Mutex;

#define  RESETREG SCCBByteWrite(0x1280);
/****************************************************************************
function name	:i2enable
about function:as name.
Problem	:
****************************************************************************/
void
i2enable ()
{
  PINMODE0 = 0x00000000;
}

/****************************************************************************
function name	:i2cmasterset
discribe	:the i2c periphrals which given number will be master
****************************************************************************/
int
i2cmasterset (int a)
{
  switch (a)
    {
    case 0:
      I20CONSET = 0x40;
      return 0;
      break;
    case 1:
      I21CONSET = 0x40;
      return 0;
      break;
    case 2:
      I22CONSET = 0x40;
      return 0;
      break;
    default:
      return 1;
    }
}

/****************************************************************************
function name	:i2cstart
discribe	:the i2c will start
****************************************************************************/
int
i2cstart (int a)
{
  switch (a)
    {
    case 0:
      I20CONSET = 0x60;
      while (I20STAT != 0x08);
      return 0;
      break;
    case 1:
      I21CONSET = 0x60;
      while (I21STAT != 0x08);
      return 0;
      break;
    case 2:
      I22CONSET = 0x60;
      while (I22STAT != 0x08);
//              printf("I2C now started\n");
      return 0;
      break;
    default:
      return 1;
    }
}

/****************************************************************************
function name	:i2crestart
discribe	:the i2c will restart
****************************************************************************/
int
i2crestart (int a)
{
  switch (a)
    {
    case 0:
      I20CONSET = 0x14;
      I20CONCLR = 0x08;
      I20CONSET = 0x60;
      while ((I20STAT != 0x10) && (I20STAT != 0x08));
      printf ("restarter report...%x", I20STAT);
      return 0;
      break;
    case 1:
      I21CONSET = 0x14;
      I21CONCLR = 0x08;
      I21CONSET = 0x60;
      while ((I21STAT != 0x10) && (I21STAT != 0x08));
      printf ("restarter report...%x", I21STAT);
      return 0;
      break;
    case 2:
      I22CONSET = 0x14;
      I22CONCLR = 0x08;
      I22CONSET = 0x60;
      while ((I22STAT != 0x10) && (I22STAT != 0x08));
      printf ("restarter report...%x", I22STAT);
      return 0;
      break;
    default:
      return 1;
    }
}

/****************************************************************************
function name	:i2stop
discribe	:the i2c will stop
****************************************************************************/
int
i2cstop (int a)
{
  switch (a)
    {
    case 2:
      I22CONSET = 0x50;
      I22CONCLR = 0x2C;
//              printf("I2C stopped\n");
      break;
    default:
      return 1;
    }
}

/*****************************************************************************
function name	:i2csender
dscribe		:i2c data and adress
parameter 	:bus_num,data
return value	:void
*****************************************************************************/
void
i2csender (int Continue, unsigned int Data, int Keta)
{
//int i2cStatus;
  if (Continue == 0)
    {
      I22DAT = Data;
      I22CONSET |= 0x04;
      I22CONCLR = 0x08;
//      printf("requesting%x",Data);
      while (I22STAT != 0x18 && I22STAT != 0x20);
      FIO2PIN1 = 0x2;
      if (I22STAT == 0x20)
	{
	  printf ("No such device%4x\n", Data);
	  i2cErr = 0x22;
	}
    }
  else
    {
      for (; Keta != 0; Keta -= 8)
	{
	  I22DAT = (Data & 0xFF);
	  I22CONCLR = 0x28;
	  while (I22STAT != 0x28);
//              printf("Data%4x::::\n",Data);
	  Data = Data >> 8;
	}
//      if(I22STAT==0x28)i2cErr = 0;
//      break;
    }
//      return i2cErr;
}

void
novorvisesender (int Continue, unsigned int Data, int Keta)
{
  if (Continue == 0)
    {
      I22DAT = Data;
      I22CONSET |= 0x04;
      I22CONCLR = 0x08;
      while (I22STAT != 0x18 && I22STAT != 0x20)
	printf (".");
      if (I22STAT == 0x20)
	{
	  printf ("No such device%4x\n", Data);
	  i2cErr = 0x22;
	}
    }
  else
    {
      for (; Keta != 0; Keta -= 8)
	{
	  I22DAT = (Data & 0xFF);
	  I22CONCLR = 0x28;
	  while (I22STAT != 0x28);
	  Data = Data >> 8;
	}
//                              if(I22STAT==0x28)i2cErr = 0;
//                              break;
    }
//      return i2cErr;
}

/***********************************************************************************************************************************
*/
int
tinyreader (int Keta, int Adr, int registernumber)
{
upper:
  I22DAT = (Adr + 1);
  I22CONSET |= 0x04;
  I22CONCLR = 0x08;
  while (I22STAT != 0x40 && I22STAT != 0x48);
  if (I22STAT == 0x48)
    {
      printf ("no acknowlege(%x)\n", I22STAT);
      i2crestart (2);
      goto upper;
    }
  if (I22STAT == 0x40);
  while (registernumber != (Keta + 1))
    {
      printf ("reading..  ");
      I22CONSET |= 0x04;
      I22CONCLR = 0x28;
      printf ("Wait\t");
      while (I22STAT != 0x50);
      printf ("%xreg\t%x", registernumber++, I22DAT);
      vTaskDelay (10 / portTICK_RATE_MS);

//                                      if(a==32)return;
    }

  I22CONCLR = 0x0C;
//      return i2cErr;
}

int
i2creader (int Keta, int Adr, int registernumber)
{
  char Loopy;
upper:
  I22DAT = (Adr + 1);
  I22CONSET |= 0x04;
  I22CONCLR = 0x08;
  printf ("Status is %x\n", I22STAT);
  while (I22STAT != 0x40 && I22STAT != 0x48)
    printf (".");
  if (I22STAT == 0x48)
    {
      printf ("no acknowlege(%x)\n", I22STAT);
      i2crestart (2);
      goto upper;
    }
  if (I22STAT == 0x40);
  printf ("Read begin[>  ");
  while (registernumber++ != (Keta + 1))
    {
      I22CONSET |= 0x04;
      I22CONCLR = 0x28;
      while (I22STAT != 0x50);
      vTaskDelay (10 / portTICK_RATE_MS);
      switch (registernumber % 4)
	{
	case 0:
	  Loopy = '|';
	  break;
	case 1:
	  Loopy = '/';
	  break;
	case 2:
	  Loopy = '-';
	  break;
	case 3:
	  Loopy = '\\';
	  break;
	}
      printf ("\b\b\b=>%c", Loopy);
      fflush (stdout);
      vTaskDelay (10 / portTICK_RATE_MS);
//                                      if(a==32)return;
    }
  printf ("done.....");
  fflush (stdout);
  I22CONCLR = 0x0C;
//      return i2cErr;
}

/*********************************************************************************************************************
*********************************************************************************************************************/
void
tinyread (int subadr, int size, int NumByte)
{
  i2cstart (2);
  i2cErr = 0;			//i want to read!b So, start.
  novorvisesender (0, 0x42, 8);	//Hey, ?
  if (i2cErr == 0x22)
    {
      printf ("fatal");
      i2cstop (2);
      return;
    }
  i2csender (1, subadr, 8);
  i2crestart (2);		//if you understand, restart,please.
  tinyreader (size, 0x42, NumByte);	//
  vTaskDelay (30 / portTICK_RATE_MS);
  i2cstop (2);
}

void
SCCBread (int subadr, int size, int NumByte)
{
  i2cstart (2);
//      vTaskDelay(300 / portTICK_RATE_MS);
  i2cErr = 0;			//i want to read!b So, start.
  i2csender (0, 0x42, 8);	//Hey, ?
  if (i2cErr == 0x22)
    {
      printf ("fatal");
      i2cstop (2);
      return;
    }
  i2csender (1, subadr, 8);
//      printf("Adress read request at%4x OK\n\n", subadr);     //0x03byte? 
  i2crestart (2);		//if you understand, restart,please.
  i2creader (size, 0x42, NumByte);	//
  i2cstop (2);
  printf ("Debu331g\n");
}

int
SCCBwrite (int subadr, int size, int Data)
{
  i2cstart (2);
//      vTaskDelay(1 / portTICK_RATE_MS);
  FIO2CLR1=2; 
  i2cErr = 0;
  i2csender (0, 0x42, 8);
  if (i2cErr == 0x22)
    {
      printf ("fatal");
      i2cstop (2);
      return -1;
    }
  i2csender (1, subadr, 8);	//3byte? 
  i2csender (1, Data, size);
  i2cstop (2);
  return 0;
}

int
SCCBByteWrite (int inp)
{
//      printf("%x,%x,%x",inp >> 8, 8, inp & 0xFF);
 return SCCBwrite ((inp >> 8 & 0xFF), 8, (inp & 0xFF)) ;
}

void
vi2c (void *pvParameters)
{
  int regpointer = 0;
  char Loopy;
  xSemaphoreTake (Mutex, portMAX_DELAY);
  i2cmasterset (2);
  I22SCLH = 80;
  I22SCLL = 180;
  printf ("Register Reset\n");
  vTaskDelay (300 / portTICK_RATE_MS);
  RESETREG
  vTaskDelay (200 / portTICK_RATE_MS);
  printf ("Setup begin[> ");
  fflush (stdout);
  for (regpointer = 0; regpointer != 129;)
    {
      if (SCCBByteWrite (OV7670[regpointer]) == -1) continue;
      regpointer++;
      	switch (regpointer % 4)
	  {
	  case 0:
	  Loopy = '|';
	  break;
	  case 1:
	  Loopy = '/';
	  break;
	  case 2:
	  Loopy = '-';
	  break;
	  case 3:
	  Loopy = '\\';
	  break;
	  }
	printf ("\b\b\b=>%c", Loopy);
	fflush (stdout);
      vTaskDelay (10 / portTICK_RATE_MS);
    }
  printf ("\rData transmition End!\n");
  fflush (stdout);
  SCCBread (0x00, 0xca, 0);
  vTaskDelay (200 / portTICK_RATE_MS);
  if (xSemaphoreGive (Mutex))
    FIO2PIN1 = 0x2;
//  vTaskResume ( g_issrhandle );
//  vTaskSuspend(NULL);
//  xTaskCreate (ISSR, (signed portCHAR *) "ITS", configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL);
  while (1);
}

int
ymzwrite0 (int frec, int adr)
{
  int highbyte;
  FIO4PIN = 0x00;
  FIO4PIN = adr;
  FIO4PIN = 0x200 + adr;
  FIO4PIN = 0x100 + (frec & 0xFF);
  FIO4PIN = 0x300 + (frec & 0xFF);
  FIO4PIN = adr + 1;
  FIO4PIN = 0x201 + adr;
  highbyte = (frec >> 8) & 0xFF;
  FIO4PIN = 0x100 + highbyte;
  FIO4PIN = 0x300 + (frec >> 8);
}

int
ymzwrite1 (int value, int adr)
{
  FIO4PIN = 0x00;
  FIO4PIN = adr;
  FIO4PIN = (0x200 + adr);
  FIO4PIN = (0x100 + value);
  FIO4PIN = (0x300 + value);
  return adr * 16 + value;
}
