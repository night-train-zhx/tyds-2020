#ifndef __LED_H__
#define __LED_H__

#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#define red_on ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1) //红色
#define red_off ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0)   

#define blue_on ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2)  //蓝色
#define blue_off ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0)  

#define green_on ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_PIN_3)  //绿色
#define green_off ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0)  

#define white_on      red_on;blue_on;green_on
#define led_all_off   red_off;blue_off;green_off


void LED_Init(void);   /*led初始化*/
void Led_Flash(uint8_t i); //led灯闪烁
#endif
