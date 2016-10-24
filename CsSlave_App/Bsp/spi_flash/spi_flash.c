/**
 * @file        spi_flash.c
 * @author      Weyne
 * @version     V01
 * @date        2016.10.24
 * @brief       spi flash驱动部分
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#include "sysconfig.h"
#include "spi_flash.h"
#include "spi.h"
#include "ack.h"

#define  SPI_CS_L HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin,GPIO_PIN_RESET)
#define  SPI_CS_H HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin,GPIO_PIN_SET)

static uint8_t ReadBackBuffer[9];


static HAL_StatusTypeDef ReadWriteFlash(uint8_t *byte ,uint8_t byte_amount)
{
	HAL_StatusTypeDef state;
	
	if(byte_amount >= sizeof(ReadBackBuffer))
		return HAL_ERROR;
	
	SPI_CS_L;
	state = HAL_SPI_TransmitReceive(&hspi2,(uint8_t*)byte,ReadBackBuffer,byte_amount,1000);
	SPI_CS_H;
	return state;
}

HAL_StatusTypeDef ResetSpiFlash(void)
{
	FlashOpCodeTypeDef reset = FLASH_Reset;
	return ReadWriteFlash((uint8_t *)&reset,sizeof(reset));
}


HAL_StatusTypeDef ReadSpiFlashID(void)
{
	uint8_t  id[] = {FLASH_ID,0xff,0xff,0xff,0xff};
	return ReadWriteFlash((uint8_t *)id,sizeof(id));	
}


void SPI_FlashCheck(void)
{
	ReadSpiFlashID();
	HAL_Delay(10);
	
	if(ReadSpiFlashID()== HAL_OK)
	{
		char c[]="Info:SPI Flash OK: ";
		UserSendArray(c,(uint8_t *)(ReadBackBuffer+2),3);
	}
	else
	{
		UserPrintf("Error:SPI Flash Error\n");
	}
}
