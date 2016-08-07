/**
 * @file        cdce913.h
 * @author      Weyne
 * @version     V01
 * @date        2016.08.07
 * @brief       CDCE913的配置
 * @attention   COYPRIGHT WEYNE
 */
#ifndef _CDCE913_H_
#define _CDCE913_H_
#include "sys.h"

typedef enum
{
  I2C_NO_ACK,
  I2C_ACK
} AckTypeDef;

#define CDCE_ADDRESS    0x65
#define CDCE_ID         0x81
#define I2C_WRITE    CDCE_ADDRESS << 1
#define I2C_READ     ((CDCE_ADDRESS << 1) | 0x01)

#define I2C_SCL      PBout(8)
#define I2C_SDA      PBout(9)
#define READ_SDA     PBin(9)

#define S0           PCout(6)


void CDCE_Init(uint16_t f_out);

#endif

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
