#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_

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
#include "driverlib/ssi.h"
#include "driverlib/interrupt.h"



/*传感器用*/
#define SYSCTL_PERIPH_SSI SYSCTL_PERIPH_SSI0
#define SPI_SYSCTL_PERIPH			SYSCTL_PERIPH_GPIOA
#define SPI_PROT			  GPIO_PORTA_BASE
#define SPI_CLK_PIN		GPIO_PIN_2
#define SPI_RX_PIN			GPIO_PIN_4  //
#define SPI_TX_PIN			GPIO_PIN_5
#define SPI_CLK			GPIO_PA2_SSI0CLK
#define SPI_RX				GPIO_PA4_SSI0RX
#define SPI_TX				GPIO_PA5_SSI0TX
#define SPI_BASE			SSI0_BASE


/**********AK8975的CS使能引脚配置*************/
#define AK_CSPIN_SYSCTL		SYSCTL_PERIPH_GPIOD
#define AK8975_CS_PORT		GPIO_PORTD_BASE
#define AK8975_CS_PIN		GPIO_PIN_0



void Drv_SpiInit(void);
uint8_t Drv_SpiSingleWirteAndRead(uint8_t SendData);
void Drv_SpiTransmit(uint8_t *ucp_Data, uint16_t us_Size);
void Drv_SpiReceive(uint8_t *ucp_Data, uint16_t us_Size);
#endif
