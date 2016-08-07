#include "com.h"
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1;
int fputc(int ch, FILE *f)
{
  uint8_t pData = (uint8_t)ch;

  HAL_UART_Transmit(&huart1, &pData, 1, 10);
  return(ch);
}
