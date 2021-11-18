
#include <stdio.h>
#include <avr/io.h>

#include "DRIVER_USART.h"

int USART_PUTCHAR(char data ,FILE* unused){
	USART_Transmit(data);
	return 0;
}

void USART_Init( unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	
	fdevopen(USART_PUTCHAR,NULL);
}

void USART_Transmit( unsigned char data){
	
	while ( !( UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}


unsigned char USART_receive(void){
	while ( !(UCSR0A & (1<<RXC0)))
	return UDR0;
}