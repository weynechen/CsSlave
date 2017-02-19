/**
 * @file        rgbif.c
 * @author      Weyne
 * @version     V01
 * @date        2016.11.20
 * @brief       rgb panel interface
 * @note				
 * @attention   COYPRIGHT WEYNE
 */
#include "rgbif.h"
#include "sysconfig.h"

static void Delay_10us(volatile uint8_t t)
{
  while (t--)
  {
	  uint8_t i = 15;
    while (i--)
    {
    }
  }
}

static void Delay(volatile uint8_t t)
{
  while (t--)
  {
  }
}

static void RGB_SPI16BitFallingEdgeWrite(SPIRWTypeDef rw , SPIDataCommandTypeDef data_command , SPIByteTypeDef high_low_byte , uint8_t data)
{
	uint16_t tmp = 0;
	uint8_t i;
	tmp = (((uint8_t)rw &0x01) <<15);
	tmp |= (((uint8_t)data_command & 0x01)<<14);
	tmp |= (((uint8_t)high_low_byte & 0x01)<<13);
	tmp |= data;
	
	SPI_CS = 0;
	SPI_SCK = 1;
	Delay_10us(5);

	for (i = 0; i < 16; i++)
  {
    if ((tmp & 0x8000) == 0x8000)
    {
      SPI_SDA = 1;
    }
    else
    {
      SPI_SDA = 0;
    }

    Delay_10us(2);
    SPI_SCK = 0;
    Delay_10us(2);
    SPI_SCK = 1;
    Delay_10us(2);
    tmp = tmp << 1;		
	}
	Delay_10us(1);
	SPI_CS = 1;
}

static void RGB_SPI16BitRisingEdgeWrite(SPIRWTypeDef rw , SPIDataCommandTypeDef data_command , SPIByteTypeDef high_low_byte , uint8_t data)
{
	uint16_t tmp = 0;
	uint8_t i;
	tmp = (((uint8_t)rw &0x01) <<15);
	tmp |= (((uint8_t)data_command & 0x01)<<14);
	tmp |= (((uint8_t)high_low_byte & 0x01)<<13);
	tmp |= data;
	
	SPI_CS = 0;
	SPI_SCK = 0;
	Delay_10us(5);

	for (i = 0; i < 16; i++)
  {
    if ((tmp & 0x8000) == 0x8000)
    {
      SPI_SDA = 1;
    }
    else
    {
      SPI_SDA = 0;
    }

    Delay_10us(2);
    SPI_SCK = 1;
    Delay_10us(2);
    SPI_SCK = 0;
    Delay_10us(2);
    tmp = tmp << 1;		
	}
	Delay_10us(1);
	SPI_CS = 1;
}

static uint8_t RGB_SPI16BitRisingEdgeRead(void)
{
  uint16_t i;
  uint8_t tmp = 0xC0;
	SPI_CS = 0;
	SPI_SCK = 0;	
  Delay_10us(1);
	
	for (i = 0; i < 8; i++)
  {
    if ((tmp & 0x80) == 0x80)
    {
      SPI_SDA = 1;
    }
    else
    {
      SPI_SDA = 0;
    }

    Delay_10us(2);
    SPI_SCK = 0;
    Delay_10us(2);
    SPI_SCK = 1;
    Delay_10us(2);
    tmp = tmp << 1;	
  }
	
	tmp = 0;
  for (i = 0; i < 8; i++)
  {
    SPI_SCK = 1;
    Delay_10us(1);
    SPI_SCK = 0;
    Delay_10us(1);
    tmp <<= 1;
    if (SPI_SDI)
    {
      tmp |= 0x01;
    }
  }
	
	SPI_CS = 1;
  return tmp;
}

static uint8_t RGB_SPI16BitFallingEdgeRead(void)
{
  uint16_t i;
  uint8_t tmp = 0xC0;
	SPI_CS = 0;
	SPI_SCK = 1;	
  Delay_10us(1);
	
	for (i = 0; i < 8; i++)
  {
    if ((tmp & 0x80) == 0x80)
    {
      SPI_SDA = 1;
    }
    else
    {
      SPI_SDA = 0;
    }

    Delay_10us(2);
    SPI_SCK = 1;
    Delay_10us(2);
    SPI_SCK = 0;
    Delay_10us(2);
    tmp = tmp << 1;	
  }
	
	tmp = 0;
  for (i = 0; i < 8; i++)
  {
    SPI_SCK = 0;
    Delay_10us(1);
    SPI_SCK = 1;
    Delay_10us(1);
    tmp <<= 1;
    if (SPI_SDI)
    {
      tmp |= 0x01;
    }
  }
	
	SPI_CS = 1;
  return tmp;
}


