/**
  * @file        task.c
  * @author      陈维
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

void SwitchTask(void)
{
	switch (TaskID)
	{
		case ACT_RE_INIT_START:
			TaskID = ACTION_NULL;
			if (RecCounter== sizeof(SystemConfig)+1)
			{
				Lcd_ReInit();
				UserPrintf("Info: ReInit complete!\n");
			}
			else if(RecCounter == 0)
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
		
		default:
		break;
	}
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/

