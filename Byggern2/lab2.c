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
#include <stdlib.h>

#include "DRIVER_USART.h"

#define MYUBRR FOSC/16/BAUD-1 //UART Baud Rate Register

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

//External memory code:

//Init function
void xmem_init(void){
	MCUCR |= (1 << SRE); //enable XMEM
	SFIOR |= (1 << XMM2); //Mask unused bits/pins, which is PC4-PC7
}
//Write function
void xmem_write(uint8_t data, uint16_t addr){
	volatile char *ext_mem = (char *) 0x0000;
	ext_mem[addr] = data;
}

//Read function
uint8_t xmem_read(uint16_t addr){
	volatile char *ext_mem = (char *) 0x1000;
	uint8_t ret_val = ext_mem[addr];
	return ret_val;
}

int main(void){
	
	//Program to set LED-lights on output of latch, to test Latch
	
	//Set SRE bit to 1 to enable External Memory interface (p.26)
	//MCUCR |= (1 << SRE);
	
	//Make some Address to set the pins that is connected to LEDs, 
	//uint16_t LED_data = 0x03; //Two LSB bits set
	
	//Set ALE high to open latch, effectively demuxing AD-bus to interpret signal as A[7,0
	//DDRE = 0x02;
	
	//Write Address value out on bus.
	//PORTA |= (1 << 6);

	//DDRA = 0xff;
	//PORTA = 0x00;

	//PORTA &= ~(1 << 0);
	
	//Set ALE low to latch address. pins Q should be set, and LED should be lit.
	//PORTE &= ~(1 << PE1);

	/*
	DDRE = 0x02;
	PORTE = 0x02;

	DDRA = 0xFF;
	PORTA = 0xFF;
	
	_delay_ms(1000);	

	PORTE = 0x00;
	_delay_ms(1000);
	
	PORTE = 0x02;

	while (1)
	{
		PORTA = 0xFF;
		_delay_ms(1000);
		PORTA = 0x00;
		_delay_ms(1000);
	}
	*/
	
	
	
	
	//Test of SRAM, task 2.
	MCUCR |= (1 << SRE); //enable XMEM
	SFIOR |= (1 << XMM2); //Mask unused bits/pins, which is PC4-PC7
	
	USART_Init(MYUBRR);
	printf("test");
	SRAM_test();
	

//	Task 3, check addresses decoder, writing to NAND Logic.
// 	xmem_init();
// 	
// 	
// 	while(1){
// 		xmem_write(0xFF, 0x1899);
// 	}
}



/*
void labexercise1(){
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
*/