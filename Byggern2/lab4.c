#define F_CPU 16000000  //  clock  frequency  in Hz

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "DRIVER_USART.h"
#include "DRIVER_ADC.h"
#include "DRIVER_XMEM.h"
#include "DRIVER_JOYSTICK.h"

#define MYUBRR FOSC/16/BAUD-1 //UART Baud Rate Register

int main(void){
	
	xmem_init();
	while(1){
		xmem_write(0x00,)
		_delay_ms(500);
		xmem_write(0x00,0x0000);
		_delay_ms(500);
	}
	
	return 0;
}
