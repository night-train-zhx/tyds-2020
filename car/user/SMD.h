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

//细分
#define subdiv       16.0f   
// 速度转化为频率          一个脉冲度数    直径
#define spd_fre     151.0f  //原来 140，实际中可能偏小，所以 增加
//subdiv*360.0f/          3.14f/3.6f/6.0f
void SMD_PWM_Init(float speed ) ;
void SMD_Control(float fre1,float fre2 )	/*功率输出控制*/;	/*功率输出控制*/
void DIR_PORT_init(void);
void SMD_PORT_init(void);

void Step1_Control(uint32_t H_time,uint32_t us_period,int steps);//控制 电机1 速度，圈数
void Step2_Control(uint32_t H_time,uint32_t us_period,int steps);	//控制 电机2 速度，圈数

void Dir_1(int n); //左边电机方向
void Dir_2(int n);//右电机方向
void Dir_control(int t1,int t2);//左右电机 方向  控制

void En_control(int n1,int n2 );  //电机抱死或者正常


#define Dir1_H ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7,GPIO_PIN_7)  
#define Dir1_L ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_7,0)

#define Dir2_H ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,GPIO_PIN_5)  
#define Dir2_L ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0)

#define Step1_H ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6,GPIO_PIN_6)  
#define Step1_L ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6,0)

#define Step2_H ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,GPIO_PIN_5)  
#define Step2_L ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0)

#endif  
