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

void TP_Callback(PproTypeDef *data);
uint8_t TP_StartTest(void);

typedef enum {
    TP_START,  /*开始测试*/
    TP_ECHO,   /*测试开始后的心跳信号*/
    TP_RESULT, /*测试结果*/
    TP_WAKUP,  /*唤醒USART1*/
    TP_SLEEP,  /*关闭USART1*/
} TPPIDTypeDef;

#endif
/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
