#define F_CPU 16000000  //  clock  frequency  in Hz

#include "DRIVER_MCP2515.h"
#include "DRIVER_SPI.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

uint8_t mcp_read(uint8_t addr){
	
	uint8_t result;
	//Set CS low
	PORTB &= ~(1<<PB4);
	
	//Send config byte
	spi_send(MCP_READ); //0x03	0b0000 0011
	spi_send(addr);
	result = spi_read();
	PORTB |= (1<<PB4);
	return result;
}

void mcp_write(uint8_t addr,uint8_t data){
	PORTB &= ~(1<<PB4);
	
	spi_send(MCP_WRITE); //0x02
	spi_send(addr);
	spi_send(data);
	
	PORTB |= (1<<PB4);
}

void mcp_req_to_send(uint8_t instruction){
	PORTB &= ~(1<<PB4);
	
	spi_send(instruction);
	
	PORTB |= (1<<PB4);
}

uint8_t mcp_read_status(){
	PORTB &= ~(1<<PB4);
	
	spi_send(MCP_READ_STATUS);
	uint8_t status = spi_read();
	
	PORTB |= (1<<PB4);
	
	return status;
}

//Setting or clearing individual bits in specific status and control registers. Not all registers are modifiable, see pg. 63.
//Mask determines which bits are allowed to change.
//Data determines what value the modified bits in the register will change to.
void mcp_bit_modify(uint8_t addr, uint8_t mask, uint8_t data){
	PORTB &= ~(1<<PB4);
	
	//Debug lør 16/10, slett når du ser dette
	//4 signaler sendes
	//0x05 0b00000101
	//0x0f 0b00001111;
	//0xe0 0b11100000;
	//0x80 0b10000000;
	
	spi_send(MCP_BITMOD);	//0x05 0b00000101
	spi_send(addr);			
	spi_send(mask);			
	spi_send(data);
	
	PORTB |= (1<<PB4);
	
}

//Re-initialize the internal registers of the MCP2515
void mcp_reset(void){
	
	PORTB &= ~(1<<PB4);
	
	spi_send(MCP_RESET); //0xC0		0b1100 0000
	
	PORTB |= (1<<PB4);
	_delay_ms(10);
}

void mcp_init(){
	
	uint8_t value;
	spi_init();
	mcp_reset();
	_delay_ms(2); //controller needs some time to do internal initialization.
	
	value = mcp_read(MCP_CANSTAT);
	
 	if ((value & MODE_MASK) != MODE_CONFIG){
 		printf("Not in config mode after reset! value: %x \n\r", value);
	}
}




