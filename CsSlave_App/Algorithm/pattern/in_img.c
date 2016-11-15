#include "in_img.h"
#include "ssd2828.h"
#include "stdio.h"
#include "sysconfig.h"
#include "fpga.h"

void Img_CT(void)
{
  uint32_t h;
  uint32_t v1_2;
  uint32_t v1_4, h1_4;
  uint32_t mod;

  mod = LCDTiming.LCDV % 4;
  for (v1_4 = 0; v1_4 < LCDTiming.LCDV / 4; v1_4++)
  {
    for (h = 0; h < LCDTiming.LCDH; h++)
    {
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
    }
  }

  for (v1_2 = 0; v1_2 < LCDTiming.LCDV / 4; v1_2++)
  {
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
    }

    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
    }
  }

  for (v1_2 = 0; v1_2 < LCDTiming.LCDV / 4; v1_2++)
  {
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
    }

    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
    }
  }

  for (v1_4 = 0; v1_4 < LCDTiming.LCDV / 4 + mod; v1_4++)
  {
    for (h = 0; h < LCDTiming.LCDH; h++)
    {
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
      LcdDrvWriteData(0x80);
    }
  }
}


void Flicker(void)
{
  uint16_t i, j;
  uint16_t half_lcd_h = LCDTiming.LCDH / 2;

  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < half_lcd_h; i++)
    {
      LcdDrvWriteData(0x7f);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x7f);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x7f);
      LcdDrvWriteData(0x00);
    }
  }
}


void Img_Full(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t i, j;

  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      LcdDrvWriteData(r);
      LcdDrvWriteData(g);
      LcdDrvWriteData(b);
    }
  }
}


void Img_Chcker58()
{
  uint16_t i;
  uint16_t j;
  uint16_t y;
  uint16_t mod = LCDTiming.LCDV % 8;

  for (j = 0; j < 4; j++)
  {
    for (y = 0; y < LCDTiming.LCDV / 8; y++)
    {
      for (i = 0; i < LCDTiming.LCDH / 5; i++)
      {
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
      {
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
      {
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
      {
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH; i++)
      {
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
      }
    }

    for (y = 0; y < LCDTiming.LCDV / 8; y++)
    {
      for (i = 0; i < LCDTiming.LCDH / 5; i++)
      {
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
      {
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
      {
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
      {
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
      }
      for ( ; i < LCDTiming.LCDH; i++)
      {
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xff);
      }
    }
  }

  for (y = 0; y < mod; y++)
  {
    for (i = 0; i < LCDTiming.LCDH / 5; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
    for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
    for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
    for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
    for ( ; i < LCDTiming.LCDH; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
  }
}


void Img_Box(void)
{
  uint32_t x, y;

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      if ((x == 0) || (x == LCDTiming.LCDH - 1) || (y == 0) || (y == LCDTiming.LCDV - 1))
      {
        LcdDrvWriteData(0xff);
        LcdDrvWriteData(0xFF);
        LcdDrvWriteData(0xFF);
      }
      else
      {
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x00);
      }
    }
  }
}


void Img_Gray256_H(void)
{
  uint32_t x, y;
  uint32_t lcd_h;
  uint8_t w;

  lcd_h = LCDTiming.LCDH - 1;

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      w = (x * 255) / (lcd_h);
      LcdDrvWriteData(w);
      LcdDrvWriteData(w);
      LcdDrvWriteData(w);
    }
  }
}


void Img_RED256_H(void)
{
  uint32_t x, y;
  uint32_t lcd_h;
  uint8_t w;

  lcd_h = LCDTiming.LCDH - 1;

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      w = (x * 255) / (lcd_h);
      LcdDrvWriteData(w);
      LcdDrvWriteData(0);
      LcdDrvWriteData(0);
    }
  }
}


void Img_GREEN256_H(void)
{
  uint32_t x, y;
  uint32_t lcd_h;
  uint8_t w;

  lcd_h = LCDTiming.LCDH - 1;

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      w = (x * 255) / (lcd_h);
      LcdDrvWriteData(0);
      LcdDrvWriteData(w);
      LcdDrvWriteData(0);
    }
  }
}


void Img_BLUE256_H(void)
{
  uint32_t x, y;
  uint32_t lcd_h;
  uint8_t w;

  lcd_h = LCDTiming.LCDH - 1;

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      w = (x * 255) / (lcd_h);
      LcdDrvWriteData(0);
      LcdDrvWriteData(0);
      LcdDrvWriteData(w);
    }
  }
}


