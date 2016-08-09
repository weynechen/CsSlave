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

void CDCE_Init(uint16_t f_out);

#endif

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
