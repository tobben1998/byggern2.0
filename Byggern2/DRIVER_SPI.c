#include "DRIVER_SPI.h"
#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void spi_init(void){
	/* Set MOSI, SCK , Slave select output*/
	DDRB |= (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	
	//Set pin 6 (MISO) as input.  
	DDRB &= ~(1<<DDB6);
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	//Set slave select
	PORTB |= 1<<PB4;
}
void spi_send(uint8_t cData){
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

uint8_t spi_read(){
	
	SPDR = 0x23;
	while(!(SPSR & (1<<SPIF)));
	
	uint8_t result,result2,result3;
	result = SPDR;
	result2 = SPDR;
	result3 = SPDR;
	
	printf("1: %x, 2: %x, 3: %x \n\r", result, result2, result3);
	
	return result;
}