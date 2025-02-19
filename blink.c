#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/binary_info.h"

#include "sh1106.h"
const uint LED_PIN = 0;

int main()
{

    int cnt = 0;

    adc_init();
    adc_gpio_init(38);
    adc_gpio_init(39);
    adc_gpio_init(40);
    adc_gpio_init(41);

    LCD_init();

    LCD_text(0, 0, "Hello World!!!");
    LCD_update();

    for (int i = 0; i < 4; i++) {
    	gpio_init(i);
    	gpio_set_dir(i, GPIO_OUT);
    }

    while (1) {
	cnt++;
    	for (int i = 0; i < 4; i++) {
            gpio_put(i, (cnt & (1 << i)) ? 1 : 0 );
	}
	
    	for (int i = 0; i < 4; i++) {
	    int val;
	    adc_select_input(0);
	    val = adc_read();

	    int x = LCD_number(0, 8+8*i, 1, 10, i);
    	    x = LCD_text(x, 8+8*i, ". ADC: ");
	    x = LCD_number(0, 8+8*i, 4, 10, val);
    	    x = LCD_text(x, 8+8*i, " mV");
	}
	LCD_number(0, 8*6, 8, 10, cnt);
        LCD_update();

        sleep_ms(250);
    }
}
