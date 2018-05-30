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
#include "flickersensor.h"
#include "tp.h"
#include "sublcd.h"
#include "build_info.h"
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
extern bool IsNeedClearBg;
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
#ifdef FUN_MTP
#define VCOM_VALUE    0x8B

static int16_t Vcom = VCOM_VALUE;

static void SetVcom(void)
{
  SSD2828_GenericLongWrite(3);
  SSD2828WriteData(0xD4);
  SSD2828WriteData(0x00);
  SSD2828WriteData(Vcom);
}


static uint8_t GetOTPTimes(void)
{
  uint8_t val[1];
  uint8_t ss;

  SSD2828_GenericLongWrite(4);
  SSD2828WriteData(0xFF);
  SSD2828WriteData(0x98);
  SSD2828WriteData(0x81);
  SSD2828WriteData(0x06);


  if (SSD2828_GenericReadDT14(0xE0, 1, val) == MIPI_READ_SUCCEED)
  {
    if ((val[0] & 0x38) == 0x00)
    {
      ss = 0;
    }
    else if ((val[0] & 0x38) == 0x20)
    {
      ss = 1;
    }
    else if ((val[0] & 0x38) == 0x30)
    {
      ss = 2;
    }
    else if ((val[0] & 0x38) == 0x38)
    {
      ss = 3;
    }
  }

  return ss;
}


void OpInLPMode(void)
{
#ifdef FUN_MTP
  SendOTPTimesToFlickerSensor(GetOTPTimes());
#endif
}


static uint8_t CheckResult(void)
{
  uint8_t buffer[3];
  uint8_t result = 1;

  if (SSD2828_GenericReadDT14(0x04, 3, buffer) == MIPI_READ_SUCCEED)
  {
    UserPrintf("vcom:0x%x 0x%x 0x%x\n", buffer[0], buffer[1], buffer[2]);
    if ((buffer[0] == 0X20) && (buffer[1] == 0X17) && (buffer[2] == 0X06))
    {
    }
    else
    {
      result = 0;
    }
  }
  else
  {
    UserPrintf("Read ID Failed!\n");
    result = 0;
  }

  SSD2828_GenericLongWrite(4);
  SSD2828WriteData(0xA2);
  SSD2828WriteData(0x87);
  SSD2828WriteData(0x78);
  SSD2828WriteData(0x01);

  if (SSD2828_GenericReadDT14(0xA3, 1, buffer) == MIPI_READ_SUCCEED)
  {
    UserPrintf("vcom:0x%x\n", buffer[0]);
    if (buffer[0] == Vcom)
    {
    }
    else
    {
      result = 0;
    }
  }
  else
  {
    UserPrintf("Read VCOM Failed!\n");
    result = 0;
  }

  return result;
}


static uint8_t MTP(void)
{
  uint8_t result = 1;

  Power_SetBLCurrent(0);

  ResetMipiLcd();
  SetMipiPara();
  SSD2828_SetMode(LP);
  SetMTPCode(Vcom);
  ResetMipiLcd();
  if (CheckResult() == 0)
  {
    result = 0;
  }

  SSD2828_DcsShortWrite(1);
  SSD2828WriteData(0x11);
  HAL_Delay(100);
  SSD2828_DcsShortWrite(1);
  SSD2828WriteData(0x29);

  Power_SetBLCurrent(SystemConfig.Backlight);
  LcdDrvOpenRGB();
  SSD2828_SetMode(VD);

  return result;
}


static void TuningVcom(KeyTypeDef key)
{
  char temp[4];

  if (key == KEY_UP)
  {
    Vcom++;
  }
  else if (key == KEY_DOWN)
  {
    Vcom--;
  }
  SetVcom();
  snprintf(temp, sizeof(temp), "%02X", Vcom);
  LCD_ShowString(0, 0, temp);
}


/*--------6,OTP烧录需要修改的函数---------*/
const static float TargetFlickerValue = 12.0; /*------flicker的目标值，该值根据所需进行调整----*/
const static uint8_t VcomMin          = 0x05; //最小的vcom值，该值根据所需进行调整
const static uint8_t VcomMax          = 0xa0; //最大的vcom值，该值根据所需进行调整

