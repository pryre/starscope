from machine import Pin

# micropython.alloc_emergency_exception_buf(100)

from .drivers.sharp_mem_display import SharpMemDisplay, Size, brown
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




class UserInterfaceDisplay():
    def __init__(self) -> None:
        size = Size(400, 240)
        sck = Pin(2, Pin.OUT)
        mosi = Pin(3, Pin.OUT)
        cs = Pin(5, Pin.OUT)
        self.screen = SharpMemDisplay(0, sck, mosi, cs, size)

    def begin(self):
        self.screen.shutdown()

    def shutdown(self):
        self.screen.shutdown()

    def run_demo(self):
        brown(self.screen)