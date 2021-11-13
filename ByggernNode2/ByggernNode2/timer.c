#include "sam.h"
#include <stdio.h>

static int count = 0;
static int SysTick_counter = 0;
static int SysTick_seconds = 0;
static uint8_t busywait_alarm = 0;
static uint32_t CompareValms;

void timer_rtt_init(int mseconds){
	
	int16_t prescaler = 3; //Gives highest possible resolution of timer
	RTT->RTT_MR |= (prescaler << RTT_MR_RTPRES_Pos);
	RTT->RTT_MR |= RTT_MR_ALMIEN; //Enable interrupt when alarmvalue matches counter
	
	RTT->RTT_AR |= 11 * mseconds; //10.9 ish 11. 
	
	NVIC_SetPriority(RTT_IRQn, 1);
}

void RTT_Handler(){
	count++;
	printf("Count: %d", count);
	RTT->RTT_MR |= (1 << RTT_MR_RTTRST);
	
}

void SysTick_init(uint32_t ticks){ //Formula for calculating number of ticks: ticks = (desired period)/(clock period) -1
	
	SysTick->CTRL = 0;									//Disable systick
	
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
	if(SysTick_counter >= CompareValms){
		busywait_alarm = 1;
	}
	
	
	if(SysTick_counter%1000 == 0){
		SysTick_seconds++;
		printf("counter: %d", SysTick_counter);
		printf("busywait: %d", busywait_alarm);
		printf("Seconds: %d \n\r", SysTick_seconds);
	}
	
	//Sjukt primitiv p-regulator:
	// 	int pos = motor_read_encoder(0);
	// 	int ref = refPos; //RefPos might change at an interrupt.
	// 	int error = ref - pos;
	// 	//printf("Error: %d \t", error);
	//
	// 	if(error > 1000){
	// 		if(error > 500){
	// 			for(int i = 0; i<16000; i++){
	// 				motor_run(1,500);
	// 			}
	// 		}
	// 		for(int i = 0; i<16000; i++){
	// 			motor_run(1,1000);
	// 		}
	// 	}
	// 	else if(error < -1000){
	// 		if(error < -500){
	// 			for(int i = 0; i<16000; i++){
	// 				motor_run(1,500);
	// 			}
	// 		}
	// 		for(int i = 0; i<16000; i++){
	// 			motor_run(0,1000);
	// 		}
	// 	}
	// 	else{
	// 		motor_stop();
	// 	}
}

void timer_busywaitms(uint32_t mseconds){
	SysTick_init(84032); //1ms
	
	CompareValms = mseconds;
	printf("Timer started... \n\r");
	while(!busywait_alarm){
		printf("--");
	};
	busywait_alarm = 0;
	printf("...Timer stopped\n\r");
	SysTick_disable();
}



//Mulig vi må resette RTTRST in RTT_MR når alarm skjer. 