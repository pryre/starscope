#ifndef _STARSCOPE_BOARD_H
#define _STARSCOPE_BOARD_H

#include <inttypes.h>
#include <stdbool.h>

class Board {
    public:
        Board();

        // Sleep for a set number of microseconds
        void sleep_us(uint32_t us) const;

        // Sleep for a set number of milliseconds
        void sleep_ms(uint32_t ms) const;

        // Initialize the GPIO pins
        void gpio_init();

        // Sets the output of a digital pin
        void digital_set(uint32_t port, uint32_t pin, bool onoff) const;

        // Initialize the SPI port
        void spi_init();

        //Writes a byte to the SPI port
    	void spi_send(uint8_t byte) const;

        // Blink the LED on the board
        void blink_forever(float frequency) const;



};

#endif