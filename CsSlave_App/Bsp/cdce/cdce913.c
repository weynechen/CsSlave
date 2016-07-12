#include "cdce913.h"
#include "i2c.h"



void CDCE_Init(void)
{
  uint8_t pData;

  HAL_I2C_Master_Receive_IT(&hi2c1, CDCE_ADDRESS, &pData, 1);
  if (pData == 0x81)
  {
    printf("Info:CDCE OK\n");
  }
  else
  {
    printf("Error:CDCE init fail\n");
  }
}
