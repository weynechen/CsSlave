/**
* @file         tp.c
* @author       Weyne Chen
* @version      V01
* @date         2017.08.03
* @brief         
* @note          
* @attention    COPYRIGHT WEYNE
**/

#include "tp.h"
#include "lcd.h"

static uint16_t HeartBeats = 0;
static uint8_t WaitTest = 0;
static uint8_t TestResult = 0;

void TP_Callback(PproTypeDef *data)
{
		char s[2];
		uint8_t echo;
    switch (data->PackageID)
    {
    case TP_ECHO:
				echo = *data->Data;
				s[1] = echo%10+'0';
				s[0] = echo/10+'0';
				//LCD_ShowString(0, 32, s);
				LCD_ShowChar(0,32,s[0]);
				LCD_ShowChar(16,32,s[1]);
        HeartBeats = 1;
        break;

    case TP_RESULT:
        WaitTest = 1;
        TestResult = 0;
        if (*data->Data == 'O')
            if (*(data->Data + 1) == 'K')
                TestResult = 1;
        break;

    case TP_NO_FILE:
        WaitTest = 1;
        TestResult = 0;
        break;

    default:
        break;
    }
}

static void TP_SendData(uint8_t pid, uint16_t data)
{
    Ppro_SendData(TP, pid, data);
}

uint8_t TP_StartTest(void)
{
    /*定义总的测试时间，不能测试太久(ms)*/
    uint16_t timeout = 60000;

    /*心跳超时时间(ms)*/
    HeartBeats = 1500;
    WaitTest = 0;
    TP_SendData(TP_START, 0x5453);

    /*等待测试结果*/
    while (WaitTest == 0)
    {
        /*若收到echo，HeartBeats会刷新*/
        HeartBeats--;
        /*超时未收到echo*/
        if (HeartBeats == 0)
        {
            //return 0;
        }
        HAL_Delay(1);

        timeout--;
        if (timeout == 0)
        {
            return 0;
        }
    }

    if (TestResult == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
