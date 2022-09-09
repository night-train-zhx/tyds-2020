#ifndef __QEI_H__
#define __QEI_H__

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/qei.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"




void QEI_Init(void);
int32_t Read_Encoder(uint8_t TIMX);
int32_t  Read_Rpm (int32_t Encoder);
#endif
