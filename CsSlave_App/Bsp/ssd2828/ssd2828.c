/**
 * @file        ssd2828.c
 * @author      Weyne
 * @version     V01
 * @date        2016.08.07
 * @brief       SSD2828的驱动
 * @note
 * @attention   COYPRIGHT WEYNE
 */

#include "ssd2828.h"
#include "sys.h"
#include "sysconfig.h"
#include "ack.h"
/**
 * @brief SSD2828 IO PIN 的配置区域
 */
#define SPI_SDI          PEin(5)  /*< SPI SDI */
#define SPI_SDO          PEout(4) /*< SPI SDO */
#define SPI_SCK          PEout(3) /*< SPI CLK */
#define SPI_CS           PEout(0) /*< SSD2828 1 的SPI使能PIN，低电平有效 */
#define SSD2828_RESET    PEout(6) /*< SSD2828 Reset pin */
#define SSD2828_SHUT     PEout(2) /*< SPI Shut down 功能脚 */


static uint16_t mode = LP;

static void Delay_10us(volatile uint8_t t)
{
  volatile uint8_t i = 15;

  while (t--)
  {
    while (i--)
    {
    }
  }
}


static void SSD2828WriteCmd(uint8_t cmd)
{
  uint8_t i;

  SPI_SDO = 0;
  Delay_10us(2);
  SPI_SCK = 0;
  Delay_10us(2);
  SPI_SCK = 1;
  Delay_10us(2);
  for (i = 0; i < 8; i++)
  {
    if ((cmd & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    Delay_10us(2);
    SPI_SCK = 0;
    Delay_10us(2);
    SPI_SCK = 1;
    Delay_10us(2);
    cmd = cmd << 1;
  }
  Delay_10us(0);
}


void SSD2828WriteData(uint8_t data)
{
  uint8_t i;

  SPI_SDO = 1;
  Delay_10us(2);
  SPI_SCK = 0;
  Delay_10us(2);
  SPI_SCK = 1;
  Delay_10us(2);
  for (i = 0; i < 8; i++)
  {
    if ((data & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    Delay_10us(2);
    SPI_SCK = 0;
    Delay_10us(2);
    SPI_SCK = 1;
    Delay_10us(2);
    data = data << 1;
  }
  Delay_10us(2);
}


static void SSD2828WriteReg(uint8_t cmd, uint8_t dat1, uint8_t dat2)
{
  uint8_t i;

  SPI_SDO = 0;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 1;
  for (i = 0; i < 8; i++)
  {
    if ((cmd & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 1;
    cmd = cmd << 1;
  }
  SPI_SDO = 1;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 1;
  for (i = 0; i < 8; i++)
  {
    if ((dat2 & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 1;
    dat2 = dat2 << 1;
  }
  SPI_SDO = 1;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 0;
  SPI_SCK = 1;
  for (i = 0; i < 8; i++)
  {
    if ((dat1 & 0x80) == 0x80)
    {
      SPI_SDO = 1;
    }
    else
    {
      SPI_SDO = 0;
    }
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 0;
    SPI_SCK = 1;
    dat1 = dat1 << 1;
  }
}


static uint8_t SSD2828Read()
{
  uint16_t i;
  uint8_t tmp = 0;

  Delay_10us(1);
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
  return tmp;
}


static uint16_t SSD2828ReadReg(uint8_t reg)
{
  uint16_t tmp;
  uint8_t data_l, data_h;

  SSD2828WriteReg(0xd4, 0x00, 0xfa);
  SSD2828WriteCmd(reg);
  SSD2828WriteCmd(0xfa);
  data_l = SSD2828Read();
  data_h = SSD2828Read();
  tmp = data_h;
  tmp <<= 8;
  tmp |= data_l;
  return tmp;
}


void SSD2828_DcsShortWrite(uint8_t n)
{
  if (mode == LP)
  {
    SSD2828WriteReg(0x00b7, 0x02, 0x50);
  }
  else if (mode == HS)
  {
    SSD2828WriteReg(0x00b7, 0x02, 0x50 & 0XEF | 0X03);
  }
  else if (mode == VD)
  {
    SSD2828WriteReg(0x00b7, 0x02 | 0x01, 0x50 & 0XEF | 0X0B);
  }
  Delay_10us(10);
  SSD2828WriteReg(0xbc, 0x00, n);
  SSD2828WriteReg(0xbd, 0x00, 0x00);
  SSD2828WriteReg(0xbe, 0x00, n);
  SSD2828WriteCmd(0xbf);
}


void SSD2828_DcsLongWrite(uint32_t n)
{
  if (mode == LP)
  {
    SSD2828WriteReg(0x00b7, 0x06, 0x50);
  }
  else if (mode == HS)
  {
    SSD2828WriteReg(0x00b7, 0x06, 0x50 & 0XEF | 0X03);
  }
  else if (mode == VD)
  {
    SSD2828WriteReg(0x00b7, 0x06 | 0x01, 0x50 & 0XEF | 0X0B);
  }
  Delay_10us(10);
  SSD2828WriteReg(0xbc, n >> 8, n);
  SSD2828WriteReg(0xbd, n >> 24, n >> 16);
  SSD2828WriteReg(0xbe, 0x0f, 0xff);
  SSD2828WriteCmd(0xbf);
}


void SSD2828_GenericShortWrite(uint8_t n)
{
  if (mode == LP)
  {
    SSD2828WriteReg(0x00b7, 0x02, 0x10);
  }
  else if (mode == HS)
  {
    SSD2828WriteReg(0x00b7, 0x02, 0x10 & 0XEF | 0X03);
  }
  else if (mode == VD)
  {
    SSD2828WriteReg(0x00b7, 0x02 | 0x01, 0x10 & 0XEF | 0X0B);
  }
  Delay_10us(10);
  SSD2828WriteReg(0xbc, 0x00, n);
  SSD2828WriteReg(0xbd, 0x00, 0x00);
  SSD2828WriteReg(0xbe, 0x00, n);
  SSD2828WriteCmd(0xbf);
}


void SSD2828_GenericLongWrite(uint32_t n)
{
  if (mode == LP)
  {
    SSD2828WriteReg(0x00b7, 0x06, 0x10);
  }
  else if (mode == HS)
  {
    SSD2828WriteReg(0x00b7, 0x06, 0x10 & 0XEF | 0X03);
  }
  else if (mode == VD)
  {
    SSD2828WriteReg(0x00b7, 0x06 | 0X01, 0x10 & 0XEF | 0X0B);
  }
  Delay_10us(10);
  SSD2828WriteReg(0xbc, n >> 8, n);
  SSD2828WriteReg(0xbd, n >> 24, n >> 16);
  SSD2828WriteReg(0xbe, 0x0f, 0xff);
  SSD2828WriteCmd(0xbf);
}


MIPI_ReadTypeDef SSD2828_DcsReadDT06(uint8_t adr, uint16_t l, uint8_t *p)
{
  uint16_t state;
  uint16_t i;
  uint16_t timeout_cnt = 0;

  do
  {
    if (mode == LP)
    {
      SSD2828WriteReg(0x00b7, 0x03, 0xc2);
    }
    else if (mode == HS)
    {
      SSD2828WriteReg(0x00b7, 0x03, 0xc3);
    }
    else if (mode == VD)
    {
      SSD2828WriteReg(0x00b7, 0x03, 0xcb);
    }
    SSD2828WriteReg(0x00bb, 0x00, 8);
    SSD2828WriteReg(0x00c1, l >> 8, l);
    SSD2828WriteReg(0x00c0, 0x00, 0x01);
    SSD2828WriteReg(0x00BC, 0x00, 0x01);
    SSD2828WriteReg(0x00BF, 0x00, adr);
    HAL_Delay(10);
    state = SSD2828ReadReg(0xc6);
    if (++timeout_cnt > 10)
    {
      return MIPI_READ_FAIL;
    }
  } while ((state & 0x01) == 0);
  SSD2828WriteCmd(0xff);
  for (i = 0; i < l; )
  {
    SSD2828WriteCmd(0xfa);
    *p++ = SSD2828Read();
    if (++i >= l)
    {
      SSD2828Read();
      break;
    }
    *p++ = SSD2828Read();
    ++i;
  }
  return MIPI_READ_SUCCEED;
}


MIPI_ReadTypeDef SSD2828_GenericReadDT14(uint8_t adr, uint16_t l, uint8_t *p)
{
  uint16_t state;
  uint16_t i;
  uint16_t timeout_cnt = 0;

  do
  {
    if (mode == LP)
    {
      SSD2828WriteReg(0x00b7, 0x03, 0x82);
    }
    else if (mode == HS)
    {
      SSD2828WriteReg(0x00b7, 0x03, 0x83);
    }
    else if (mode == VD)
    {
      SSD2828WriteReg(0x00b7, 0x03, 0x8B);
    }
    SSD2828WriteReg(0x00bb, 0x00, 8);
    SSD2828WriteReg(0x00c1, l >> 8, l);
    SSD2828WriteReg(0x00c0, 0x00, 0x01);
    SSD2828WriteReg(0x00BC, 0x00, 1);
    SSD2828WriteReg(0x00BF, 0x00, adr);
    HAL_Delay(10);
    state = SSD2828ReadReg(0xc6);
    if (++timeout_cnt > 10)
    {
      return MIPI_READ_FAIL;
    }
  } while ((state & 0x01) == 0);
  SSD2828WriteCmd(0xff);
  for (i = 0; i < l; )
  {
    SSD2828WriteCmd(0xfa);
    *p++ = SSD2828Read();
    if (++i >= l)
    {
      SSD2828Read();
      break;
    }
    *p++ = SSD2828Read();
    ++i;
  }
  return MIPI_READ_SUCCEED;
}


void SSD2828_SetMode(MIPI_ModeTypeDef m)
{
  mode = m;
  if (mode == VD)
  {
    SSD2828WriteReg(0x00b7, 0x03, 0x0B);
  }
}



/**
 * @brief  SSD2828 初始化
 * @param  lane : 通道数
 * @param data_rate : 速率
 * @retval None
 */
void SSD2828_Init(uint8_t lane, uint16_t data_rate)
{
  uint8_t i;

  SSD2828_RESET = 0;
  SSD2828_SHUT = 0;
  HAL_Delay(50);
  SSD2828_RESET = 1;
  HAL_Delay(10);
	SPI_CS = 0;
	HAL_Delay(10);	
	if (SSD2828ReadReg(0xB0) == 0x2828)
	{
		UserPrintf("Info:SSD2828 OK\n", i);
	}
	else
	{
		UserPrintf("Error:SSD2828 configuration failed\n", i);
	}


	SSD2828WriteReg(0x00b9, 0x00, 0x00);
	SSD2828WriteReg(0x00b1, LCDTiming.VSPW, LCDTiming.HSPW);
	SSD2828WriteReg(0x00b2, LCDTiming.VBPD, LCDTiming.HBPD+10);
	SSD2828WriteReg(0x00b3, LCDTiming.VFPD, LCDTiming.HFPD);
	SSD2828WriteReg(0xb4, (LCDTiming.LCDH >> 8) & 0xff, LCDTiming.LCDH & 0xff);
	SSD2828WriteReg(0xb5, (LCDTiming.LCDV >> 8) & 0xff, LCDTiming.LCDV & 0xff);
	SSD2828WriteReg(0x00b6, 0x00, 0x07);
	if (data_rate < 500)
	{
		SSD2828WriteReg(0x00ba, 0x82, data_rate / 12);
	}
	else
	{
		SSD2828WriteReg(0x00ba, 0xc1, data_rate / 24);
	}
	SSD2828WriteReg(0x00bb, 0x00, 0x06);
	SSD2828WriteReg(0x00b8, 0x00, 0x00);
	SSD2828WriteReg(0x00c9, 0x25, 0x09);
	SSD2828WriteReg(0x00ca, 0x23, 0x01);
	SSD2828WriteReg(0x00cb, 0x05, 0x10);
	SSD2828WriteReg(0x00cc, 0x10, 0x05);
	SSD2828WriteReg(0x00de, 0x00, lane - 1);
	SSD2828WriteReg(0x00d6, 0x00, 0x05);
	SSD2828WriteReg(0x00c4, 0x00, 0x01);
	SSD2828WriteReg(0x00eb, 0x80, 0x00);
	HAL_Delay(10);
	SSD2828WriteReg(0x00b9, 0x00, 0x01);
	HAL_Delay(120);
  
}


/**
 * @brief  是否关闭MIPI数据传输
 * @param  state : ENABLE or DISABLE
 * @retval None
 */
void SSD2828_ShutDown(FunctionalState state)
{
  SSD2828_SHUT = (uint8_t)state & 0x01;
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
