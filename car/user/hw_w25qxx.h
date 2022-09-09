#ifndef HW_W25QXX_H_
#define HW_W25QXX_H_


//===芯片ID
#define W25Q80 0xEF13
#define W25Q16 0xEF14
#define W25Q32 0xEF15
#define W25Q64 0xEF16

//===命令指令集
#define W25QXX_CMD_ENABLE_WRITE              0x06
#define W25QXX_CMD_ENABLE_VOLATILE_WRITE     0x50
#define W25QXX_CMD_DISABLE_WRITE             0x04
#define W25QXX_CMD_READ_REG_SR1              0x05
#define W25QXX_CMD_READ_REG_SR2              0x35
#define W25QXX_CMD_WRITE_REG_SR              0x01
#define W25QXX_CMD_READ_DATA                 0x03
#define W25QXX_CMD_READ_FAST                 0x0B
#define W25QXX_CMD_READ_DUAL_FAST            0x3B
#define W25QXX_CMD_PAGE_PROGRAM              0x02
#define W25QXX_CMD_ERASE_SECTOR              0x20
#define W25QXX_CMD_ERASE_BLOCK_64KB          0xD8
#define W25QXX_CMD_ERASE_BLOCK_32KB          0x52
#define W25QXX_CMD_ERASE_CHIP                0xC7
#define W25QXX_CMD_ERASE_SUSPEND             0x75
#define W25QXX_CMD_ERASE_RESUME              0x7A
#define W25QXX_CMD_POWER_DOWN                0xB9
#define W25QXX_CMD_RELEASE_POWER_DOWN        0xAB
#define W25QXX_CMD_DEVICE_ID                 0xAB
#define W25QXX_CMD_MANUFACT_DEVICE_ID        0x90
#define W25QXX_CMD_READ_UNIQUE_ID            0x4B
#define W25QXX_CMD_JEDEC_ID                  0x9F
#define W25QXX_CMD_ERASE_SECURITY_REG        0x44
#define W25QXX_CMD_PROGRAM_SECURITY_REG      0x42
#define W25QXX_CMD_READ_SECURITY_REG         0x48
#define W25QXX_CMD_ENABLE_RESET              0x66
#define W25QXX_CMD_RESET                     0x99

//===地址
#define W25QXX_SECURITY_REG1              0x001000
#define W25QXX_SECURITY_REG2              0x002000
#define W25QXX_SECURITY_REG3              0x003000

//===每页的字节数
#define W25QXX_PAGE_BYTE_SIZE             256
#define W25QXX_PAGE_NUM_MASK              0xFFFFFF00
#define W25QXX_PAGE_BYTE_MASK             0xFF

//===每个扇区的字节数
#define W25QXX_SECTOR_BYTE_SIZE           4096
#define W25QXX_SECTOR_NUM_MASK            0xFFFF0000
#define W25QXX_SECTOR_BYTE_MASK           0xFFFF

//===32Kb大小的块字节数
#define W25QXX_BLOCK_32KB_BYTE_SIZE       4096*32
#define W25QXX_BLOCK_32KB_NUM_MASK        0xFFFE0000
#define W25QXX_BLOCK_32KB_BYTE_MASK       0x1FFFF

//===64Kb大小的块字节数
#define W25QXX_BLOCK_64KB_BYTE_SIZE       4096*64
#define W25QXX_BLOCK_64KB_NUM_MASK        0xFFFC0000
#define W25QXX_BLOCK_64KB_BYTE_MASK       0x3FFFF

extern void ssi0_init(void);
extern void gpio_ss_init(void);
extern uint16_t w25qxx_readID(void);
extern void ssi_w25qxx_read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
extern void ssi_w25qxx_write_page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite) ;//写一页
extern void ssi_w25qxx_write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); //写 ;
extern  void ssi_w25qxx_write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); //写，但不校验是否已擦除;


#endif /* HW_W25QXX_H_ */
