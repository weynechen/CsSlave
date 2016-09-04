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
	volatile uint32_t crc32 = 0;
	volatile uint32_t crc32_result = 0;
	len = (uint32_t)(SystemBuf[0]<<8 | SystemBuf[1]);
	if(len % 4 != 0)
		return	DATA_NG;
	if(len > BUFFER_SIZE)
		return DATA_NG;
	
	crc32 = (uint32_t)((uint32_t)SystemBuf[len]<<24 | (uint32_t)SystemBuf[len+1]<<16 | (uint32_t)SystemBuf[len+2]<<8 | (uint32_t)SystemBuf[len+3]);
	
	__HAL_CRC_DR_RESET(&hcrc);
	
	for(i=0;i<len;i+=4)
	{
		hcrc.Instance->DR = (uint32_t)((uint32_t)SystemBuf[i+3]<<24 | (uint32_t)SystemBuf[i+2]<<16 | (uint32_t)SystemBuf[i+1]<<8 | (uint32_t)SystemBuf[i]);
	}
	crc32_result = hcrc.Instance->DR;
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

  interface = (InterfaceTypeDef)SystemBuf[2];

	if((interface == IF_UART1) || (interface == IF_USB))
	{
		ActionIDTypeDef id = (ActionIDTypeDef) SystemBuf[3];
		switch(id)
		{
			case RE_INIT_START:
					memcpy(&SystemConfig , &SystemBuf[4] , sizeof(SystemConfig));
					TaskID = RE_INIT_START;
				break;

			default:
				break;
		}

	}
	
	return P_SUCCESS;
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
