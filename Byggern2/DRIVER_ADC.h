#ifndef DRIVER_ADC_H_
#define DRIVER_ADC_H_

typedef struct {
	int X;
	int Y;
	} pos_t;

void clock_init(void);
void adc_init (void);
int adc_read(int channel);

#endif