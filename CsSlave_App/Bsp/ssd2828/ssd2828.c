#include "stm32f1xx_hal.h"
#include "ssd2828.h"

#define SPI_SDO_H      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2,GPIO_PIN_SET)
#define SPI_SDO_L      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2,GPIO_PIN_RESET)
#define SPI_SCLK_H     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3,GPIO_PIN_SET)
#define SPI_SCLK_L     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3,GPIO_PIN_RESET)
#define SPI_SDI        HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1)
#define SSD_RESET_H    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0,GPIO_PIN_SET)
#define SSD_RESET_L    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0,GPIO_PIN_RESET)
#define SSD_SHUT_H     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5,GPIO_PIN_SET)
#define SSD_SHUT_L     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5,GPIO_PIN_RESET)

#define LP             0
#define HS             1
#define VD             2

uint16_t mode;

static void Delay_10us(volatile uint8_t t)
{
	volatile uint8_t i=15;
	while(t--)
	{
		while(i--)
		{
			;
		}
	}
}


void SSD2828WriteCmd(uint8_t cmd)
{
  uint8_t i;

  SPI_SDO_L;
  Delay_10us(2);
  SPI_SCLK_L;
  Delay_10us(2);
  SPI_SCLK_H;
  Delay_10us(2);
  for (i = 0; i < 8; i++)
  {
    if ((cmd & 0x80) == 0x80)
    {
      SPI_SDO_H;
    }
    else
    {
      SPI_SDO_L;
    }
    Delay_10us(2);
    SPI_SCLK_L;
    Delay_10us(2);
    SPI_SCLK_H;
    Delay_10us(2);
    cmd = cmd << 1;
  }
  Delay_10us(0);
}


void SSD2828WriteData(uint8_t dat)
{
  uint8_t i;

  SPI_SDO_H;
  Delay_10us(2);
  SPI_SCLK_L;
  Delay_10us(2);
  SPI_SCLK_H;
  Delay_10us(2);
  for (i = 0; i < 8; i++)
  {
    if ((dat & 0x80) == 0x80)
    {
      SPI_SDO_H;
    }
    else
    {
      SPI_SDO_L;
    }
    Delay_10us(2);
    SPI_SCLK_L;
    Delay_10us(2);
    SPI_SCLK_H;
    Delay_10us(2);
    dat = dat << 1;
  }
  Delay_10us(2);
}


void SSD2828WriteReg(uint8_t cmd, uint8_t dat1, uint8_t dat2)
{
  uint8_t i;

  SPI_SDO_L;
  SPI_SCLK_L;
  SPI_SCLK_L;
  SPI_SCLK_L;
  SPI_SCLK_H;
  for (i = 0; i < 8; i++)
  {
    if ((cmd & 0x80) == 0x80)
    {
      SPI_SDO_H;
    }
    else
    {
      SPI_SDO_L;
    }
    SPI_SCLK_L;
    SPI_SCLK_L;
    SPI_SCLK_L;
    SPI_SCLK_H;
    cmd = cmd << 1;
  }
  SPI_SDO_H;
  SPI_SCLK_L;
  SPI_SCLK_L;
  SPI_SCLK_L;
  SPI_SCLK_H;
  for (i = 0; i < 8; i++)
  {
    if ((dat2 & 0x80) == 0x80)
    {
      SPI_SDO_H;
    }
    else
    {
      SPI_SDO_L;
    }
    SPI_SCLK_L;
    SPI_SCLK_L;
    SPI_SCLK_L;
    SPI_SCLK_H;
    dat2 = dat2 << 1;
  }
  SPI_SDO_H;
  SPI_SCLK_L;
  SPI_SCLK_L;
  SPI_SCLK_L;
  SPI_SCLK_H;
  for (i = 0; i < 8; i++)
  {
    if ((dat1 & 0x80) == 0x80)
    {
      SPI_SDO_H;
    }
    else
    {
      SPI_SDO_L;
    }
    SPI_SCLK_L;
    SPI_SCLK_L;
    SPI_SCLK_L;
    SPI_SCLK_H;
    dat1 = dat1 << 1;
  }
}


