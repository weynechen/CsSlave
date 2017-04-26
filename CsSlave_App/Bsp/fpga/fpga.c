/**
 * @file        fpga.c
 * @author      Weyne
 * @version     V01
 * @date        2016.09.04
 * @brief       fpga控制
 * @note				通过FSMC与FPGA通讯，控制lcd时序，传输数据等
 * @attention   COYPRIGHT WEYNE
 */
#include "fpga.h"
#include "sysconfig.h"


void LcdDrvWriteData(uint8_t para)
{
  FPGA_WRITE_DATA(para);
}


static void FPGAWrite16BitData(uint16_t data)
{
	if(data > 0)
  data = data - 1;
  FPGA_WRITE_DATA(data >> 8);
  FPGA_WRITE_DATA(data);
}


void LcdDrvSetTiming(void)
{
  FPGA_WRITE_CMD(0xB0);
  FPGAWrite16BitData(LCDTiming.LCDH);
  FPGAWrite16BitData(LCDTiming.LCDV);

  FPGA_WRITE_CMD(0xB4);
  FPGAWrite16BitData(LCDTiming.HSPW);
  FPGAWrite16BitData(LCDTiming.HFPD);
  FPGAWrite16BitData(LCDTiming.HBPD);

  FPGA_WRITE_CMD(0xB6);
  FPGAWrite16BitData(LCDTiming.VSPW);
  FPGAWrite16BitData(LCDTiming.VFPD);
  FPGAWrite16BitData(LCDTiming.VBPD);

  FPGA_WRITE_CMD(0x28);
}


void LcdDrvOpenRGB(void)
{
  FPGA_WRITE_CMD(0x29);
}


void LcdDrvCloseRGB(void)
{
  FPGA_WRITE_CMD(0x28);
}


void LcdDrvSetPattern(void)
{
  FPGA_WRITE_CMD(0x3C);
}

void LcdDrvSetXY(uint16_t x , uint16_t y)
{
  uint32_t address = 0;
  uint8_t i = 0;
  uint8_t *p = (uint8_t *)&address;
	
	address = LCDTiming.LCDH;
	address *= y;
	address += x;
	
  FPGA_WRITE_CMD(0x2A);
	
  for ( ; i < sizeof(address); i++)
  {
    FPGA_WRITE_DATA(*(p + 3 - i));
  }

	FPGA_WRITE_CMD(0x2C);	
	
}

void LcdDrvSetCharIndex(uint8_t frame)
{
  uint32_t address = 0;
  uint8_t i = 0;
  uint8_t *p = (uint8_t *)&address;

  address = LCDTiming.LCDH;
  address *= LCDTiming.LCDV;
  address *= frame;


  FPGA_WRITE_CMD(0x3A);

  for ( ; i < sizeof(address); i++)
  {
    FPGA_WRITE_DATA(*(p + 3 - i));
  }	
	
}

void LcdDrvShowPattern(uint32_t data)
{
  uint32_t address = 0;
  uint8_t i = 0;
  uint8_t *p = (uint8_t *)&address;

	
	if((data & 0xff000000) == 0xff000000)
	{
			uint8_t r,g,b;
			r = (data&0xffffff)>>16;
			g = (data&0xffffff)>>8;
			b = (data&0xffffff)>>0;
			FPGA_WRITE_CMD(0x1B);
      FPGA_WRITE_DATA(r);
      FPGA_WRITE_DATA(g);
      FPGA_WRITE_DATA(b);
	}
	else
	{
		  FPGA_WRITE_CMD(0x1B);
      FPGA_WRITE_DATA(0xAA);
      FPGA_WRITE_DATA(0xAA);
      FPGA_WRITE_DATA(0xAA);
		
			HAL_Delay(1);
		
			address = LCDTiming.LCDH;
			address *= LCDTiming.LCDV;
			address *= data;


			FPGA_WRITE_CMD(0x0B);

			for ( ; i < sizeof(address); i++)
			{
				FPGA_WRITE_DATA(*(p + 3 - i));
			}
	}
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
