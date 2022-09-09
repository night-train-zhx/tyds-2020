#include "system_init.h"

int time_ms[3]={0,0,0};  // ��ͬʱ��
float way_cm[3]={0,0,0};  // ��ͬ���룬��λdm,ע����float
float fre_Cha[3]={0,0,0}; //�ٶ�
int time_flag=0;  //��ʼ��ʱ��־
int way_flag=0; //·�̱�־


void TIMER4A_Handler(void)				//ϵͳ�����жϺ���
{
	 static uint32_t tick=0;

		
	//�������� ����ʱ��� �����ٶ�
	if(time_flag)  
	{
	  if( time_ms[0] > 0)
		{
			frequency = fre_Cha[0]*spd_fre; //�ٶ�ת��ΪƵ��
			time_ms[0] -=5;		
		}
		if( (time_ms[1] > 0)&&(time_ms[0] <= 0)  )
		{
		frequency = fre_Cha[1]*spd_fre;
		time_ms[1] -=5;	
			
		}
		if((time_ms[2]>0)&&(time_ms[1]<=0)&& (time_ms[0]<=0) )
		{
			
			frequency = fre_Cha[2]*spd_fre;
			time_ms[2] -=5;		
		}
		 
		if((time_ms[2]<=0)&&(time_ms[1]<=0)&& (time_ms[0]<=0))
		{			
			time_flag = 0 ;
			frequency=15*spd_fre;
			En_control(0,0); //ֹͣ
		}
	}

	
	
	if(way_flag)
	{	
		//�������� ����·�̶� �����ٶ�
		 if( way_cm[0] > 0)
		{
			frequency = fre_Cha[0]*spd_fre; //�ٶ�ת��ΪƵ��
			way_cm[0] -=(float)fre_Cha[0]*0.005f; //�ٶ�*ʱ��	
		}
		if( (way_cm[1] > 0)&&(way_cm[0] <= 0)  )
		{
			frequency = fre_Cha[1]*spd_fre;
			way_cm[1] -=(float)fre_Cha[1]*0.005f; 			
		}
			
		if((way_cm[2]>0)&&(way_cm[1]<=0) &&(way_cm[0]<=0) )
		{		
			frequency = fre_Cha[2]*spd_fre;
			way_cm[2] -= fre_Cha[2]*0.005f;  		
		}		 
		if((way_cm[2]<=0) &&(way_cm[1]<=0) &&(way_cm[0]<=0) )
		{			
		way_flag = 0 ;
		frequency=15*spd_fre;
		En_control(0,0); //ֹͣ
		}		
	}

//�޸Ķ�ʱ�� ���޸� �˴�
	
	 Get_line_val(); //�ɼ�5·���⣬���
	 power_control();
	
		if(++tick>=1500)
		tick=0;
		TimerIntClear(TIMER4_BASE,TIMER_TIMA_TIMEOUT);
		
}

void Time_init(void)//ϵͳ���ȶ�ʱ����ʼ��
{
	
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);							//��ʱ��4ʹ��				
  ROM_TimerConfigure(TIMER4_BASE,TIMER_CFG_PERIODIC);						//32λ���ڶ�ʱ��				
  ROM_TimerLoadSet(TIMER4_BASE,TIMER_A,ROM_SysCtlClockGet()/200);		//�趨װ��ֵ, 10ms				
  ROM_IntEnable(INT_TIMER0A);																		//���ж�ʹ��				
  ROM_TimerIntEnable(TIMER4_BASE,TIMER_TIMA_TIMEOUT); 					//�ж����, ����ģʽ;			
  TimerIntRegister(TIMER4_BASE,TIMER_A,TIMER4A_Handler);		//�жϺ���ע��
  ROM_IntMasterEnable();			
  ROM_TimerEnable(TIMER4_BASE,TIMER_A); 												//��ʱ��ʹ�ܿ�ʼ����

}

void Prepare_ALL(void)
{

			/*����ϵͳ��ƵΪ80M*/
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);	
//	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN);

	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();	
	LED_Init();       //led�Ƴ�ʼ��
    	
	UART0_Init(115200);  //û�п����ж�
		
//	Time_init();      /*���ж϶�ʱ��5ms*/
	
	ROM_IntMasterEnable();  /*��ȫ���ж�*/

}




