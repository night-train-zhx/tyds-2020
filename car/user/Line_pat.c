#include "Line_pat.h"
//#include "SMD.h"

uint32_t col_val[7];
int turn_flag = 0; 
/*
*************************************************************
*   功能说明: 5路ADC 初始化
*   端   口 ：PE3,PE2,PE1,PE0,PD3 
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
	
	
    // Sets the clock configuration for the ADC: use piosc（16M）, full rate.
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_FULL, 1);

    // Set the reference voltage.
    // Attention: tm4c123x default reference voltage is 3.3v, can't change.
    ADCReferenceSet(ADC0_BASE, ADC_REF_INT);

    // Enable the first sample sequencer to capture the value of channel 0
    // and channel 1 when the processor trigger occurs.
	//                           序列发生器号       软件触发         优先级0~3  
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure a step of the sample sequencer.
    //! \param ui32SequenceNum is the sample sequence number. 序列发生器号
    //! \param ui32Step is the step to be configured.
    //! \param ui32Config is the configuration of this step;
    // *Specific information refer to data sheet and drverlib.
		//                                       通道0（AIN0 PE3） 结束      中断

		ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3);
	  ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH4 |ADC_CTL_END | ADC_CTL_IE);

     // param ui32Factor is the number of samples to be averaged. 平均值 只能是2的倍数，最大64，0的时候禁止过采样
    ADCHardwareOversampleConfigure(ADC0_BASE, 8);

    ADCSequenceEnable(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 0);
}

/*
*************************************************************
*   功能说明: 获取5路 ADC 值
*   入口参数: 数组
*   返回参数: 数组
*************************************************************
*/
uint32_t *Get_Adc(uint32_t *ulADC0_Value)
{
    
		//触发采集
    ADCProcessorTrigger(ADC0_BASE, 0);
		//等待采集结束
    while(!ADCIntStatus(ADC0_BASE, 0, false)) ;
		ADCIntClear(ADC0_BASE, 0);
		//获取采集结果  0-4095代表0-3.3V
    ADCSequenceDataGet(ADC0_BASE, 0, ulADC0_Value);
	
		//UARTprintf("%d\n",ValueAvg);
    return ulADC0_Value;
}

/*
*************************************************************
*   功能说明: 5路电平检测 初始化
*   端   口 ：PE3,PE2,PE1,,PD3 PD2 光敏  PD1  蜂鸣器 PB5
*************************************************************
*/
void Line_GPIO_init(void)
{

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能 PB 端口
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3);//配置管脚用作 GPIO 输出
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2);//配置管脚用作 GPIO 输出
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);//配置管脚用作 GPIO 输出

	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_3);//配置管脚用作 GPIO 输
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_2);//配置管脚用作 GPIO 输
	  //光敏模块
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_2);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_5);
	
	//蜂鸣器
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_7);
  buzzer_off;	
	
}

/*
*************************************************************
*   功能说明: 获取5路 引脚 值
*   入口参数: 数组
*   返回参数: 数组  1代表黑线检测到
*************************************************************
*/
void Get_line_val(void)
{
	
  for(int i=0;i<7;i++)
			col_val[i]=0;    // 默认没有检测到
    
	//如果电平是0  黑线,检测到赋值为1
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
	
	
 	if(col_val[6]||col_val[5]) //光敏模块,触发蜂鸣器
	{
		buzzer_on;  //声音响应
		blue_on;   //光响应
	}
	else
	{
		buzzer_off;
	  blue_off;
	}
	
}


/*
*************************************************************
*   功能说明: 巡线控制算法
*   入口参数: 5路 高低 电平数组
*   返回参数: 无
*     注 意 
*************************************************************
*/
float std_fre=2000.0f;//stand frequency ,初始频率，也是  error 的 参考比例，所以不便更改
float frequency=2000.0f;
float pro1=1.0f,pro2=1.0f;  //比例，根据 不同速度 线性调整，
float error=0.0f;
float way=0;//总距离

void power_control(void)
{
	if(col_val[2]==1)  //中间检测到
	{
		error=0;
	}
	if(col_val[3]==1)  //最you边检测到
	{
		error=-30*15;

	}
	if(col_val[1]==1)  //偏离中间
	{
		error=30*15;
		
	}
	if(col_val[4])  //&&col_val[3] 两边都检测到才转弯
	{
		error=-60*15;
	}		
	if(col_val[0])  // &&col_val[1]两个都检测到才转圈  直角可以    锐角待实验  锐角必须要5路
	{
		error=60*15;	
	}
	
	if((col_val[1]+col_val[2]+col_val[3]+col_val[4]+col_val[0])>=5)  //至少4个色块都要检测到  停止
	{	
		if(!way_flag)
			 En_control(0,0);				
	}
	
	pro1 = frequency/ std_fre;
	pro2 = frequency/ std_fre;
	
  SMD_Control(frequency - pro1*error,frequency + pro2*error);
	
	
}
