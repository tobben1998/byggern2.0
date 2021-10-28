#include "DRIVER_ADC.h"
#include "DRIVER_JOYSTICK.h"
#include "DRIVER_CAN.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "util.h"


volatile uint8_t x_offset;
volatile uint8_t y_offset;

void joystick_calibrate(){
	int n=100,arrx[n],arry[n];
	for(int i = 0;i < n;i++) {
		arrx[i]=adc_read(1);
		arry[i]=adc_read(0);
	}
	
	x_offset = util_median(arrx,n);
	y_offset = util_median(arry,n);
}

joystick_position joystick_getPosition(void){
	joystick_position position;
	uint8_t x = adc_read(1);
	uint8_t y = adc_read(0);

	if(x > x_offset){
			position.x_pos = (int8_t)100*(x-x_offset)/(0xFF - x_offset);
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


joystick_direction joystick_getDirection(void){
	//enums LEFT = 0, RIGHT = 1, DOWN = 2, UP = 3, NEUTRAL = 4
	joystick_position position;

	position = joystick_getPosition();
	
	if(position.x_pos < -50){
		return LEFT;
	}
	else if(position.x_pos > 50){
		return RIGHT;	
	}

	if(position.y_pos < -50){
		return DOWN;
	}
	else if(position.y_pos > 50){
		return UP;
	}
	
	if(position.x_pos <= 50 && position.x_pos >= -50 && position.y_pos <= 50 && position.y_pos >= -50){
		return NEUTRAL;
	}
	return 0;
}

void joystick_sendPositionCan(joystick_position pos){
	
	can_message msg;
	msg.id=1;
	msg.length=2;
	msg.data[0] = (char)pos.x_pos;
	msg.data[1] = (char)pos.y_pos;
	
	can_send_message(&msg);
	//enums LEFT = 0, RIGHT = 1, DOWN = 2, UP = 3, NEUTRAL = 4

}