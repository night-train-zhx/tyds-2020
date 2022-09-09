#include "SMD.h"
#include "Line_pat.h"
/*
*************************************************************
*   函  数  名: SMD_Init
*   功能说明: 步进电机  初始化函数,出指定频率、占空比的脉冲
*   入口参数: ui32fre -- 频率;ui8duty -- 占空比
*   返 回 值: none
*   端   口 ：PB6   PB4
*   注   意 ：频率最低 50Hz
*************************************************************
*/
void SMD_PWM_Init(float speed )   /*pwm初始化*/
{
	frequency= speed *  spd_fre;
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);//使能 PWM0 模块
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能 PB 端口

    ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
 //   ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);

    ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
 //  ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
	
		ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV);//PWM 时钟配置，80MHz/64

    ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);

    ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, (SysCtlClockGet()/DIV)/frequency);//设置 PWM 发生器的周期，
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,20);	//B6  高电平时间 = 10 * 64/80 000 000  = 8微妙

    ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT , true);//使能 PWM 输出。

    ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);//使能 PWM 发生器模块的定时器/计数器
		
		
		//PB4  
		     SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); 
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); 
				
		  	GPIOPinConfigure(GPIO_PB4_M0PWM2);
        GPIOPinTypePWM(GPIO_PORTB_BASE,GPIO_PIN_4 );
//				GPIOPinConfigure(GPIO_PE5_M0PWM5);
//       GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);
				PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	      PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,(SysCtlClockGet()/DIV)/frequency);
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,20); 
       // PWMPulseWidthSet(PWM0_BASE,PWM_OUT_5,10); 
        PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT ,true);
      //  PWMOutputState(PWM0_BASE,PWM_OUT_5_BIT ,true);
        PWMGenEnable(PWM0_BASE,PWM_GEN_1);
				
				

}

/*
*************************************************************
*   功能说明  :  改变频率-->改变速度
*   入口参数  : 频率  数组
*   返  回  值: none
*   端     口 ：PB6   PE5 
*   注     意 ：限幅最低频率 只能是 20 
*************************************************************
*/
void SMD_Control(float fre1,float fre2 )	/*功率输出控制*/
{
		//制动
//	if(fre1==0)
//		ROM_PWMGenDisable(PWM0_BASE, PWM_GEN_0);
//	else
//		ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
//	if(fre2==0)
//		ROM_PWMGenDisable(PWM0_BASE, PWM_GEN_1);
//	else
//		ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	
		fre1 = limit(fre1,40,10000);//输出限幅  高电平计数20，最大装载值 2^16*64
	  fre2 = limit(fre2,40,10000);
	//En_control(0,0);
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, (int)((SysCtlClockGet()/DIV)/fre1));//设置 PWM 发生器的周期，80MHz/8000=10kHz
	 ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,20);	//B6  高电平时间 = 10 * 64/80 000 000  = 8微妙
	
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, (int)((SysCtlClockGet()/DIV)/fre2));//设置 PWM 发生器的周期，80MHz/8000=10kHz
	 ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,20);	//B6  高电平时间 = 10 * 64/80 000 000  = 8微妙
		
	//En_control(1,1);
}




/*
*************************************************************
*   功能说明  :  方向端口 初始化
*   端     口 ：左  PB4   右 PB5 
*   注     意 ：
*************************************************************
*/

void En_PORT_init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能 PB 端口
	
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);//配置管脚用作 GPIO 输出
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);//配置管脚用作 GPIO 输出
}

/*
*************************************************************
*   功能说明  :  方向端口 初始化
*   端     口 ：左  PB7   右 PE5 
*   注     意 ：
*************************************************************
*/

void DIR_PORT_init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能 PB 端口
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_7);//配置管脚用作 GPIO 输出
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_5);//配置管脚用作 GPIO 输出

}

/*
*************************************************************
*   功能说明  :  PWM 端口 初始化
*   端     口 ：左  PB6   右 PE4 
*   注     意 ：
*************************************************************
*/
void Step_PORT_init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能 PB 端口
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6);//配置管脚用作 GPIO 输出
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_4);//配置管脚用作 GPIO 输出

}

/*
*************************************************************
*   功能说明  :  方向端口 初始化
*   端     口 ：左  PB4   右 PB5 
*   注     意 ：
*************************************************************
*/
void SMD_PORT_init(void)
{
	En_PORT_init();
  //Step_PORT_init();
	//DIR_PORT_init();
}


/*
*************************************************************
*   功能说明  : 控制  左步 进电机转动
*   参    数  ：高电平时间   低电平时间     度数= steps * 1.8°
*   注     意 ：  delay_us;   自己调整 高电平时间
*************************************************************
*/
void Step1_Control(uint32_t H_time,uint32_t us_period,int steps)
{
    uint32_t i;
	if(steps>0)
	{
		Dir_1(1);
	}
	else
	{
		Dir_1(0);
		steps=-steps;
	}

  for(i=0; i <= steps;i++)
  {
  
    Step1_H;
    delay_us(H_time);
    Step1_L;
    delay_us(us_period);//period??D?￡?×a?ù???ì￡?2?òaD?óú1000
  }
}

/*
*************************************************************
*   功能说明  : 控制右边步进电机转动
*   参    数  ：高电平时间  低电平时间   度数= steps * 1.8°(正负带方向)
*   注     意 ：
*************************************************************
*/
void Step2_Control(uint32_t H_time,uint32_t us_period,int steps)
{
    uint32_t i;
	if(steps>0)
	{
		Dir_2(1);
	}
	else
	{
		Dir_2(0);
		steps=-steps;
	}

  for(i=0; i <= steps;i++)
  {
     Step2_H;
    delay_us(H_time);
    Step2_L;
    delay_us(us_period);
  }
}


/*
*************************************************************
*   功能说明  : 左边电机 方向
*   参    数  ：
*   注     意 ：
*************************************************************
*/
void Dir_1(int n)
{
	if(n==1)
			ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7,GPIO_PIN_7);//配置管脚用作 GPIO 输出
	if(n==0)
			ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7,0);//配置管脚用作 GPIO 输出
}

/*
*************************************************************
*   功能说明  : 右边电机 方向
*   参    数  ：
*   注     意 ：
*************************************************************
*/
void Dir_2(int n)
{
	if(n==1)
			ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4,GPIO_PIN_4);//配置管脚用作 GPIO 输出
	if(n==0)
			ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0);//配置管脚用作 GPIO 输出
}

/*
*************************************************************
*   功能说明  : 两路 电机 方向
*   参    数  ：
*   注     意 ：
*************************************************************
*/
void Dir_control(int t1,int t2)
{
	Dir_1(t1);
	Dir_2(t2);
}


/*
*************************************************************
*   功能说明  : 驱动 抱死或者正常
*   参    数  ：n1 左电机   n2 右电机
*   注     意 ： 
*************************************************************
*/

void En_control(int n1,int n2 )
{
	if(n1==1)
	{
		 //ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0);//配置管脚用作 GPIO 输出
		
			ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	}
	if(n1==0)
	{
		//ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);//配置管脚用作 GPIO 输出
		
		ROM_PWMGenDisable(PWM0_BASE, PWM_GEN_0);
	}
	
	if(n2==1)
	{
		// ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,0);//配置管脚用作 GPIO 输出
		
			ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	}
	if(n2==0)
	{
		//ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,GPIO_PIN_5);//配置管脚用作 GPIO 输出
	
			ROM_PWMGenDisable(PWM0_BASE, PWM_GEN_1);
	}
	
}





