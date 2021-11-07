#include <stdint.h>
#include <stdio.h>


#include "DRIVER_MOTOR.h"
#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "printf-stdarg.h"




//pin til DACC = PB16. på shield A_out

//motor
//actaviate oe! pin


//PC1-PC8=DO0-D07
//!OE=pd0, !RST= PD1, SEL=PD2, EN=PD9, DIR=PD10

/*	
eksempel på sette led pinne
	
PIOA->PIO_PER |= PIO_PER_P19;  //pio enable register
PIOA->PIO_OER |= PIO_OER_P19;  //pio enable output register
PIOA->PIO_SODR |= PIO_SODR_P19;  //set output data register */

void motor_init(void){
	//enable PIO registers
	//PB27=pwm13
	PIOB->PIO_PER |= PIO_PER_P27;
	
	PIOC->PIO_PER |= PIO_PER_P1;	
	PIOC->PIO_PER |= PIO_PER_P2;  
	PIOC->PIO_PER |= PIO_PER_P3;  
	PIOC->PIO_PER |= PIO_PER_P4;
	PIOC->PIO_PER |= PIO_PER_P5;  
	PIOC->PIO_PER |= PIO_PER_P6;  
	PIOC->PIO_PER |= PIO_PER_P7;  
	PIOC->PIO_PER |= PIO_PER_P8;
	
	PIOD->PIO_PER |= PIO_PER_P0;
	PIOD->PIO_PER |= PIO_PER_P1;
	PIOD->PIO_PER |= PIO_PER_P2;
	PIOD->PIO_PER |= PIO_PER_P9;
	PIOD->PIO_PER |= PIO_PER_P10;
	
	//enable output
	PIOB->PIO_OER |= PIO_OER_P27;
	
	PIOD->PIO_OER |= PIO_OER_P0;
	PIOD->PIO_OER |= PIO_OER_P1;
	PIOD->PIO_OER |= PIO_OER_P2;
	PIOD->PIO_OER |= PIO_OER_P9;
	PIOD->PIO_OER |= PIO_OER_P10;
	
	PIOD->PIO_SODR |= PIO_SODR_P9; //set EN=1 to enable motor
	PIOD->PIO_SODR |= PIO_SODR_P10; //set direction. USE CODR for the other direction
	
	PIOD->PIO_SODR |= PIO_SODR_P0;	//Set !OE high to disable output of encoder
	PIOD->PIO_SODR |= PIO_SODR_P1;	//!RST should be high
	
	//mangler noe her?
	//punkt 3.2 in user guide for dc motor interface. 
	//provide an analog value between  0 and 5 volt on pin DA! of MJEX to control output volatge to the motor
		
}



int motor_read_encoder(void){
	PIOD->PIO_CODR |= PIO_CODR_P0;	//Set !OE low to enable output of encoder
	PIOD->PIO_CODR |= PIO_CODR_P2;	//Set SEL low to get the high byte out
	for(int i =0; i<5000; i++);		//waiting..
	int MSB = PIOC->PIO_PDSR;		//Read D0 to D7 to get the MSB
	PIOD->PIO_SODR |= PIO_SODR_P2;	//Set SEL high to get the low byte out
	for(int i =0; i<5000; i++);		//waiting..
	int LSB = PIOC->PIO_PDSR;
	PIOD->PIO_CODR |= PIO_CODR_P1;	//Toggle !RST to reset encoder
	PIOD->PIO_SODR |= PIO_SODR_P1;
	PIOD->PIO_SODR |= PIO_SODR_P0;	//Set !OE high to disable output of encoder
	int result = (short)(((MSB) & 0xFF) << 8 | (LSB) & 0xFF); //Spleise MSB og LSB til ett tall. 
	return result;
}

void motor_dac_init(void){
	//REG_PWM_CMR6 |= (PWM_CMR_CALG); eksemple pwm
	//enable clock. pheriperhal
	//acess DACC_MR DACC_CHER and DACC_CDR
	
	
	REG_PMC_PCER1 |= PMC_PCER1_PID38; //enable clock
	REG_DACC_MR &= ~DACC_MR_TRGEN_DIS; //freerunning mode
	REG_DACC_MR |= DACC_MR_USER_SEL_CHANNEL1; //velg kanal 1
	REG_DACC_CHER |= DACC_CHER_CH1;

}

void motor_dac_send(CAN_MESSAGE *msg){
	uint16_t tall=msg->data[0];
	uint16_t scaler=40; //DAC er 12 bit. kommer max opp til 4000 nå, som er under 2^12=4096.
	if(tall>=0 && tall<=100){
		tall=tall*scaler;
		PIOD->PIO_CODR |= PIO_CODR_P10; //set direction right?
	}
	else if (tall>=156 && tall<=255 ){
		tall=(255-tall)*scaler;
		PIOD->PIO_SODR |= PIO_SODR_P10; //set direction left?
	}
	REG_DACC_CDR=tall;
}


// void motor_dac_send(CAN_MESSAGE *msg){
// 	REG_DACC_CDR=(msg->data[0]);
// }

void motor_solenoid(CAN_MESSAGE *msg){
	if(msg->data[2]){
	PIOB->PIO_SODR |= PIO_SODR_P27;
	}
	else{
	PIOB->PIO_CODR |= PIO_CODR_P27;		
	}
}
