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



/* 数据缓冲区，接收串口，USB，以及读写SDCard,flash用*/
uint8_t SystemBuf[BUFFER_SIZE];
ConfigTypeDef SystemConfig;
LCDTimingParaTypeDef LCDTiming;
PatternPropertyTypeDef PatternProperty;
ActionIDTypeDef TaskID = ACTION_NULL;

//信号量
ComStateTypeDef UartState = DATA_NULL;
ComStateTypeDef USBState =  DATA_NULL;
uint8_t USBConnect =0;

void FlashConfig(void)
{
	if(FLASH_If_Erase(CONFIG_BASE_ADDRESS,NUMBER_OF_CONFIG_PAGES) == FLASHIF_ERASEKO)
	{
		printf("Error:flash config error\n");
	}
	else
	{
		FLASH_If_Write(CONFIG_BASE_ADDRESS,(uint32_t *)&SystemConfig, sizeof(SystemConfig)/sizeof(uint32_t));
	}
}

void ReadSystemConfig(void)
{
		SystemConfig = *((ConfigTypeDef *)CONFIG_BASE_ADDRESS);	
		if(SystemConfig.Backlight < 200)
		{
			TaskID = RE_INIT_START;
		}
}



/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
