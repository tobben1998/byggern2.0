#include "DRIVER_CAN.h"
#include "DRIVER_MCP2515.h"
#include "DRIVER_SPI.h"
#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>



static uint8_t buffer_number=0;


void can_init(){
    mcp_init();


    mcp_write(MCP_CANCTRL, MODE_LOOPBACK); //velge loopback mode
   
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



    //configure bit timing for the CAN bus interface?? CNF1, CNF2 and CNF3


    //enable recive intterupts for mcp2515. page 51 in mcp2515
    mcp_write(MCP_CANINTE, MCP_RX_INT);

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
        mcp_write(TXBnDm+m*16*buffer_number,msg->data[m]); //see register 3-8
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
		printf("Message length: %x", p->id);
		
		mcp_bit_modify(MCP_CANINTF, 0x01, 0);
	}
	if(flag & MCP_RX1IF){
		can_message *p1=can_recive_msg(1);
		printf("Message data: %x", p1->id);
		
		mcp_bit_modify(MCP_CANINTF, 0x02, 0);
	}
	
}






