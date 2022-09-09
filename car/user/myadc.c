
#include "myadc.h"



/*
*名称： ADC_Init
*功能：AD0 CH0  PE3  
*注意：  使用CH1 不要忘记  ui32AdcValue 
*/

bool  bDataReady = true;
uint32_t Ch0Value;          // Save the voltage value of ch0 and ch1. 注意与FIFO匹配

void ADC0IntHander(void)
{
	 //采样深度
    uint32_t ui32AdcValue[1] = {0};

    ADCIntClear(ADC0_BASE, 0);

    // Get the register value.
    ADCSequenceDataGet(ADC0_BASE, 0, ui32AdcValue);

    // Calculate the voltage, unit in mv.
    Ch0Value = ((float)ui32AdcValue[0])*(3300.0/4096.0);
//    Ch1Value = ((float)ui32AdcValue[1])*(3300.0/4096.0);  

    // Set the flag.
    bDataReady = true;
}

   /*******注意每一个序列发生器对应的最大深度不一样 8——4——4——1*******/       
void ADC_Init(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
       while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) ;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // Configure the GPIOE2,3 to peripheral function.
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
		 ///GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2 |GPIO_PIN_3);

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
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 |ADC_CTL_END | ADC_CTL_IE);
			//		ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
			//    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1 |ADC_CTL_END | ADC_CTL_IE);


     // param ui32Factor is the number of samples to be averaged. 平均值 只能是2的倍数，最大64，0的时候禁止过采样
    ADCHardwareOversampleConfigure(ADC0_BASE, 64);

    ADCSequenceEnable(ADC0_BASE, 0);
    ADCIntRegister(ADC0_BASE, 0, ADC0IntHander);
    ADCIntEnable(ADC0_BASE, 0);

}

void ADC_Trig(void)
{
    ADCProcessorTrigger(ADC0_BASE, 0);
	     while(true != bDataReady);
				 
    bDataReady = false;
}






