/**
 * @file        cdce913.c
 * @author      Weyne
 * @version     V01
 * @date        2016.08.07
 * @brief       CDCE913的驱动
 * @note
 *				      使用软件模拟I2C与CDCE913通讯，并自动计算
 *              CDCE913的各个参数设置
 * @attention   COYPRIGHT WEYNE
 */

#include "cdce913.h"
#include <stdio.h>
#include "stdlib.h"
#include "math.h"
#include "ack.h"
#include "stdbool.h"

/**
 * @brief CDCE913 接口 PIN 的配置区域
 */
#define CDCE_ADDRESS    0x65
#define CDCE_ID         0x81
#define I2C_SCL         PBout(8)
#define I2C_SDA         PBout(9)
#define READ_SDA        PBin(9)
#define S0              PCout(6)
#define I2C_WRITE       CDCE_ADDRESS << 1
#define I2C_READ        ((CDCE_ADDRESS << 1) | 0x01)



static void I2C_Delay(volatile uint8_t t)
{
  uint8_t i, n;

  for (n = 0; n < t; n++)
  {
    for (i = 0; i < 35; i++)
    {
    }
  }
}


static void I2C_Start(void)
{
  I2C_SDA = 1;
  I2C_SCL = 1;
  I2C_Delay(40);
  I2C_SDA = 0;
  I2C_Delay(6);
  I2C_SCL = 0;
}


static void I2C_Stop(void)
{
  I2C_SCL = 0;
  I2C_SDA = 0;
  I2C_Delay(4);
  I2C_SCL = 1;
  I2C_Delay(4);
  I2C_SDA = 1;
  I2C_Delay(4);
}


static AckTypeDef I2C_WaitAck(void)
{
  uint8_t timeout = 0;

  I2C_SDA = 1;
  I2C_Delay(1);
  I2C_SCL = 1;
  I2C_Delay(3);
  while (READ_SDA)
  {
    timeout++;
    if (timeout > 250)
    {
      I2C_Stop();

      return I2C_NO_ACK;
    }
  }
  I2C_SCL = 0;
  return I2C_ACK;
}


static void I2C_Ack(void)
{
  I2C_SCL = 0;
  I2C_SDA = 0;
  I2C_Delay(3);
  I2C_SCL = 1;
  I2C_Delay(3);
  I2C_SCL = 0;
  I2C_Delay(1);
  I2C_SDA = 1;
  I2C_Delay(4);
}


static void I2C_NAck(void)
{
  I2C_SCL = 0;
  I2C_SDA = 1;
  I2C_Delay(3);
  I2C_SCL = 1;
  I2C_Delay(3);
  I2C_SCL = 0;
}


static void I2C_SendByte(uint8_t txd)
{
  uint8_t t;

  I2C_SCL = 0;
  for (t = 0; t < 8; t++)
  {
    I2C_SDA = (txd & 0x80) >> 7;
    txd   <<= 1;
    I2C_Delay(2);
    I2C_SCL = 1;
    I2C_Delay(4);
    I2C_SCL = 0;
    I2C_Delay(2);
  }
  I2C_SDA = 0;
}


static uint8_t I2C_ReadByte(AckTypeDef ack)
{
  uint8_t i, receive = 0;

  for (i = 0; i < 8; i++)
  {
    I2C_SCL = 0;
    I2C_Delay(3);
    I2C_SCL   = 1;
    receive <<= 1;
    if (READ_SDA)
    {
      receive++;
    }
    I2C_Delay(2);
  }
  if (!ack)
  {
    I2C_NAck();
  }
  else
  {
    I2C_Ack();
  }
  return receive;
}


AckTypeDef CDCE_WriteByte(uint8_t reg, uint8_t data)
{
  I2C_Start();
  I2C_SendByte(I2C_WRITE);
  if (I2C_WaitAck() == I2C_NO_ACK)
  {
    return I2C_NO_ACK;
  }
  I2C_SendByte(reg | 0x80);
  if (I2C_WaitAck() == I2C_NO_ACK)
  {
    return I2C_NO_ACK;
  }
  I2C_SendByte(data);
  if (I2C_WaitAck() == I2C_NO_ACK)
  {
    return I2C_NO_ACK;
  }
  I2C_Stop();
  return I2C_ACK;
}


