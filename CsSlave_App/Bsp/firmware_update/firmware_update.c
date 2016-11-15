/**
 * @file        firmware_update.c
 * @author      陈维
 * @version     V02
 * @date        2016.06.27
 *               2016.10.08
 * @brief       接收firmware，并将firmware写入到flash空闲区域
 * @note
 *
 * @attention   COYPRIGHT INMOTION ROBOT
 **/

#include "firmware_update.h"
#include "delay.h"
#include "angle_and_speed.h"
#include "sys.h"
#include "stm32f4xx_crc.h"
#include "stm32f4xx_rcc.h"
#include "ccd.h"
#include "laser.h"
#include "pro.h"
#include "transmit.h"
#include "sysconfig.h"

/**
 * @brief  软件复位
 * @param  None
 * @retval None
 */
static void SoftwareReset(void)
{
  __set_FAULTMASK(1);       // close all interrupt
  SCB->VTOR = 0x08000000;   //re-located boot address
  __DSB();                  /* Ensure all outstanding memory accesses included
                             *  buffered write are completed before reset */
  SCB->AIRCR = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                SCB_AIRCR_VECTCLRACTIVE_Msk |
                SCB_AIRCR_SYSRESETREQ_Msk);   /* Keep priority group unchanged */
  __DSB();                                    /* Ensure completion of memory access */
  while (1)
  {
  }
}


/**
 * @brief  将数据写入flash
 * @param  offset: 写入数据的偏移量
 * @param  data_len: 写入数据的长度
 * @param  data: 数据缓冲区
 * @retval None
 */
static void WriteDataToFlash(s32 offset, u32 data_len, u32 *data)
{
  u16 i = 0;
  FLASH_Status flash_status;

  FLASH_Unlock();
  FLASH_DataCacheCmd(DISABLE);
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_BSY | FLASH_FLAG_RDERR);

  for (i = 0; i < data_len; i++)
  {
    flash_status = FLASH_ProgramWord(FIRMWARE_BASE_ADDRESS + i * 4 + offset, data[i]);
    if (flash_status != FLASH_COMPLETE)
    {
      //flash 写入错误,发送应答,重启系统
      SendSignal(FW_FLASH_ERROR);
      Delay_ms(1000);       //necessary delay
      SoftwareReset();
    }
  }
  FLASH_DataCacheCmd(ENABLE);
  FLASH_Lock();
}


/**
 * @brief  发送准备接收firmware数据的信号
 * @param  None
 * @retval None
 */
void SendSignal(u8 signal)
{
  u8 i = 0;
  PackageDataStruct package;
  u32 out_len;

  package.DataID = PACK_FIRMWARE_UPDATE;
  package.DataInBuff = (u8 *)&signal;
  package.DataInLen = sizeof(signal);
  package.DataOutBuff = ComBuffer.TxBuffer;
  package.DataOutLen = &out_len;
  Package(package);
  for (i = 0; i < out_len; i++)
  {
    while ((USART1->SR & 0X40) == 0)
    {
    }
    USART1->DR = (u8)ComBuffer.TxBuffer[i];
  }
}


/**
 * @brief  处理接收到的firmware数据
 * @note   此函数不返回
 * @param  None
 * @retval None
 */
void HanldeFirmwareData(void)
{
  FirmwareDataStruct *firmware;
  u32 crc_result = 0xffffffff;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
  // 清除整个缓存扇区
  FLASH_Unlock();
  FLASH_EraseSector(FIRMWARE_SECTOR, VoltageRange_3);
  FLASH_ClearFlag(FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  FLASH_Lock();

  RecPackage.DataID = PACK_NULL;

  while (1)
  {
    //等待接收1段firmware数据
    while (RecPackage.DataID != PACK_FIRMWARE_UPDATE)
    {
    }

    RecPackage.DataID = PACK_NULL;

    firmware = (FirmwareDataStruct *)RecPackage.DataOutBuff;

    if (firmware->Offset == -1)
    {
      u32 update_ok = 1;
      //更新结束，添加标志位(所在扇区的最后一个地址),重启系统
      SendSignal(FW_OK);
      WriteDataToFlash(0x1fffc, 1, &update_ok);
      Delay_ms(1000);       //necessary delay
      SoftwareReset();
    }

    //进行CRC校验
    CRC->CR = CRC_CR_RESET;
    crc_result = CRC_CalcBlockCRC(&firmware->DataBaseAddress, firmware->DataLen / 4);

    if (crc_result != firmware->Crc32)
    {
      SendSignal(FW_CRC_ERROR);
      Delay_ms(1000);       //necessary delay
      SoftwareReset();
    }

    //写入数据到flash
    WriteDataToFlash(firmware->Offset, firmware->DataLen / 4, &firmware->DataBaseAddress);

    // 发送继续接收信号
    SendSignal(FW_UPDATE_READY);
  }
}


/**
 * @brief  停转lidar，关闭激光和CCD
 * @param  None
 * @retval None
 */
void CloseLidar(void)
{
  LaserOff();
  Set_CCD_PD(0);
  //重复几次，确保转速为0
  SendSpeed(0);
  SendSpeed(0);
  SendSpeed(0);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
