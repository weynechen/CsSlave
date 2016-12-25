#include "stm32f1xx_hal.h"
#include "ssd2828.h"
#include "mtp.h"

#define GP_COMMAD_PA(x) SSD2828_GenericLongWrite(x)
#define SPI_WriteData(x) SSD2828WriteData(x)
#define Delay(x) HAL_Delay(x)

void SetMTPCode(uint8_t vcom)
{
	GP_COMMAD_PA(2);SPI_WriteData(0xE0);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xE1);SPI_WriteData(0x93);
	GP_COMMAD_PA(2);SPI_WriteData(0xE2);SPI_WriteData(0x65);
	GP_COMMAD_PA(2);SPI_WriteData(0xE3);SPI_WriteData(0xF8);

	GP_COMMAD_PA(2);SPI_WriteData(0xE5);SPI_WriteData(0x00);

	GP_COMMAD_PA(1);SPI_WriteData(0x28);
	Delay(50);
	GP_COMMAD_PA(1);SPI_WriteData(0x10);
	Delay(500);

	//page 1
	GP_COMMAD_PA(2);SPI_WriteData(0xE0);SPI_WriteData(0x01);//set vgh=7.5v
	GP_COMMAD_PA(2);SPI_WriteData(0x1D);SPI_WriteData(0x01);
	GP_COMMAD_PA(2);SPI_WriteData(0x1E);SPI_WriteData(0x03);

	//page 0
	GP_COMMAD_PA(2);SPI_WriteData(0xE0);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);

	GP_COMMAD_PA(1);SPI_WriteData(0x11);  	// sleep out 
	Delay(500);

	//page 0 OTP key
	GP_COMMAD_PA(2);SPI_WriteData(0xE0);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0x84);SPI_WriteData(0x5A);
	GP_COMMAD_PA(2);SPI_WriteData(0x85);SPI_WriteData(0xA5);
	//OTP index

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x23); //WD 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x1F); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(vcom); //VCOM  
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x30); //VCOM_R 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x3E); //POWER
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);


	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x4B); //GAMMA
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x53); //53 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x57); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x59); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x6A); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x68); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x6B); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x73); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x87); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x01);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x00); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x01);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x08); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x01);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x12); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x01);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x28); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x01);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x3E); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x01);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x54); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x01);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x6A); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x01);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x7E); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x03);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x28); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x03);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x05); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x03);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x08); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	GP_COMMAD_PA(2);SPI_WriteData(0xEA);SPI_WriteData(0x03);
	GP_COMMAD_PA(2);SPI_WriteData(0xEB);SPI_WriteData(0x25); // 
	Delay(50);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x02);
	GP_COMMAD_PA(2);SPI_WriteData(0xEC);SPI_WriteData(0x03);
	Delay(500);

	//end OTP
	GP_COMMAD_PA(2);SPI_WriteData(0xE0);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0x84);SPI_WriteData(0x00);
	GP_COMMAD_PA(2);SPI_WriteData(0x85);SPI_WriteData(0x00);
}

