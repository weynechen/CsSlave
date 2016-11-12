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
#include "string.h"

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
	uint8_t Protection;
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

static void W25Nxx_WriteStatus(uint8_t status_address , uint8_t value)
{
	uint8_t data[] = {FLASH_WRITE_STATUS , status_address , value};
	
	SPI_CS_L;
	HAL_SPI_Transmit(&hspi2,data,sizeof(data),1000);
	SPI_CS_H;		
}

static void W25Nxx_ReadProtection(void)
{
	uint8_t data[] = {FLASH_READ_STATUS , STATUS_ADDRESS1};
	
	SPI_CS_L;
	HAL_SPI_Transmit(&hspi2,data,sizeof(data),1000);
	HAL_SPI_Receive(&hspi2,&W25NxxProperty.Protection,1,1000);
	SPI_CS_H;	
}

static void W25Nxx_ReadCfg(void)
{
	uint8_t read_back;
	uint8_t data[] = {FLASH_READ_STATUS , STATUS_ADDRESS2};
	
	SPI_CS_L;
	HAL_SPI_Transmit(&hspi2,data,sizeof(data),1000);
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
	HAL_SPI_Transmit(&hspi2,data,sizeof(data),1000);
	HAL_SPI_Receive(&hspi2,&read_back,1,1000);
	SPI_CS_H;
	W25NxxProperty.Busy = (read_back >> 0) &0x01;
	W25NxxProperty.WriteEnable = (read_back >> 1) &0x01;
	W25NxxProperty.EraseFailure = (read_back >> 2) &0x01;
	W25NxxProperty.ProgramFailure = (read_back >> 3) &0x01;

}

