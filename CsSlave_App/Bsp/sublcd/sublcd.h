/**
* @file         sublcd.h
* @author       Weyne Chen
* @version      V01
* @date         2017.11.27
* @brief         
* @note          
* @attention    COPYRIGHT WEYNE
**/

#ifndef __SUBLCD_H
#define __SUBLCD_H
#include "stm32f1xx_hal.h"

void SubLCD_Init(void);
void SubLCD_WriteFull(uint16_t color);
void SubLCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p);
void SubLCD_Fill(uint16_t ybegin, uint16_t yend);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
#endif
/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
