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

void SetLcdPower(StateTypeDef state)
{
  PowerTypeDef power_list[POWER_AMOUT] = { POWER_1V8, POWER_2V8, POWER_3V3, POWER_P5V, POWER_N5V };
  uint8_t i = 0;

  if (state == ON)
  {
    //顺序开启电源
    for (i = 0; i < POWER_AMOUT; i++)
    {
      if (((SystemConfig.PowerSettings >> i) & 0x01) == 0x01)
      {
        Power_SetState(power_list[i], ON);
        HAL_Delay(50);
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
        Power_SetState(power_list[i - 1], OFF);
        HAL_Delay(50);
      }
    }
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
  uint8_t lane = SystemConfig.MIPIConfig[3];
  uint16_t speed = (SystemConfig.MIPIConfig[1] << 8) | SystemConfig.MIPIConfig[2];

  SSD2828_Init(lane, speed);
}


/**
 * @brief  解析并设定LCD初始化
 * @note   头两个字节为整个initcode的长度
 *          后面为MIPI_TYPE + LEN + DATA
 * @param  None
 * @retval None
 */
void SetLcdInitCode(void)
{
  uint16_t code_size = (SystemConfig.LCDInitCode[0] << 8) | SystemConfig.LCDInitCode[1];
  uint8_t *p = &SystemConfig.LCDInitCode[2];
  uint8_t para_amount = 0;
  uint8_t para;
  MipiPackageDef package = NO_PACKAGE;
  uint16_t i = 0, j = 0;
  uint16_t delay_time;
  MIPI_ReadTypeDef result;
  uint8_t buffer[32];

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
          SSD2828_DcsShortWrite(para_amount);
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
          SSD2828_GenericShortWrite(para_amount);
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
      para = *(p + i++);
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
        UserPrintf("Info:read 0x%x\n", para);
        for (j = 0; j < para_amount; j++)
        {
          UserPrintf("para%d = 0x%x\n", j + 1, buffer[j]);
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


void SetPattern(void)
{
  uint16_t size = (SystemConfig.Pattern[0] << 8) | SystemConfig.Pattern[1];
  uint8_t *p = &SystemConfig.Pattern[2];
  uint16_t i = 0;
  uint8_t r, g, b;
  uint16_t stay_time;

  memset(&PatternProperty, 0, sizeof(PatternProperty));

  LcdDrvSetPattern();
  while (i < size)
  {
    if (PatternProperty.Counter > PATTERN_AMOUNT)
    {
      break;
    }

    switch ((PatternTypeDef) * (p + i++))
    {
    case PATTERN_START:

      break;

    case RGB:
      r = *(p + i++);
      g = *(p + i++);
      b = *(p + i++);
      Img_Full(r, g, b);
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,RGB(0x%x,%x,%x)", PatternProperty.Counter, r, g, b);
      PatternProperty.Counter++;
      break;

    case FLICKERV:
      Flicker();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,flicker", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case FLICKERH:
      Flicker();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,flicker", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case COLORBARV:
      Img_ColorBarV();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,vertical colorbar", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case COLORBARH:
      Img_ColorBar();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,horizontal colorbar", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case GRAYLEVEL_V:
      Img_Gray256_V();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,vertical gray level", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case GRAYLEVEL_H:
      Img_Gray256_H();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,horizontal gray level", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case CROSSTALK:
      Img_CT();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,crosstalk", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case CHESSBOARD:
      Img_Chcker58();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,chessboard", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case RGBBAR:
      RGBBar();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,rgbbar", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case RGBLEVEL:
      RGBLevel();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,rgblevel", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case FRAME:
      Img_Box();
      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,frame", PatternProperty.Counter);
      PatternProperty.Counter++;
      break;

    case PIC:
      Res_ReadPic((char *)(p + i));

      sprintf(PatternProperty.Name[PatternProperty.Counter], "%d,PIC %s", PatternProperty.Counter, (char *)(p + i));
      PatternProperty.Counter++;
      i += strlen((char *)p + i);
      break;


    case PATTERN_END:
      break;

    case PATTERN_STAY:
      stay_time = *(p + i++);
      stay_time = (stay_time << 8) | *(p + i++);
      if (PatternProperty.Counter != 0)
      {
        PatternProperty.StayTime[PatternProperty.Counter - 1] = stay_time;
      }
      break;

    default:
      UserPrintf("Error:pattern syntax error\n");
      break;
    }

    if (TaskID != ACTION_NULL)
    {
      break;
    }

    HAL_Delay(500);
  }

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


void ResetLcd(void)
{
  HAL_GPIO_WritePin(LS245_OE_GPIO_Port, LS245_OE_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(MIPIRESET_GPIO_Port, MIPIRESET_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(MIPIRESET_GPIO_Port, MIPIRESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(120);
  HAL_GPIO_WritePin(MIPIRESET_GPIO_Port, MIPIRESET_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
}


void Lcd_ReInit(void)
{
  memset(&PatternProperty, 0, sizeof(PatternProperty));
  SetLcdPower(OFF);
  SetLcdPower(ON);
  SetLcdTiming();
  SetMipiPara();
  ResetLcd();
  SetLcdInitCode();
  Power_SetBLCurrent(SystemConfig.Backlight);
  LcdDrvOpenRGB();
  SSD2828_SetMode(VD);
	RGB_SPI_Test();
  SetPattern();
}


void Lcd_Sleep(void)
{
  SSD2828_DcsShortWrite(1);
  SSD2828WriteData(0x11);
}


void Lcd_LightOn(void)
{
  SetLcdPower(ON);
  SetLcdTiming();
  SetMipiPara();
  ResetLcd();
  SetLcdInitCode();
  Power_SetBLCurrent(SystemConfig.Backlight);
  LcdDrvOpenRGB();
  SSD2828_SetMode(VD);
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
