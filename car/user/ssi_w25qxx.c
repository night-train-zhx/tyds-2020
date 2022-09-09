#include "ssi_w25qxx.h"
#include "hw_w25qxx.h"
#include "utils/uartstdio.h"


void W25Q_ssi_init(void)//初始化ssi0
{
    SysCtlPeripheralEnable(W25Q_PERIPH_SSI);
    SysCtlPeripheralEnable(W25Q_PERIPH_GPIO);
	
    GPIOPinConfigure(W25Q_GPIO_SSICLK);
    GPIOPinConfigure(W25Q_GPIO_SSIFSS);
    GPIOPinConfigure(W25Q_GPIO_SSIRX);
    GPIOPinConfigure(W25Q_GPIO_SSITX);
    GPIOPinTypeSSI(W25Q_PORT_BASE, W25Q_RX_PIN | W25Q_FSS_PIN | W25Q_CLK_PIN |
                   W25Q_TX_PIN);
    SSIConfigSetExpClk(W25Q_SPI_BASE, SysCtlClockGet(),SSI_FRF_MOTO_MODE_0,SSI_MODE_MASTER, 1000000,8);
    SSIEnable(W25Q_SPI_BASE);
}

void W25Q_gpio_ss_init(void)//使用gpio口作为ss脚
{
    SysCtlPeripheralEnable(W25Q_PERIPH_GPIO);
			while(!SysCtlPeripheralReady(W25Q_PERIPH_GPIO))
        ;
    GPIOPinTypeGPIOOutput(W25Q_PORT_BASE,W25Q_FSS_PIN);
    GPIOPinWrite(W25Q_PORT_BASE, W25Q_FSS_PIN, W25Q_FSS_PIN);
}

static void gpio_ss_high()//拉高ss脚电平
{
    GPIOPinWrite(W25Q_PORT_BASE, W25Q_FSS_PIN, W25Q_FSS_PIN);
}

static void gpio_ss_low()//传输时拉低ss脚电平
{
    GPIOPinWrite(W25Q_PORT_BASE, W25Q_FSS_PIN, 0x0);
}

static void ssi0_ReadWriteByte(uint32_t TxData, uint32_t *RxData)
{
  SSIDataPut(W25Q_SPI_BASE, TxData); //通过外设ssi0发送一个数据
		while (SSIBusy(W25Q_SPI_BASE));
  SSIDataGet(W25Q_SPI_BASE, RxData);
		while (SSIBusy(W25Q_SPI_BASE));
}

uint16_t w25qxx_readID(void)
{
  uint16_t Temp = 0;
  uint32_t RxData = 0;
  gpio_ss_low();//拉低ss脚电平
  ssi0_ReadWriteByte(W25QXX_CMD_MANUFACT_DEVICE_ID, &RxData); //发送读取ID命令
  ssi0_ReadWriteByte(0x00, &RxData);
  ssi0_ReadWriteByte(0x00, &RxData);
  ssi0_ReadWriteByte(0x00, &RxData);
  RxData &= 0x0;
  ssi0_ReadWriteByte(0xFF, &RxData);
  Temp |= RxData << 8;
  ssi0_ReadWriteByte(0xFF, &RxData);
  Temp |= RxData;
  gpio_ss_high();//拉高ss脚电平，传输结束
  return Temp;
}

uint8_t ssi_w25qxx_readSR(void) //读取flash状态
{
  uint32_t RxData;
  gpio_ss_low();
  ssi0_ReadWriteByte(W25QXX_CMD_READ_REG_SR1,&RxData);
  gpio_ss_high();
  return (uint8_t)RxData;
}

void w25qxx_wait() //等待flash
{
  uint8_t Status;
  do
  {
    Status = ssi_w25qxx_readSR();
    Status &= 0x01;
  }
  while (Status != 0);
}

 void ssi_w25qxx_write_enable(void) //写使能
{
  uint32_t RxData;
  gpio_ss_low();
  ssi0_ReadWriteByte(W25QXX_CMD_ENABLE_WRITE,&RxData);
  gpio_ss_high();
}

 void ssi_w25qxx_write_disable(void) //写禁能
{
    uint32_t RxData;
    gpio_ss_low();
    ssi0_ReadWriteByte(W25QXX_CMD_DISABLE_WRITE,&RxData);
    gpio_ss_high();
}

void ssi_w25qxx_erase_sector(uint32_t Dst_Addr) //擦除扇区
{
  uint32_t RxData;
  Dst_Addr *= 4096;
  ssi_w25qxx_write_enable();
  w25qxx_wait();
  gpio_ss_low();
  ssi0_ReadWriteByte(W25QXX_CMD_READ_REG_SR1,&RxData);
  ssi0_ReadWriteByte((uint8_t)((Dst_Addr) >> 16),&RxData);
  ssi0_ReadWriteByte((uint8_t)((Dst_Addr) >> 8),&RxData);
  ssi0_ReadWriteByte((uint8_t)((Dst_Addr) >> 0),&RxData);
  gpio_ss_high();
  w25qxx_wait();
}

