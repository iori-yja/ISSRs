#include "lpc23xx.h"
//#include "target.h"
#include "adc.h"

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

/***********************initialize A/D converter.*****************************************/
//How to USE. Just call me, if so, i'll prepare A/D environments only for you//
//void ADCint(){
	
//}
