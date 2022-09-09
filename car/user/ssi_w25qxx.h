#ifndef SSI_W25QXX_H_
#define SSI_W25QXX_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"



#define W25Q_PERIPH_SSI   SYSCTL_PERIPH_SSI0
#define W25Q_PERIPH_GPIO			SYSCTL_PERIPH_GPIOA
#define W25Q_PORT_BASE		  GPIO_PORTA_BASE
#define W25Q_CLK_PIN		GPIO_PIN_2
#define W25Q_FSS_PIN    GPIO_PIN_3
#define W25Q_RX_PIN			GPIO_PIN_4  //
#define W25Q_TX_PIN			GPIO_PIN_5
#define W25Q_GPIO_SSICLK			GPIO_PA2_SSI0CLK
#define W25Q_GPIO_SSIFSS      GPIO_PA3_SSI0FSS
#define W25Q_GPIO_SSIRX				GPIO_PA4_SSI0RX
#define W25Q_GPIO_SSITX				GPIO_PA5_SSI0TX
#define W25Q_SPI_BASE			SSI0_BASE



 void W25Q_ssi_init(void);
 void W25Q_gpio_ss_init(void);


 uint16_t w25qxx_readID(void);
 uint8_t ssi_w25qxx_readSR(void); //读取flash状态
 void w25qxx_wait(void); //等待flash
 void ssi_w25qxx_write_enable(void); //写使能
 void ssi_w25qxx_write_disable(void); //写禁能
 void ssi_w25qxx_erase_sector(uint32_t Dst_Addr); //擦除扇区
 void ssi_w25qxx_read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead); //读取
 void ssi_w25qxx_write_page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); //写一页
 void ssi_w25qxx_write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);//写，但不校验是否已擦除
 void ssi_w25qxx_write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); //写




#endif /* SSI_W25QXX_H_ */
