/**
 * @file        ssd2828.h
 * @author      Weyne
 * @version     V01
 * @date        2016.08.07
 * @brief       SSD2828的驱动
 * @note
 * @attention   COYPRIGHT WEYNE
 */

#ifndef _SSD2828_H_
#define _SSD2828_H_
#include "stm32f1xx_hal.h"

typedef enum
{
  MIPI_READ_FAIL,
  MIPI_READ_SUCCEED
} MIPI_ReadTypeDef;

typedef enum
{
  LP,
  HS,
  VD
} MIPI_ModeTypeDef;


void SSD2828_ShutDown(FunctionalState state);
void SSD2828_Init(uint8_t lane, uint16_t data_rate);
void SSD2828_SetMode(MIPI_ModeTypeDef m);
MIPI_ReadTypeDef SSD2828_GenericReadDT14(uint8_t adr, uint16_t l, uint8_t *p);
MIPI_ReadTypeDef SSD2828_DcsReadDT06(uint8_t adr, uint16_t l, uint8_t *p);
void SSD2828_DcsShortWrite(uint8_t n);
void SSD2828_DcsLongWrite(uint32_t n);
void SSD2828_GenericShortWrite(uint8_t n);
void SSD2828_GenericLongWrite(uint32_t n);
void SSD2828WriteData(uint8_t data);
uint16_t SSD2828ReadReg(uint8_t reg);
void SSD2828WriteReg(uint8_t cmd, uint8_t dat1, uint8_t dat2);
void SSD2828_SetReset(uint8_t t);

#endif
