#ifndef LINE_PAT_H_
#define LINE_PAT_H_


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
#include "driverlib/adc.h"
#include "utils/uartstdio.h"

#include "sysconfig.h"

#define 	buzzer_on  GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,0) //�͵�ƽ����
#define 	buzzer_off  GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_7)  //�ߵ�ƽ����


void Line_ADC_init(void);
uint32_t *Get_Adc(uint32_t *ulADC0_Value);

void Line_GPIO_init(void);
void Get_line_val(void);
void power_control(void);

extern uint32_t col_val[7];
extern float frequency;
extern float way; //�ܾ���  


extern float std_fre;
extern float pro1;
extern float pro2;  //���������� ��ͬ�ٶ� ���Ե�����
extern float error;

#endif  
