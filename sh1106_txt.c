
#include "sh1106.h"
#include <string.h>

static const unsigned char aaFont[][5] = {
0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x5F, 0x00, 0x00,
0x00, 0x07, 0x00, 0x07, 0x00,
0x14, 0x7F, 0x14, 0x7F, 0x14,
0x24, 0x2A, 0x7F, 0x2A, 0x12,
0x23, 0x13, 0x08, 0x64, 0x62,
0x36, 0x49, 0x56, 0x20, 0x50,
0x00, 0x08, 0x07, 0x03, 0x00,
0x00, 0x1C, 0x22, 0x41, 0x00,
0x00, 0x41, 0x22, 0x1C, 0x00,
0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
0x08, 0x08, 0x3E, 0x08, 0x08,
0x00, 0x80, 0x70, 0x30, 0x00,
0x08, 0x08, 0x08, 0x08, 0x08,
0x00, 0x00, 0x60, 0x60, 0x00,
0x20, 0x10, 0x08, 0x04, 0x02,
0x3E, 0x51, 0x49, 0x45, 0x3E,
0x00, 0x42, 0x7F, 0x40, 0x00,
0x72, 0x49, 0x49, 0x49, 0x46,
0x21, 0x41, 0x49, 0x4D, 0x33,
0x18, 0x14, 0x12, 0x7F, 0x10,
0x27, 0x45, 0x45, 0x45, 0x39,
0x3C, 0x4A, 0x49, 0x49, 0x31,
0x41, 0x21, 0x11, 0x09, 0x07,
0x36, 0x49, 0x49, 0x49, 0x36,
0x46, 0x49, 0x49, 0x29, 0x1E,
0x00, 0x00, 0x14, 0x00, 0x00,
0x00, 0x40, 0x34, 0x00, 0x00,
0x00, 0x08, 0x14, 0x22, 0x41,
0x14, 0x14, 0x14, 0x14, 0x14,
0x00, 0x41, 0x22, 0x14, 0x08,
0x02, 0x01, 0x59, 0x09, 0x06,
0x3E, 0x41, 0x5D, 0x59, 0x4E,
0x7C, 0x12, 0x11, 0x12, 0x7C,
0x7F, 0x49, 0x49, 0x49, 0x36,
0x3E, 0x41, 0x41, 0x41, 0x22,
0x7F, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x49, 0x49, 0x49, 0x41,
0x7F, 0x09, 0x09, 0x09, 0x01,
0x3E, 0x41, 0x41, 0x51, 0x73,
0x7F, 0x08, 0x08, 0x08, 0x7F,
0x00, 0x41, 0x7F, 0x41, 0x00,
0x20, 0x40, 0x41, 0x3F, 0x01,
0x7F, 0x08, 0x14, 0x22, 0x41,
0x7F, 0x40, 0x40, 0x40, 0x40,
0x7F, 0x02, 0x1C, 0x02, 0x7F,
0x7F, 0x04, 0x08, 0x10, 0x7F,
0x3E, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x09, 0x09, 0x09, 0x06,
0x3E, 0x41, 0x51, 0x21, 0x5E,
0x7F, 0x09, 0x19, 0x29, 0x46,
0x26, 0x49, 0x49, 0x49, 0x32,
0x03, 0x01, 0x7F, 0x01, 0x03,
0x3F, 0x40, 0x40, 0x40, 0x3F,
0x1F, 0x20, 0x40, 0x20, 0x1F,
0x3F, 0x40, 0x38, 0x40, 0x3F,
0x63, 0x14, 0x08, 0x14, 0x63,
0x03, 0x04, 0x78, 0x04, 0x03,
0x61, 0x59, 0x49, 0x4D, 0x43,
0x00, 0x7F, 0x41, 0x41, 0x41,
0x02, 0x04, 0x08, 0x10, 0x20,
0x00, 0x41, 0x41, 0x41, 0x7F,
0x04, 0x02, 0x01, 0x02, 0x04,
0x40, 0x40, 0x40, 0x40, 0x40,
0x00, 0x03, 0x07, 0x08, 0x00,
0x20, 0x54, 0x54, 0x78, 0x40,
0x7F, 0x28, 0x44, 0x44, 0x38,
0x38, 0x44, 0x44, 0x44, 0x28,
0x38, 0x44, 0x44, 0x28, 0x7F,
0x38, 0x54, 0x54, 0x54, 0x18,
0x00, 0x08, 0x7E, 0x09, 0x02,
0x18, 0xA4, 0xA4, 0x9C, 0x78,
0x7F, 0x08, 0x04, 0x04, 0x78,
0x00, 0x44, 0x7D, 0x40, 0x00,
0x20, 0x40, 0x40, 0x3D, 0x00,
0x7F, 0x10, 0x28, 0x44, 0x00,
0x00, 0x41, 0x7F, 0x40, 0x00,
0x7C, 0x04, 0x78, 0x04, 0x78,
0x7C, 0x08, 0x04, 0x04, 0x78,
0x38, 0x44, 0x44, 0x44, 0x38,
0xFC, 0x18, 0x24, 0x24, 0x18,
0x18, 0x24, 0x24, 0x18, 0xFC,
0x7C, 0x08, 0x04, 0x04, 0x08,
0x48, 0x54, 0x54, 0x54, 0x24,
0x04, 0x04, 0x3F, 0x44, 0x24,
0x3C, 0x40, 0x40, 0x20, 0x7C,
0x1C, 0x20, 0x40, 0x20, 0x1C,
0x3C, 0x40, 0x30, 0x40, 0x3C,
0x44, 0x28, 0x10, 0x28, 0x44,
0x4C, 0x90, 0x90, 0x90, 0x7C,
0x44, 0x64, 0x54, 0x4C, 0x44,
0x00, 0x08, 0x36, 0x41, 0x00,
0x00, 0x00, 0x77, 0x00, 0x00,
0x00, 0x41, 0x36, 0x08, 0x00,
0x02, 0x01, 0x02, 0x04, 0x02
};

