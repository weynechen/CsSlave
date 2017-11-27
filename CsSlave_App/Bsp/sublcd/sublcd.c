/**
 * @file         sublcd.c
 * @author       Weyne Chen
 * @version      V01
 * @date         2017.11.27
 * @brief
 * @note
 * @attention    COPYRIGHT WEYNE
 **/

#include "sublcd.h"
#include "sys.h"


#define CS           PDout(12)
#define RESET        PDout(10)

#define FSMC_CMD     ((uint32_t)0x60000000)                   //A16
#define FSMC_DATA    ((uint32_t)0x60010000)

#define LCD_XSIZE    240
#define LCD_YSIZE    320

#define WRITE_CMD(x)                 \
  do {                               \
    CS = 0;                          \
    *(__IO uint8_t *)FSMC_CMD = (x); \
    CS = 1;                          \
  } while (0)

#define WRITE_DATA(x)                 \
  do {                                \
    CS = 0;                           \
    *(__IO uint8_t *)FSMC_DATA = (x); \
    CS = 1;                           \
  } while (0)

/* 当CS使能以后，可以使用下面的简化操作*/
#define WRITE_CMD_FAST(x)     *(__IO uint8_t *)FSMC_CMD  = (x)
#define WRITE_DATA_FAST(x)    *(__IO uint8_t *)FSMC_DATA = (x)


#define READ_DATA()             \
  do {                          \
    CS = 0;                     \
    *(__IO uint8_t *)FSMC_DATA; \
    CS = 1;                     \
  } while (0)


static void SetBeginAddress(uint16_t x, uint16_t y)
{
  WRITE_CMD(0x2A);
  WRITE_DATA(x >> 8);
  WRITE_DATA(x);
  WRITE_DATA(LCD_XSIZE >> 8);
  WRITE_DATA(LCD_XSIZE);

  WRITE_CMD(0x2B);
  WRITE_DATA(y >> 8);
  WRITE_DATA(y);
  WRITE_DATA(LCD_YSIZE >> 8);
  WRITE_DATA((uint8_t)LCD_YSIZE);

  WRITE_CMD(0x2c);
}


void SubLCD_Init(void)
{
  RESET = 1;
  HAL_Delay(10);  //50
  RESET = 0;
  HAL_Delay(50);  //50
  RESET = 1;
  HAL_Delay(120);  //120

  WRITE_CMD(0x11);
  HAL_Delay(100);                  //HAL_Delay 120ms
  //--------------------------------ST7789S Frame rate setting---------
  WRITE_CMD(0xb2);
  WRITE_DATA(0x0c);
  WRITE_DATA(0x0c);
  WRITE_DATA(0x00);
  WRITE_DATA(0x33);
  WRITE_DATA(0x33);
  WRITE_CMD(0xb7);
  WRITE_DATA(0x35);
  //---------------------------------ST7789S Power setting-------------
  WRITE_CMD(0xbb);
  WRITE_DATA(0x2b);
  WRITE_CMD(0xc0);
  WRITE_DATA(0x2c);
  WRITE_CMD(0xc2);
  WRITE_DATA(0x01);
  WRITE_CMD(0xc3);
  WRITE_DATA(0x17);
  WRITE_CMD(0xc4);
  WRITE_DATA(0x20);
  WRITE_CMD(0xc6);
  WRITE_DATA(0x0f);
  WRITE_CMD(0xca);
  WRITE_DATA(0x0f);
  WRITE_CMD(0xc8);
  WRITE_DATA(0x08);
  WRITE_CMD(0x55);
  WRITE_DATA(0x90);
  WRITE_CMD(0xd0);
  WRITE_DATA(0xa4);
  WRITE_DATA(0xa1);
  //--------------------------------ST7789S gamma setting--------------
  WRITE_CMD(0xe0);
  WRITE_DATA(0xf0);
  WRITE_DATA(0x00);
  WRITE_DATA(0x0a);
  WRITE_DATA(0x10);
  WRITE_DATA(0x12);
  WRITE_DATA(0x1b);
  WRITE_DATA(0x39);
  WRITE_DATA(0x44);
  WRITE_DATA(0x47);
  WRITE_DATA(0x28);
  WRITE_DATA(0x12);
  WRITE_DATA(0x10);
  WRITE_DATA(0x16);
  WRITE_DATA(0x1b);
  WRITE_CMD(0xe1);
  WRITE_DATA(0xf0);
  WRITE_DATA(0x00);
  WRITE_DATA(0x0a);
  WRITE_DATA(0x10);
  WRITE_DATA(0x11);
  WRITE_DATA(0x1a);
  WRITE_DATA(0x3b);
  WRITE_DATA(0x34);
  WRITE_DATA(0x4e);
  WRITE_DATA(0x3a);
  WRITE_DATA(0x17);
  WRITE_DATA(0x16);
  WRITE_DATA(0x21);
  WRITE_DATA(0x22);

  WRITE_CMD(0x36);
  WRITE_DATA(0x00);
  WRITE_CMD(0x3A);
  WRITE_DATA(0x05);

  WRITE_CMD(0x2A);
  WRITE_DATA(0);
  WRITE_DATA(0);
  WRITE_DATA(LCD_XSIZE >> 8);
  WRITE_DATA(LCD_XSIZE);

  WRITE_CMD(0x2b);
  WRITE_DATA(0);
  WRITE_DATA(0);
  WRITE_DATA(LCD_YSIZE >> 8);
  WRITE_DATA(LCD_YSIZE & 0xff);

  WRITE_CMD(0x29);    //Display on
  //HAL_Delay(20);
  WRITE_CMD(0x2C);    //write GRAM
  //HAL_Delay(10);
}


void SubLCD_WriteFull(uint16_t color)
{
  uint16_t i = 0;
  uint16_t j = 0;

  //SetBeginAddress(0, 0);
  CS = 0;
  for (i = 0; i < LCD_XSIZE; i++)
  {
    for (j = 0; j < LCD_YSIZE; j++)
    {
      WRITE_DATA_FAST(color >> 8);
      WRITE_DATA_FAST((uint8_t)color);
    }
  }
  CS = 1;
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
