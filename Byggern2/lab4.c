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
#define MYUBRR FOSC/16/BAUD-1 //UART Baud Rate Register

int main(void){
	
	xmem_init();
	oled_init();
	/*oled_light_screen();*/

// 	
// 	for (int i = 0; i <256; i++){
// 		oled_wrc(0x81);
// 		oled_wrc(i);
// 		_delay_ms(10);
// 	}

	oled_goto_pos(4,60);
	
	
	while(1){
		


	};
	
	return 0;
}
