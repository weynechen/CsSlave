/**
 * @file        rec.c
 * @author      Weyne
 * @version     V01
 * @date        2016.08.09
 * @brief       接收处理
 * @note
 * @attention   COYPRIGHT WEYNE
 */

#include "rec.h"
#include "sys.h"
#include "string.h"
#include "sysconfig.h"

/**
 * @brief  解包函数
 * @param  待解包数据指针
 * @retval 返回解包成功或者失败
 */
ResultTypeDef Unpacking(PackageDataStruct *package)
{
  uint16_t i = 0;

  if ((package->DataInBuff == NULL) && (package->DataInLen < MIN_PRO_NUM))
  {
    return PACK_FAIL;
  }

  if (package->DataInLen >= MIN_PRO_NUM)
  {
    if ((*(package->DataInBuff + package->DataInLen - 1) == P_TAIL) && (*(package->DataInBuff + package->DataInLen - 2) == P_TAIL))
    {
      i = MIN_PRO_NUM - 2;
      while (i++)
      {
        if (*(package->DataInBuff + package->DataInLen - i) == P_HEADER)
        {
          if (*(package->DataInBuff + package->DataInLen - (i + 1)) == P_HEADER)
          {
            uint8_t *pbuff = package->DataInBuff + package->DataInLen - (i - 1); // pbuff指向DataInBuff有效数据起始位置(即AA AA 后面的一位)
            uint16_t len = i - 3;                                                //i 的长度为数据包(AA AA ... 55 55) 长度减一，len为有效数据长度(除去AA AA 55 55部分，包含checksum)
            uint16_t j = 0;
            uint8_t checksum = 0;
            uint16_t data_out_count = 0;

            if (len > sizeof(SystemBuf)) //ParseBuffer的size要足够大
            {
              return PACK_FAIL;
            }

            for (j = 0; j < len; j++)
            {
              if (*(pbuff + j) == P_CTRL)
              {
                j++;
              }
              SystemBuf[data_out_count++] = *(pbuff + j);
              if (data_out_count == sizeof(SystemBuf))
              {
                package->DataID = ACTION_NULL;
                return PACK_FAIL;
              }
            }

            for (j = 0; j < data_out_count - 1; j++)
            {
              checksum += SystemBuf[j];
            }

            if (checksum == SystemBuf[data_out_count - 1])
            {
              SdkProtocolHeaderTypeDef *sdk = (SdkProtocolHeaderTypeDef *)SystemBuf;
              *package->DataOutLen = data_out_count - 1 - sizeof(SdkProtocolHeaderTypeDef);
              package->DataOutBuff = SystemBuf + sizeof(SdkProtocolHeaderTypeDef);

              if (sdk->DeviceAddr == LCD_ADDRESS)
              {
                package->DataID = (ActionIDTypeDef)sdk->FunctionCode;
              }
              else
              {
                package->DataID = ACTION_NULL;
              }

              return PACK_OK;
            }
            else
            {
              package->DataID = ACTION_NULL;
              *package->DataOutLen = 0;
              return PACK_FAIL;
            }
          }
        }
        if (i == package->DataInLen)
        {
          package->DataID = ACTION_NULL;
          return PACK_FAIL;
        }
      }
    }
  }

  return PACK_FAIL;
}


///**
// * @brief  CRC检查通讯数据
// * @param  None
// * @retval 数据OK或者NG
// */
//static DataStateTypeDef CheckData(void)
//{
//  uint16_t i;
//  uint16_t len;
//  uint32_t crc32 = 0;

//  len = (uint32_t)(RecBuffer[0] << 8 | RecBuffer[1]);
//  if (len % 4 != 0)
//  {
//    return DATA_NG;
//  }
//  if (len > BUFFER_SIZE)
//  {
//    return DATA_NG;
//  }

//  crc32 = (uint32_t)((uint32_t)RecBuffer[len] << 24 | (uint32_t)RecBuffer[len + 1] << 16 | (uint32_t)RecBuffer[len + 2] << 8 | (uint32_t)RecBuffer[len + 3]);

//  __HAL_CRC_DR_RESET(&hcrc);

//  for (i = 0; i < len; i += 4)
//  {
//    hcrc.Instance->DR = (uint32_t)((uint32_t)RecBuffer[i + 3] << 24 | (uint32_t)RecBuffer[i + 2] << 16 | (uint32_t)RecBuffer[i + 1] << 8 | (uint32_t)RecBuffer[i]);
//  }
//  if (crc32 != hcrc.Instance->DR)
//  {
//    return DATA_NG;
//  }

//  return DATA_OK;
//}

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
