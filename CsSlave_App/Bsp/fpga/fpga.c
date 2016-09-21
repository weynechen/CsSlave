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

void LcdDrvSetTiming(void)
{
	FPGA_WRITE_CMD(0xB0);
	FPGA_WRITE_DATA(LCDTiming.LCDH>>8);
	FPGA_WRITE_DATA(LCDTiming.LCDH);
	FPGA_WRITE_DATA(LCDTiming.LCDV>>8);
	FPGA_WRITE_DATA(LCDTiming.LCDV);
  
	FPGA_WRITE_CMD(0xB4);
	FPGA_WRITE_DATA(LCDTiming.HSPW>>8);
	FPGA_WRITE_DATA(LCDTiming.HSPW);
	FPGA_WRITE_DATA(LCDTiming.HFPD>>8);
	FPGA_WRITE_DATA(LCDTiming.HFPD);
	FPGA_WRITE_DATA(LCDTiming.HBPD>>8);		
	FPGA_WRITE_DATA(LCDTiming.HBPD);


	FPGA_WRITE_CMD(0xB6);
	FPGA_WRITE_DATA(LCDTiming.VSPW>>8);
	FPGA_WRITE_DATA(LCDTiming.VSPW);
	FPGA_WRITE_DATA(LCDTiming.VFPD>>8);
	FPGA_WRITE_DATA(LCDTiming.VFPD);
	FPGA_WRITE_DATA(LCDTiming.VBPD>>8);	
	FPGA_WRITE_DATA(LCDTiming.VBPD);
	
	FPGA_WRITE_CMD(0x29);	
}

//void LcdDrvSetTiming(void)
//{
//	FPGA_WRITE_CMD(0xB0);
//	FPGA_WRITE_DATA(LCDTiming.LCDH);
//	FPGA_WRITE_DATA(LCDTiming.LCDH>>8);
//	FPGA_WRITE_DATA(LCDTiming.LCDV);
//	FPGA_WRITE_DATA(LCDTiming.LCDV>>8);
//  
//	FPGA_WRITE_CMD(0xB4);
//	FPGA_WRITE_DATA(LCDTiming.HSPW);
//	FPGA_WRITE_DATA(LCDTiming.HSPW>>8);
//	FPGA_WRITE_DATA(LCDTiming.HFPD);
//	FPGA_WRITE_DATA(LCDTiming.HFPD>>8);
//	FPGA_WRITE_DATA(LCDTiming.HBPD);
//	FPGA_WRITE_DATA(LCDTiming.HBPD>>8);		


//	FPGA_WRITE_CMD(0xB6);
//	FPGA_WRITE_DATA(LCDTiming.VSPW);
//	FPGA_WRITE_DATA(LCDTiming.VSPW>>8);
//	FPGA_WRITE_DATA(LCDTiming.VFPD);
//	FPGA_WRITE_DATA(LCDTiming.VFPD>>8);
//	FPGA_WRITE_DATA(LCDTiming.VBPD);
//	FPGA_WRITE_DATA(LCDTiming.VBPD>>8);	
//	
//	FPGA_WRITE_CMD(0x29);	
//}

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
