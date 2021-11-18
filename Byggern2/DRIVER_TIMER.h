//Joystick Header file


#ifndef DRIVER_TIMER_H_
#define DRIVER_TIMER_H_

#include <stdint.h>

void timer_init(void);
int timer_get_seconds(void);
int timer_get_mseconds(void);

void timer2_init(void);
void timer2_start(void);
void timer2_stop(void);
void timer2_set_ctrl(uint8_t ctrl);

#endif
