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

#endif
/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
