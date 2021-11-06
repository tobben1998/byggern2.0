#include <stdint.h>
#include <stdio.h>

#include "DRIVER_ADC.h"
#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "./printf-stdarg.h"

static int activateGoal=1; //global variable
static int goalCount=0; //global variable




//activate perperal.
// not ab periperhal, but extra //see 43.5.3
//singel ended freerunning mode
//adc_mr mode register
//adc_cher channel enable register
//adc_cr controller register
//adc_cdr[n] only in free runing mode, but i chosse this- channel n data register

//A7 på shieldet er valhgt

void adc_init(void){
		REG_PMC_PCER1	|= PMC_PCER1_PID37; 	//enable clock
		REG_ADC_CHER	|= ADC_CHDR_CH0; // Valgte kanal 0. medfører at pa2 blir valgt=A7/AD/ 
		REG_ADC_MR		|= ADC_MR_FREERUN_ON;
		//REG_ADC_MR |= ADC_MR_PRESCAL(???);
		//REG_ADC_MR |= ADC_MR_TRACKTIM(???);
		//REG_ADC_MR |= ADC_MR_TRANSFER(???);
		REG_ADC_CR		|= ADC_CR_START; //The software trigger that starts ADC conversion
		
		//adc_cwr for comparing settings of ADC_EMR
	
}

void adc_read_putty(void){
	//printf("test");
	printf("adc value: %d \n\r",REG_ADC_CDR);//used for reading current result
}

void adc_ballpoint(int *goal){
	int adcval = REG_ADC_CDR;
	if(adcval < 1000  && activateGoal==1){ // eller annen tersekl verdi
		goalCount++;
		activateGoal=0;
		printf("Goal Count: %d \n\r", goalCount);
		
	}
	if (adcval> 4000 && activateGoal==0){
		activateGoal=1;
	}
}