/**
 * @file        res.c
 * @author      Weyne
 * @version     V01
 * @date        2016.09.04
 * @brief       格式化文件读写
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#include "res.h"
#include "ff.h"
#include "diskio.h"
#include "sysconfig.h"
#include "fpga.h"
#include "ack.h"

void Res_ReadPic(char *file_name)
{
  uint32_t a;
  FRESULT res;
  UINT br;      // File R/W count
  FIL fsrc;     // file objects
  uint32_t pic_size = 0;

  res = f_open(&fsrc, file_name, FA_OPEN_EXISTING | FA_READ); 
  if (res != 0)
  {
    UserPrintf("Error:open file error,error code %d \n", res);
    return;
  }

  for ( ; ; )
  {
    br = 1;
    res = f_read(&fsrc, SystemBuf, BUFFER_SIZE, &br);
    if (res || (br == 0))
    {
      break;                     // error or eof
    }
    for (a = 0; a < br; a++)
    {
      LcdDrvWriteData(SystemBuf[a]);
      pic_size++;
    }
   }

  if (pic_size != (uint32_t)LCDTiming.LCDH * LCDTiming.LCDV * 3)
  {
    UserPrintf("Error:picture size mismatch %d\n", pic_size);
  }

  f_close(&fsrc);
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