//VCOM自动调节函数，不用改动
#define SAMPLE_AMOUNT    3
typedef struct
{
  uint8_t vcom;
  float   flk;
} VFMap;
uint8_t AutoTuningVcom(void)
{
  VFMap min = { VCOM_VALUE, 0xff };
  VFMap tmp[SAMPLE_AMOUNT];
  uint32_t time = HAL_GetTick();
  float flk;
  uint8_t status  = 0;
  uint8_t counter = 0;
  int k           = 0;
  int step;
  uint8_t reverse_times = 0;

  Vcom = VCOM_VALUE;
  for (uint8_t i = 0; i < 3; i++)
  {
    SetVcom();
    if (GetFlickerValue(&flk) == FLICKER_TIMEOUT)
    {
      UserPrintf("com err\n");
      return 0;
    }
  }

  while ((Vcom > VcomMin) && (Vcom < VcomMax))
  {
    SetVcom();
    //HAL_Delay(500);
    if (GetFlickerValue(&flk) == FLICKER_TIMEOUT)
    {
      return 0;
    }
    step = flk / 10;
    if (step == 0)
    {
      step = 1;
    }

    if (flk < min.flk)
    {
      min.flk  = flk;
      min.vcom = Vcom;
    }

    switch (status)
    {
    case 0:
      if (flk < 100)
      {
        status = 2;
      }
      else
      {
        Vcom = VcomMin + 10;
        status++;
      }
      break;

    case 1:
      Vcom += 10;
      if (flk < 100)
      {
        status++;
      }
      break;

    case 2:
      tmp[counter].flk  = flk;
      tmp[counter].vcom = Vcom;
      counter++;
      Vcom += step;
      if (counter == SAMPLE_AMOUNT)
      {
        k    = (tmp[0].flk < tmp[SAMPLE_AMOUNT - 1].flk) ? (-1) : 1;
        Vcom = (k == 1) ? (tmp[SAMPLE_AMOUNT - 1].vcom + 1) : (tmp[0].vcom - 1);
        status++;
      }
      break;

    case 3:
      Vcom += step * k;
      if (flk > min.flk)
      {
        reverse_times++;
        Vcom += step * k;
      }
      if (reverse_times > 3)
      {
        Vcom = min.vcom;
        status++;
      }
      break;

    case 4:
      if (flk < TargetFlickerValue)
      {
        UserPrintf("time:%dms\n", HAL_GetTick() - time);
        SetVcom();
        return 1;
      }
      else
      {
        k             = 1;
        min.flk       = 0xff;
        Vcom          = VcomMin + 5;
        reverse_times = 0;
        status        = 3;
      }
    }

    if (HAL_GetTick() - time > 30000)//超时时间，单位ms
    {
      //UserPrintf("over time\n");
      return 0;
    }
    //UserPrintf("vcom:%X,flk:%0.1f\n",Vcom,flk);
  }

  return 0;
}
#endif

/* USER CODE END 0 */

int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t key_control    = 0;
  uint8_t power_on       = 1;
  uint8_t mtp_mode       = 0;
  uint32_t power_on_time = 0;
  uint32_t power_off_time = 0;
  bool tp_draw_line      = false;
  bool lock_key       = false;
  bool tp_is_cell        = false;

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
  MX_USART3_UART_Init();
  MX_FATFS_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
	//MX_TIM4_Init();//beep music,not be use yet
  MX_SPI2_Init();
  MX_ADC1_Init();
  MX_USB_DEVICE_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();

  /* USER CODE BEGIN 2 */
  UserPrintf("\n-- CD310 --\n");
  UserPrintf("|----------------|\n");
  UserPrintf("| 当前版本支持功能: |\n");
  UserPrintf("| 点屏        [√] |\n");
#ifdef FUN_TP
  UserPrintf("| TP测试      [√] |\n");
#endif

#ifdef FUN_MTP
  UserPrintf("| 自动OTP     [√] |\n");