extern tLCD LCD_lcd;
int LCD_text(int sx, int sy, const char *pText)
{
	int y = sy / 8;
	LCD_lcd.aaBuffer[y][0] = 1;
	if ((sy & 7) == 0) {
		// single line change
		unsigned char *pDst = LCD_lcd.aaBuffer[y] + 4 + sx;
		while (*pText && sx < LCD_MAX_X) {
			char c = *pText++;
			if (' ' <= c && c <= '~') {
				const unsigned char *pSrc = aaFont[c - 0x20];
				for (int cx = 0; cx < 5 && sx < LCD_MAX_X; cx++, sx++) {
					*pDst++ = *pSrc++;
				}
				if (sx < LCD_MAX_X && *pText) {
					*pDst++ = 0;
					sx++;
				}
			}
		}
	} else {
		// two lines
		unsigned char *pDst0 = LCD_lcd.aaBuffer[y] + 4 + sx;
		unsigned char *pDst1 = LCD_lcd.aaBuffer[y+1] + 4 + sx;
		int shift = sy & 7;
		int mask = ~(0xff << shift);

		LCD_lcd.aaBuffer[y+1][0] = 1;

		while (*pText && sx < LCD_MAX_X) {
			char c = *pText++;
			if (' ' <= c && c <= '~') {
				const unsigned char *pSrc = aaFont[c - 0x20];
				for (int cx = 0; cx < 5 && sx < LCD_MAX_X; cx++, sx++) {
					unsigned int ch = *pSrc++ << shift;
					unsigned int val = (*pDst1 << 8) | *pDst0;

					val = (val & mask) | ch;
					*pDst0++ = val;
					*pDst1++ = val >> 8;
				}
				if (sx < LCD_MAX_X && *pText) {
					*pDst0++ &= mask;
					*pDst1++ &= mask >> 8;
					sx++;
				}
			}
		}
	}

	return sx;
}


int
LCD_number(int sx, int sy, int digits, int base, int number)
{
	static const char aHex[] = "0123456789abcdef";
	char aBuffer[33];
	char *pPtr = aBuffer + sizeof(aBuffer);
	*--pPtr = 0;
	base = 1 < base && base <= 16 ? base : 16;
	int sign = number < 0 ? '-' : 0;
	number = number < 0 ? -number : number;
	do {
		int idx = number % base;
		*--pPtr = aHex[idx];
		number /= base;
	} while (--digits && number);

	if (sign && digits) {
		*--pPtr = sign;
		digits--;
	}
	while (0 < digits--) {
		*--pPtr = ' ';
	}

	return LCD_text(sx, sy, pPtr);
}


int
LCD_number_dot(int sx, int sy, int digits, int base, int number, int dotPos)
{
	static const char aHex[] = "0123456789abcdef";
	char aBuffer[34];
	char *pPtr = aBuffer + sizeof(aBuffer);
	*--pPtr = 0;
	int chrCnt = 0;
	do {
		*--pPtr = aHex[number % base];
		chrCnt++;
		number /= base;
		if (0 == --dotPos) {
			*--pPtr = '.';
			chrCnt++;
		}
	} while (aBuffer < pPtr && number);

	if (digits < chrCnt) {
		pPtr[digits+1] = 0;
	} else {
		while (chrCnt < digits) {
			*--pPtr = ' ';
			chrCnt++;
		}
	}

	return LCD_text(sx, sy, pPtr);
}


