#include "DRIVER_ADC.h"
#include "DRIVER_JOYSTICK.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


volatile uint8_t x_offset;
volatile uint8_t y_offset;

void joystick_calibrate(){
	x_offset = adc_read(1);
	y_offset = adc_read(0);
}

joystick_position joystick_getPosition(void){
	joystick_position position;
	uint8_t x = adc_read(1);
	uint8_t y = adc_read(0);

	if(x > x_offset){
			position.x_pos = 100*(x-x_offset)/(0xFF - x_offset);
	}
	else if (x < x_offset){
		position.x_pos = 100*(x-x_offset)/(x_offset - 0);
	}
	else {
		position.x_pos = 0;
	}

	if(y > y_offset){
		position.y_pos = 100*(y-y_offset)/(0xFF - y_offset);
	}
	else if (y < y_offset){
		position.y_pos = 100*(y-y_offset)/(y_offset - 0);
	}
	else {
		position.y_pos = 0;
	}
	
	return position;
}

joystick_position joystick_getDirection(void){
	joystick_position position;

	position = joystick_getDirection();
	
	if(position.x_pos < -50){
		position.direction = "LEFT";
	}
	else if(position.x_pos > 50){
		position.direction = "Right";	
	}

	if(position.y_pos < -50){
		position.direction = "DOWN";
	}
	else if(position.y_pos > 50){
		position.direction = "UP";
	}
	
	if(position.x_pos == 0 && position.y_pos == 0){
		position.direction = "NEUTRAL";
	}
	
	return position;
}