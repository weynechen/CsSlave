/**
 * @file        sysconfig.c
 * @author      Weyne
 * @version     V01
 * @date        2016.08.23
 * @brief       系统配置部分
 * @note				系统配置定义，flash操作等
 * @attention   COYPRIGHT WEYNE
 */
#include "sysconfig.h"
#include "ack.h"
#include "string.h"

#define KEY_STORE_ADDRESS ADDR_FLASH_PAGE_10
#define BOOT_KEY_ADDRESS ADDR_FLASH_PAGE_5

//KEY
const uint32_t __attribute__((at(KEY_STORE_ADDRESS + 20))) KeyStore = 0x5A2F5D81;

/* 数据缓冲区，读写SDCard,flash等用*/
uint8_t SystemBuf[BUFFER_SIZE + 1];
/*接收串口和USB口数据用*/
uint8_t RecBuffer[BUFFER_SIZE];
uint8_t Uart3RxBuffer[UART3_BUFFER_SIZE];
/*接收有效数据计数*/
uint32_t RecCounter = 0;
/*接收封包信息*/
PackageDataStruct RecPackage;
/*缓存一些配置数据*/
uint8_t ConfigData[128];
uint8_t Security = 0x18;

static const uint8_t Version[4] = 
{
  [3] = 2,
  [2] = 3,
  [0] = 1,
};
ConfigTypeDef SystemConfig =
{
  .ConfigVersion = 1,
};
LCDTimingParaTypeDef LCDTiming;
PatternPropertyTypeDef PatternProperty;
ActionIDTypeDef TaskID = ACTION_NULL;

//信号量
ComStateTypeDef USBState = DATA_NULL;
uint16_t USBDataShift = 0;
uint8_t USBPlugin = 0;
uint8_t USBConnect = 0;
uint8_t USBIdle = 0;

void FlashConfig(void)
{
  if (FLASH_If_Erase(CONFIG_BASE_ADDRESS, NUMBER_OF_CONFIG_PAGES) == FLASHIF_ERASEKO)
  {
    UserPrintf("Error:flash config error\n");
  }
  else
  {
    uint16_t config_size = sizeof(SystemConfig) / sizeof(uint32_t) + 1; // 防止不能被4整除
    if (FLASH_If_Write(CONFIG_BASE_ADDRESS, (uint32_t *)&SystemConfig, config_size) == 0)
    {
      UserPrintf("Info:Flash success\n");
    }
    else
    {
      UserPrintf("Error:Flash error!Please retry\n");
    }
  }
}

void ReadSystemConfig(void)
{
  SystemConfig = *((ConfigTypeDef *)CONFIG_BASE_ADDRESS);
  if (SystemConfig.Backlight < 200)
  {
    UserPrintf("Project:%s\n",SystemConfig.ProjectName);
    TaskID = ACT_RE_INIT_START;
  }
  else
  {
    UserPrintf("Error: LCD config not found or wrong\n");
  }
}

static uint32_t CalSecurityCode(void)
{
  uint32_t i;

  typedef struct
  {
    uint32_t id1;
    uint32_t id2;
    uint32_t id3;
  } UIDTypeDef;

  UIDTypeDef id = *(volatile UIDTypeDef *)(0x1FFF0000 + 0xF7E8);
  uint32_t result = 0;

  for (i = 0x08000000; i < BOOT_KEY_ADDRESS; i += 4)
  {
    result ^= *((volatile uint32_t *)i);
  }

  result ^= 0x12345678;
  result ^= id.id1;
  result ^= id.id2;
  result ^= id.id3;

  return result;
}

static uint8_t CheckSecurity(void)
{
  uint32_t key_store = (uint32_t) * (vu32 *)(BOOT_KEY_ADDRESS);
  uint32_t key_cal = CalSecurityCode();

  if (key_cal == key_store)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void InitSystemConfig(void)
{
  if (CheckSecurity() == 0)
  {
    Security = 0;
    UserPrintf("Error: bad firmware \n");
    while (1)
      ;
  }

  RecPackage.DataID = ACTION_NULL;
  RecPackage.DataInBuff = RecBuffer;
  RecPackage.DataOutBuff = SystemBuf;
  RecPackage.DataOutLen = &RecCounter;

  memset(&PatternProperty, 0, sizeof(PatternProperty));
}

void GetFirmwareVersion(void)
{
  uint32_t version = *(uint32_t *)VERSION_ADDRESS;
  const uint8_t *p = (uint8_t *)&version;

  if(version == 0xffffffff)
  {
    p = Version;
  }
  UserPrintf("Info:Version--v%d.%d.%d\n", *(p + 3), *(p + 2), *p);
}

void CacheData(void)
{
  switch (TaskID)
  {
  case ACT_RE_INIT_START:
    memcpy(&SystemConfig, RecPackage.DataOutBuff, sizeof(SystemConfig));
    break;

  case ACT_READ_SSD2828:
    ConfigData[0] = *RecPackage.DataOutBuff;
    break;

  case ACT_SET_KEY:
    ConfigData[0] = *RecPackage.DataOutBuff;
    break;

  case ACT_SET_SSD2828:
    memcpy(ConfigData, RecPackage.DataOutBuff, RecCounter);
    break;

  default:
    break;
  }
}

/**
 * @brief  软件复位
 * @param  None
 * @retval None
 */
void SoftwareReset(void)
{
  __set_FAULTMASK(1);     // close all interrupt
  SCB->VTOR = 0x08000000; //re-located boot address
  __DSB();                /* Ensure all outstanding memory accesses included
                             *  buffered write are completed before reset */
  SCB->AIRCR = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                SCB_AIRCR_VECTCLRACTIVE_Msk |
                SCB_AIRCR_SYSRESETREQ_Msk); /* Keep priority group unchanged */
  __DSB();                                  /* Ensure completion of memory access */
  while (1)
  {
  }
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
