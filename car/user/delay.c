#include "delay.h"


void delay_ms(short int t)
{
	SysCtlDelay(t*(SysCtlClockGet()/3000));   //��ʱ t*1(ms)
}


void delay_us(short int t)
{
	SysCtlDelay(t*(SysCtlClockGet()/3000000)); //��ʱ t*1(us)
}

void delay_s(short int t)
{
	SysCtlDelay(t*(SysCtlClockGet()/3)); //��ʱ t*1 (s)
}
