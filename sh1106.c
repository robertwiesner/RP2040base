
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"


#include "sh1106.h"

#define I2C1 0

#if I2C1
#define I2C i2c1
#define I2C1_SDA_PIN 18
#define I2C1_SCL_PIN 19
#else
#define I2C i2c0
#define I2C0_SDA_PIN 24
#define I2C0_SCL_PIN 25
#endif
#define ADDR 0x3c
#define MAXY LCD_MAX_Y
#define MAXX LCD_MAX_X

#define LOW_COLUMN_ADDRESS  (0x00)
#define HIGH_COLUMN_ADDRESS (0x10)
#define SET_MEMORY_MODE     (0x20)
#define SET_START_LINE      (0x40)
#define SET_CONTRAST        (0x81)
#define SET_CHARGE_PUMP     (0x8D)
#define SET_SEG_REMAP       (0xA0)
#define SET_DISPLAY_RESUME  (0xA4)
#define SET_NORM_INV        (0xA6)
#define SET_MULTIPLEX       (0xA8)
#define SET_DISP            (0xAE)
#define SET_SCAN_DIR        (0xC0)
#define SET_PAGE_ADDRESS    (0xB0)
#define SET_COM_PINS        (0xDA)
#define SET_DISP_OFFSET     (0xD3)
#define SET_CLOCK_DIV       (0xD5)
#define SET_PRECHARGE       (0xD9)
#define SET_VCOM_DETECT     (0xDB)

typedef struct sCoord {
	int x, y;
} tCoord;

tLCD LCD_lcd;

static void
writeToLCD(int len, char *pBuffer)
{
	i2c_write_blocking(I2C, ADDR, pBuffer, len, false);
}

static void
writeCommand(int cmd)
{
    char aBuffer[2] = {0x00, cmd};

    writeToLCD(2, aBuffer);
}

void
LCD_update()
{
	for (int y = 0; y < MAXY/8; y++) {
		if (LCD_lcd.aaBuffer[y][0]) {
			LCD_lcd.aaBuffer[y][0] = 0;
			writeCommand(LOW_COLUMN_ADDRESS  | 0);
			writeCommand(HIGH_COLUMN_ADDRESS | 0);
			writeCommand(SET_PAGE_ADDRESS + y);
    			writeToLCD(MAXX+3, LCD_lcd.aaBuffer[y]+1);
		}
	}
}

void
my_memset(int *pPtr, int size)
{
	while(0 < size) {
		*pPtr++ = 0;
		size -= 4;
	}
}

