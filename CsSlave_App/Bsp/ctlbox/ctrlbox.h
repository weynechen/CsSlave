/*
 * @file        ctrlbox.h
 * @author      Weyne
 * @version     V01
 * @date        2016.11.13
 * @brief       控制盒驱动
 * @note
 * @attention   COYPRIGHT WEYNE
 */
#ifndef _CTRL_BOX_H_
#define _CTRL_BOX_H_

typedef enum
{
  KEY_UP,
  KEY_DOWN,
  KEY_POWER,
	KEY_MTP,
  KEY_TP,
  KEY_NULL = 0xff,
} KeyTypeDef;

void ScanKey(void);

extern KeyTypeDef CtrlKey;

#endif

/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
