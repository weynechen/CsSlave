/**
 * @file        firmwareupgrade.c
 * @author      Weyne
 * @version     V01
 * @date        2016.10.29
 * @brief       firmware升级函数
 * @note
 *
 * @attention   COYPRIGHT WEYNE
 **/

#include "firmwareupgrade.h"
#include "system_config.h"
#include "aes128.h"
#include "flash_if.h"

extern CRC_HandleTypeDef hcrc;

const uint8_t KeyStr[AES_KEY_SIZE * 4] =
{
  0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
  0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
};

static uint32_t ExtKey[AES_EXPKEY_SIZE];

static FirmwareTypeDef Firmware;
static uint32_t Seed[4];
static uint32_t SeedBeforeData[4];

/**
 * @brief  判断是否需要更新firmware
 * @param  None
 * @retval TRUE or FALSE
 */
bool IsNeedToUpdate(void)
{
  uint32_t flag;

  flag = (uint32_t)*(vu32 *)FIRMWARE_UPGRADE_FLAG_BASE_ADDRESS;
  if (flag == 1)
  {
    return TRUE;
  }
  return FALSE;
}


/**
 * @brief  擦除整个firmware存储区
 * @param  None
 * @retval None
 */
static void EraseFirmwareSector(void)
{
  FLASH_If_Erase(FIRMWARE_BASE_ADDRESS, NUMBER_OF_UPGRADE_PAGES);
}


/**
 * @brief  擦除整个App程序区
 * @param  None
 * @retval None
 */
static void EraseAppSector(void)
{
  FLASH_If_Erase(KEY_STORE_ADDRESS, 1);	
  FLASH_If_Erase(APP_BASE_ADDRESS, NUMBER_OF_APP_PAGAES);
	FLASH_If_Erase(ADDR_FLASH_PAGE_127,1);
}


/**
 * @brief  解密函数
 * @param  input : 输入数据的首地址
 * @param  output : 输出数据的首地址
 * @param  input_size_by128bit : 输入数据的大小(需要折算成128bit的大小)
 * @retval None
 */
static void Decryption(uint32_t *input, uint32_t *output, uint32_t input_size_by_128bit)
{
  uint32_t cnt = 0;
  uint8_t *seed_byte = (uint8_t *)Seed;  //seed需要按byte操作
  uint8_t *output_byte = (uint8_t *)output;
  uint8_t i = 0;

  while (input_size_by_128bit != cnt)
  {
    AES_decrypt((uint32_t *)(input + (cnt * 4)), (uint32_t *)(output + (cnt * 4)), ExtKey);
    for (i = 0; i < 4; i++)
    {
      output[(cnt * 4) + i] ^= Firmware.RandomValue[i];
    }
    for (i = 0; i < 16; i++)
    {
      output_byte[(cnt * 16) + i] -= seed_byte[i];
      seed_byte[i] += 1;
    }
    cnt += 1;
  }
}


/**
 * @brief  获取随机数
 * @param  None
 * @retval None
 */
static void GetRandomValue(void)
{
  uint32_t crypto_data[4];
  uint32_t random_value_offset;
  uint8_t i;

  //解密得到RandomValue
  random_value_offset = (uint32_t)&Firmware.RandomValue - (uint32_t)&Firmware;
  for (i = 0; i < 4; i++)
  {
    crypto_data[i] = (uint32_t)*(vu32 *)(FIRMWARE_BASE_ADDRESS + random_value_offset + i * 4);
  }
  AES_decrypt(crypto_data, Firmware.RandomValue, ExtKey);

  //初始化seed
  for (i = 0; i < 4; i++)
  {
    Seed[i] = Firmware.RandomValue[i];
  }

  //保存此时的seed，因为数据段需要多次解密，需要用到这个Seed.
  for (i = 0; i < 4; i++)
  {
    SeedBeforeData[i] = Seed[i];
  }
}


/**
 * @brief  获取Firmware文件的头信息和CRC值
 * @param  None
 * @retval None
 */
static void GetFirmwareInfo(void)
{
  //一定未加密字段 -- 直接读出信息
  Firmware.FileSize = (uint32_t)*(vu32 *)FIRMWARE_BASE_ADDRESS;
  Firmware.EncryptFlag = (uint32_t)*(vu32 *)(FIRMWARE_BASE_ADDRESS + ((uint32_t)&Firmware.EncryptFlag - (uint32_t)&Firmware));
  Firmware.SectionNumber = (uint32_t)*(vu32 *)(FIRMWARE_BASE_ADDRESS + ((uint32_t)&Firmware.SectionNumber - (uint32_t)&Firmware));
  Firmware.FileCRC32 = (uint32_t)*(vu32 *)(FIRMWARE_BASE_ADDRESS + (Firmware.FileSize - 4));

  //可能加密字段
  if (Firmware.EncryptFlag == 1) //解密处理
  {
    GetRandomValue();
  }
  else  //不解密处理
  {
    //不需要randomvalue
  }
}


