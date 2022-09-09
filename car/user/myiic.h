#ifndef __MYIIC_H
#define __MYIIC_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"
#include "driverlib/rom.h"



void IIC_Init(void);  /*初始化IIC的IO口*/ 
void DEV_Write_Len(uint8_t addr, uint8_t regAddr, uint8_t length, uint8_t *data) ;  /*连续写*/
void  DEV_Read_Len(uint8_t addr, uint8_t regAddr, uint8_t length, uint8_t *data) ;  /*连续读*/
void DEV_Write_Byte(uint8_t addr, uint8_t regAddr, uint8_t data) ;   /*写一位*/
uint8_t DEV_Read_Byte(uint8_t addr, uint8_t regAddr) ;   /*读一位*/
short int Double_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address);
#endif
