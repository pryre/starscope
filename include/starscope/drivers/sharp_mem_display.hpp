#ifndef _STARSCOPE_DRIVER_SHARPMEMDISPLAY_H
#define _STARSCOPE_DRIVER_SHARPMEMDISPLAY_H

#include "starscope/drivers/utils.hpp"
#include <array>
#include <span>

//XXX: https://github.com/pramasoul/micropython-SHARP_Memory_Display

namespace Starscope::Drivers::SharpMemDisplay {

const size_t MAX_BYTE_COMBINATIONS = 0x100; //0xFF + 1
// Lookup table for byte values in LSB format
const std::array<std::byte, MAX_BYTE_COMBINATIONS> BIT_FLIP = {
    std::byte{0x00}, std::byte{0x80}, std::byte{0x40}, std::byte{0xC0}, std::byte{0x20}, std::byte{0xA0}, std::byte{0x60}, std::byte{0xE0}, std::byte{0x10}, std::byte{0x90}, std::byte{0x50}, std::byte{0xD0}, std::byte{0x30}, std::byte{0xB0}, std::byte{0x70}, std::byte{0xF0},
    std::byte{0x08}, std::byte{0x88}, std::byte{0x48}, std::byte{0xC8}, std::byte{0x28}, std::byte{0xA8}, std::byte{0x68}, std::byte{0xE8}, std::byte{0x18}, std::byte{0x98}, std::byte{0x58}, std::byte{0xD8}, std::byte{0x38}, std::byte{0xB8}, std::byte{0x78}, std::byte{0xF8},
    std::byte{0x04}, std::byte{0x84}, std::byte{0x44}, std::byte{0xC4}, std::byte{0x24}, std::byte{0xA4}, std::byte{0x64}, std::byte{0xE4}, std::byte{0x14}, std::byte{0x94}, std::byte{0x54}, std::byte{0xD4}, std::byte{0x34}, std::byte{0xB4}, std::byte{0x74}, std::byte{0xF4},
    std::byte{0x0C}, std::byte{0x8C}, std::byte{0x4C}, std::byte{0xCC}, std::byte{0x2C}, std::byte{0xAC}, std::byte{0x6C}, std::byte{0xEC}, std::byte{0x1C}, std::byte{0x9C}, std::byte{0x5C}, std::byte{0xDC}, std::byte{0x3C}, std::byte{0xBC}, std::byte{0x7C}, std::byte{0xFC},
    std::byte{0x02}, std::byte{0x82}, std::byte{0x42}, std::byte{0xC2}, std::byte{0x22}, std::byte{0xA2}, std::byte{0x62}, std::byte{0xE2}, std::byte{0x12}, std::byte{0x92}, std::byte{0x52}, std::byte{0xD2}, std::byte{0x32}, std::byte{0xB2}, std::byte{0x72}, std::byte{0xF2},
    std::byte{0x0A}, std::byte{0x8A}, std::byte{0x4A}, std::byte{0xCA}, std::byte{0x2A}, std::byte{0xAA}, std::byte{0x6A}, std::byte{0xEA}, std::byte{0x1A}, std::byte{0x9A}, std::byte{0x5A}, std::byte{0xDA}, std::byte{0x3A}, std::byte{0xBA}, std::byte{0x7A}, std::byte{0xFA},
    std::byte{0x06}, std::byte{0x86}, std::byte{0x46}, std::byte{0xC6}, std::byte{0x26}, std::byte{0xA6}, std::byte{0x66}, std::byte{0xE6}, std::byte{0x16}, std::byte{0x96}, std::byte{0x56}, std::byte{0xD6}, std::byte{0x36}, std::byte{0xB6}, std::byte{0x76}, std::byte{0xF6},
    std::byte{0x0E}, std::byte{0x8E}, std::byte{0x4E}, std::byte{0xCE}, std::byte{0x2E}, std::byte{0xAE}, std::byte{0x6E}, std::byte{0xEE}, std::byte{0x1E}, std::byte{0x9E}, std::byte{0x5E}, std::byte{0xDE}, std::byte{0x3E}, std::byte{0xBE}, std::byte{0x7E}, std::byte{0xFE},
    std::byte{0x01}, std::byte{0x81}, std::byte{0x41}, std::byte{0xC1}, std::byte{0x21}, std::byte{0xA1}, std::byte{0x61}, std::byte{0xE1}, std::byte{0x11}, std::byte{0x91}, std::byte{0x51}, std::byte{0xD1}, std::byte{0x31}, std::byte{0xB1}, std::byte{0x71}, std::byte{0xF1},
    std::byte{0x09}, std::byte{0x89}, std::byte{0x49}, std::byte{0xC9}, std::byte{0x29}, std::byte{0xA9}, std::byte{0x69}, std::byte{0xE9}, std::byte{0x19}, std::byte{0x99}, std::byte{0x59}, std::byte{0xD9}, std::byte{0x39}, std::byte{0xB9}, std::byte{0x79}, std::byte{0xF9},
    std::byte{0x05}, std::byte{0x85}, std::byte{0x45}, std::byte{0xC5}, std::byte{0x25}, std::byte{0xA5}, std::byte{0x65}, std::byte{0xE5}, std::byte{0x15}, std::byte{0x95}, std::byte{0x55}, std::byte{0xD5}, std::byte{0x35}, std::byte{0xB5}, std::byte{0x75}, std::byte{0xF5},
    std::byte{0x0D}, std::byte{0x8D}, std::byte{0x4D}, std::byte{0xCD}, std::byte{0x2D}, std::byte{0xAD}, std::byte{0x6D}, std::byte{0xED}, std::byte{0x1D}, std::byte{0x9D}, std::byte{0x5D}, std::byte{0xDD}, std::byte{0x3D}, std::byte{0xBD}, std::byte{0x7D}, std::byte{0xFD},
    std::byte{0x03}, std::byte{0x83}, std::byte{0x43}, std::byte{0xC3}, std::byte{0x23}, std::byte{0xA3}, std::byte{0x63}, std::byte{0xE3}, std::byte{0x13}, std::byte{0x93}, std::byte{0x53}, std::byte{0xD3}, std::byte{0x33}, std::byte{0xB3}, std::byte{0x73}, std::byte{0xF3},
    std::byte{0x0B}, std::byte{0x8B}, std::byte{0x4B}, std::byte{0xCB}, std::byte{0x2B}, std::byte{0xAB}, std::byte{0x6B}, std::byte{0xEB}, std::byte{0x1B}, std::byte{0x9B}, std::byte{0x5B}, std::byte{0xDB}, std::byte{0x3B}, std::byte{0xBB}, std::byte{0x7B}, std::byte{0xFB},
    std::byte{0x07}, std::byte{0x87}, std::byte{0x47}, std::byte{0xC7}, std::byte{0x27}, std::byte{0xA7}, std::byte{0x67}, std::byte{0xE7}, std::byte{0x17}, std::byte{0x97}, std::byte{0x57}, std::byte{0xD7}, std::byte{0x37}, std::byte{0xB7}, std::byte{0x77}, std::byte{0xF7},
    std::byte{0x0F}, std::byte{0x8F}, std::byte{0x4F}, std::byte{0xCF}, std::byte{0x2F}, std::byte{0xAF}, std::byte{0x6F}, std::byte{0xEF}, std::byte{0x1F}, std::byte{0x9F}, std::byte{0x5F}, std::byte{0xDF}, std::byte{0x3F}, std::byte{0xBF}, std::byte{0x7F}, std::byte{0xFF}
};

const std::byte SHARPMEM_BIT_WRITECMD = BIT_FLIP[0x80]; //0x01  # 0x80 in LSB format
const std::byte SHARPMEM_BIT_VCOM = BIT_FLIP[0x40]; //0x02  # 0x40 in LSB format
const std::byte SHARPMEM_BIT_CLEAR = BIT_FLIP[0x20]; //0x04  # 0x20 in LSB format

// XXX: By default all of the pixel operations are inverted (0x01 is off)
const std::byte SHARPMEM_DUMMY_BYTE = std::byte{0x00};

const std::byte SHARPMEM_PIXEL_BYTE_OFF = std::byte{0xFF};
const std::byte SHARPMEM_PIXEL_BYTE_ON = std::byte{0x00};

template<size_t size_x, size_t size_y>
class Driver : public Utils::StatefulSystem  {
    private:
    static constexpr size_t _buffer_length = size_x * size_y;
    std::array<std::byte, _buffer_length> _lines;
    std::array<size_t, size_y> _lines_changed;
    std::byte _vcom;

    public:
    Driver();
    inline const Utils::Size size() const { return Utils::Size(size_x, size_y); };

    // Clears the internal buffer
    void clear(const bool set_changed=true);
    void fill(const bool set_changed=true);

    bool get_pixel(const size_t x, const size_t y) const;
    void set_pixel(const size_t x, const size_t y, const bool value, const bool set_changed=true);

    void set_line_horizontal(const size_t y, const bool value, const bool set_changed=true);
    void set_line_vertical(const size_t x, const bool value, const bool set_changed=true);

    void clear_sync();
    void sync();

    void demo_zebra();
    void demo_checker();

    private:
    bool _init();
    void deinit();

    void toggle_vcom();
    void write(std::span<std::byte>data) const;
};

}

#endif