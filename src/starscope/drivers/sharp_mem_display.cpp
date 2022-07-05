#include "starscope/drivers/sharp_mem_display.hpp"

namespace Starscope::Drivers::SharpMemDisplay {

template<size_t size_x, size_t size_y>
Driver<size_x, size_y>::Driver(spi_inst_t* spi) :
_vcom(SHARPMEM_BIT_VCOM),
_spi(spi) {
    _lines_changed.reserve(size_y);
    // self.spi = None
    // self._vcom = SharpMemDisplay.SHARPMEM_BIT_VCOM

    // self.spi_id = spi_id
    // self.sck = sck
    // self.mosi = mosi
    // self.cs = cs
    // self.cs.value(0)

    // self.size = size
    // self.lines = [[SharpMemDisplay.SHARPMEM_PIXEL_BYTE_OFF]*(self.size.x//8) for i in range(self.size.y)]
    // self.changed:MutableSet[int] = set()
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::toggle_vcom() {
    _vcom = _vcom == SHARPMEM_BIT_VCOM ? SHARPMEM_ZERO_BYTE : SHARPMEM_BIT_VCOM;
}

template<size_t size_x, size_t size_y>
bool Driver<size_x, size_y>::_init() {
    _vcom = SHARPMEM_BIT_VCOM;
    _lines_changed.clear();

    return true;
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::_deinit() {
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::write(std::span<std::byte>data) const {
    spi_write_blocking(_spi, (uint8_t*)(data.data()), data.size());
}


template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::clear(const bool set_changed) {
    //TODO:
    // for i in range(self.size.y):
    //     self.set_line_horizontal(i, False, set_changed)

    // # for i in range(self.size.y):
    // #     self.set_line_horizontal(i, False)
    // # self.set_all(0 for i in range(self.size.x * self.size.y // 8))

}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::fill(const bool set_changed) {
    //TODO:
    // for i in range(self.size.y):
    //     self.set_line_horizontal(i, True, set_changed)
    // # self.set_all(1 for i in range(self.size.x * self.size.y // 8))

}

template<size_t size_x, size_t size_y>
bool Driver<size_x, size_y>::get_pixel(const size_t x, const size_t y) const {
    //TODO:
    // return not bool(self.lines[y][x//8] & (1 << x%8))
    return false;
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::set_pixel(const size_t x, const size_t y, const bool value, const bool set_changed) {
    // # if (x < 0) or (x >= self.size.x) or (y < 0) or (y >= self.size.y):
    // #     raise IndexError("[{}, {}] is out of range of screen size: {}".format(x, y, self.size))

    // # Get appropriate index of byte in line
    // byte = x//8
    // # Get a bitmask for the bit we want to set
    // bitmask = 1 << x%8
    // # Get the line we want to edit in the y axis
    // line_buf = self.lines[y]
    // # Apply the bitmask to our byte in the x axis
    // if v:
    //     # On
    //     line_buf[byte] &= ~bitmask
    // else:
    //     # Off
    //     line_buf[byte] |= bitmask

    // # Mark the line to be sync'd later
    // if set_changed:
    //     self.changed.add(y)
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::set_line_horizontal(const size_t y, const bool value, const bool set_changed) {
    //TODO:
    // # steps = (self.size.x//8)
    // # for i in self.lines[y]:
    // #     i = SharpMemDisplay.SHARPMEM_PIXEL_BYTE_ON if v else SharpMemDisplay.SHARPMEM_PIXEL_BYTE_OFF
    // if v:
    //     for i in range(len(self.lines[y])):
    //         self.lines[y][i] = SharpMemDisplay.SHARPMEM_PIXEL_BYTE_ON
    // else:
    //     for i in range(len(self.lines[y])):
    //         self.lines[y][i] = SharpMemDisplay.SHARPMEM_PIXEL_BYTE_OFF

    // # Mark the line to be sync'd later
    // if set_changed:
    //     self.changed.add(y)
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::set_line_vertical(const size_t x, const bool value, const bool set_changed) {
    //TODO:
    // # Get appropriate index of byte in line
    // byte = x//8
    // # Get a bitmask for the bit we want to set
    // bitmask = 1 << x%8

    // for y, line_buf in enumerate(self.lines):
    //     # Apply the bitmask to our byte in the x axis
    //     if v:
    //         # On
    //         line_buf[byte] &= ~bitmask
    //     else:
    //         # Off
    //         line_buf[byte] |= bitmask

    //     # Mark the line to be sync'd later
    //     if set_changed:
    //         self.changed.add(y)

}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::clear_sync() {
    //TODO:

    // def _clear_sync_generator(self):
    //     yield SharpMemDisplay.BIT_FLIP[self._vcom | SharpMemDisplay.SHARPMEM_BIT_CLEAR]
    //     yield SharpMemDisplay.BIT_FLIP[SharpMemDisplay.SHARPMEM_DUMMY_BYTE]


    // self.cs.value(1)

    // # data = bytes([self._vcom | SharpMemDisplay.SHARPMEM_BIT_CLEAR, SharpMemDisplay.SHARPMEM_DUMMY_BYTE])
    // data = self._clear_sync_generator()
    // self._write(data)

    // self.TOGGLE_VCOM()
    // self.cs.value(0)

    // self.clear(False)
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::sync() {
    //TODO:

    // def _sync_generator(self):
    //     yield SharpMemDisplay.BIT_FLIP[self._vcom | self.SHARPMEM_BIT_WRITECMD]

    //     while self.changed:
    //         ix = self.changed.pop()
    //         # print("Line: " + str(ix))

    //         # for i in bytes([ix+1]):
    //         #     yield i
    //         yield SharpMemDisplay.BIT_FLIP[ix + 1]

    //         for i in self.lines[ix]:
    //             yield SharpMemDisplay.BIT_FLIP[i]

    //         yield SharpMemDisplay.BIT_FLIP[SharpMemDisplay.SHARPMEM_DUMMY_BYTE]

    //     # Final byte
    //     # print("Tail")
    //     yield SharpMemDisplay.BIT_FLIP[SharpMemDisplay.SHARPMEM_DUMMY_BYTE]


    // self.cs.value(1)
    // sync_data = self._sync_generator()
    // # print("Head")
    // self._write(sync_data)

    // # out = bytearray()

    // # out += b'0x00'
    // # self.write(out)
    // self.TOGGLE_VCOM()
    // self.cs.value(0)
}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::demo_zebra() {
    //TODO:
    // self.clear()

    // state = True
    // for iy in range(self.size.y):
    //     self.set_line_horizontal(iy, state)
    //     state = not state

    // # self.sync()

}

template<size_t size_x, size_t size_y>
void Driver<size_x, size_y>::demo_checker() {
    //TODO:
    // self.clear()

    // state = True
    // for iy in range(self.size.y):
    //     self.set_line_horizontal(iy, state)
    //     state = not state

    // state = True
    // for ix in range(self.size.x):
    //     if(state):
    //         self.set_line_vertical(ix, state)
    //     state = not state

    // # self.sync()
}

}
