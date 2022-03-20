#include <inttypes.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/usart.h>
// #include <libopencm3/stm32/dma.h>
// #include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/spi.h>

#include <config.h>
#include <board.hpp>

Board::Board() {
}

void Board::sleep_us(uint32_t us) const {
    for (uint32_t j = 0; j < us; j++)
	    for (uint32_t i = 0; i < 2; i++)   //XXX: Board is running at 100MHz, so ~100 instructions per us
			__asm__("nop");
}

void Board::sleep_ms(uint32_t ms) const {
    for (uint32_t j = 0; j < ms; j++)
        sleep_us(1000);
}

void Board::blink_forever(const float frequency) const {
    uint32_t period_ms = 1000.0 / frequency;

	while (1) {
		gpio_toggle(PORT_LED, PIN_LED);
        sleep_ms(period_ms);
	}
}

void Board::gpio_init() {
	/* Enable GPIO clock. */
	/* Using API functions: */
	rcc_periph_clock_enable(DEV_LED);
	/* Set pin to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED);
}

void Board::spi_init() {
	// Enable the clocks for the devices and ports we are using
	rcc_periph_clock_enable(DEV_SPI);
	rcc_periph_clock_enable(PDEV_SPI_MAIN);
	rcc_periph_clock_enable(PDEV_SPI_AUX);

	// Communication line setup
	gpio_mode_setup(PORT_SPI_MAIN, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, PIN_SPI_MAIN_SCK | PIN_SPI_MAIN_MOSI | PIN_SPI_MAIN_NSS);
	gpio_set_af(PORT_SPI_MAIN, GPIO_AF5, PIN_SPI_MAIN_SCK | PIN_SPI_MAIN_MOSI | PIN_SPI_MAIN_NSS);
	gpio_set_output_options(PORT_SPI_MAIN, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_SPI_MAIN_SCK | PIN_SPI_MAIN_MOSI | PIN_SPI_MAIN_NSS);

	// Control line setup
	gpio_mode_setup(PORT_SPI_AUX, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_SPI_AUX_CE | PIN_SPI_AUX_DC | PIN_SPI_AUX_RE);
    gpio_set_output_options(PORT_SPI_AUX, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_SPI_AUX_CE | PIN_SPI_AUX_DC | PIN_SPI_AUX_RE);

    gpio_set(PORT_SPI_AUX, PIN_SPI_AUX_CE | PIN_SPI_AUX_RE);
    gpio_clear(PORT_SPI_AUX, PIN_SPI_AUX_DC);

    // SPI Setup
    spi_reset(PERIPH_SPI);

	spi_init_master(PERIPH_SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_8, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_LSBFIRST);

    /*
    * Set NSS management to software.
    *
    * Note:
    * Setting nss high is very important, even if we are controlling the GPIO
    * ourselves this bit needs to be at least set to 1, otherwise the spi
    * peripheral will not send any data out.
    */
    spi_enable_software_slave_management(PERIPH_SPI);
    spi_set_nss_high(PERIPH_SPI);

	spi_enable(PERIPH_SPI);

    // Wait a moment for the clock to start
    for (int i = 0; i < 200; i++)
        __asm__("nop");
}

void Board::digital_set(uint32_t port, uint32_t pin, bool onoff) const{
    if(onoff) {
	    gpio_set(port, pin);
    } else {
	    gpio_clear(port, pin);
    }
}

void Board::spi_send(uint8_t byte) const{
	spi_write(PERIPH_SPI, byte);
}