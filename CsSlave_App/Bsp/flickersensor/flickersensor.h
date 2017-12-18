/**
* @file         flickersensor.h
* @author       Weyne Chen
* @version      V01
* @date         2017.06.25
* @brief         
* @note          
* @attention    COPYRIGHT WEYNE
**/

#ifndef __FLICKERSENSOR_H
#define __FLICKERSENSOR_H
#include "ppro.h"

#define FLICKER_TIMEOUT 0
#define FLICKER_ERROR 2
#define FLICKER_OK 1

typedef enum
{
    FLICKER_VALUE,
    VCOM_VALUE,
    ID_VALUE,
	OTP_TIMES,
    IS_FLICKER,
	TASK_NULL = 0xff,
}TaskIDTypeDef;


extern uint8_t FlickerDataReady;

uint8_t GetFlickerValue(float *flicker);
uint8_t WaitFlickerOK(void);
void SendVcomToFlickerSensor(uint16_t vcom);
void SendIdToFlickerSensor(uint16_t id);
void SendOTPTimesToFlickerSensor(uint16_t times);
void FS_Callback(PproTypeDef *data);
#endif
/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
