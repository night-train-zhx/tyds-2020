
#include "ANO_transfer_data.h"

/*���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���*/
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
	
uint8_t datatosend[50];  /*�������ݻ���*/
uint8_t data_to_send[50];	/*�������ݻ���*/
uint8_t ANO_Send_PID_Flag[6]={0};//PID���ͱ�־λ


void ANO_UART_Int_Handler(void)    /*����0�жϺ���*/
{
   //�жϱ�־�Ķ�ȡ����������
    unsigned long ulStatus;
    char cThisChar;
    ROM_IntMasterDisable();//��ֹȫ���ж�
    ulStatus = UARTIntStatus(UART0_BASE, true);//��ȡ��ǰ���ж�״̬	
    ROM_UARTIntClear(UART0_BASE, ulStatus);//��� UART �ж�Դ

    //�����жϱ�־��������¼�
    if(ulStatus & UART_INT_RX)
    {
			cThisChar = ROM_UARTCharGet(UART0_BASE);//����һ���ַ�
      ROM_UARTCharPut(UART0_BASE,cThisChar);  //����
    }
		
    ROM_IntMasterEnable();//ʹ��ȫ���ж�
}

   //�����жϣ�����ʹ��
void UART0_Init(unsigned long ulBaud)/*����0��ʼ��*/
{
	 //��ʼ�������� UART0 ģ��,PA0/PA1
	 ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	 ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	 ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	 ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), ulBaud, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE); 

   UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
   UARTStdioConfig(0, 115200, 16000000);
	
//	 UARTIntRegister(UART0_BASE, ANO_UART_Int_Handler);//ע��һ�� UART �жϵ��жϴ������
//	 ROM_UARTIntEnable(UART0_BASE, UART_INT_RX);
//	 ROM_UARTIntEnable(UART0_BASE, UART_INT_TX);

}

/******�������**********/
/*****�κδ������ֶ�����ʹ��***/
void send_userdata(uint8_t *dataToSend,uint8_t length)/*��������վ����*/
{
  static uint8_t i;
  for(i = 0; i <length; i++)
	{

     ROM_UARTCharPut(UART0_BASE,*dataToSend++); //�����ַ���Ӧ��ASCII��

  }
}




void Send_v65(uint8_t function,uint32_t data1,uint32_t data2,uint32_t data3) /*�����û����ݵ���λ�� V65*/
{
   uint8_t _cnt=0;
   uint8_t sum = 0;  //����Ϊ����sumУ���ֽڣ���0xAAҲ�������ֽڣ�һֱ��sum�ֽ�ǰһ�ֽ�
   uint8_t i = 0;
	
   datatosend[_cnt++]=0xAA;//0xAAΪ֡ͷ
   datatosend[_cnt++]=0x05;//0x05Ϊ���ݷ���Դ��������ο�����Э�飬���ֽ��û������������
   datatosend[_cnt++]=0xAF;//0xAFΪ����Ŀ�ĵأ�AF��ʾ��λ����������ο�����Э��
	
	 datatosend[_cnt++]=function;//������

   datatosend[_cnt++]=0;//���ֽڱ�ʾ���ݳ��ȣ�������=0����������ٸ�ֵ�������Ͳ����˹����㳤����



   datatosend[_cnt++]=BYTE1(data1);//��Ҫ���͵����ݷ������ͻ�����,�ֽڲ�֣���λ��ǰ
   datatosend[_cnt++]=BYTE0(data1);

   datatosend[_cnt++]=BYTE1(data2);//��Ҫ���͵����ݷ������ͻ�����,�ֽڲ�֣���λ��ǰ
   datatosend[_cnt++]=BYTE0(data2);

   datatosend[_cnt++]=BYTE1(data3);//��Ҫ���͵����ݷ������ͻ�����,�ֽڲ�֣���λ��ǰ
   datatosend[_cnt++]=BYTE0(data3);


   datatosend[4] = _cnt-5;//__cnt�����������ݳ��ȣ���5Ϊ��ȥ֡��ͷ5���������ֽ�


   for(i=0;i<_cnt;i++)
         sum += datatosend[i];

   datatosend[_cnt++]=sum; //��sumУ�����ݷ������һ�ֽ�

   send_userdata(datatosend, _cnt);//���ʹ���
}



static void ANO_DT_Send_Check(uint8_t head, uint8_t check_sum)
{
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	send_userdata(data_to_send, 7);
}

