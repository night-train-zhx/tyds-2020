
#include "myadc.h"



/*
*���ƣ� ADC_Init
*���ܣ�AD0 CH0  PE3  
*ע�⣺  ʹ��CH1 ��Ҫ����  ui32AdcValue 
*/

bool  bDataReady = true;
uint32_t Ch0Value;          // Save the voltage value of ch0 and ch1. ע����FIFOƥ��

void ADC0IntHander(void)
{
	 //�������
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

   /*******ע��ÿһ�����з�������Ӧ�������Ȳ�һ�� 8����4����4����1*******/       
void ADC_Init(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
       while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) ;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
      while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // Configure the GPIOE2,3 to peripheral function.
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
		 ///GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2 |GPIO_PIN_3);

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
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 |ADC_CTL_END | ADC_CTL_IE);
			//		ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
			//    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1 |ADC_CTL_END | ADC_CTL_IE);


     // param ui32Factor is the number of samples to be averaged. ƽ��ֵ ֻ����2�ı��������64��0��ʱ���ֹ������
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






