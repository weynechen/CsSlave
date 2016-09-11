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
 
typedef __IO uint32_t  vu32;
 
typedef void (*IapFun)(void); 
#define APP_BASE_ADDRESS 0x0801C800
 

void JumpToApp(void);
 
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址 

#endif
/************************ (C) COPYRIGHT  WEYNE *****END OF FILE****/