void ssi_w25qxx_read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) //读取
{
  uint16_t i;
  uint32_t data;
  gpio_ss_low();
  ssi0_ReadWriteByte(W25QXX_CMD_READ_DATA,&data);
  ssi0_ReadWriteByte((uint8_t)((ReadAddr) >> 16),&data);
  ssi0_ReadWriteByte((uint8_t)((ReadAddr) >> 8),&data);
  ssi0_ReadWriteByte((uint8_t)((ReadAddr) >> 0),&data);
  while (SSIDataGetNonBlocking(W25Q_SPI_BASE, &data)); //清除缓冲区
  for (i = 0; i < NumByteToRead; i++)
  {
    ssi0_ReadWriteByte(0xff,&data);
    pBuffer[i] = (uint8_t)data;
  }
  gpio_ss_high();
}

void ssi_w25qxx_write_page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) //写一页
{
  uint16_t i;
  uint32_t data;
  ssi_w25qxx_write_enable();
  gpio_ss_low();
  ssi0_ReadWriteByte(W25QXX_CMD_PAGE_PROGRAM,&data);
  ssi0_ReadWriteByte((uint8_t)((WriteAddr) >> 16),&data);
  ssi0_ReadWriteByte((uint8_t)((WriteAddr) >> 8),&data);
  ssi0_ReadWriteByte((uint8_t)((WriteAddr) >> 0),&data);
  for (i = 0; i < NumByteToWrite; i++)
  {
      ssi0_ReadWriteByte(pBuffer[i],&data);
  }
  gpio_ss_high();
  w25qxx_wait();
}

void ssi_w25qxx_write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) //写，但不校验是否已擦除
{
  uint16_t pageremain;
  pageremain = 256 - WriteAddr % 256;

  if (NumByteToWrite <= pageremain)
    pageremain = NumByteToWrite;

  while (1)
  {
    ssi_w25qxx_write_page(pBuffer, WriteAddr, pageremain); //要写入的字节数小或等于单页剩余的字节数直接写
    if (NumByteToWrite == pageremain) break; //写入结束了
    else //NumByteToWrite>pageremain，如果要写入的数据大于单页剩余的字节数。
    {
      pBuffer += pageremain;
      WriteAddr += pageremain;
      NumByteToWrite -= pageremain;//减去已经写入了的字节数

    if (NumByteToWrite > 256)
      pageremain = 256; //一次可以写入256个字节
      else
          pageremain = NumByteToWrite;
       //不够256个字节了
    }
  }
}

uint8_t W25QXX_BUFFER[4096];

void ssi_w25qxx_write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) //写
{
  uint32_t secpos;
  uint16_t secoff;
  uint16_t secremain;
  uint16_t i;
  uint8_t *W25QXX_BUF;
  W25QXX_BUF = W25QXX_BUFFER;
  secpos = WriteAddr / 4096; //算出来扇区地址
  secoff = WriteAddr % 4096; //取余数，算出在扇区内的偏移
  secremain = 4096 - secoff; //扇区剩余空间大小
  if (NumByteToWrite <= secremain) //没有跨扇区
  secremain = NumByteToWrite;
  while (1)
  {
    ssi_w25qxx_read(W25QXX_BUF, secpos * 4096, 4096); //读出整个扇区的内容，保存在Buffer中。
    for (i = 0; i < secremain; i++)//校验数据
    {
      if (W25QXX_BUF[secoff + i] != 0XFF)break; //如果有不等于0xFF的数据，就需要擦除
    }
    if (i < secremain) //需要擦除
    {
      ssi_w25qxx_erase_sector(secpos); //擦除这个扇区
      for (i = 0; i < secremain; i++)  //更新缓存中的数据
      {
        W25QXX_BUF[secoff + i] = pBuffer[i];
        //这里的pBuffer中是我们要写的数据，把这些数据更新到缓存中对应的位置
      }
      ssi_w25qxx_write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //重新写入整个扇区
    }
    else
      ssi_w25qxx_write_NoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
    if (NumByteToWrite == secremain) break; //写入结束了
    else //写入未结束
    {
      secpos++; //扇区地址增1
      secoff = 0; //偏移位置为0
      pBuffer += secremain;
      //指针偏移
      WriteAddr += secremain; //写地址偏移
      NumByteToWrite -= secremain; //字节数递减
      if (NumByteToWrite > 4096)
          secremain = 4096; //下一个扇区还是写不完
      else
          secremain = NumByteToWrite; //下一个扇区可以写完了
    }
  }
}
