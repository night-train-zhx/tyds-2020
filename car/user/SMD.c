#include "SMD.h"
#include "Line_pat.h"
/*
*************************************************************
*   ��  ��  ��: SMD_Init
*   ����˵��: �������  ��ʼ������,��ָ��Ƶ�ʡ�ռ�ձȵ�����
*   ��ڲ���: ui32fre -- Ƶ��;ui8duty -- ռ�ձ�
*   �� �� ֵ: none
*   ��   �� ��PB6   PB4
*   ע   �� ��Ƶ����� 50Hz
*************************************************************
*/
void SMD_PWM_Init(float speed )   /*pwm��ʼ��*/
{
	frequency= speed *  spd_fre;
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);//ʹ�� PWM0 ģ��
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ�� PB �˿�

    ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
 //   ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);

    ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
 //  ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
	
		ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV);//PWM ʱ�����ã�80MHz/64

    ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);

    ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, (SysCtlClockGet()/DIV)/frequency);//���� PWM �����������ڣ�
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,20);	//B6  �ߵ�ƽʱ�� = 10 * 64/80 000 000  = 8΢��

    ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT , true);//ʹ�� PWM �����

    ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);//ʹ�� PWM ������ģ��Ķ�ʱ��/������
		
		
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
*   ����˵��  :  �ı�Ƶ��-->�ı��ٶ�
*   ��ڲ���  : Ƶ��  ����
*   ��  ��  ֵ: none
*   ��     �� ��PB6   PE5 
*   ע     �� ���޷����Ƶ�� ֻ���� 20 
*************************************************************
*/
void SMD_Control(float fre1,float fre2 )	/*�����������*/
{
		//�ƶ�
//	if(fre1==0)
//		ROM_PWMGenDisable(PWM0_BASE, PWM_GEN_0);
//	else
//		ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
//	if(fre2==0)
//		ROM_PWMGenDisable(PWM0_BASE, PWM_GEN_1);
//	else
//		ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	
		fre1 = limit(fre1,40,10000);//����޷�  �ߵ�ƽ����20�����װ��ֵ 2^16*64
	  fre2 = limit(fre2,40,10000);
	//En_control(0,0);
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, (int)((SysCtlClockGet()/DIV)/fre1));//���� PWM �����������ڣ�80MHz/8000=10kHz
	 ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,20);	//B6  �ߵ�ƽʱ�� = 10 * 64/80 000 000  = 8΢��
	
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, (int)((SysCtlClockGet()/DIV)/fre2));//���� PWM �����������ڣ�80MHz/8000=10kHz
	 ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,20);	//B6  �ߵ�ƽʱ�� = 10 * 64/80 000 000  = 8΢��
		
	//En_control(1,1);
}




/*
*************************************************************
*   ����˵��  :  ����˿� ��ʼ��
*   ��     �� ����  PB4   �� PB5 
*   ע     �� ��
*************************************************************
*/

void En_PORT_init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ�� PB �˿�
	
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);//���ùܽ����� GPIO ���
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);//���ùܽ����� GPIO ���
}

/*
*************************************************************
*   ����˵��  :  ����˿� ��ʼ��
*   ��     �� ����  PB7   �� PE5 
*   ע     �� ��
*************************************************************
*/

void DIR_PORT_init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ�� PB �˿�
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_7);//���ùܽ����� GPIO ���
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_5);//���ùܽ����� GPIO ���

}

/*
*************************************************************
*   ����˵��  :  PWM �˿� ��ʼ��
*   ��     �� ����  PB6   �� PE4 
*   ע     �� ��
*************************************************************
*/
void Step_PORT_init(void)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//ʹ�� PB �˿�
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6);//���ùܽ����� GPIO ���
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_4);//���ùܽ����� GPIO ���

}

/*
*************************************************************
*   ����˵��  :  ����˿� ��ʼ��
*   ��     �� ����  PB4   �� PB5 
*   ע     �� ��
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
*   ����˵��  : ����  �� �����ת��
*   ��    ��  ���ߵ�ƽʱ��   �͵�ƽʱ��     ����= steps * 1.8��
*   ע     �� ��  delay_us;   �Լ����� �ߵ�ƽʱ��
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
    delay_us(us_period);//period??D?��?��a?��???����?2?��aD?����1000
  }
}

/*
*************************************************************
*   ����˵��  : �����ұ߲������ת��
*   ��    ��  ���ߵ�ƽʱ��  �͵�ƽʱ��   ����= steps * 1.8��(����������)
*   ע     �� ��
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
*   ����˵��  : ��ߵ�� ����
*   ��    ��  ��
*   ע     �� ��
*************************************************************
*/
void Dir_1(int n)
{
	if(n==1)
			ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7,GPIO_PIN_7);//���ùܽ����� GPIO ���
	if(n==0)
			ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7,0);//���ùܽ����� GPIO ���
}

/*
*************************************************************
*   ����˵��  : �ұߵ�� ����
*   ��    ��  ��
*   ע     �� ��
*************************************************************
*/
void Dir_2(int n)
{
	if(n==1)
			ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4,GPIO_PIN_4);//���ùܽ����� GPIO ���
	if(n==0)
			ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0);//���ùܽ����� GPIO ���
}

/*
*************************************************************
*   ����˵��  : ��· ��� ����
*   ��    ��  ��
*   ע     �� ��
*************************************************************
*/
void Dir_control(int t1,int t2)
{
	Dir_1(t1);
	Dir_2(t2);
}


/*
*************************************************************
*   ����˵��  : ���� ������������
*   ��    ��  ��n1 ����   n2 �ҵ��
*   ע     �� �� 
*************************************************************
*/

void En_control(int n1,int n2 )
{
	if(n1==1)
	{
		 //ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0);//���ùܽ����� GPIO ���
		
			ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	}
	if(n1==0)
	{
		//ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);//���ùܽ����� GPIO ���
		
		ROM_PWMGenDisable(PWM0_BASE, PWM_GEN_0);
	}
	
	if(n2==1)
	{
		// ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,0);//���ùܽ����� GPIO ���
		
			ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	}
	if(n2==0)
	{
		//ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,GPIO_PIN_5);//���ùܽ����� GPIO ���
	
			ROM_PWMGenDisable(PWM0_BASE, PWM_GEN_1);
	}
	
}





