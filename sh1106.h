

#ifndef SH1106_H
#define SH1106_H

#define LCD_MAX_X (128)
#define LCD_MAX_Y ( 64)

typedef struct sLCD {
	int colorType; // 0: set, 1: clear, 2:inverse
	// aaBuffer[y][0]: flag indicating this line has changed
	// aaBuffer[y][1]: 0x40
	// aaBuffer[y][2]: 0x00
	// aaBuffer[y][3]: 0x00
	// aaBuffer[y][4-132]: 8 pixels
	unsigned char aaBuffer[LCD_MAX_Y/8][4+LCD_MAX_X]; 
} tLCD;

void LCD_update();
void LCD_init();
void LCD_hline(int sx, int sy, int ex);
void LCD_vline(int sx, int sy, int ey);
void LCD_rect_set(int sx, int sy, int ex, int ey);
void LCD_rect_clr(int sx, int sy, int ex, int ey);
int  LCD_text(int sx, int sy, const char *pText);
int  LCD_number(int sx, int sy, int digits, int base, int number);
int  LCD_number_dot(int sx, int sy, int digits, int base, int number, int dotPos);

#endif
