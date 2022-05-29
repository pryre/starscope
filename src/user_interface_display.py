import time
from machine import Pin

from .user_interface_base import UserInterfaceBase

# micropython.alloc_emergency_exception_buf(100)

from .drivers.sharp_mem_display import SharpMemDisplay, Size
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

class UserInterfaceDisplay(UserInterfaceBase):
    def __init__(self) -> None:
        size = Size(400, 240)
        sck = Pin(2, Pin.OUT)
        mosi = Pin(3, Pin.OUT)
        cs = Pin(5, Pin.OUT)
        self.screen = SharpMemDisplay(0, sck, mosi, cs, size)

    def init(self):
        self.screen.init()
        self.clear()

    def deinit(self):
        self.clear()
        self.screen.deinit()

    def run_demo(self):
        # self.screen.demo_brown()
        # time.sleep(2)
        print("Demo zebra!")
        self.screen.demo_zebra()
        time.sleep(1)
        print("Demo checker!")
        self.screen.demo_checker()
        time.sleep(1)
        print("Demo done!")

    def clear(self):
        self.screen.clear_sync()
        # self.screen.sync()

    def fill(self):
        self.screen.fill_pix()
        self.screen.sync()