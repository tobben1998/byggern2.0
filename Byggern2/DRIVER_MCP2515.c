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
	PORTB &= ~(1<<PB4);
	
	spi_send(MCP_READ);
	spi_send(addr);
	result = spi_read();
	
	PORTB |= (1<<PB4);
	
	return result;
}

void mcp_write(uint8_t addr,uint8_t data){
	
	PORTB &= ~(1<<PB4);
	
	spi_send(MCP_WRITE);
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


void mcp_bit_modify(uint8_t addr, uint8_t mask, uint8_t data){
	
	PORTB &= ~(1<<PB4);
	
	spi_send(MCP_BITMOD);
	spi_send(addr);			
	spi_send(mask);			
	spi_send(data);
	
	PORTB |= (1<<PB4);
	
}


void mcp_reset(void){
	
	PORTB &= ~(1<<PB4);
	
	spi_send(MCP_RESET); 
	
	PORTB |= (1<<PB4);
	_delay_ms(10);
}

void mcp_init(){
	
	uint8_t value;
	spi_init();
	mcp_reset();
	_delay_ms(2); 
	
	value = mcp_read(MCP_CANSTAT);
	
 	if ((value & MODE_MASK) != MODE_CONFIG){
 		printf("Not in config mode after reset! value: %x \n\r", value);
	}
}




