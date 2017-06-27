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
#include "sysconfig.h"

#define FLICKER_TIMEOUT 0

__packed typedef struct
{
    uint8_t DeviceID;
    uint8_t PackageID;
    uint16_t DataLength;
    uint16_t Data;
    uint8_t Crc8;
} FlickerPackageTypeDef;

typedef enum {
    TP = 0,
    FLCIKER_SENSOR,
    U_DISK
} DeviceIDTypeDef;

typedef enum {
    FLICKER_VALUE,
    VCOM_VALUE,
    ID,

} FlickerSensorTypeDef;

extern uint8_t FlickerDataReady;

uint8_t GetFlickerValue(float *flicker);
void SendVcomToFlickerSensor(uint16_t vcom);
void SendIdToFlickerSensor(uint16_t id);
uint8_t ParseFlickerData(void);
#endif
/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
