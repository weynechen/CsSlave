/**
  * @file        upgrade.h
  * @author      陈维
  * @version     V01
  * @date        2016.09.21
  * @brief       firmware升级
  * @note        
  *
  * @attention   COYPRIGHT INMOTION ROBOT
  **/
#ifndef _UPGRADE_H_
#define _UPGRADE_H_

typedef enum {
  FW_UPDATE_READY,
  FW_CRC_ERROR,
  FW_FLASH_ERROR,
  FW_OK,
  FW_NULL = 0xff,
} FirmwareUpdateType;


#endif

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
