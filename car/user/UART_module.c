
#include "UART_module.h"

void UART0_Int_Handler(void)    /*串口0中断函数*/
{
   //中断标志的读取、处理和清除
    unsigned long ulStatus;
    char cThisChar;
//    ROM_IntMasterDisable();//禁止全局中断
    ulStatus = UARTIntStatus(UART0_BASE, true);//获取当前的中断状态	
    ROM_UARTIntClear(UART0_BASE, ulStatus);//清除 UART 中断源

    //根据中断标志处理相关事件
    if(ulStatus & UART_INT_RX)
    {
			cThisChar = ROM_UARTCharGet(UART0_BASE);//接收一个字符
      ROM_UARTCharPut(UART0_BASE,cThisChar);  //回显
    }
		

}

/*
*************************************************************
*   函  数  名: ConfigUART0   
*   功能说明: UART初始化函数,使用虚拟串口   115200
*   入口参数: none
*   返  回  值: none
*************************************************************
*/
// This function sets up UART0 to be used for a console to display information
void ConfigureUART0(void)       
{
	   
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0, 115200, 16000000);
	
//		 //UART0 中断配置
//	 ROM_UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX7_8, UART_FIFO_RX1_8);//触发电平设置
//	 UARTIntRegister(UART0_BASE, UART0_Int_Handler);//注册一个 UART 中断的中断处理程序
//	 ROM_UARTIntEnable(UART0_BASE, UART_INT_RX);
//	 //使UART0传输数据使能
//	 ROM_UARTIntEnable(UART0_BASE, UART_INT_TX);
//	  ROM_IntMasterEnable();//使能全局中断

}
/*
*************************************************************
*   函  数  名: ConfigUART1   
*   功能说明: UART1  初始化函数
*************************************************************
*/
void UART1_Int_Handler(void)    
{
	uint8_t thischar=0;
    //获取中断标志 原始中断状态 屏蔽中断标志
   uint32_t flag = ROM_UARTIntStatus(UART1_BASE,1);
    //清除中断标志
    ROM_UARTIntClear(UART1_BASE,flag);
    if(flag&UART_INT_RX)
		{ //UARTCharsAvail()判断FIFO是否还有数据
        while(UARTCharsAvail(UART1_BASE))
        {

				thischar=ROM_UARTCharGet(UART1_BASE);
				ROM_UARTCharPut(UART1_BASE,thischar);
					
				}
		}		
}

  void UART1_Init(unsigned long ulBaud)/*串口1初始化*/
{

		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);  
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //使能GPIO外设

    ROM_GPIOPinConfigure(GPIO_PB0_U1RX);    //GPIO模式配置 PB0--RX PB1--TX
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);  
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);  //GPIO的UART模式配置
  
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(),ulBaud,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                    UART_CONFIG_PAR_NONE));  //UART协议配置 波特率ulBaud 8位 1停止位  无校验位
					
					
    //UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
    //禁用后接收1位就产生中断
    ROM_UARTFIFODisable(UART1_BASE);   
    ROM_IntEnable(INT_UART1);//使能UART1中断  
    ROM_UARTIntEnable(UART1_BASE,UART_INT_RX); //使能UART1接收中断
 
    UARTIntRegister(UART1_BASE,UART1_Int_Handler);   //UART中断地址注册
//		ROM_IntMasterEnable();  /*开全局中断*/
}	 


/*
*************************************************************
*   函  数  名: ConfigUART2  
*   功能说明: UART2  初始化函数   D6  D7
*************************************************************
*/
void UART2_Int_Handler(void)    /*串口2中断函数*/
{
    //获取中断标志 原始中断状态 屏蔽中断标志
    uint32_t flag = ROM_UARTIntStatus(UART2_BASE,1);
    //清除中断标志
    ROM_UARTIntClear(UART2_BASE,flag);
    if(flag&UART_INT_RX)
        //UARTCharsAvail()判断FIFO是否还有数据
        while(UARTCharsAvail(UART2_BASE))
        {

        }
}
void UART2_Init(unsigned long ulBaud)
{

	 ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);   
	 ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); //使能GPIO外设

	 ROM_GPIOPinConfigure(GPIO_PD6_U2RX);   
	 ROM_GPIOPinConfigure(GPIO_PD7_U2TX);
	 ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);  //GPIO的UART模式配置

	 ROM_UARTConfigSetExpClk(UART2_BASE, ROM_SysCtlClockGet(),ulBaud,
					(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
									UART_CONFIG_PAR_NONE));  //UART协议配置 波特率ulBaud 8位 1停止位  无校验位
					
					
    //UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
    //禁用后接收1位就产生中断
    ROM_UARTFIFODisable(UART2_BASE);    
    ROM_IntEnable(INT_UART2);//使能UART2中断   
    ROM_UARTIntEnable(UART2_BASE,UART_INT_RX); //使能UART2接收中断
 
    UARTIntRegister(UART2_BASE,UART2_Int_Handler);   //UART中断地址注册
//		ROM_IntMasterEnable();  /*开全局中断*/
}	



/*
*************************************************************
*   函  数  名: UARTX_INIT   
*   功能说明: 备用 UART初始化函数
*            请在使用前 更新 宏定义
*************************************************************
*/

void IntHandler()
{
	char thischar;
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(UART_BASE, true);//获取当前的中断状态	
	//根据中断标志处理相关事件
	if(ulStatus & UART_INT_RX)
	{
		while(UARTCharsAvail(UART_BASE))
		   thischar	= ROM_UARTCharGet(UART_BASE);//接收一个字符   
		ROM_UARTCharPut(UART_BASE,thischar);
		ROM_UARTIntClear(UART_BASE, ulStatus);//清除 UART 中断源

   }
}

void UARTX_INIT(unsigned long ulBaud)
{

	SysCtlPeripheralEnable(SYS_PERIPH);
		while(!SysCtlPeripheralReady(SYS_PERIPH));
	SysCtlPeripheralEnable(SYSCTL_GPIO);
		while(!SysCtlPeripheralReady(SYSCTL_GPIO));
	
	GPIOPinConfigure(GPIO_RX);
	GPIOPinConfigure(GPIO_TX);
	GPIOPinTypeUART(PORT_BASE, PINs);

	UARTConfigSetExpClk(UART_BASE,  ROM_SysCtlClockGet(), ulBaud,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	
	ROM_UARTFIFODisable(UART_BASE);
//	ROM_UARTFIFOLevelSet(UART5_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);//触发电平设?
	
	
	ROM_IntEnable(INT_UART);//使能UART1中断
	ROM_UARTIntEnable(UART_BASE, UART_INT_RX);
	
	UARTIntRegister(UART_BASE, IntHandler);
			
	ROM_IntMasterEnable();  /*开全局中断*/

}



