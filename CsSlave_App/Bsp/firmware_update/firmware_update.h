/**
  * @file        firmware_update.h
  * @author      陈维
  * @version     V01
  * @date        2016.06.27
  * @brief       将firmware写入到flash空闲区域
  * @note
  *
  * @attention   COYPRIGHT INMOTION ROBOT
  **/

#ifndef _FIRMWARE_UPDATE_H_
#define _FIRMWARE_UPDATE_H_
#include "usart.h"
#include "pro.h"

#define BOOT_BASE_ADDRESS 0x08000000
#define APP_BASE_ADDRESS 0x0800C000
#define FIRMWARE_BASE_ADDRESS 0x8020000
#define FIRMWARE_SECTOR FLASH_Sector_5
#define FIRMWARE_VERSION_ADDRESS 0x0801FC00

typedef enum {
  FW_UPDATE_READY,
  FW_CRC_ERROR,
  FW_FLASH_ERROR,
  FW_OK,
  FW_NULL = 0xff,
} FirmwareUpdateType;

typedef struct
{
  s32 Offset;
  u32 DataLen; // 有效数据的长度
  u32 Crc32; // 数据区的CRC
  u32 DataBaseAddress;
} FirmwareDataStruct;

void SendSignal(u8 signal);
void HanldeFirmwareData(void);
void CloseLidar(void);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
