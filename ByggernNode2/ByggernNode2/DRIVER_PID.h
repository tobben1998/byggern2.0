#ifndef DRIVER_PID
#define DRIVER_PID

int alarm;

void SysTick_init(uint32_t ticks);

void SysTick_Handler(void);


#endif