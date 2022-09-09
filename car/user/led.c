#include "led.h"

/*
*************************	
	F1 ��ɫ
	F2 ��ɫ
	F3 ��ɫ	
*************************
*/

void LED_Init()    /*led��ʼ��*/
{
	
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//ʹ�� F �˿�

  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//GPIO_PIN_1|GPIO_PIN_2| ���ùܽ����� GPIO ���
	
}
/*
*************************************

�������ܣ�����led����˸ 
��ڲ�������ɫled�ƶ�Ӧ����� 1��� 2���� 3����
����  ֵ����
 ��   ע��ÿ��ִ�иú�����Ƶ�״̬��ת

*************************************
*/
void Led_Flash(uint8_t i)/*��˸Ƶ����˺�����ִ�е�Ƶ���й�*/
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






