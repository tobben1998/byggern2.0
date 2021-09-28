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
	
	//typedef enum{ UP, DOWN, LEFT, RIGHT, NEUTRAL} joyDir;

	
	
	// 	TCCR0 &= ~(1 << FOC0);
	// 	TCCR0 |= (1 << WGM00);
	// 	TCCR0 |= (1 << WGM01);

	//ASSR &= ~(1 << AS2);
		
// 	joyDir get_joyDirAxis1( uint8_t axis1){
// 		
// 		if(axis1 > 128){
// 			return UP;
// 		}
// 		else if (axis1 < 128){
// 			return DOWN;
// 		}
// 		else{
// 			return NEUTRAL;
// 		}
// 	}
// 	
// 	joyDir get_joyDirAxis2( uint8_t axis2){
// 
// 		if(axis2 > 128){
// 			return RIGHT;
// 		}
// 		else if (axis2 < 128){
// 			return LEFT;
// 		}
// 		else{
// 			return NEUTRAL;
// 		}
// 	}

	


	adc_init();

	volatile uint8_t data = 0;
	data = adc_read(0);
	
	USART_Init(MYUBRR);
	
	joystick_calibrate();
	joystick_position position;
	xmem_init();
	
	while(1){
		//position = joystick_getPosition();
		position = joystick_getDirection();
		
		uint8_t x = adc_read(1);
		uint8_t y = adc_read(0);
		uint8_t left_slider = adc_read(2);
		uint8_t right_slider = adc_read(3);
		//printf("X: %d\t Y: %d\t Left: %d\t Right: %d\t\n", position.x_pos, position.y_pos, left_slider, right_slider);
		printf("Direction: %d\n", position.direction);
		_delay_ms(10);

	}
	
	return 0;
}
