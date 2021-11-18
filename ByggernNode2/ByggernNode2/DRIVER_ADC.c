#include <stdint.h>
#include <stdio.h>

#include "DRIVER_ADC.h"
#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "./printf-stdarg.h"

static int activateGoal=1; 
static int goalCount=0;




// not ab peripherals, but extra //see 43.5.3
void adc_init(void){
	REG_PMC_PCER1	|= PMC_PCER1_PID37; 	//enable clock
	REG_ADC_CHER	|= ADC_CHDR_CH0; // Channel 0. entails that PA2 is chosen (A7/AD)
	REG_ADC_MR		|= ADC_MR_FREERUN_ON; //chosen free-running mode
	REG_ADC_CR		|= ADC_CR_START; //The software trigger that starts ADC conversion
	
}

void adc_read_putty(void){
	printf("adc value: %d \n\r",REG_ADC_CDR);
}

void adc_ballpoint(void){
	int adcval = REG_ADC_CDR;
	if(adcval < 250  && activateGoal == 1){ // adc value threshold depends on IR condition
		goalCount++;
		activateGoal = 0;
		printf("Goal Count: %d \n\r", goalCount);
		
		CAN_MESSAGE msg;
		msg.id = 0;
		msg.data_length = 1;
		msg.data[0] = 1;
		can_send(&msg, 0);
		
	}
	if (adcval> 4000 && activateGoal==0){
		activateGoal = 1;
	}
}