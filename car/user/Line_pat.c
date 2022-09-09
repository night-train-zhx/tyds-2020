#include "Line_pat.h"
//#include "SMD.h"

uint32_t col_val[7];
int turn_flag = 0; 
/*
*************************************************************
*   ����˵��: 5·ADC ��ʼ��
*   ��   �� ��PE3,PE2,PE1,PE0,PD3 
*************************************************************
*/
void Line_ADC_init(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
       while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) ;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
	
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    // Configure the GPIOE 0,1,2,3   PD3 to peripheral function.
		 GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 |GPIO_PIN_2 |GPIO_PIN_1 |GPIO_PIN_0);
		 GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3 );
	
	
    // Sets the clock configuration for the ADC: use piosc��16M��, full rate.
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_FULL, 1);

    // Set the reference voltage.
    // Attention: tm4c123x default reference voltage is 3.3v, can't change.
    ADCReferenceSet(ADC0_BASE, ADC_REF_INT);

    // Enable the first sample sequencer to capture the value of channel 0
    // and channel 1 when the processor trigger occurs.
	//                           ���з�������       �������         ���ȼ�0~3  
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure a step of the sample sequencer.
    //! \param ui32SequenceNum is the sample sequence number. ���з�������
    //! \param ui32Step is the step to be configured.
    //! \param ui32Config is the configuration of this step;
    // *Specific information refer to data sheet and drverlib.
		//                                       ͨ��0��AIN0 PE3�� ����      �ж�

		ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3);
	  ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH4 |ADC_CTL_END | ADC_CTL_IE);

     // param ui32Factor is the number of samples to be averaged. ƽ��ֵ ֻ����2�ı��������64��0��ʱ���ֹ������
    ADCHardwareOversampleConfigure(ADC0_BASE, 8);

    ADCSequenceEnable(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 0);
}

/*
*************************************************************
*   ����˵��: ��ȡ5· ADC ֵ
*   ��ڲ���: ����
*   ���ز���: ����
*************************************************************
*/
uint32_t *Get_Adc(uint32_t *ulADC0_Value)
{
    
		//�����ɼ�
    ADCProcessorTrigger(ADC0_BASE, 0);
		//�ȴ��ɼ�����
    while(!ADCIntStatus(ADC0_BASE, 0, false)) ;
		ADCIntClear(ADC0_BASE, 0);
		//��ȡ�ɼ����  0-4095����0-3.3V
    ADCSequenceDataGet(ADC0_BASE, 0, ulADC0_Value);
	
		//UARTprintf("%d\n",ValueAvg);
    return ulADC0_Value;
}

/*
*************************************************************
*   ����˵��: 5·��ƽ��� ��ʼ��
*   ��   �� ��PE3,PE2,PE1,,PD3 PD2 ����  PD1  ������ PB5
*************************************************************
*/
void Line_GPIO_init(void)
{

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//ʹ�� PB �˿�
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3);//���ùܽ����� GPIO ���
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2);//���ùܽ����� GPIO ���
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);//���ùܽ����� GPIO ���

	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_3);//���ùܽ����� GPIO ��
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_2);//���ùܽ����� GPIO ��
	  //����ģ��
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_2);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_5);
	
	//������
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_7);
  buzzer_off;	
	
}

/*
*************************************************************
*   ����˵��: ��ȡ5· ���� ֵ
*   ��ڲ���: ����
*   ���ز���: ����  1������߼�⵽
*************************************************************
*/
void Get_line_val(void)
{
	
  for(int i=0;i<7;i++)
			col_val[i]=0;    // Ĭ��û�м�⵽
    
	//�����ƽ��0  ����,��⵽��ֵΪ1
	if(  GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3)==0)
		col_val[0] =1;
	if(!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2))
		col_val[1] = 1;
	if (! GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1))
			col_val[2] =1;
	if(! GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3))
		 col_val[3]=1;
	if(! GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_2))
		col_val[4]=1;
	
	if(! GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_5))
		col_val[6]=1;
	if(! GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_2))
		col_val[5]=1;	
	
	
 	if(col_val[6]||col_val[5]) //����ģ��,����������
	{
		buzzer_on;  //������Ӧ
		blue_on;   //����Ӧ
	}
	else
	{
		buzzer_off;
	  blue_off;
	}
	
}


/*
*************************************************************
*   ����˵��: Ѳ�߿����㷨
*   ��ڲ���: 5· �ߵ� ��ƽ����
*   ���ز���: ��
*     ע �� 
*************************************************************
*/
float std_fre=2000.0f;//stand frequency ,��ʼƵ�ʣ�Ҳ��  error �� �ο����������Բ������
float frequency=2000.0f;
float pro1=1.0f,pro2=1.0f;  //���������� ��ͬ�ٶ� ���Ե�����
float error=0.0f;
float way=0;//�ܾ���

void power_control(void)
{
	if(col_val[2]==1)  //�м��⵽
	{
		error=0;
	}
	if(col_val[3]==1)  //��you�߼�⵽
	{
		error=-30*15;

	}
	if(col_val[1]==1)  //ƫ���м�
	{
		error=30*15;
		
	}
	if(col_val[4])  //&&col_val[3] ���߶���⵽��ת��
	{
		error=-60*15;
	}		
	if(col_val[0])  // &&col_val[1]��������⵽��תȦ  ֱ�ǿ���    ��Ǵ�ʵ��  ��Ǳ���Ҫ5·
	{
		error=60*15;	
	}
	
	if((col_val[1]+col_val[2]+col_val[3]+col_val[4]+col_val[0])>=5)  //����4��ɫ�鶼Ҫ��⵽  ֹͣ
	{	
		if(!way_flag)
			 En_control(0,0);				
	}
	
	pro1 = frequency/ std_fre;
	pro2 = frequency/ std_fre;
	
  SMD_Control(frequency - pro1*error,frequency + pro2*error);
	
	
}
