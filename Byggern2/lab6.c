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


#define MYUBRR FOSC/16/BAUD-1 //UART Baud Rate Register

int main(void){
	
	USART_Init(MYUBRR);
	xmem_init();
	oled_init();
	menu_init();
	adc_init();
	joystick_calibrate();
	
	//mcp_init();
	can_init();
	
// 	mcp_write(MCP_CANCTRL,MODE_CONFIG);
//mcp_bit_modify(MCP_CANCTRL,MODE_MASK,MODE_CONFIG);
/*	uint8_t value = mcp_read(MCP_CANSTAT);*/
	
/*	if ((value & MODE_MASK) != MODE_CONFIG){
		printf("Not in config mode after reset! value: %x \n\r", value);
	}
	printf(" in config mode after reset! value: %x \n\r", value); */

	//mcp_bit_modify(MCP_CANCTRL,MODE_MASK,MODE_LOOPBACK);
//	mcp_write(MCP_CANCTRL,MODE_CONFIG);
//	mcp_write(MCP_CANCTRL, MODE_LOOPBACK);
/*	uint8_t value1 = mcp_read(MCP_CANSTAT);*/
/*	printf(" in MODE value: %x \n\r", value1);*/

	can_message msg;
	msg.id=6;
	msg.length=8;
	msg.data[0] = 'A';
	msg.data[1] = 'B';
	
	joystick_position pos;
	pos.x_pos= -1;
	pos.y_pos= 41;


	while(1){
		_delay_us(1);
		//can_send_message(&msg);
		joystick_sendPositionCan(joystick_getPosition());
		//can_interrupt_handler();
	
	}
	
	return 0;
}
