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
#include "DRIVER_OLED.h"
#include "DRIVER_MENU.h"
#define MYUBRR FOSC/16/BAUD-1 //UART Baud Rate Register

int main(void){
	
	xmem_init();
	oled_init();
	menu_init();
	adc_init();
	joystick_calibrate();
	
	while(1){
		navigate();
		_delay_ms(80);
	};
	
	return 0;
}
