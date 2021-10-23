/*
 * ByggernNode2.c
 *
 * Created: 23.10.2021 13:48:16
 * Author : sandebs
 */ 


#include "sam.h"
//#include <util/delay.h>
//#include <time.h>

//1kOhm motstand måtte byttes til en 500 ohm motstand når man holder på med den motoren og no relays
//baudrate til Can init må gjøres

int main(void)
{
		//PA19
		//PA20 
    /* Initialize the SAM system */
    SystemInit();
	configure_uart();
	
	
	//REG_PIOA_OWER = PIO_PA20;
	//REG_PIOA_OWDR = ~PIO_PA20;
	
	PIOA->PIO_PER |= PIO_PER_P19;  //pio enable register
	PIOA->PIO_OER |= PIO_OER_P19;  //pio enable output register
	PIOA->PIO_SODR |= PIO_SODR_P19;  //set output data register
	
	PIOA->PIO_PER |= PIO_PER_P20;  //pio enable register
	PIOA->PIO_OER |= PIO_OER_P20;  //pio enable output register
	PIOA->PIO_SODR |= PIO_SODR_P20;  //set output data register
	

    /* Replace with your application code */
    while (1) 
    {
		//REG_PIOA_ODSR = PIO_PA20;
		
		//REG_PIOA_ODSR = 0x00000000;	
    }
}
