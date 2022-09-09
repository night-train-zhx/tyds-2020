

// Used for motor
#ifndef PWM_H_
#define PWM_H_

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


void PWM_Init(void);   /* pwm 初始化*/
void powerControl(uint32_t pwm[]);	/*功率输出控制*/
void Port_In_Init(void);  /*电机驱动芯片使能端初始化*/
void Set_Pwm(int moto1,int moto2);

 
#define Ain1_H ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4)  
#define Ain1_L ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0)
 
#define Bin1_H ROM_GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3)  
#define Bin1_L ROM_GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0) 


//#define Ain1_H ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0,GPIO_PIN_0)  
//#define Ain2_H ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,GPIO_PIN_2) 
//#define Ain1_L ROM_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0,0)  
//#define Ain2_L ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0)

//#define Bin1_H ROM_GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4)  
//#define Bin2_H ROM_GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3) 
//#define Bin1_L ROM_GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0)  
//#define Bin2_L ROM_GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0)


#endif /* PWM_H_ */


