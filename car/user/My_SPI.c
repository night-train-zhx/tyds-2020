#include "My_SPI.h"
/*
*名称：Drv_SpiInit
*功能：SSI初始化
*注意：请注意 设备模式 是否一致
*/

void Drv_SpiInit(void)
{	
	ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_SSI );
	ROM_SysCtlPeripheralEnable(SPI_SYSCTL_PERIPH);
	/*配置IO口*/	
	ROM_GPIOPinTypeSSI(SPI_PROT,SPI_CLK_PIN|SPI_RX_PIN|SPI_TX_PIN);
	ROM_GPIOPinConfigure(SPI_CLK);	
	ROM_GPIOPinConfigure(SPI_RX);
	ROM_GPIOPinConfigure(SPI_TX);
	/* SSI配置 模式3(Polarity = 1 Phase = 1) 主设备模式 速率1MHz 数据长度8位*/
	ROM_SSIConfigSetExpClk(SPI_BASE, ROM_SysCtlClockGet(),
						SSI_FRF_MOTO_MODE_3,  SSI_MODE_MASTER, 1000000,  8);
	/*开启SSI0*/
	ROM_SSIEnable(SPI_BASE);
}

/* SPI读写函数 */
uint8_t Drv_SpiSingleWirteAndRead(uint8_t SendData)
{
    uint32_t ui_TempData;
    uint8_t uc_ReceiveData;
    /* 向SSI FIFO写入数据 */
    ROM_SSIDataPut(SPI_BASE, SendData);
    /* 等待SSI不忙 */
    while(ROM_SSIBusy(SPI_BASE));
    /* 从FIFO读取数据 */
    ROM_SSIDataGet(SPI_BASE, &ui_TempData);
    /* 截取数据的低八位 */
    uc_ReceiveData = ui_TempData & 0xff;
    return uc_ReceiveData;
}

void Drv_SpiTransmit(uint8_t *ucp_Data, uint16_t us_Size)
{
    uint16_t i = 0;
    /* 连续写入数据 */
    for(i = 0; i < us_Size; i++)
    {
        Drv_SpiSingleWirteAndRead(ucp_Data[i]);
    }
}

void Drv_SpiReceive(uint8_t *ucp_Data, uint16_t us_Size)
{
    uint16_t i = 0;
    /* 连续读取数据 */
    for(i = 0; i < us_Size; i++)
    {
        ucp_Data[i] = Drv_SpiSingleWirteAndRead(0xFF);
    }
}

