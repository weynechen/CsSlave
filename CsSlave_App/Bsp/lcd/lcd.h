/**
 * @file        lcd.h
 * @author      Weyne
 * @version     V01
 * @date        2016.08.23
 * @brief       LCD驱动部分
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#ifndef _LCD_H_
#define _LCD_H_
#include "stm32f1xx_hal.h"


#define RESET_PINA     GPIO_PIN_2
#define RESET_PORTA    GPIOA

typedef enum
{
  MIPI_START,
  MIPI_DCS,
  MIPI_GP,
  MIPI_DELAY,
  MIPI_WRITE,
  MIPI_READ,
  MIPI_END
} MipiTypeDef;

typedef enum
{
  NO_PACKAGE,
  GP,
  DCS
} MipiPackageDef;

typedef enum
{
  PATTERN_START,
  RGB,
  FLICKERV,
  FLICKERH,
  COLORBARV,
  COLORBARH,
  GRAYLEVEL_V,
  GRAYLEVEL_H,
  CROSSTALK,
  PIC,
  FRAME,
  CHESSBOARD,
  RGBBAR,
  RGBLEVEL,
  PATTERN_STAY,
  NULL_PATTERN,
  PATTERN_END
} PatternTypeDef;



void Lcd_ReInit(void);

#endif
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
