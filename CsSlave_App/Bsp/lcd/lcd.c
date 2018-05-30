/**
 * @file        lcd.c
 * @author      Weyne
 * @version     V01
 * @date        2016.08.23
 * @brief       LCD驱动部分
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#include "lcd.h"
#include "sysconfig.h"
#include "power.h"
#include "ssd2828.h"
#include "string.h"
#include "in_img.h"
#include "res.h"
#include "cdce913.h"
#include "fpga.h"
#include "tim.h"
#include "ack.h"
#include "rgbif.h"
#include "font.h"
#include <stdarg.h>


FontColorTypeDef FontColor = { 0xffffff, 0 };
uint8_t FontScale = 1;
uint8_t ReadBackAmount = 0;
uint8_t ReadBackTemp[32];
uint8_t ReadBackValue[16];
uint8_t ReadBackValueAmount = 0;
bool IsNeedClearBg = false;

static uint8_t ShowIDPattern = 0xff;
static uint8_t FlickerIndex  = 0xff;
static uint8_t FindSDRAMPatternAmount(void);

static const PowerTypeDef PowerLUT[POWER_AMOUT] =
{
  POWER_1V8,  POWER_2V8, POWER_3V3, POWER_VSP, POWER_VSN, POWER_OUT5V, POWER_MTP, POWER_AVDD,
  POWER_VCOM, POWER_VGH, POWER_VGL
};

static struct
{
  GPIO_TypeDef *gpio_port;
  uint16_t     gpio_pin;
}
ResetPortPin;

void closeAllPower(void)
{
  HAL_GPIO_WritePin(ResetPortPin.gpio_port, ResetPortPin.gpio_pin, GPIO_PIN_RESET);
  //倒序关闭电源
  for (int i = POWER_AMOUT; i > 0; i--)
  {
    Power_SetState(PowerLUT[i - 1], OFF);
    HAL_Delay(10);
  }
}


void SetFontColor(uint32_t color)
{
  FontColor.Fore = color;
}


void SetBgColor(uint32_t color)
{
  FontColor.Background = color;
}


void SetLcdPower(StateTypeDef state)
{
  uint8_t i = 0;
  SSD2828_SetReset(0);
  if (state == ON)
  {
    //顺序开启电源
    for (i = 0; i < POWER_AMOUT; i++)
    {
      if (((SystemConfig.PowerSettings >> i) & 0x01) == 0x01)
      {
        Power_SetState(PowerLUT[i], ON);
        HAL_Delay(10);
      }
    }
  }
  else
  {
    //倒序关闭电源
    for (i = POWER_AMOUT; i > 0; i--)
    {
      if (((SystemConfig.PowerSettings >> (i - 1)) & 0x01) == 0x01)
      {
        Power_SetState(PowerLUT[i - 1], OFF);
        HAL_Delay(50);
      }
    }
  }
}


/*
 * 每个电源或者reset用3个字节表示，
 * 第一个字节表示电源索引，
 * 第二三字节表示延时时间，
 * 包的第一个字节表示总的索引个数。
 */
void SetLcdPowerByUser(StateTypeDef state)
{
  uint8_t *p;

  SSD2828_SetReset(0);

  if (state == ON)
  {
    p = (uint8_t *)SystemConfig.PowerOnSequence;
  }
  else
  {
    p = (uint8_t *)SystemConfig.PowerOffSequence;
  }

  uint8_t len = *p;
  if (len > POWER_LEN / 3)
  {
    //error here
    UserPrintf("Error:power settings error!\n");
    return;
  }

  uint8_t i = 1;
  while (len--)
  {
    uint8_t index = *(p + i);
    if (index > POWER_AMOUT + 1)
    {
      //error here
      UserPrintf("Error:power settings error!\n");
      return;
    }

    //电源查找表后面两个索引解析为reset high和reset low
    if (index == POWER_AMOUT)
    {
      HAL_GPIO_WritePin(ResetPortPin.gpio_port, ResetPortPin.gpio_pin, GPIO_PIN_SET);
    }
    else if (index == POWER_AMOUT + 1)
    {
      HAL_GPIO_WritePin(ResetPortPin.gpio_port, ResetPortPin.gpio_pin, GPIO_PIN_RESET);
    }
    else
    {
      Power_SetState(PowerLUT[index], state);
    }
    i++;
    uint16_t delay_time;
    delay_time = *(p + i++);
    delay_time = (delay_time << 8) | *(p + i++);

    if (delay_time > 10000)
    {
      //error here
      UserPrintf("Error:power settings error!\n");
      return;
    }
    HAL_Delay(delay_time);
  }
}


void SetLcdTiming(void)
{
  memcpy((uint16_t *)&LCDTiming, (uint16_t *)SystemConfig.LCDTimingPara, sizeof(SystemConfig.LCDTimingPara));
  CDCE_Init(LCDTiming.DCLK);
  LcdDrvSetTiming();
}


