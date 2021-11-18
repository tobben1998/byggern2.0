#ifndef DRIVER_MOTOR_H_
#define DRIVER_MOTOR_H_

#include "can_controller.h"

void motor_init(void);
int16_t motor_read_encoder(int doReset);
void motor_dac_send(CAN_MESSAGE *msg);
void motor_dac_init(void);
void motor_solenoid(CAN_MESSAGE *msg);
void motor_encoder_tglreset(void);
void motor_run(int dirLeft, int speed);
void motor_stop(void);
void motor_calibrate(void);


#endif

