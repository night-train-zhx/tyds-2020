#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include <math.h>
#include "inc/hw_gpio.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "driverlib/rom.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom_map.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"


typedef struct
{
  float Last_Time;
  float Now_Time;
  float Time_Delta;
  uint16_t Time_Delta_INT;//��λms
}Testime; //ʱ�����ṹ��


typedef struct
{
  uint32_t  SYSCTL_PERIPH_TIMER;
	uint32_t  TIMER_BASE;
  uint32_t  INT_TIMER;
	uint8_t   Priority;        //�ж����ȼ�
	void   (*Hander)(void);   //�жϺ���ָ��
}TIMER;



void SysTick_Init(void);    //1us
uint32_t micros(void);  //��ȡus
uint32_t millis(void);  //��ȡms
void Test_Period(Testime *Time_Lab);//ʱ������

void TIMERX_Handler(void);				//ϵͳ�����жϺ���
void TimeX_init( TIMER timerX );//ϵͳ���ȶ�ʱ����ʼ��
	


#endif






