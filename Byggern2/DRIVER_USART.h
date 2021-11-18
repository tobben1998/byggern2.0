#ifndef DRIVER_USART_H_
#define DRIVER_USART_H_

#define FOSC 4915200
#define BAUD 9600


unsigned char USART_printf(unsigned char);	
void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data);
unsigned char USART_receive(void);

#endif