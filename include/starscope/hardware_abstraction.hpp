#ifndef _STARSCOPE_HARDWARE_ABSTRACTION_H
#define _STARSCOPE_HARDWARE_ABSTRACTION_H

#include<span>
#include <inttypes.h>

uint64_t time_monotonic_us();

size_t spi_write(const std::span<const std::byte>src);
// size_t spi_write_read(const std::span<const std::byte>src, const std::span<const std::byte>dst);

size_t i2c_write(const std::byte addr, const std::span<const std::byte>src, const bool nostop = false);
size_t i2c_read(const std::byte addr, const std::span<std::byte>dst, const bool nostop = false);

#endif
