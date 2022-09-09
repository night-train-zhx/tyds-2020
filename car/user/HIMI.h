#ifndef HIMI_H_
#define HIMI_H_

#include <stdint.h>
#include <stdbool.h> 
#include "driverlib/rom.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "driverlib/rom_map.h"
#include "delay.h"


void UART_HIMI_INIT(void);
void HIMI_check(void);
void HIMI_Send(uint8_t *str);
void send_num( uint8_t *numBuffer,int number);

extern uint8_t buffer_Page[];
extern	uint8_t buffer_Page2[];
extern uint8_t num_show[];
extern uint8_t tx[];



/********选取使用的 端口*****X=3用不了，玄学*****/
#define X 2
#if (X==1)
   /**********C4  C5**********/
#define  HIMI_UART_BASE     UART1_BASE 
#define  HIMI_SYSCTL_GPIO   SYSCTL_PERIPH_GPIOC        
#define  HIMI_SYS_PERIPH    SYSCTL_PERIPH_UART1
#define  HIMI_GPIO_RX       GPIO_PC4_U1RX
#define  HIMI_GPIO_TX       GPIO_PC5_U1TX
#define  HIMI_PORT_BASE     GPIO_PORTC_BASE
#define  HIMI_PINs          GPIO_PIN_4 | GPIO_PIN_5
#define  HIMI_INT_UART      INT_UART1

  /**********B0  B1**********/
#elif(X==2)
#define  HIMI_UART_BASE     UART1_BASE 
#define  HIMI_SYSCTL_GPIO   SYSCTL_PERIPH_GPIOB        
#define  HIMI_SYS_PERIPH    SYSCTL_PERIPH_UART1
#define  HIMI_GPIO_RX       GPIO_PB0_U1RX
#define  HIMI_GPIO_TX       GPIO_PB1_U1TX
#define  HIMI_PORT_BASE     GPIO_PORTB_BASE
#define  HIMI_PINs          GPIO_PIN_0 | GPIO_PIN_1
#define  HIMI_INT_UART      INT_UART1

  /*********D6   D7*********/
#elif (X==3)
#define  HIMI_UART_BASE     UART2_BASE
#define  HIMI_SYSCTL_GPIO   SYSCTL_PERIPH_GPIOD 
#define  HIMI_SYS_PERIPH    SYSCTL_PERIPH_UART2
#define  HIMI_GPIO_RX       GPIO_PD6_U2RX
#define  HIMI_GPIO_TX       GPIO_PD7_U2TX
#define  HIMI_PORT_BASE     GPIO_PORTD_BASE
#define  HIMI_PINs          GPIO_PIN_6 | GPIO_PIN_7
#define  HIMI_INT_UART      INT_UART2


#endif

#define Receive_END      0xED    //E(N)D  接收结束符 
#define data_limit       10      //接收的数据不能超过此（包括帧头和桢尾）








 #endif /* MYUART_H_ */