void Img_Gray256_V(void)
{
  uint32_t x, y;
  uint32_t lcd_y;
  uint8_t w;

  lcd_y = LCDTiming.LCDV - 1;

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    w = (y * 255) / (lcd_y);
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      LcdDrvWriteData(w);
      LcdDrvWriteData(w);
      LcdDrvWriteData(w);
    }
  }
}


void Img_RED256_V(void)
{
  uint32_t x, y;
  uint32_t lcd_y;
  uint8_t w;

  lcd_y = LCDTiming.LCDV - 1;

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    w = (y * 255) / (lcd_y);
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      LcdDrvWriteData(w);
      LcdDrvWriteData(0);
      LcdDrvWriteData(0);
    }
  }
}


void Img_GREEN256_V(void)
{
  uint32_t x, y;
  uint32_t lcd_y;
  uint8_t w;

  lcd_y = LCDTiming.LCDV - 1;

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    w = (y * 255) / (lcd_y);
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      LcdDrvWriteData(0);
      LcdDrvWriteData(w);
      LcdDrvWriteData(0);
    }
  }
}


void Img_BLUE256_V(void)
{
  uint32_t x, y;
  uint32_t lcd_y;
  uint8_t w;

  lcd_y = LCDTiming.LCDV - 1;

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    w = (y * 255) / (lcd_y);
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      LcdDrvWriteData(0);
      LcdDrvWriteData(0);
      LcdDrvWriteData(w);
    }
  }
}


void Img_ColorBar(void)
{
  uint32_t i, j, x;
  uint8_t mod;

  x = LCDTiming.LCDH / 8;
  mod = LCDTiming.LCDH % 8;

  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < x; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
    }
    for (i = 0; i < x; i++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
    }
    for (i = 0; i < x; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
    for (i = 0; i < x; i++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
    }
    for (i = 0; i < x; i++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
    for (i = 0; i < mod; i++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
  }
}


void Img_ColorBarV(void)
{
  uint32_t i, j, y;

  y = LCDTiming.LCDV / 8;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
  }
  y = LCDTiming.LCDV % 8;
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
  }
}


void two_windows(void)
{
  uint32_t i, j;

  for (j = 0; j < 128; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
    }
  }

  for (j = 0; j < 384; j++)
  {
    for (i = 0; i < 108; i++)
    {
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
    }
    for (i = 0; i < 216; i++)
    {
      LcdDrvWriteData(0);
      LcdDrvWriteData(0);
      LcdDrvWriteData(0);
    }
    for (i = 0; i < 396; i++)
    {
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
    }
  }

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
    }
  }

  for (j = 0; j < 384; j++)
  {
    for (i = 0; i < 396; i++)
    {
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
    }
    for (i = 0; i < 216; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
    for (i = 0; i < 108; i++)
    {
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
    }
  }

  for (j = 0; j < 128; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
      LcdDrvWriteData(127);
    }
  }
}


void MAX_Current(void)
{
  uint32_t i, j;

  for (j = 0; j < LCDTiming.LCDV / 2; j++)
  {
    for (i = 0; i < LCDTiming.LCDH / 2; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
    }
    for (i = 0; i < LCDTiming.LCDH / 2; i++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
    }
  }
}


void RGBBar(void)
{
  uint32_t i, j, y, mod;

  y = LCDTiming.LCDV / 3;
  mod = LCDTiming.LCDV % 3;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
    }
  }

  for (i = 0; i < mod; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff);
    }
  }
}


void RGBLevel(void)
{
  uint32_t i, j, y, mod, color = 0;

  y = LCDTiming.LCDV / 3;
  mod = LCDTiming.LCDV % 3;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      LcdDrvWriteData(0xff - color);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff - color);
      LcdDrvWriteData(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff - color);
    }
  }

  for (i = 0; i < mod; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0x00);
      LcdDrvWriteData(0xff - color);
    }
  }
}


void Img_NG(void)
{
  uint32_t y;

  for (y = 0; y < LCDTiming.LCDV / 2; y++)
  {
    LcdDrvWriteData(0xff);
    LcdDrvWriteData(0xFF);
    LcdDrvWriteData(0xFF);
  }
  for ( ; y < LCDTiming.LCDV; y++)
  {
    LcdDrvWriteData(0xff);
    LcdDrvWriteData(0x00);
    LcdDrvWriteData(0x00);
  }
}
