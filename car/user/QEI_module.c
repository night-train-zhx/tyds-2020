
#include "QEI_module.h"

#include "PWM_Motor.h"
#include "pid.h"

#define QEI_MAX_POSITION				0x00000410-1
#define QEI_PERIOD_ms   10

/*
*************************************************************
*   ��  ��  ��: ConfigQEI
*   ����˵��: QEI��ʼ������
*   ��ڲ���: none
*   ��  ��  ֵ: none
*************************************************************
*/
void ConfigQEI(void)
{
	 ////////////////////QEI1   C5 C6//////////////////////////////////////
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);

		ROM_GPIOPinConfigure(GPIO_PC5_PHA1);                    
		ROM_GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5);
		ROM_GPIOPinConfigure(GPIO_PC6_PHB1);
		ROM_GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_6);

		ROM_GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_5,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
		ROM_GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_6,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);

		ROM_QEIConfigure(QEI1_BASE,(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), QEI_MAX_POSITION);

		ROM_QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, SysCtlClockGet()/QEI_PERIOD_ms);
		ROM_QEIVelocityEnable(QEI1_BASE);

		ROM_QEIEnable(QEI1_BASE);
	
	////////////////////QEI0     D6  D7//////////////////////////////////////
	
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
		
			/*PD7��������*/
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
		HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = GPIO_PIN_7;
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x00;
		

		ROM_GPIOPinConfigure(GPIO_PD6_PHA0);                    
		ROM_GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6);
		ROM_GPIOPinConfigure(GPIO_PD7_PHB0);
		ROM_GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_7);

		ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
		ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);

		ROM_QEIConfigure(QEI0_BASE,(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 0);

		ROM_QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, SysCtlClockGet()/QEI_PERIOD_ms);
		ROM_QEIVelocityEnable(QEI0_BASE);

		ROM_QEIEnable(QEI0_BASE);
		
		    //INT_QEI0
//		QEIIntEnable(QEI0_BASE, QEI_INTDIR | QEI_INTTIMER);
//    IntEnable(QEI0_BASE);
//    IntRegister(INT_QEI0,QEI0_IRQHandler);

}

/*
*************************************************************
*   �� �� ��:  Read_Encoder
*   ����˵��:  ��ȡ������ֵ,
*   ��ڲ���:  TIMX���ж� ��������ӣ������ұ�����
*    ����ֵ :  ���� ����� rpm
*************************************************************
*/



int32_t Read_Encoder(uint8_t TIMX)
{
	int32_t Encoder=0;
	
	if (TIMX ==1)
	{
				//���з���� 1s �ڲɼ����� ������
		Encoder = ROM_QEIDirectionGet(QEI1_BASE)*QEI_PERIOD_ms*ROM_QEIVelocityGet(QEI1_BASE);
	}
	else if (TIMX == 2)
	{
		Encoder = ROM_QEIDirectionGet(QEI0_BASE)*QEI_PERIOD_ms*ROM_QEIVelocityGet(QEI0_BASE);
	}
	
   return Encoder;
		
}
/***********************************************************************
���ٶȵ�λת��Ϊ:rpm r--3300���� 1m -- 60s  0.018= 60/3300 ||| 0.0577=60/1040
* **********************************************************************/

int32_t  Read_Rpm (int32_t Encoder)
{
	if(Encoder > 0)
	{
		Encoder=(int)(Encoder*0.0577+0.5);		//��������ȡ��
	}
	else
	{
		Encoder=(int)(Encoder*0.0577-0.5);		//��������ȡ��
	}
	
	return Encoder;

}

   //*******from  KK*************//
float  velocity;
int direction=1;
float init_pos=0;//��ʼλ��
bool Init_flag=1;
void QEI0_IRQHandler(void)
{
    uint32_t status = QEIIntStatus(QEI0_BASE, true);
    QEIIntClear(QEI0_BASE, status);

    if(status & QEI_INTTIMER)
    {
    velocity = QEIVelocityGet(QEI0_BASE) * QEI_PERIOD_ms * QEIDirectionGet(QEI0_BASE);
    //qei_data_array[QEInum].Now_Pos = QEIPositionGet(qei_data_array[QEInum].base);
    }

    if(status & QEI_INTDIR)
    {
        direction= -direction;
    }
    //��ʼλ��
    if(Init_flag)
    {
       init_pos= QEIPositionGet(QEI0_BASE);
        Init_flag = false;
    }
    //��ȡλ�ú��ٶ���Ϣ
//    qei_data_array[QEInum].Now_Pos = QEIPositionGet(qei_data_array[QEInum].base);
//    qei_data_array[QEInum].velocity = QEIVelocityGet(qei_data_array[QEInum].base) * MOTOR_QEI_VEL_FREQ * QEIDirectionGet(qei_data_array[QEInum].base);

}





















