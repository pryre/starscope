#include "stdio.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "starscope/hardware_abstraction.hpp"
#include "starscope/system/state.hpp"
#include "starscope/drivers/utils.hpp"
#include "starscope/drivers/mpu6050.hpp"
#include "starscope/user_interface/display.hpp"
#include "starscope/user_interface/heartbeat.hpp"
#include <chrono>

using namespace std::chrono_literals;
using namespace Starscope;

#define STARSCOPE_SPI_HARDWARE spi0
#define STARSCOPE_SPI_SPEED 2'000'000
#define STARSCOPE_PIN_SPI_SCK 2
#define STARSCOPE_PIN_SPI_MOSI 3
// #define STARSCOPE_PIN_SPI_MISO 4
#define STARSCOPE_PIN_SPI_CS_ACTIVE true
#define STARSCOPE_PIN_SPI_CS_STOP false
#define STARSCOPE_PIN_SPI_CS_SHARPMEMDISPLAY 5

#define STARSCOPE_I2C_HARDWARE i2c1
#define STARSCOPE_I2C_SPEED 400'000
#define STARSCOPE_PIN_I2C_SDA 6
#define STARSCOPE_PIN_I2C_SCL 7

#define STARSCOPE_PIN_LED PICO_DEFAULT_LED_PIN
#define STARSCOPE_LED_ON true
#define STARSCOPE_LED_OFF false

//=====================
// Hardware Abstraction
//=====================
uint64_t time_monotonic_us() {
    return time_us_64();
}

void heartbeat_set(const bool onoff) {
    if(onoff) {
        gpio_put(STARSCOPE_PIN_LED, STARSCOPE_LED_ON);
    } else {
        gpio_put(STARSCOPE_PIN_LED, STARSCOPE_LED_OFF);
    }
}


size_t spi_write(const std::byte src) {
    return spi_write_blocking(STARSCOPE_SPI_HARDWARE, (uint8_t*)&src, 1);
}

size_t spi_write(const std::span<const std::byte>src) {
    return spi_write_blocking(STARSCOPE_SPI_HARDWARE, (uint8_t*)(src.data()), src.size());
}

void spi_set_cs(STARSCOPE_SPI_SELECT chip) {
    switch(chip) {
    case STARSCOPE_SPI_SELECT::SharpMemDisplay: {
        gpio_put(STARSCOPE_PIN_SPI_CS_SHARPMEMDISPLAY, STARSCOPE_PIN_SPI_CS_ACTIVE);
        break;
    }
    default: { //None
        //XXX: Set all CS pins high here
        gpio_put(STARSCOPE_PIN_SPI_CS_SHARPMEMDISPLAY, STARSCOPE_PIN_SPI_CS_STOP);
        break;
    }
    }
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

void init_hardware_heartbeat() {
    gpio_init(STARSCOPE_PIN_LED);
    gpio_set_dir(STARSCOPE_PIN_LED, GPIO_OUT);
    gpio_put(STARSCOPE_PIN_LED, 0);
}

void init_hardware_i2c() {
    i2c_init(STARSCOPE_I2C_HARDWARE, STARSCOPE_I2C_SPEED);
    gpio_set_function(STARSCOPE_PIN_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(STARSCOPE_PIN_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(STARSCOPE_PIN_I2C_SDA);
    gpio_pull_up(STARSCOPE_PIN_I2C_SCL);
    // Make the I2C pins available to picotool (Fixed Pin Information)
    bi_decl(bi_2pins_with_func(STARSCOPE_PIN_I2C_SDA, STARSCOPE_PIN_I2C_SCL, GPIO_FUNC_I2C));
}

void init_hardware_spi() {
    // This example will use SPI0 at 0.5MHz.
    spi_init(STARSCOPE_SPI_HARDWARE, STARSCOPE_SPI_SPEED);
    gpio_set_function(STARSCOPE_PIN_SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(STARSCOPE_PIN_SPI_MOSI, GPIO_FUNC_SPI);
    // XXX: Unused: gpio_set_function(STARSCOPE_PIN_SPI_MISO, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    // bi_decl(bi_3pins_with_func(PIN_MISO, PIN_MOSI, PIN_SCK, GPIO_FUNC_SPI));
    bi_decl(bi_2pins_with_func(STARSCOPE_PIN_SPI_SCK, STARSCOPE_PIN_SPI_MOSI, GPIO_FUNC_SPI));
    // XXX: Chip select for each attached device
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(STARSCOPE_PIN_SPI_CS_SHARPMEMDISPLAY);
    gpio_set_dir(STARSCOPE_PIN_SPI_CS_SHARPMEMDISPLAY, GPIO_OUT);
    spi_set_cs(STARSCOPE_SPI_SELECT::None);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(STARSCOPE_PIN_SPI_CS_SHARPMEMDISPLAY, "SPI CS"));
}

//=====================
// Main
//=====================
int main() {
    stdio_init_all();
    System::set_mode(System::Mode::INITIALIZE);
    Utils::Timer timer_init("Initialization");
    init_hardware_heartbeat();
    init_hardware_i2c();
    init_hardware_spi();
    timer_init.end();

    UserInterface::Heartbeat::Light light;
    if(!light.init())
        printf("Failed to initialize light\n");

    Drivers::MPU6050::Driver mpu6050;
    if(!mpu6050.init())
        printf("Failed to initialize MPU6050\n");

    UserInterface::Display::Display display;
    if(!display.init())
        printf("Failed to initialize display\n");

    bool first_pass = true;
    bool usb_cdc_sent_header = false;

    starscope_clock::time_point demo_last = starscope_clock::time_point::min();
    starscope_clock::duration demo_period = 5s;

    System::set_mode(System::Mode::ACTIVE);
    while (true) {
        if(first_pass || (!usb_cdc_sent_header && stdio_usb_connected() ))  {
            printf("\n--== STARSCOPE ==--\n");
            printf(timer_init.to_string().c_str());

            first_pass = false;

            if(stdio_usb_connected())
                usb_cdc_sent_header = true;
        }

        light.update();
        display.update();

        const auto now = starscope_clock::now();
        if((now - demo_last > demo_period) || (demo_last == starscope_clock::time_point::min())) {
            if(display.ready()) {
                printf("Running demo...\n");
                display.run_demo();
            } else {
                printf("display not initialized\n");
            }

            demo_last = now;
        }
    }
// #endif
    return 0;
}
