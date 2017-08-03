/**
* @file         ppro.h
* @author       Weyne Chen
* @version      V01
* @date         2017.08.03
* @brief         
* @note          
* @attention    COPYRIGHT WEYNE
**/

#ifndef __PPRO_H
#define __PPRO_H
#include "sysconfig.h"

__packed typedef struct
{
    uint8_t DeviceID;
    uint8_t PackageID;
    uint16_t DataLength;
    uint16_t Data;
    uint8_t Crc8;
} PproTypeDef;

typedef enum {
    TP = 0,
    FLCIKER_SENSOR,
    U_DISK
} DeviceIDTypeDef;

void Ppro_SendData(uint8_t device_id,uint8_t package_id,uint16_t data);
uint8_t Ppro_ParseData(uint8_t *buffer);

#endif
/********************* (C) COPYRIGHT WEYNE CHEN *******END OF FILE ********/
