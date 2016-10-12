/**
  * @file        transmit.c
  * @author      陈维
  * @version     V01
  * @date        2016.09.21
  * @brief       打包
  * @note        
  *
  * @attention   COYPRIGHT INMOTION ROBOT
  **/
#include "ack.h"  
#include <stdarg.h>
#include "sysconfig.h"
#include "usbd_cdc_if.h"
#include "usart.h"


static char sprint_buf[512];

/**
  * @brief  数据打包函数
  * @param  package :待打包数据信息
  * @note   需要确保输入的数据信息正确，特别是out buffer最好要是待打包数据长度的两倍
  * @retval None
  */
ResultTypeDef Package(PackageDataStruct package)
{
	uint32_t j = 0;
	uint32_t i = 0;
	SdkProtocolHeaderTypeDef sdk_header;
	uint8_t *psdk = (uint8_t *)&sdk_header;
	uint8_t checksum = 0;
	
	if((package.DataInBuff == NULL) || (package.DataOutBuff == NULL) || (package.DataOutLen == NULL))
		return PACK_FAIL;
	
	sdk_header.DeviceAddr = 0x10;
	sdk_header.FunctionCode = package.DataID;
	sdk_header.StartAddr = 0;
	sdk_header.Len = package.DataInLen;

	*(package.DataOutBuff+i ++) = P_HEADER;
	*(package.DataOutBuff+i ++) = P_HEADER;

	for(j = 0 ; j<sizeof(SdkProtocolHeaderTypeDef);j++)
	{
		if((*(psdk+j) == P_CTRL) || (*(psdk+j) == P_HEADER) || (*(psdk+j) == P_TAIL))
		{
			*(package.DataOutBuff+i ++) = P_CTRL;
		}
		*(package.DataOutBuff+i ++) = *(psdk+j);
		checksum += *(psdk+j);
	}
	
	for(j = 0 ; j<package.DataInLen; j++)
	{
		if((*(package.DataInBuff+j) == P_CTRL) || (*(package.DataInBuff+j) == P_HEADER) || (*(package.DataInBuff+j) == P_TAIL))
		{
			*(package.DataOutBuff+i++) = P_CTRL;
		}
		checksum += *(package.DataInBuff+j);
		*(package.DataOutBuff+i++) = *(package.DataInBuff+j);
	}
	
	if((checksum == P_CTRL) || (checksum == P_HEADER) || (checksum == P_TAIL))
	{
		*(package.DataOutBuff+i++) = P_CTRL;
	}
	*(package.DataOutBuff+i++) = checksum;
	
	*(package.DataOutBuff+i++) = P_TAIL;
	*(package.DataOutBuff+i++) = P_TAIL;
	
	*package.DataOutLen = i;
	
	return PACK_OK;
}


void UserPrintf(char * fmt, ...)
{
	va_list args;
	uint32_t out_len;
	PackageDataStruct  package;

	va_start(args, fmt);
	package.DataInLen = vsprintf(sprint_buf, fmt, args);
	va_end(args);

	package.DataID = ACK_STRING;
	package.DataInBuff = (uint8_t *)sprint_buf;
	package.DataOutBuff = SystemBuf;
	package.DataOutLen = &out_len;
	
	Package(package);
	
	if(USBConnect == 1)
	{
		CDC_Transmit_FS((uint8_t *)SystemBuf,out_len);
	}
		HAL_UART_Transmit(&huart1,SystemBuf,out_len,10);
}

  
/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/

