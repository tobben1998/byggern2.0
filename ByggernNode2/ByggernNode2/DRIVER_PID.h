#ifndef DRIVER_PID
#define DRIVER_PID

#include "can_controller.h"

void PID_update_refPos(CAN_MESSAGE *msg);

#endif