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

void Power_SetState(PowerTypeDef power, StateTypeDef state)
{
	MEM_ADDR(power) = (uint8_t)state;
}


void Power_SetBLCurrent(uint16_t value)
{
  TIM1->CCR1 = 200 - value;
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/

