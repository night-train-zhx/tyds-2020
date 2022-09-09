
#include "PWM_Motor.h"
#include  "My_math.h"

/*******************************

	B6 ����   
	B7 �ҵ��

*******************************/
void PWM_Init(void)    /*pwm��ʼ��*/
{
	
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);//ʹ�� PWM0 ģ��
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ�� PB �˿�
	

    ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
    ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);

    ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
	
		ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);//PWM ʱ�����ã�80MHz

    ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);

    ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 8000);//���� PWM �����������ڣ�80MHz/8000=10kHz

    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,0);	//B6
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,0);	//B7

    ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);//ʹ�� PWM �����

    ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);//ʹ�� PWM ������ģ��Ķ�ʱ��/������

}

void powerControl(uint32_t pwm[])	/*�����������*/
{
		pwm[0] = limit(pwm[0],0,7999);//����޷�
	  pwm[1] = limit(pwm[1],0,7999);
	
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,pwm[0]);	//B6
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,pwm[1]);	//B7
	
}


/*
*******************************************

	��չ��Ӳ�����ӣ�
	Ain1 -- B2

	Bin1 -- A3

	Ain1������ǰ�� 
	Bin1���ҵ��ǰ��

*******************************************
*/
void	Port_In_Init(void)		/*�������оƬʹ�ܶ˳�ʼ��*/
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ�� B �˿�
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//ʹ�� A �˿�
	
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_4);//���ùܽ����� GPIO ���
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_3);//���ùܽ����� GPIO ���
	
	
}

void Set_Pwm(int moto1,int moto2)
{
	uint32_t PWM_AB[2];
	if (moto1<0) // ǰ��
	{
		Ain1_H;
		moto1=7999+moto1;
	}
	else  //����
	{
		Ain1_L;
	}

	
	if (moto2<0)// ǰ��
	{
		Bin1_H;
		moto2=7999+moto2;
	}
	else    //����
	{
		Bin1_L;
	}
	
	PWM_AB[0]=abs(moto1); //����ֵ
	PWM_AB[1]=abs(moto2);
	
	powerControl(PWM_AB); //ȷ�����ת��
	
}


//void	Port_In_Init(void)		/*�������оƬʹ�ܶ˳�ʼ��*/
//{
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ�� B �˿�
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//ʹ�� E �˿�
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//ʹ�� A �˿�
//	
//  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_2);//���ùܽ����� GPIO ���
//	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_0);//���ùܽ����� GPIO ���
//	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_3|GPIO_PIN_4);//���ùܽ����� GPIO ���
//	
//	
//}



//void Set_Pwm(int moto1,int moto2)
//{
//	uint32_t PWM_AB[2];
//	if (moto1<0) // ǰ��
//	{
//		Ain1_H;
//		Ain2_L;
//	}
//	else  //����
//	{
//		Ain1_L;
//		Ain2_H;
//	}

//	PWM_AB[0]=abs(moto1);
//	if (moto2<0)// ǰ��
//	{
//		Bin1_L;
//		Bin2_H;
//	}
//	else    //����
//	{
//		Bin1_H;
//		Bin2_L;
//	}
//	PWM_AB[1]=abs(moto2);
//	
//	powerControl(PWM_AB); //ȷ�����ת��
//	
//}


