
#include "Button.h"

uint8_t ReadPin1,ReadPin2;         
    bool    KeyPress1 = false;
    bool    KeyPress2 = false;

/******************************************************************************************************************
*������:ButtonInit()
*��   ��������1��2��ʼ������
*��   �룺PFO  SW2   PF4  SW1
******************************************************************************************************************/
void ButtionInit()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

		//SW2 ����
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
     HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;//����
     HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	//������������
    ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	//PF4��Ϊ�ж�����Դ��ӦLaunchPad�İ���1 SW1
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);	
	//����PF4Ϊ�������裬�����������2mA
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	//�ն�����Ϊ�½��ش���
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 , GPIO_FALLING_EDGE);
	//ʹ��PF4�ж�
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
	//ȫ���ж�
	IntMasterEnable();
	IntEnable(INT_GPIOF); 
	GPIOIntRegister(GPIO_PORTF_BASE, Button_IntHandler);

}



void Button_IntHandler(void)
{
	
		ReadPin1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);//��ⰴ�� 1
		ReadPin2 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);//��ⰴ�� 2
		
	
	if((ReadPin1&GPIO_PIN_4) != GPIO_PIN_4)
			{ 
				//�������
         delay_ms(10);//����
         ReadPin1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);//�ٴμ��
				if((ReadPin1&GPIO_PIN_4) != GPIO_PIN_4)
						{//�ٴ��ж�
                KeyPress1 = true;//������£�����һ����־λ   
                while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)) {} ;//�Ȱ����ɿ�     
											//����жϱ�־
							GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
            }
      }
	
		if((ReadPin2&GPIO_PIN_0) != GPIO_PIN_0) 
			{      
           delay_ms(10);   
           ReadPin2 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
           if((ReadPin2&GPIO_PIN_0) != GPIO_PIN_0) 
						{ 
               KeyPress2 = true;
							 while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)) {}; 
										GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
						}
      }
		
			
			if(true == KeyPress1)
			{			
							//��־λ��Ӧ�Ĺ���
						KeyPress1 = false;          

					
      }
			
		
			if(true == KeyPress2)
			 {
           KeyPress2 = false;


				 
       }	
}




void Key_Init(void) //������ʼ��
{
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  // Unlock PF0 so we can change it to a GPIO input
  // Once we have enabled (unlocked) the commit register then re-lock it
  // to prevent further changes.  PF0 is muxed with NMI thus a special case.
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
  HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
  ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_DIR_MODE_IN);//SW1
  ROM_GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
  ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_DIR_MODE_IN);//SW2
  ROM_GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}

uint8_t Key1_Press=0,Key2_Press=0; 


unsigned char Key_Scan(void)//����ɨ��
{

//  if(QuadKey2==0)
//  {
//    delay_ms(10);//��ʱȥ��
//    if(QuadKey2==0)
//    {
//      while(QuadKey2==0);//�ȴ������ͷ�
//			Key1_Press=1;
//    }
//  }
//  
//  if(QuadKey1==0)
//  {
//    delay_ms(10);//��ʱȥ��
//    if(QuadKey1==0)
//    {
//      while(QuadKey1==0);//�ȴ������ͷ�
//			Key2_Press=1;
//    }
//  }
	 if(KEY1==0||KEY2==0)
    {
        delay_ms (10);
        if(KEY1==0)return 1;
        if(KEY2==0)return 2;
    }
    return 0;

}






















