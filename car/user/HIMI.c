#include "HIMI.h"
#include "led.h"
#include "sysconfig.h"

/*******************通信格式****************
*****单片机发送：指令 + 0xFF0XFF0XFF************
*****单片机接收：类型符+数据+ Receive_END***************/



int deal_flag=0;  //处理数据标志
int HIMI_Buffer[data_limit]; //接收数据缓存


uint8_t buffer_Page[]="page main";  //待发送命令
uint8_t buffer_Page2[]="page button";

uint8_t num_show[]="con.n0.val=";
uint8_t tx[]="t0.bco=63488";
/*
*************************************************************
*   函 数 名: HIMI_IntHandler   
*   功能说明: 串口屏UART 中断函数，输入缓存
*   注    意：要在 数据处理含函数中清除 UART中断标志位
*************************************************************
*/

int i=0;
void HIMI_IntHandler()
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(HIMI_UART_BASE, true);//获取当前的中断状态	
	//根据中断标志处理相关事件
	if(ulStatus & UART_INT_RX)
	{
		while(UARTCharsAvail(HIMI_UART_BASE))
			 HIMI_Buffer[i++] = ROM_UARTCharGet(HIMI_UART_BASE);//接收一个字符   
		
		if(HIMI_Buffer[i-1] == Receive_END ) //接收到结束符
		{
			deal_flag=1;  //解析数据标志置位
			
		}
				
		ROM_UARTIntClear(HIMI_UART_BASE, ulStatus);//清除 UART 中断源

   }
}
/*
*************************************************************
*   函  数  名: UART_HIMI_INIT   
*   功能说明: 串口屏UART初始化
*    波特率 ： 9600
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
	ROM_IntEnable(HIMI_INT_UART);//使能UART1中断
	ROM_UARTIntEnable(HIMI_UART_BASE, UART_INT_RX);
	
	IntPrioritySet(HIMI_INT_UART, 0x0); //最高优先级
	UARTIntRegister(HIMI_UART_BASE, HIMI_IntHandler);
	
	ROM_IntMasterEnable();  /*开全局中断*/
}

/*
*************************************************************
*   函  数  名: get_num   
*   功能说明: 接收数字，可配合小端模式，配合 prints
*   入口参数: 转化后的数字，接收缓冲
*   返 回 值: 无
*************************************************************
*/
int  num_receive_flag =1;
int get_num( int numBuffer[2])
{
 return  numBuffer[0]*256 + numBuffer[1];

}



/*
*************************************************************
*   函  数  名: send_num   
*   功能说明: 发送数字
*   入口参数: char命令，个数，数字
*   返 回 值: 无
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
*   函  数  名: get_direction   
*   功能说明: 
*   入口参数: 转化后的数字，接收缓冲
*   返 回 值: 无
*************************************************************
*/
int dir_receive_flag =1;
void get_direction(int dir,int numBuffer[2])
{


}




/*******发送指令*******/
void HIMI_Send(uint8_t *str)
{
   while(*str !=0)
       UARTCharPut(HIMI_UART_BASE,*(str++));
	 
	 /*******发送结束符******/
   for(int i=0;i<3;i++)
    UARTCharPut(HIMI_UART_BASE,0xFF);
}



/*
*************************************************************
*   函 数 名: HIMI_check   
*   功能说明: 处理串口屏发来的指令
*   注    意：要在 数据处理后清除清除 UART中断标志位
*************************************************************
*/
int number;
int flag[9]={0,0,0,0,0,0,0,0,0};
void HIMI_check(void)
{
 if(deal_flag)
 {
	 //设置速度
	 if(HIMI_Buffer[0]== 0xAA) //数字2
	 { 
		fre_Cha[0] = (HIMI_Buffer[2]*256+HIMI_Buffer[1])  ;
		 flag[0]=1;
	 }
	 
	 if(HIMI_Buffer[0]== 0xAB) //数字2
	 { 
		fre_Cha[1] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]) ;
		 flag[1]=1;
		 red_off;
	 }
	 if(HIMI_Buffer[0]== 0xAC) //数字2
	 { 
			fre_Cha[2] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]);
		 flag[2]=1;
	 }

	 
	 //设置时间段
	 if(HIMI_Buffer[0]== 0xFA) //数字2
	 { 
		 time_ms[0] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]);
		 flag[3]=1;
	 }
	 	if(HIMI_Buffer[0]== 0xFB) //数字2
	 { 
		 time_ms[1] = (HIMI_Buffer[2]*256+HIMI_Buffer[1])  ;
		 flag[4]=1;
	 }
	 if(HIMI_Buffer[0]== 0xFC) //数字2
	 { 
		 time_ms[2] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]) ;
		 flag[5]=1;
	 }
	 
	 
	  //设置距离段
	 if(HIMI_Buffer[0]== 0xEA) //数字2
	 { 
		 way_cm[0] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]);
		 flag[6]=1;
	 }
	 if(HIMI_Buffer[0]== 0xEB) //数字2
	 { 
		 way_cm[1] = (HIMI_Buffer[2]*256+HIMI_Buffer[1])  ;
		 flag[7]=1;
	 }
	 if(HIMI_Buffer[0]== 0xEC) //数字2
	 { 
		 way_cm[2] = (HIMI_Buffer[2]*256+HIMI_Buffer[1]) ;
		 flag[8]=1;
	 }
	 
	 
	 if(flag[0]&& flag[1]&& flag[2]&& flag[3]&& flag[4]&& flag[5] )  // 三个速度，三个时间或者三个距离段 已经设置完毕
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
	 
	 if(flag[6]&& flag[7]&& flag[8]&& flag[0]&& flag[1]&& flag[2] )  // 三个速度，三个时间或者三个距离段 已经设置完毕
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
	 

 	 if(HIMI_Buffer[0]== 0xBA) //数字2，开始发车
	 { 
			En_control(1,1); 
	 }
	 
	 if(HIMI_Buffer[0]== 0xBB) //数字2，停止
	 { 
		 En_control(0,0); 
	 }	  	 
	 
	 deal_flag=0;   //重置标志位
	 i=0;

 }
}






