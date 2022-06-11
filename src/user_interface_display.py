# from typing import Iterable
# from typing import Any, Union
from machine import Pin, Timer

# from .drivers.utils import benchmark

from .tools_stateful_system import StatefulSystem
from .drivers.font import FONT_WIDTH, FONT_HEIGHT
from .drivers.font import font8x8_basic, FONT8X8_BASIC_MIN, FONT8X8_BASIC_MAX
from .drivers.font import font8x8_block, FONT8X8_BLOCK_MIN, FONT8X8_BLOCK_MAX
from .drivers.font import font8x8_box, FONT8X8_BOX_MIN, FONT8X8_BOX_MAX
from .drivers.font import font8x8_ext_latin, FONT8X8_EXT_LATIN_MIN, FONT8X8_EXT_LATIN_MAX
from .drivers.font import font8x8_greek, FONT8X8_GREEK_MIN, FONT8X8_GREEK_MAX
from .drivers.font import font8x8_hiragana, FONT8X8_HIRAGANA_MIN, FONT8X8_HIRAGANA_MAX
from .drivers.font import font8x8_sga, FONT8X8_SGA_MIN, FONT8X8_SGA_MAX
# micropython.alloc_emergency_exception_buf(100)

from .drivers.sharp_mem_display import SharpMemDisplay
from .drivers.utils import Size
# screen = SharpMemDisplay(2, 'Y5', 96, 96)
# screen.clear()
# xdim = screen.xdim
# ydim = screen.ydim
# x = xdim // 2
# y = ydim // 2
# mls = 255 * xdim // 8
# on = True
# while True:
#     if on:
#         on = any(sum(line) < mls for line in screen.lines)
#     else:
#         on = all(sum(line) == 0 for line in screen.lines)
#     for i in range(1000):
#         x = (x + rng()%3 - 1) % xdim
#         y = (y + rng()%3 - 1) % ydim
#         screen.set_pix(x, y, on)
#         screen.sync()

class UserInterfaceDisplay(StatefulSystem):
    def __init__(self) -> None:
        size = Size(400, 240)
        sck = Pin(2, Pin.OUT)
        mosi = Pin(3, Pin.OUT)
        cs = Pin(5, Pin.OUT)
        self.screen = SharpMemDisplay(0, sck, mosi, cs, size)
        self.timer = Timer()
        self.demo_state = 0

    def init(self):
        self.screen.init()
        self.clear()
        self.timer.init(freq=10, callback=self._update) #Need to sync the screen at 0.5Hz < f < 10Hz

    def _update(self, event):
        # with benchmark("sync"):
        self.screen.sync()

    def deinit(self):
        self.timer.deinit()
        self.clear()
        self.screen.deinit()

    # def _constrain(self, x, minimum, maximum):
    #     return minimum if x < minimum else maximum if x > maximum else x

    def px_from_line(self, line:int, start_y:int=0):
        return (start_y + FONT_HEIGHT * line) % self.screen.size.y

    def px_from_column(self, column:int, start_x:int=0):
        return (start_x + FONT_WIDTH * column) % self.screen.size.x

    def _render_font(self, bitmap:Iterable[int], x, y, invert=False, scaling=1): #type:ignore
        for i, data in enumerate(bitmap):
            for j in range(FONT_WIDTH):
                v = bool(data & (1 << j)) ^ invert
                for k in range(scaling):
                    for l in range(scaling):
                        self.screen.set_pixel(x + j*scaling + k, y + i*scaling + l, v)

    def print(self, text:Union[str,Any], location:Size=Size(), invert=False, trailing_newline=True, scaling=1):#type: ignore
        col = 0
        row = 0
        for char in str(text):
            i = ord(char)
            printed_char = None
            if (i >= FONT8X8_BASIC_MIN) and (i <= FONT8X8_BASIC_MAX):
                if i == ord('\r'):
                    col = 0
                elif i == ord('\n'):
                    col = 0
                    row += scaling
                else:
                    printed_char = font8x8_basic[i-FONT8X8_BASIC_MIN]
            elif (i >= FONT8X8_BLOCK_MIN) and (i <= FONT8X8_BLOCK_MAX):
                printed_char = font8x8_block[i-FONT8X8_BLOCK_MIN]
            elif (i >= FONT8X8_BOX_MIN) and (i <= FONT8X8_BOX_MAX):
                printed_char = font8x8_box[i-FONT8X8_BOX_MIN]
            elif (i >= FONT8X8_EXT_LATIN_MIN) and (i <= FONT8X8_EXT_LATIN_MAX):
                printed_char = font8x8_ext_latin[i-FONT8X8_EXT_LATIN_MIN]
            elif (i >= FONT8X8_GREEK_MIN) and (i <= FONT8X8_GREEK_MAX):
                printed_char = font8x8_greek[i-FONT8X8_GREEK_MIN]
            elif (i >= FONT8X8_HIRAGANA_MIN) and (i <= FONT8X8_HIRAGANA_MAX):
                printed_char = font8x8_hiragana[i-FONT8X8_HIRAGANA_MIN]
            elif (i >= FONT8X8_SGA_MIN) and (i <= FONT8X8_SGA_MAX):
                printed_char = font8x8_sga[i-FONT8X8_SGA_MIN]
            else:
                print(f'Unknown char: 0x{i:x}')
            #else different font map?

            if printed_char:
                self._render_font(printed_char, location.x + col*FONT_WIDTH, location.y + row*FONT_HEIGHT, invert, scaling)
                col += scaling

        if trailing_newline:
            col = 0
            row += scaling

        s = Size(self.px_from_column(col, location.x), self.px_from_line(row, location.y))
        return s

    def run_demo(self):
        # self.screen.demo_brown()
        self.demo_state += 1
        self.screen.clear_sync()

        if self.demo_state == 1:
            print("Demo zebra!")
            self.screen.demo_zebra()
        elif self.demo_state == 2:
            print("Demo checker!")
            self.screen.demo_checker()
        elif self.demo_state == 3:
            print("Demo words!")
            start = Size()
            start = self.print("Hello World", start, trailing_newline=False)
            start = self.print(", this is me~", start)
            start = self.print("Thus, a new\nlegend is born", start, True)
            start = self.print("...and the world goes on anew", start)
        # elif self.demo_state == 2:
        #     print("Demo brown!")
        #     self.screen.demo_brown()
        else:
            self.demo_state = 0
            print("Demo done!")

    def clear(self):
        self.screen.clear_sync()
        # self.screen.sync()

    def fill(self):
        self.screen.fill()
        # self.screen.sync()
