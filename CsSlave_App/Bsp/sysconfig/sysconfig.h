/**
 * @file        sysconfig.h
 * @author      Weyne
 * @version     V01
 * @date        2016.08.23
 * @brief       系统配置部分
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#ifndef _SYSCONFIG_H
#define _SYSCONFIG_H
#include "stm32f1xx_hal.h"

/**
* @brief 系统缓冲区的长度
*/
#define BUFFER_SIZE 5120


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
  uint8_t LCDInitCode[LCD_INIT_LEN];   /*< LCD 初始化设置,第一个和第二个字节表示长度*/
  uint8_t MIPIConfig[MIPI_CONFIG_LEN]; /*< MIPI 参数设置 第一个和第二个字节表示长度*/
  uint8_t Pattern[PATTERN_LEN];        /*< pattern 设置 第一个和第二个字节表示长度*/
  uint8_t ProjectName[MAX_NAME_LEN];   /*< 项目名称设置 */
  uint8_t IsAutoRun;                   /*< 是否自动跑 */
} ConfigTypeDef;


extern uint8_t SystemBuf[];
extern ConfigTypeDef SystemConfig;
#endif 
 /************************ (C) COPYRIGHT WEYNE *****END OF FILE****/


