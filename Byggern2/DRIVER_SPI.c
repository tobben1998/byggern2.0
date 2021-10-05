#include "DRIVER_SPI.h"
#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void spi_init(void){
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void spi_send(char cData){
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

char spi_read(){
	spi_send(0x00);
	char a=SPDR;
	return a;
}