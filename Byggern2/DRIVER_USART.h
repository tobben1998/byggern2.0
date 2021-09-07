//Driver for UART

//Functions for sending and receiving data to and from the serial interface

#ifndef DRIVER_USART_H_
#define DRIVER_USART_H_

//Initialisering


#define FOSC 4915200
#define BAUD 9600


unsigned char USART_printf(unsigned char);
	
void USART_Init( unsigned int ubrr);

//Transmit function
void USART_Transmit( unsigned char data);


//Receive function
unsigned char USART_receive(void);

#endif