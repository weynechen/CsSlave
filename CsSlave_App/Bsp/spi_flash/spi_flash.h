/**
 * @file        spi_flash.h
 * @author      Weyne
 * @version     V01
 * @date        2016.10.24
 * @brief       spi flash驱动部分
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#ifndef _SPI_FLASH_H
#define _SPI_FLASH_H

typedef enum
{
	FLASH_RESET = 0xff,
	FLASH_PAGE_ERASE = 0xD8,
	FLASH_PROGRAM_EXECUTE = 0x10,
	FLASH_ID = 0x9f,
	FLASH_PROGRAM_DATA = 0x02,
	FLASH_READ = 0x03,
	FLASH_PAGE_DATA_READ = 0x13,
	FLASH_READ_STATUS  = 0x0F,
	FLASH_WRITE_STATUS = 0x1F,
	FLASH_WRITE_ENABLE = 0x06,
	FLASH_WRITE_DISABLE = 0x04,
}FlashOpCodeTypeDef;

typedef enum
{
	STATUS_ADDRESS1 = 0xA0,
	STATUS_ADDRESS2 = 0xB0,
	STATUS_ADDRESS3 = 0xC0,
}FlashStatusAddressTypeDef;

#define PAGE_SIZE 2048
#define DUMMY 0xff
void W25Nxx_Init(void);
void W25Nxx_ReadData(uint8_t *buf , uint32_t blk_addr, uint16_t blk_len);
#endif
