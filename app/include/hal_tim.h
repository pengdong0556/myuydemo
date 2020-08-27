#ifndef __HAL_TIM__H
#define __HAL_TIM__H

#include "include.h"

#define RS485_POLL_INTERVAL     (1000*1)


typedef int clock_time_t;

struct timer {
  clock_time_t start;
  clock_time_t interval;
};


extern struct timer rs485_poll;


clock_time_t clock_time(void);
clock_time_t GetRunTime(void);

void timer_set(struct timer *t, clock_time_t interval);
void timer_reset(struct timer *t);
int  timer_expired(struct timer *t);
void timer_restart(struct timer *t);
void timer_Init(void);

#endif //__HAL_TIM__H
