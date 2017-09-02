/**
 * @file        task.c
 * @author      Weyne
 * @version     V01
 * @date        2016.10.25
 * @brief       切换任务
 * @note
 *
 * @attention   COYPRIGHT WEYNE
 **/
#include "sysconfig.h"
#include "task.h"
#include "lcd.h"
#include "ack.h"
#include "upgrade.h"
#include "tim.h"
#include "usb_device.h"
#include "usbd_core.h"
#include "ssd2828.h"
#include "ctrlbox.h"

static const uint16_t SysConfigSize0 = sizeof(SystemConfig);
static const uint16_t SysConfigSize1 = sizeof(SystemConfig) - (POWER_LEN * 2 + 2);

void SwitchTask(void)
{
  uint8_t data8;

  switch (TaskID)
  {
  case ACT_RE_INIT_START:
    TaskID = ACTION_NULL;

    //兼容多版本，并且考虑补全(4的整数倍)
    if ((RecCounter - SysConfigSize0 < 5) || (RecCounter - SysConfigSize1 < 5))
    {
      Lcd_ReInit();
      UserPrintf("Info: ReInit complete!\n");
    }
    else if (RecCounter == 0)
    {
      Lcd_ReInit();
      UserPrintf("Info: System initialize complete!\n");
    }
    else
    {
      UserPrintf("Error: Init para error!\n");
    }
    break;

  case ACT_FLASH_PARA:
    TaskID = ACTION_NULL;
    FlashConfig();
    break;

  case ACT_UPGRADE_FIRMWARE:
    TaskID = ACTION_NULL;
    UpgradeFirmwareData();

    break;

  case ACT_REBOOT:
    __HAL_TIM_DISABLE(&htim2);
    USBD_Stop(&hUsbDeviceFS);
    HAL_Delay(600);
    SoftwareReset();
    break;

  case ACT_GET_VERSION:
    TaskID = ACTION_NULL;
    GetFirmwareVersion();
    break;

  case ACT_READ_SSD2828:
    TaskID = ACTION_NULL;
    data8 = (uint8_t)ConfigData[0];
    if ((data8 < 0xB0) && (data8 > 0xFF))
    {
      UserPrintf("Error:SSD2828 register must between 0xB0 ~ 0xFF\n");
    }
    else
    {
      UserPrintf("Info:Read 0x%x = 0x%x \n", data8, SSD2828ReadReg(data8));
    }
    break;

  case ACT_SET_SSD2828:
    TaskID = ACTION_NULL;
    data8 = (uint8_t)ConfigData[0];

    if ((data8 < 0xB0) && (data8 > 0xFF))
    {
      UserPrintf("Error:SSD2828 register must between 0xB0 ~ 0xFF\n");
    }
    else
    {
      SSD2828WriteReg(data8, ConfigData[1], ConfigData[2]);
    }

    break;

  case ACT_SET_KEY:
    TaskID = ACTION_NULL;
    CtrlKey = (KeyTypeDef)ConfigData[0];
    break;

  default:
    TaskID = ACTION_NULL;
    break;
  }
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
