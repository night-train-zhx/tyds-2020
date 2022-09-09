#ifndef MYUART_H_
#define MYUART_H_


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

void UART_send_int64_t(uint32_t ui32Base,const int64_t temp,char on);
void UART_send_int32_t(uint32_t ui32Base,const int32_t temp,char on);
void UART_send_int16_t(uint32_t ui32Base,const int16_t temp,char on);
void UART_send_float(uint32_t ui32Base,const float temp,int64_t lenth);
void UART_send_int64_t_string(uint32_t ui32Base,const int64_t *temp, uint64_t num);
void UART_send_int32_t_string( uint32_t ui32Base, const int32_t *temp , uint64_t num) ;
void UART_send_int16_t_string(uint32_t ui32Base,const int16_t *temp, uint64_t num);
void UART_send_char_string(uint32_t ui32Base,const unsigned char *temp, uint64_t num);
void UART_send_float_string(uint32_t ui32Base,const float *temp, int64_t lenth, uint64_t num);
void UART_recive_string(uint32_t ui32Base,char *reciveBuf,int num);
void UART_send_string (uint32_t ui32Base,const char *sendbuf );





#endif /* MYUART_H_ */
