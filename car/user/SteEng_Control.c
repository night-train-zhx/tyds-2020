#include "SteEng_Control.h"

/***********************************************************
@ 函数名 ：PWM_SteEng
@入口参数：
@出口参数：无
@功能描述：180°舵机初始化 周期20ms，停留在 0（1.5ms）
@  注 意 ：更改PWM请更改宏定义 ,此是180°舵机 
*************************************************************/

void PWM_SteEng(void)
{
	//PWM分频设置!! 1/4/8/16/32/64
	SysCtlPWMClockSet(SYSCTL_PWMDIV_32);

	//使能PWM_1模块
	SysCtlPeripheralEnable(SE_SYSCTL_PERIPH_PWM);

	//使能外设——GPIOD
	SysCtlPeripheralEnable(SE_SYSCTL_PERIPH_GPIO);

	//GPIO复用
	GPIOPinConfigure(SE_GPIO_PIN_PWMOUTPUT_A);
	GPIOPinTypePWM(SE_GPIO_PORT_BASE,SE_GPIO_PIN);

	//PWM发生器配置
	PWMGenConfigure(SE_PWM_BASE,SE_PWM_GEN,SE_PWM_GEN_MODE_COUNTER|
						SE_PWM_GEN_MODE_UPDATE);

	//PWM发生器周期设置
	PWMGenPeriodSet(SE_PWM_BASE,SE_PWM_GEN,SysCtlClockGet()/1600-1);//(SysCtlClockGet()/32)/50-1

	//PWM信号占空比设置
	PWMPulseWidthSet(SE_PWM_BASE,SE_PWM_OUT_A,
						(PWMGenPeriodGet(SE_PWM_BASE,SE_PWM_GEN)*0.075-1));
	
	//使能M1PWM0 
	PWMOutputState(SE_PWM_BASE,SE_PWM_OUT_BIT_A ,true);

	//使能发生器
	PWMGenEnable(SE_PWM_BASE,SE_PWM_GEN);
	
}

/***********************************************************
@ 函数名 ：Set_Angle
@入口参数：角度 （-90到90）
@出口参数：无
@功能描述：舵机设定角度
@  注 意 ：  
*************************************************************/

void Set_Angle(double Angle)
{ 
 float duty;	
	
	Angle=LIMIT( Angle , -90 , 90 );  //限幅
	duty =  Angle/1800.0 + 0.075 ;

	PWMPulseWidthSet(SE_PWM_BASE,SE_PWM_OUT_A,
						(PWMGenPeriodGet(SE_PWM_BASE,SE_PWM_GEN)*duty-1));

}
