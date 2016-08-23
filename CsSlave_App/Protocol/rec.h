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


/**
 * @brief  接口定义
 */
typedef enum
{
	IF_UART1,
	IF_USB,
	IF_UART3,
}InterfaceTypeDef;

/**
 * @brief  操作数据包ID号，同一个数据包只能有一个ACTION.
 */
typedef enum
{
  RE_INIT_START,     /*< 重新初始化开始标志*/
  LCD_READ,          /*< 回读LCD寄存器，高速*/
  LCD_WRITE,         /*< 写LCD寄存器，高速模式*/
  SET_FRAME,         /*< 选择显示的画面*/
  FLASH_PARA,        /*< 固化调试好的参数到Flash*/
  FLASH_CONFIG_FILE, /*< 烧录配置文件 */
  CHANNEL_SEL,        /*< 选择通道 */
	UPDATE_FIRMWARE,    /*< 更新固件 */
	ACTION_NULL = 0xff /*< 空动作*/
}ActionIDTypeDef;

/**
 * @brief  数据包头尾控制字
 */
typedef enum
{
  P_HEAD = 0xAA,
  P_TAIL = 0x55,
  P_CTRL = 0xA5,
} PackCtrlWordTypeDef;

/**
 * @brief  解包标志位
 */
typedef enum
{
  P_SUCCESS = 0xFF,
  P_FAIL = 0x00	
}PackFlagTypeDef;

/**
 * @brief  数据校验标志
 */
typedef enum
{
	DATA_NG,	
	DATA_OK,
}DataStateTypeDef;

/**
 * @brief  数据是否准备好标志
 */
typedef enum
{
	DATA_NULL,	
	DATA_READY,
}ComStateTypedef;

extern ActionIDTypeDef TaskID ;


PackFlagTypeDef ParseComData(void);

#endif
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
