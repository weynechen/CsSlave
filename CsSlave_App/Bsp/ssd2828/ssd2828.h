#ifndef _SSD2828_H_
#define _SSD2828_H_
#include "stm32f1xx_hal.h"

#define MIPI_READ_FAIL       0X01
#define MIPI_READ_SUCCEED    0X00

void SSD2828_RST_H(void);
void SSD2828_RST_L(void);
void SSD2828_IC1_CS_0(void);
void SSD2828_IC1_CS_1(void);
void SSD2828_IC2_CS_0(void);
void SSD2828_IC2_CS_1(void);
void SSD2828Init(uint8_t lane, uint16_t dataRate);
void SSD2828Video(void);
void SSD2828LP(void);
uint16_t SSD2828ReadReg(uint8_t reg);
void SSD2828GenericLongWrite(uint32_t n);
void SSD2828GenericShortWrite(uint8_t n);
void SSD2828DcsLongWrite(uint32_t n);
void SSD2828DcsShortWrite(uint8_t n);
uint16_t SSD2828DcsReadDT06(uint8_t adr, uint16_t l, uint8_t *p);
uint16_t SSD2828GenericReadDT14(uint8_t adr, uint16_t l, uint8_t *p);
void SSD2828WriteData(uint8_t dat);
void SSD2828WriteCmd(uint8_t cmd);
void SSD2828WriteReg(uint8_t cmd, uint8_t dat1, uint8_t dat2);
void WR_DT29(uint32_t n);
void WR_DT03_13_23(uint8_t n);
void WR_DT39(uint32_t n);
void WR_DT05_15(uint8_t n);
void RD_DT06(uint8_t adr, uint16_t l, uint8_t *p);
void RD_DT14(uint8_t adr, uint16_t l, uint8_t *p);

#endif
