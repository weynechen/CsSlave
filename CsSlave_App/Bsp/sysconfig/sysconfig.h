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
#include "rec.h"
#include "flash_if.h"

#define APP_BASE_ADDRESS 0x0801C800
#define CONFIG_BASE_ADDRESS ADDR_FLASH_PAGE_11
#define NUMBER_OF_CONFIG_PAGES 5
/**
 * @brief 系统缓冲区的长度
 */
#define BUFFER_SIZE    8192


/**
 * @brief  系统配置数据长度
 */
#define LCD_PARA_LEN       18
#define LCD_INIT_LEN       3000
#define PATTERN_LEN        1024
#define MIPI_CONFIG_LEN    256
#define MAX_NAME_LEN       128

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


/**
 * @breif  LCD时序参数定义
 */
typedef struct
{
  uint16_t DCLK;
  uint16_t LCDH;
  uint16_t LCDV;

  uint16_t HBPD;
  uint16_t HFPD;
  uint16_t HSPW;

  uint16_t VBPD;
  uint16_t VFPD;
  uint16_t VSPW;
} LCDTimingParaTypeDef;


/**
 * @breif  pattern 属性
 */
#define PATTERN_AMOUNT      20
#define PATTERN_NAME_LEN    30

typedef struct
{
  char     Name[PATTERN_AMOUNT][PATTERN_NAME_LEN];
  uint16_t StayTime[PATTERN_AMOUNT];
  uint8_t  Counter;
  uint8_t  CurrentPattern;
} PatternPropertyTypeDef;

/**
 * @brief  数据是否准备好标志
 */
typedef enum
{
	DATA_NULL,	
	DATA_READY,
}ComStateTypeDef;

extern uint8_t SystemBuf[];
extern uint8_t RecBuffer[];
extern ConfigTypeDef SystemConfig;
extern LCDTimingParaTypeDef LCDTiming;
extern PatternPropertyTypeDef PatternProperty;
extern ActionIDTypeDef TaskID;
extern ComStateTypeDef USBState;
extern uint16_t USBDataShift;
extern uint8_t USBConnect;
extern uint8_t USBIdle;
extern uint8_t USBPlugin;

void FlashConfig(void);
void ReadSystemConfig(void);
#endif
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
