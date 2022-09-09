#include "led.h"

/*
*************************	
	F1 红色
	F2 蓝色
	F3 绿色	
*************************
*/

void LED_Init()    /*led初始化*/
{
	
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//使能 F 端口

  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//GPIO_PIN_1|GPIO_PIN_2| 配置管脚用作 GPIO 输出
	
}
/*
*************************************

函数功能：控制led灯闪烁 
入口参数：三色led灯对应的序号 1表红 2表蓝 3表绿
返回  值：无
 备   注：每次执行该函数后灯的状态翻转

*************************************
*/
void Led_Flash(uint8_t i)/*闪烁频率与此函数所执行的频率有关*/
{
	static bool toggle=0; 
	toggle = !toggle;  
	
	switch (i)
	{
		case 1: 	if(toggle)red_on;else red_off;break;
		case 2:   if(toggle)blue_on;else blue_off;break;
		case 3:   if(toggle)green_on;else green_off;break;
	}	
}






