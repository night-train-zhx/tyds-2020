#include "system_init.h"


/*
*LED  PF1  PF2  PF3
*
*����    *RX        *TX 
          PB0       PB1
					
*//*������  *RX        *TX
*//*          PC4       PC5
					
*mpu6050  SCL     SDA
          PB2     PB3				
*
*OLED   *SCL     *SDA
         PD2      PD3
*
*�������
  Dir2     STP2      STP1    DIR1
* PE5      PB4       PB6     PB7  
*
*Ѳ��ģ�� 5·
* PE3  PE2  PE1  PD3  PD2

*���� ģ��  1·
*   PB2    PB5
*
*������
*PB3  ����3V��5V���
*/

int main(void)
{
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);	

	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();	
	LED_Init();  //led�Ƴ�ʼ��	
	
	UART_HIMI_INIT();//������ ������ʼ��  C4  C5(B0,B1)
	Line_GPIO_init(); //��·Ѳ�߳�ʼ��  
	
	Time_init();      /*���ж϶�ʱ��5ms*/
	SMD_PWM_Init(15);//������� ���ٶ�
  En_control(0,0);//ֹͣ���ȴ�ָ�ʼ����
	
  ROM_IntMasterEnable();  /*��ȫ���ж�*/	
		while(1)
		{ 
			HIMI_check();//���������,wΪ������ԣ�����ĵƶ�����
		}


}


















