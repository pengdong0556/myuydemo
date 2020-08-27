#include "include.h"
//#define OS_TICKS_PER_SEC          200 5ms

struct timer rs485_poll;




clock_time_t GetRunTime(void)
{
	int runtime; 

	//__set_PRIMASK(1);  		/* 关中断 */
	
	runtime = OSTimeGet();	
		
	//__set_PRIMASK(0);  		/* 开中断 */

	return runtime;
}

clock_time_t
clock_time(void)
{
	
	return GetRunTime(); /* 5ms 单位 */
}


void
timer_set(struct timer *t, clock_time_t interval)
{
  t->interval = interval;
  t->start = clock_time();
}

void
timer_reset(struct timer *t)
{
  t->start += t->interval;
}


int
timer_expired(struct timer *t)
{  
	/*返回1或0*/
  return (clock_time_t)(clock_time() - t->start) >= (clock_time_t)t->interval;
}


void
timer_restart(struct timer *t)
{
  t->start = clock_time();
}
void 
timer_Init(void)
{
	timer_set(&rs485_poll, RS485_POLL_INTERVAL);
}
