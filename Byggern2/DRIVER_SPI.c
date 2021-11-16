#include "DRIVER_SPI.h"
#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void spi_init(void){
	/* Set MOSI, SCK , Slave select output, all others become input*/
	DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	
	//vet ikke hvorfor denne mp?tte v?re output, men f?r excercise 5 til ? funke. blir masse loopingom ikke.
	DDRD |= (1<<PD2);

	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	//Set slave select
	PORTB |= 1<<PB4;
	
	//button pull up
	PORTB |= 1<<PB2;
	
	
}

void spi_send(uint8_t cData){
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

uint8_t spi_read(){
	
	char dummyByte = 0x45; //0b01000101
	
	/* Start transmission of dummy byte such that Output from slave arrives at SPDR*/
	SPDR = dummyByte;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	/*Return data register*/
	return SPDR;
}