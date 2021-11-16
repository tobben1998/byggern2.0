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


//Viktig å tune disse litt nøyere.
//Tror det har mye å si hva slags brett vi bruker. Da må vi sørge for å ha god tune før vi skal vise fram.
//Kan bli hakkete dersom motoren ikke glir glatt, mulig å olje stangen?
//printf i interrupthandleren tar for lang tid og vil påvirke systemet.
//i Motor_run har jeg lagt inn en threshold som gir en øvre terskel for pådrag. Denne kan økes for at motoren skal få mer fart.

double dt = 0.010;
double kp = 1;
double ki = 0.5;
double kd = 0.01;

void PID_rtt_init(void){
	NVIC_EnableIRQ(RTT_IRQn); // Enable RTT interrupts in NVIC
	uint8_t mseconds = 1000*dt;
	uint32_t prescaler = mseconds * 33; //Blir en litt unøyaktig teller
	REG_RTT_MR = 0;
	REG_RTT_MR |= (prescaler << RTT_MR_RTPRES_Pos);
	REG_RTT_MR |= RTT_MR_RTTINCIEN;
};



void RTT_Handler(void){
	
	//Denne interrupt-handleren kan brukes til å styre regulatoren! Husk å kalle pid_rtt_init();
	int pos = motor_read_encoder(0);
	int ref = refPos; //RefPos might change at an interrupt.
	int error = ref - pos;
	sumError += error;

	int derivative = kd/dt*(error-prevError);
	int integral = dt*ki*sumError;
	
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
	// 				motor_run(0,500);
	// 			}
	// 		}
	// 		for(int i = 0; i<16000; i++){
	// 			motor_run(0,1000);
	// 		}
	// 	}
	// 	else{
	// 		motor_stop();
	// 	}
	
	//reading status register will clear interrupt flags
	uint32_t status = REG_RTT_SR;
	// 	if(status & RTT_SR_RTTINC){ //ALMS generated the interrupt
	// 		count++;
	// 		//printf("Inc %d \n\r", count);
	// 		if(count%50 == 0){
	// 			seconds++;
	// 			//printf("Seconds: %d \n\r", seconds);
	// 		}
	// 	}
}


void PID_update_refPos(CAN_MESSAGE *msg){
	int8_t posData = msg->data[0]; //[-100,100]
	refPos = -(posData - 100)*190/2; //Converting [-100,100] to [19000,0]
}

void PID_update_rightSlider_refPos(CAN_MESSAGE *msg){
	int8_t posData = msg->data[1]; //[-100,100]
	refPos = -(posData - 100)*190/2; //Converting [-100,100] to [19000,0]
}
