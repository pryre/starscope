#include "starscope/drivers/sharp_mem_display.hpp"

namespace Starscope::Drivers::SharpMemDisplay {

template<size_t size_x, size_t size_y>
Driver<size_x, size_y>::Driver() :
_vcom(SHARPMEM_BIT_VCOM),
_last_update(starscope_clock::time_point::min()),
_rate_update(SHARPMEM_UPDATE_RATE) {
    _lines_changed.reserve(size_y);
    // self.lines = [[SharpMemDisplay.SHARPMEM_PIXEL_BYTE_OFF]*(self.size.x//8) for i in range(self.size.y)]
    // self.changed:MutableSet[int] = set()
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::_toggle_vcom() {
    _vcom = _vcom == SHARPMEM_BIT_VCOM ? SHARPMEM_ZERO_BYTE : SHARPMEM_BIT_VCOM;
}

template<size_t size_x, size_t size_y>
bool Driver<size_x, size_y>::_init() {
    _vcom = SHARPMEM_BIT_VCOM;
    _lines_changed.clear();
    clear(false);

    return true;
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::_deinit() {
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::_update(const starscope_clock::time_point now) {
    //Maintain a background update rate to keep the screen happy (~10Hz)
    //This should also alleviate the need to perform manual sync's on the implementation side
    if(now - _last_update > _rate_update) {
        // with benchmark("sync"):
        sync();
        _last_update = now;
    }
}

// template<size_t size_x, size_t size_y>
// void Driver<size_x, size_y>::write(std::span<std::byte>data) const {
//     spi_write_blocking(_spi, (uint8_t*)(data.data()), data.size());
// }


template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::set_all(const bool onoff, const bool set_changed) {
    if(set_changed)
        for(size_t i=0; i<size_y; i++)
        _lines_changed.push_back(i);

    _lines.fill(onoff ? SHARPMEM_PIXEL_BYTE_ON : SHARPMEM_PIXEL_BYTE_OFF);
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::clear(const bool set_changed) {
    set_all(false, set_changed);
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::fill(const bool set_changed) {
    set_all(true, set_changed);
}

// template<size_t size_x, size_t size_y>
// std::byte Driver<size_x, size_y>::_flipped(const std::byte b) const {
//     return BIT_FLIP[(size_t)b];
// }

template<size_t size_x, size_t size_y>
size_t Driver<size_x, size_y>::_ind_from_x_y(const size_t x, const size_t y) const {
    return y * _step + x/sizeof(std::byte);
}

template<size_t size_x, size_t size_y>
std::byte Driver<size_x, size_y>::_sub_byte_bitmask(const size_t x) const {
    const uint8_t step = x%sizeof(std::byte);
    return std::byte{(uint8_t)(0b1000'0000 >> step)};
}

template<size_t size_x, size_t size_y>
bool Driver<size_x, size_y>::get_pixel(const size_t x, const size_t y) const {
    return ((x < size_x) && (y < size_y)) ? !(bool)(_lines[_ind_from_x_y(x,y)] & _sub_byte_bitmask(x)) : false;
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::set_pixel(const size_t x, const size_t y, const bool value, const bool set_changed) {
    if((x < size_x) && (y < size_y)) {
        // Get a bitmask for the bit we want to set
        const std::byte bitmask = _sub_byte_bitmask(x);
        // Apply the bitmask to our byte in the x axis (invert to be consistent with on/off layout)
        if (value) {
            _lines[_ind_from_x_y(x,y)] &= ~bitmask;
        } else {
            _lines[_ind_from_x_y(x,y)] |= bitmask;
        }

        // Mark the line to be sync'd later
        if (set_changed)
            _lines_changed.push_back(y);
    }
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::set_line_horizontal(const size_t y, const bool value, const bool set_changed) {
    std::span lines{_lines};
    std::span line = lines.subspan(_ind_from_x_y(0,y), _step);
    // auto line = lines.subspan(_ind_from_x_y(0,y), line_len);
    std::fill(line.begin(), line.end(), value ? SHARPMEM_PIXEL_BYTE_ON : SHARPMEM_PIXEL_BYTE_OFF);

    // Mark the line to be sync'd later
    if (set_changed)
        _lines_changed.push_back(y);
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::set_line_vertical(const size_t x, const bool value, const bool set_changed) {
    size_t ind0 = _ind_from_x_y(x,0);
    std::byte bitmask = _sub_byte_bitmask(x);

    for(size_t y=0; y<size_y; y++) {
        size_t ind = ind0 + y*_step;

        if (value) {
            _lines[ind] &= ~bitmask;
        } else {
            _lines[ind] |= bitmask;
        }

        // Mark the line to be sync'd later
        if (set_changed)
            _lines_changed.push_back(y);
    }
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::clear_sync() {
    spi_set_cs(STARSCOPE_SPI_SELECT::SharpMemDisplay);

    const std::array<std::byte, 2> data = {
        _vcom | SHARPMEM_BIT_CLEAR,
        SHARPMEM_ZERO_BYTE
    };

    spi_write(data);

    _toggle_vcom();
    spi_set_cs(STARSCOPE_SPI_SELECT::None);

    clear(false);
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::sync() {
    std::span lines{_lines};

    spi_set_cs(STARSCOPE_SPI_SELECT::SharpMemDisplay);

    spi_write(_vcom | SHARPMEM_BIT_WRITECMD);

    for(auto&y : _lines_changed) {
        spi_write(BIT_FLIP[y+1]);

        std::span line = lines.subspan(_ind_from_x_y(0,y), _step);

        for(auto&b : line)
            spi_write(b);

        spi_write(SHARPMEM_ZERO_BYTE);
    }

    spi_write(SHARPMEM_ZERO_BYTE);

    _toggle_vcom();
    spi_set_cs(STARSCOPE_SPI_SELECT::None);
    _lines_changed.clear();
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::demo_zebra() {
    clear();

    bool state = true;
    for(size_t y=0; y<size_y; y++) {
        set_line_horizontal(y, state);
        state = !state;
    }

    sync();
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::demo_checker() {
    clear();

    bool state = true;
    for(size_t y=0; y<size_y; y++) {
        set_line_horizontal(y, state);
        state = !state;
    }

    state = true;
    for(size_t x=0; x<size_x; x++) {
        if(state)
            set_line_vertical(x, state);
        state = !state;
    }

    sync();
}

template class Driver_LS013B4DN04;
template class Driver_LS027B7DH01;

}
