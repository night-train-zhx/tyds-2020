#include "HIMI.h"
#include "led.h"
#include "sysconfig.h"

/*******************ͨ�Ÿ�ʽ****************
*****��Ƭ�����ͣ�ָ�� + 0xFF0XFF0XFF************
*****��Ƭ�����գ����ͷ�+����+ Receive_END***************/



int deal_flag=0;  //�������ݱ�־
int HIMI_Buffer[data_limit]; //�������ݻ���


uint8_t buffer_Page[]="page main";  //����������
uint8_t buffer_Page2[]="page button";

uint8_t num_show[]="con.n0.val=";
uint8_t tx[]="t0.bco=63488";
/*
*************************************************************
*   �� �� ��: HIMI_IntHandler   
*   ����˵��: ������UART �жϺ��������뻺��
*   ע    �⣺Ҫ�� ���ݴ������������ UART�жϱ�־λ
*************************************************************
*/

int i=0;
void HIMI_IntHandler()
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(HIMI_UART_BASE, true);//��ȡ��ǰ���ж�״̬	
	//�����жϱ�־��������¼�
	if(ulStatus & UART_INT_RX)
	{
		while(UARTCharsAvail(HIMI_UART_BASE))
			 HIMI_Buffer[i++] = ROM_UARTCharGet(HIMI_UART_BASE);//����һ���ַ�   
		
		if(HIMI_Buffer[i-1] == Receive_END ) //���յ�������
		{
			deal_flag=1;  //�������ݱ�־��λ
			
		}
				
		ROM_UARTIntClear(HIMI_UART_BASE, ulStatus);//��� UART �ж�Դ

   }
}
/*
*************************************************************
*   ��  ��  ��: UART_HIMI_INIT   
*   ����˵��: ������UART��ʼ��
*    ������ �� 9600
*************************************************************
*/
void UART_HIMI_INIT(void)
{

	SysCtlPeripheralEnable(HIMI_SYS_PERIPH);
		while(!SysCtlPeripheralReady(HIMI_SYS_PERIPH));
	SysCtlPeripheralEnable(HIMI_SYSCTL_GPIO);
		while(!SysCtlPeripheralReady(HIMI_SYSCTL_GPIO));
	

	GPIOPinConfigure(HIMI_GPIO_RX);
	GPIOPinConfigure(HIMI_GPIO_TX);
	GPIOPinTypeUART(HIMI_PORT_BASE, HIMI_PINs);

	UARTConfigSetExpClk(HIMI_UART_BASE,  ROM_SysCtlClockGet(), 9600,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	
	ROM_UARTFIFODisable(HIMI_UART_BASE);
	ROM_IntEnable(HIMI_INT_UART);//ʹ��UART1�ж�
	ROM_UARTIntEnable(HIMI_UART_BASE, UART_INT_RX);
	
	IntPrioritySet(HIMI_INT_UART, 0x0); //������ȼ�
	UARTIntRegister(HIMI_UART_BASE, HIMI_IntHandler);
	
	ROM_IntMasterEnable();  /*��ȫ���ж�*/
}

/*
*************************************************************
*   ��  ��  ��: get_num   
*   ����˵��: �������֣������С��ģʽ����� prints
*   ��ڲ���: ת��������֣����ջ���
*   �� �� ֵ: ��
*************************************************************
*/
int  num_receive_flag =1;
int get_num( int numBuffer[2])
{
 return  numBuffer[0]*256 + numBuffer[1];

}



/*
*************************************************************
*   ��  ��  ��: send_num   
*   ����˵��: ��������
*   ��ڲ���: char�������������
*   �� �� ֵ: ��
*************************************************************
*/

void HIMI_NtC(const int64_t temp,char on)
{
    int64_t temp_p = temp;

    if(temp_p < 0)
    {
        UARTCharPut(HIMI_UART_BASE,'-');
        temp_p = 0-temp_p;
    }
    if(temp_p > 0)
    {
        HIMI_NtC(temp_p/10,0);
        UARTCharPut(HIMI_UART_BASE,temp_p%10 + '0');
    }
    else if(temp_p == 0)
    {
        if(on)
            UARTCharPut(HIMI_UART_BASE,'0');
   
    }
}
void send_num( uint8_t *numBuffer,int number)
{
  while(*numBuffer !=0)
      UARTCharPut(HIMI_UART_BASE,*(numBuffer++));
	 
	HIMI_NtC(number,1);

  for(int i=0;i<3;i++)
    UARTCharPut(HIMI_UART_BASE,0xFF);
}


/*
*************************************************************
*   ��  ��  ��: get_direction   
*   ����˵��: 
*   ��ڲ���: ת��������֣����ջ���
*   �� �� ֵ: ��
*************************************************************
*/
int dir_receive_flag =1;
void get_direction(int dir,int numBuffer[2])
{


}




/*******����ָ��*******/
void HIMI_Send(uint8_t *str)
{
   while(*str !=0)
       UARTCharPut(HIMI_UART_BASE,*(str++));
	 
	 /*******���ͽ�����******/
   for(int i=0;i<3;i++)
    UARTCharPut(HIMI_UART_BASE,0xFF);
}



/*
*************************************************************
*   �� �� ��: HIMI_check   
*   ����˵��: ��������������ָ��
*   ע    �⣺Ҫ�� ���ݴ���������� UART�жϱ�־λ
*************************************************************
*/
int number;
int flag[9]={0,0,0,0,0,0,0,0,0};
void HIMI_check(void)
{
 if(deal_flag)
 {
	 //�����ٶ�
	 if(HIMI_Buffer[0]== 0xAA) //����2
	 { 
		fre_Cha[0] = (HIMI_Buffer[2]*256+HIMI_Buffer[1])  ;
		 flag[0]=1;
	 }
	 
	 if(HIMI_Buffer[0]== 0xAB) //����2
	 { 
		fre_Cha[1] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]) ;
		 flag[1]=1;
		 red_off;
	 }
	 if(HIMI_Buffer[0]== 0xAC) //����2
	 { 
			fre_Cha[2] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]);
		 flag[2]=1;
	 }

	 
	 //����ʱ���
	 if(HIMI_Buffer[0]== 0xFA) //����2
	 { 
		 time_ms[0] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]);
		 flag[3]=1;
	 }
	 	if(HIMI_Buffer[0]== 0xFB) //����2
	 { 
		 time_ms[1] = (HIMI_Buffer[2]*256+HIMI_Buffer[1])  ;
		 flag[4]=1;
	 }
	 if(HIMI_Buffer[0]== 0xFC) //����2
	 { 
		 time_ms[2] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]) ;
		 flag[5]=1;
	 }
	 
	 
	  //���þ����
	 if(HIMI_Buffer[0]== 0xEA) //����2
	 { 
		 way_cm[0] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]);
		 flag[6]=1;
	 }
	 if(HIMI_Buffer[0]== 0xEB) //����2
	 { 
		 way_cm[1] = (HIMI_Buffer[2]*256+HIMI_Buffer[1])  ;
		 flag[7]=1;
	 }
	 if(HIMI_Buffer[0]== 0xEC) //����2
	 { 
		 way_cm[2] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]) ;
		 flag[8]=1;
	 }
	 
	 
	 if(flag[0]&& flag[1]&& flag[2]&& flag[3]&& flag[4]&& flag[5] )  // �����ٶȣ�����ʱ�������������� �Ѿ��������
	 {
			flag[0]=0;
		  flag[1]=0;
		  flag[2]=0;
		  flag[3]=0;
		  flag[4]=0;
		  flag[5]=0;
		 
			red_off;
			time_flag = 1;
		 En_control(1,1);		 
	 }
	 
	 if(flag[6]&& flag[7]&& flag[8]&& flag[0]&& flag[1]&& flag[2] )  // �����ٶȣ�����ʱ�������������� �Ѿ��������
	 {
			flag[6]=0;
		  flag[7]=0;
		  flag[8]=0;
		  flag[0]=0;
		  flag[1]=0;
		  flag[2]=0;

		 way_flag = 1;
		 En_control(1,1); 
//	  	ROM_TimerEnable(TIMER4_BASE,TIMER_A); 
	 }
	 

 	 if(HIMI_Buffer[0]== 0xBA) //����2����ʼ����
	 { 
			En_control(1,1); 
	 }
	 
	 if(HIMI_Buffer[0]== 0xBB) //����2��ֹͣ
	 { 
		 En_control(0,0); 
	 }	  	 
	 
	 deal_flag=0;   //���ñ�־λ
	 i=0;

 }
}






