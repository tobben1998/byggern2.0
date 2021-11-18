#include <stdint.h>
#include <stdio.h>

#include "DRIVER_PWM.h"
#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "printf-stdarg.h"




//data sheet chaopter 38 pwm perhiperal.
//register pmw clock,


//first enable the clock of the periperal, 
//then coose if you you are using the a or b peripheral //section 31.5.3
//then diable the pio for the pin      data sheet section 31.5.2
//Then comes the periperal specifics in important registeres
//PWM_CLOCK:  he use a 2MHZ clock
//REG_PWM CMRn: duals slope pwm mode
//REG_PWM_CPRDn: pwm channel period register 
// REG_PWM_CDTn: pwm channel duty cycle register
//pwm_ENA: pwm enable



void pwm_init(void){
	
	//pin 44 på shieldet

	//perhipeal B kanal 6

	REG_PMC_PCER1 |= PMC_PCER1_PID36; 	//enable clock 
	

    PIOC->PIO_ABSR |= PIO_ABSR_P18; //A=0 B=1 //  //B er valgt her
	PIOC->PIO_PDR |= PIO_PDR_P18;  //pio disable register for i/o  set pwm pin to output 


	REG_PWM_CLK |= PWM_CLK_PREB(0) | PWM_CLK_DIVB(42); //sette klokke B til mck/42=2MHz

	REG_PWM_CMR6 |= (PWM_CMR_CALG); //dual slope mode
	REG_PWM_CMR6 |= (PWM_CMR_CPRE_CLKB); //bruker klokke b
	REG_PWM_CMR6 |= (PWM_CMR_CPOL); //skifter polaritet. begynner nedde istednefor opp

	REG_PWM_CPRD6 &= 0;
	REG_PWM_CDTY6 &= 0;  
	REG_PWM_CPRD6=20000; //period
	REG_PWM_CDTY6=1500; //duty cycle
	REG_PWM_ENA=PWM_ENA_CHID6; 
	
	
	
}


void pwm_update_duty_cycle(CAN_MESSAGE *msg){
	 
	 int8_t tall=msg->data[0];
	 int16_t dutyCycle = tall*5+1500; //Presisjonen til joystick er rævva, går fra 1050 til 1850 ish. Fiks joystick-funksjonene. joystick går fra -100 til 70 ish.
	 
	 ////////////////////////////////////////////////////////////////////////////////
	 //dutyCycle=-dutyCycle+3000; //inverterer pwm, om det trengs. comment/uncomment to invert
	 ///////////////////////////////////////////////////////////////////////////////
	
	 if(dutyCycle>2000){ //har satt margin på 100 her for test.
		 dutyCycle=2000;
	 }
	 else if(dutyCycle<1000){//og her
		 dutyCycle=1000;
	 }
	 REG_PWM_CDTYUPD6=dutyCycle;
}

//servo driver with freq of 50HZ=period 20ms and pulse width of 0.9 and 2.1ms

//after that to amke changes to ducty cycle all you need to do is make changes to the duty cycle register. //skal dette i annen fil? eller i samme? 
