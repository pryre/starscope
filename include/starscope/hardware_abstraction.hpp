#ifndef _STARSCOPE_HARDWARE_ABSTRACTION_H
#define _STARSCOPE_HARDWARE_ABSTRACTION_H

#include<span>
#include <inttypes.h>

uint64_t time_monotonic_us();

void heartbeat_set(const bool onoff);

typedef enum {
    None,
    SharpMemDisplay,
} STARSCOPE_SPI_SELECT;

size_t spi_write(const std::byte src);
size_t spi_write(const std::span<const std::byte>src);
// size_t spi_write_read(const std::span<const std::byte>src, const std::span<const std::byte>dst);
void spi_set_cs(STARSCOPE_SPI_SELECT chip);

size_t i2c_write(const std::byte addr, const std::span<const std::byte>src, const bool nostop = false);
size_t i2c_read(const std::byte addr, const std::span<std::byte>dst, const bool nostop = false);

#endif
