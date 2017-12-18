/**
 * @file         flickersensor.c
 * @author       Weyne Chen
 * @version      V01
 * @date         2017.06.25
 * @brief
 * @note
 * @attention    COPYRIGHT WEYNE
 **/

#include "flickersensor.h"
#include "usart.h"
#include "stdbool.h"

uint8_t FlickerDataReady = 0;
static uint16_t FlickerValue;
static bool IsFlicker;

static void FS_SendData(uint8_t pid, uint16_t data)
{
  Ppro_SendData(FLCIKER_SENSOR, pid, data);
  HAL_Delay(50);
}


void FS_Callback(PproTypeDef *data)
{
  switch (data->PackageID)
  {
  case FLICKER_VALUE:
    FlickerDataReady = 1;
    FlickerValue     = *(uint16_t *)(data->Data);
    break;

  case IS_FLICKER:
    IsFlicker = (bool) * (uint16_t *)(data->Data);
    break;

  default:
    break;
  }
}


bool IsFlickerPattern(void)
{
  return IsFlicker;
}


uint8_t GetFlickerValue(float *flicker)
{
  uint32_t timeout = 10;

  FS_SendData(FLICKER_VALUE, 0);
  while (FlickerDataReady != 1)
  {
    timeout--;
    if (timeout == 0)
    {
      return FLICKER_TIMEOUT;
    }

    FS_SendData(FLICKER_VALUE, 0);
    HAL_Delay(100);
  }

  FlickerDataReady = 0;

  if (!IsFlicker)
  {
    return FLICKER_ERROR;
  }

  *flicker = (float)FlickerValue / 10.0;

  return FLICKER_OK;
}


uint8_t WaitFlickerOK(void)
{
  float flicker;
  uint8_t flicker_result = GetFlickerValue(&flicker);
  uint32_t timeout      = 10;

  while (flicker_result != FLICKER_OK)
  {
    if (flicker_result == FLICKER_TIMEOUT)
    {
      return 0;
    }
    timeout--;
    HAL_Delay(100);
    flicker_result = GetFlickerValue(&flicker);

    if (timeout == 1)
    {
      return 0;
    }
  }

  return 1;
}


void SendVcomToFlickerSensor(uint16_t vcom)
{
  FS_SendData(VCOM_VALUE, vcom);
}


void SendIdToFlickerSensor(uint16_t id)
{
  FS_SendData(ID_VALUE, id);
}


void SendOTPTimesToFlickerSensor(uint16_t times)
{
  FS_SendData(OTP_TIMES, times);
}


/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
