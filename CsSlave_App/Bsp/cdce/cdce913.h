#ifndef _CDCE913_H_
#define _CDCE913_H_
#include "sys.h"


#define CDCE_ADDRESS    0x65

#define TP_ACK 1
#define TP_NO_ACK 0

#define CDCE_ADDRESS 0x65 

#define SLAVE_WRITE CDCE_ADDRESS<<1
#define SLAVE_READ ((CDCE_ADDRESS<<1) | 0x01)

#define I2C_SCL    PBout(8) //SCL
#define I2C_SDA    PBout(9) //SDA	 
#define READ_SDA   PBin(9)


void CDCE_Init(uint16_t f_out);

#endif
