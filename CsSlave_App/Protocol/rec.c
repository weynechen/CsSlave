/**
 * @file        rec.c
 * @author      Weyne
 * @version     V01
 * @date        2016.08.09
 * @brief       接收处理
 * @note
 *				      协议: Len , interface number , ID ,datalen , section , data... , crc32
								 len : 2 byte 包括len本身，不包括crc
								 interface number : 1 byte 表明使用哪个接口(usart1,usb,usart2...)
								 ID ：1 byte 数据包ID
								 datalen : 2 byte 数据长度
								 section : 1 byte 当数据大于BUFFER SIZE时，需要分段传输
								 data : 数据
								 crc32 : 4 byte 对不包括数据本身的CRC校验
 
 *              解析分三步:
 * 							- 第一步，检查数据包CRC
 *              - 第二步，解析数据包
 *              - 第三步，处理数据包或者发出信号量
 * @attention   COYPRIGHT WEYNE
 */

#include "rec.h"
#include "sys.h"
#include "crc.h"
#include "string.h"
#include "sysconfig.h"
 
/**
	* @brief  CRC检查通讯数据
	* @param  None
	* @retval 数据OK或者NG
  */
static DataStateTypeDef CheckData(void)
{
	uint16_t i;
	uint16_t len;
	uint32_t crc32 = 0;
	len = (uint32_t)(RecBuffer[0]<<8 | RecBuffer[1]);
	if(len % 4 != 0)
		return	DATA_NG;
	if(len > BUFFER_SIZE)
		return DATA_NG;
	
	crc32 = (uint32_t)((uint32_t)RecBuffer[len]<<24 | (uint32_t)RecBuffer[len+1]<<16 | (uint32_t)RecBuffer[len+2]<<8 | (uint32_t)RecBuffer[len+3]);
	
	__HAL_CRC_DR_RESET(&hcrc);
	
	for(i=0;i<len;i+=4)
	{
		hcrc.Instance->DR = (uint32_t)((uint32_t)RecBuffer[i+3]<<24 | (uint32_t)RecBuffer[i+2]<<16 | (uint32_t)RecBuffer[i+1]<<8 | (uint32_t)RecBuffer[i]);
	}
	if(crc32 != hcrc.Instance->DR)
		return DATA_NG;
	
	return DATA_OK;
}

/**
	* @brief  解析通讯数据
	* @param  None
	* @retval 解包成功或者失败
  */
PackFlagTypeDef ParseComData(void)
{
	InterfaceTypeDef interface;

	if(CheckData() != DATA_OK)
		return P_FAIL;

  interface = (InterfaceTypeDef)RecBuffer[2];

	if((interface == IF_UART1) || (interface == IF_USB))
	{
		TaskID = (ActionIDTypeDef) RecBuffer[3];
		switch(TaskID)
		{
			case RE_INIT_START:
					memcpy(&SystemConfig , &RecBuffer[4] , sizeof(SystemConfig));
				break;

			default:
				break;
		}

	}
	
	return P_SUCCESS;
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
