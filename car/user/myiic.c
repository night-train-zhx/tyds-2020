
#include "myiic.h"

/*****************************
		A6 - SCL
	  A7 - SDA
*****************************/

void IIC_Init(void)
{
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1); // Enable I2C1 peripheral
  ROM_SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable GPIOA peripheral
  ROM_SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  
  // Use alternate function
  ROM_GPIOPinConfigure(GPIO_PA6_I2C1SCL);
  ROM_GPIOPinConfigure(GPIO_PA7_I2C1SDA);
  
  ROM_GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6); // Use pin with I2C SCL peripheral
  ROM_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7); // Use pin with I2C peripheral
  
	//                            模块系统时钟   数据速率：true 速率 400 kbit/s（块速），，false 100（标速）
  I2CMasterInitExpClk(I2C1_BASE, 400*100000,true); // Enable and set frequency to 400 kHz
  

}


void  DEV_Write_Len(uint8_t addr, uint8_t regAddr, uint8_t length, uint8_t *data) 
{
  ROM_I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode  
	ROM_SysCtlDelay(2); // Insert a few cycles after enabling the I2C to allow the clock to be fully activated
		
  ROM_I2CMasterDataPut(I2C1_BASE, regAddr); // Place address into data register
  ROM_I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START); // Send start condition
		
  while (ROM_I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  
  for (uint8_t i = 0; i < length - 1; i++) {
    ROM_I2CMasterDataPut(I2C1_BASE, data[i]); // Place data into data register
    ROM_I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); // Send continues condition
    while (ROM_I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  }
  //最后一个字节搭配的是 结束信号
  ROM_I2CMasterDataPut(I2C1_BASE, data[length - 1]); // Place data into data register
  ROM_I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // Send finish condition
  while (ROM_I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
}



void  DEV_Read_Len(uint8_t addr, uint8_t regAddr, uint8_t length, uint8_t *data) 
{
  ROM_I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode
  ROM_I2CMasterDataPut(I2C1_BASE, regAddr); // Place address into data register
  ROM_I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
		while (ROM_I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done 
	
  ROM_I2CMasterSlaveAddrSet(I2C1_BASE, addr, true); // Set to read mode
  ROM_I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START); // Send start condition
		while (ROM_I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  data[0] = ROM_I2CMasterDataGet(I2C1_BASE); // Place data into data register 
  for (uint8_t i = 1; i < length - 1; i++) 
	{
    ROM_I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT); // Send continues condition
			while (ROM_I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
    data[i] = ROM_I2CMasterDataGet(I2C1_BASE); // Place data into data register
  }
  ROM_I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); // Send finish condition
		while (ROM_I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  data[length - 1] = ROM_I2CMasterDataGet(I2C1_BASE); // Place data into data register
}

void  DEV_Write_Byte(uint8_t addr, uint8_t regAddr, uint8_t data) 
{
  DEV_Write_Len(addr, regAddr, 1, &data);
}

uint8_t DEV_Read_Byte(uint8_t addr, uint8_t regAddr) 
{
  ROM_I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode
  
  ROM_I2CMasterDataPut(I2C1_BASE, regAddr); // Place address into data register
  ROM_I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
		while (ROM_I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  
  ROM_I2CMasterSlaveAddrSet(I2C1_BASE, addr, true); // Set to read mode
  
  ROM_I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); // Tell master to read data
		while (ROM_I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
	return ROM_I2CMasterDataGet(I2C1_BASE); // Read data
}

short int Double_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address)
{
  unsigned char msb , lsb ;
  msb = DEV_Read_Byte(SlaveAddress,REG_Address);
  lsb = DEV_Read_Byte(SlaveAddress,REG_Address+1);
  return ( ((short int)msb) << 8 | lsb) ;
}


