
#include "UART_module.h"

void UART0_Int_Handler(void)    /*����0�жϺ���*/
{
   //�жϱ�־�Ķ�ȡ����������
    unsigned long ulStatus;
    char cThisChar;
//    ROM_IntMasterDisable();//��ֹȫ���ж�
    ulStatus = UARTIntStatus(UART0_BASE, true);//��ȡ��ǰ���ж�״̬	
    ROM_UARTIntClear(UART0_BASE, ulStatus);//��� UART �ж�Դ

    //�����жϱ�־��������¼�
    if(ulStatus & UART_INT_RX)
    {
			cThisChar = ROM_UARTCharGet(UART0_BASE);//����һ���ַ�
      ROM_UARTCharPut(UART0_BASE,cThisChar);  //����
    }
		

}

/*
*************************************************************
*   ��  ��  ��: ConfigUART0   
*   ����˵��: UART��ʼ������,ʹ�����⴮��   115200
*   ��ڲ���: none
*   ��  ��  ֵ: none
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
	
//		 //UART0 �ж�����
//	 ROM_UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX7_8, UART_FIFO_RX1_8);//������ƽ����
//	 UARTIntRegister(UART0_BASE, UART0_Int_Handler);//ע��һ�� UART �жϵ��жϴ������
//	 ROM_UARTIntEnable(UART0_BASE, UART_INT_RX);
//	 //ʹUART0��������ʹ��
//	 ROM_UARTIntEnable(UART0_BASE, UART_INT_TX);
//	  ROM_IntMasterEnable();//ʹ��ȫ���ж�

}
/*
*************************************************************
*   ��  ��  ��: ConfigUART1   
*   ����˵��: UART1  ��ʼ������
*************************************************************
*/
void UART1_Int_Handler(void)    
{
	uint8_t thischar=0;
    //��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־
   uint32_t flag = ROM_UARTIntStatus(UART1_BASE,1);
    //����жϱ�־
    ROM_UARTIntClear(UART1_BASE,flag);
    if(flag&UART_INT_RX)
		{ //UARTCharsAvail()�ж�FIFO�Ƿ�������
        while(UARTCharsAvail(UART1_BASE))
        {

				thischar=ROM_UARTCharGet(UART1_BASE);
				ROM_UARTCharPut(UART1_BASE,thischar);
					
				}
		}		
}

  void UART1_Init(unsigned long ulBaud)/*����1��ʼ��*/
{

		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);  
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //ʹ��GPIO����

    ROM_GPIOPinConfigure(GPIO_PB0_U1RX);    //GPIOģʽ���� PB0--RX PB1--TX
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);  
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);  //GPIO��UARTģʽ����
  
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(),ulBaud,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                    UART_CONFIG_PAR_NONE));  //UARTЭ������ ������ulBaud 8λ 1ֹͣλ  ��У��λ
					
					
    //UART����FIFO Ĭ��FIFO LevelΪ4/8 �Ĵ�����8�ֽں�����ж�
    //���ú����1λ�Ͳ����ж�
    ROM_UARTFIFODisable(UART1_BASE);   
    ROM_IntEnable(INT_UART1);//ʹ��UART1�ж�  
    ROM_UARTIntEnable(UART1_BASE,UART_INT_RX); //ʹ��UART1�����ж�
 
    UARTIntRegister(UART1_BASE,UART1_Int_Handler);   //UART�жϵ�ַע��
//		ROM_IntMasterEnable();  /*��ȫ���ж�*/
}	 


/*
*************************************************************
*   ��  ��  ��: ConfigUART2  
*   ����˵��: UART2  ��ʼ������   D6  D7
*************************************************************
*/
void UART2_Int_Handler(void)    /*����2�жϺ���*/
{
    //��ȡ�жϱ�־ ԭʼ�ж�״̬ �����жϱ�־
    uint32_t flag = ROM_UARTIntStatus(UART2_BASE,1);
    //����жϱ�־
    ROM_UARTIntClear(UART2_BASE,flag);
    if(flag&UART_INT_RX)
        //UARTCharsAvail()�ж�FIFO�Ƿ�������
        while(UARTCharsAvail(UART2_BASE))
        {

        }
}
void UART2_Init(unsigned long ulBaud)
{

	 ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);   
	 ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); //ʹ��GPIO����

	 ROM_GPIOPinConfigure(GPIO_PD6_U2RX);   
	 ROM_GPIOPinConfigure(GPIO_PD7_U2TX);
	 ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);  //GPIO��UARTģʽ����

	 ROM_UARTConfigSetExpClk(UART2_BASE, ROM_SysCtlClockGet(),ulBaud,
					(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
									UART_CONFIG_PAR_NONE));  //UARTЭ������ ������ulBaud 8λ 1ֹͣλ  ��У��λ
					
					
    //UART����FIFO Ĭ��FIFO LevelΪ4/8 �Ĵ�����8�ֽں�����ж�
    //���ú����1λ�Ͳ����ж�
    ROM_UARTFIFODisable(UART2_BASE);    
    ROM_IntEnable(INT_UART2);//ʹ��UART2�ж�   
    ROM_UARTIntEnable(UART2_BASE,UART_INT_RX); //ʹ��UART2�����ж�
 
    UARTIntRegister(UART2_BASE,UART2_Int_Handler);   //UART�жϵ�ַע��
//		ROM_IntMasterEnable();  /*��ȫ���ж�*/
}	



/*
*************************************************************
*   ��  ��  ��: UARTX_INIT   
*   ����˵��: ���� UART��ʼ������
*            ����ʹ��ǰ ���� �궨��
*************************************************************
*/

void IntHandler()
{
	char thischar;
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(UART_BASE, true);//��ȡ��ǰ���ж�״̬	
	//�����жϱ�־��������¼�
	if(ulStatus & UART_INT_RX)
	{
		while(UARTCharsAvail(UART_BASE))
		   thischar	= ROM_UARTCharGet(UART_BASE);//����һ���ַ�   
		ROM_UARTCharPut(UART_BASE,thischar);
		ROM_UARTIntClear(UART_BASE, ulStatus);//��� UART �ж�Դ

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
//	ROM_UARTFIFOLevelSet(UART5_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);//������ƽ��?
	
	
	ROM_IntEnable(INT_UART);//ʹ��UART1�ж�
	ROM_UARTIntEnable(UART_BASE, UART_INT_RX);
	
	UARTIntRegister(UART_BASE, IntHandler);
			
	ROM_IntMasterEnable();  /*��ȫ���ж�*/

}



