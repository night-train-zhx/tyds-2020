#include "system_init.h"

int time_ms[3]={0,0,0};  // 不同时间
float way_cm[3]={0,0,0};  // 不同距离，单位dm,注意是float
float fre_Cha[3]={0,0,0}; //速度
int time_flag=0;  //开始计时标志
int way_flag=0; //路程标志


void TIMER4A_Handler(void)				//系统调度中断函数
{
	 static uint32_t tick=0;

		
	//自由设置 三个时间段 三种速度
	if(time_flag)  
	{
	  if( time_ms[0] > 0)
		{
			frequency = fre_Cha[0]*spd_fre; //速度转化为频率
			time_ms[0] -=5;		
		}
		if( (time_ms[1] > 0)&&(time_ms[0] <= 0)  )
		{
		frequency = fre_Cha[1]*spd_fre;
		time_ms[1] -=5;	
			
		}
		if((time_ms[2]>0)&&(time_ms[1]<=0)&& (time_ms[0]<=0) )
		{
			
			frequency = fre_Cha[2]*spd_fre;
			time_ms[2] -=5;		
		}
		 
		if((time_ms[2]<=0)&&(time_ms[1]<=0)&& (time_ms[0]<=0))
		{			
			time_flag = 0 ;
			frequency=15*spd_fre;
			En_control(0,0); //停止
		}
	}

	
	
	if(way_flag)
	{	
		//自由设置 三个路程段 三种速度
		 if( way_cm[0] > 0)
		{
			frequency = fre_Cha[0]*spd_fre; //速度转化为频率
			way_cm[0] -=(float)fre_Cha[0]*0.005f; //速度*时间	
		}
		if( (way_cm[1] > 0)&&(way_cm[0] <= 0)  )
		{
			frequency = fre_Cha[1]*spd_fre;
			way_cm[1] -=(float)fre_Cha[1]*0.005f; 			
		}
			
		if((way_cm[2]>0)&&(way_cm[1]<=0) &&(way_cm[0]<=0) )
		{		
			frequency = fre_Cha[2]*spd_fre;
			way_cm[2] -= fre_Cha[2]*0.005f;  		
		}		 
		if((way_cm[2]<=0) &&(way_cm[1]<=0) &&(way_cm[0]<=0) )
		{			
		way_flag = 0 ;
		frequency=15*spd_fre;
		En_control(0,0); //停止
		}		
	}

//修改定时器 请修改 此处
	
	 Get_line_val(); //采集5路红外，光电
	 power_control();
	
		if(++tick>=1500)
		tick=0;
		TimerIntClear(TIMER4_BASE,TIMER_TIMA_TIMEOUT);
		
}

void Time_init(void)//系统调度定时器初始化
{
	
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);							//定时器4使能				
  ROM_TimerConfigure(TIMER4_BASE,TIMER_CFG_PERIODIC);						//32位周期定时器				
  ROM_TimerLoadSet(TIMER4_BASE,TIMER_A,ROM_SysCtlClockGet()/200);		//设定装载值, 10ms				
  ROM_IntEnable(INT_TIMER0A);																		//总中断使能				
  ROM_TimerIntEnable(TIMER4_BASE,TIMER_TIMA_TIMEOUT); 					//中断输出, 设置模式;			
  TimerIntRegister(TIMER4_BASE,TIMER_A,TIMER4A_Handler);		//中断函数注册
  ROM_IntMasterEnable();			
  ROM_TimerEnable(TIMER4_BASE,TIMER_A); 												//定时器使能开始计数

}

void Prepare_ALL(void)
{

			/*设置系统主频为80M*/
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);	
//	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);

	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();	
	LED_Init();       //led灯初始化
    	
	UART0_Init(115200);  //没有开起中断
		
//	Time_init();      /*主中断定时器5ms*/
	
	ROM_IntMasterEnable();  /*开全局中断*/

}




