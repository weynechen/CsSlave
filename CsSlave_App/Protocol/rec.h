/**
 * @file        rec.h
 * @author      Weyne
 * @version     V01
 * @date        2016.08.09
 * @brief       接收通讯协议定义及配置
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#ifndef _REC_H_
#define _REC_H_
#include "stm32f1xx_hal.h"
#include "pro.h"

/**
 * @brief  接口定义
 */
typedef enum
{
	IF_UART1,
	IF_USB,
	IF_UART3,
}InterfaceTypeDef;

ResultTypeDef Unpacking(PackageDataStruct *package);

#endif
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