/**
 * @brief  检查firmware文件数据段的CRC值(不包括CRC本身)
 * @note   firmware数据段的CRC是解密后的CRC
 * @param  None
 * @retval 校验成功返回TRUE,校验失败返回FALSE
 */
static bool CheckSectionCRC(SectionTypeDef section, uint32_t SectionBaseAddress)
{
  uint8_t i;
  uint32_t j;
  uint32_t real_data[4];
  uint32_t crypto_data[4];
  uint32_t section_crc;

  hcrc.Instance->CR = CRC_CR_RESET;

  if (Firmware.EncryptFlag == 1)
  {
    for (j = 0; j < section.SectionLen; j += 16)
    {
      for (i = 0; i < 16; i += 4)
      {
        crypto_data[i / 4] = (uint32_t)*(vu32 *)(SectionBaseAddress + i + j);
      }

      Decryption(crypto_data, real_data, 1);

      section_crc = HAL_CRC_Accumulate(&hcrc, real_data, 4);
    }

    if (section_crc == section.SectionCRC32)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    for (j = 0; j < section.SectionLen; j += 4)
    {
      hcrc.Instance->DR = (uint32_t)*(vu32 *)(SectionBaseAddress + j);
    }
    section_crc = hcrc.Instance->DR;
    if (section_crc == section.SectionCRC32)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
}


/**
 * @brief  检查整个文件的CRC值(不包括CRC本身)
 * @note   整个文件的CRC是不需要解密的CRC
 * @param  None
 * @retval 校验成功返回TRUE,校验失败返回FALSE
 */
static bool CheckFileCRC(void)
{
  uint32_t i;
  uint32_t file_crc;

  hcrc.Instance->CR = CRC_CR_RESET;

  //越界判断，防止haltfault
  if ((Firmware.FileSize < 4) || (Firmware.FileSize > 0x20000))
  {
    return FALSE;
  }


  for (i = 0; i < Firmware.FileSize - 4; i += 4)
  {
    hcrc.Instance->DR = (uint32_t)*(vu32 *)(FIRMWARE_BASE_ADDRESS + i);
  }

  file_crc = hcrc.Instance->DR;

  if (file_crc == Firmware.FileCRC32)
  {
    return TRUE;
  }

  return FALSE;
}


/**
 * @brief  解密firmware data，并写入到app区
 * @param  data_offset : 数据地址的偏移量
 * @retval None
 */
static void DecryptAndWrtieToApp(SectionTypeDef section, uint32_t section_base)
{
  uint32_t i = 0;
  uint32_t j;
  uint32_t real_data[4];
  uint32_t crypto_data[4];

  for (j = 0; j < section.SectionLen; j += 16)
  {
    for (i = 0; i < 16; i += 4)
    {
      crypto_data[i / 4] = (uint32_t)*(vu32 *)(section_base + i + j);
    }

    Decryption(crypto_data, real_data, 1);

    if (FLASH_If_Write(section.SectionAddress + j, real_data, 4) != 0)
    {
      HAL_FLASH_Lock();
      NVIC_SystemReset();
    }
  }
}


static SectionTypeDef GetSectionInfo(uint32_t section_offset)
{
  SectionTypeDef section;

  if (Firmware.EncryptFlag == 1)
  {
    uint32_t crypto_data[4];
    uint8_t i;
    for (i = 0; i < 4; i++)
    {
      crypto_data[i] = (uint32_t)*(vu32 *)((FIRMWARE_BASE_ADDRESS + ((uint32_t)&Firmware.Section.SectionLen + section_offset - (uint32_t)&Firmware)) + i * 4);
    }
    Decryption(crypto_data, (uint32_t *)&section, 1);
  }
  else
  {
    section.SectionLen = (uint32_t)*(vu32 *)(FIRMWARE_BASE_ADDRESS + ((uint32_t)&Firmware.Section.SectionLen + section_offset - (uint32_t)&Firmware));
    section.SectionRealLen = (uint32_t)*(vu32 *)(FIRMWARE_BASE_ADDRESS + ((uint32_t)&Firmware.Section.SectionRealLen + section_offset - (uint32_t)&Firmware));
    section.SectionAddress = (uint32_t)*(vu32 *)(FIRMWARE_BASE_ADDRESS + ((uint32_t)&Firmware.Section.SectionAddress + section_offset - (uint32_t)&Firmware));
    section.SectionCRC32 = (uint32_t)*(vu32 *)(FIRMWARE_BASE_ADDRESS + ((uint32_t)&Firmware.Section.SectionCRC32 + section_offset - (uint32_t)&Firmware));
  }

  return section;
}


/**
 * @brief  将firmware写入到app区
 * @param  None
 * @retval None
 */
void WriteFirmwareToApp(void)
{
  uint32_t i = 0;
  uint8_t section_count;
  uint32_t section_offset;
  uint32_t section_data_base;
  SectionTypeDef section = { 0, 0, 0, 0, 0 };

  //计算解密字符串
  AES_keyschedule_dec((uint32_t *)KeyStr, ExtKey);

  //获取Firmware信息
  GetFirmwareInfo();

  //检测文件crc
  if (CheckFileCRC() == TRUE)
  {
    uint8_t result = 1;

    //检查section crc
    for (section_count = 0; section_count < Firmware.SectionNumber; section_count++)
    {
      if (section_count == 0)
      {
        section_offset = 0;
      }
      else
      {
        section_offset += section.SectionLen + 0x10;
      }

      section = GetSectionInfo(section_offset);

      section_data_base = (uint32_t)(FIRMWARE_BASE_ADDRESS + ((uint32_t)&Firmware.Section.SectionData + section_offset - (uint32_t)&Firmware));
      if (CheckSectionCRC(section, section_data_base) == FALSE)
      {
        result = 0;
        break;
      }
    }
		
    if (result == 1)
    {
      //擦除APP和config区
      EraseAppSector();

      //恢复seed
      for (i = 0; i < 4; i++)
      {
        Seed[i] = SeedBeforeData[i];
      }

      //写入APP和Config
      for (section_count = 0; section_count < Firmware.SectionNumber; section_count++)
      {
        if (section_count == 0)
        {
          section_offset = 0;
        }
        else
        {
          section_offset += section.SectionLen + 0x10;
        }

        section = GetSectionInfo(section_offset);

        section_data_base = (uint32_t)(FIRMWARE_BASE_ADDRESS + ((uint32_t)&Firmware.Section.SectionData + section_offset - (uint32_t)&Firmware));

        if (Firmware.EncryptFlag == 1)
        {
          DecryptAndWrtieToApp(section, section_data_base);
        }
        else
        {
          HAL_FLASH_Unlock();
          for (i = 0; i < section.SectionRealLen; i += 4)
          {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, section.SectionAddress + i, (uint32_t)*(vu32 *)(section_data_base + i)) == HAL_OK)
            {
              /* Check the written value */
              if (*(uint32_t *)(section.SectionAddress + i) != (uint32_t)*(vu32 *)(section_data_base + i))
              {
                HAL_FLASH_Lock();
                NVIC_SystemReset();
              }
            }
						else
						{
								HAL_FLASH_Lock();
                NVIC_SystemReset();
						}
          }

          HAL_FLASH_Lock();
        }
      }
    }
		
    //擦除firmware备份区
    EraseFirmwareSector();
  }
}

