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
#include "fpga.h"
#include "math.h"
#include "string.h"

static uint16_t HeartBeats = 0;
static uint8_t QuitTest    = 0;
static uint8_t TestResult  = 0;
static bool IsCell         = true;

typedef struct
{
  uint16_t x;
  uint16_t y;
} Point;

static uint32_t CellStepH;
static uint32_t CellStepV;
static uint32_t CellStepHRem;
static uint32_t CellStepVRem;
static Point LastCoor[5];
static uint16_t LineWeight        = 1;
static const uint32_t Palette [5] = { 0xff0000, 0xff00, 0xff, 0xffff00, 0xffff };
#define CELL_AMOUNT    ((CELL_DIV_H + CELL_DIV_V) * 3)
static Point CellArrayCoor[CELL_AMOUNT];
static uint16_t CellCounter = 0;

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

#define FINGER_AMOUNT    5

typedef struct
{
  TPCoorTypeDef coor[FINGER_AMOUNT];
} FingerCoorTypeDef;

#define QUEUE_AMOUNT    10
static FingerCoorTypeDef *Coordinate;
static FingerCoorTypeDef FingerCoor[QUEUE_AMOUNT];

typedef struct
{
  FingerCoorTypeDef *head;
  FingerCoorTypeDef *tail;
  FingerCoorTypeDef *pool_head;
  FingerCoorTypeDef *pool_tail;
} FingerCoorQueueTypeDef;

FingerCoorQueueTypeDef FingerCoorQueue = { NULL, NULL, NULL, NULL };

void QueueInit(void)
{
  FingerCoorQueue.pool_head = FingerCoor;
  FingerCoorQueue.pool_tail = &FingerCoor[QUEUE_AMOUNT];
  FingerCoorQueue.head      = FingerCoorQueue.tail = FingerCoorQueue.pool_head;
}


bool Enqueue(FingerCoorTypeDef finger_coor)
{
  if (FingerCoorQueue.tail == NULL)
  {
    QueueInit();
  }
  //circle
  if (FingerCoorQueue.tail == FingerCoorQueue.pool_tail)
  {
    FingerCoorQueue.tail = FingerCoorQueue.pool_head;
  }

  *FingerCoorQueue.tail++ = finger_coor;

  return true;
}


FingerCoorTypeDef *Dequeue(void)
{
  FingerCoorTypeDef *finger_coor;

  if (FingerCoorQueue.head == NULL)
  {
    QueueInit();
  }

  if (FingerCoorQueue.head == FingerCoorQueue.tail)
  {
    finger_coor = NULL;
  }
  else
  {
    if (FingerCoorQueue.head == FingerCoorQueue.pool_tail)
    {
      FingerCoorQueue.head = FingerCoorQueue.pool_head;
    }
    finger_coor = FingerCoorQueue.head;
    FingerCoorQueue.head++;
  }

  return finger_coor;
}


const char *TESTNAME1  = "Start...";
const char *TESTNAME2  = "Download boot";
const char *TESTNAME3  = "Download Bin";
const char *TESTNAME4  = "FW version test";
const char *TESTNAME5  = "ID test";
const char *TESTNAME6  = "IC version test";
const char *TESTNAME7  = "Project code test";
const char *TESTNAME8  = "Channel num test";
const char *TESTNAME9  = "Short circuit test";
const char *TESTNAME10 = "Open test";
const char *TESTNAME11 = "CB test";
const char *TESTNAME12 = "Raw data test";
const char *TESTNAME13 = "Reset pin test";
const char *TESTNAME14 = "Int pin test";
const char *TESTNAME15 = "Noise test";
const char *TESTNAME16 = "LCD noise test";
const char *TESTNAME17 = "SNR test";
const char *TESTNAME18 = "Differ test";
const char *TESTNAME19 = "Differ uniformity test";
const char *TESTNAME20 = "LPWG raw data test";
const char *TESTNAME21 = "LPWG noise test";


#define ECHO_AMOUNT    21
static struct ECHOLIST
{
  uint8_t    index;
  const char *text;
  bool       result;
}
EchoList[ECHO_AMOUNT];

static uint8_t EchoNumber;

