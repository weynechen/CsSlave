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

#define APP_BASE_ADDRESS                      0x0801C800
#define VERSION_ADDRESS                       0x0803F800
#define CONFIG_BASE_ADDRESS                   ADDR_FLASH_PAGE_11
#define FIRMWARE_BASE_ADDRESS                 ADDR_FLASH_PAGE_17
#define FIRMWARE_UPGRADE_FLAG_BASE_ADDRESS    ADDR_FLASH_PAGE_56
#define NUMBER_OF_UPGRADE_PAGES               40
#define NUMBER_OF_CONFIG_PAGES                5
#define NUMBER_OF_APP_PAGAES                  40


#define CDC_USB 0
#define COMPOSITE_USB 1

#define USB_TYPE CDC_USB

/**
 * @brief 系统缓冲区的长度
 */
#define BUFFER_SIZE                           8192 //在SD卡DMA模式时，必须为512的倍数
#define  UART3_BUFFER_SIZE                     128

/**
 * @brief  系统配置数据长度
 */
#define LCD_PARA_LEN       18
#define LCD_INIT_LEN       3000
#define PATTERN_LEN        1024
#define MIPI_CONFIG_LEN    256
#define MAX_NAME_LEN       128
#define POWER_LEN          128
/**
 * @brief  系统配置数据结构体，这一部分数据会被烧录到flash
 */
typedef struct
{
  uint8_t PowerSettings;               /*< 默认电源设置 */
  uint8_t Backlight;                   /*< 背光电流设置 */
  uint8_t LCDTimingPara[LCD_PARA_LEN]; /*< LCD 时序参数设置 */
  uint8_t LCDInitCode[LCD_INIT_LEN];   /*< LCD 初始化设置,第一个和第二个字节表示长度*/
  uint8_t MIPIConfig[MIPI_CONFIG_LEN]; /*< MIPI 参数设置 第一个和第二个字节表示长度*/
  uint8_t Pattern[PATTERN_LEN];        /*< pattern 设置 第一个和第二个字节表示长度*/
  uint8_t ProjectName[MAX_NAME_LEN];   /*< 项目名称设置 */
  uint8_t IsAutoRun;                   /*< 是否自动跑 */
  uint8_t LcdType;                     /*< 接口类型 */
  uint8_t PowerOnSequence[POWER_LEN];  /*< 自定义上电时序*/
  uint8_t PowerOffSequence[POWER_LEN]; /*< 自定义下电时序*/
  uint8_t ConfigVersion;
  uint8_t Checksum;
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
#define FPGA_IO_MODE  0xFF000000
typedef struct
{
  char     Name[PATTERN_AMOUNT][PATTERN_NAME_LEN];
  uint16_t StayTime[PATTERN_AMOUNT];
  uint8_t  Counter;
  uint8_t  CurrentPattern;
	uint8_t  SDRAMCounter;
	uint32_t Data[PATTERN_AMOUNT];
} PatternPropertyTypeDef;

/**
 * @brief  数据是否准备好标志
 */
typedef enum
{
  DATA_NULL,
  DATA_READY,
} ComStateTypeDef;


typedef __IO uint32_t  vu32;

/**
 * @brief  操作数据包ID号，同一个数据包只能有一个ACTION.
 */
typedef enum
{
  ACT_RE_INIT_START,     /*< 重新初始化开始标志*/
  ACT_LCD_READ,          /*< 回读LCD寄存器，高速*/
  ACT_LCD_WRITE,         /*< 写LCD寄存器，高速模式*/
  ACT_SET_FRAME,         /*< 选择显示的画面*/
  ACT_FLASH_PARA,        /*< 固化调试好的参数到Flash*/
  ACT_FLASH_CONFIG_FILE, /*< 烧录配置文件 */
  ACT_CHANNEL_SEL,       /*< 选择通道 */
  ACT_UPGRADE_FIRMWARE,  /*< 更新固件 */
  ACT_REBOOT,
  ACT_GET_VERSION,
  ACT_READ_SSD2828,
  ACT_SET_SSD2828,
  ACT_RESET_SSD2828,
	ACT_SET_KEY,
  ACTION_NULL = 0xff /*< 空动作*/
} ActionIDTypeDef;

extern uint8_t SystemBuf[BUFFER_SIZE+1];
extern uint8_t RecBuffer[BUFFER_SIZE];
extern uint8_t Uart3RxBuffer[UART3_BUFFER_SIZE];
extern ConfigTypeDef SystemConfig;
extern LCDTimingParaTypeDef LCDTiming;
extern PatternPropertyTypeDef PatternProperty;
extern ActionIDTypeDef TaskID;
extern ComStateTypeDef USBState;
extern uint16_t USBDataShift;
extern uint8_t USBConnect;
extern uint8_t USBIdle;
extern uint8_t USBPlugin;
extern uint32_t RecCounter;
extern PackageDataStruct RecPackage;
extern uint8_t ConfigData[];

void FlashConfig(void);
void ReadSystemConfig(void);
void InitSystemConfig(void);
void SoftwareReset(void);
void GetFirmwareVersion(void);
void CacheData(void);

#define RED_LIGHT_ON 				HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin,GPIO_PIN_SET)
#define RED_LIGHT_OFF    		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin,GPIO_PIN_RESET)
#define GREEN_LIGHT_ON 				HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin,GPIO_PIN_SET)
#define GREEN_LIGHT_OFF    		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin,GPIO_PIN_RESET)


#endif
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
