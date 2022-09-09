#ifndef __SMD_H
#define __SMD_H


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include  "My_math.h"

#include "delay.h"


// PWM clock set
#define SYSCTL_PWMDIV					SYSCTL_PWMDIV_64
#define DIV 							64

//ϸ��
#define subdiv       16.0f   
// �ٶ�ת��ΪƵ��          һ���������    ֱ��
#define spd_fre     151.0f  //ԭ�� 140��ʵ���п���ƫС������ ����
//subdiv*360.0f/          3.14f/3.6f/6.0f
void SMD_PWM_Init(float speed ) ;
void SMD_Control(float fre1,float fre2 )	/*�����������*/;	/*�����������*/
void DIR_PORT_init(void);
void SMD_PORT_init(void);

void Step1_Control(uint32_t H_time,uint32_t us_period,int steps);//���� ���1 �ٶȣ�Ȧ��
void Step2_Control(uint32_t H_time,uint32_t us_period,int steps);	//���� ���2 �ٶȣ�Ȧ��

void Dir_1(int n); //��ߵ������
void Dir_2(int n);//�ҵ������
void Dir_control(int t1,int t2);//���ҵ�� ����  ����

void En_control(int n1,int n2 );  //���������������


#define Dir1_H ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7,GPIO_PIN_7)  
#define Dir1_L ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_7,0)

#define Dir2_H ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,GPIO_PIN_5)  
#define Dir2_L ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0)

#define Step1_H ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6,GPIO_PIN_6)  
#define Step1_L ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6,0)

#define Step2_H ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,GPIO_PIN_5)  
#define Step2_L ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0)

#endif  
