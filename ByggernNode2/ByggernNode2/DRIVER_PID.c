#include "sam.h"

volatile int32_t TimeDelay = 50;
volatile int32_t LEDval = 1;
int alarm = 0;

void SysTick_init(uint32_t ticks){
	SysTick->CTRL = 0;									//Disable systick
	
	SysTick->LOAD = ticks - 1;							//Reload Register
	
	NVIC_SetPriority(SysTick_IRQn, 0);					//Set the highest priority on systick interrupts. May be too much?
	
	SysTick->VAL = 0;									//Resets the systick counter value
	
	SysTick->CTRL &= ~(0 << SysTick_CTRL_CLKSOURCE_Pos);	//Clocksource: 1 = MCK, 0 = MCK/8
	SysTick->CTRL |= (1 << SysTick_CTRL_TICKINT_Pos);	//Enable Systick interrupts. 1 = Enable, 0 = Disable
	
	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos);	//Enable Systick
}

void SysTick_Handler(void){
	//PID-regulering
	  
	//Les encoder-verdier
	
	
	//test ved å sette LED-pinner
	
	if(TimeDelay > 0){
		TimeDelay--;
	}
	
	else if(TimeDelay == 0){
		if(LEDval){
			PIOA->PIO_CODR |= PIO_CODR_P20;  //set output data register
			LEDval = 0;
			TimeDelay = 50;
		}
		else{
			PIOA->PIO_SODR |= PIO_SODR_P20;  //set output data register
			LEDval = 1;
			TimeDelay = 50;
			alarm = 1;
		}

	}
	return;
}
