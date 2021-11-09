#ifndef DRIVER_MOTOR_H_
#define DRIVER_MOTOR_H_

#include "can_controller.h"
//pin til DACC = PB16. på shield A_out

void motor_init(void);
int motor_read_encoder(int doReset);
void motor_dac_send(CAN_MESSAGE *msg);
void motor_dac_init(void);
void motor_solenoid(CAN_MESSAGE *msg);




#endif

