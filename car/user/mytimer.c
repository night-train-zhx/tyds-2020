
#include "mytimer.h"
#include "led.h"
/*******************************

	SysTick  ����Ƶ�� 1Mhz

*******************************/

static volatile uint32_t counter = 0;

void SysTick_Handler(void)  //ÿ��1us������ж� ����ֵ��һ
{
	counter++;  
}


void SysTick_Init()    /*��ʱ1us*/
{
	ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / 1000000UL);  /*1Mhz*/
	
  ROM_SysTickIntEnable();  /*Enable the SysTick Interrupt.*/

  ROM_SysTickEnable();   /*Enable SysTick.*/		
}


uint32_t millis(void)//���ص�ǰʱ��  ��λms
{
		return counter / 1000UL;
}

uint32_t micros(void) //���ص�ǰʱ��  ��λus
{
		return counter;
}



void Test_Period(Testime *Time_Lab)
{
	Time_Lab->Last_Time = Time_Lab->Now_Time;  //ʱ����� 
	Time_Lab->Now_Time = micros()/1000.0f; //���浱ǰʱ�� ��λ ms
	Time_Lab->Time_Delta = (Time_Lab->Now_Time - Time_Lab->Last_Time);//ʱ���� float ms 
	Time_Lab->Time_Delta_INT = (uint16_t)Time_Lab->Time_Delta; //ʱ���� uint16_t ms 
	
}

/*
*************************************************************
*   ��  ��  ��: TimeX_init   
*   ����˵��: 32 λ��ʱ����ʼ����
*   ��ڲ���: TIMER  �ṹ��
*************************************************************
*/

TIMER timer4={
	  SYSCTL_PERIPH_TIMER4,
	  TIMER4_BASE,
    INT_TIMER4A,
	  0x01,     //���ȼ�
	 TIMERX_Handler  //�жϺ���ָ��
};

void TIMERX_Handler(void)				//ϵͳ�����жϺ���
{
	static uint32_t tick=0;
		
	if(tick%100==0)  //500ms	
		Led_Flash(3);//ָʾ����˸����������������

	if(++tick>=1500)
		tick=0;
			
	TimerIntClear(TIMER4_BASE,TIMER_TIMA_TIMEOUT);
	
}
void TimeX_init( TIMER timerX )//ϵͳ���ȶ�ʱ����ʼ��
{
  ROM_SysCtlPeripheralEnable(timerX.SYSCTL_PERIPH_TIMER);							//��ʱ��4ʹ��	
	
  ROM_TimerConfigure(timerX.TIMER_BASE,TIMER_CFG_PERIODIC);						//32λ���ڶ�ʱ��				
  ROM_TimerLoadSet(timerX.TIMER_BASE,TIMER_A,ROM_SysCtlClockGet()/100-1);		//�趨װ��ֵ, 5ms
	
  ROM_IntEnable(INT_TIMER0A);																		//���ж�ʹ��				
  ROM_TimerIntEnable(timerX.TIMER_BASE,TIMER_TIMA_TIMEOUT); 					//�ж����, ����ģʽ;			
  TimerIntRegister(timerX.TIMER_BASE,TIMER_A,TIMERX_Handler);		//�жϺ���ע��

  ROM_IntMasterEnable();	
 ROM_TimerEnable(timerX.TIMER_BASE,TIMER_A); 		
	
	IntPrioritySet(timerX.INT_TIMER, timerX.Priority);

}



