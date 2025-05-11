#include "oled.h"
#include "main.h"
#include "fonts.h"  // ????????(???)

extern I2C_HandleTypeDef hi2c1;  // ????I2C1
static uint32_t oled_pow(uint8_t m, uint8_t n);
/* ?????OLED */
static void OLED_WriteCmd(uint8_t cmd)
{
    uint8_t buf[2] = {0x00, cmd};  // 0x00????
    HAL_I2C_Master_Transmit(&hi2c1, OLEDAddress, buf, 2, 100);
}

/* ???OLED */
void OLED_Init(void)
{
    HAL_Delay(100);  // ??OLED????
    
    OLED_WriteCmd(0xAE);  // ????
    
    OLED_WriteCmd(0xD5);  // ?????????/?????
    OLED_WriteCmd(0x80);
    
    OLED_WriteCmd(0xA8);  // ???????
    OLED_WriteCmd(0x3F);
    
    OLED_WriteCmd(0xD3);  // ??????
    OLED_WriteCmd(0x00);
    
    OLED_WriteCmd(0x40);  // ???????
    
    OLED_WriteCmd(0xA1);  // ??????
    OLED_WriteCmd(0xC8);  // ??COM????
    
    OLED_WriteCmd(0xDA);  // ??COM??????
    OLED_WriteCmd(0x12);
    
    OLED_WriteCmd(0x81);  // ???????
    OLED_WriteCmd(0xCF);
    
    OLED_WriteCmd(0xD9);  // ???????
    OLED_WriteCmd(0xF1);
    
    OLED_WriteCmd(0xDB);  // ??VCOMH??????
    OLED_WriteCmd(0x30);
    
    OLED_WriteCmd(0xA4);  // ????????
    
    OLED_WriteCmd(0xA6);  // ??????
    
    OLED_WriteCmd(0x20);  // ????????
    OLED_WriteCmd(0x00);  // ??????
    
    OLED_WriteCmd(0x8D);  // ?????
    OLED_WriteCmd(0x14);
    
    OLED_Clear();         // ??
    OLED_WriteCmd(0xAF);  // ????
}

/* ?? */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (i = 0; i < 8; i++) {
        OLED_WriteCmd(0xB0 + i);  // ?????
        OLED_WriteCmd(0x00);      // ??????4?
        OLED_WriteCmd(0x10);      // ??????4?
        
        for (j = 0; j < 128; j++) {
            OLED_WriteData(0x00);  // ??????
        }
    }
}

/* ???? */
void OLED_DisplayOn(void)
{
    OLED_WriteCmd(0x8D);  // ???
    OLED_WriteCmd(0x14);
    OLED_WriteCmd(0xAF);  // ????
}

/* ???? */
void OLED_DisplayOff(void)
{
    OLED_WriteCmd(0x8D);  // ???
    OLED_WriteCmd(0x10);
    OLED_WriteCmd(0xAE);  // ????
}

/* ?????? */
void OLED_SetCursor(uint8_t x, uint8_t y)
{
    OLED_WriteCmd(0xB0 + y);              // ?????
    OLED_WriteCmd(((x & 0xF0) >> 4) | 0x10);  // ??????4?
    OLED_WriteCmd((x & 0x0F) | 0x00);     // ??????4?
}

/* ???? */
void OLED_WriteData(uint8_t data)
{
    uint8_t buf[2] = {0x40, data};  // 0x40????
    HAL_I2C_Master_Transmit(&hi2c1, OLEDAddress, buf, 2, 100);
}

/* ???? */

void OLED_ShowChar6x8(uint8_t x, uint8_t y, char c) {
    uint8_t i;
    if(c < 32 || c > 126) c = ' '; // ·Ç·¨×Ö·ûÏÔÊ¾Îª¿Õ¸ñ
    OLED_SetCursor(x, y);
    for(i=0; i<6; i++) {
        OLED_WriteData(F6x8[c-32][i]);
    }
}

/* ????? */
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t wrap) {
    uint8_t col = x;
    uint8_t row = y;
    
    while(*str) {
        // ±ß½ç¼ì²â£¨SSD1306¿í¶È128ÏñËØ£©
        if(col + 6 > 128) {
            if(wrap && row < 7) { // ×Ô¶¯»»ÐÐ
                col = 0;
                row++;
            } else { // ³¬³öÆÁÄ»·¶Î§
                break;
            }
        }
        
        OLED_ShowChar6x8(col, row, *str++);
        col += 6; // ×Ö·û¿í¶È6ÏñËØ + 1ÏñËØ¼ä¾à
        
        // ÌØÊâ×Ö·û´¦Àí
        if(*(str-1) == '\n') { // »»ÐÐ·û
            col = 0;
            if(++row > 7) break;
        }
    }
}

/* ???? */
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar6x8(x + (8 / 2) * t, y, ' ');
                continue;
            } else {
                enshow = 1;
            }
        }
        OLED_ShowChar6x8(x + (8 / 2) * t, y, temp + '0');
    }
}

/* ???? */
void OLED_Refresh(void)
{
    // ??SSD1306,??????????,?????????
}
static uint32_t oled_pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while(n--) {
        result *= m;
    }
    return result;
}
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *bmp)
{
    uint32_t j = 0;
    uint8_t x, y;
    
    for (y = y0; y < y1; y++) {
        OLED_SetCursor(x0, y);
        for (x = x0; x < x1; x++) {
            OLED_WriteData(bmp[j++]);
        }
    }
}