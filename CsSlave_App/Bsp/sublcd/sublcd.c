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
#include "ack.h"

#define SUB_CS       PDout(12)
#define SUB_RESET    PDout(10)

#define FSMC_CMD     ((uint32_t)0x60000000)                   //A18
#define FSMC_DATA    ((uint32_t)0x60040000)

#define LCD_XSIZE    240
#define LCD_YSIZE    320

/* 当CS使能以后，可以使用下面的简化操作*/
#define WRITE_CMD_FAST(x)     *(__IO uint8_t *)FSMC_CMD  = (x)
#define WRITE_DATA_FAST(x)    *(__IO uint8_t *)FSMC_DATA = (x)

#define DELAY(x)    \
                    \
  do                \
  {                 \
    uint16_t i = x; \
    while (i--) {   \
      ; }           \
                    \
  }                 \
                    \
  while (0)

uint16_t SubLCDFontColor = 0xffff;
uint16_t SubLCDBackColor = 0x00;
extern const unsigned char asc2_3216[95][64];

void WriteCmd(uint8_t cmd)
{
  SUB_CS = 0;
  DELAY(10);
  *(__IO uint8_t *)FSMC_CMD = cmd;
  DELAY(10);
  SUB_CS = 1;
}


void WriteData(uint8_t data)
{
  SUB_CS = 0;
  DELAY(10);
  *(__IO uint8_t *)FSMC_DATA = data;
  DELAY(10);
  SUB_CS = 1;
}


static void ReadID(void)
{
  uint8_t read[4];

  SUB_CS = 0;
  DELAY(10);
  *(__IO uint8_t *)FSMC_CMD = 0x04;
  for (uint8_t i = 0; i < 4; i++)
  {
    read[i] = *(__IO uint8_t *)FSMC_DATA;
  }
  DELAY(10);
  SUB_CS = 1;
  UserSendArray("id:", read, 4);
}


static void SetBeginAddress(uint16_t x, uint16_t y)
{
  WriteCmd(0x2A);
  WriteData(x >> 8);
  WriteData(x);
  WriteData(LCD_XSIZE >> 8);
  WriteData(LCD_XSIZE);

  WriteCmd(0x2B);
  WriteData(y >> 8);
  WriteData(y);
  WriteData(LCD_YSIZE >> 8);
  WriteData((uint8_t)LCD_YSIZE);

  WriteCmd(0x2c);
}


void SubLCD_Init(void)
{
  PDout(7) = 1;

  SUB_RESET = 1;
  HAL_Delay(10); //50
  SUB_RESET = 0;
  HAL_Delay(50); //50
  SUB_RESET = 1;
  HAL_Delay(50); //120

  ReadID();
  WriteCmd(0x11);
  HAL_Delay(100);                  //HAL_Delay 120ms
  //--------------------------------ST7789S Frame rate setting---------
  WriteCmd(0xb2);
  WriteData(0x0c);
  WriteData(0x0c);
  WriteData(0x00);
  WriteData(0x33);
  WriteData(0x33);
  WriteCmd(0xb7);
  WriteData(0x35);
  //---------------------------------ST7789S Power setting-------------
  WriteCmd(0xbb);
  WriteData(0x2b);
  WriteCmd(0xc0);
  WriteData(0x2c);
  WriteCmd(0xc2);
  WriteData(0x01);
  WriteCmd(0xc3);
  WriteData(0x17);
  WriteCmd(0xc4);
  WriteData(0x20);
  WriteCmd(0xc6);
  WriteData(0x0f);
  WriteCmd(0xca);
  WriteData(0x0f);
  WriteCmd(0xc8);
  WriteData(0x08);
  WriteCmd(0x55);
  WriteData(0x90);
  WriteCmd(0xd0);
  WriteData(0xa4);
  WriteData(0xa1);
  //--------------------------------ST7789S gamma setting--------------
  WriteCmd(0xe0);
  WriteData(0xf0);
  WriteData(0x00);
  WriteData(0x0a);
  WriteData(0x10);
  WriteData(0x12);
  WriteData(0x1b);
  WriteData(0x39);
  WriteData(0x44);
  WriteData(0x47);
  WriteData(0x28);
  WriteData(0x12);
  WriteData(0x10);
  WriteData(0x16);
  WriteData(0x1b);
  WriteCmd(0xe1);
  WriteData(0xf0);
  WriteData(0x00);
  WriteData(0x0a);
  WriteData(0x10);
  WriteData(0x11);
  WriteData(0x1a);
  WriteData(0x3b);
  WriteData(0x34);
  WriteData(0x4e);
  WriteData(0x3a);
  WriteData(0x17);
  WriteData(0x16);
  WriteData(0x21);
  WriteData(0x22);

  WriteCmd(0x36);
  WriteData(0xC0);
  WriteCmd(0x3A);
  WriteData(0x05);

  WriteCmd(0x2A);
  WriteData(0);
  WriteData(0);
  WriteData(LCD_XSIZE >> 8);
  WriteData(LCD_XSIZE);

  WriteCmd(0x2b);
  WriteData(0);
  WriteData(0);
  WriteData(LCD_YSIZE >> 8);
  WriteData(LCD_YSIZE & 0xff);

  WriteCmd(0x29);    //Display on
  //HAL_Delay(20);
  WriteCmd(0x2C);    //write GRAM
  //HAL_Delay(10);

  SubLCD_WriteFull(0);
  SubLCDBackColor = 33808;
  SubLCD_Fill(0,32);
  SubLCD_Fill(LCD_YSIZE - 32,LCD_YSIZE);  
  //SubLCDBackColor = 0;  
  SubLCD_ShowString(60,0,"CoolSaven");
  SubLCD_ShowString(0,LCD_YSIZE - 32,">>>>>>>>>  90%");  
}


