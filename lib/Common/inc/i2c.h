/*********************************************************************
i2c.h header file.
don't ask any more.
*********************************************************************/
#define ADR  0xA0
#define CamADR 0x42
void i2enable();
int i2cmasterset();
int i2cstart();
int i2cstop();
void i2csender();
extern int i2cbuff[];
//extern xTaskHandle g_issrhandle;
void read_ROM();
void write_ROM();
//void vi2c();
