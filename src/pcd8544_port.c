//XXX: handlers for pcd8544_port.h
#ifdef __cplusplus
extern "C" {
#endif

#include <pcd8544_port.h>
#include <config.h>

void _ce_high(void) {
	gpio_set(PORT_SPI_AUX, PIN_SPI_AUX_CE);
}

void _ce_low(void) {
	gpio_clear(PORT_SPI_AUX, PIN_SPI_AUX_CE);
}

void _dc_high(void) {
	gpio_set(PORT_SPI_AUX, PIN_SPI_AUX_DC);
}

void _dc_low(void) {
	gpio_clear(PORT_SPI_AUX, PIN_SPI_AUX_DC);
}

void _rst_high(void) {
	gpio_set(PORT_SPI_AUX, PIN_SPI_AUX_RE);
}

void _rst_low(void) {
	gpio_clear(PORT_SPI_AUX, PIN_SPI_AUX_RE);
}

void _spi_send_byte(uint8_t byte) {
	spi_write(PERIPH_SPI, byte);
}

void _delay_ms(uint32_t ms) {
    for (uint32_t j = 0; j < ms; j++)
		for (uint32_t i = 0; i < 16*1000; i++) //XXX: Board is running at 100MHz, so a few instructions per us?
			__asm__("nop");
}

#ifdef __cplusplus
}
#endif