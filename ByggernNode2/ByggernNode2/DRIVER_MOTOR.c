#include <stdint.h>
#include <stdio.h>


#include "DRIVER_MOTOR.h"
#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "printf-stdarg.h"
#include "DRIVER_PID.h"




//pin til DACC = PB16. på shield A_out

//	DO0-D07 = PC1-PC8
//	!OE	=	PD0
//	!RST=	PD1
//	SEL	=	PD2
//	EN	=	PD9
//	DIR	=	PD10

/*	
eksempel på sette led pinne
	
PIOA->PIO_PER |= PIO_PER_P19;  //pio enable register
PIOA->PIO_OER |= PIO_OER_P19;  //pio enable output register
PIOA->PIO_SODR |= PIO_SODR_P19;  //set output data register */


void motor_init(void){
	//enable PIO registers
	PIOB->PIO_PER |= PIO_PER_P27; //pwm13
	//MJ1
	PIOD->PIO_PER |= PIO_PER_P0; //!OE
	PIOD->PIO_PER |= PIO_PER_P1; //!RST
	PIOD->PIO_PER |= PIO_PER_P2; //SEL
	PIOD->PIO_PER |= PIO_PER_P9; //EN
	PIOD->PIO_PER |= PIO_PER_P10;//DIR
	//MJ2
	PIOC->PIO_PER |= PIO_PER_P1; //D0
	PIOC->PIO_PER |= PIO_PER_P2; //D1
	PIOC->PIO_PER |= PIO_PER_P3; //D2
	PIOC->PIO_PER |= PIO_PER_P4; //D3
	PIOC->PIO_PER |= PIO_PER_P5; //D4
	PIOC->PIO_PER |= PIO_PER_P6; //D5
	PIOC->PIO_PER |= PIO_PER_P7; //D6
	PIOC->PIO_PER |= PIO_PER_P8; //D7
	
	//Enable clock to PIOC peripheral, in order to be able to read pin input 
	PMC->PMC_PCER0 |= (1 << 13);
	
	//enable input of MJ2 pins
	PIOC->PIO_ODR |= (PIO_ODR_P1 | PIO_ODR_P2 | PIO_ODR_P3 | PIO_ODR_P4 | PIO_ODR_P5 | PIO_ODR_P6 | PIO_ODR_P7 | PIO_ODR_P8);
	
	//enable output of MJ1 pins
	PIOB->PIO_OER |= PIO_OER_P27;
	PIOD->PIO_OER |= (PIO_OER_P0 | PIO_OER_P1 | PIO_OER_P2 | PIO_OER_P9 | PIO_OER_P10);
	
	PIOD->PIO_SODR |= PIO_SODR_P9; //set EN to enable motor
	PIOD->PIO_SODR |= PIO_SODR_P10; //set direction. USE CODR for the other direction
	PIOD->PIO_CODR |= PIO_SODR_P0;	//Set !OE high to disable output of encoder
	PIOD->PIO_SODR |= PIO_SODR_P1;	//!RST should be high
	
	
	//mangler noe her?
	//punkt 3.2 in user guide for dc motor interface. 
	//provide an analog value between  0 and 5 volt on pin DA! of MJEX to control output voltage to the motor		
}

int16_t motor_read_encoder(int doReset){
	
	PIOD->PIO_CODR |= PIO_CODR_P0;	//Set !OE low to enable output of encoder
	PIOD->PIO_CODR |= PIO_CODR_P2;	//Set SEL low to get the high byte out
	
	
	for(int i =0; i<2000; i++);		//waiting..
	int8_t MSB = PIOC->PIO_PDSR;
	//printf("MSB: %x \t", (0xff & MSB));		//Read D0 to D7 to get the MSB
	
	PIOD->PIO_SODR |= PIO_SODR_P2;	//Set SEL high to get the low byte out
	for(int i =0; i<2000; i++);		//waiting..
	int8_t LSB = PIOC->PIO_PDSR;
	//printf("LSB: %d \t", (0xff & LSB));
	
	if(doReset){
		motor_encoder_tglreset();
	}

	PIOD->PIO_SODR |= PIO_SODR_P0;	//Set !OE high to disable output of encoder
	int16_t result = (short)(((MSB) & 0xFF) << 8 | (LSB) & 0xFF); //Spleise MSB og LSB til ett tall. 
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
	uint16_t scaler=12; //DAC er 12 bit. Dersom scaler = 40, kommer max opp til 4000, som er under 2^12=4096. Satt til 12 for å roe ned pådrag. 
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

void motor_solenoid(CAN_MESSAGE *msg){
	if(msg->data[2]){
	PIOB->PIO_CODR |= PIO_CODR_P27; //Pinne 13 på shield? Lurer på om må bytte sånn at knappetrykket gir 0 V (aktivt lav)
	}
	else{
	PIOB->PIO_SODR |= PIO_SODR_P27;		
	}
}

void motor_encoder_tglreset(void){
	PIOD->PIO_CODR |= PIO_CODR_P1;	//Toggle !RST to reset encoder
	for(int i=0; i<2000; i++);		//Necessary for the motorbox to change value of 16-bit register
	PIOD->PIO_SODR |= PIO_SODR_P1;
}

void motor_calibrate(void){ //Veldig hardkodet mtp. tid den kjører mot høyre.
	for(int i =0; i < 5000000; i++){
		motor_run(0,1000);	
	}
	motor_encoder_tglreset();
	printf("Calibration finished, position value: %d", motor_read_encoder(0));
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

void motor_calibrate2(void){
	int pos = motor_read_encoder(0);
	int prevPos = pos +200;
	while(prevPos != pos){
		prevPos = pos;
		for(int i = 0; i < 100000; i++){
			motor_run(0, 800); //Må ha nok pådrag til å orke å kjøre til enden, men ikke spinne på slutten. 900 fungerer såvidt. 
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