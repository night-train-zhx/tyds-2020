#include "blue_tooth.h"





void UART1_Handler(void)
{
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART1_BASE, true);
   
		while(UARTCharsAvail(UART1_BASE))
			{
				
			}
			 UARTIntClear(UART1_BASE, ui32Status);
			//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
			//UARTprintf("%x %x %x\n\n\n",ReciveData_UART1[0],ReciveData_UART1[1],ReciveData_UART1[2]);
}		


/*
*************************************************************
*   功能说明: 串口1用来与遥控器进行通信（必备）
*   端   口 ： RX PB0   TX  PB1 
*************************************************************
*/
void Uart1Iint(void)
{
     SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		 GPIOPinConfigure(GPIO_PB0_U1RX);
     GPIOPinConfigure(GPIO_PB1_U1TX);
     GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
     UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,
                                  (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                   UART_CONFIG_PAR_NONE));
	   UARTStdioConfig(1, 9600, SysCtlClockGet());	 
   
//		 //优先级根据实际情况配置，若不配置优先级，则默认为中断向量表中的优先级
	//	 IntPrioritySet(INT_UART1,10);
//     UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
//	  IntEnable(INT_UART1);
//    IntMasterEnable();
}
