#include "in_img.h"
#include "ssd2828.h"
#include "stdio.h"
#include "sysconfig.h"
#include "fpga.h"
#include "stdbool.h"

void Img_CT(void)
{
  uint32_t h;
  uint32_t v1_2;
  uint32_t v1_4, h1_4;
  uint32_t mod;

  mod = LCDTiming.LCDV % 4;
  FPGA_WRITE_ENABLE(0);
  for (v1_4 = 0; v1_4 < LCDTiming.LCDV / 4; v1_4++)
  {
    for (h = 0; h < LCDTiming.LCDH; h++)
    {
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
    }
  }

  for (v1_2 = 0; v1_2 < LCDTiming.LCDV / 4; v1_2++)
  {
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
    }

    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
    }
  }

  for (v1_2 = 0; v1_2 < LCDTiming.LCDV / 4; v1_2++)
  {
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
    }

    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
    for (h1_4 = 0; h1_4 < LCDTiming.LCDH / 4; h1_4++)
    {
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
    }
  }

  for (v1_4 = 0; v1_4 < LCDTiming.LCDV / 4 + mod; v1_4++)
  {
    for (h = 0; h < LCDTiming.LCDH; h++)
    {
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x80);
    }
  }
  FPGA_WRITE_ENABLE(1);
}


