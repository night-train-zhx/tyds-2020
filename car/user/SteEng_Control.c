#include "SteEng_Control.h"

/***********************************************************
@ ������ ��PWM_SteEng
@��ڲ�����
@���ڲ�������
@����������180������ʼ�� ����20ms��ͣ���� 0��1.5ms��
@  ע �� ������PWM����ĺ궨�� ,����180���� 
*************************************************************/

void PWM_SteEng(void)
{
	//PWM��Ƶ����!! 1/4/8/16/32/64
	SysCtlPWMClockSet(SYSCTL_PWMDIV_32);

	//ʹ��PWM_1ģ��
	SysCtlPeripheralEnable(SE_SYSCTL_PERIPH_PWM);

	//ʹ�����衪��GPIOD
	SysCtlPeripheralEnable(SE_SYSCTL_PERIPH_GPIO);

	//GPIO����
	GPIOPinConfigure(SE_GPIO_PIN_PWMOUTPUT_A);
	GPIOPinTypePWM(SE_GPIO_PORT_BASE,SE_GPIO_PIN);

	//PWM����������
	PWMGenConfigure(SE_PWM_BASE,SE_PWM_GEN,SE_PWM_GEN_MODE_COUNTER|
						SE_PWM_GEN_MODE_UPDATE);

	//PWM��������������
	PWMGenPeriodSet(SE_PWM_BASE,SE_PWM_GEN,SysCtlClockGet()/1600-1);//(SysCtlClockGet()/32)/50-1

	//PWM�ź�ռ�ձ�����
	PWMPulseWidthSet(SE_PWM_BASE,SE_PWM_OUT_A,
						(PWMGenPeriodGet(SE_PWM_BASE,SE_PWM_GEN)*0.075-1));
	
	//ʹ��M1PWM0 
	PWMOutputState(SE_PWM_BASE,SE_PWM_OUT_BIT_A ,true);

	//ʹ�ܷ�����
	PWMGenEnable(SE_PWM_BASE,SE_PWM_GEN);
	
}

/***********************************************************
@ ������ ��Set_Angle
@��ڲ������Ƕ� ��-90��90��
@���ڲ�������
@��������������趨�Ƕ�
@  ע �� ��  
*************************************************************/

void Set_Angle(double Angle)
{ 
 float duty;	
	
	Angle=LIMIT( Angle , -90 , 90 );  //�޷�
	duty =  Angle/1800.0 + 0.075 ;

	PWMPulseWidthSet(SE_PWM_BASE,SE_PWM_OUT_A,
						(PWMGenPeriodGet(SE_PWM_BASE,SE_PWM_GEN)*duty-1));

}
