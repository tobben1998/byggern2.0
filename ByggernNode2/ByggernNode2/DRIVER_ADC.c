#include <stdint.h>
#include <stdio.h>

#include "DRIVER_ADC.h"
#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "./printf-stdarg.h"


//activate perperal.
// not ab periperhal, but extra //see 43.5.3
//singel ended freerunning mode
//adc_mr mode register
//adc_cher channel enable register
//adc_cr controller register
//adc_cdr[n] only in free runing mode, but i chosse this- channel n data register

void adc_init(void){
		REG_PMC_PCER1 |= PMC_PCER1_PID37; 	//enable clock
		REG_ADC_CHER |=ADC_CHDR_CH0; // Valgte kanal 0. medfører at pa2 blir valgt=A7/AD/ 
		REG_ADC_MR |= ADC_MR_FREERUN_ON;
		//REG_ADC_MR |= ADC_MR_PRESCAL(???);
		//REG_ADC_MR |= ADC_MR_TRACKTIM(???);
		//REG_ADC_MR |= ADC_MR_TRANSFER(???);
		REG_ADC_CR |= ADC_CR_START;
		
		//adc_cwr for comparing settings of ADC_EMR
	
}

	
	//REG_ADC_CDRx used for reading current result
	