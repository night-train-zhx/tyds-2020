#include "system_init.h"


/*
*LED  PF1  PF2  PF3
*
*蓝牙    *RX        *TX 
          PB0       PB1
					
*//*串口屏  *RX        *TX
*//*          PC4       PC5
					
*mpu6050  SCL     SDA
          PB2     PB3				
*
*OLED   *SCL     *SDA
         PD2      PD3
*
*步进电机
  Dir2     STP2      STP1    DIR1
* PE5      PB4       PB6     PB7  
*
*巡线模块 5路
* PE3  PE2  PE1  PD3  PD2

*光敏 模块  1路
*   PB2    PB5
*
*蜂鸣器
*PB3  必须3V，5V会叫
*/

int main(void)
{
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);	

	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();	
	LED_Init();  //led灯初始化	
	
	UART_HIMI_INIT();//串口屏 蓝牙初始化  C4  C5(B0,B1)
	Line_GPIO_init(); //五路巡线初始化  
	
	Time_init();      /*主中断定时器5ms*/
	SMD_PWM_Init(15);//传入参数 是速度
  En_control(0,0);//停止，等待指令开始运行
	
  ROM_IntMasterEnable();  /*开全局中断*/	
		while(1)
		{ 
			HIMI_check();//串口屏检测,w为方便测试，里面的灯都打开了
		}


}


















