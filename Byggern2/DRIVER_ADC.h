//Driver for ADC


#ifndef DRIVER_ADC_H_
#define DRIVER_ADC_H_

typedef struct {
	int X;
	int Y;
	} pos_t;

void clock_init(void);

//Configure ADC
void adc_init (void);

//Function for reading digital output values of ADC
int adc_read(int channel);




//Function for reading joystick position
pos_t pos_read(void);

#endif