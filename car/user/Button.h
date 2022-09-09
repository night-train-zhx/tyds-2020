

#ifndef BUTTON_H_
#define BUTTON_H_


#include <stdint.h>
#include <stdbool.h>
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/qei.h"
#include "delay.h"

#define KEY1 GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)
#define KEY2 GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)

//*********************外部调用函数声明*********************
void Button_IntHandler(void);
void ButtionInit(void);
void Key_Init(void);//按键初始化
unsigned char Key_Scan(void); //按键扫描

#endif /* UART_MODULE_H_ */
