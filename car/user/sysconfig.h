#ifndef _SYSCONFIG_H_
#define _SYSCONFIG_H_



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
#include "driverlib/fpu.h"
#include "driverlib/rom_map.h"
#include "driverlib/ssi.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "driverlib/debug.h"
#include "utils/uartstdio.h"

#include "delay.h"

#include "led.h"
#include "ANO_transfer_data.h"

#include "Line_pat.h" //巡线
#include "SMD.h"  //步进电机
#include "blue_tooth.h"//蓝牙
#include "HIMI.h"   //串口屏
#include "oled.h"

#include "system_init.h"


#define  USER_INT0  0x00
#define  USER_INT1  0x20
#define  USER_INT2  0x40
#define  USER_INT3  0x60
#define  USER_INT4  0x80
#define  USER_INT5  0xA0
#define  USER_INT6  0xD0
#define  USER_INT7  0xE0



typedef	uint32_t 	u32;
typedef	uint16_t 	u16;
typedef	uint8_t 	u8;
typedef	int32_t 	s32;
typedef	int16_t 	s16;
typedef	int8_t 		s8;




#endif

