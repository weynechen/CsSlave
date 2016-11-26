#ifndef __CH376_H
#define __CH376_H

#ifndef uint8_t
typedef unsigned char  uint8_t;
#endif
#include "coolsaven_config.h"

/* 附加的USB操作状态定义 */
#define	ERR_USB_UNKNOWN		0xFA				/* 未知错误,不应该发生的情况,需检查硬件或者程序错误 */

#define SPI_CS_SET()   GPIO_SetBits(GPIO_SPI_CS_PORT,GPIO_SPI_CS_PIN) 
#define SPI_CS_CLR()   GPIO_ResetBits(GPIO_SPI_CS_PORT,GPIO_SPI_CS_PIN) 

uint8_t CH376_Init(void);
uint8_t mWaitInterrupt( void );
void xWriteCH376Cmd( uint8_t mCmd );
uint8_t	Query376Interrupt( void );
//SPI读写一字节数据
uint8_t CH376_ReadWrite(uint8_t writedat);
void xWriteCH376Data(uint8_t mData);
uint8_t xReadCH376Data(void);

extern void Delayus(uint32_t us);
extern void Delayms(uint32_t ms);
#endif
