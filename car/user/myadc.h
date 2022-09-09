#ifndef MYADC_H
#define MYADC_H



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
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"



void ADC0IntHander(void);
void ADC_Init(void);
void ADC_Trig(void);


extern uint32_t Ch0Value; 
extern bool  bDataReady;

#endif

