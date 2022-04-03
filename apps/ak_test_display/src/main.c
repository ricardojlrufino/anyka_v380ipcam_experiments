#include "ssd1306.h"

int main(int argc, char *argv[]) {

 /* Replace the line below with ssd1306_128x32_i2c_init() if you need to use 128x32 display */
	ssd1306_128x64_i2c_init();
	ssd1306_fillScreen(0x00);
	ssd1306_setFixedFont(ssd1306xled_font6x8);
	ssd1306_printFixed (0,  8, "Line 1. Normal text", STYLE_NORMAL);
	ssd1306_printFixed (0, 16, "Line 2. Bold text", STYLE_BOLD);
	ssd1306_printFixed (0, 24, "Line 3. Italic text", STYLE_ITALIC);
	ssd1306_printFixedN (0, 32, "Line 4. Double size", STYLE_BOLD, FONT_SIZE_2X);
	return 0;
}
