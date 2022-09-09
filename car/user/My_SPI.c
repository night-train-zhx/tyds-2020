#include "My_SPI.h"
/*
*���ƣ�Drv_SpiInit
*���ܣ�SSI��ʼ��
*ע�⣺��ע�� �豸ģʽ �Ƿ�һ��
*/

void Drv_SpiInit(void)
{	
	ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_SSI );
	ROM_SysCtlPeripheralEnable(SPI_SYSCTL_PERIPH);
	/*����IO��*/	
	ROM_GPIOPinTypeSSI(SPI_PROT,SPI_CLK_PIN|SPI_RX_PIN|SPI_TX_PIN);
	ROM_GPIOPinConfigure(SPI_CLK);	
	ROM_GPIOPinConfigure(SPI_RX);
	ROM_GPIOPinConfigure(SPI_TX);
	/* SSI���� ģʽ3(Polarity = 1 Phase = 1) ���豸ģʽ ����1MHz ���ݳ���8λ*/
	ROM_SSIConfigSetExpClk(SPI_BASE, ROM_SysCtlClockGet(),
						SSI_FRF_MOTO_MODE_3,  SSI_MODE_MASTER, 1000000,  8);
	/*����SSI0*/
	ROM_SSIEnable(SPI_BASE);
}

/* SPI��д���� */
uint8_t Drv_SpiSingleWirteAndRead(uint8_t SendData)
{
    uint32_t ui_TempData;
    uint8_t uc_ReceiveData;
    /* ��SSI FIFOд������ */
    ROM_SSIDataPut(SPI_BASE, SendData);
    /* �ȴ�SSI��æ */
    while(ROM_SSIBusy(SPI_BASE));
    /* ��FIFO��ȡ���� */
    ROM_SSIDataGet(SPI_BASE, &ui_TempData);
    /* ��ȡ���ݵĵͰ�λ */
    uc_ReceiveData = ui_TempData & 0xff;
    return uc_ReceiveData;
}

void Drv_SpiTransmit(uint8_t *ucp_Data, uint16_t us_Size)
{
    uint16_t i = 0;
    /* ����д������ */
    for(i = 0; i < us_Size; i++)
    {
        Drv_SpiSingleWirteAndRead(ucp_Data[i]);
    }
}

void Drv_SpiReceive(uint8_t *ucp_Data, uint16_t us_Size)
{
    uint16_t i = 0;
    /* ������ȡ���� */
    for(i = 0; i < us_Size; i++)
    {
        ucp_Data[i] = Drv_SpiSingleWirteAndRead(0xFF);
    }
}

