/**
  * @file        firmwareupgrade.h
  * @author      陈维
  * @version     V01
  * @date        2016.10.29
  * @brief       firmware更新函数
  * @note		 
  *
  * @attention   COYPRIGHT WEYNE
  **/

#ifndef _FIRMWARE_RW_H_
#define _FIRMWARE_RW_H_
#include "stm32f1xx_hal.h"
#include "system_config.h"


#define KEY_STORE 0x5A2F5D81



typedef void (*IapFun)(void);

typedef struct
{
	uint32_t SectionLen;
	uint32_t SectionRealLen;
	uint32_t SectionAddress;
	uint32_t SectionCRC32;
	uint32_t SectionData; //保留这个是为了计算Data的地址偏移量base address
}SectionTypeDef;

typedef struct
{
	uint32_t FileSize;
	uint32_t EncryptFlag;
	uint32_t SectionNumber;
	uint32_t RandomValue[4];
	SectionTypeDef Section;
	uint32_t FileCRC32;
}FirmwareTypeDef;

void WriteFirmwareToApp(void);
bool IsNeedToUpdate(void);
void JumpToApp(void);
bool CheckAppValidity(void);

#endif 
 
 /************************ (C) COPYRIGHT WEYNE *****END OF FILE****/

