/**
 * @file        sysconfig.c
 * @author      Weyne
 * @version     V01
 * @date        2016.08.23
 * @brief       系统配置部分
 * @note				系统配置定义，flash操作等
 * @attention   COYPRIGHT WEYNE
 */
#include "sysconfig.h"

/* 数据缓冲区，接收串口，USB，以及读写SDCard,flash用*/
uint8_t SystemBuf[BUFFER_SIZE];
ConfigTypeDef SystemConfig;
LCDTimingParaTypeDef LCDTiming;
PatternPropertyTypeDef PatternProperty;


/************************ (C) COPYRIGHT WEYNE *****END OF FILE****/
