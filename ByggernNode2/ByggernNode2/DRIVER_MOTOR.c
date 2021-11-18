#include <stdint.h>
#include <stdio.h>


#include "DRIVER_MOTOR.h"
#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "printf-stdarg.h"
#include "DRIVER_PID.h"



void motor_init(void){
	
	//enable PIO registers
	PIOB->PIO_PER	|= PIO_PER_P27; //pwm13
	//MJ1
	PIOD->PIO_PER	|= PIO_PER_P0; //!OE
	PIOD->PIO_PER	|= PIO_PER_P1; //!RST
	PIOD->PIO_PER	|= PIO_PER_P2; //SEL
	PIOD->PIO_PER	|= PIO_PER_P9; //EN
	PIOD->PIO_PER	|= PIO_PER_P10;//DIR
	//MJ2
	PIOC->PIO_PER	|= PIO_PER_P1; //D0
	PIOC->PIO_PER	|= PIO_PER_P2; //D1
	PIOC->PIO_PER	|= PIO_PER_P3; //D2
	PIOC->PIO_PER	|= PIO_PER_P4; //D3
	PIOC->PIO_PER	|= PIO_PER_P5; //D4
	PIOC->PIO_PER	|= PIO_PER_P6; //D5
	PIOC->PIO_PER	|= PIO_PER_P7; //D6
	PIOC->PIO_PER	|= PIO_PER_P8; //D7
	
	//Enable clock to PIOC peripheral, in order to be able to read pin input 
	PMC->PMC_PCER0	|= (1 << 13);
	
	//enable input of MJ2 pins
	PIOC->PIO_ODR	|= (PIO_ODR_P1 | PIO_ODR_P2 | PIO_ODR_P3 | PIO_ODR_P4 | PIO_ODR_P5 | PIO_ODR_P6 | PIO_ODR_P7 | PIO_ODR_P8);
	
	//enable output of MJ1 pins
	PIOB->PIO_OER	|= PIO_OER_P27;
	PIOD->PIO_OER	|= (PIO_OER_P0 | PIO_OER_P1 | PIO_OER_P2 | PIO_OER_P9 | PIO_OER_P10);
	
	PIOD->PIO_SODR	|= PIO_SODR_P9; //set EN to enable motor
	PIOD->PIO_SODR	|= PIO_SODR_P10; //set direction. USE CODR for the other direction
	PIOD->PIO_CODR	|= PIO_SODR_P0;	//Set !OE high to disable output of encoder
	PIOD->PIO_SODR	|= PIO_SODR_P1;	//!RST should be high
	
}

int16_t motor_read_encoder(int doReset){
	
	PIOD->PIO_CODR	|= PIO_CODR_P0;	//Set !OE low to enable output of encoder
	PIOD->PIO_CODR	|= PIO_CODR_P2;	//Set SEL low to get the high byte out
	
	
	for(int i =0; i<2000; i++);		//waiting..
	int8_t MSB = PIOC->PIO_PDSR;
	
	PIOD->PIO_SODR	|= PIO_SODR_P2;	//Set SEL high to get the low byte out
	for(int i =0; i<2000; i++);		//waiting..
	int8_t LSB = PIOC->PIO_PDSR;
	
	if(doReset){
		motor_encoder_tglreset();
	}

	PIOD->PIO_SODR	|= PIO_SODR_P0;	//Set !OE high to disable output of encoder
	int16_t result = (short)(((MSB) & 0xFF) << 8 | (LSB) & 0xFF); //merge MSB and LSB
	return result;
}

void motor_dac_init(void){
	
	REG_PMC_PCER1	|= PMC_PCER1_PID38; //enable clock
	REG_DACC_MR		&= ~DACC_MR_TRGEN_DIS; //free-running mode
	REG_DACC_MR		|= DACC_MR_USER_SEL_CHANNEL1; //channel 1
	REG_DACC_CHER	|= DACC_CHER_CH1;
}

void motor_dac_send(CAN_MESSAGE *msg){
	uint16_t temp	= msg->data[0];
	uint16_t scaler = 12; // to scale the power. 
	if(temp >= 0 && temp <= 100){
		temp = temp*scaler;
		PIOD->PIO_CODR |= PIO_CODR_P10; //set direction right
	}
	else if (temp >= 156 && temp <=255){
		temp=(255-temp)*scaler;
		PIOD->PIO_SODR |= PIO_SODR_P10; //set direction left
	}
	REG_DACC_CDR = temp;
}

void motor_solenoid(CAN_MESSAGE *msg){
	if(msg->data[2]){
	PIOB->PIO_CODR |= PIO_CODR_P27;//pin 13 on shield. active low
	}
	else{
	PIOB->PIO_SODR |= PIO_SODR_P27;		
	}
}

void motor_encoder_tglreset(void){
	PIOD->PIO_CODR |= PIO_CODR_P1;	
	for(int i=0; i<2000; i++);		
	PIOD->PIO_SODR |= PIO_SODR_P1;
}

void motor_run(int dirLeft, int speed){
	if(dirLeft){
		PIOD->PIO_SODR |= PIO_SODR_P10; //DIR left
	}
	else{
		PIOD->PIO_CODR |= PIO_CODR_P10; //DIR right
	}
	int threshold = 1000;
	if(speed > threshold){
		speed = threshold;
	}
	else if( speed < 0){
		speed = 0;
	}
	REG_DACC_CDR=speed;
}

void motor_calibrate(void){
	int pos = motor_read_encoder(0);
	int prevPos = pos + 200;
	while(prevPos != pos){
		prevPos = pos;
		for(int i = 0; i < 100000; i++){
			motor_run(0, 800);
		}
		pos = motor_read_encoder(0);
		printf("prevPos: %d, pos: %d \n\r", prevPos, pos);
	}
	motor_stop();
	
	motor_encoder_tglreset();
	
	printf("Calibration done, Position %d \n\r", motor_read_encoder(0));
}

void motor_stop(void){
	motor_run(0,0);
}