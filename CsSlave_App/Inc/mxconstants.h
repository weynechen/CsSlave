/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal_gpio.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define SSD_SHUT_Pin GPIO_PIN_2
#define SSD_SHUT_GPIO_Port GPIOE
#define SSD_SCK_Pin GPIO_PIN_3
#define SSD_SCK_GPIO_Port GPIOE
#define SSD_SDO_Pin GPIO_PIN_4
#define SSD_SDO_GPIO_Port GPIOE
#define SSD_SDI_Pin GPIO_PIN_5
#define SSD_SDI_GPIO_Port GPIOE
#define SSD_RESET_Pin GPIO_PIN_6
#define SSD_RESET_GPIO_Port GPIOE
#define USB_pull_up_Pin GPIO_PIN_13
#define USB_pull_up_GPIO_Port GPIOC
#define EN_5V_Pin GPIO_PIN_0
#define EN_5V_GPIO_Port GPIOC
#define TF_DET_Pin GPIO_PIN_2
#define TF_DET_GPIO_Port GPIOC
#define MIPIRESET_Pin GPIO_PIN_2
#define MIPIRESET_GPIO_Port GPIOA
#define TPINT_Pin GPIO_PIN_4
#define TPINT_GPIO_Port GPIOA
#define DIR_CTRL1_Pin GPIO_PIN_6
#define DIR_CTRL1_GPIO_Port GPIOA
#define LS245_OE_Pin GPIO_PIN_7
#define LS245_OE_GPIO_Port GPIOA
#define IO_EN_Pin GPIO_PIN_4
#define IO_EN_GPIO_Port GPIOC
#define EN_3V3_Pin GPIO_PIN_5
#define EN_3V3_GPIO_Port GPIOC
#define EN_1V8_Pin GPIO_PIN_0
#define EN_1V8_GPIO_Port GPIOB
#define EN_VGH_Pin GPIO_PIN_1
#define EN_VGH_GPIO_Port GPIOB
#define EN_VCOM_Pin GPIO_PIN_11
#define EN_VCOM_GPIO_Port GPIOB
#define EN_AVDD_Pin GPIO_PIN_2
#define EN_AVDD_GPIO_Port GPIOB
#define KEY_MTP_Pin GPIO_PIN_10
#define KEY_MTP_GPIO_Port GPIOB
#define KEY_POWER_Pin GPIO_PIN_11
#define KEY_POWER_GPIO_Port GPIOE
#define KEY_DOWN_Pin GPIO_PIN_12
#define KEY_DOWN_GPIO_Port GPIOE
#define KEY_UP_Pin GPIO_PIN_13
#define KEY_UP_GPIO_Port GPIOE
#define EN_VSP_Pin GPIO_PIN_14
#define EN_VSP_GPIO_Port GPIOE
#define SPI2_CS_Pin GPIO_PIN_15
#define SPI2_CS_GPIO_Port GPIOE
#define EN_VSN_Pin GPIO_PIN_12
#define EN_VSN_GPIO_Port GPIOB
#define RST_24LCD_Pin GPIO_PIN_10
#define RST_24LCD_GPIO_Port GPIOD
#define RS_24LCD_Pin GPIO_PIN_12
#define RS_24LCD_GPIO_Port GPIOD
#define CDCE_SO_Pin GPIO_PIN_6
#define CDCE_SO_GPIO_Port GPIOC
#define EN_2V8_Pin GPIO_PIN_7
#define EN_2V8_GPIO_Port GPIOC
#define EN_MTP_Pin GPIO_PIN_3
#define EN_MTP_GPIO_Port GPIOD
#define TP_RESET_Pin GPIO_PIN_5
#define TP_RESET_GPIO_Port GPIOB
#define TP_SCL_Pin GPIO_PIN_6
#define TP_SCL_GPIO_Port GPIOB
#define TP_SDA_Pin GPIO_PIN_7
#define TP_SDA_GPIO_Port GPIOB
#define CDCE_SCL_Pin GPIO_PIN_8
#define CDCE_SCL_GPIO_Port GPIOB
#define CDCE_SDA_Pin GPIO_PIN_9
#define CDCE_SDA_GPIO_Port GPIOB
#define SSD1_CS_Pin GPIO_PIN_0
#define SSD1_CS_GPIO_Port GPIOE
#define SSD2_CS_Pin GPIO_PIN_1
#define SSD2_CS_GPIO_Port GPIOE
#define RED_LED_Pin GPIO_PIN_15
#define RED_LED_GPIO_Port GPIOC
#define GREEN_LED_Pin GPIO_PIN_14
#define GREEN_LED_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
