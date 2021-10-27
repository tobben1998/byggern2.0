/*
 * ByggernNode2.c
 *
 * Created: 23.10.2021 13:48:16
 * Author : sandebs
 */ 

#include <stdio.h>
#include <stdlib.h>

#include "can_controller.h"
#include "can_interrupt.h"
#include "printf-stdarg.h"
#include "uart.h"
#include "sam.h"

#define CAN_BR 0x00290561
//#include <util/delay.h>
//#include <time.h>

//1kOhm motstand måtte byttes til en 500 ohm motstand når man holder på med den motoren og no relays
//baudrate til Can init må gjøres

int main(void)
{
		PIOA->PIO_PER |= PIO_PER_P19;  //pio enable register
		PIOA->PIO_OER |= PIO_OER_P19;  //pio enable output register
		PIOA->PIO_SODR |= PIO_SODR_P19;  //set output data register
								
		PIOA->PIO_PER |= PIO_PER_P20;  //pio enable register
		PIOA->PIO_OER |= PIO_OER_P20;  //pio enable output register
		PIOA->PIO_SODR |= PIO_SODR_P20;  //set output data register
		

    /* Initialize the SAM system */
    SystemInit();
	WDT->WDT_MR |= WDT_MR_WDDIS; // reset the watchdog timer
	configure_uart();
	can_init_def_tx_rx_mb(CAN_BR);
	
	printf("test");
    /* Replace with your application code */
	
	
		CAN_MESSAGE msg;
		msg.id=2;
		msg.data_length=5;

		msg.data[8] = "ABCDEFGH";
		
		
    while (1) 
    {

		//can_send(&msg,0);

		//REG_PIOA_ODSR = PIO_PA20;
		
		//REG_PIOA_ODSR = 0x00000000;	
    }
}