void RGB_SPIWrite_16Bit(uint16_t address , uint8_t data , SPIEdgeTypeDef edge)
{
	if(edge == SPI_RISING)
	{
		RGB_SPI16BitRisingEdgeWrite(SPI_WRITE,SPI_COMMAND,SPI_HIGH_BYTE,address>>8);
		RGB_SPI16BitRisingEdgeWrite(SPI_WRITE,SPI_COMMAND,SPI_LOW_BYTE,(uint8_t)address);
		RGB_SPI16BitRisingEdgeWrite(SPI_WRITE,SPI_DATA,SPI_LOW_BYTE,data);
	}
	else
	{
		RGB_SPI16BitFallingEdgeWrite(SPI_WRITE,SPI_COMMAND,SPI_HIGH_BYTE,address>>8);
		RGB_SPI16BitFallingEdgeWrite(SPI_WRITE,SPI_COMMAND,SPI_LOW_BYTE,(uint8_t)address);
		RGB_SPI16BitFallingEdgeWrite(SPI_WRITE,SPI_DATA,SPI_LOW_BYTE,data);		
	}
}

uint8_t RGB_SPIRead_16Bit(uint16_t address , SPIEdgeTypeDef edge)
{
	if(edge == SPI_RISING)
	{
		RGB_SPI16BitRisingEdgeWrite(SPI_WRITE,SPI_COMMAND,SPI_HIGH_BYTE,address>>8);
		RGB_SPI16BitRisingEdgeWrite(SPI_WRITE,SPI_COMMAND,SPI_LOW_BYTE,(uint8_t)address);
		return RGB_SPI16BitRisingEdgeRead();
	}
	else
	{
		RGB_SPI16BitFallingEdgeWrite(SPI_WRITE,SPI_COMMAND,SPI_HIGH_BYTE,address>>8);
		RGB_SPI16BitFallingEdgeWrite(SPI_WRITE,SPI_COMMAND,SPI_LOW_BYTE,(uint8_t)address);
		return RGB_SPI16BitFallingEdgeRead();		
	}	
}

void RGB_SPIWrite8Bit(uint8_t data ,SPIDataCommandTypeDef rs)
{
	uint8_t i = 0;
	SPI_CS = 0;
	SPI_RS = rs;
	SPI_SCK = 0;
	Delay_10us(2);
	
	for (i = 0; i < 8; i++)
  {
		SPI_SDA = data >>(7-i);
		
    Delay_10us(2);
    SPI_SCK = 1;
    Delay_10us(2);
    SPI_SCK = 0;
    Delay_10us(2);
	}
	
	SPI_CS = 1;
	
}

void RGB_SPIWrite9Bit(uint8_t data ,SPIDataCommandTypeDef rs)
{
	uint8_t i = 0;
	SPI_CS = 0;
	SPI_SCK = 0;
	Delay_10us(2);
	
	SPI_SDA = rs;
	Delay_10us(2);
	SPI_SCK = 1;
	Delay_10us(2);
	SPI_SCK = 0;
	
	for (i = 0; i < 8; i++)
  {
		SPI_SDA = data >>(7-i);
		
    Delay_10us(2);
    SPI_SCK = 1;
    Delay_10us(2);
    SPI_SCK = 0;
    Delay_10us(2);
	}
	
	SPI_CS = 1;
	
}


void SPI_2DataLaneWritePixel(uint16_t color)	
{
	uint8_t i,data_h,data_l;
	data_h=color>>8;
	data_l=color;
	SPI_CS=0;
	Delay(1);
	SPI_SDA=1;
	SPI_SDA2=1;
	Delay(1);
	SPI_SCK=0;
	Delay(1);
	SPI_SCK=1;
	Delay(1);
	for (i = 0; i < 8; i++)
  {
		SPI_SDA = data_h >>(7-i);
		SPI_SDA2 = data_l >>(7-i);
    Delay(1);
    SPI_SCK = 0;
    Delay(1);
    SPI_SCK = 1;
    Delay(1);
	}
	SPI_CS = 1;
}


void SPI_WriteFrame(uint16_t color)
{
	uint16_t i;
	uint16_t j;

   for(i=0;i<LCDTiming.LCDV;i++)   
	 {
			for(j=0;j<LCDTiming.LCDH;j++)
			{	
				 SPI_2DataLaneWritePixel(color); 	
			}
	 }	
}
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
