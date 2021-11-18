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
#include "DRIVER_SPI.h"
#include "DRIVER_MCP2515.h"
#include "DRIVER_CAN.h"
#include "DRIVER_SLIDER.h"
#include "DRIVER_TIMER.h"

#define MYUBRR FOSC/16/BAUD-1 //UART Baud Rate Register


int main(void){
	
	USART_Init(MYUBRR);
	xmem_init();
	oled_init();
	menu_init();
	adc_init();
	joystick_calibrate();
	can_init();
	
	while(1){
		_delay_us(1);
		navigate();
		//joystick_sendPositionButtonCan(joystick_getPosition());
		//slider_sendPositionButtonCan(slider_getPosition());
	}
	
	return 0;
}
