#ifndef _POWER_H_
#define _POWER_H_

#include "gpio.h"

typedef enum
{	
	POWER_1V8=GPIO_PIN_0,
	POWER_2V8=GPIO_PIN_15,
	POWER_3V3=GPIO_PIN_14,
	POWER_P5V=GPIO_PIN_13,
	POWER_N5V=GPIO_PIN_12

}PowerPinTypeDef;

void PowerOff(PowerPinTypeDef power);
void PowerOn(PowerPinTypeDef power);
void SetBLCurrent(uint16_t value);

#endif

