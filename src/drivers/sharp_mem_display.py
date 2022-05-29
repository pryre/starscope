#XXX: Typing module not supported by micropython, comment below to not break things at runtime!
# from typing import Iterator, MutableSet
from machine import Pin, SPI
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

    def __str__(self):
        return "Size: [{},{}]".format(self.x, self.y)

class SharpMemDisplay:
    # Lookup table for byte values
    # SET_BIT = bytearray([1, 2, 4, 8, 16, 32, 64, 128])

    SHARPMEM_BIT_WRITECMD = 0x01 # 0x80 in LSB format
    SHARPMEM_BIT_VCOM = 0x02     # 0x40 in LSB format
    SHARPMEM_BIT_CLEAR = 0x04    # 0x20 in LSB format

    def __init__(self, spi_id:int, sck:Pin, mosi:Pin, cs:Pin, size:Size):
        self.spi = None
        self._vcom = SharpMemDisplay.SHARPMEM_BIT_VCOM

        self.spi_id = spi_id
        self.sck = sck
        self.mosi = mosi
        self.cs = cs
        self.cs.value(0)

        self.size = size
        self.lines = [bytearray(self.size.x//8) for i in range(self.size.y)]
        self.changed:MutableSet[int] = set() #type: ignore

    def TOGGLE_VCOM(self):
        self._vcom = 0x00 if self._vcom else SharpMemDisplay.SHARPMEM_BIT_VCOM

    def init(self):
        self._vcom = SharpMemDisplay.SHARPMEM_BIT_VCOM
        self.spi = SPI(self.spi_id, 2_000_00, firstbit=SPI.LSB, sck=self.sck, mosi=self.mosi)

    def deinit(self):
        if self.spi:
            self.spi.deinit()

    # Clears the entire screen with a hardware wipe
    def clear_sync(self):
        if self.spi is not None:
            self.cs.value(1)

            data = (self._vcom | SharpMemDisplay.SHARPMEM_BIT_CLEAR).to_bytes(1, 'big') + b'0x00'
            self.spi.write(data)

            self.TOGGLE_VCOM()
            self.cs.value(0)

        self.clear()

    # Clears the internal buffer
    def clear(self):
        for i in range(self.size.y):
            self.set_line_horizontal(i, False)

        # for i in range(self.size.y):
        #     self.set_line_horizontal(i, False)
        # self.set_all(0 for i in range(self.size.x * self.size.y // 8))

    def fill_pix(self):
        for i in range(self.size.y):
            self.set_line_horizontal(i, True)
        # self.set_all(1 for i in range(self.size.x * self.size.y // 8))

    def get_pix(self, x, y):
        return bool(self.lines[y][x//8] & (1 << x%8))

    def set_pix(self, x:int, y:int, v:bool):
        if (x < 0) or (x >= self.size.x) or (y < 0) or (y >= self.size.y):
            raise IndexError("[{}, {}] is out of range of screen size: {}".format(x, y, self.size))

        # Get appropriate index of byte in line
        byte = x//8
        # Get a bitmask for the bit we want to set
        bitmask = 1 << x%8
        # Get the line we want to edit in the y axis
        line_buf = self.lines[y]
        # Apply the bitmask to our byte in the x axis
        if v:
            # On
            line_buf[byte] |= bitmask
        else:
            # Off
            line_buf[byte] &= ~bitmask

        # Mark the line to be sync'd later
        self.changed.add(y)

    def set_line_horizontal(self, y:int, v:bool):
        steps = (self.size.x//8)
        if v:
            self.lines[y] = bytearray(steps)
        else:
            self.lines[y] = bytearray(b'0xFF' * steps)

        # Mark the line to be sync'd later
        self.changed.add(y)

    def set_line_vertical(self, x:int, v:bool):
        # Get appropriate index of byte in line
        byte = x//8
        # Get a bitmask for the bit we want to set
        bitmask = 1 << x%8

        for y, line_buf in enumerate(self.lines):
            # Apply the bitmask to our byte in the x axis
            if v:
                # On
                line_buf[byte] |= bitmask
            else:
                # Off
                line_buf[byte] &= ~bitmask

            # Mark the line to be sync'd later
            self.changed.add(y)

    def sync(self):
        if self.spi is not None:
            self.cs.value(1)

            self.spi.write((self._vcom | self.SHARPMEM_BIT_WRITECMD).to_bytes(1, 'big'))
            self.TOGGLE_VCOM()

            while self.changed:
                ix = self.changed.pop()
                line_num = (ix+1).to_bytes(1, 'big')
                self.spi.write(line_num + self.lines[ix] + b'0x00')

            # Final byte
            self.spi.write(b'0x00')
            self.cs.value(0)

    def demo_zebra(self):
        self.clear()

        state = True
        for iy in range(self.size.y):
            self.set_line_horizontal(iy, state)
            state = not state

        self.sync()

    def demo_checker(self):
        self.clear()

        state = True
        for iy in range(self.size.y):
            self.set_line_horizontal(iy, state)
            state = not state

        state = True
        for ix in range(self.size.x):
            self.set_line_vertical(ix, state)
            state = not state

        self.sync()

    def demo_brown(self):
        self.clear()

        x = self.size.x // 2
        y = self.size.y // 2
        mls = 255 * self.size.x // 8

        on = True
        step = 0
        while step < 1000:
            if on:
                on = any(sum(line) < mls for line in self.lines)
            else:
                on = all(sum(line) == 0 for line in self.lines)
            for i in range(1000):
                x = (x + random.randint(0, 2) - 1) % self.size.x
                y = (y + random.randint(0, 2) - 1) % self.size.y
                self.set_pix(x, y, on)
                self.sync()

            step = 1000