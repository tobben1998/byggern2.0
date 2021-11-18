#include "sam.h"
#include "DRIVER_PID.h"
#include "DRIVER_MOTOR.h"
#include "can_controller.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>


static int count = 0;
static int seconds = 0;

int refPos;
static int sumError;
static int prevError;


//tuning parameters
double dt = 0.010;
double kp = 1;
double ki = 0.5;
double kd = 0.01;

void PID_rtt_init(void){
	NVIC_EnableIRQ(RTT_IRQn); // Enable RTT interrupts in NVIC
	uint8_t mseconds = 1000*dt;
	uint32_t prescaler = mseconds * 33; //not fully accurate, but good enough.
	REG_RTT_MR = 0;
	REG_RTT_MR |= (prescaler << RTT_MR_RTPRES_Pos);
	REG_RTT_MR |= RTT_MR_RTTINCIEN;
};



void RTT_Handler(void){
	//this interupt handler can be used to control the regulator. rembemer to call pid_rtt_init();
	int pos = motor_read_encoder(0);
	int ref = refPos; //RefPos might change at an interrupt.
	int error = ref - pos;
	sumError += error;

	int derivative	= kd/dt*(error-prevError);
	int integral	= dt*ki*sumError;
	
	int power = kp*error + integral + derivative;
	
	bool dir = 0;
	if(power < 0){
		dir = 0;
		power = -power;
	}
	else if(power > 0){
		dir = 1;
	}
	motor_run(dir,power);
	
	prevError = error;

	//reading status register will clear interrupt flags
	uint32_t status = REG_RTT_SR;
}


void PID_update_refPos(CAN_MESSAGE *msg){
	int8_t posData = msg->data[0]; //[-100,100]
	refPos = -(posData - 100)*190/2; //Converting [-100,100] to [19000,0]
}

void PID_update_rightSlider_refPos(CAN_MESSAGE *msg){
	int8_t posData = msg->data[1]; //[-100,100]
	refPos = -(posData - 100)*190/2; //Converting [-100,100] to [19000,0]
}
