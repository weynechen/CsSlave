/**
 * @file        pro.h
 * @author      陈维
 * @version     V01
 * @date        2016.09.21
 * @brief       协议定义
 * @note
 *
 * @attention   COYPRIGHT WEYNE
 **/

#ifndef _PRO_H_
#define _PRO_H_

#define LCD_ADDRESS    0x10
#define MIN_PRO_NUM      14

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


//数据包头尾、控制字
#define P_HEADER     0xAA
#define P_TAIL       0x55
#define P_CTRL       0xA5
#define P_FAIL       0
#define P_SUCCESS    1


typedef struct
{
    uint8_t							  DataID;
    uint8_t               *DataInBuff;
    uint32_t              DataInLen;
    uint8_t               *DataOutBuff;
    uint32_t              *DataOutLen;
} PackageDataStruct;


#endif



/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
