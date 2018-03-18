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
#include "sys.h"

#define FSMC_NE1 PDout(7)
#define FSMC_LCD_CMD     ((uint32_t)0x60000000)                   //A16
#define FSMC_LCD_DATA    ((uint32_t)0x60010000)
#define FPGA_WRITE_CMD(x)\
do {\
	FSMC_NE1 = 0;\
*(__IO uint8_t *)FSMC_LCD_CMD = x;\
		FSMC_NE1 = 1;\
}while(0)

#define FPGA_WRITE_DATA(x)\
do {\
	FSMC_NE1 = 0;\
*(__IO uint8_t *)FSMC_LCD_DATA = x;\
		FSMC_NE1 = 1;\
}while(0)

/* 当NE使能以后，可以使用下面的简化操作*/
//#define FPGA_WRITE_CMD(x) *(__IO uint8_t *)FSMC_LCD_CMD = x
#define FPGA_WRITE_FAST(x) *(__IO uint8_t *)FSMC_LCD_DATA = x
#define FPGA_WRITE_ENABLE(x) FSMC_NE1 = x

#define FPGA_READ_DATA()\
do {\
	FSMC_NE1 = 0;\
*(__IO uint8_t *)FSMC_LCD_DATA;\
	FSMC_NE1 = 1;\
}while(0)

void LcdDrvWriteData(uint8_t para);
void LcdDrvSetTiming(void);
void LcdDrvSetPattern(void);
void LcdDrvShowPattern(uint32_t data);
void LcdDrvOpenRGB(void);
void LcdDrvSetXY(uint16_t x, uint16_t y);
void LcdDrvSetCharIndex(uint8_t frame);
void LcdDrvEnterLVDS(uint8_t mode);
#endif

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
