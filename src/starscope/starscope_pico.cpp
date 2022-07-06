#include "stdio.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "starscope/hardware_abstraction.hpp"
#include "starscope/drivers/utils.hpp"
#include "starscope/drivers/mpu6050.hpp"
#include "starscope/user_interface/display.hpp"

using namespace Starscope;

#define STARSCOPE_SPI_HARDWARE spi0
#define STARSCOPE_SPI_SPEED 2'000'000
#define STARSCOPE_PIN_SPI_SCK 2
#define STARSCOPE_PIN_SPI_MOSI 3
// #define STARSCOPE_PIN_SPI_MISO 4
#define STARSCOPE_PIN_SPI_CS 5

#define STARSCOPE_I2C_HARDWARE i2c1
#define STARSCOPE_I2C_SPEED 400'000
#define STARSCOPE_PIN_I2C_SDA 6
#define STARSCOPE_PIN_I2C_SCL 7

#define STARSCOPE_PIN_LED PICO_DEFAULT_LED_PIN

//=====================
// Hardware Abstraction
//=====================
uint64_t time_monotonic_us() {
    return time_us_64();
}

size_t spi_write(const std::span<const std::byte>src) {
    return spi_write_blocking(STARSCOPE_SPI_HARDWARE, (uint8_t*)(src.data()), src.size());
}

// size_t spi_write_read(const std::span<const std::byte>src, const std::span<const std::byte>dst) {
//     static_assert(src.size() == dst.size())
//     return spi_write_read_blocking (spi_inst_t *spi, const uint8_t *src, uint8_t *dst, size_t len);
// }

size_t i2c_write(const std::byte addr, const std::span<const std::byte>src, const bool nostop) {
    return i2c_write_blocking(STARSCOPE_I2C_HARDWARE, (uint8_t)addr, (uint8_t*)src.data(), src.size(), nostop);
}

size_t i2c_read(const std::byte addr, const std::span<std::byte>dst, const bool nostop) {
    return i2c_read_blocking(STARSCOPE_I2C_HARDWARE, (uint8_t)addr, (uint8_t*)dst.data(), dst.size(), nostop);
}

//=====================
// Hardware Setup
//=====================

void init_hardware_i2c() {
    i2c_init(STARSCOPE_I2C_HARDWARE, STARSCOPE_I2C_SPEED);
    gpio_set_function(STARSCOPE_PIN_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(STARSCOPE_PIN_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(STARSCOPE_PIN_I2C_SDA);
    gpio_pull_up(STARSCOPE_PIN_I2C_SCL);
    // Make the I2C pins available to picotool (Fixed Pin Information)
    bi_decl(bi_2pins_with_func(STARSCOPE_PIN_I2C_SDA, STARSCOPE_PIN_I2C_SCL, GPIO_FUNC_I2C));
}

void init_hardware_heartbeat() {
    gpio_init(STARSCOPE_PIN_LED);
    gpio_set_dir(STARSCOPE_PIN_LED, GPIO_OUT);
    gpio_put(STARSCOPE_PIN_LED, 0);
}

//=====================
// Main
//=====================
int main() {
    stdio_init_all();

    Utils::Timer timer_init("Initialization");
    init_hardware_heartbeat();
    init_hardware_i2c();
    timer_init.end();

    Drivers::MPU6050::Driver mpu6050;
    if(!mpu6050.init())
        printf("Failed to initialize MPU6050\n");

    UserInterfaceDisplay::Display display;
    if(!display.init())
        printf("Failed to initialize display\n");


    bool stdio_sent_header = false;

    while (true) {
        if(stdio_usb_connected() && !stdio_sent_header) {
            printf("\n--== STARSCOPE ==--\n");
            printf(timer_init.to_string().c_str());
            stdio_sent_header = true;
        }

        gpio_put(STARSCOPE_PIN_LED, 1);
        // run_scan(STARSCOPE_I2C);
        if(mpu6050.ready()) {
            auto data = mpu6050.get_values();
            printf("\nAccel: ");
            printf(data.acceleration.to_string().c_str());
            printf("\nGyro: ");
            printf(data.rates.to_string().c_str());
            printf("\nTemp: %0.2f\n", data.temperature);
        } else {
            printf("MPU6050 not initialized\n");
        }
        if(display.ready()) {
            display.run_demo();
        } else {
            printf("display not initialized\n");
        }
        sleep_ms(250);
        gpio_put(STARSCOPE_PIN_LED, 0);
        sleep_ms(250);
    }
// #endif
    return 0;
}
