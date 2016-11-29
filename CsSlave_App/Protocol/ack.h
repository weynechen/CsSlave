/**
 * @file        ack.h
 * @author      Weyne
 * @version     V01
 * @date        2016.08.24
 * @brief       通讯返回部分
 * @note
 * @attention   COYPRIGHT WEYNE
 */

#ifndef _ACK_H_
#define _ACK_H_
#include "stm32f1xx_hal.h"
#include "pro.h"

typedef enum
{
  ACK_STRING,
  ACK_UPGRADE,
  ACK_HEATBEATS,
  ACK_NULL = 0xff,
} AckDataIDTypeDef;


ResultTypeDef Package(PackageDataStruct package);
void UserPrintf(char *fmt, ...);
void UserSendArray(char *c, uint8_t *array, uint8_t number_size);
void SendUpgradeSignal(uint8_t signal);
void SendHeartBeat(void);

#endif

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
