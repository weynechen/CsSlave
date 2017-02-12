/**
 * @file        rgbif.h
 * @author      Weyne
 * @version     V01
 * @date        2016.11.20
 * @brief       rgb panel interface
 * @note				
 * @attention   COYPRIGHT WEYNE
 */
#include "sys.h"

void RGB_SPI_Test(void);

typedef enum 
{
	SPI_WRITE,
	SPI_READ,
}SPIRWTypeDef;

typedef enum
{
	SPI_COMMAND,	 
	SPI_DATA,	 
}SPIDataCommandTypeDef;

typedef enum
{
	SPI_LOW_BYTE,	
	SPI_HIGH_BYTE,
}SPIByteTypeDef;

typedef enum
{
	SPI_RISING,
	SPI_FALLING,
}SPIEdgeTypeDef;


/**
 * @brief RGB IO PIN ?????
 */
#define SPI_SDI          PEin(5)  /*< SPI SDI */
#define SPI_SDA          PEout(4) /*< SPI SDO */
#define SPI_SDA2          PEout(2) /*< SPI SDO */
#define SPI_SCK          PEout(3) /*< SPI CLK */
#define SPI_CS           PEout(0) /*< RGB 2 ?SPI??PIN,????? */
#define RGB_RESET    		 PEout(6) /*< RGB Reset pin */
#define SPI_RS     		 PEout(2) /*< SPI Shut down ??? */

void RGB_SPIWrite_16Bit(uint16_t address , uint8_t data , SPIEdgeTypeDef edge);
uint8_t RGB_SPIRead_16Bit(uint16_t address , SPIEdgeTypeDef edge);
void RGB_SPIWrite8Bit(uint8_t data ,SPIDataCommandTypeDef rs);
void RGB_SPIWrite9Bit(uint8_t data ,SPIDataCommandTypeDef rs);
void SPI_2DataLaneWritePixel(uint16_t color);
void SPI_WriteFrame(uint16_t color);

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
