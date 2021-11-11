#include "sam.h"
#include "DRIVER_PID.h"
#include "DRIVER_MOTOR.h"
#include "can_controller.h"


int refPos;
int dt_ms = 20;
double Kp = 1;
double Kd = 1;
double Ki = 1;

void SysTick_init(uint32_t ticks){ //Formula for calculating number of tics: ticks = (desired period)/(clock period) -1
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
	
	//Sjukt primitiv p-regulator:
	int pos = motor_read_encoder(0);
	int ref = refPos; //RefPos might change at an interrupt.
	int error = ref - pos; 
	printf("Error: %d \t", error);
	
	if(error > 1000){
		if(error > 500){
			for(int i = 0; i<16000; i++){
				motor_run(1,500);
			}
		}
		for(int i = 0; i<16000; i++){
			motor_run(1,1000);
		}
	}
	else if(error < -1000){
		if(error < -500){
			for(int i = 0; i<16000; i++){
				motor_run(1,500);
			}
		}
		for(int i = 0; i<16000; i++){
			motor_run(0,1000);
		}
	}
	else{
		motor_stop();
	}
}

void PID_update_refPos(CAN_MESSAGE *msg){
	int8_t posData = msg->data[0]; //[-100,100]
	refPos = -(posData - 100)*190/2; //Converting [-100,100] to [19000,0]
}
