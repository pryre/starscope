#ifndef _STARSCOPE_USER_INTERFACE_DISPLAY_H
#define _STARSCOPE_USER_INTERFACE_DISPLAY_H

#include <string>
#include <span>
#include <inttypes.h>
#include "starscope/drivers/utils.hpp"
#include "starscope/drivers/sharp_mem_display.hpp"

using namespace Starscope;
namespace Starscope::UserInterface::Display {

typedef enum {
    TITLE,
    WORDS,
    ZEBRA,
    CHECKER,
    FILL,
} DEMO_STATE;

const size_t SCREEN_SIZE_X = 400;
const size_t SCREEN_SIZE_Y = 240;
constexpr size_t FONT_WIDTH = sizeof(uint8_t)*8;
const size_t FONT_HEIGHT = 8;

const char ASCII_PRINTABLE_START = 0x20; //First ascii char: ' '

class Display : public Utils::StatefulSystem {
    private:
    DEMO_STATE _state;
    Drivers::SharpMemDisplay::Driver<SCREEN_SIZE_X,SCREEN_SIZE_Y> _screen;

    public:
    Display();

    void clear();
    void fill();

    Utils::Size print_ascii(const std::string text, const Utils::Size location=Utils::Size(), const bool invert=false, const bool trailing_newline=true, const int scaling=1);

    void run_demo();

    private:
    bool _init();
    void _deinit();
    void _update(const starscope_clock::time_point now);
    size_t px_from_line(const size_t line, const size_t start_y=0) const;
    size_t px_from_column(const size_t column, const size_t start_x=0) const;
    void render_font(std::span<const char, FONT_HEIGHT>bitmap, const size_t x, const size_t y, const bool invert=false, const unsigned int scaling=1);
};

}

#endif
