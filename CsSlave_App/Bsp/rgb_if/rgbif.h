/**
 * @file        rgbif.h
 * @author      Weyne
 * @version     V01
 * @date        2016.11.20
 * @brief       rgb panel interface
 * @note				
 * @attention   COYPRIGHT WEYNE
 */
 
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
 
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
