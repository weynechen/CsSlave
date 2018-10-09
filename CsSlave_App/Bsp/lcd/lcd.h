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
#include <stdbool.h>

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
  RGB_START,
  RGB_SPI_RISING,
  RGB_SPI_FALLING,
  RGB_DELAY,
  RGB_WRITE,
  RGB_READ,
  RGB_END
} RGBTypeDef;

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
  SHOW_ID,
  SLEEP_IN,
  SLEEP_OUT,
  FLICKER_DOT,
  GRAYLEVEL_V_USER,
  GRAYLEVEL_H_USER,
  CROSSTALK_WHITE,
  PATTERN_END
} PatternTypeDef;

typedef struct
{
  uint32_t Background;
  uint32_t Fore;
} FontColorTypeDef;

typedef enum
{
  PAGE_UP,
  PAGE_DOWN,
} PageTurningTypeDef;

typedef enum
{
  MIPI_LCD,
  RGB_SPI16BIT,
  RGB_SPI8BIT,
  RGB_SPI9BIT,
  SPI_2_Data_Lane,
  LVDS_666_VESA,
  LVDS_666_JEIDA,
  LVDS_888_VESA,
  LVDS_888_JEIDA,
} LcdTypeDef;

typedef enum
{
  F_COLUMN,
  F_DOT,
  F_ROW,
} FlickertypeDef;

void Lcd_ReInit(void);
uint8_t IsStayTimeOver(uint8_t frame);
void MipiLcdSleepIn(void);
void MipiLcdSleepOut(void);
void closeAllPower(void);
void SetLcdPower(StateTypeDef state);
void PowerOff(void);
void PowerOn(void);
void Lcd_LightOn(void);
void ResetStayTimeCounter(void);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t chars);
void LCD_ShowString(uint16_t x, uint16_t y, const char *p);
void PageTurning(PageTurningTypeDef page);
void ResetMipiLcd(void);
void SetMipiPara(void);
void PrepareBg(void);
void LCD_SetFontScale(uint8_t scale);
void LCD_SetFlickerType(FlickertypeDef type);
void LCD_EraseFlickerString(void);
void SetFontColor(uint32_t color);
void SetBgColor(uint32_t color);
void OpInLPMode(void);
void EnterBg(void);
void LCD_PrintfSetAddress(uint16_t x , uint16_t y);
int LCD_Printf(char *fmt, ...);
void LCD_ClearPrintf(void);
void LCD_GetCurrAddress(uint16_t *x , uint16_t *y);
void LCD_SetCurrAddress(uint16_t x , uint16_t y);
void LCD_ClearLine(void);
bool InspectionAfterPowerOn(void);

extern FontColorTypeDef FontColor;
extern uint8_t FontScale;


#endif
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
