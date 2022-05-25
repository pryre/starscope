#XXX: Typing module not supported by micropython, comment below to not break things at runtime!
# from typing import Iterator, MutableSet
from machine import Pin, SPI, Timer
import time
import random

#XXX: https://github.com/pramasoul/micropython-SHARP_Memory_Display

# class HardwareVCOM:
#     def __init__(self):
#         # VCOM must alternate at 0.5Hz to 30Hz to prevent damage to
#         # the LCD We use a hardware timer to make it more resistant to
#         # software failure. Once the timer is set up and driving the
#         # EXTCOMIN input to the display, we have the necessary
#         # hardware signal even if the code crashes (as long as we
#         # don't cause a deinit of the timer).  As written this uses
#         # timer 11, and pin Y4. We send the "Frame Inversion Flag",
#         # aka VCOM, in every serial transmission, so you can also
#         # maintain the necessary alternation of VCOM without
#         # connecting EXTCOMIN, as long as you sync() the display at
#         # least once every period of the timer (every 500ms as
#         # written), and don't crash.
#         self.vcom = 2
#         def toggle_vcom(l):
#             self.vcom ^= 2
#             pyb.LED(2).toggle() # DEBUG
#         self.timer = t = Timer(11)
#         # drive the hardware EXTCOMIN (terminal 4 on display):
#         self.timer_ch = t.channel(1, Timer.OC_TOGGLE, pin=Pin.board.Y4)
#         t.callback(toggle_vcom) # For if we don't use hardware VCOM
#         t.init(freq=2)          # with OC_TOGGLE results in output at 1Hz


# _hw_vcom = HardwareVCOM()       # Create the singleton immediately
# def get_vcom():
#     global _hw_vcom
#     return _hw_vcom.vcom


# /**************************************************************************/
# /*!
#     @brief Clears the screen
# */
# /**************************************************************************/
# void Adafruit_SharpMem::clearDisplay() {
#   memset(sharpmem_buffer, 0xff, (WIDTH * HEIGHT) / 8);

#   spidev->beginTransaction();
#   // Send the clear screen command rather than doing a HW refresh (quicker)
#   digitalWrite(_cs, HIGH);

#   uint8_t clear_data[2] = {_sharpmem_vcom | SHARPMEM_BIT_CLEAR, 0x00};
#   spidev->transfer(clear_data, 2);

#   TOGGLE_VCOM;
#   digitalWrite(_cs, LOW);
#   spidev->endTransaction();
# }

# /**************************************************************************/
# /*!
#     @brief Renders the contents of the pixel buffer on the LCD
# */
# /**************************************************************************/
# void Adafruit_SharpMem::refresh(void) {
#   uint16_t i, current_line;

#   spidev->beginTransaction();
#   // Send the write command
#   digitalWrite(_cs, HIGH);

#   spidev->transfer(_sharpmem_vcom | SHARPMEM_BIT_WRITECMD);
#   TOGGLE_VCOM;

#   uint8_t bytes_per_line = WIDTH / 8;
#   uint16_t total_bytes = (WIDTH * HEIGHT) / 8;

#   for (i = 0; i < total_bytes; i += bytes_per_line) {
#     uint8_t line[bytes_per_line + 2];

#     // Send address byte
#     current_line = ((i + 1) / (WIDTH / 8)) + 1;
#     line[0] = current_line;
#     // copy over this line
#     memcpy(line + 1, sharpmem_buffer + i, bytes_per_line);
#     // Send end of line
#     line[bytes_per_line + 1] = 0x00;
#     // send it!
#     spidev->transfer(line, bytes_per_line + 2);
#   }

#   // Send another trailing 8 bits for the last line
#   spidev->transfer(0x00);
#   digitalWrite(_cs, LOW);
#   spidev->endTransaction();
# }

class Size:
    def __init__(self, x=0, y=0) -> None:
        self.x = x
        self.y = y

class SharpMemDisplay:
    def __init__(self, spi_id:int, sck:Pin, mosi:Pin, cs:Pin, size:Size):
        self.spi = SPI(spi_id)
        self.sck = sck
        self.mosi = mosi
        self.cs = cs
        self.cs.value(0)
        self.size = size
        self.lines = [bytearray(self.size.x//8) for i in range(self.size.y)]
        self.changed:MutableSet[int] = set()

    def begin(self):
        self.spi.init(10_000_000, sck=self.sck, mosi=self.mosi)
        self.clear()

    def shutdown(self):
        self.spi.deinit()

    def clear(self):
        self.set_all(0 for i in range(self.size.x * self.size.y // 8))

    def get_pix(self, x, y):
        return bool(self.lines[y][x//8] & (1 << x%8))

    def set_pix(self, x:int, y:int, v:bool):
        byte = x//8
        bitmask = 1 << x%8
        line_buf = self.lines[y]
        line_buf[byte] &= ~bitmask
        if v:
            line_buf[byte] |= bitmask
        self.changed.add(y)

    def set_line(self, line_ix, values):
        b = self.lines[line_ix]
        for i, v in enumerate(values):
            b[i] = v
        self.changed.add(line_ix)

    def set_all(self, values:Iterator[int]):
        lines = self.lines
        # vi = iter(values)
        xb = self.size.x // 8
        for ix in range(len(self.lines)):
            b = lines[ix]
            for i, v in zip(range(xb), values): # type: ignore
                b[i] = v
            self.changed.add(ix)

    def sync(self):
        # send = self.spi.send
        # set_cs = self.cs.value
        # vcom = get_vcom()
        syncing = True
        while syncing:
            self.cs.value(1)
            time.sleep_us(6)          # tsSCS
            try:
                ix = self.changed.pop()
            except KeyError:
                syncing = False
                # self.spi.write(0 | vcom)
                self.spi.write(b'0x00')
            else:
                self.spi.write(b'0x01')
                self.spi.write((ix+1).to_bytes(1, 'big'))
                self.spi.write(self.lines[ix])
            self.spi.write(b'0x00')
            time.sleep_us(2)          # th_SCS
            self.cs.value(0)
            time.sleep_us(2)          # th_SCS_L


# A little demo: Brownian motion.
def brown(screen:SharpMemDisplay):
    screen.clear()
    x = screen.size.x // 2
    y = screen.size.y // 2
    mls = 255 * screen.size.x // 8
    on = True
    step = 0
    while step < 1000:
        if on:
            on = any(sum(line) < mls for line in screen.lines)
        else:
            on = all(sum(line) == 0 for line in screen.lines)
        for i in range(1000):
            x = (x + random.randint(0, 2) - 1) % screen.size.x
            y = (y + random.randint(0, 2) - 1) % screen.size.y
            screen.set_pix(x, y, on)
            screen.sync()

        step = 1000