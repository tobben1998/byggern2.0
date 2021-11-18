#include "sam.h"
#include <stdio.h>

static int SysTick_counter = 0;
static int SysTick_seconds = 0;
static volatile uint8_t busywait_alarm = 0;
static uint32_t CompareValms;



void SysTick_init(uint32_t ticks){ //Formula for calculating number of ticks: ticks = (desired period)/(clock period) -1
	
	SysTick->CTRL  = 0;									//Disable systick
	SysTick->CTRL |= (1 << SysTick_CTRL_CLKSOURCE_Pos); //Clocksource: 1 = MCK, 0 = MCK/8
	SysTick->CTRL |= (1 << SysTick_CTRL_TICKINT_Pos);	//Enable Systick interrupts. 1 = Enable, 0 = Disable
	
	SysTick->LOAD = ticks - 1;							//Reload Register
	
	NVIC_SetPriority(SysTick_IRQn, 0);					//Set the highest priority on systick interrupts. May be too much?
	
	SysTick->VAL = 0;									//Resets the systick counter value
	
	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos);	//Enable Systick
}

void SysTick_disable(void){
	SysTick->CTRL = 0;
}

void SysTick_Handler(void){
	
	SysTick_counter++;
	if(SysTick_counter == CompareValms){
		busywait_alarm = 1;
	}
	
	if(SysTick_counter%1000 == 0){
		SysTick_seconds++;
		printf("counter: %d", SysTick_counter);
		printf("busywait: %d", busywait_alarm);
		printf("Seconds: %d \n\r", SysTick_seconds);
	}
}

void timer_busywaitms(uint32_t mseconds){
	SysTick_init(84032); //1ms
	CompareValms = mseconds;
	printf("Timer started... \n\r");
	while(!busywait_alarm){};
	busywait_alarm = 0;
	printf("...Timer stopped\n\r");
	SysTick_disable();
}

