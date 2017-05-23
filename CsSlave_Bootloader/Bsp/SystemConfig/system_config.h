/**
 * @file        system_config.h
 * @author      Weyne
 * @version     V01
 * @date        2016.09.11
 * @brief       系统配置部分
 * @note				
 * @attention   COYPRIGHT WEYNE
 */
 
#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

#include "stm32f1xx_hal.h"
#include "flash_if.h"

#define APP_BASE_ADDRESS 0x0801C800
#define CONFIG_BASE_ADDRESS ADDR_FLASH_PAGE_11
#define FIRMWARE_BASE_ADDRESS ADDR_FLASH_PAGE_17
#define FIRMWARE_UPGRADE_FLAG_BASE_ADDRESS ADDR_FLASH_PAGE_56
#define KEY_STORE_ADDRESS ADDR_FLASH_PAGE_10
#define BOOT_KEY_ADDRESS ADDR_FLASH_PAGE_5
#define NUMBER_OF_UPGRADE_PAGES 40
#define NUMBER_OF_CONFIG_PAGES 5
#define NUMBER_OF_APP_PAGAES 40


typedef __IO uint32_t  vu32;
 
typedef void (*IapFun)(void); 

typedef enum
{
	FALSE,
	TRUE,
}bool;


void JumpToApp(void);
 
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址 

#endif
/************************ (C) COPYRIGHT  WEYNE *****END OF FILE****/
