//Using timer counter0, 8-bit counter
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

static int counter = 0;
static int seconds = 0;
static int mseconds = 0;
static uint8_t control;

void timer_init(void){
	
	cli();
	counter = 0;
	seconds = 0;
	mseconds = 0;
	TCCR0 |= (1<<CS02);
	
	
	TCCR0 |= (1<<WGM01); //Set mode to 'Clear Timer on Compare'
	TCCR0 &= ~(1<<WGM00);
	
	int8_t counterVal = 192; //set the number that counter stops and creates interrupt at.
	OCR0 |= counterVal;

	TIMSK |= (1<< OCIE0); //Enable interrupt by Output Compare
	sei();
}

int timer_get_seconds(void){
	return seconds;
}

int timer_get_mseconds(void){
	return mseconds;
}

ISR(TIMER0_COMP_vect){
	
	counter++;
	if(counter%10 == 0){
		mseconds++;
		
		if(counter%100 == 0){
			seconds++;
			mseconds = 0;
		}
		printf("Time: %d,%d \n\r", seconds, mseconds);
	}
}

void timer2_init(void){
	cli();
	TCCR2 = 0;
	TCCR2 |= (1 << CS22) | (1 << CS21) | (1 << CS20); //Clock selection. MCK/1024 = 4,8 KHz
	
	int8_t counterVal = 255; //set the number that counter stops and creates interrupt at.
	OCR2 |= counterVal;
	
	TCCR2 |= (1 << WGM21); //Mode of operation: Clear Timer on Compare
	TCCR2 &= ~(1 << WGM20);
	
	TIMSK |= (1 << OCIE2); //Enable interrupts on Output Compare event
	sei();
}

void timer2_stop(void){
	TCCR2 &= ~((1 << CS22) | (1 << CS21)); //Stop timer
	TCNT2 = 0;
	
	TIMSK &= ~(1 << OCIE2);
}

void timer2_start(void){
	TCCR2 |= (1 << CS22) | (1 << CS21) | (1 << CS20); //Clock selection. MCK/1024 = 4,8 KHz
	TIMSK |= (1 << OCIE2);
}

void timer2_set_ctrl(uint8_t ctrl){
	control = ctrl;
}

ISR(TIMER2_COMP_vect){
	if(control){
		slider_sendPositionButtonCan(slider_getPosition());
		
	}
	else{
		joystick_sendPositionButtonCan(joystick_getPosition());
	}
}