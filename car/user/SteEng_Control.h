#ifndef STEENG_CONTROL_H
#define STEENG_CONTROL_H


#define LIMIT( x , min , max ) ( ((x) <= (min)) ? (min ) : ( ((x) > (max))? (max) : (x) ) )  //оч╥Ы

///******PD0***********/
//#define SE_PWM_GEN							  PWM_GEN_0
//#define SE_PWM_OUT_A						  PWM_OUT_0
//#define SE_PWM_OUT_BIT_A					PWM_OUT_0_BIT
//#define SE_SYSCTL_PERIPH_PWM			SYSCTL_PERIPH_PWM1
//#define SE_PWM_BASE						    PWM1_BASE
//#define SE_GPIO_PIN_PWMOUTPUT_A		GPIO_PD0_M1PWM0
//#define SE_PWM_GEN_MODE_COUNTER   PWM_GEN_MODE_DOWN
//#define SE_PWM_GEN_MODE_UPDATE		PWM_GEN_MODE_NO_SYNC
//#define SE_DIV 							      8
//#define SE_SYSCTL_PWMDIV					SYSCTL_PWMDIV_1
//#define SE_SYSCTL_PERIPH_GPIO			SYSCTL_PERIPH_GPIOD
//#define SE_GPIO_PORT_BASE					GPIO_PORTD_BASE
//#define SE_GPIO_PIN						    GPIO_PIN_0 


/******PC4***********/
#define SE_PWM_GEN							  PWM_GEN_3
#define SE_PWM_OUT_A						  PWM_OUT_6
#define SE_PWM_OUT_BIT_A					PWM_OUT_6_BIT
#define SE_SYSCTL_PERIPH_PWM			SYSCTL_PERIPH_PWM0
#define SE_PWM_BASE						    PWM0_BASE
#define SE_GPIO_PIN_PWMOUTPUT_A		GPIO_PC4_M0PWM6
#define SE_PWM_GEN_MODE_COUNTER   PWM_GEN_MODE_DOWN
#define SE_PWM_GEN_MODE_UPDATE		PWM_GEN_MODE_NO_SYNC
#define SE_DIV 							      8
#define SE_SYSCTL_PWMDIV					SYSCTL_PWMDIV_1
#define SE_SYSCTL_PERIPH_GPIO			SYSCTL_PERIPH_GPIOC
#define SE_GPIO_PORT_BASE					GPIO_PORTC_BASE
#define SE_GPIO_PIN						    GPIO_PIN_4 


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "delay.h"


extern void PWM_SteEng(void);
extern void Set_Angle(double Angle);

#endif /* STEENG_CONTROL_H*/

