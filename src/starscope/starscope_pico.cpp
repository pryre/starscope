#include "stdio.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "starscope/drivers/utils.hpp"

using namespace Starscope;

#define STARSCOPE_I2C i2c1
#define STARSCOPE_PIN_I2C_SDA 6
#define STARSCOPE_PIN_I2C_SCL 7
#define STARSCOPE_PIN_LED PICO_DEFAULT_LED_PIN

// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void run_scan(i2c_inst_t *i2c) {
    Utils::Timer timer("I2C scan");
    printf("\nI2C Bus Scan\n");

    //Initial padding and set to hex mode
    printf("   ");
    for(int i=0; i<=0xF; i++)
        printf("%01x  ", i);
    printf("\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rx_data;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c, addr, &rx_data, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
    printf(timer.to_string().c_str());
}

int main() {
    // Enable UART so we can print status output
    stdio_init_all();

// #ifndef PICO_DEFAULT_LED_PIN
// #warning blink example requires a board with a regular LED
// #else
    gpio_init(STARSCOPE_PIN_LED);
    gpio_set_dir(STARSCOPE_PIN_LED, GPIO_OUT);
    gpio_put(STARSCOPE_PIN_LED, 0);
// #endif
// #if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
// #warning i2c/bus_scan example requires a board with I2C pins
//     puts("Default I2C pins were not defined");
// #else
    // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
    // i2c_init(i2c_default, 400 * 1000);
    // gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    // gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    // gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    i2c_init(STARSCOPE_I2C, 400 * 1000);
    gpio_set_function(STARSCOPE_PIN_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(STARSCOPE_PIN_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(STARSCOPE_PIN_I2C_SDA);
    gpio_pull_up(STARSCOPE_PIN_I2C_SCL);
    // Make the I2C pins available to picotool (Fixed Pin Information)
    bi_decl(bi_2pins_with_func(STARSCOPE_PIN_I2C_SDA, STARSCOPE_PIN_I2C_SCL, GPIO_FUNC_I2C));

    while (true) {
        gpio_put(STARSCOPE_PIN_LED, 1);
        run_scan(STARSCOPE_I2C);
        sleep_ms(250);
        gpio_put(STARSCOPE_PIN_LED, 0);
        sleep_ms(1000);
    }
// #endif
    return 0;
}