static void __INLINE SubLCD_DrawPoint(uint16_t color)
{
  WriteData(color >> 8);
  WriteData((uint8_t)color);
}


void SubLCD_ShowChar(uint16_t x, uint16_t y, uint8_t num)
{
  uint8_t temp = 0x01;
  uint8_t pos, t;

  num = num - ' ';

  for (pos = 0; pos < 64; pos += 2)
  {
    temp = asc2_3216[num][pos];
    SetBeginAddress(x, y);
    for (t = 0; t < 8; t++)
    {
      if (temp & 0x01)
      {
        SubLCD_DrawPoint(SubLCDFontColor);
      }
      else
      {
        SubLCD_DrawPoint(SubLCDBackColor);
      }
      temp >>= 1;
    }
    temp = asc2_3216[num][pos + 1];
    for (t = 8; t < 16; t++)
    {
      if (temp & 0x01)
      {
        SubLCD_DrawPoint(SubLCDFontColor);
      }
      else
      {
        SubLCD_DrawPoint(SubLCDBackColor);
      }
      temp >>= 1;
    }
    y++;
  }
}


void SubLCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p)
{
  while (*p != '\0')
  {
    SubLCD_ShowChar(x, y, *p);
    x += 16;
    p++;
  }
}


void SubLCD_Fill(uint16_t ybegin, uint16_t yend)
{
  uint16_t i;
  uint16_t j;

  SetBeginAddress(0, ybegin);
  SUB_CS = 0;
  DELAY(10);
  for (i = ybegin; i < yend; i++)
  {
    for (j = 0; j < LCD_XSIZE; j++)
    {
      WRITE_DATA_FAST(SubLCDBackColor >> 8);
      WRITE_DATA_FAST((uint8_t)SubLCDBackColor);
    }
  }
  DELAY(10);
  SUB_CS = 1;
}


void SubLCD_WriteFull(uint16_t color)
{
  SubLCD_Fill(0, LCD_YSIZE);
}


void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  uint16_t t;
  int xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int incx, incy, uRow, uCol;

  delta_x = x2 - x1; //计算坐标增量
  delta_y = y2 - y1;
  uRow    = x1;
  uCol    = y1;
  if (delta_x > 0)
  {
    incx = 1; //设置单步方向
  }
  else if (delta_x == 0)
  {
    incx = 0; //垂直线
  }
  else
  {
    incx    = -1;
    delta_x = -delta_x;
  }
  if (delta_y > 0)
  {
    incy = 1;
  }
  else if (delta_y == 0)
  {
    incy = 0; //水平线
  }
  else
  {
    incy    = -1;
    delta_y = -delta_y;
  }
  if (delta_x > delta_y)
  {
    distance = delta_x; //选取基本增量坐标轴
  }
  else
  {
    distance = delta_y;
  }
  for (t = 0; t <= distance + 1; t++)     //画线输出
  {
    SetBeginAddress(uRow, uCol);
    SubLCD_DrawPoint(SubLCDFontColor); //画点
    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance)
    {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance)
    {
      yerr -= distance;
      uCol += incy;
    }
  }
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
