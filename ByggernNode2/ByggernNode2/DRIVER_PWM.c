#include <stdint.h>
#include <stdio.h>

#include "DRIVER_PWM.h"
#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "printf-stdarg.h"


void pwm_init(void){
	
	//pin 44 on shield
	//peripheral B channel 6

	REG_PMC_PCER1 |= PMC_PCER1_PID36; 	//enable clock 
	

    PIOC->PIO_ABSR |= PIO_ABSR_P18; //A=0 B=1 //  //B is chosen
	PIOC->PIO_PDR |= PIO_PDR_P18;  //pio disable register for i/o  set pwm pin to output 


	REG_PWM_CLK |= PWM_CLK_PREB(0) | PWM_CLK_DIVB(42); //clockB = mck/42=2MHz

	REG_PWM_CMR6 |= (PWM_CMR_CALG); //dual slope mode
	REG_PWM_CMR6 |= (PWM_CMR_CPRE_CLKB); //use clock B
	REG_PWM_CMR6 |= (PWM_CMR_CPOL); // change polarity. start high instead of low

	REG_PWM_CPRD6 &= 0;
	REG_PWM_CDTY6 &= 0;  
	REG_PWM_CPRD6=20000; //period
	REG_PWM_CDTY6=1500; //duty cycle
	REG_PWM_ENA=PWM_ENA_CHID6; 
	
	
	
}


void pwm_update_duty_cycle(CAN_MESSAGE *msg){
	 
	 int8_t tall=msg->data[0];
	 int16_t dutyCycle = tall*5+1500;
	 
	  //////////////////////////////////////////////////////////////////////////////////
	 dutyCycle=-dutyCycle+3000; //invert pwm if needed. comment/uncomment to invert//
	//////////////////////////////////////////////////////////////////////////////////
	
	 if(dutyCycle>2000){
		 dutyCycle=2000;
	 }
	 else if(dutyCycle<1000){
		 dutyCycle=1000;
	 }
	 REG_PWM_CDTYUPD6=dutyCycle;
}