#endif
  UserPrintf("|----------------|\n");  
  UserPrintf("Info:build time:%d-%d-%d %d:%d:%d\n", CURR_YEAR, CURR_MONTH, CURR_DAY, CURR_HOUR, CURR_MIN, CURR_SEC);
  GetFirmwareVersion();
  InitSystemConfig();
  SDCardCheck();
  UART_SetDMA();
  CDCE_Init(30);
  BLAndIDInit();

  /*@TODO
   * W25Nxx_Init();
   */
  SSD2828_Init(4, 480);
  ReadSystemConfig();
  //SubLCD_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    ScanKey();
    SwitchTask();

    if (tp_draw_line || lock_key || PatternProperty.ForceStay[PatternProperty.CurrentPattern])
    {
      if ((CtrlKey == KEY_DOWN) || (CtrlKey == KEY_UP))
      {
        CtrlKey = KEY_NULL;
      }
    }

  //跑机模式
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)==GPIO_PIN_SET)
    {
      if ((power_on==1)&&((HAL_GetTick() - power_on_time) > 3000) ||\
          (power_on==0)&&((HAL_GetTick() - power_off_time) > 1000)\
      )
      {
        CtrlKey = KEY_POWER;
      }


    }

    switch (CtrlKey)
    {
    case KEY_UP:
      key_control = 1;
      if (mtp_mode == 0)
      {
        PageTurning(PAGE_UP);
      }
      else
      {
#ifdef FUN_MTP
        TuningVcom(KEY_UP);
#endif
      }
      break;

    case KEY_DOWN:
      key_control = 1;
      if (mtp_mode == 0)
      {
        PageTurning(PAGE_DOWN);
      }
      else
      {
#ifdef FUN_MTP
        TuningVcom(KEY_DOWN);
#endif
      }
      break;

    case KEY_POWER:
      key_control  = 0;
      mtp_mode     = 0;
      tp_draw_line = false;
      if (power_on == 1)
      {
        GREEN_LIGHT_OFF;
        RED_LIGHT_OFF;
#ifdef FUN_TP
        TP_SendData(TP_POWER_OFF, 0);
#endif
        if (SystemConfig.LcdType == MIPI_LCD)
        {
          MipiLcdSleepIn();
          SSD2828_SetReset(0);
        }
        Power_SetBLCurrent(0);
        HAL_Delay(10);
        PowerOff();
        if(IsNeedClearBg)
        {
          PrepareBg();
          IsNeedClearBg = false;
        }
        RED_LIGHT_ON;        
        power_on = 0;
        power_off_time = HAL_GetTick();
      }
      else
      {
        RED_LIGHT_OFF;
        GREEN_LIGHT_ON;
#ifdef FUN_TP
        TP_SendData(TP_POWER_OFF, 1);
#endif
				PatternProperty.CurrentPattern = 0;
				LcdDrvShowPattern(PatternProperty.Data[PatternProperty.CurrentPattern]);
        Lcd_LightOn();
        power_on      = 1;
        power_on_time = HAL_GetTick();
				ResetStayTimeCounter();
        // LCD_SetFlickerType(F_DOT);//F_DOT or F_COLUMN
        // LCD_EraseFlickerString();
        if(InspectionAfterPowerOn() == false)
        {
          lock_key = true;
          key_control = 1;
        }
        else
        {
					TP_ParallelStart();//concurrent tp test
          lock_key = false;
        }

      }
      break;

    case KEY_MTP:
#ifdef FUN_MTP
      if (mtp_mode == 0)
      {
        LcdDrvSetCharIndex(PatternProperty.Data[PatternProperty.CurrentPattern]);
        mtp_mode = 1;
      }
      else
      {
        if (AutoTuningVcom() == 1)
        {
          UserPrintf("vcom:0x%x\n", Vcom);
          if (MTP() == 1)
          {
            LCD_ShowString(0, 0, "OK");
          }
          else
          {
            LCD_ShowString(0, 0, "NG");
          }
          SendVcomToFlickerSensor(Vcom);
          SendIdToFlickerSensor(0);
          SendOTPTimesToFlickerSensor(GetOTPTimes());
        }
        mtp_mode = 0;
      }
#endif
      break;

#ifdef FUN_TP
    case KEY_TP:
      //break;
      key_control = 1;
      EnterBg();
      SetFontColor(0);
      #ifdef FUN_DRAW_LINE
      LCD_ShowString(0, 0, "TP Testing...");
      #endif
      if (TP_Test() == true)
      {
      #ifdef FUN_DRAW_LINE        
        uint16_t x = LCDTiming.LCDH / CELL_DIV_H;
        uint16_t y = LCDTiming.LCDV / CELL_DIV_V;

        LCD_ShowString(0, 0, "              ");
        LCD_ShowString(0, 32*FontScale, "                     ");
        SetFontColor(0xff00);
        LCD_ShowString(x + 2, y + 1, "Donwload OK");
        SetFontColor(0);
        TP_DrawBG();
        tp_draw_line = true;
        tp_is_cell   = true;
      #else
        SetFontColor(0xff00);        
        LCD_Printf("\nTP Test OK");
        tp_draw_line = true;
        tp_is_cell   = false;
      #endif
        
      }
      else
      {
      #ifdef FUN_DRAW_LINE        
        SetFontColor(0xff0000);
        LCD_ShowString(0, 64*FontScale, "TP NG");
        SetFontColor(0);
      #else
        SetFontColor(0xff0000);        
        LCD_Printf("\nTP Test NG");
      #endif  
      }
      break;

      case KEY_DRAW_LINE:
      key_control = 1;
      PrepareBg();
      tp_draw_line = true;
      tp_is_cell   = false;
      break;

#endif

    default:
      break;
    }
    CtrlKey = KEY_NULL;

    if ((SystemConfig.IsAutoRun == 1) && (key_control == 0) && (power_on == 1))
    {
      if (IsStayTimeOver(PatternProperty.CurrentPattern) == 1)
      {
        PageTurning(PAGE_UP);
      }
    }

    if ((HAL_GetTick() - power_on_time) > 2000)
    {
      BLWatchDog();
    }

#ifdef FUN_TP
    if (tp_draw_line)
    {
      if (IsTPToggle())
      {
        SetCellOrLine(tp_is_cell);
        if ((TP_DrawLine() == true) && (tp_is_cell))
        {
          //ok
//          tp_draw_line = false;
          tp_is_cell = false;
          PrepareBg();
          for (int i = 0; i < 10; i++)
          {
            IsTPToggle();
          }
          SetFontColor(0xff00);
          LCD_ShowString(0, 0, "TP Test OK!");
        }
      }
    }
#endif

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
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection    = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection    = RCC_USBCLKSOURCE_PLL_DIV1_5;
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
	
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);

}


/* USER CODE BEGIN 4 */
int fputc(int ch, FILE *file)
{
  uint8_t pData = (uint8_t)ch;

  HAL_UART_Transmit(&huart1, &pData, 1, 10);
  return ch;
}


// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//   if (htim == &htim2)
//   {
//     SendHeartBeat();
//   }
// }


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