static void W25Nxx_EnableBufMode(FunctionStateTypeDef state)
{
	uint8_t data[] = {FLASH_WRITE_STATUS , STATUS_ADDRESS2 , 0x10 | (state<<3)};
	
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



void W25Nxx_LoadPage(uint16_t page_address)
{
		uint8_t timeout = 255;
		uint8_t  load_page_xx[] = {FLASH_PAGE_DATA_READ,0xff,page_address >>8 , page_address & 0xff};
		SPI_CS_L;                             
		HAL_SPI_Transmit(&hspi2,(uint8_t*)load_page_xx,sizeof(load_page_xx),1000);   
		SPI_CS_H; 
		
		W25NxxProperty.Busy = 0 ;
		HAL_Delay(1000);
		W25Nxx_ReadStatus();
		while(W25NxxProperty.Busy)
		{
			 W25Nxx_ReadStatus();
			 timeout--;
			 if(timeout == 0)
				break;
		}
		}


void W25Nxx_ReadData(uint8_t *buf , uint32_t start_addr, uint32_t len)
{
	uint16_t column_address = start_addr&0xfff;
	uint16_t page_address = (start_addr >> 12) & 0xffff;
	
	uint8_t buff_read[] = {FLASH_READ , column_address >>8,column_address &0xff,0xff};
	
	if(buf == NULL)
		return;
	
	W25Nxx_EnableBufMode(FUN_ENABLE);
			W25Nxx_ReadCfg();
	W25Nxx_LoadPage(page_address);
	
	if(len <= PAGE_SIZE - column_address) //read a part of data
	{
	SPI_CS_L;                             
	HAL_SPI_Transmit(&hspi2,(uint8_t*)buff_read,sizeof(buff_read),1000);
	HAL_SPI_Receive(&hspi2,buf,len,1000);		
	SPI_CS_H; 
	}
	else //data will cross multi pages
	{
	SPI_CS_L;                             
	HAL_SPI_Transmit(&hspi2,(uint8_t*)buff_read,sizeof(buff_read),1000);
	HAL_SPI_Receive(&hspi2,buf,PAGE_SIZE - column_address,1000);
	SPI_CS_H;				
		
	W25Nxx_EnableBufMode(FUN_DISABLE);// set read mode to continues read
			W25Nxx_ReadCfg();
	//continues read from next page column 0
	W25Nxx_LoadPage(page_address+1);
	HAL_SPI_Transmit(&hspi2,(uint8_t*)buff_read,sizeof(buff_read),1000);
		HAL_Delay(1000);
		HAL_SPI_Receive(&hspi2,buf + PAGE_SIZE - column_address,len-(PAGE_SIZE - column_address),1000);		//recive the rest data
	}
	W25Nxx_EnableBufMode(FUN_ENABLE);	
	//W25Nxx_Reset();
}

void W25Nxx_ErasePage(uint16_t page_address)
{
	uint8_t  erase[] = {FLASH_PAGE_ERASE,DUMMY,page_address >>8 , page_address & 0xff};
		uint8_t timeout = 255;
	
	W25Nxx_WriteEnable();
	SPI_CS_L;                             
	HAL_SPI_Transmit(&hspi2,(uint8_t*)erase,sizeof(erase),1000);
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
	W25Nxx_WriteEnable();	
}


void LoadProgramData(uint16_t column_address , uint8_t *buf , uint16_t len)
{
	uint8_t  load_column[] = {FLASH_PROGRAM_DATA,column_address >>8 , column_address & 0xff};
	
	if(len > PAGE_SIZE - column_address)
		return;
	
	W25Nxx_WriteEnable();
	SPI_CS_L;                             
	HAL_SPI_Transmit(&hspi2,(uint8_t*)load_column,sizeof(load_column),1000);
	HAL_SPI_Transmit(&hspi2,(uint8_t*)buf,len,1000);	
	SPI_CS_H; 	
	W25Nxx_WriteDisable();		
}

void ProgramExecute(uint16_t page_address)
{
	uint8_t  execute[] = {FLASH_PROGRAM_EXECUTE,DUMMY,page_address >>8 , page_address & 0xff};
		uint8_t timeout = 255;
	
	W25Nxx_WriteEnable();
	SPI_CS_L;                             
	HAL_SPI_Transmit(&hspi2,(uint8_t*)execute,sizeof(execute),1000);
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
	W25Nxx_WriteDisable();
		
}

void W25Nxx_WriteData(uint8_t *buf , uint32_t blk_addr, uint16_t blk_len)
{
	uint16_t column_address = blk_addr&0xfff;
	uint16_t page_address = (blk_addr >> 12) & 0xffff;
	uint32_t first_page_address = blk_addr - column_address;
	
  //restore first page data
  W25Nxx_ReadData(SystemBuf,first_page_address,PAGE_SIZE);
	memcpy((uint8_t *)(SystemBuf + column_address),buf,PAGE_SIZE - column_address);
	W25Nxx_ErasePage(page_address);
	LoadProgramData(0,SystemBuf,PAGE_SIZE);
	ProgramExecute(page_address);
	
	if(column_address + blk_len > PAGE_SIZE)
	{
		uint16_t len = column_address + blk_len - PAGE_SIZE;
		uint16_t pages = len / PAGE_SIZE;
		uint16_t i = 1;
		while(pages != 0)
		{
			W25Nxx_ErasePage(page_address + i);
			LoadProgramData(0,(uint8_t *)(buf + PAGE_SIZE - column_address + i * PAGE_SIZE),PAGE_SIZE);			
			ProgramExecute(page_address + i);
			i++;
			pages--;
		}
		
		//restore last page data
	W25Nxx_ReadData(SystemBuf,(uint32_t)((page_address + i) << 12),PAGE_SIZE);
	memcpy(SystemBuf,(uint8_t *)(buf + PAGE_SIZE - column_address + (i-1) * PAGE_SIZE) ,blk_len - (PAGE_SIZE - column_address + (i-1) * PAGE_SIZE));
	W25Nxx_ErasePage(page_address + i);
	LoadProgramData(0,SystemBuf,PAGE_SIZE);
	ProgramExecute(page_address + i);	
		
	}
}

void W25Nxx_Init(void)
{
	uint8_t buf[3];
	char flash_data[]="flash data:";
	W25Nxx_Reset();
	HAL_Delay(10);
	
	if(W25Nxx_ReadID(buf)== HAL_OK)
	{
		char c[]="Info:SPI Flash OK: ";
		UserSendArray(c,buf,3);
		W25Nxx_WriteStatus(STATUS_ADDRESS1,0); //remove protection
		W25Nxx_ReadCfg();
		W25Nxx_ReadStatus();
		W25Nxx_ReadProtection();
		UserSendArray(c,(uint8_t *)&W25NxxProperty,sizeof(W25NxxProperty));		
	}
	else
	{
		UserPrintf("Error:SPI Flash Error\n");
	}
	memset(RecBuffer,0xAA,512);
 	RecBuffer[2049]=0x11;
	RecBuffer[10]=0x10;
	//W25Nxx_ErasePage(0);
	//W25Nxx_WriteData(RecBuffer,0,512);
	W25Nxx_ReadData(SystemBuf , 0 , 3000);
//	UserSendArray(flash_data,&RecBuffer[2047],4);
}
