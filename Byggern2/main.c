/*
 * Byggern2.c
 *
 * Created: 07.09.2021 09:58:53
 * Author : tobiajh
 */ 

#define F_CPU 16000000  //  clock  frequency  in Hz

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "DRIVER_USART.h"

#define MYUBRR FOSC/16/BAUD-1 //UART Baud Rate Register


int main(void){
	
	USART_Init(MYUBRR);
	//USART_Transmit(USART_receive() + 1);
	int a = 1;
	printf(a);

	while (1)
	{
			printf(a);
			printf("tall: %d", a);
	}
}