bool CheckAppValidity(void)
{
  uint32_t i;
  uint32_t app_crc;
  uint32_t uid;
  uint32_t key_cal;
	uint32_t key_random[6];
	uint32_t key_seed[5] = {0x1234,0x3456,0x5555,0x5589,0x9954};
  uint32_t key_store;
	uint32_t result = 0;
	
  hcrc.Instance->CR = CRC_CR_RESET;

  for (i = APP_BASE_ADDRESS; i < APP_BASE_ADDRESS + NUMBER_OF_APP_PAGAES; i += 4)
  {
    hcrc.Instance->DR = (uint32_t)*(vu32 *)(i);
  }

  app_crc = hcrc.Instance->DR;

  uid = (uint32_t)*(vu32 *)(0x1FFF0000 + 0xF7E8);
  key_cal = app_crc ^ uid;
	key_cal +=10;
	
	for(i = 0 ; i<5 ; i++)
	{
		key_random[i] = key_cal^key_seed[i];
	}

	key_random[5] = key_cal;
	
	key_store = (uint32_t)*(vu32 *)(KEY_STORE_ADDRESS + 20);
	
  if (key_store == KEY_STORE)
  {
		FLASH_If_Erase(KEY_STORE_ADDRESS, 1);	
		result = FLASH_If_Write(KEY_STORE_ADDRESS, key_random, 6);
    return TRUE;
  }
  else
  {
    if (key_cal == key_store)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
}


/************************ (C) COPYRIGHT INMOTION *****END OF FILE****/
