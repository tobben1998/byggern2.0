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

	DDRD |= (1 << DDD4);
	
	cli();
	TCCR3A |= (1 << COM3A0);
	TCCR3B |= (1 << CS30);
	TCCR3B |= (1 << WGM32);
	OCR3A = 1;
	sei();
};

void adc_init (void){
	clock_init();	
}

int adc_read(int channel){
	//Successive read pulses sense through RAM, beginning with channel 0:
	
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