void SSD2828DcsShortWrite(uint8_t n)
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


void SSD2828DcsLongWrite(uint32_t n)
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


void SSD2828GenericShortWrite(uint8_t n)
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


void SSD2828GenericLongWrite(uint32_t n)
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


uint8_t SSD2828Read()
{
  uint16_t i;
  uint8_t tmp = 0;

  Delay_10us(1);
  for (i = 0; i < 8; i++)
  {
    SPI_SCLK_L;
    Delay_10us(1);
    SPI_SCLK_H;
    Delay_10us(1);
    tmp <<= 1;
    if (SPI_SDI)
    {
      tmp |= 0x01;
    }
  }
  return tmp;
}


uint16_t SSD2828ReadReg(uint8_t reg)
{
  uint16_t tmp;
  uint8_t data_l, data_h;

  SSD2828WriteReg(0xd4, 0x00, 0xfa);
  SSD2828WriteCmd(reg);
  SSD2828WriteCmd(0xfa);
  data_l = SSD2828Read();
  data_h = SSD2828Read();
  tmp    = data_h;
  tmp  <<= 8;
  tmp   |= data_l;
  return tmp;
}


uint16_t SSD2828DcsReadDT06(uint8_t adr, uint16_t l, uint8_t *p)
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


uint16_t SSD2828GenericReadDT14(uint8_t adr, uint16_t l, uint8_t *p)
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


void SSD2828LP()
{
  mode = LP;
}


void SSD2828Video()
{
  mode = VD;
  SSD2828WriteReg(0x00b7, 0x03, 0x0B);
}


void SSD2828Init(uint8_t lane, uint16_t dataRate)
{
  SSD_RESET_L;
  SSD_SHUT_L;
	HAL_Delay(50);
  SSD_RESET_H;
	HAL_Delay(10);
  SSD2828_IC1_CS_0();
  SSD2828_IC2_CS_1();
  SSD2828WriteReg(0x00b9, 0x00, 0x00);
  SSD2828WriteReg(0x00b1, 0, 0);
  SSD2828WriteReg(0x00b2, 0, 0 + 10);
  SSD2828WriteReg(0x00b3, 0, 0);
  SSD2828WriteReg(0xb4, (0 >> 8) & 0xff, 0 & 0xff);
  SSD2828WriteReg(0xb5, (0 >> 8) & 0xff, 0 & 0xff);
  SSD2828WriteReg(0x00b6, 0x00, 0x07);
  if (dataRate < 500)
  {
    SSD2828WriteReg(0x00ba, 0x82, dataRate / 12);
  }
  else
  {
    SSD2828WriteReg(0x00ba, 0xc1, dataRate / 24);
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


void SSD2828_RST_H(void)
{
  SSD_RESET_H;
}


void SSD2828_RST_L(void)
{
  SSD_RESET_L;
}


void SSD2828_IC1_CS_0(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6,GPIO_PIN_RESET);
}


void SSD2828_IC1_CS_1(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6,GPIO_PIN_SET);
}


void SSD2828_IC2_CS_0(void)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_RESET);
}


void SSD2828_IC2_CS_1(void)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_SET);
}


void WR_DT29(uint32_t n)
{
  SSD2828GenericLongWrite(n);
}


void WR_DT03_13_23(uint8_t n)
{
  SSD2828GenericShortWrite(n);
}


void WR_DT39(uint32_t n)
{
  SSD2828DcsLongWrite(n);
}


void WR_DT05_15(uint8_t n)
{
  SSD2828DcsShortWrite(n);
}


void RD_DT06(uint8_t adr, uint16_t l, uint8_t *p)
{
  SSD2828DcsReadDT06(adr, l, p);
}


void RD_DT14(uint8_t adr, uint16_t l, uint8_t *p)
{
  SSD2828GenericReadDT14(adr, l, p);
}
