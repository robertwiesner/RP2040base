A small example for a RP2040 board with I2C (LCD display), LED blinking and some GPIOs

GPIO 0 - 3: LED

J4: 
GPIO6: GPIO/I2C1_SDA
GPIO7: GPIO/I2C1_SCL
GPIO8: GPIO/UART1_TX/I2C0_SDA/SPI1_RX
GPIO9: GPIO/UART1_RX/I2C0_SCL/SPI1_CSn
GPIO10: GPIO/I2C1_SDA/SPI1_SCK
GPIO11: GPIO/I2C1_SCL/SPI1_TX
GPIO12: GPIO/UART0_TX/I2C0_SDA/SPI1_RX
GPIO13: GPIO/UART0_RX/I2C0_SCL/SPI1_CSn

J3:
I2C0, GND, and 3.3V

| End Symb |  |
| --- | --- |
| PICO_SDK_PATH | point to <path>/pico-sdk |
| PICO_EXTRAS_PATH | point to <path>/pico-extras |

Clone the pico repositories:
* https://github.com/raspberrypi/pico-sdk.git
* https://github.com/raspberrypi/pico-extras.git
* Install the picotool repository

How to build:
* First time:
 * make build directory in the source directory: mkdir build
 * chdir build
** cmake ..
* every time
 * Change into build directory
 * Start build using "make"
 * install to pico: picotool load test.uf2 && picotool reboot
