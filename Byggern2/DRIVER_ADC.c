#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "DRIVER_ADC.h"
#include "DRIVER_XMEM.h"


#define CONVERSION_DELAY 50


void clock_init(void){
	//Set pin pd4 as output
	DDRD |= (1 << DDD4);
	
	
	cli(); //clears the global interrupt flag in SREG to prevent any form of interrupt occuring.
	TCCR3A |= (1 << COM3A0);
	TCCR3B |= (1 << CS30);
	TCCR3B |= (1 << WGM32);
	OCR3A = 1;
	sei(); // Enables interrupts by setting the global interrupt mask.
};

adc_init (void){
	//Configuration data to be written to ADC

	clock_init();
		

	
}

int adc_read(int channel){
	//Successive read pulses sense through RAM, beginning with channel 0:
	
	//Table 1
	//D0 = 1 select A as input channel
	//D1 = 0
	//D2 = 0
	//D3 = 0 for normal ADC operation
	//D4 = 0, a conversion starts when !WR goes high
	//D5 = 0, unipolar conversion for the channel specified by A0
	//D6 = 0, Single ended configuration for the channel specified
	//D7 = 1, only the channel specified by An is converted. A single !RD pulse reads the result of that conversion

	uint8_t confData = 0b10000000 | channel;
	uint16_t adcMemArea = 0x400;

	xmem_init();
	xmem_write(confData, adcMemArea);
	_delay_us(CONVERSION_DELAY);
	uint8_t result = xmem_read(adcMemArea);

	_delay_us(CONVERSION_DELAY);

	return result;
}



pos_t pos_read(void);