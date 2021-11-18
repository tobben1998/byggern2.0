#define F_CPU 16000000  //  clock  frequency  in Hz

#include "DRIVER_CAN.h"
#include "DRIVER_MCP2515.h"
#include "DRIVER_SPI.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>



volatile uint8_t CAN_flag	 = 0;
static uint8_t buffer_number = 0;



void can_init(){
	
	mcp_init();
	
	mcp_write(MCP_CNF3, 0b00000001); //PS2 = 2Tq
	mcp_write(MCP_CNF2, 0b10110101); //Propagation segment length = prseg+1=6*Tq, PS1 = phseg+1=7*tq, length of PS" determined by PHSEG22: PHSEG20 bits of cn3
	mcp_write(MCP_CNF1, 0b01000011); //brp=3, sjw=2*Tq
	mcp_write(MCP_CANCTRL, MODE_NORMAL);
	
	cli();
	MCUCR	|= (1 << ISC01);//table 44. falling edge generates interrupt request on INT0
	MCUCR	&= ~(1 << ISC00);//-------------------||------------------------------------
	GICR	|= (1 << INT0);//enable interrupt on INT0/PD2
	sei();
	
	mcp_write(MCP_CANINTE, MCP_RX0IF);
}

void can_send_message(can_message *msg){
	//message: TXBnSIDH and TXBnSIDL
	//data length:TXBnDLC
	//data: TXBnDm

	uint8_t id_high	= (msg->id)/8;
	uint8_t id_low	= ((msg->id)%8)*32;
	mcp_write(TXBnSIDH + 16 * buffer_number, id_high); //see register 3-3
	mcp_write(TXBnSIDL + 16 * buffer_number, id_low);

	uint8_t length	= (msg->length);
	mcp_write(TXBnDLC + 16 *buffer_number, (length & 0b1111)); //see register 3-7


	for(uint8_t m=0; m<length;m++){
		mcp_write(TXBnDm+m+16*buffer_number,msg->data[m]); //see register 3-8
	}

	mcp_req_to_send(MCP_RTS_TX0 + buffer_number);
}


can_message *can_recive_msg(uint8_t buffer_number){
	static can_message msg;
	msg.id = 0x00;
	int id_high = mcp_read(RXBnSIDH+16*buffer_number); //see register 4-4
	int id_low = mcp_read(RXBnSIDL+16*buffer_number);
	msg.id = 0b1000*id_high + id_low/0b100000;

	int length = (0b1111 & mcp_read(RXBnDLC+16*buffer_number)); //see register 4-8
	msg.length = length;
	for(uint8_t m = 0; m<length; m++){
		msg.data[m] = mcp_read(RXBnDM+m*16*buffer_number); //see register 4-9
	}
	return &msg;
}

ISR(INT0_vect){
	CAN_flag = 1;
}

int can_clearIfInterrupted(void){
	if (CAN_flag == 1){
		CAN_flag = 0;
		mcp_bit_modify(MCP_CANINTF, 0x01, 0);
		return 1;
	}
	else{
		return 0;
	}
}



