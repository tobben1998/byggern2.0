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

int main(void)
{
		//LED TEST
		PIOA->PIO_PER |= PIO_PER_P19;  //pio enable register
		PIOA->PIO_OER |= PIO_OER_P19;  //pio enable output register
		PIOA->PIO_SODR |= PIO_SODR_P19;  //set output data register
								
		PIOA->PIO_PER |= PIO_PER_P20;  //pio enable register
		PIOA->PIO_OER |= PIO_OER_P20;  //pio enable output register
		PIOA->PIO_SODR |= PIO_SODR_P20;  //set output data register
		
	
		SystemInit(); //init SAM system
		WDT->WDT_MR |= WDT_MR_WDDIS; // disable the watchdog timer
		configure_uart();
		can_init_def_tx_rx_mb(CAN_BR);
		pwm_init();
		adc_init();
		motor_init();
		motor_dac_init();
		
		
		
		motor_stop();
		motor_encoder_tglreset();
		motor_calibrate();
		PID_rtt_init();
	
		
		
	
    while (1){
		//adc_read_putty();
		adc_ballpoint();

    }
}
