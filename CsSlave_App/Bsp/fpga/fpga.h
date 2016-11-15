/**
 * @file        fpga.h
 * @author      Weyne
 * @version     V01
 * @date        2016.09.04
 * @brief       fpga控制的配置
 * @note
 * @attention   COYPRIGHT WEYNE
 */

#ifndef _FPGA_H_
#define _FPGA_H_
#include "stm32f1xx_hal.h"

#define FSMC_LCD_CMD     ((uint32_t)0x60000000)                   //A16
#define FSMC_LCD_DATA    ((uint32_t)0x60010000)
#define FPGA_WRITE_CMD(x)     *(__IO uint8_t *)FSMC_LCD_CMD = x;
#define FPGA_WRITE_DATA(x)    *(__IO uint8_t *)FSMC_LCD_DATA = x;
#define FPGA_READ_DATA()      *(__IO uint8_t *)FSMC_LCD_DATA

void LcdDrvWriteData(uint8_t para);
void LcdDrvSetTiming(void);
void LcdDrvSetPattern(void);
void LcdDrvShowPattern(uint8_t frame);
void LcdDrvOpenRGB(void);

#endif

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