void SetMipiPara(void)
{
  uint8_t size = *(uint8_t *)SystemConfig.MIPIConfig;
  uint16_t i;

  if (size % 3 != 0)
  {
    return;
  }

  if (size == 3)
  {
    uint8_t lane   = SystemConfig.MIPIConfig[3];
    uint16_t speed = (SystemConfig.MIPIConfig[1] << 8) | SystemConfig.MIPIConfig[2];

    SSD2828_Init(lane, speed);
  }
  else
  {
    SSD2828_Reset();

    for (i = 1; i < size; i += 3)
    {
      SSD2828WriteReg(SystemConfig.MIPIConfig[i], SystemConfig.MIPIConfig[i + 1], SystemConfig.MIPIConfig[i + 2]);
    }
    HAL_Delay(20);
  }
}


void ResetRGBLcd(void)
{
  HAL_GPIO_WritePin(LS245_OE_GPIO_Port, LS245_OE_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  //  HAL_GPIO_WritePin(MIPIRESET_GPIO_Port, MIPIRESET_Pin, GPIO_PIN_SET);
  //  HAL_Delay(10);

  RGB_RESET = 0;
  HAL_Delay(120);
  RGB_RESET = 1;
  HAL_Delay(120);
}


void SetRGBSPI8Or9BitLcdInitCode(void)
{
  uint16_t code_size  = (SystemConfig.LCDInitCode[0] << 8) | SystemConfig.LCDInitCode[1];
  uint8_t *p          = &SystemConfig.LCDInitCode[2];
  uint8_t para_amount = 0;

  //  SPIEdgeTypeDef package;
  uint16_t i = 0, j = 0;
  uint16_t delay_time;

  //uint8_t buffer[32];
  //  uint8_t para;
  //ReadBackAmount = 0;

  while (i < code_size)
  {
    switch ((RGBTypeDef)(*(p + i++)))
    {
    case RGB_SPI_RISING:
      //    package = SPI_RISING;
      break;

    case RGB_SPI_FALLING:
      //  package = SPI_FALLING;
      break;

    case RGB_DELAY:
      delay_time = *(p + i++);
      delay_time = (delay_time << 8) | *(p + i++);
      HAL_Delay(delay_time);
      break;

    case RGB_WRITE:
      para_amount = *(p + i++);
      if (SystemConfig.LcdType == RGB_SPI8BIT)
      {
        RGB_SPIWrite8Bit(*(p + i++), SPI_COMMAND);

        for (j = 0; j < para_amount - 1; j++)
        {
          RGB_SPIWrite8Bit(*(p + i++), SPI_DATA);
        }
      }
      else
      {
        RGB_SPIWrite9Bit(*(p + i++), SPI_COMMAND);

        for (j = 0; j < para_amount - 1; j++)
        {
          RGB_SPIWrite9Bit(*(p + i++), SPI_DATA);
        }
      }
      break;

    case RGB_READ:
      UserPrintf("Waring:This function will be supported next version!\n");
      break;

    default:
      break;
    }
  }
}


void SetRGBSPI16BitLcdInitCode(void)
{
  uint16_t code_size = (SystemConfig.LCDInitCode[0] << 8) | SystemConfig.LCDInitCode[1];
  uint8_t *p         = &SystemConfig.LCDInitCode[2];
  uint16_t reg       = 0;
  uint8_t para;
  SPIEdgeTypeDef package = SPI_RISING;
  uint16_t delay_time;
  uint16_t i = 0;

  ReadBackAmount = 0;

  while (i < code_size)
  {
    switch ((RGBTypeDef)(*(p + i++)))
    {
    case RGB_SPI_RISING:
      package = SPI_RISING;
      break;

    case RGB_SPI_FALLING:
      package = SPI_FALLING;
      break;

    case RGB_DELAY:
      delay_time = *(p + i++);
      delay_time = (delay_time << 8) | *(p + i++);
      HAL_Delay(delay_time);
      break;

    case RGB_WRITE:
      reg  = 0;
      reg  = *(p + i++);
      reg  = *(p + i++) << 8 | reg;
      para = *(p + i++);
      RGB_SPIWrite_16Bit(reg, para, package);
      break;

    case RGB_READ:
      reg = 0;
      reg = *(p + i++);
      reg = *(p + i++) << 8 | reg;

      UserPrintf("Info:read 0x%04X\n", reg);
      para = RGB_SPIRead_16Bit(reg, package);
      if (sizeof(ReadBackTemp) - ReadBackAmount > +2)
      {
        ReadBackTemp[ReadBackAmount++] = 2;   //len
        ReadBackTemp[ReadBackAmount++] = reg; //register
        ReadBackTemp[ReadBackAmount++] = para;
      }

      UserPrintf("para = 0x%02X\n", para);

      break;

    default:
      break;
    }
  }
}


/**
 * @brief  解析并设定LCD初始化
 * @note   头两个字节为整个initcode的长度
 *          后面为MIPI_TYPE + LEN + DATA
 * @param  None
 * @retval None
 */
void SetMipiLcdInitCode(void)
{
  uint16_t code_size  = (SystemConfig.LCDInitCode[0] << 8) | SystemConfig.LCDInitCode[1];
  uint8_t *p          = &SystemConfig.LCDInitCode[2];
  uint8_t para_amount = 0;
  uint8_t para;
  MipiPackageDef package = NO_PACKAGE;
  uint16_t i             = 0, j = 0;
  uint16_t delay_time;
  MIPI_ReadTypeDef result;
  uint8_t buffer[32];

  ReadBackAmount = 0;
  ReadBackValueAmount = 0;
  SSD2828_SetMode(LP);

  while (i < code_size)
  {
    switch ((MipiTypeDef)(*(p + i++)))
    {
    case MIPI_DCS:
      package = DCS;
      break;

    case MIPI_GP:
      package = GP;
      break;

    case MIPI_DELAY:
      delay_time = *(p + i++);
      delay_time = (delay_time << 8) | *(p + i++);
      HAL_Delay(delay_time);
      break;

    case MIPI_WRITE:
      if (package == DCS)
      {
        para_amount = *(p + i++);
        if (para_amount == 1)
        {
          SSD2828_DcsShortWrite(1);
          SSD2828WriteData(*(p + i++));
        }
        else
        {
          SSD2828_DcsLongWrite(para_amount);
          for (j = 0; j < para_amount; j++)
          {
            SSD2828WriteData(*(p + i++));
          }
        }
      }
      else if (package == GP)
      {
        para_amount = *(p + i++);
        if (para_amount == 1)
        {
          SSD2828_GenericShortWrite(1);
          SSD2828WriteData(*(p + i++));
        }
        else
        {
          SSD2828_GenericLongWrite(para_amount);
          for (j = 0; j < para_amount; j++)
          {
            SSD2828WriteData(*(p + i++));
          }
        }
      }
      else
      {
        para_amount = *(p + i++);
        for (j = 0; j < para_amount; j++)
        {
          i++;
        }
        UserPrintf("Error: not specified MIPI package\n");
        return;
      }
      break;

    case MIPI_READ:
      para        = *(p + i++);
      para_amount = *(p + i++);
      if (package == DCS)
      {
        result = SSD2828_DcsReadDT06(para, para_amount, buffer);
      }
      if (package == GP)
      {
        result = SSD2828_GenericReadDT14(para, para_amount, buffer);
      }

      if (result == MIPI_READ_SUCCEED)
      {
        UserPrintf("Info:read 0x%02X\n", para);

        if (sizeof(ReadBackTemp) - ReadBackAmount > para_amount + 2)
        {
          ReadBackTemp[ReadBackAmount++] = para_amount + 1; //len
          ReadBackTemp[ReadBackAmount++] = para;            //register
          memcpy(ReadBackTemp + ReadBackAmount, buffer, para_amount);
          ReadBackAmount+=para_amount;
          memcpy(ReadBackValue + ReadBackValueAmount, buffer, para_amount);
          ReadBackValueAmount+=para_amount;
        }

        for (j = 0; j < para_amount; j++)
        {
          HAL_Delay(10);
          UserPrintf("para%d = 0x%02X\n", j + 1, buffer[j]);
        }
      }
      else
      {
        UserPrintf("Waring:MIPI read failed!\n");
      }
      break;

    default:
      break;
    }
  }
}


static void ShowID(void)
{
  uint16_t j = 0, x = 0, y = 0;
  char temp[16];

  EnterBg();
  SetFontColor(0);
  while (j < ReadBackAmount)
  {
    uint8_t len = ReadBackTemp[j++];
    uint8_t n;

    for (n = 0; n < len; n++)
    {
      memset(temp, 0, sizeof(temp));
      sprintf(temp, "0x%02X ", ReadBackTemp[j++]);
      x = 16 * n * 5*FontScale;
      LCD_ShowString(x, y, temp);
    }
    x  = 0;
    y += 32*FontScale;
  }
  IsNeedClearBg = true;
}


void SetPattern(void)
{
  uint16_t size = (SystemConfig.Pattern[0] << 8) | SystemConfig.Pattern[1];
  uint8_t *p    = &SystemConfig.Pattern[2];
  uint16_t i    = 0;
  uint8_t r, g, b;
  uint16_t level;
  uint16_t stay_time;

  memset(&PatternProperty, 0, sizeof(PatternProperty));
  ShowIDPattern = 0xff;
  while (i < size)
  {
    uint8_t is_pattern = 1;

    FPGA_WRITE_CMD(0x1B);
    FPGA_WRITE_DATA(0xAA);
    FPGA_WRITE_DATA(0xAA);
    FPGA_WRITE_DATA(0xAA);
    LcdDrvSetPattern();
    if (PatternProperty.Counter > PATTERN_AMOUNT)
    {
      break;
    }

    switch ((PatternTypeDef) * (p + i++))
    {
    case PATTERN_START:
      is_pattern = 0;
      break;

    case RGB:
      r = *(p + i++);
      g = *(p + i++);
      b = *(p + i++);
      //Img_Full(r, g, b);
      FPGA_WRITE_CMD(0x1B);
      FPGA_WRITE_DATA(r);
      FPGA_WRITE_DATA(g);
      FPGA_WRITE_DATA(b);
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,RGB(0x%02X,%02X,%02X)", PatternProperty.Counter, r, g, b);
      PatternProperty.Data[PatternProperty.Counter] = (uint32_t)((uint32_t)(FPGA_IO_MODE) | (uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b);
      PatternProperty.Counter++;
      break;

    case SHOW_ID:
      if (PatternProperty.Counter != 0)
      {
        ShowIDPattern = PatternProperty.Counter - 1;
      }
      else
      {
        ShowIDPattern = 0;
      }

      //ShowID();
      break;

    case FLICKERV:
      Flicker();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,flicker", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      FlickerIndex = PatternProperty.Counter;
      PatternProperty.Counter++;
      break;

    case FLICKERH:
      Flicker();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,flicker", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      FlickerIndex = PatternProperty.Counter;
      PatternProperty.Counter++;
      break;

    case FLICKER_DOT:
      Flicker2Dot();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,flicker2Dot", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      FlickerIndex = PatternProperty.Counter;
      PatternProperty.Counter++;
      break;

    case COLORBARV:
      Img_ColorBarV();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,vertical colorbar", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case COLORBARH:
      Img_ColorBar();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,horizontal colorbar", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case GRAYLEVEL_V:
      Img_Gray256_V(256);
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,vertical gray level", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case GRAYLEVEL_H:
      Img_Gray256_H(256);
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,horizontal gray level", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case CROSSTALK:
      Img_CT();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,crosstalk", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case CHESSBOARD:
      Img_Chcker58();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,chessboard", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case RGBBAR:
      RGBBar();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,rgbbar", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case RGBLEVEL:
      RGBLevel();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,rgblevel", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case FRAME:
      Img_Box();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,frame", PatternProperty.Counter);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case PIC:
      Res_ReadPic((char *)(p + i));

      snprintf(PatternProperty.Name[PatternProperty.Counter], PATTERN_NAME_LEN, "%d,PIC %s", PatternProperty.Counter, (char *)(p + i));
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      i += strlen((char *)p + i);
      i++; // 跳过结束符
      break;

    case PATTERN_END:
      is_pattern = 0;
      break;

    case PATTERN_STAY:
      stay_time = *(p + i++);
      stay_time = (stay_time << 8) | *(p + i++);

      PatternProperty.ForceStay[PatternProperty.Counter - 1] = (bool)(stay_time>>15);

      stay_time &= 0x7fff;

      if (PatternProperty.Counter != 0)
      {
        PatternProperty.StayTime[PatternProperty.Counter - 1] = stay_time;
      }
      is_pattern = 0;
      break;

    case SLEEP_IN:
      Img_Full(0xff, 0xff, 0xff);
      sprintf(PatternProperty.Name[PatternProperty.Counter], "SleepIn");
      PatternProperty.Data[PatternProperty.Counter] = 0xff000000;
      PatternProperty.Counter++;
      is_pattern = 0;
      break;

    case SLEEP_OUT:
      Img_Full(0xff, 0xff, 0xff);
      sprintf(PatternProperty.Name[PatternProperty.Counter], "SleepOut");
      PatternProperty.Data[PatternProperty.Counter] = 0xff000001;
      PatternProperty.Counter++;
      is_pattern = 0;
      break;

    case GRAYLEVEL_V_USER:
      level = *(uint16_t *)(p + i);
      i    += 2;
      Img_Gray256_V(level);
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,vertical gray level,%d", PatternProperty.Counter, level);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    case GRAYLEVEL_H_USER:
      level = *(uint16_t *)(p + i);
      i    += 2;
      Img_Gray256_H(level);
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,horizontal gray level,%d", PatternProperty.Counter, level);
      PatternProperty.Data[PatternProperty.Counter] = PatternProperty.SDRAMCounter++;
      PatternProperty.Counter++;
      break;

    default:
      UserPrintf("Error:pattern syntax error\n");
      break;
    }

    if (TaskID != ACTION_NULL)
    {
      break;
    }

    if (is_pattern)
    {
      HAL_Delay(500);
    }
  }
  FPGA_WRITE_CMD(0x1B);
  FPGA_WRITE_DATA(0xAA);
  FPGA_WRITE_DATA(0xAA);
  FPGA_WRITE_DATA(0xAA);
  LcdDrvSetPattern();
  Img_Full(0xff, 0xff, 0xff);
  LcdDrvShowPattern(0);
}


uint8_t IsStayTimeOver(uint8_t frame)
{
  if (PatternProperty.StayTime[frame] == 0)
  {
    return 0;
  }

  if (htim3.Instance->CNT > PatternProperty.StayTime[frame] * 10)
  {
    htim3.Instance->CNT = 0;
    return 1;
  }

  return 0;
}


void ResetStayTimeCounter(void)
{
  htim3.Instance->CNT = 0;
}


void ResetMipiLcd(void)
{
  HAL_GPIO_WritePin(LS245_OE_GPIO_Port, LS245_OE_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(MIPIRESET_GPIO_Port, MIPIRESET_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(MIPIRESET_GPIO_Port, MIPIRESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(MIPIRESET_GPIO_Port, MIPIRESET_Pin, GPIO_PIN_SET);
  HAL_Delay(120);
}


static void ResetLcdByDefault(void)
{
  HAL_Delay(10);
  HAL_GPIO_WritePin(ResetPortPin.gpio_port, ResetPortPin.gpio_pin, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(ResetPortPin.gpio_port, ResetPortPin.gpio_pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(ResetPortPin.gpio_port, ResetPortPin.gpio_pin, GPIO_PIN_SET);
  HAL_Delay(120);
}


void PowerOff(void)
{
  if (SystemConfig.PowerSettings != 0)
  {
    SetLcdPower(OFF);
  }
  else
  {
    SetLcdPowerByUser(OFF);
  }
}


void PowerOn(void)
{
  if (SystemConfig.PowerSettings != 0)
  {
    SetLcdPower(ON);
  }
  else
  {
    SetLcdPowerByUser(ON);
  }
}


void Lcd_ReInit(void)
{
  GREEN_LIGHT_OFF;
  RED_LIGHT_ON;
  memset(&PatternProperty, 0, sizeof(PatternProperty));
  HAL_GPIO_WritePin(MIPIRESET_GPIO_Port, MIPIRESET_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LS245_OE_GPIO_Port, LS245_OE_Pin, GPIO_PIN_RESET);
  SetLcdTiming();
  FontScale = SystemConfig.FontScale;
  if((FontScale<1)||(FontScale>10))
  {
    FontScale = 1;
  }

  if (SystemConfig.LcdType == MIPI_LCD)
  {
    ResetPortPin.gpio_port = MIPIRESET_GPIO_Port;
    ResetPortPin.gpio_pin  = MIPIRESET_Pin;
  }
  else
  {
    ResetPortPin.gpio_port = GPIOE;
    ResetPortPin.gpio_pin  = GPIO_PIN_6;
  }

  //default settings
  if (SystemConfig.PowerSettings != 0)
  {
    HAL_Delay(10);
    SetLcdPower(OFF);
    HAL_Delay(10);
    SetLcdPower(ON);
    ResetLcdByDefault();
  }
  else
  {
    closeAllPower();
    SetLcdPowerByUser(ON);
  }

  switch (SystemConfig.LcdType)
  {
  case MIPI_LCD:
    SetMipiPara();
    SetMipiLcdInitCode();
    SSD2828_SetMode(VD);
    break;

  case RGB_SPI16BIT:
    SetRGBSPI16BitLcdInitCode();
    break;

  case RGB_SPI8BIT:
  case RGB_SPI9BIT:
    SetRGBSPI8Or9BitLcdInitCode();

    break;

  case LVDS_888_VESA:
    LcdDrvEnterLVDS(1);
    break;

  case LVDS_666_VESA:
    LcdDrvEnterLVDS(2);
    break;

  case LVDS_888_JEIDA:
    LcdDrvEnterLVDS(3);
    break;

  case LVDS_666_JEIDA:
    LcdDrvEnterLVDS(4);
    break;

  default:
    UserPrintf("Error:LCD type definition error!\n");
    break;
  }
  LcdDrvOpenRGB();
  Power_SetBLCurrent(SystemConfig.Backlight);

  //RGB_SPI_Test();
  SetPattern();
  GREEN_LIGHT_ON;
  RED_LIGHT_OFF;
  //	LcdDrvShowPattern(1);
}


__weak void OpInLPMode(void)
{
}


void Lcd_LightOn(void)
{
  //default settings
  if (SystemConfig.PowerSettings != 0)
  {
    HAL_Delay(10);
    SetLcdPower(OFF);
    HAL_Delay(10);
    SetLcdPower(ON);
    ResetLcdByDefault();
  }
  else
  {
    closeAllPower();
    SetLcdPowerByUser(ON);
  }

  if (SystemConfig.LcdType == MIPI_LCD)
  {
    SetMipiPara();
    SetMipiLcdInitCode();
    OpInLPMode();
    SSD2828_SetMode(VD);
  }
  else if (SystemConfig.LcdType == RGB_SPI16BIT)
  {
    SetRGBSPI16BitLcdInitCode();
  }
  else if ((SystemConfig.LcdType == RGB_SPI8BIT) || (SystemConfig.LcdType == RGB_SPI9BIT))
  {
    SetRGBSPI8Or9BitLcdInitCode();
  }
  else
  {
    UserPrintf("Error:LCD type definition error!\n");
  }
  LcdDrvOpenRGB();
  Power_SetBLCurrent(SystemConfig.Backlight);

  if (ShowIDPattern != 0xff)
  {
    uint16_t times = 0;
    ShowID();
    while (HAL_GPIO_ReadPin(GPIOE, KEY_DOWN_Pin) == GPIO_PIN_SET)
    {
      times++;
      if (times > 1000)
      {
        break;
      }
      HAL_Delay(1);
    }
    while (HAL_GPIO_ReadPin(GPIOE, KEY_DOWN_Pin) == GPIO_PIN_RESET)
    {
    }
  }
}


void MipiLcdSleepIn(void)
{
  SSD2828_SetMode(LP);
  SSD2828WriteReg(0x00b7, 0x01, 0x02);
  Power_SetBLCurrent(0);
  SSD2828_DcsShortWrite(1);
  SSD2828WriteData(0x28);
  HAL_Delay(20);

  SSD2828_DcsShortWrite(1);
  SSD2828WriteData(0x10);
  HAL_Delay(100);
}


void MipiLcdSleepOut(void)
{
  SSD2828_DcsShortWrite(1);
  SSD2828WriteData(0x11);
  HAL_Delay(120);

  SSD2828_DcsShortWrite(1);
  SSD2828WriteData(0x29);
  HAL_Delay(120);
  Power_SetBLCurrent(SystemConfig.Backlight);
}


/*
 * static void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
 * {
 * LcdDrvSetXY(x, y);
 * LcdDrvWriteData(color >> 8);
 * LcdDrvWriteData(color);
 * }
 */


void LCD_SetFontScale(uint8_t scale)
{
  FontScale = scale;
}


void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t chars)
{
  uint8_t temp = 0x01;
  uint8_t pos, t, i,j;

  chars = chars - ' ';

  for (pos = 0; pos < 64; pos += 2)
  {
    for (i = 0; i < FontScale; i++)
    {
      LcdDrvSetXY(x, y);
      temp = asc2_3216[chars][pos];
      for (t = 0; t < 8; t++)
      {
        for (j = 0; j < FontScale; j++)
        {
          if (temp & 0x01)
          {
            LcdDrvWriteData(FontColor.Fore >> 16);
            LcdDrvWriteData(FontColor.Fore >> 8);
            LcdDrvWriteData(FontColor.Fore);
          }
          else
          {
            LcdDrvWriteData(FontColor.Background >> 16);
            LcdDrvWriteData(FontColor.Background >> 8);
            LcdDrvWriteData(FontColor.Background);
          }
        }
        temp >>= 1;
      }
      temp = asc2_3216[chars][pos + 1];
      for (t = 8; t < 16; t++)
      {
        for (j = 0; j < FontScale; j++)
        {
          if (temp & 0x01)
          {
            LcdDrvWriteData(FontColor.Fore >> 16);
            LcdDrvWriteData(FontColor.Fore >> 8);
            LcdDrvWriteData(FontColor.Fore);
          }
          else
          {
            LcdDrvWriteData(FontColor.Background >> 16);
            LcdDrvWriteData(FontColor.Background >> 8);
            LcdDrvWriteData(FontColor.Background);
          }
        }       
        temp >>= 1;
      }
      y++;
    }
  }
  IsNeedClearBg = true;
}


static uint16_t CurrX = 0,CurrY = 0;
static uint16_t StartX = 0,StartY = 0;
static char StrBuffer[64];
static uint8_t MaxCharLen = 0;

void LCD_PrintfSetAddress(uint16_t x , uint16_t y)
{
  CurrX = x;
  CurrY = y;
  StartX = x;
  StartY = y;
  MaxCharLen = 0;
}

void LCD_GetCurrAddress(uint16_t *x , uint16_t *y)
{
  *x = CurrX;
  *y = CurrY;
}

void LCD_SetCurrAddress(uint16_t x , uint16_t y)
{
  CurrX = x;
  CurrY = y;
}

void LCD_ClearLine(void)
{
  CurrX = 0;
}

int LCD_Printf(char *fmt, ...)
{
  va_list args;
  uint32_t len;
  char *p = StrBuffer;
  memset(StrBuffer,0,sizeof(StrBuffer));

  va_start(args, fmt);
  len = vsnprintf(StrBuffer, sizeof(StrBuffer), fmt, args);
  va_end(args);

  while (*p != '\0')
  {
    if(*p == '\n')
    {
      CurrY += 32*FontScale;
      CurrX = StartX;
    }
    else
    {
      LCD_ShowChar(CurrX, CurrY, *p);
      CurrX = CurrX + 16*FontScale;
    }
		p++;
  }
  if(len > MaxCharLen)
  {
    MaxCharLen = len;
  }

  return len;
}

void LCD_ClearPrintf(void)
{
  uint16_t i, j;
  uint8_t amount = FindSDRAMPatternAmount();
  uint8_t max_line = MaxCharLen*16*FontScale;

  LcdDrvShowPattern(amount);
  LcdDrvSetCharIndex(amount);
  for (j = StartY; j < CurrY; j++)
  {
    LcdDrvSetXY(StartX, j);
    for (i = StartX; i < max_line; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
  }
  LcdDrvSetCharIndex(amount);
  LCD_PrintfSetAddress(0,0);
}


void LCD_ShowString(uint16_t x, uint16_t y, const char *p)
{
  while (*p != '\0')
  {
    LCD_ShowChar(x, y, *p);
    x = x + 16*FontScale;
    p++;
  }
}


void static Turning(void)
{
  if (strcmp(PatternProperty.Name[PatternProperty.CurrentPattern], "SleepOut") == 0)
  {
    MipiLcdSleepOut();
  }
  else if (strcmp(PatternProperty.Name[PatternProperty.CurrentPattern], "SleepIn") == 0)
  {
    MipiLcdSleepIn();
  }
  else
  {
    LcdDrvShowPattern(PatternProperty.Data[PatternProperty.CurrentPattern]);
  }
}


void PageTurning(PageTurningTypeDef page)
{
  if (page == PAGE_UP)
  {
    if (PatternProperty.CurrentPattern == PatternProperty.Counter - 1)
    {
      PatternProperty.CurrentPattern = 0;
    }
    else
    {
      PatternProperty.CurrentPattern++;
    }
  }
  else
  {
    if (PatternProperty.CurrentPattern == 0)
    {
      PatternProperty.CurrentPattern = PatternProperty.Counter - 1;
    }
    else
    {
      PatternProperty.CurrentPattern--;
    }
  }
  Turning();
}


static uint8_t FindSDRAMPatternAmount(void)
{
  uint8_t i;
  uint8_t counter = 0;

  for (i = 0; i < PatternProperty.Counter; i++)
  {
    uint32_t data = PatternProperty.Data[i];
    if ((data & FPGA_IO_MODE) != FPGA_IO_MODE)
    {
      counter++;
    }
  }

  return counter;
}

void EnterBg(void)
{
  uint8_t amount = FindSDRAMPatternAmount();

  LcdDrvShowPattern(amount);
  LcdDrvSetCharIndex(amount);
}

void PrepareBg(void)
{
  uint16_t i, j;
  uint8_t amount = FindSDRAMPatternAmount();

  LcdDrvShowPattern(amount);
  LcdDrvSetCharIndex(amount);
  for (j = 0; j < LCDTiming.LCDV; j++)
  {
    LcdDrvSetXY(0, j);
    for (i = 0; i < LCDTiming.LCDH; i++)
    {
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
      LcdDrvWriteData(0xff);
    }
  }
  LcdDrvSetCharIndex(amount);
}


static FlickertypeDef FlickerType = F_COLUMN;

void LCD_SetFlickerType(FlickertypeDef type)
{
  FlickerType = type;
}


void LCD_EraseFlickerString(void)
{
  uint16_t i, j;

  if (FlickerIndex == 0xff)
  {
    return;
  }

  LcdDrvSetCharIndex(PatternProperty.Data[FlickerIndex]);

  if (FlickerType == F_COLUMN)
  {
    for (j = 0; j < LCDTiming.LCDV / 4; j++)
    {
      LcdDrvSetXY(0, j);
      for (i = 0; i < LCDTiming.LCDH / 4; i++)
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
  else
  {
    uint16_t quarter_h = LCDTiming.LCDH / 4;
    uint16_t quarter_v = LCDTiming.LCDV / 8;

    for (j = 0; j < quarter_v; j++)
    {
      LcdDrvSetXY(0, j);
      for (i = 0; i < quarter_h; i++)
      {
        LcdDrvWriteData(0x80);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x80);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x80);
        LcdDrvWriteData(0x00);
      }

      for (i = 0; i < quarter_h; i++)
      {
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x80);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x80);
        LcdDrvWriteData(0x00);
        LcdDrvWriteData(0x80);
      }
    }
  }
}


extern TIM_HandleTypeDef htim2;
static uint8_t uhCaptureIndex = 0;
static uint32_t uwIC2Value1 ,uwIC2Value2 , uwDiffCapture = 1,uwFrequency;
static bool IsCaptureFinish = false;
static uint32_t OverflowTimes = 0;
uint32_t AvgData[10];
uint16_t GetFrequence(void)
{
  uint32_t avg = 0;
  uint32_t times = 0;
  uint32_t tick = HAL_GetTick();
  while(HAL_GetTick() - tick < 500)
  {
    if(IsCaptureFinish)
    {
      IsCaptureFinish = false;
      OverflowTimes = 0;
      AvgData[times] = uwFrequency;
      avg+=uwFrequency;
      times++;
      if(times == 10)
      {
        return avg/times;
      }
    }
  }
  return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  OverflowTimes++;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    if(uhCaptureIndex == 0)
    {
      uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
      uhCaptureIndex = 1;
    }
    else if(uhCaptureIndex == 1)
    {
      uwIC2Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); 
      if (uwIC2Value2 > uwIC2Value1)
      {
        uwDiffCapture = (uwIC2Value2 - uwIC2Value1); 
      }
      else if (uwIC2Value2 < uwIC2Value1)
      {
        uwDiffCapture = ((0xFFFF - uwIC2Value1) + uwIC2Value2) + 1;
      }   
      uwFrequency = HAL_RCC_GetHCLKFreq() / uwDiffCapture;
			IsCaptureFinish = true;
      uhCaptureIndex = 0;
    }
  }
  else
  {
    if(uhCaptureIndex == 0)
    {
      uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
      uhCaptureIndex = 1;
      OverflowTimes = 0;
    }
    else if(uhCaptureIndex == 1)
    {
      uwIC2Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); 
      uwDiffCapture = ((0xFFFF*OverflowTimes - uwIC2Value1) + uwIC2Value2) + 1;
      uwFrequency = HAL_RCC_GetHCLKFreq() / uwDiffCapture;
			IsCaptureFinish = true;
      uhCaptureIndex = 0;
    }
  }
}

bool InspectionAfterPowerOn(void)
{
  bool result[4] = {true,true,true,true};
  bool total_result = true;
	
  if((SystemConfig.HardwareID[0] != 0) || (SystemConfig.HardwareID[1] != 0))
  {
    if((GetIDVol() < SystemConfig.HardwareID[0]) || (GetIDVol() > SystemConfig.HardwareID[1]))
    {
        result[0] = false;
        total_result  = false;
    }
		UserPrintf("ID voltage is:%d\n",GetIDVol());
  }

	uhCaptureIndex = 0;
  OverflowTimes = 0;
  __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_Delay(100);
	IsCaptureFinish = false;
  if((SystemConfig.TE[0] != 0) || (SystemConfig.TE[1] != 0))
  {
    uint32_t te = GetFrequence();
    if(( te< SystemConfig.TE[0]) || (te> SystemConfig.TE[1]))
    {
        result[1] = false;
        total_result = false;
    }
		UserPrintf("TE:%d HZ \n",te);
  }
  __HAL_TIM_DISABLE_IT(&htim2, TIM_IT_UPDATE);
	HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);

	uhCaptureIndex = 0;
  OverflowTimes = 0;
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
	HAL_Delay(100);
	IsCaptureFinish = false;
  if((SystemConfig.PWM[0] != 0) || (SystemConfig.PWM[1] != 0))
  {
    uint32_t pwm = GetFrequence();
    if((pwm < SystemConfig.PWM[0]) || (pwm > SystemConfig.PWM[1]))
    {
        total_result = false;
        result[2] = false;
    }
		UserPrintf("PWM:%d HZ \n",pwm);		
  }
	HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_2);

  if(SystemConfig.ReadBack[0] != 0)
  {
    for(int i=0;i<SystemConfig.ReadBack[0];i++)
    {
      if(SystemConfig.ReadBack[i+1] > 0xff)
      {
        uint8_t low = SystemConfig.ReadBack[i+1] & 0xff;
        uint8_t high = (SystemConfig.ReadBack[i+1] >> 8) & 0xff;
        if((ReadBackValue[i] < low) || (ReadBackValue[i] > high)) 
        {
          result[3] = false;
          total_result = false;
        }
      }
      else if(SystemConfig.ReadBack[i+1] != ReadBackValue[i])
      {
        result[3] = false;
				total_result = false;
      }
    }
  }
	
  if(total_result == false)
  {
    EnterBg();
		LcdDrvSetCharIndex(FindSDRAMPatternAmount());
    SetFontColor(0xff0000);
    LCD_ClearPrintf();
    
    if(result[0] == false)
    {
        LCD_Printf("ID NG\n");
    }
    if(result[1] == false)
    {
        LCD_Printf("TE NG\n");
    }
    if(result[2] == false)
    {
        LCD_Printf("PWM NG\n");
    }
    if(result[3] == false)
    {
        LCD_Printf("Read NG\n");
    }
    SetFontColor(0);    
  }
	

  return total_result;

}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
