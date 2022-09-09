
#include "Button.h"

uint8_t ReadPin1,ReadPin2;         
    bool    KeyPress1 = false;
    bool    KeyPress2 = false;

/******************************************************************************************************************
*函数名:ButtonInit()
*描   述：按键1或2初始化函数
*输   入：PFO  SW2   PF4  SW1
******************************************************************************************************************/
void ButtionInit()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

		//SW2 解锁
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
     HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;//解锁
     HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	//设置上拉输入
    ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	//PF4作为中断输入源对应LaunchPad的按键1 SW1
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);	
	//配置PF4为上拉电阻，输出电流能力2mA
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	//终端类型为下降沿触发
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 , GPIO_FALLING_EDGE);
	//使能PF4中断
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
	//全局中断
	IntMasterEnable();
	IntEnable(INT_GPIOF); 
	GPIOIntRegister(GPIO_PORTF_BASE, Button_IntHandler);

}



void Button_IntHandler(void)
{
	
		ReadPin1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);//检测按键 1
		ReadPin2 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);//检测按键 2
		
	
	if((ReadPin1&GPIO_PIN_4) != GPIO_PIN_4)
			{ 
				//如果按下
         delay_ms(10);//消抖
         ReadPin1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);//再次检测
				if((ReadPin1&GPIO_PIN_4) != GPIO_PIN_4)
						{//再次判断
                KeyPress1 = true;//如果按下，则置一个标志位   
                while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)) {} ;//等按键松开     
											//清除中断标志
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
							//标志位对应的功能
						KeyPress1 = false;          

					
      }
			
		
			if(true == KeyPress2)
			 {
           KeyPress2 = false;


				 
       }	
}




void Key_Init(void) //按键初始化
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


unsigned char Key_Scan(void)//按键扫描
{

//  if(QuadKey2==0)
//  {
//    delay_ms(10);//延时去抖
//    if(QuadKey2==0)
//    {
//      while(QuadKey2==0);//等待按键释放
//			Key1_Press=1;
//    }
//  }
//  
//  if(QuadKey1==0)
//  {
//    delay_ms(10);//延时去抖
//    if(QuadKey1==0)
//    {
//      while(QuadKey1==0);//等待按键释放
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






















