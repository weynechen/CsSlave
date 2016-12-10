/**
 * @file        ctrlbox.c
 * @author      Weyne
 * @version     V01
 * @date        2016.11.13
 * @brief       控制盒驱动
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#include "ctrlbox.h"
#include "gpio.h"

KeyTypeDef CtrlKey = KEY_NULL;

void ScanKey(void)
{
  if (HAL_GPIO_ReadPin(GPIOE, KEY_POWER_Pin) == GPIO_PIN_RESET)
  {
    HAL_Delay(30);
    while (HAL_GPIO_ReadPin(GPIOE, KEY_POWER_Pin) == GPIO_PIN_RESET)
    {
      CtrlKey = KEY_POWER;
    }
  }
  else if (HAL_GPIO_ReadPin(GPIOE, KEY_UP_Pin) == GPIO_PIN_RESET)
  {
    HAL_Delay(30);
    while (HAL_GPIO_ReadPin(GPIOE, KEY_UP_Pin) == GPIO_PIN_RESET)
    {
      CtrlKey = KEY_UP;
    }
  }
  else if (HAL_GPIO_ReadPin(GPIOE, KEY_DOWN_Pin) == GPIO_PIN_RESET)
  {
    HAL_Delay(30);
    while (HAL_GPIO_ReadPin(GPIOE, KEY_DOWN_Pin) == GPIO_PIN_RESET)
    {
      CtrlKey = KEY_DOWN;
    }
  }
//	else
//	{
//		CtrlKey = KEY_NULL;
//	}
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/