void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, int32_t alt, uint8_t fly_model, uint8_t armed)
{
	uint8_t _cnt=0;
	int16_t _temp;
	int32_t _temp2 = alt;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(angle_rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++] = fly_model;
	
	data_to_send[_cnt++] = armed;
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	send_userdata(data_to_send, _cnt);
}
void ANO_DT_Send_RCData(uint16_t thr,uint16_t yaw,uint16_t rol,uint16_t pit,uint16_t aux1,uint16_t aux2,uint16_t aux3,uint16_t aux4,uint16_t aux5,uint16_t aux6)
{
	uint8_t _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(thr);
	data_to_send[_cnt++]=BYTE0(thr);
	data_to_send[_cnt++]=BYTE1(yaw);
	data_to_send[_cnt++]=BYTE0(yaw);
	data_to_send[_cnt++]=BYTE1(rol);
	data_to_send[_cnt++]=BYTE0(rol);
	data_to_send[_cnt++]=BYTE1(pit);
	data_to_send[_cnt++]=BYTE0(pit);
	data_to_send[_cnt++]=BYTE1(aux1);
	data_to_send[_cnt++]=BYTE0(aux1);
	data_to_send[_cnt++]=BYTE1(aux2);
	data_to_send[_cnt++]=BYTE0(aux2);
	data_to_send[_cnt++]=BYTE1(aux3);
	data_to_send[_cnt++]=BYTE0(aux3);
	data_to_send[_cnt++]=BYTE1(aux4);
	data_to_send[_cnt++]=BYTE0(aux4);
	data_to_send[_cnt++]=BYTE1(aux5);
	data_to_send[_cnt++]=BYTE0(aux5);
	data_to_send[_cnt++]=BYTE1(aux6);
	data_to_send[_cnt++]=BYTE0(aux6);

	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	send_userdata(data_to_send, _cnt);
}

void ANO_DT_Send_PID(uint8_t group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	uint8_t _cnt=0;
	int16_t _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	
	_temp = p1_p;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_i;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_d;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_p;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_i;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_d;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_p;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_i;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_d;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	send_userdata(data_to_send, _cnt);
}

//Data_Receive_Prepare������Э��Ԥ����������Э��ĸ�ʽ�����յ������ݽ���һ�θ�ʽ�Խ�������ʽ��ȷ�Ļ��ٽ������ݽ���
//��ֲʱ���˺���Ӧ���û���������ʹ�õ�ͨ�ŷ�ʽ���е��ã����紮��ÿ�յ�һ�ֽ����ݣ�����ô˺���һ��
//�˺������������ϸ�ʽ������֡�󣬻����е������ݽ�������

void ANO_DT_Data_Receive_Prepare(uint8_t data)
{
	static uint8_t RxBuffer[50];
	static uint8_t _data_len = 0,_data_cnt = 0;
	static uint8_t state = 0;
	
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
}

//Data_Receive_Anl������Э�����ݽ������������������Ƿ���Э���ʽ��һ������֡���ú��������ȶ�Э�����ݽ���У��
//У��ͨ��������ݽ��н�����ʵ����Ӧ����
//�˺������Բ����û����е��ã��ɺ���Data_Receive_Prepare�Զ�����

