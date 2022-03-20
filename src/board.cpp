#include <inttypes.h>
#include <stdbool.h>
#include <mutex>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/usart.h>
// #include <libopencm3/stm32/dma.h>
// #include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/spi.h>

#include <config.h>
#include <board.hpp>

Board::Board() {
    static bool _first_init_flag = Board::_first_init_setup();
    if(_first_init_flag) {
        //XXX: Perform any second-time setup?
        //XXX: Also hides unused var warning
    }
}

bool Board::_first_init_setup() {
    rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_84MHZ]);

    return true;
}

void Board::sleep_us(uint32_t us) const {
    for (uint32_t j = 0; j < 16*us; j++) //XXX: Board is running at 100MHz, so a few instructions per us?
        __asm__("nop");
}

void Board::sleep_ms(uint32_t ms) const {
    for (uint32_t j = 0; j < ms; j++)
        sleep_us(1000);
}

void Board::blink_forever(const float frequency) const {
    uint32_t period_ms = 1000.0 / frequency;

	while (1) {
		gpio_toggle(PORT_IOC, PIN_LED);
        sleep_ms(period_ms);
	}
}

void Board::gpio_init() {
	/* Enable GPIO clock. */
	/* Using API functions: */
	rcc_periph_clock_enable(DEV_IOC);
	/* Set pin to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(PORT_IOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED | PIN_POWER);
    gpio_set(PORT_SPI_AUX, PIN_LED);
    gpio_clear(PORT_SPI_AUX, PIN_POWER);

}

void Board::spi_init() {
	// Enable the clocks for the devices and ports we are using
	rcc_periph_clock_enable(DEV_SPI);
	rcc_periph_clock_enable(PDEV_SPI_MAIN);
	rcc_periph_clock_enable(PDEV_SPI_AUX);

	// Communication line setup
	gpio_mode_setup(PORT_SPI_MAIN, GPIO_MODE_AF, GPIO_PUPD_NONE, PIN_SPI_MAIN_SCK | PIN_SPI_MAIN_MOSI);
	gpio_set_af(PORT_SPI_MAIN, GPIO_AF5, PIN_SPI_MAIN_SCK | PIN_SPI_MAIN_MOSI);
	// gpio_set_output_options(PORT_SPI_MAIN, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_SPI_MAIN_SCK | PIN_SPI_MAIN_MOSI | PIN_SPI_MAIN_NSS);

    gpio_mode_setup(PORT_SPI_MAIN, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_SPI_MAIN_NSS);
    gpio_set_output_options(PORT_SPI_MAIN, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_SPI_MAIN_NSS);
    gpio_set(PORT_SPI_MAIN, PIN_SPI_MAIN_NSS);

	// Control line setup
	gpio_mode_setup(PORT_SPI_AUX, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_SPI_AUX_CE | PIN_SPI_AUX_DC | PIN_SPI_AUX_RE);
    gpio_set_output_options(PORT_SPI_AUX, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, PIN_SPI_AUX_CE | PIN_SPI_AUX_DC | PIN_SPI_AUX_RE);

    gpio_set(PORT_SPI_AUX, PIN_SPI_AUX_CE | PIN_SPI_AUX_RE);
    gpio_clear(PORT_SPI_AUX, PIN_SPI_AUX_DC);

    // SPI Setup
    /*
    spi_reset(PERIPH_SPI);
	spi_init_master(PERIPH_SPI,
                    SPI_CR1_BAUDRATE_FPCLK_DIV_8,
                    SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_1,
                    SPI_CR1_DFF_8BIT,
                    SPI_CR1_MSBFIRST);

    spi_set_bidirectional_transmit_only_mode(PERIPH_SPI);

    // spi_enable_software_slave_management(PERIPH_SPI);
    // spi_set_nss_high(PERIPH_SPI);
    spi_enable_ss_output(PERIPH_SPI);

	spi_enable(PERIPH_SPI);
    */


    /* Reset and disable SPI */
    spi_reset(PERIPH_SPI);

    /* Disable I2S */
    SPI2_I2SCFGR = 0;

    /* CR1 */
    spi_set_clock_phase_0(PERIPH_SPI);                /* CPHA = 0    */
    spi_set_clock_polarity_0(PERIPH_SPI);             /* CPOL = 0    */
    spi_send_msb_first(PERIPH_SPI);                   /* LSB = 0     */
    spi_set_full_duplex_mode(PERIPH_SPI);             /* RXONLY = 0  */
    spi_set_unidirectional_mode(PERIPH_SPI);          /* BIDI = 0    */
    spi_enable_software_slave_management(PERIPH_SPI); /* SSM = 1     */
    spi_set_nss_high(PERIPH_SPI);                     /* SSI = 1     */
    spi_set_master_mode(PERIPH_SPI);                  /* MSTR = 1    */
    spi_set_dff_8bit(PERIPH_SPI);                     /* DFf = 8 bit */
//    spi_enable_crc(SPI2);
    spi_set_baudrate_prescaler(SPI2, SPI_CR1_BR_FPCLK_DIV_64);

    /* CR2 */
    spi_enable_ss_output(PERIPH_SPI); /* SSOE = 1 */
    /* Disable regular interrupt flags */
    spi_disable_tx_buffer_empty_interrupt(PERIPH_SPI);
    spi_disable_rx_buffer_not_empty_interrupt(PERIPH_SPI);

    spi_disable_error_interrupt(PERIPH_SPI);

    /* Enabling RX/TX DMA flags */
    // spi_enable_tx_dma(SPI2);
    // spi_enable_rx_dma(SPI2);

    // d_print("REG: %lu:%lu\r\n", SPI_CR1(SPI2), SPI_CR2(SPI2));

    spi_enable(PERIPH_SPI);

    // Wait a moment for the clock to start
    sleep_ms(1);
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

void Board::power_extdev_display(bool onoff) const {
    digital_set(PORT_IOC, PIN_POWER, onoff);
}