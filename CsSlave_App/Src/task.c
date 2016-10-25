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

void SwitchTask(void)
{
	switch (TaskID)
	{
		case ACT_RE_INIT_START:
			TaskID = ACTION_NULL;
			if (RecCounter== sizeof(SystemConfig)+1)
			{
				Lcd_ReInit();
				UserPrintf("Info: ReInit success!\n");
			}
			else if(RecCounter == 0)
			{
				Lcd_ReInit();
				UserPrintf("Info: LCD initialize success!\n");
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
			SendUpgradeSignal(FW_UPDATE_READY);
			break;
		
		
		default:
		break;
	}
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/

