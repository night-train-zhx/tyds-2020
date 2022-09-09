#include "ssi_w25qxx.h"
#include "hw_w25qxx.h"
#include "utils/uartstdio.h"


void W25Q_ssi_init(void)//��ʼ��ssi0
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

void W25Q_gpio_ss_init(void)//ʹ��gpio����Ϊss��
{
    SysCtlPeripheralEnable(W25Q_PERIPH_GPIO);
			while(!SysCtlPeripheralReady(W25Q_PERIPH_GPIO))
        ;
    GPIOPinTypeGPIOOutput(W25Q_PORT_BASE,W25Q_FSS_PIN);
    GPIOPinWrite(W25Q_PORT_BASE, W25Q_FSS_PIN, W25Q_FSS_PIN);
}

static void gpio_ss_high()//����ss�ŵ�ƽ
{
    GPIOPinWrite(W25Q_PORT_BASE, W25Q_FSS_PIN, W25Q_FSS_PIN);
}

static void gpio_ss_low()//����ʱ����ss�ŵ�ƽ
{
    GPIOPinWrite(W25Q_PORT_BASE, W25Q_FSS_PIN, 0x0);
}

static void ssi0_ReadWriteByte(uint32_t TxData, uint32_t *RxData)
{
  SSIDataPut(W25Q_SPI_BASE, TxData); //ͨ������ssi0����һ������
		while (SSIBusy(W25Q_SPI_BASE));
  SSIDataGet(W25Q_SPI_BASE, RxData);
		while (SSIBusy(W25Q_SPI_BASE));
}

uint16_t w25qxx_readID(void)
{
  uint16_t Temp = 0;
  uint32_t RxData = 0;
  gpio_ss_low();//����ss�ŵ�ƽ
  ssi0_ReadWriteByte(W25QXX_CMD_MANUFACT_DEVICE_ID, &RxData); //���Ͷ�ȡID����
  ssi0_ReadWriteByte(0x00, &RxData);
  ssi0_ReadWriteByte(0x00, &RxData);
  ssi0_ReadWriteByte(0x00, &RxData);
  RxData &= 0x0;
  ssi0_ReadWriteByte(0xFF, &RxData);
  Temp |= RxData << 8;
  ssi0_ReadWriteByte(0xFF, &RxData);
  Temp |= RxData;
  gpio_ss_high();//����ss�ŵ�ƽ���������
  return Temp;
}

uint8_t ssi_w25qxx_readSR(void) //��ȡflash״̬
{
  uint32_t RxData;
  gpio_ss_low();
  ssi0_ReadWriteByte(W25QXX_CMD_READ_REG_SR1,&RxData);
  gpio_ss_high();
  return (uint8_t)RxData;
}

void w25qxx_wait() //�ȴ�flash
{
  uint8_t Status;
  do
  {
    Status = ssi_w25qxx_readSR();
    Status &= 0x01;
  }
  while (Status != 0);
}

 void ssi_w25qxx_write_enable(void) //дʹ��
{
  uint32_t RxData;
  gpio_ss_low();
  ssi0_ReadWriteByte(W25QXX_CMD_ENABLE_WRITE,&RxData);
  gpio_ss_high();
}

 void ssi_w25qxx_write_disable(void) //д����
{
    uint32_t RxData;
    gpio_ss_low();
    ssi0_ReadWriteByte(W25QXX_CMD_DISABLE_WRITE,&RxData);
    gpio_ss_high();
}

void ssi_w25qxx_erase_sector(uint32_t Dst_Addr) //��������
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

void ssi_w25qxx_read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) //��ȡ
{
  uint16_t i;
  uint32_t data;
  gpio_ss_low();
  ssi0_ReadWriteByte(W25QXX_CMD_READ_DATA,&data);
  ssi0_ReadWriteByte((uint8_t)((ReadAddr) >> 16),&data);
  ssi0_ReadWriteByte((uint8_t)((ReadAddr) >> 8),&data);
  ssi0_ReadWriteByte((uint8_t)((ReadAddr) >> 0),&data);
  while (SSIDataGetNonBlocking(W25Q_SPI_BASE, &data)); //���������
  for (i = 0; i < NumByteToRead; i++)
  {
    ssi0_ReadWriteByte(0xff,&data);
    pBuffer[i] = (uint8_t)data;
  }
  gpio_ss_high();
}

void ssi_w25qxx_write_page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) //дһҳ
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

void ssi_w25qxx_write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) //д������У���Ƿ��Ѳ���
{
  uint16_t pageremain;
  pageremain = 256 - WriteAddr % 256;

  if (NumByteToWrite <= pageremain)
    pageremain = NumByteToWrite;

  while (1)
  {
    ssi_w25qxx_write_page(pBuffer, WriteAddr, pageremain); //Ҫд����ֽ���С����ڵ�ҳʣ����ֽ���ֱ��д
    if (NumByteToWrite == pageremain) break; //д�������
    else //NumByteToWrite>pageremain�����Ҫд������ݴ��ڵ�ҳʣ����ֽ�����
    {
      pBuffer += pageremain;
      WriteAddr += pageremain;
      NumByteToWrite -= pageremain;//��ȥ�Ѿ�д���˵��ֽ���

    if (NumByteToWrite > 256)
      pageremain = 256; //һ�ο���д��256���ֽ�
      else
          pageremain = NumByteToWrite;
       //����256���ֽ���
    }
  }
}

uint8_t W25QXX_BUFFER[4096];

void ssi_w25qxx_write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) //д
{
  uint32_t secpos;
  uint16_t secoff;
  uint16_t secremain;
  uint16_t i;
  uint8_t *W25QXX_BUF;
  W25QXX_BUF = W25QXX_BUFFER;
  secpos = WriteAddr / 4096; //�����������ַ
  secoff = WriteAddr % 4096; //ȡ����������������ڵ�ƫ��
  secremain = 4096 - secoff; //����ʣ��ռ��С
  if (NumByteToWrite <= secremain) //û�п�����
  secremain = NumByteToWrite;
  while (1)
  {
    ssi_w25qxx_read(W25QXX_BUF, secpos * 4096, 4096); //�����������������ݣ�������Buffer�С�
    for (i = 0; i < secremain; i++)//У������
    {
      if (W25QXX_BUF[secoff + i] != 0XFF)break; //����в�����0xFF�����ݣ�����Ҫ����
    }
    if (i < secremain) //��Ҫ����
    {
      ssi_w25qxx_erase_sector(secpos); //�����������
      for (i = 0; i < secremain; i++)  //���»����е�����
      {
        W25QXX_BUF[secoff + i] = pBuffer[i];
        //�����pBuffer��������Ҫд�����ݣ�����Щ���ݸ��µ������ж�Ӧ��λ��
      }
      ssi_w25qxx_write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //����д����������
    }
    else
      ssi_w25qxx_write_NoCheck(pBuffer, WriteAddr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
    if (NumByteToWrite == secremain) break; //д�������
    else //д��δ����
    {
      secpos++; //������ַ��1
      secoff = 0; //ƫ��λ��Ϊ0
      pBuffer += secremain;
      //ָ��ƫ��
      WriteAddr += secremain; //д��ַƫ��
      NumByteToWrite -= secremain; //�ֽ����ݼ�
      if (NumByteToWrite > 4096)
          secremain = 4096; //��һ����������д����
      else
          secremain = NumByteToWrite; //��һ����������д����
    }
  }
}
