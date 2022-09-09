
#include "PWM_Motor.h"
#include  "My_math.h"

/*******************************

	B6 左电机   
	B7 右电机

*******************************/
void PWM_Init(void)    /*pwm初始化*/
{
	
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);//使能 PWM0 模块
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能 PB 端口
	

    ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
    ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);

    ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
	
		ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);//PWM 时钟配置，80MHz

    ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);

    ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 8000);//设置 PWM 发生器的周期，80MHz/8000=10kHz

    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,0);	//B6
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,0);	//B7

    ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);//使能 PWM 输出。

    ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);//使能 PWM 发生器模块的定时器/计数器

}

void powerControl(uint32_t pwm[])	/*功率输出控制*/
{
		pwm[0] = limit(pwm[0],0,7999);//输出限幅
	  pwm[1] = limit(pwm[1],0,7999);
	
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,pwm[0]);	//B6
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,pwm[1]);	//B7
	
}


/*
*******************************************

	扩展板硬件连接：
	Ain1 -- B2

	Bin1 -- A3

	Ain1低左电机前进 
	Bin1低右电机前进

*******************************************
*/
void	Port_In_Init(void)		/*电机驱动芯片使能端初始化*/
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能 B 端口
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//使能 A 端口
	
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_4);//配置管脚用作 GPIO 输出
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_3);//配置管脚用作 GPIO 输出
	
	
}

void Set_Pwm(int moto1,int moto2)
{
	uint32_t PWM_AB[2];
	if (moto1<0) // 前进
	{
		Ain1_H;
		moto1=7999+moto1;
	}
	else  //后退
	{
		Ain1_L;
	}

	
	if (moto2<0)// 前进
	{
		Bin1_H;
		moto2=7999+moto2;
	}
	else    //后退
	{
		Bin1_L;
	}
	
	PWM_AB[0]=abs(moto1); //绝对值
	PWM_AB[1]=abs(moto2);
	
	powerControl(PWM_AB); //确定电机转速
	
}


//void	Port_In_Init(void)		/*电机驱动芯片使能端初始化*/
//{
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能 B 端口
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能 E 端口
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//使能 A 端口
//	
//  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_2);//配置管脚用作 GPIO 输出
//	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_0);//配置管脚用作 GPIO 输出
//	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_3|GPIO_PIN_4);//配置管脚用作 GPIO 输出
//	
//	
//}



//void Set_Pwm(int moto1,int moto2)
//{
//	uint32_t PWM_AB[2];
//	if (moto1<0) // 前进
//	{
//		Ain1_H;
//		Ain2_L;
//	}
//	else  //后退
//	{
//		Ain1_L;
//		Ain2_H;
//	}

//	PWM_AB[0]=abs(moto1);
//	if (moto2<0)// 前进
//	{
//		Bin1_L;
//		Bin2_H;
//	}
//	else    //后退
//	{
//		Bin1_H;
//		Bin2_L;
//	}
//	PWM_AB[1]=abs(moto2);
//	
//	powerControl(PWM_AB); //确定电机转速
//	
//}


