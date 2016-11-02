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
	FLASH_Reset = 0xff,
	FLASH_ID = 0x9f,
	FLASH_WRITE = 0x02,
	FLASH_READ = 0x03,
	FLASH_ReadStatus  = 0x0F,
	FLASH_WriteStatus = 0x1F,
	FLASH_WriteEnable = 0x06,
	FLASH_WriteDisable = 0x04,
}FlashOpCodeTypeDef;


HAL_StatusTypeDef ReadSpiFlashID(void);
void SPI_FlashCheck(void);
void ReadSPIFlash(uint8_t *buf , uint32_t blk_addr, uint16_t blk_len);

#endif
