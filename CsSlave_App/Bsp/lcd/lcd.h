
/**
 * @file        lcd.h
 * @author      Weyne
 * @version     V01
 * @date        2016.08.23
 * @brief       LCD驱动部分
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#ifndef _LCD_H_
#define _LCD_H_
typedef enum
{
		MIPI_START,
		MIPI_DCS,
		MIPI_GP,
		MIPI_DELAY,
		MIPI_WRITE,
		MIPI_READ,
		MIPI_END
}MipiTypeDef;

typedef enum
{
		NO_PACKAGE,
		GP,
		DCS
}MipiPackageDef;
		
		
void Lcd_ReInit(void);
#endif
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/

