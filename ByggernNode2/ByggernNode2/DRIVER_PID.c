#include "sam.h"
#include "DRIVER_PID.h"
#include "DRIVER_MOTOR.h"
#include "can_controller.h"
#include <stdio.h>
#include <math.h>

int refPos;
static int count = 0;
static int seconds = 0;


void PID_rtt_init(int mseconds){
	NVIC_EnableIRQ(RTT_IRQn); // Enable RTT interrupts in NVIC
	uint32_t prescaler = mseconds * 33; //Blir en litt unøyaktig teller
	REG_RTT_MR = 0;
	REG_RTT_MR |= (prescaler << RTT_MR_RTPRES_Pos);
	REG_RTT_MR |= RTT_MR_RTTINCIEN;
};



void RTT_Handler(void){
	//Denne interrupt-handleren kan brukes til å styre regulatoren! Husk å kalle rtt_init(ms)
	//Sjukt primitiv p-regulator:
	int pos = motor_read_encoder(0);
	int ref = refPos; //RefPos might change at an interrupt.
	int error = ref - pos;
	//printf("Error: %d \t", error);

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
	
	//reading status register will clear interrupt flags
	uint32_t status = REG_RTT_SR;
	if(status & RTT_SR_RTTINC){ //ALMS generated the interrupt
		count++;
		printf("Inc %d \n\r", count);
		if(count%50 == 0){
			seconds++;
			printf("Seconds: %d \n\r", seconds);
		}
	}
}


void PID_update_refPos(CAN_MESSAGE *msg){
	int8_t posData = msg->data[0]; //[-100,100]
	refPos = -(posData - 100)*190/2; //Converting [-100,100] to [19000,0]
}
