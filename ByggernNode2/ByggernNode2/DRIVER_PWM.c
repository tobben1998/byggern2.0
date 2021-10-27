#include <stdint.h>
#include <stdio.h>

#include "DRIVER_PWM.h"
#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "./printf-stdarg.h"



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


	//perhipeal B kanal 6

	//enable clock of the peripheral
	//PMC->PMC_PCR = PMC_PCR_EN | (ID_PWM << PMC_PCR_PID_Pos); //Enable clock on PID 36 (PWM)
	REG_PMC_PCER1 |= PMC_PCER1_PID36;
	
	//PIOC->PIO_ABSR &= ~PIO_ABSR_P18; //A=0 B=1 //  //A er valgt her
    PIOC->PIO_ABSR |= PIO_ABSR_P18; //A=0 B=1 //  //B er valgt her
	PIOC->PIO_PDR |= PIO_PDR_P18;  //pio disable register for i/o  set pwm pin to output       
	
	//MCK=84
	//sette clock A MCK  og divide factor på 41 0x2A
// 	PWM->PWM_CLK |= (0b0000 << PWM_CLK_PREA_Pos); //delt på 42 her ogs, istedneofro delt på 2 og så 21?
// 	PWM->PWM_CLK |= (0x2A << PWM_CLK_DIVA_Pos);

	//PWM_CMR_CPRE_MCK_DIV_1024 

	REG_PWM_CLK |= PWM_CLK_PREB(0) | PWM_CLK_DIVB(42);
	
	//REG_PWM_CMR6 |= PWM_CMR_CALG | PWM_CMR_CPRE_CLKA | PWM_CMR_DTHI;

	REG_PWM_CMR6 &= ~(PWM_CMR_CALG);
	REG_PWM_CMR6 &= ~(PWM_CMR_CPRE_MCK);
	REG_PWM_CMR6 |= (PWM_CMR_CPOL);

	//CPOL = 0 (starter på lav), MCK er valgt by default, CALG = 0 (Period Left alligned), 
	REG_PWM_CPRD6=40000;
	REG_PWM_CDTY6=1500;
	REG_PWM_ENA=PWM_ENA_CHID6; 

	
}


// void pwm_set_duty_cycle(int dutyCycle){
// 	if (dutyCycle)
// }

//servo driver with freq of 50HZ=period 20ms and pulse width of 0.9 and 2.1ms

//after that to amke changes to ducty cycle all you need to do is make changes to the duty cycle register. //skal dette i annen fil? eller i samme? 
