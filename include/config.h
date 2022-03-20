#ifndef _STARSCOPE_CONFIG_H
#define _STARSCOPE_CONFIG_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#define DEV_LED (RCC_GPIOC)
#define PORT_LED (GPIOC)
#define PIN_LED (GPIO13)

#define PERIPH_SPI (SPI2)
#define DEV_SPI (RCC_SPI2)
#define PDEV_SPI_MAIN (RCC_GPIOB)
#define PDEV_SPI_AUX (RCC_GPIOB)
#define PORT_SPI_MAIN (GPIOB)
#define PORT_SPI_AUX (GPIOB)
#define PIN_SPI_MAIN_NSS (GPIO12)
#define PIN_SPI_MAIN_SCK (GPIO13)
#define PIN_SPI_MAIN_MISO (GPIO14)
#define PIN_SPI_MAIN_MOSI (GPIO15)
#define PIN_SPI_AUX_CE (GPIO10)
#define PIN_SPI_AUX_RE (GPIO2)
#define PIN_SPI_AUX_DC (GPIO1)

#ifdef __cplusplus
}
#endif

#endif