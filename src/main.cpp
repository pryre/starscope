/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (C) 2012 Karl Palsson <karlp@tweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <setup.h>
#include <pcd8544.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/usart.h>
// #include <libopencm3/stm32/dma.h>
// #include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/spi.h>



#include <pcd8544_conf.h>
#include <pcd8544_port.h>
#include <pcd8544.h>
#include <inttypes.h>
#define FALSE 0
#define TRUE 1

static const uint8_t simple_logo_bitmap[14] = {
     0xFF, 0x81, 0xC3, 0xA5, 0x99, 0xA5, 0xDB,
     0xDB, 0xA5, 0x99, 0xA5, 0xC3, 0x81, 0xFF
};

static const pcd8544_image simple_logo_image = {
    .bitmap = (const uint8_t *)simple_logo_bitmap,
    .lookup = FALSE,
    .width_px = 14,
    .height_pg = 1
};

void test_txt_size_and_color(void)
{
    // font size 1, normal mode
    pcd8544_setup_brush(FALSE, 1, 1);
    pcd8544_clear();
    pcd8544_set_cursor(0, 0);  // 0, 0 for real coords
    pcd8544_print_s("txt 1");

    // font size 2, inverse mode
    pcd8544_setup_brush(TRUE, 2, 1);
    pcd8544_set_cursor(1, 1);  // 0, 16 for real coords
    pcd8544_print_s("txt 2");
    _delay_ms(2000);

    // font size 3, normal mode
    pcd8544_setup_brush(FALSE, 3, 1);
    /*
     * if called befor pcd8544_setup_brush() the display will be cleared with
     * black color
     */
    pcd8544_clear();
    pcd8544_set_cursor(1, 0);  // 18, 0 for real coords
    pcd8544_print_s("txt 3");
}

void test_txt_lang_and_frame(void)
{
    pcd8544_setup_brush(FALSE, 1, 1);
    pcd8544_clear();

    // text with english and russian words printed in a frame 72x36 px
    pcd8544_set_cursor(1, 1);
    pcd8544_print_s_f(6, 78, 4, "In Russian, word bear means word ");
    pcd8544_set_txt_language(PCD8544_LANG_RU);
    pcd8544_print_s_f(6, 78, 4, "\"медведь\".");
    pcd8544_set_txt_language(PCD8544_LANG_EN);
}

void test_img_scale(void)
{
    /*
     * Here we are going to print a simple logo on the whole display, but the
     * logo is only 14 (instead of 504) bytes size.
     */
    pcd8544_setup_brush(FALSE, 1, 6);
    pcd8544_clear();
    pcd8544_draw_img(0, 0, &simple_logo_image);
}

static void gpio_init(void)
{
	/* Enable GPIO clock. */
	/* Using API functions: */
	rcc_periph_clock_enable(DEV_LED);
	/* Set pin to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED);
}

void spi_init(void) {
	/* Enable the GPIO ports whose pins we are using */
	rcc_periph_clock_enable(PDEV_SPI_MAIN);
	rcc_periph_clock_enable(PDEV_SPI_AUX);

	gpio_mode_setup(PORT_SPI_MAIN, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, PIN_SPI_MAIN_SCK | PIN_SPI_MAIN_MOSI | PIN_SPI_MAIN_MISO);
	gpio_set_af(PORT_SPI_MAIN, GPIO_AF5, GPIO3 | GPIO4 | GPIO5);
	gpio_set_output_options(PORT_SPI_MAIN, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, PIN_SPI_MAIN_SCK | PIN_SPI_MAIN_MOSI);

	/* Chip select line */
	// gpio_set(GPIOC, GPIO1);
	// gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);

	gpio_mode_setup(PORT_SPI_AUX, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_SPI_AUX_CE | PIN_SPI_AUX_DC | PIN_SPI_AUX_RE);
	gpio_set(PORT_SPI_AUX, PIN_SPI_AUX_CE | PIN_SPI_AUX_DC | PIN_SPI_AUX_RE);

	rcc_periph_clock_enable(DEV_SPI);

	//Configure SPI
	// uint32_t cr_tmp = SPI_CR1_BAUDRATE_FPCLK_DIV_8 |
	// 	 SPI_CR1_MSTR |
	// 	 SPI_CR1_SPE |
	// 	 SPI_CR1_CPHA |
	// 	 SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE;

	// //Initialise SPI
	// SPI_CR2(PERIPH_SPI) |= SPI_CR2_SSOE;
	// SPI_CR1(PERIPH_SPI) = cr_tmp;

	spi_init_master(PERIPH_SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_8, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_LSBFIRST);
	spi_enable(PERIPH_SPI);
}



int main(void)
{
	gpio_init();
	spi_init();

	test_img_scale();

	/* Blink the LED on the board. */
	while (1) {
		/* Using API function gpio_toggle(): */
		gpio_toggle(PORT_LED, PIN_LED);	/* LED on/off */
		for (int i = 0; i < 1000000; i++) {	/* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;
}