static void InitEcho()
{
  EchoList[0].text  = TESTNAME1;
  EchoList[1].text  = TESTNAME2;
  EchoList[2].text  = TESTNAME3;
  EchoList[3].text  = TESTNAME4;
  EchoList[4].text  = TESTNAME5;
  EchoList[5].text  = TESTNAME6;
  EchoList[6].text  = TESTNAME7;
  EchoList[7].text  = TESTNAME8;
  EchoList[8].text  = TESTNAME9;
  EchoList[9].text  = TESTNAME10;
  EchoList[10].text = TESTNAME11;
  EchoList[11].text = TESTNAME12;
  EchoList[12].text = TESTNAME13;
  EchoList[13].text = TESTNAME14;
  EchoList[14].text = TESTNAME15;
  EchoList[15].text = TESTNAME16;
  EchoList[16].text = TESTNAME17;
  EchoList[17].text = TESTNAME18;
  EchoList[18].text = TESTNAME19;
  EchoList[19].text = TESTNAME20;
  EchoList[20].text = TESTNAME21;

  EchoList[0].index  = 0x01;
  EchoList[1].index  = 0x02;
  EchoList[2].index  = 0x03;
  EchoList[3].index  = 0x20;
  EchoList[4].index  = 0x21;
  EchoList[5].index  = 0x22;
  EchoList[6].index  = 0x23;
  EchoList[7].index  = 0x24;
  EchoList[8].index  = 0x25;
  EchoList[9].index  = 0x26;
  EchoList[10].index = 0x27;
  EchoList[11].index = 0x28;
  EchoList[12].index = 0x29;
  EchoList[13].index = 0x2a;
  EchoList[14].index = 0x2b;
  EchoList[15].index = 0x2c;
  EchoList[16].index = 0x2d;
  EchoList[17].index = 0x2e;
  EchoList[18].index = 0x2f;
  EchoList[19].index = 0x30;
  EchoList[20].index = 0x31;

  for (int i = 0; i < ECHO_AMOUNT; i++)
  {
    EchoList[i].result = true;
  }
  EchoNumber = 1;
}


uint16_t EchoCharLen = 0;

void ShowEcho(uint16_t data)
{
  uint8_t index  = (uint8_t)data;
  uint8_t result = data >> 8;

  for (int i = 0; i < ECHO_AMOUNT; i++)
  {
    if (EchoList[i].index == index)
    {
      if (result == 0x80)
      {
        SetFontColor(0xff0000);
        LCD_Printf("%d,%s -- NG\n", EchoNumber, EchoList[i].text);
      }
      else if (result == 0x00)
      {
        LCD_Printf("%d,%s -- OK\n", EchoNumber, EchoList[i].text);
      }
      else
      {
        EchoCharLen = LCD_Printf("%d,%s",EchoNumber, EchoList[i].text);
        LCD_ClearLine();
      }

      if(result != 0x01)
      {
        EchoNumber++;
      }

      SetFontColor(0);

      break;
    }
  }
}


#define HEART_BEATS_TIME    15000

void TP_Callback(PproTypeDef *data)
{
  char s[128];
  uint8_t byte;
  FingerCoorTypeDef finger_coor;
  uint16_t x, y;

  switch (data->PackageID)
  {
  case TP_ECHO:
#if FUN_DRAW_LINE
    HeartBeats = 1;
#else
    HeartBeats = HEART_BEATS_TIME;
    ShowEcho(*(uint16_t *)data->Data);
#endif
    break;

  case TP_RESULT:
    QuitTest   = 1;
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
    QuitTest   = 1;
    TestResult = 0;
    break;

  case TP_COORDINATE:
    memcpy(&finger_coor, data->Data, sizeof(finger_coor));
    Enqueue(finger_coor);
    break;

  case TP_PROGRESS:
#if FUN_DRAW_LINE
    byte = *data->Data;
    memset(s, 0, sizeof(s));
    sprintf(s, "Downloading:%d%%", byte);
    LCD_ShowString(0, 32 * FontScale, s);
#else
    LCD_GetCurrAddress(&x, &y);
    byte = *data->Data;
    memset(s, 0, sizeof(s));
    sprintf(s, ":%d%%", byte);
    LCD_ShowString(EchoCharLen * 16 * FontScale, y, s);
#endif
    HeartBeats = HEART_BEATS_TIME;
    break;

  case TP_FIRMWARE_VERSION:
#if FUN_DRAW_LINE
    byte = *data->Data;
    memset(s, 0, sizeof(s));
    sprintf(s, "FW:0x%X", byte);
    SetFontColor(0);
    LCD_ShowString(CellStepH + 2, CellStepV + 33 * FontScale, s);
#else
    //SetFontColor(0);
    LCD_GetCurrAddress(&x, &y);
    byte = *data->Data;
    memset(s, 0, sizeof(s));
    sprintf(s, "FW:0x%X", byte);
    LCD_ShowString(EchoCharLen * 16 * FontScale, y, s);
#endif
    break;

  default:
    break;
  }
}


