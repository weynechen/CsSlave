/**
 * @file         tp.c
 * @author       Weyne Chen
 * @version      V01
 * @date         2017.08.03
 * @brief
 * @note
 * @attention    COPYRIGHT WEYNE
 **/

#include "tp.h"
#include "lcd.h"
#include "fsmc.h"

static uint16_t HeartBeats = 0;
static uint8_t WaitTest    = 0;
static uint8_t TestResult  = 0;

typedef enum
{
  TP_DOWN,
  TP_UP,
  TP_CONTACT,
} TPEventTypeDef;

// __packed typedef struct
// {
//   uint8_t  event;
//   uint8_t  id;
//   uint16_t x;
//   uint16_t y;
// } TPCoorTypeDef;

__packed typedef struct
{
  uint8_t event;
  uint8_t id;
  uint8_t xh;
  uint8_t xl;
  uint8_t yh;
  uint8_t yl;
} TPCoorTypeDef;

static TPCoorTypeDef Coordinate[5];
static bool TPToggle = false;

void TP_Callback(PproTypeDef *data)
{
  char s[2];
  uint8_t echo;

  switch (data->PackageID)
  {
  case TP_ECHO:
    echo = *data->Data;
    s[1] = echo % 10 + '0';
    s[0] = echo / 10 + '0';
    //LCD_ShowString(0, 32, s);
    LCD_ShowChar(0, 32, s[0]);
    LCD_ShowChar(16, 32, s[1]);
    HeartBeats = 1;
    break;

  case TP_RESULT:
    WaitTest   = 1;
    TestResult = 0;
    if (*data->Data == 'O')
    {
      if (*(data->Data + 1) == 'K')
      {
        TestResult = 1;
      }
    }
    break;

  case TP_NO_FILE:
    WaitTest   = 1;
    TestResult = 0;
    break;

  case TP_COORDINATE:
    memcpy(Coordinate, data->Data - 1, sizeof(Coordinate));
    TPToggle = true;
    break;

  default:
    break;
  }
}


bool IsTPToggle(void)
{
  if (TPToggle)
  {
    TPToggle = false;
    return true;
  }
  else
  {
    return false;
  }
}


static void TP_SendData(uint8_t pid, uint16_t data)
{
  Ppro_SendData(TP, pid, data);
}


uint8_t TP_StartTest(void)
{
  /*定义总的测试时间，不能测试太久(ms)*/
  uint16_t timeout = 60000;

  /*心跳超时时间(ms)*/
  HeartBeats = 1500;
  WaitTest   = 0;
  TP_SendData(TP_START, 0x5453);

  /*等待测试结果*/
  while (WaitTest == 0)
  {
    /*若收到echo，HeartBeats会刷新*/
    HeartBeats--;
    /*超时未收到echo*/
    if (HeartBeats == 0)
    {
      //return 0;
    }
    HAL_Delay(1);

    timeout--;
    if (timeout == 0)
    {
      return 0;
    }
  }

  if (TestResult == 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}


static void DrawPointer(uint32_t color)
{
  LcdDrvWriteData(color >> 16);
  LcdDrvWriteData(color >> 8);
  LcdDrvWriteData((uint8_t)color);
}


const uint32_t Palette [5] = { 0xff000, 0xff00, 0xff, 0xffff00, 0xffff };

void TP_DrawLine(void)
{
  uint16_t x;
  uint16_t y;

  for (int i = 0; i < 5; i++)
  {
    if (Coordinate[i].event == 2)
    {
      x = (uint16_t)Coordinate[i].xh << 8 | Coordinate[i].xl;
      y = (uint16_t)Coordinate[i].yh << 8 | Coordinate[i].yl;
      
      for(int j=0;j<10;j++)
      {
        LcdDrvSetXY(x, y+j);
        DrawPointer(Palette [i]);
        DrawPointer(Palette [i]);
        DrawPointer(Palette [i]);
        DrawPointer(Palette [i]);
        DrawPointer(Palette [i]);
        
      }
    }
  }
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
