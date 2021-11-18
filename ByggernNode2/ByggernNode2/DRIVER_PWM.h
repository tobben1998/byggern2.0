#ifndef DRIVER_PWM_H_
#define DRIVER_PWM_H_

#include "can_controller.h"

void pwm_init(void);
void pwm_update_duty_cycle(CAN_MESSAGE *msg);


#endif