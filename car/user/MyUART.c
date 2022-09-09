#include "MyUART.h"


/*****字符串中间不能带有 空格********/
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
*   函  数  名: UART_send_int64_t   
*   功能说明: 发送 64位整数，成为字符串显示
*   入口参数: 地址，数字，是否显示 0
*   返  回  值: none
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
*   函  数  名: UART_send_int32_t   
*   功能说明: 发送 32位整数，成为字符串显示
*   入口参数: 地址，数字，是否结束换行
*   返  回  值: none
*************************************************************
*/
void UART_send_int32_t(uint32_t ui32Base,const int32_t temp,char on)
{
    UART_send_int64_t( ui32Base,(int64_t) temp,on);
}

/*
*************************************************************
*   函  数  名: UART_send_int16_t   
*   功能说明: 发送 16 位整数，成为字符串显示
*   入口参数: 地址，数字，是否结束换行
*   返 回 值: none
*    注  意 ：范围 -32768到 32768
*************************************************************
*/
void UART_send_int16_t(uint32_t ui32Base,const int16_t temp,char on)
{
   UART_send_int64_t( ui32Base,(int64_t) temp,on);
}

/*
*************************************************************
*   函  数  名: UART_send_float   
*   功能说明: 发送 小数，成为字符串显示
*   入口参数: 地址，小数，小数位数
*   返 回 值: none
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
		
		//发送整数部分	
    temp_p = (int64_t)num;
    UART_send_int64_t( ui32Base , temp_p , 1 );
     UARTCharPut(ui32Base,'.');
    //发送n位小数
    for(i=0; i<lenth; i++)
    {
        UARTCharPut(ui32Base,((int)(10*(num - temp_p)))%10 + '0');
        num = num*10;
        temp_p = (int64_t)num;
    }

}

/*
*************************************************************
*   函  数  名: UART_send_int64_t_string   
*   功能说明: 发送 64位 数字数组 ，成为字符串显示
*   入口参数: 地址，数组，数字个数
*   返  回  值: none
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
*   函  数  名: UART_send_int32_t_string   
*   功能说明: 发送 32位 数字数组 ，成为字符串显示
*   入口参数: 地址，数组，数字个数
*   返  回  值: none
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
*   函  数  名: UART_send_int16_t_string   
*   功能说明: 发送 16位 数字数组 ，成为字符串显示
*   入口参数: 地址，数组，数字个数
*   返 回 值: none
*    注  意 ：范围 范围 -32768到 32768
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
*   函  数  名: UART_send_char_string   
*   功能说明: 发送 字符串
*   入口参数: 地址，字符串，个数
*   返 回 值: none
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
*   函  数  名: UART_send_float_string   
*   功能说明: 发送 有小数的数组
*   入口参数: 地址，字符串，小数位数
*   返 回 值: none
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
*   函  数  名: UART_recive_string   
*   功能说明: 接收
*   入口参数: 地址，存储 ，个数
*   返 回 值: none
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
*   函  数  名: char_to_num   
*   功能说明: 将字符串转化为数字
*   入口参数: 字符串
*   返 回 值: 数字
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









