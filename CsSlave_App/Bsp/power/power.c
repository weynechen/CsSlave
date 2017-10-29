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


void BLWatchDog(void)
{
  HAL_ADC_Start(&hadc1);
  if(HAL_ADC_PollForConversion(&hadc1,50)==HAL_OK)
  {
    uint16_t value = HAL_ADC_GetValue(&hadc1);
    if(value < 10)
    {
      Power_SetState(POWER_OUT5V,OFF);
    }
  }
  HAL_ADC_Stop(&hadc1);
  
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