bool IsTPToggle(void)
{
  FingerCoorTypeDef *x = Dequeue();

  if (x == NULL)
  {
    return false;
  }
  else
  {
    Coordinate = x;
  }
  return true;
}


void TP_SendData(uint8_t pid, uint16_t data)
{
  Ppro_SendData(TP, pid, data);
}


uint8_t TP_StartTest(void)
{
#ifdef FUN_DRAW_LINE
  /*定义总的测试时间，不能测试太久(ms)*/
  uint16_t timeout = 30000;
#else
  uint16_t timeout = 60000;
  LCD_PrintfSetAddress(1, 0);
#endif
  /*心跳超时时间(ms)*/
  HeartBeats = HEART_BEATS_TIME;
  QuitTest   = 0;
  TP_SendData(TP_START, 0x5453);
  CellStepH = LCDTiming.LCDH / CELL_DIV_H;
  CellStepV = LCDTiming.LCDV / CELL_DIV_V;
#ifdef FUN_DRAW_LINE
  LCD_ShowString(CellStepH, CellStepV, "        ");
  LCD_ShowString(0, 32 * FontScale, "                ");
#endif

  InitEcho();

  /*等待测试结果*/
  while (QuitTest == 0)
  {
    /*若收到echo，HeartBeats会刷新*/
    HeartBeats--;
    /*超时未收到echo*/
    if (HeartBeats == 0)
    {
      SetFontColor(0xff0000);
      LCD_Printf("no data in -- NG\n");
      SetFontColor(0);
      return 0;
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
    TP_SendData(TP_FIRMWARE_VERSION, 4);
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


static void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
  uint16_t t;
  int xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int incx, incy, uRow, uCol;

  delta_x = x2 - x1;
  delta_y = y2 - y1;
  uRow    = x1;
  uCol    = y1;
  if (delta_x > 0)
  {
    incx = 1; //设置单步方向
  }
  else if (delta_x == 0)
  {
    incx = 0; //垂直线
  }
  else
  {
    incx    = -1;
    delta_x = -delta_x;
  }
  if (delta_y > 0)
  {
    incy = 1;
  }
  else if (delta_y == 0)
  {
    incy = 0; //水平线
  }
  else
  {
    incy    = -1;
    delta_y = -delta_y;
  }
  if (delta_x > delta_y)
  {
    distance = delta_x; //选取基本增量坐标轴
  }
  else
  {
    distance = delta_y;
  }
  for (t = 0; t <= distance + 1; t++)     //画线输出
  {
    LcdDrvSetXY(uRow, uCol);
    for (int i = 0; i < LineWeight; i++)
    {
      LcdDrvSetXY(uRow, uCol + i);
      for (int j = 0; j < LineWeight; j++)
      {
        DrawPointer(color);
      }
    }
    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance)
    {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance)
    {
      yerr -= distance;
      uCol += incy;
    }
  }
}


static void DrawCellFrame(Point p)
{
  uint16_t div_h, div_v;

  if (p.x > LCDTiming.LCDH)
  {
    return;
  }

  if (p.y > LCDTiming.LCDV)
  {
    return;
  }

  if (p.x == CellStepH * CELL_DIV_H)
  {
    p.x  += CellStepHRem;
    div_h = CellStepH + CellStepHRem;
  }
  else
  {
    div_h = CellStepH;
  }

  if (p.y == CellStepV * CELL_DIV_V)
  {
    p.y  += CellStepVRem;
    div_v = CellStepV + CellStepVRem;
  }
  else
  {
    div_v = CellStepV;
  }

  CellArrayCoor[CellCounter++] = p;

  if ((p.y != 0) && (p.y <= LCDTiming.LCDV - CellStepV))
  {
    LCD_DrawLine(p.x - div_h, p.y, p.x, p.y, 0); //draw horizantol line
  }

  if ((p.x != 0) && (p.x <= LCDTiming.LCDH - CellStepH))
  {
    LCD_DrawLine(p.x, p.y - div_v, p.x, p.y, 0); // draw vertical line
  }
}


static void FillCell(Point p)
{
  Point s;
  uint16_t i, j;

  uint16_t stepx, stepy;

  stepx = (p.x > LCDTiming.LCDH - CellStepH) ? (CellStepH + CellStepHRem) : CellStepH;
  stepy = (p.y > LCDTiming.LCDV - CellStepV) ? (CellStepV + CellStepVRem) : CellStepV;

  s.x = p.x - stepx + 2;
  s.y = p.y - stepy + 1;

  for (j = 0; j < stepy - 1; j++)
  {
    LcdDrvSetXY(s.x, s.y + j);
    for (i = 0; i < stepx - 2; i++)
    {
      DrawPointer(0xff00);
    }
  }
}


static void DrawCellFrameArrayH(uint8_t index)
{
  Point p;

  p.x = CellStepH;
  p.y = CellStepV * index;

  for (int i = 1; i < CELL_DIV_H + 1; i++)
  {
    p.x = i * CellStepH;
    DrawCellFrame(p);
  }
}


static void DrawCellFrameArrayV(uint8_t index)
{
  Point p;

  p.x = CellStepH * index;
  p.y = CellStepV;

  for (int i = 1; i < CELL_DIV_V + 1; i++)
  {
    p.y = CellStepV * i;
    DrawCellFrame(p);
  }
}


void TP_DrawBG(void)
{
  CellCounter  = 0;
  CellStepH    = LCDTiming.LCDH / CELL_DIV_H;
  CellStepV    = LCDTiming.LCDV / CELL_DIV_V;
  CellStepHRem = LCDTiming.LCDH % CELL_DIV_H;
  CellStepVRem = LCDTiming.LCDV % CELL_DIV_V;

  DrawCellFrameArrayH(1);
  DrawCellFrameArrayH(7);
  DrawCellFrameArrayH(CELL_DIV_V);

  DrawCellFrameArrayV(1);
  DrawCellFrameArrayV(5);
  DrawCellFrameArrayV(CELL_DIV_H);

  LCD_DrawLine(0, CELL_DIV_V / 2 * CellStepV, LCDTiming.LCDH - 1, CELL_DIV_V / 2 * CellStepV, 0);
  LCD_DrawLine(0, CellStepV * (CELL_DIV_V - 1), LCDTiming.LCDH - 1, CellStepV * (CELL_DIV_V - 1), 0);

  LCD_DrawLine(CELL_DIV_H / 2 * CellStepH, 0, CELL_DIV_H / 2 * CellStepH, LCDTiming.LCDV - 1, 0);
  LCD_DrawLine(CellStepH * (CELL_DIV_H - 1), 0, CellStepH * (CELL_DIV_H - 1), LCDTiming.LCDV - 1, 0);
  for (int i = 0; i < 10; i++)
  {
    Dequeue();
  }
}


void SetCellOrLine(bool is_cell)
{
  IsCell = is_cell;
}


bool TP_DrawLine(void)
{
  uint16_t x, x1;
  uint16_t y, y1;
  static bool draw_over = true;
  bool result           = true;

  //UserPrintf("%d\n",Coordinate->coor[0].event);

  for (int i = 0; i < 5; i++)
  {
    if (Coordinate->coor[i].event == 2)
    {
      x = (uint16_t)(Coordinate->coor[i].xh) << 8 | (Coordinate->coor[i].xl);
      y = (uint16_t)(Coordinate->coor[i].yh) << 8 | (Coordinate->coor[i].yl);
      // UserPrintf("x:%d,y:%d\n", x, y);

      x1 = (x > (CellStepH * (CELL_DIV_H - 1))) ? LCDTiming.LCDH : (x / CellStepH + 1) * CellStepH;
      y1 = (y > CellStepV * (CELL_DIV_V - 1)) ? LCDTiming.LCDV : (y / CellStepV + 1) * CellStepV;
      if (IsCell)
      {
        for (int n = 0; n < CELL_AMOUNT; n++)
        {
          if ((x1 == CellArrayCoor[n].x) && (y1 == CellArrayCoor[n].y))
          {
            FillCell(CellArrayCoor[n]);
            CellArrayCoor[n].x = 0;
            CellArrayCoor[n].y = 0;
            break;
          }
        }
      }
      if (IsCell == false)
      {
        if (!draw_over)
        {
          LineWeight = 4;
          LCD_DrawLine(x, y, LastCoor[i].x, LastCoor[i].y, Palette[i]);
          LineWeight = 1;
        }
        else
        {
          draw_over = false;
        }
      }

      LastCoor[i].x = x;
      LastCoor[i].y = y;
    }
    else if (Coordinate->coor[i].event == 0)
    {
      x             = (uint16_t)(Coordinate->coor[i].xh) << 8 | (Coordinate->coor[i].xl);
      y             = (uint16_t)(Coordinate->coor[i].yh) << 8 | (Coordinate->coor[i].yl);
      LastCoor[i].x = x;
      LastCoor[i].y = y;
    }
    else if (Coordinate->coor[i].event == 1)
    {
      draw_over = true;
    }
  }

  for (int n = 0; n < CELL_AMOUNT; n++)
  {
    if ((CellArrayCoor[n].x != 0) || (CellArrayCoor[n].y != 0))
    {
      result = false;
    }
  }

  return result;
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