void
LCD_init()
{

	i2c_init(I2C, 400 * 1000);
#if I2C1
	gpio_set_function(I2C1_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(I2C1_SCL_PIN, GPIO_FUNC_I2C);
    	bi_decl(bi_2pins_with_func(I2C1_SDA_PIN, I2C1_SCL_PIN, GPIO_FUNC_I2C));
#else
	gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
    	bi_decl(bi_2pins_with_func(I2C0_SDA_PIN, I2C0_SCL_PIN, GPIO_FUNC_I2C));
#endif
	for (int y = 0; y < MAXY/8; y++) {
		LCD_lcd.aaBuffer[y][0] = 1;
		LCD_lcd.aaBuffer[y][1] = 0x40;
		LCD_lcd.aaBuffer[y][2] = 0;
		LCD_lcd.aaBuffer[y][3] = 0;
		for (int x = 0; x < MAXX; x++) {
			LCD_lcd.aaBuffer[y][4+x] = 0;
		}
	}
	
	writeCommand(SET_DISP | 0);

	writeCommand(SET_CLOCK_DIV);
	writeCommand(0x80);

	writeCommand(SET_MULTIPLEX);
	writeCommand(64-1);

	writeCommand(SET_DISP_OFFSET);
	writeCommand(0);

	writeCommand(SET_START_LINE);
	writeCommand(SET_CHARGE_PUMP);

	writeCommand(0x14); // or 0x10?

	writeCommand(SET_MEMORY_MODE);
	writeCommand(0x00);

	writeCommand(SET_SEG_REMAP | 1);

	writeCommand(SET_SCAN_DIR | 8);

	writeCommand(SET_COM_PINS);
	writeCommand(0x12);

	writeCommand(SET_CONTRAST);
	writeCommand(0x8f);

	writeCommand(SET_PRECHARGE);

	writeCommand(0xF1); // or 0x22?

	writeCommand(SET_VCOM_DETECT);
	writeCommand(0x40);

	writeCommand(SET_DISPLAY_RESUME);
	writeCommand(SET_NORM_INV);
	writeCommand(SET_DISP | 1);

	LCD_update();
}

void
LCD_hline(int sx, int sy, int ex)
{
	int y = sy / 8;
	int bit = 0x1 << (sy & 7);
	LCD_lcd.aaBuffer[y][0] = 1;

	unsigned char *pDst = LCD_lcd.aaBuffer[y] + 4;	
	while (sx <= ex) {
		pDst[sx++] |= bit;
	}
}

void
LCD_vline(int sx, int sy, int ey)
{
	int y = sy / 8;
	int ye = ey / 8;
	int bits = 0xff << (sy & 7);

	unsigned char *pDst = LCD_lcd.aaBuffer[y] + 4;	

	for (y = sy / 8; y < ye; y++) {
		pDst[-4] = 1;
		pDst[sx] |= bits;
		bits = 0xff;
		pDst += sizeof(LCD_lcd.aaBuffer[0]); 
	}

	pDst[-4] = 1;
	// deal with the last line
	if (ey & 7) {
		bits &= 0xff >> (7 - (ey & 7));
	}
	pDst[sx] |= bits;
}

void
LCD_rect_set(int sx, int sy, int ex, int ey)
{
	int y;
	int ye = ey / 8;
	int bits = 0xff << (sy & 7);

	unsigned char *pDst = LCD_lcd.aaBuffer[y] + 4;

	for (y = sy / 8; y < ye; y++) {
		pDst[-4] = 1;
		for (int x = sx; x <= ex; x++) {
			pDst[x] |= bits;
		}
		bits = 0xff;
		pDst += sizeof(LCD_lcd.aaBuffer[0]); 
	}

	pDst[-4] = 1;
	// deal with the last line
	if (ey & 7) {
		bits &= 0xff >> (7 - (ey & 7));
	}

	for (int x = sx; x <= ex; x++) {
		pDst[x] |= bits;
	}
}

void
LCD_rect_inv(int sx, int sy, int ex, int ey)
{
	int y;
	int ye = ey / 8;
	int bits = 0xff << (sy & 7);

	unsigned char *pDst = LCD_lcd.aaBuffer[y] + 4;

	for (y = sy / 8; y < ye; y++) {
		pDst[-4] = 1;
		for (int x = sx; x <= ex; x++) {
			pDst[x] ^= bits;
		}
		bits = 0xff;
		pDst += sizeof(LCD_lcd.aaBuffer[0]); 
	}

	pDst[-4] = 1;
	// deal with the last line
	if (ey & 7) {
		bits &= 0xff >> (7 - (ey & 7));
	}

	for (int x = sx; x <= ex; x++) {
		pDst[x] ^= bits;
	}
}

void
LCD_rect_clr(int sx, int sy, int ex, int ey)
{
	int y;
	int ye = ey / 8;
	int bits = ~(0xff << (sy & 7));

	unsigned char *pDst = LCD_lcd.aaBuffer[y] + 4;

	for (y = sy / 8; y < ye; y++) {
		pDst[-4] = 1;
		for (int x = sx; x <= ex; x++) {
			pDst[x] &= bits;
		}
		bits = 0x00;
		pDst += sizeof(LCD_lcd.aaBuffer[0]); 
	}

	pDst[-4] = 1;
	// deal with the last line
	if (ey & 7) {
		bits &= ~(0xff >> (7 - (ey & 7)));
	}

	for (int x = sx; x <= ex; x++) {
		pDst[x] &= bits;
	}
}

