#ifndef UART_MODULE_H_
#define UART_MODULE_H_

#include <stdint.h>
#include <stdbool.h>              
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "driverlib/rom.h"

#include "delay.h"

     //*********************外部调用函数声明*********************
void ConfigureUART0(void);
void UART1_Init(unsigned long ulBaud);
void UART2_Init(unsigned long ulBaud);
void UARTX_INIT(unsigned long ulBaud);


#define X  5

#if X==3   //UART3
    /********C6  C7**************/
#define  UART_BASE     UART3_BASE 
#define  SYSCTL_GPIO   SYSCTL_PERIPH_GPIOC        
#define  SYS_PERIPH    SYSCTL_PERIPH_UART3
#define  GPIO_RX       GPIO_PC6_U3RX
#define  GPIO_TX       GPIO_PC7_U3TX
#define  PORT_BASE     GPIO_PORTC_BASE
#define  PINs          GPIO_PIN_6 | GPIO_PIN_7
#define  INT_UART      INT_UART3

#elif(X==4)  //UART4
    /********C4  C5**************/
#define  UART_BASE     UART4_BASE 
#define  SYSCTL_GPIO   SYSCTL_PERIPH_GPIOC        
#define  SYS_PERIPH    SYSCTL_PERIPH_UART4
#define  GPIO_RX       GPIO_PC4_U4RX
#define  GPIO_TX       GPIO_PC5_U5TX
#define  PORT_BASE     GPIO_PORTC_BASE
#define  PINs          GPIO_PIN_4 | GPIO_PIN_5
#define  INT_UART      INT_UART4


#elif(X==5)  //UART5
		/********E4  E5**************/
#define  UART_BASE     UART5_BASE 
#define  SYSCTL_GPIO   SYSCTL_PERIPH_GPIOE        
#define  SYS_PERIPH    SYSCTL_PERIPH_UART5
#define  GPIO_RX       GPIO_PE4_U5RX
#define  GPIO_TX       GPIO_PE5_U5TX
#define  PORT_BASE     GPIO_PORTE_BASE
#define  PINs          GPIO_PIN_4 | GPIO_PIN_5
#define  INT_UART      INT_UART5


#endif






#endif /* UART_MODULE_H_ */
