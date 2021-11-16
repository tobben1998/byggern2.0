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



volatile uint8_t CAN_flag = 0;

static uint8_t buffer_number=0;



void can_init(){
	mcp_init();
	
	//configure bit timing for the CAN bus interface?? CNF1, CNF2 and CNF3
	//cnflags write cnf3, cnf2, cnf1. cnf3 first,
	//
	mcp_write(MCP_CNF3, 0b00000001); //PS2 = 2Tq
	mcp_write(MCP_CNF2, 0b10110101); //Propagation segment length = prseg+1=6*Tq, PS1 = phseg+1=7*tq, length of PS" determined by PHSEG22: PHSEG20 bits of cn3
	mcp_write(MCP_CNF1, 0b01000011); //brp=3, sjw=2*Tq
	


	//mcp_write(MCP_CANCTRL, MODE_LOOPBACK); //velge loopback mode //denne må vels
	mcp_write(MCP_CANCTRL, MODE_NORMAL); // velge normal mode
	
	//enable interrupt for can on atmega
	cli();
	MCUCR |= (1 << ISC01);//table 44. falling edge generates interrupt request on INT0
	MCUCR &= ~(1 << ISC00);//table 44. falling edge generates interrupt request on INT0
	GICR |= (1 << INT0);//enable interupt on INT0/PD2
	// 	GICR &= ~(1<<INT2);
	// 	EMCUCR &= ~(1<<ISC2);
	// 	GIFR |= (1<<INTF2);
	// 	GICR |= (1<<INT2);
	
	sei();
	






	//enable recive intterupts for mcp2515. page 51 in mcp2515
	//mcp_write(MCP_CANINTE,MCP_NO_INT);
	mcp_write(MCP_CANINTE, MCP_RX0IF);

}






void can_send_message(can_message *msg){
	//message: TXBnSIDH and TXBnSIDL
	//data length:TXBnDLC
	//data: TXBnDm

	//setup//

	uint8_t id_high=(msg->id)/8; //ikke de tre siste bittene
	uint8_t id_low=((msg->id)%8)*32; //de tre siste bittene shiftet 5 plasser til venstre
	mcp_write(TXBnSIDH + 16 * buffer_number, id_high); //0x31  see register 3-3
	mcp_write(TXBnSIDL + 16 * buffer_number, id_low);  //0x32

	uint8_t length=(msg->length); // tallet kan ikke være større enn 0xb00001111 //den er slik man gjør det med &?
	mcp_write(TXBnDLC + 16 *buffer_number, (length & 0b1111)); //see register 3-7


	for(uint8_t m=0; m<length;m++){
		mcp_write(TXBnDm+m+16*buffer_number,msg->data[m]); //see register 3-8
	}

	//send message//
	mcp_req_to_send(MCP_RTS_TX0 + buffer_number);
}


can_message *can_recive_msg(uint8_t buffer_number){
	// part of wait for recived message // the other part is done in init
	//uint8_t status= mcp_read_status();
	//Read message
	static can_message msg;
	//if(mcp_read(MCP_CANINTF)&(0b11)){
	
	msg.id=0x00;

	int id_high = mcp_read(RXBnSIDH+16*buffer_number); //see register 4-4
	int id_low = mcp_read(RXBnSIDL+16*buffer_number);
	msg.id=0b1000*id_high + id_low/0b100000;

	int length=(0b1111 & mcp_read(RXBnDLC+16*buffer_number)); //see register 4-8
	msg.length=length;
	for(uint8_t m=0; m<length;m++){
		msg.data[m] = mcp_read(RXBnDM+m*16*buffer_number); //see register 4-9
		// }

		//Filter and mask //må jeg legge til noe mer her?
		//  ?????????????????????????

		//Reset interrupt flag
		//mcp_bit_modify(MCP_CANINTF, 0x01, 0);
		//mcp_bit_modify(MCP_CANINTF, 0x02, 0);
		
		
	}
	return &msg;
}


void can_interrupt_handler(void){
	uint8_t flag = mcp_read(MCP_CANINTF);
	
	if(flag & MCP_RX0IF){
		can_message *p=can_recive_msg(0);
		printf("Message id %x \r\n", p->id);
		printf("Message length: %x \r\n", p->length);
		printf("Message length: %x", p->length);
		
		mcp_bit_modify(MCP_CANINTF, 0x01, 0);
	}
	if(flag & MCP_RX1IF){
		can_message *p1=can_recive_msg(1);
		printf("Message data: %x", p1->id);
		
		mcp_bit_modify(MCP_CANINTF, 0x02, 0);
	}
	
}


/*

ISR(INT0_vect){ //Interrupt handler for IR-goal signal CAN message from node 2 
	CAN_flag = 1;
	printf("CAN INTERRUPT \t");
	
	mcp_bit_modify(MCP_CANINTF, 0x01, 0);
	mcp_bit_modify(MCP_CANINTF, 0x01, 0); //Må skrive to ganger. MCP må få det inn med teskje. 
	
	uint8_t test2 = mcp_read(MCP_CANSTAT);
	uint8_t mask1 = 0xf0;
	uint8_t mask2 = 0x0f;
	uint8_t test1 = test2 & mask1;
	test2 = test2 & mask2;
	printf("OPMODE: %d \t ICOD: %d \n\r", test1, test2);
	
	
}


*/



