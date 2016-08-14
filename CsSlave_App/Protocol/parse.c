/**
 * @file        parse.c
 * @author      Weyne
 * @version     V01
 * @date        2016.08.09
 * @brief       通讯协议
 * @note
 *				      协议采用头 + checksum + 尾 的形式
 *              解析分三步:
 * 							- 第一步，检查数据包
 *              - 第二步，解析ACTION
 *              - 第三步，处理ACTION(在主线程中完成)
 * @attention   COYPRIGHT WEYNE
 */

#include "parse.h"
#include "action.h"
#include "sys.h"

static DataStateTypeDef CheckData(void)
{
	return DATA_OK;
}

/**
	* @brief  解析通讯数据
	* @param  p: 数据缓冲区头指针
	* @retval 解包成功或者失败
  */
PackFlagTypeDef ParseComData(uint8_t *p)
{
	if(CheckData() != DATA_OK)
		return P_FAIL;
	
	
	
}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
