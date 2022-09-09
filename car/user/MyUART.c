#include "MyUART.h"


/*****�ַ����м䲻�ܴ��� �ո�********/
void UART_send_string (uint32_t ui32Base,const char *sendbuf )
{
    while(*sendbuf != '\0')
    {
        UARTCharPut(ui32Base,*sendbuf);
        sendbuf ++;
    }
}



/*
*************************************************************
*   ��  ��  ��: UART_send_int64_t   
*   ����˵��: ���� 64λ��������Ϊ�ַ�����ʾ
*   ��ڲ���: ��ַ�����֣��Ƿ���ʾ 0
*   ��  ��  ֵ: none
*************************************************************
*/

void UART_send_int64_t(uint32_t ui32Base,const int64_t temp,char on)
{
    int64_t temp_p = temp;

    if(temp_p < 0)
    {
        UARTCharPut(ui32Base,'-');
        temp_p = 0-temp_p;
    }
    if(temp_p > 0)
    {
        UART_send_int64_t(ui32Base,temp_p/10,0);
        UARTCharPut(ui32Base,temp_p%10 + '0');
    }
    else if(temp_p == 0)
    {
        if(on)
            UARTCharPut(ui32Base,'0');
         UARTCharPut(ui32Base,'\n');
    }
}

/*
*************************************************************
*   ��  ��  ��: UART_send_int32_t   
*   ����˵��: ���� 32λ��������Ϊ�ַ�����ʾ
*   ��ڲ���: ��ַ�����֣��Ƿ��������
*   ��  ��  ֵ: none
*************************************************************
*/
void UART_send_int32_t(uint32_t ui32Base,const int32_t temp,char on)
{
    UART_send_int64_t( ui32Base,(int64_t) temp,on);
}

/*
*************************************************************
*   ��  ��  ��: UART_send_int16_t   
*   ����˵��: ���� 16 λ��������Ϊ�ַ�����ʾ
*   ��ڲ���: ��ַ�����֣��Ƿ��������
*   �� �� ֵ: none
*    ע  �� ����Χ -32768�� 32768
*************************************************************
*/
void UART_send_int16_t(uint32_t ui32Base,const int16_t temp,char on)
{
   UART_send_int64_t( ui32Base,(int64_t) temp,on);
}

/*
*************************************************************
*   ��  ��  ��: UART_send_float   
*   ����˵��: ���� С������Ϊ�ַ�����ʾ
*   ��ڲ���: ��ַ��С����С��λ��
*   �� �� ֵ: none
*************************************************************
*/
void UART_send_float(uint32_t ui32Base,const float temp,int64_t lenth)
{
    int64_t i;
    float num = temp;
    int64_t temp_p;

    if(num<0)
    {
        UARTCharPut(ui32Base,'-');
        num = 0-num;
    }
		
		//������������	
    temp_p = (int64_t)num;
    UART_send_int64_t( ui32Base , temp_p , 1 );
     UARTCharPut(ui32Base,'.');
    //����nλС��
    for(i=0; i<lenth; i++)
    {
        UARTCharPut(ui32Base,((int)(10*(num - temp_p)))%10 + '0');
        num = num*10;
        temp_p = (int64_t)num;
    }

}

/*
*************************************************************
*   ��  ��  ��: UART_send_int64_t_string   
*   ����˵��: ���� 64λ �������� ����Ϊ�ַ�����ʾ
*   ��ڲ���: ��ַ�����飬���ָ���
*   ��  ��  ֵ: none
*************************************************************
*/
void UART_send_int64_t_string(uint32_t ui32Base,const int64_t *temp, uint64_t num)
{
    uint64_t i;
    for(i=0; i<num; i++)
        UART_send_int64_t( ui32Base , temp[i], 1);
}


/*
*************************************************************
*   ��  ��  ��: UART_send_int32_t_string   
*   ����˵��: ���� 32λ �������� ����Ϊ�ַ�����ʾ
*   ��ڲ���: ��ַ�����飬���ָ���
*   ��  ��  ֵ: none
*************************************************************
*/
void UART_send_int32_t_string( uint32_t ui32Base, const int32_t *temp , uint64_t num)
{
    uint64_t i;
    for(i=0; i<num; i++)
        UART_send_int32_t( ui32Base , temp[i] , 1);
}

/*
*************************************************************
*   ��  ��  ��: UART_send_int16_t_string   
*   ����˵��: ���� 16λ �������� ����Ϊ�ַ�����ʾ
*   ��ڲ���: ��ַ�����飬���ָ���
*   �� �� ֵ: none
*    ע  �� ����Χ ��Χ -32768�� 32768
*************************************************************
*/
void UART_send_int16_t_string(uint32_t ui32Base,const int16_t *temp, uint64_t num)
{
    uint64_t i;
    for(i=0; i<num; i++)
        UART_send_int16_t(ui32Base,temp[i],1);
	
}
/*
*************************************************************
*   ��  ��  ��: UART_send_char_string   
*   ����˵��: ���� �ַ���
*   ��ڲ���: ��ַ���ַ���������
*   �� �� ֵ: none
*************************************************************
*/
void UART_send_char_string(uint32_t ui32Base,const unsigned char *temp, uint64_t num)
{
    uint64_t i;
    for(i=0; i<num; i++)
            UARTCharPut(ui32Base,temp[i]);
}
/*
*************************************************************
*   ��  ��  ��: UART_send_float_string   
*   ����˵��: ���� ��С��������
*   ��ڲ���: ��ַ���ַ�����С��λ��
*   �� �� ֵ: none
*************************************************************
*/
void UART_send_float_string(uint32_t ui32Base,const float *temp, int64_t lenth, uint64_t num)
{
    uint64_t i;
    for(i=0; i<num; i++)
        UART_send_float(ui32Base,temp[i],lenth);
}
/*
*************************************************************
*   ��  ��  ��: UART_recive_string   
*   ����˵��: ����
*   ��ڲ���: ��ַ���洢 ������
*   �� �� ֵ: none
*************************************************************
*/
void UART_recive_string(uint32_t ui32Base,char *reciveBuf,int num)
{
    int i;
    for(i=0; i<num;i++)
        reciveBuf[i] =  UARTCharGet( ui32Base);
}
/*
*************************************************************
*   ��  ��  ��: char_to_num   
*   ����˵��: ���ַ���ת��Ϊ����
*   ��ڲ���: �ַ���
*   �� �� ֵ: ����
*************************************************************
*/
int char_to_num(char *reciveBuf)
{	
    int n = 0;  
    for (int i = 0; reciveBuf[i] >= '0' && reciveBuf[i] <= '9'; ++i)  
     {  
        n = 10 * n + (reciveBuf[i] - '0');  
     }  
    return n;  
}









