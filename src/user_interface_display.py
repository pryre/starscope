from machine import Pin, Timer

# from .drivers.utils import benchmark

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

    def run_demo(self):
        # self.screen.demo_brown()
        if self.demo_state == 0:
            print("Demo zebra!")
            self.screen.demo_zebra()
        elif self.demo_state == 1:
            print("Demo checker!")
            self.screen.demo_checker()
        # elif self.demo_state == 2:
        #     print("Demo brown!")
        #     self.screen.demo_brown()

        self.demo_state += 1
        if self.demo_state >= 2:
            self.demo_state = 0
        # print("Demo checker!")
        # self.screen.demo_checker()
        # time.sleep(1)
        print("Demo done!")

    def clear(self):
        self.screen.clear_sync()
        # self.screen.sync()

    def fill(self):
        self.screen.fill()
        # self.screen.sync()
