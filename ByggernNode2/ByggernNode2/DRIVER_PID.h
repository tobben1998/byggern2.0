#ifndef DRIVER_PID
#define DRIVER_PID

#include "can_controller.h"

void SysTick_init(uint32_t ticks);

void SysTick_Handler(void);

void PID_update_refPos(CAN_MESSAGE *msg);

#endif