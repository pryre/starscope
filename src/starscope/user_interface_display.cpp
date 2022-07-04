#include "starscope/user_interface_display.hpp"
#include "font8x8/font8x8_basic.h"
#include "starscope/drivers/sharp_mem_display.hpp"

using namespace Starscope;

UserInterfaceDisplay::UserInterfaceDisplay() :
_state(DEMO_STATE::WORDS) {
    // screen = SharpMemDisplay(0, sck, mosi, cs, size)
    // timer = Timer()
    //TODO: Finish
}

bool UserInterfaceDisplay::_init() {
    // self.screen.init()
    clear();
    // self.timer.init(freq=10, callback=self._update) #Need to sync the screen at 0.5Hz < f < 10Hz
    //TODO: Finish

    return true;
}

void UserInterfaceDisplay::_deinit() {
    // self.timer.deinit()
    clear();
    // self.screen.deinit()
    //TODO: Finish
}


size_t UserInterfaceDisplay::px_from_line(const size_t line, const size_t start_y) const {
    return (start_y + FONT_HEIGHT * line) % _screen.size().y;
}

size_t UserInterfaceDisplay::px_from_column(const size_t column, const size_t start_x) const {
    return (start_x + FONT_WIDTH * column) % _screen.size().x;
}

void UserInterfaceDisplay::render_font(std::span<const char, FONT_HEIGHT>bitmap, const size_t x, const size_t y, const bool invert, const unsigned int scaling) {
    for(size_t i; i<bitmap.size(); i++) {
        const uint8_t data = bitmap[i];

        for(size_t j; j<FONT_WIDTH; j++) {
            const bool v = ((bool)(data & (1 << j))) ^ invert;

            for(size_t k; k<scaling; k++) {
                for(size_t l; l<scaling; l++) {
                    _screen.set_pixel(x + j*scaling + k, y + i*scaling + l, v);
                }
            }
        }
    }
}

void UserInterfaceDisplay::update() const {
    // with benchmark("sync"):
    // self.screen.sync()
    //TODO: Finish
}


void UserInterfaceDisplay::clear() {
    _screen.clear_sync();
}

void UserInterfaceDisplay::fill() {
    _screen.fill();
}

Utils::Size UserInterfaceDisplay::print_ascii(const std::string text, Utils::Size location, const bool invert, const bool trailing_newline, const int scaling) {
    size_t col = 0;
    size_t row = 0;

    for(const auto i : text) {
        if (i == '\r') {
            col = 0;
        } else if (i == '\n') {
            col = 0;
            row += scaling;
        } else if(i >= ASCII_PRINTABLE_START) {
            render_font(font8x8_basic[i-ASCII_PRINTABLE_START], location.x + col*FONT_WIDTH, location.y + row*FONT_HEIGHT, invert, scaling);
            col += scaling;
        }
    }

    if(trailing_newline) {
        col = 0;
        row += scaling;
    }

    return Utils::Size(px_from_column(col, location.x), px_from_line(row, location.y));
}

void UserInterfaceDisplay::run_demo() {
    clear();

    switch (_state)
    {
    case DEMO_STATE::ZEBRA: {
        // print_ascii("Demo zebra!");
        _screen.demo_zebra();
        _state = DEMO_STATE::CHECKER;
        break;
    }
    case DEMO_STATE::CHECKER: {
        // print_ascii("Demo zebra!");
        _screen.demo_checker();
        _state = DEMO_STATE::WORDS;
        break;
    }
    case DEMO_STATE::WORDS : {
        // print("Demo words!");
        Utils::Size start;
        start = print_ascii("Hello World", start, false, false);
        start = print_ascii(", this is me~", start);
        start = print_ascii("Thus, a new\nlegend is born", start, true);
        start = print_ascii("...and the world goes on anew", start);

        _state = DEMO_STATE::ZEBRA;
        break;
    }
    default: {
        print_ascii("Unknown demo!");
        break;
    }
    }
}
