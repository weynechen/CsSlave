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

void Res_ReadPic(char *file_name)
{
  unsigned int a, temp1 = 0;
  FRESULT res;
  FATFS fs;     // Work area (file system object) for logical drive
  UINT br;      // File R/W count
  FIL fsrc;     // file objects
  uint32_t pic_size = 0;

  disk_initialize(0);

  f_mount(&fs,0,0);                                            // 把盘符0分给fs

  res = f_open(&fsrc, file_name, FA_OPEN_EXISTING | FA_READ); //打开对应文件
  if (res != 0)
  {
    printf("Error:open file error,error code %d \n", res);
    return;
  }
  br = 1;

  for ( ; ; )
  {
    res = f_read(&fsrc, SystemBuf, BUFFER_SIZE - 1, &br);//res = f_read(&fsrc, gbuffer, sizeof(gbuffer), &br);

    for (a = 0; a < br; a++)
    {
      temp1 = (uint8_t)(SystemBuf[a]);
      LcdDrvWriteData(temp1);
      pic_size++;
    }

    if (res || (br == 0))
    {
      break;                     // error or eof
    }
  }

  printf("Info:write picture size %d\n", pic_size);

  f_close(&fsrc);	
}


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
