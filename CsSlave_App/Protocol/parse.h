/**
 * @file        parse.h
 * @author      Weyne
 * @version     V01
 * @date        2016.08.09
 * @brief       通讯协议定义及配置
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#ifndef _PARSE_H_
#define _PARSE_H_
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
 * @brief  系统配置数据长度
 */
#define LCD_PARA_LEN       18
#define LCD_INIT_LEN       3000
#define PATTERN_LEN        1024
#define MIPI_CONFIG_LEN    256
#define MAX_NAME_LEN 128

/**
 * @brief  系统配置数据结构体，这一部分数据会被烧录到flash
 */
typedef struct
{
  uint8_t PowerSettings;               /*< 电源设置 */
  uint8_t Backlight;                   /*< 背光电流设置 */
  uint8_t LCDTimingPara[LCD_PARA_LEN]; /*< LCD 时序参数设置 */
  uint8_t LCDInitCode[LCD_INIT_LEN];   /*< LCD 初始化设置 */
  uint8_t MIPIConfig[MIPI_CONFIG_LEN]; /*< MIPI 参数设置 */
  uint8_t Pattern[PATTERN_LEN];        /*< pattern 设置 */
  uint8_t ProjectName[MAX_NAME_LEN];   /*< 项目名称设置 */
  uint8_t IsAutoRun;                   /*< 是否自动跑 */
} ConfigTypeDef;

extern ConfigTypeDef SystemConfig;

/**
 * @brief  系统配置数据包ID号
 */
typedef enum
{
  LCD_POWER,         /*< 初始化电源*/
  LCD_BACKLIGHT,     /*< 初始化背光*/
  LCD_TIMING_PARA,   /*< 初始化lcd pclk，前后肩等参数*/
  LCD_INIT_CODE,     /*< 初始化lcd*/
  SSD2828_PARA,      /*< 初始化SSD2828*/
  PATTERN,           /*< 初始化pattern*/
  PROJECT_NAME,      /*<  项目名称*/
  AUTO_RUN,          /*< 自动切换画面*/
} ConfigIDTypeDef;

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

#endif
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
