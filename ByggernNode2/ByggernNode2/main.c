/*
 * ByggernNode2.c
 *
 * Created: 23.10.2021 13:48:16
 * Author : sandebs
 */ 

#include <stdio.h>
#include <stdlib.h>

#include "DRIVER_ADC.h"
#include "DRIVER_PWM.h"
#include "DRIVER_MOTOR.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "printf-stdarg.h"
#include "uart.h"
#include "sam.h"
#include "DRIVER_PID.h"
#include "timer.h"

#define CAN_BR 0x00290561
//#include <util/delay.h>
//#include <time.h>

//1kOhm motstand måtte byttes til en 500 ohm motstand når man holder på med den motoren og no relays
//baudrate til Can init må gjøres

int main(void)
{
		PIOA->PIO_PER |= PIO_PER_P19;  //pio enable register
		PIOA->PIO_OER |= PIO_OER_P19;  //pio enable output register
		PIOA->PIO_SODR |= PIO_SODR_P19;  //set output data register
								
		PIOA->PIO_PER |= PIO_PER_P20;  //pio enable register
		PIOA->PIO_OER |= PIO_OER_P20;  //pio enable output register
		PIOA->PIO_SODR |= PIO_SODR_P20;  //set output data register
		
		/* Initialize the SAM system */
		SystemInit();
		WDT->WDT_MR |= WDT_MR_WDDIS; // disable the watchdog timer
		configure_uart();
		can_init_def_tx_rx_mb(CAN_BR);
		pwm_init();
		adc_init();
		motor_init();
		motor_dac_init();
		
		
		
		motor_stop();
		//for(int i= 0; i < 1000000; i++);
		
		int16_t encval;
		motor_encoder_tglreset();
		motor_calibrate();
		//motor_calibrate2();
		PID_rtt_init(20);
	
		
		
	
    while (1){
			
		//encval = motor_read_encoder(0);
		//printf("Encoder Value: %d \n\r", encval);
		
		//encval = motor_read_encoder(0);
		//for(int i = 0; i < 1000000; i++);
		//printf("Encoder Value: %x \t Decimal val: %d \n\r", encval, encval);		
		//encval = motor_read_encoder(0);
		//printf("Encoder val: %x \n\r", encval);
		
		//int tull=0;
// 		a = motor_read_encoder(1);
// 		printf("Motor Position: %x \n\r", a);
		
		//adc_read_putty();
		//adc_ballpoint(&tull);

		//can_send(&msg,0);

		//REG_PIOA_ODSR = PIO_PA20;
		
		//REG_PIOA_ODSR = 0x00000000;	
    }
}
