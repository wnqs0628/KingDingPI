#ifndef __OLED_H
#define __OLED_H

#include "main.h"

#define OLEDAddress 0x78
/* ???? */
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DisplayOn(void);
void OLED_DisplayOff(void);
void OLED_SetCursor(uint8_t x, uint8_t y);
void OLED_WriteData(uint8_t data);
void OLED_ShowChar6x8(uint8_t x, uint8_t y, char c);
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t wrap);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);
void OLED_Refresh(void);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,const uint8_t *bmp);
#endif
