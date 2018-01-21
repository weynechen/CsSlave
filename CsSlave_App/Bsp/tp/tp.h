/**
* @file         tp.h
* @author       Weyne Chen
* @version      V01
* @date         2017.08.03
* @brief         
* @note          
* @attention    COPYRIGHT WEYNE
**/

#ifndef __TP_H
#define __TP_H
#include "ppro.h"
#include "stdbool.h"

#define CELL_DIV_H    9
#define CELL_DIV_V    13

void TP_Callback(PproTypeDef *data);
uint8_t TP_StartTest(void);
bool TP_DrawLine(void);
bool IsTPToggle(void);
void TP_DrawBG(void);
void SetCellOrLine(bool is_cell);
void TP_SendData(uint8_t pid, uint16_t data);
typedef enum {
    TP_START,  /*开始测试*/
    TP_ECHO,   /*测试开始后的心跳信号*/
		TP_NO_FILE,
    TP_COORDINATE,
    TP_FIRMWARE_VERSION,  
    TP_POWER_OFF,  
    TP_PROGRESS,  
    TP_RESULT = 128, /*测试结果*/
} TPPIDTypeDef;

#endif
/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
