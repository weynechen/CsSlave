#ifndef _IN_IMG_H_
#define _IN_IMG_H_
#include "stdint.h"
void Img_CT(void);
void Img_Full(unsigned char r, unsigned char g, unsigned char b);
void Img_Chcker58(void);
void Img_Box(void);
void Img_Gray256_V(uint16_t level);
void Img_Gray256_H(uint16_t level);
void Img_RED256_H(void);
void Img_GREEN256_H(void);
void Img_BLUE256_H(void);
void Img_RED256_V(void);
void Img_GREEN256_V(void);
void Img_BLUE256_V(void);
void Img_ColorBar(void); //彩条
void Img_ColorBarV(void);
void Flicker(void);
void MAX_Current(void);
void two_windows(void);
void Img_NG(void);
void RGBBar(void);
void RGBLevel(void);
void Flicker2Dot(void);
#endif
