#define F_CPU 16000000  //  clock  frequency  in Hz

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "can_controller.h"
#include "can_interrupt.h"
#include "printf-stdarg.h"
#include "uart.h"



//brukes denne her?
//#define MYUBRR FOSC/16/BAUD-1 //UART Baud Rate Register

int main(void){
	
	configure_uart();
	//D1=PA5
	//D2=PA0
	
	//inits
	



	while(1){
		_delay_us(1);

		
	}
	
	return 0;
}