AckTypeDef CDCE_Read8bit(uint8_t reg, uint16_t para_amount, uint8_t *data)
{
  uint16_t i;
  AckTypeDef ack;

  if (para_amount == 0)
  {
    para_amount = 1;
  }

  I2C_Start();
  I2C_SendByte(I2C_WRITE);
  ack = I2C_WaitAck();
  if (ack != I2C_ACK)
  {
    return I2C_NO_ACK;
  }
  I2C_SendByte(reg | 0x80);
  ack = I2C_WaitAck();

  if (ack != I2C_ACK)
  {
    return I2C_NO_ACK;
  }

  I2C_Stop();

  I2C_Start();
  I2C_SendByte(I2C_READ);
  ack = I2C_WaitAck();

  if (ack != I2C_ACK)
  {
    return I2C_NO_ACK;
  }
  for (i = 0; i < para_amount - 1; i++)
  {
    data[i] = I2C_ReadByte(I2C_ACK);
    I2C_Delay(100);
  }
  data[para_amount - 1] = I2C_ReadByte(I2C_NO_ACK);
  I2C_Stop();


  return I2C_ACK;
}


#define CLK_IN    8

void CDCE_Init(uint16_t f_out)
{
  uint8_t read_back;
  uint8_t i = 1;
  AckTypeDef ack;
  uint32_t M, N, Pdiv, Q, R;
  uint8_t reg18, reg19, reg1A, reg1B;
  int P;
  uint16_t f_vco = f_out;
  bool result    = false;
  uint8_t f_range;

  while (f_vco < 80)
  {
    i++;
    f_vco = f_out * i;
  }

  while (f_vco < 231)
  {
    for (N = 4095; N > 0; N--)
    {
      for (M = 511; M > 0; M--)
      {
        if ((N * CLK_IN / M) == f_vco)
        {          
          {
            result = true;
            break;
          }
        }
      }
      if (result)
      {
        break;
      }
    }
    if (result)
    {
      break;
    }
    else
    {
      i++;
      f_vco = f_out * i;
    }
  }

  if (!result)
  {
    UserPrintf("Error:unsupport pclk frequence\n");
    return;
  }

  P = 4 - (int)((log((double)N / (double)M))/log(2));
  if (P < 0)
  {
    P = 0;
  }
  Q = (int)((double)N * pow(2, (double)P) / (double)M);
  R = (double)N * pow(2, (double)P) - M * Q;

  if (f_vco < 125)
  {
    f_range = 0;
  }
  else if ((f_vco >= 125) && (f_vco < 150))
  {
    f_range = 1;
  }
  else if ((f_vco >= 150) && (f_vco < 175))
  {
    f_range = 2;
  }
  else
  {
    f_range = 3;
  }

  S0 = 0;

  ack = CDCE_Read8bit(0x00, 1, &read_back);
  if (ack != I2C_ACK)
  {
    UserPrintf("Error:clk configuration failed , maybe no pullup res\n");
    return;
  }

  if (read_back != CDCE_ID)
  {
    UserPrintf("Error:clk device ID error\n");
    return;
  }

  Pdiv = f_vco / f_out;

  //UserPrintf("M:%d,N:%d,Pdiv:%d,f_vco:%d,P:%d,Q:%d,R:%d\n", M, N, Pdiv,f_vco,P,Q, R);

  CDCE_WriteByte(0x02, 0xB4);
  CDCE_WriteByte(0x03, (uint8_t)Pdiv);
  CDCE_WriteByte(0x04, 0x02);
  CDCE_WriteByte(0x05, 0x00);
  CDCE_WriteByte(0x06, 0x40);
  CDCE_WriteByte(0x12, 0x00);
  CDCE_WriteByte(0x13, 0x01);
  CDCE_WriteByte(0x14, 0x6D);
  CDCE_WriteByte(0x15, 0x02);
  CDCE_WriteByte(0x16, 0);
  CDCE_WriteByte(0x17, 0);

  reg18 = (N >> 4) & 0xFFF;
  reg19 = (N & 0xf) << 4 | (R & 0xf0) >> 5;
  reg1A = (R & 0x1f) << 3 | ((Q >> 3) & 0x7);
  reg1B = (Q & 0x7) << 5 | (P & 0x07) << 2 | (f_range & 0x03);

  CDCE_WriteByte(0x18, reg18);
  CDCE_WriteByte(0x19, reg19);
  CDCE_WriteByte(0x1A, reg1A);
  CDCE_WriteByte(0x1B, reg1B);

  CDCE_WriteByte(0x1C, N);
  CDCE_WriteByte(0x1D, ((N & 0xf) << 4) | (R & 0xf0));
  CDCE_WriteByte(0x1E, (R & 0x0f) | (Q & 0xf0));
  CDCE_WriteByte(0x1F, ((Q & 0x07) << 5) | ((P & 0x07) << 2) | (f_range & 0x03));

  S0 = 1;
  UserPrintf("Info:clk well configured\n");
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
