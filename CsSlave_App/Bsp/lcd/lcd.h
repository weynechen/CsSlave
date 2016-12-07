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
#include "power.h"

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

typedef struct
{
	uint32_t Background;
	uint32_t Fore;
}FontColorTypeDef;


void Lcd_ReInit(void);
uint8_t IsStayTimeOver(uint8_t frame);
void Lcd_Sleep(void);
void SetLcdPower(StateTypeDef state);
void Lcd_LightOn(void);
void ResetStayTimeCounter(void);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t chars);
void LCD_ShowString(uint16_t x,uint16_t y,const char *p);
extern FontColorTypeDef FontColor;

#endif
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
