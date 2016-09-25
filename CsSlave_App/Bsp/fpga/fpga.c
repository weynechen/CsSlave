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
	data = data -1;
	FPGA_WRITE_DATA(data>>8);
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

void LcdDrvShowPattern(uint8_t frame)
{
	FPGA_WRITE_CMD(0x0B);
	FPGA_WRITE_DATA(0);
	FPGA_WRITE_DATA(0);
	FPGA_WRITE_DATA(0);	
	FPGA_WRITE_DATA(frame);	
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
