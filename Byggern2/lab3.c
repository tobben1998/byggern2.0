#define F_CPU 16000000  //  clock  frequency  in Hz

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "DRIVER_USART.h"
#define MYUBRR FOSC/16/BAUD-1 //UART Baud Rate Register

int main(void){
	
// 	typedef enum{ UP, DOWN, LEFT, RIGHT, NEUTRAL} joyDir;
// 		
// 	joyDir get_joyDirAxis1( int axis1){
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
// 	joyDir get_joyDirAxis2( int axis2){
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
// 	
	
	//Set pin pd4 as output
	DDRD |= (1 << DDD4);
	
	
	cli();
	TCCR3A |= (1 << COM3A0);
	TCCR3B |= (1 << CS30);
	TCCR3B |= (1 << WGM32);
	OCR3A = 1;
	sei();
	
// 	TCCR0 &= ~(1 << FOC0);
// 	TCCR0 |= (1 << WGM00);
// 	TCCR0 |= (1 << WGM01);

	//ASSR &= ~(1 << AS2); 	
	
	
	return 0;
}