void Flicker(void)
{
  uint16_t i, j;
  uint16_t half_lcd_h = LCDTiming.LCDH / 2;
  FPGA_WRITE_ENABLE(0);  
  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < half_lcd_h; i++)
    {
      FPGA_WRITE_FAST(0x7f);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x7f);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x7f);
      FPGA_WRITE_FAST(0x00);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Flicker2Dot(void)
{
  uint16_t i, j;
  uint16_t half_lcd_h = LCDTiming.LCDH / 2;
  uint16_t half_lcd_v = LCDTiming.LCDV / 2;
  FPGA_WRITE_ENABLE(0);

  for (j = 0; j < half_lcd_v; j++)
  {
    for (i = 0; i < half_lcd_h; i++)
    {
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x00);
    }

    for (i = 0; i < half_lcd_h; i++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x80);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x80);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_Full(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t i, j;
  FPGA_WRITE_ENABLE(0);

  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      FPGA_WRITE_FAST(r);
      FPGA_WRITE_FAST(g);
      FPGA_WRITE_FAST(b);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_Chcker58()
{
  uint16_t i;
  uint16_t j;
  uint16_t y;
  uint16_t mod = LCDTiming.LCDV % 8;
  FPGA_WRITE_ENABLE(0);

  for (j = 0; j < 4; j++)
  {
    for (y = 0; y < LCDTiming.LCDV / 8; y++)
    {
      for (i = 0; i < LCDTiming.LCDH / 5; i++)
      {
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
      {
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
      {
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
      {
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
      }
      for ( ; i < LCDTiming.LCDH; i++)
      {
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
      }
    }

    for (y = 0; y < LCDTiming.LCDV / 8; y++)
    {
      for (i = 0; i < LCDTiming.LCDH / 5; i++)
      {
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
      {
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
      }
      for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
      {
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
      }
      for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
      {
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
      }
      for ( ; i < LCDTiming.LCDH; i++)
      {
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xff);
      }
    }
  }

  for (y = 0; y < mod; y++)
  {
    for (i = 0; i < LCDTiming.LCDH / 5; i++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
    }
    for ( ; i < LCDTiming.LCDH * 2 / 5; i++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
    for ( ; i < LCDTiming.LCDH * 3 / 5; i++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
    }
    for ( ; i < LCDTiming.LCDH * 4 / 5; i++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
    for ( ; i < LCDTiming.LCDH; i++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_Box(void)
{
  uint32_t x, y;
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      if ((x == 0) || (x == LCDTiming.LCDH - 1) || (y == 0) || (y == LCDTiming.LCDV - 1))
      {
        FPGA_WRITE_FAST(0xff);
        FPGA_WRITE_FAST(0xFF);
        FPGA_WRITE_FAST(0xFF);
      }
      else
      {
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
        FPGA_WRITE_FAST(0x00);
      }
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_Gray256_H(uint16_t level)
{
  uint32_t x, y, i;
  uint16_t div          = LCDTiming.LCDV / level;
  uint16_t mod          = LCDTiming.LCDV % level;
  uint16_t color_factor = 256 / level;
  uint16_t color        = 0;
  uint16_t n            = 0;
  uint16_t base         = (mod > level - mod) ? mod : (level - mod);
  uint16_t less         = (mod > level - mod) ? (level - mod) : mod;
  uint16_t mod_div      = (base == mod) ? (div + 1) : div;
  uint16_t remain_div   = (base == mod) ? div : (div + 1);
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < base; y++)
  {
    for (i = 0; i < mod_div; i++)
    {
      for (x = 0; x < LCDTiming.LCDH; x++)
      {
        FPGA_WRITE_FAST(color);
        FPGA_WRITE_FAST(color);
        FPGA_WRITE_FAST(color);
      }
    }
    n++;
    color = n * color_factor;

    if (less != 0)
    {
      for (i = 0; i < remain_div; i++)
      {
        for (x = 0; x < LCDTiming.LCDH; x++)
        {
          FPGA_WRITE_FAST(color);
          FPGA_WRITE_FAST(color);
          FPGA_WRITE_FAST(color);
        }
      }
      n++;
      color = n * color_factor;
      less--;
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_RED256_H(void)
{
  uint32_t x, y;
  uint32_t lcd_h;
  uint8_t w;

  lcd_h = LCDTiming.LCDH - 1;
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      w = (x * 255) / (lcd_h);
      FPGA_WRITE_FAST(w);
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(0);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_GREEN256_H(void)
{
  uint32_t x, y;
  uint32_t lcd_h;
  uint8_t w;

  lcd_h = LCDTiming.LCDH - 1;
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      w = (x * 255) / (lcd_h);
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(w);
      FPGA_WRITE_FAST(0);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_BLUE256_H(void)
{
  uint32_t x, y;
  uint32_t lcd_h;
  uint8_t w;

  lcd_h = LCDTiming.LCDH - 1;
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      w = (x * 255) / (lcd_h);
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(w);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_Gray256_V(uint16_t level)
{
  uint32_t x, y, i;
  uint16_t div          = LCDTiming.LCDH / level;
  uint16_t mod          = LCDTiming.LCDH % level;
  uint16_t color_factor = 256 / level;
  uint16_t color        = 0;
  uint16_t n            = 0;
  uint16_t base         = (mod > level - mod) ? mod : (level - mod);
  uint16_t less         = (mod > level - mod) ? (level - mod) : mod;
  uint16_t less_tmp;
  uint16_t mod_div    = (base == mod) ? (div + 1) : div;
  uint16_t remain_div = (base == mod) ? div : (div + 1);
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    less_tmp = less;
    n        = 0;
    color    = 0;
    for (x = 0; x < base; x++)
    {
      for (i = 0; i < mod_div; i++)
      {
        FPGA_WRITE_FAST(color);
        FPGA_WRITE_FAST(color);
        FPGA_WRITE_FAST(color);
      }
      n++;
      color = n * color_factor;

      if (less_tmp != 0)
      {
        for (i = 0; i < remain_div; i++)
        {
          FPGA_WRITE_FAST(color);
          FPGA_WRITE_FAST(color);
          FPGA_WRITE_FAST(color);
        }
        n++;
        color = n * color_factor;
        less_tmp--;
      }
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_RED256_V(void)
{
  uint32_t x, y;
  uint32_t lcd_y;
  uint8_t w;

  lcd_y = LCDTiming.LCDV - 1;
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    w = (y * 255) / (lcd_y);
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      FPGA_WRITE_FAST(w);
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(0);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_GREEN256_V(void)
{
  uint32_t x, y;
  uint32_t lcd_y;
  uint8_t w;

  lcd_y = LCDTiming.LCDV - 1;
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    w = (y * 255) / (lcd_y);
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(w);
      FPGA_WRITE_FAST(0);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_BLUE256_V(void)
{
  uint32_t x, y;
  uint32_t lcd_y;
  uint8_t w;

  lcd_y = LCDTiming.LCDV - 1;
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < LCDTiming.LCDV; y++)
  {
    w = (y * 255) / (lcd_y);
    for (x = 0; x < LCDTiming.LCDH; x++)
    {
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(w);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_ColorBar(void)
{
  uint32_t i, j, x;
  uint8_t mod;

  x   = LCDTiming.LCDH / 8;
  mod = LCDTiming.LCDH % 8;
  FPGA_WRITE_ENABLE(0);

  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    for (i = 0; i < x; i++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
    }
    for (i = 0; i < x; i++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
    }
    for (i = 0; i < x; i++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
    }
    for (i = 0; i < x; i++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
    }
    for (i = 0; i < x; i++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
    }
    for (i = 0; i < x; i++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
    for (i = 0; i < x; i++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
    }
    for (i = 0; i < x; i++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
    for (i = 0; i < mod; i++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_ColorBarV(void)
{
  uint32_t i, j, y;

  y = LCDTiming.LCDV / 8;
  FPGA_WRITE_ENABLE(0);

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
  }
  y = LCDTiming.LCDV % 8;
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void two_windows(void)
{
  uint32_t i, j;
  FPGA_WRITE_ENABLE(0);

  for (j = 0; j < 128; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
    }
  }

  for (j = 0; j < 384; j++)
  {
    for (i = 0; i < 108; i++)
    {
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
    }
    for (i = 0; i < 216; i++)
    {
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(0);
      FPGA_WRITE_FAST(0);
    }
    for (i = 0; i < 396; i++)
    {
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
    }
  }

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
    }
  }

  for (j = 0; j < 384; j++)
  {
    for (i = 0; i < 396; i++)
    {
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
    }
    for (i = 0; i < 216; i++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0xff);
    }
    for (i = 0; i < 108; i++)
    {
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
    }
  }

  for (j = 0; j < 128; j++)
  {
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
      FPGA_WRITE_FAST(127);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void MAX_Current(void)
{
  uint32_t i, j;
  FPGA_WRITE_ENABLE(0);

  for (j = 0; j < LCDTiming.LCDV / 2; j++)
  {
    for (i = 0; i < LCDTiming.LCDH / 2; i++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
    }
    for (i = 0; i < LCDTiming.LCDH / 2; i++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void RGBBar(void)
{
  uint32_t i, j, y, mod;

  y   = LCDTiming.LCDV / 3;
  mod = LCDTiming.LCDV % 3;

  FPGA_WRITE_ENABLE(0);

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
      FPGA_WRITE_FAST(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
    }
  }

  for (i = 0; i < mod; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void RGBLevel(void)
{
  uint32_t i, j, y, mod, color = 0;

  y   = LCDTiming.LCDV / 3;
  mod = LCDTiming.LCDV % 3;
  FPGA_WRITE_ENABLE(0);

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      FPGA_WRITE_FAST(0xff - color);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff - color);
      FPGA_WRITE_FAST(0x00);
    }
  }
  for (i = 0; i < y; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff - color);
    }
  }

  for (i = 0; i < mod; i++)
  {
    for (j = 0; j < LCDTiming.LCDH; j++)
    {
      color = j * 255 / LCDTiming.LCDH;
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0x00);
      FPGA_WRITE_FAST(0xff - color);
    }
  }
  FPGA_WRITE_ENABLE(1);
  
}


void Img_NG(void)
{
  uint32_t y;
  FPGA_WRITE_ENABLE(0);

  for (y = 0; y < LCDTiming.LCDV / 2; y++)
  {
    FPGA_WRITE_FAST(0xff);
    FPGA_WRITE_FAST(0xFF);
    FPGA_WRITE_FAST(0xFF);
  }
  for ( ; y < LCDTiming.LCDV; y++)
  {
    FPGA_WRITE_FAST(0xff);
    FPGA_WRITE_FAST(0x00);
    FPGA_WRITE_FAST(0x00);
  }
  FPGA_WRITE_ENABLE(1);
  
}
