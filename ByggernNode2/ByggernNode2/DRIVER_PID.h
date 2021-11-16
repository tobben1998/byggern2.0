#ifndef DRIVER_PID
#define DRIVER_PID

#include "can_controller.h"

void PID_rtt_init(void);

void RTT_Handler(void);

void PID_update_refPos(CAN_MESSAGE *msg);

void PID_update_rightSlider_refPos(CAN_MESSAGE *msg);

#endif