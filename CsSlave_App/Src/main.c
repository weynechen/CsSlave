/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fsmc.h"

/* USER CODE BEGIN Includes */
#include "cdce913.h"
#include "ssd2828.h"
#include "usbd_cdc_if.h"
#include "power.h"
#include "rec.h"
#include "lcd.h"
#include "sysconfig.h"
#include "fpga.h"
#include "ack.h"
#include "in_img.h"
#include "spi_flash.h"
#include "task.h"
#include "ctrlbox.h"
#include "mtp.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

static uint8_t Vcom = 0x9B;

static void SetVcom(void)
{
	SSD2828_GenericLongWrite(3);
	SSD2828WriteData(0xD4);
	SSD2828WriteData(0x00);
	SSD2828WriteData(Vcom);	
}

static void TuningVcom(KeyTypeDef key)
{
	char temp[4];
	if(key == KEY_UP)
	{
		Vcom ++;
	}
	else if(key == KEY_DOWN)
	{
		Vcom --;
	}
	SetVcom();
	snprintf(temp,sizeof(temp),"%02X",Vcom);
	LCD_ShowString(0,0,temp);
}

static void MTP(void)
{
	Power_SetBLCurrent(0);
	
	ResetMipiLcd();
	SetMipiPara();
	SetMTPCode(Vcom);
	ResetMipiLcd();
	
	SSD2828_DcsShortWrite(1);
	SSD2828WriteData(0x11);
	HAL_Delay(100);
	SSD2828_DcsShortWrite(1);
	SSD2828WriteData(0x29);
	
	Power_SetBLCurrent(SystemConfig.Backlight);
  LcdDrvOpenRGB();
	SSD2828_SetMode(VD);
}


/* USER CODE END 0 */

int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t key_control = 0;
  uint8_t power_on = 1;
	uint8_t mtp_mode = 0;
	
  SCB->VTOR = APP_BASE_ADDRESS;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CRC_Init();
  MX_FSMC_Init();
  MX_SDIO_SD_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  //MX_USART3_UART_Init();
  MX_FATFS_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_SPI2_Init();
  MX_ADC1_Init();
  MX_USB_DEVICE_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();

  /* USER CODE BEGIN 2 */
  UserPrintf("\n--CD310--\n");
  GetFirmwareVersion();
  InitSystemConfig();
  SDCardCheck();
  UART_SetDMA();
  CDCE_Init(30);
	/*@TODO 
	W25Nxx_Init();
  */
	SSD2828_Init(4, 480);
  ReadSystemConfig();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {    
		ScanKey();
    SwitchTask();

    switch (CtrlKey)
    {
    case KEY_UP:
      key_control = 1;
			if(mtp_mode == 0)
				PageTurning(PAGE_UP);
			else
				TuningVcom(KEY_UP);
      break;

    case KEY_DOWN:
      key_control = 1;
			if(mtp_mode == 0)
				PageTurning(PAGE_DOWN);
			else
				TuningVcom(KEY_DOWN);
      break;

    case KEY_POWER:
      key_control = 0;
			mtp_mode =0;
		
      if (power_on == 1)
      {
				GREEN_LIGHT_OFF;
				RED_LIGHT_ON;
				if(SystemConfig.LcdType == MIPI_LCD)
				{
					MipiLcdSleepIn();
					SSD2828_SetReset(0);
        }
				SetLcdPower(OFF);
        Power_SetBLCurrent(0);
        power_on = 0;
      }
      else
      {
				RED_LIGHT_OFF;
				GREEN_LIGHT_ON;
        Lcd_LightOn();
        LcdDrvShowPattern(PatternProperty.CurrentPattern = 0);
        power_on = 1;
        ResetStayTimeCounter();
      }
      break;

		case KEY_MTP:
			if(mtp_mode == 0)
			{
				LcdDrvSetCharIndex(PatternProperty.CurrentPattern);
				mtp_mode =1;
			}
			else
			{
				//TODO
				MTP();
				mtp_mode =0;
			}
			break;
			
    default:
      break;
    }
		CtrlKey = KEY_NULL;
		
    if ((SystemConfig.IsAutoRun == 1) && (key_control == 0) &&(power_on == 1))
    {
      if (IsStayTimeOver(PatternProperty.CurrentPattern) == 1)
      {
				PageTurning(PAGE_UP);
      }
    }



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


/** System Clock Configuration
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/** NVIC Configuration
 */
static void MX_NVIC_Init(void)
{
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USART3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* SDIO_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SDIO_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(SDIO_IRQn);
	
	HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
	
  /* USB_LP_CAN1_RX0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}


/* USER CODE BEGIN 4 */
int fputc(int ch, FILE *file)
{
  uint8_t pData = (uint8_t)ch;

  HAL_UART_Transmit(&huart1, &pData, 1, 10);
  return ch;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim2)
  {
    SendHeartBeat();
  }
}


/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
  }
  /* USER CODE END Error_Handler */
}


#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */

  /* User can add his own implementation to report the file name and line number,
   *  ex: UserPrintf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
