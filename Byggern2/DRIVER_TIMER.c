//Using timer counter0, 8-bit counter
#include <avr/io.h>
#include <avr/interrupt.h>

static int counter = 0;
static int seconds = 0;
static int mseconds = 0;

void timer_init(void){
	//TCCR0 |= (1<< CS00); //Clock with no prescaling 4.9 MHz? Change to get higher period. 
	cli();
	TCCR0 |= (1<<CS02);
	
	
	TCCR0 |= (1<<WGM01); //Set mode to 'Clear Timer on Compare'
	TCCR0 &= ~(1<<WGM00);
	
	int8_t counterVal = 192; //set the number that counter stops and creates interrupt at.
	OCR0 |= counterVal;

	TIMSK |= (1<< OCIE0); //Enable interrupt by Ouput Compare
	sei(); //Enable global interrupts
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
	
	//LEgg til start og stopp "Knapp"
}