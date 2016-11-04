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

static FlashOpCodeTypeDef OpCode;

struct
{
	uint8_t ECC_Enable;
	uint8_t BufMode;
	uint8_t ProgramFailure;
	uint8_t EraseFailure;
	uint8_t WriteEnable;
	uint8_t Busy;	
}W25NxxProperty;
	
#define WRITE_OP_CODE \
do{\
		SPI_CS_L;\
		HAL_SPI_Transmit(&hspi2,(uint8_t*)&OpCode,1,1000);\
		SPI_CS_H;\
 } while(0)

	
void W25Nxx_Reset(void)
{
	OpCode = FLASH_RESET;
	WRITE_OP_CODE;
}


static HAL_StatusTypeDef W25Nxx_ReadID(uint8_t *read_back_buf)
{
	HAL_StatusTypeDef status;
	OpCode = FLASH_ID;
	SPI_CS_L;
	status = HAL_SPI_Transmit(&hspi2,(uint8_t*)&OpCode,2,1000);
	status = HAL_SPI_Receive(&hspi2,read_back_buf,3,1000);
	SPI_CS_H;
	
	return status;
}

static void W25Nxx_ReadCfg(void)
{
	uint8_t read_back;
	uint8_t data[] = {FLASH_READ_STATUS , STATUS_ADDRESS2};
	
	SPI_CS_L;
	HAL_SPI_Transmit(&hspi2,data,2,1000);
	HAL_SPI_Receive(&hspi2,&read_back,1,1000);
	SPI_CS_H;
	W25NxxProperty.ECC_Enable = (read_back >> 4) &0x01;
	W25NxxProperty.BufMode = (read_back >> 3) &0x01;
}

static void W25Nxx_ReadStatus(void)
{
	uint8_t read_back;
	uint8_t data[] = {FLASH_READ_STATUS , STATUS_ADDRESS3};
	
	SPI_CS_L;
	HAL_SPI_Transmit(&hspi2,data,2,1000);
	HAL_SPI_Receive(&hspi2,&read_back,1,1000);
	SPI_CS_H;
	W25NxxProperty.Busy = (read_back >> 0) &0x01;
	W25NxxProperty.WriteEnable = (read_back >> 1) &0x01;
	W25NxxProperty.EraseFailure = (read_back >> 2) &0x01;
	W25NxxProperty.ProgramFailure = (read_back >> 3) &0x01;

}

static void W25Nxx_SetNonBufMode(void)
{
	uint8_t read_back;
	uint8_t data[] = {FLASH_WRITE_STATUS , STATUS_ADDRESS2 , 0x10};
	
	SPI_CS_L;
	HAL_SPI_Transmit(&hspi2,data,sizeof(data),1000);
	SPI_CS_H;
}


void W25Nxx_WriteEnable(void)
{
	OpCode = FLASH_WRITE_ENABLE;
	WRITE_OP_CODE;	
}

void W25Nxx_WriteDisable(void)
{
	OpCode = FLASH_WRITE_DISABLE;
	WRITE_OP_CODE;	
}

void W25Nxx_Init(void)
{
	uint8_t buf[3];
	
	W25Nxx_Reset();
	HAL_Delay(10);
	
	if(W25Nxx_ReadID(buf)== HAL_OK)
	{
		char c[]="Info:SPI Flash OK: ";
		UserSendArray(c,buf,3);
		W25Nxx_ReadCfg();
		W25Nxx_ReadStatus();

		UserSendArray(c,(uint8_t *)&W25NxxProperty,sizeof(W25NxxProperty));		
	}
	else
	{
		UserPrintf("Error:SPI Flash Error\n");
	}
}

void W25Nxx_LoadPage(uint16_t page_address)
{
		uint8_t timeout = 255;
		uint8_t  load_page_xx[] = {FLASH_PAGE_DATA_READ,0xff,page_address >>8 , page_address & 0xff};
		SPI_CS_L;                             
		HAL_SPI_Transmit(&hspi2,(uint8_t*)load_page_xx,sizeof(load_page_xx),1000);   
		SPI_CS_H; 
		
		W25NxxProperty.Busy = 0 ;
		W25Nxx_ReadStatus();
		while(W25NxxProperty.Busy)
		{
			 W25Nxx_ReadStatus();
			 timeout--;
			 if(timeout == 0)
				break;
		}		
}


void W25Nxx_ReadData(uint8_t *buf , uint32_t blk_addr, uint16_t blk_len)
{
	uint16_t column_address = blk_addr&0xfff;
	uint16_t page_address = (blk_addr >> 12) & 0xffff;

	uint8_t buff_read[] = {FLASH_READ , column_address >>8,column_address &0xff,0xff};
	
	if(buf == NULL)
		return;
	
	W25Nxx_LoadPage(page_address);
	
	if(blk_len < PAGE_SIZE - column_address) //read a part of data
	{
	SPI_CS_L;                             
	HAL_SPI_Transmit(&hspi2,(uint8_t*)buff_read,sizeof(buff_read),1000);
	HAL_SPI_Receive(&hspi2,buf,blk_len,1000);		
	SPI_CS_H; 
	}
	else //data will cross multi pages
	{
	SPI_CS_L;                             
	HAL_SPI_Transmit(&hspi2,(uint8_t*)buff_read,sizeof(buff_read),1000);
	HAL_SPI_Receive(&hspi2,buf,PAGE_SIZE - column_address,1000);
	SPI_CS_H;				
		
	W25Nxx_SetNonBufMode();// set read mode to continues read
	
	W25Nxx_LoadPage(page_address+1);		
	HAL_SPI_Transmit(&hspi2,(uint8_t*)buff_read,sizeof(buff_read),1000);
	HAL_SPI_Receive(&hspi2,buf,blk_len-(PAGE_SIZE - column_address),1000);		//recive the rest data
	}
	
	W25Nxx_Reset();
}


void W25Nxx_WriteData(uint8_t *buf , uint32_t blk_addr, uint16_t blk_len)
{
	HAL_StatusTypeDef state;

	uint8_t  read[] = {FLASH_WRITE,(uint8_t)(blk_addr>>16),(uint8_t)(blk_addr>>8),(uint8_t)(blk_addr>>0)};	
	
	if(buf == NULL)
		return;
	

	read[0] = 
	
		state = HAL_SPI_Receive(&hspi2,buf,blk_len,1000); 
	
}
