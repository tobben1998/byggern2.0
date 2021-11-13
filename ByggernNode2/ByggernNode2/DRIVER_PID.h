#ifndef DRIVER_PID
#define DRIVER_PID

#include "can_controller.h"

void PID_rtt_init(int mseconds);

void RTT_Handler(void);

void PID_update_refPos(CAN_MESSAGE *msg);

#endif