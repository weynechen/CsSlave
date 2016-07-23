#include "power.h"
#include "tim.h"

void PowerOff(PowerPinTypeDef power)
{
	if(power==POWER_1V8)
	  HAL_GPIO_WritePin(GPIOC,POWER_1V8,GPIO_PIN_RESET);
	else
	  HAL_GPIO_WritePin(GPIOB,power,GPIO_PIN_RESET);
}

void PowerOn(PowerPinTypeDef power)
{
	if(power==POWER_1V8)
	  HAL_GPIO_WritePin(GPIOC,POWER_1V8,GPIO_PIN_SET);
	else
	  HAL_GPIO_WritePin(GPIOB,power,GPIO_PIN_SET);
}

void SetBLCurrent(uint16_t value)
{
	PowerOn(POWER_P5V);	
	TIM1->CCR1 = 200 - value;
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
}

