/**
 * @file        upgrade.c
 * @author      Weyne
 * @version     V01
 * @date        2016.10.25
 * @brief       firmware升级
 * @note
 *
 * @attention   COYPRIGHT WEYNE
 **/
#include "upgrade.h"
#include "sysconfig.h"
#include "flash_if.h"
#include "ack.h"
#include "crc.h"
#include "flash_if.h"



/**
 * @brief  处理接收到的firmware数据
 * @note   此函数不返回
 * @param  None
 * @retval None
 */
void UpgradeFirmwareData(void)
{
  FirmwareDataStruct *firmware;

  if (FLASH_If_Erase(FIRMWARE_BASE_ADDRESS, NUMBER_OF_UPGRADE_PAGES) == FLASHIF_ERASEKO)
  {
    UserPrintf("Error:upgrade failed , system will reboot\n");
    HAL_Delay(1000);
    SoftwareReset();
  }

  SendUpgradeSignal(FW_UPDATE_READY);

  TaskID = ACTION_NULL;

  while (1)
  {
    //等待接收1段firmware数据
    while (TaskID != ACT_UPGRADE_FIRMWARE)
    {
    }

    TaskID = ACTION_NULL;

    firmware = (FirmwareDataStruct *)RecPackage.DataOutBuff;

    if (firmware->Offset == -1)
    {
      uint32_t update_ok = 1;

      if (FLASH_If_Write(FIRMWARE_UPGRADE_FLAG_BASE_ADDRESS, (uint32_t *)&update_ok, 1) == 0)
      {
        SendUpgradeSignal(FW_OK);
      }
      else
      {
        SendUpgradeSignal(FW_FLASH_ERROR);
      }

      HAL_Delay(1000);     //necessary delay
      SoftwareReset();
    }

    //写入数据到flash
    if (FLASH_If_Write(FIRMWARE_BASE_ADDRESS + firmware->Offset, (uint32_t *)(RecPackage.DataOutBuff + 4), (RecCounter - 4) / 4) != 0)
    {
      SendUpgradeSignal(FW_FLASH_ERROR);
      HAL_Delay(1000);       //necessary delay
      SoftwareReset();
    }

    // 发送继续接收信号
    SendUpgradeSignal(FW_UPDATE_READY);
  }
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
