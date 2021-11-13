#ifndef TIMER_H_
#define TIMER_H_

void timer_rtt_init(int mseconds);

void SysTick_init(uint32_t ticks);

void SysTick_Handler(void);

void SysTick_disable(void);

void timer_busywaitms(uint32_t mseconds);


#endif