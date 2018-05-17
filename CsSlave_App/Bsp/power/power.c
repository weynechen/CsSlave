/**
 * @file        power.c
 * @author      Weyne
 * @version     V01
 * @date        2016.08.07
 * @brief       CD310电源驱动
 * @attention   COYPRIGHT WEYNE
 */

#include "power.h"
#include "tim.h"
#include "adc.h"
#include "ack.h"

static uint16_t ADC_Buffer[2];

void Power_SetState(PowerTypeDef power, StateTypeDef state)
{
  MEM_ADDR(power) = (uint8_t)state;
}

void Power_SetBLCurrent(uint16_t value)
{
  //TIM1->CCR1 = BL_MAX - value;
  TIM1->CCR1 = value;
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

#define SAMPLE_AMOUNT 12
void BLWatchDog(void)
{
  static uint16_t tmp[SAMPLE_AMOUNT];
  static uint8_t amount = 0;
  static uint32_t time = 0;

  if ((HAL_GetTick() - time) < 10)
  {
    return;
  }

  time = HAL_GetTick();

  tmp[amount] = ADC_Buffer[1];
  if (amount == (SAMPLE_AMOUNT - 1))
  {
    uint16_t data = tmp[0];
    for (uint8_t i = 1; i < SAMPLE_AMOUNT; i++)
    {
      if (data < tmp[i])
      {
        data = tmp[i];
      }
    }

    if (data < 10)
    {
      Power_SetState(POWER_OUT5V, OFF);
    }
    amount = 0;
  }
  else
  {
    amount++;
  }
}

void BLAndIDInit(void)
{
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Buffer, 2);
}

uint32_t GetIDVol(void)
{
  return (uint32_t)ADC_Buffer[0]*3300/4096;
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
