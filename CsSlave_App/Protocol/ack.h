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

typedef enum
{
    PACK_FAIL,
    PACK_OK
} ResultTypeDef;

#define NULL    0


typedef struct
{
    uint8_t  DeviceAddr;   //  cmd
    uint8_t  FunctionCode; // sub cmd
    uint16_t StartAddr;    //sub sub cmd
    uint32_t Len;
} SdkProtocolHeaderTypeDef;


/**
 * @brief  数据包头尾控制字
 */
typedef enum
{
  P_HEADER = 0xAA,
  P_TAIL = 0x55,
  P_CTRL = 0xA5,
} PackCtrlWordTypeDef;


typedef enum
{
	ACK_STRING,
	ACK_NULL = 0xff,
}AckDataIDTypeDef;



typedef struct
{
    AckDataIDTypeDef DataID;
    uint8_t               *DataInBuff;
    uint32_t              DataInLen;
    uint8_t               *DataOutBuff;
    uint32_t              *DataOutLen;
} PackageDataStruct;

ResultTypeDef Package(PackageDataStruct package);
void UserPrintf(char * fmt, ...);

#endif

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
