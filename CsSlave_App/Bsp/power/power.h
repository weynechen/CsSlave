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


#define POWER_AMOUT    8

/* 修改下面BITBAND中宏，即可修改power控制脚 */
typedef enum
{
  POWER_1V8 = BITBAND(GPIOB_ODR_Addr, 0),
  POWER_2V8 = BITBAND(GPIOC_ODR_Addr, 7),
  POWER_3V3 = BITBAND(GPIOC_ODR_Addr, 5),
	POWER_VSP = BITBAND(GPIOE_ODR_Addr, 14),
  POWER_VSN = BITBAND(GPIOB_ODR_Addr, 12),
	POWER_OUT5V = BITBAND(GPIOC_ODR_Addr,0),
  POWER_MTP = BITBAND(GPIOD_ODR_Addr, 3),
	POWER_AVDD = BITBAND(GPIOB_ODR_Addr,2),
	POWER_VCOM = BITBAND(GPIOB_ODR_Addr,11),
	POWER_VGH = BITBAND(GPIOB_ODR_Addr,1),
} PowerTypeDef;


typedef enum
{
  OFF,
  ON
} StateTypeDef;


void Power_SetState(PowerTypeDef power, StateTypeDef state);
void Power_SetBLCurrent(uint16_t value);

#endif
