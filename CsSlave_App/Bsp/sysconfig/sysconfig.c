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


/* 数据缓冲区，读写SDCard,flash等用*/
uint8_t SystemBuf[BUFFER_SIZE];
/*接收串口和USB口数据用*/
uint8_t RecBuffer[BUFFER_SIZE];
/*接收数据计数*/
uint32_t RecCounter = 0;
/*接收封包信息*/
PackageDataStruct RecPackage;

ConfigTypeDef SystemConfig;
LCDTimingParaTypeDef LCDTiming;
PatternPropertyTypeDef PatternProperty;
ActionIDTypeDef TaskID = ACTION_NULL;

//信号量
ComStateTypeDef USBState = DATA_NULL;
uint16_t USBDataShift =  0;
uint8_t USBPlugin = 0;
uint8_t USBConnect =0;
uint8_t USBIdle = 0;

void FlashConfig(void)
{
	if(FLASH_If_Erase(CONFIG_BASE_ADDRESS,NUMBER_OF_CONFIG_PAGES) == FLASHIF_ERASEKO)
	{
		UserPrintf("Error:flash config error\n");
	}
	else
	{
		uint16_t config_size = sizeof(SystemConfig)/sizeof(uint32_t) + 1; // 防止不能被4整除
		if(FLASH_If_Write(CONFIG_BASE_ADDRESS,(uint32_t *)&SystemConfig, config_size) == 0)
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
		if(SystemConfig.Backlight < 200)
		{
			TaskID = RE_INIT_START;
		}
		else
		{
			UserPrintf("Error: LCD config not found or wrong\n");
		}
}

void InitSystemConfig(void)
{
	RecPackage.DataID = ACTION_NULL;
	RecPackage.DataInBuff = RecBuffer;
	RecPackage.DataOutBuff = SystemBuf;
	RecPackage.DataOutLen = &RecCounter;
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