void ANO_DT_Data_Receive_Anl(uint8_t *data_buf,uint8_t num)
{
	uint8_t sum = 0;
	for(uint8_t i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//�ж�sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//�ж�֡ͷ
	
//	if(*(data_buf+2)==0X01)
//	{
//		if(*(data_buf+4)==0X01)
//			mpu6050.Acc_CALIBRATE = 1;
//		if(*(data_buf+4)==0X02)
//			mpu6050.Gyro_CALIBRATE = 1;
//		if(*(data_buf+4)==0X03)
//		{
//			mpu6050.Acc_CALIBRATE = 1;		
//			mpu6050.Gyro_CALIBRATE = 1;			
//		}
//	}
	
	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{	
      ANO_Send_PID_Flag[0]=1;
		}
		if(*(data_buf+4)==0X02)
		{
			
		}
		if(*(data_buf+4)==0XA0)		//��ȡ�汾��Ϣ
		{
//			f.send_version = 1;
		}
		if(*(data_buf+4)==0XA1)		//�ָ�Ĭ�ϲ���
		{
//			B_kp=300.0f;
//			B_kd=0.8f;
//			
//			V_kp=-95.0f;
//			V_ki=-0.8f;
//			ANO_Send_PID_Flag[0]=1;
//			
		}
	}
		if(*(data_buf+2)==0X10)								//����PID
			{
//        B_kp  = 0.1f*(float)( (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) );
////        _pidRateRoll.ki  = ( (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) );
//        B_kd  = 0.1f*(float)( (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) );
//				V_kp  = -0.1f*(float)( (int16_t)(*(data_buf+10)<<8)|*(data_buf+11) );
//				V_ki = -0.1f*(float)( (int16_t)(*(data_buf+12)<<8)|*(data_buf+13) );
////        _pidRatePitch.kd  = ( (int16_t)(*(data_buf+14)<<8)|*(data_buf+15) );
//        flow_Kp = 0.1f*(float)( (int16_t)(*(data_buf+16)<<8)|*(data_buf+17) );
////        _pidRateYaw.ki  = ( (int16_t)(*(data_buf+18)<<8)|*(data_buf+19) );
////			  _pidRateYaw.kd  = ( (int16_t)(*(data_buf+20)<<8)|*(data_buf+21) );
//				ANO_DT_Send_Check(*(data_buf+2),sum);
//			}
//			if(*(data_buf+2)==0X11)								//����PID
//			{
////        _pidAngleRoll.kp 	= ( (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) );
////        _pidAngleRoll.ki 	= ( (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) );
////        _pidAngleRoll.kd 	= ( (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) );
////        _pidAnglePitch.kp 	= ( (int16_t)(*(data_buf+10)<<8)|*(data_buf+11) );
////        _pidAnglePitch.ki 	= ( (int16_t)(*(data_buf+12)<<8)|*(data_buf+13) );
////        _pidAnglePitch.kd 	= ( (int16_t)(*(data_buf+14)<<8)|*(data_buf+15) );
////        _pidAngleYaw.kp	  = ( (int16_t)(*(data_buf+16)<<8)|*(data_buf+17) );
////        _pidAngleYaw.ki 	= ( (int16_t)(*(data_buf+18)<<8)|*(data_buf+19) );
////        _pidAngleYaw.kd 	= ( (int16_t)(*(data_buf+20)<<8)|*(data_buf+21) );
//				ANO_DT_Send_Check(*(data_buf+2),sum);

			}
			if(*(data_buf+2)==0X12)								//PID3
			{	
//        _pidVZ.kp  = ( (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) );
//        _pidVZ.ki  = ( (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) );
//        _pidVZ.kd  = ( (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) );
//        _pidZ.kp = ( (int16_t)(*(data_buf+10)<<8)|*(data_buf+11) );
//        _pidZ.ki = ( (int16_t)(*(data_buf+12)<<8)|*(data_buf+13) );
//        _pidZ.kd = ( (int16_t)(*(data_buf+14)<<8)|*(data_buf+15) );
//        _pidVX.kp 	= ( (int16_t)(*(data_buf+16)<<8)|*(data_buf+17) );
//        _pidVX.ki 	= ( (int16_t)(*(data_buf+18)<<8)|*(data_buf+19) );
//        _pidVX.kd 	= ( (int16_t)(*(data_buf+20)<<8)|*(data_buf+21) );
//				_pidVY = _pidVX;
				ANO_DT_Send_Check(*(data_buf+2),sum);

			}
			
		if(*(data_buf+2)==0X13)								//PID4
		{
//        _pidX.kp  =( (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) );
//        _pidX.ki  = ( (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) );
//        _pidX.kd  =( (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) );	
//				_pidY = _pidX;
				
			ANO_DT_Send_Check(*(data_buf+2),sum);

		}
		if(*(data_buf+2)==0X14)								//PID5
		{
			ANO_DT_Send_Check(*(data_buf+2),sum);

		}
		if(*(data_buf+2)==0X15)								//PID6
		{
			ANO_DT_Send_Check(*(data_buf+2),sum);
		}

}
void ANO_DT_Send_MotoPWM(uint16_t m_1,uint16_t m_2,uint16_t m_3,uint16_t m_4,uint16_t m_5,uint16_t m_6,uint16_t m_7,uint16_t m_8)
{
	uint8_t _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE1(m_1);
	data_to_send[_cnt++]=BYTE0(m_1);
	data_to_send[_cnt++]=BYTE1(m_2);
	data_to_send[_cnt++]=BYTE0(m_2);
	data_to_send[_cnt++]=BYTE1(m_3);
	data_to_send[_cnt++]=BYTE0(m_3);
	data_to_send[_cnt++]=BYTE1(m_4);
	data_to_send[_cnt++]=BYTE0(m_4);
	data_to_send[_cnt++]=BYTE1(m_5);
	data_to_send[_cnt++]=BYTE0(m_5);
	data_to_send[_cnt++]=BYTE1(m_6);
	data_to_send[_cnt++]=BYTE0(m_6);
	data_to_send[_cnt++]=BYTE1(m_7);
	data_to_send[_cnt++]=BYTE0(m_7);
	data_to_send[_cnt++]=BYTE1(m_8);
	data_to_send[_cnt++]=BYTE0(m_8);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	send_userdata(data_to_send, _cnt);
}

void ANO_DT_Send_Senser(int16_t a_x,int16_t a_y,int16_t a_z,int16_t g_x,int16_t g_y,int16_t g_z,int16_t m_x,int16_t m_y,int16_t m_z,int16_t bar)
{
	uint8_t _cnt=0;
	int16_t _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	_temp = a_x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	send_userdata(data_to_send, _cnt);
}
