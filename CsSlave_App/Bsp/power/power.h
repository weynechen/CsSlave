/**
 * @file        power.h
 * @author      Weyne
 * @version     V01
 * @date        2016.08.07
 * @brief       CD310电源驱动配置
 * @attention   COYPRIGHT WEYNE
 */
#ifndef _POWER_H_
#define _POWER_H_

#include "sys.h"


#define POWER_AMOUT    5

/* 修改下面BITBAND中宏，即可修改power控制脚 */
typedef enum
{
  POWER_1V8 = BITBAND(GPIOD_ODR_Addr, 10),
  POWER_2V8 = BITBAND(GPIOB_ODR_Addr, 15),
  POWER_3V3 = BITBAND(GPIOB_ODR_Addr, 14),
  POWER_P5V = BITBAND(GPIOB_ODR_Addr, 13),
  POWER_N5V = BITBAND(GPIOB_ODR_Addr, 12),
} PowerTypeDef;


typedef enum
{
  OFF,
  ON
} StateTypeDef;


void Power_SetState(PowerTypeDef power, StateTypeDef state);
void Power_SetBLCurrent(uint16_t value);

#endif
