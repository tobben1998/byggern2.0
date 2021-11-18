#define F_CPU 16000000  //  clock  frequency  in Hz

#include "DRIVER_ADC.h"
#include "DRIVER_SLIDER.h"
#include "DRIVER_CAN.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include "util.h"
#include <util/delay.h>


slider_position slider_getPosition(void){
	slider_position position;
	uint8_t midpoint = 127;
	uint8_t l = adc_read(2);
	uint8_t r = adc_read(3);

	if(l > midpoint){
		position.l_pos = (int8_t)100*(l-midpoint)/midpoint;
	}
	else if (l < midpoint){
		position.l_pos = 100*(l-midpoint)/midpoint;
	}
	else {
		position.l_pos = 0;
	}

	if(r > midpoint){
		position.r_pos = 100*(r-midpoint)/midpoint;
	}
	else if (r < midpoint){
		position.r_pos = 100*(r-midpoint)/midpoint;
	}
	else {
		position.r_pos = 0;
	}
	
	return position;
}


void slider_sendPositionButtonCan(slider_position pos){
	can_message msg;
	msg.id = 1;
	msg.length = 3;
	//slider_pos
	msg.data[0] = (char)pos.l_pos;
	msg.data[1] = (char)pos.r_pos;
	
	//button
	msg.data[2] = (char)(PINB & (1<<0));

	can_send_message(&msg);
}