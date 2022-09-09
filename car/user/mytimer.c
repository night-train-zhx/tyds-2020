
#include "mytimer.h"
#include "led.h"
/*******************************

	SysTick  计数频率 1Mhz

*******************************/

static volatile uint32_t counter = 0;

void SysTick_Handler(void)  //每隔1us进入此中断 计数值加一
{
	counter++;  
}


void SysTick_Init()    /*定时1us*/
{
	ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / 1000000UL);  /*1Mhz*/
	
  ROM_SysTickIntEnable();  /*Enable the SysTick Interrupt.*/

  ROM_SysTickEnable();   /*Enable SysTick.*/		
}


uint32_t millis(void)//返回当前时间  单位ms
{
		return counter / 1000UL;
}

uint32_t micros(void) //返回当前时间  单位us
{
		return counter;
}



void Test_Period(Testime *Time_Lab)
{
	Time_Lab->Last_Time = Time_Lab->Now_Time;  //时间更新 
	Time_Lab->Now_Time = micros()/1000.0f; //保存当前时间 单位 ms
	Time_Lab->Time_Delta = (Time_Lab->Now_Time - Time_Lab->Last_Time);//时间间隔 float ms 
	Time_Lab->Time_Delta_INT = (uint16_t)Time_Lab->Time_Delta; //时间间隔 uint16_t ms 
	
}

/*
*************************************************************
*   函  数  名: TimeX_init   
*   功能说明: 32 位计时器初始化函
*   入口参数: TIMER  结构体
*************************************************************
*/

TIMER timer4={
	  SYSCTL_PERIPH_TIMER4,
	  TIMER4_BASE,
    INT_TIMER4A,
	  0x01,     //优先级
	 TIMERX_Handler  //中断函数指针
};

void TIMERX_Handler(void)				//系统调度中断函数
{
	static uint32_t tick=0;
		
	if(tick%100==0)  //500ms	
		Led_Flash(3);//指示灯闪烁表明程序正常运行

	if(++tick>=1500)
		tick=0;
			
	TimerIntClear(TIMER4_BASE,TIMER_TIMA_TIMEOUT);
	
}
void TimeX_init( TIMER timerX )//系统调度定时器初始化
{
  ROM_SysCtlPeripheralEnable(timerX.SYSCTL_PERIPH_TIMER);							//定时器4使能	
	
  ROM_TimerConfigure(timerX.TIMER_BASE,TIMER_CFG_PERIODIC);						//32位周期定时器				
  ROM_TimerLoadSet(timerX.TIMER_BASE,TIMER_A,ROM_SysCtlClockGet()/100-1);		//设定装载值, 5ms
	
  ROM_IntEnable(INT_TIMER0A);																		//总中断使能				
  ROM_TimerIntEnable(timerX.TIMER_BASE,TIMER_TIMA_TIMEOUT); 					//中断输出, 设置模式;			
  TimerIntRegister(timerX.TIMER_BASE,TIMER_A,TIMERX_Handler);		//中断函数注册

  ROM_IntMasterEnable();	
 ROM_TimerEnable(timerX.TIMER_BASE,TIMER_A); 		
	
	IntPrioritySet(timerX.INT_TIMER, timerX.Priority);

}



