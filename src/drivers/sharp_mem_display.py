#XXX: Typing module not supported by micropython, comment below to not break things at runtime!
# from typing import Iterator, MutableSet
from machine import Pin, SPI
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
    def __init__(self, x:int=0, y:int=0) -> None:
        self.x = x
        self.y = y

    def __str__(self):
        return "Size: [{},{}]".format(self.x, self.y)

class SharpMemDisplay:
    # Lookup table for byte values in LSB format
    BIT_FLIP = bytes([
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
        0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
        0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
        0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
        0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
        0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
        0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
        0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
        0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
        0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
        0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
        0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
        0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
        0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
        0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
        0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
    ])

    SHARPMEM_BIT_WRITECMD = 0x01 # 0x80 in LSB format
    SHARPMEM_BIT_VCOM = 0x02     # 0x40 in LSB format
    SHARPMEM_BIT_CLEAR = 0x04    # 0x20 in LSB format

    # XXX: By default all of the pixel operations are inverted (0x01 is off)
    SHARPMEM_DUMMY_BYTE = 0

    SHARPMEM_PIXEL_BYTE_OFF = 0xFF
    SHARPMEM_PIXEL_BYTE_ON = 0x00

    def __init__(self, spi_id:int, sck:Pin, mosi:Pin, cs:Pin, size:Size):
        self.spi = None
        self._vcom = SharpMemDisplay.SHARPMEM_BIT_VCOM

        self.spi_id = spi_id
        self.sck = sck
        self.mosi = mosi
        self.cs = cs
        self.cs.value(0)

        self.size = size
        self.lines = [[0xFF]*(self.size.x//8) for i in range(self.size.y)]
        self.changed:MutableSet[int] = set() #type: ignore

    def TOGGLE_VCOM(self):
        self._vcom = 0x00 if self._vcom else SharpMemDisplay.SHARPMEM_BIT_VCOM

    def init(self):
        self._vcom = SharpMemDisplay.SHARPMEM_BIT_VCOM
        self.spi = SPI(self.spi_id, 2_000_000, sck=self.sck, mosi=self.mosi)

    def deinit(self):
        if self.spi:
            self.spi.deinit()

    # def _flip_bytes(self, data:bytes):
    #     for d in data:
    #         yield SharpMemDisplay.BIT_FLIP[d]

    def _write(self, data:Iterator[int]): #type: ignore
        if self.spi is not None:
            # out = data if not is_flipped else self.flip_bytes(data)
            # print(("Flipped: " if is_flipped else "As is: ") + str(len(out)) + " | " + str(out))
            # self.spi.write(data if not is_flipped else self.flip_bytes(data))
            self.spi.write(bytes(data))

    # Clears the internal buffer
    def clear(self, set_changed:bool = True):
        for i in range(self.size.y):
            self.set_line_horizontal(i, False, set_changed)

        # for i in range(self.size.y):
        #     self.set_line_horizontal(i, False)
        # self.set_all(0 for i in range(self.size.x * self.size.y // 8))

    def fill(self, set_changed:bool = True):
        for i in range(self.size.y):
            self.set_line_horizontal(i, True, set_changed)
        # self.set_all(1 for i in range(self.size.x * self.size.y // 8))

    def get_pixel(self, x, y):
        return not bool(self.lines[y][x//8] & (1 << x%8))

    def set_pixel(self, x:int, y:int, v:bool, set_changed:bool = True):
        # if (x < 0) or (x >= self.size.x) or (y < 0) or (y >= self.size.y):
        #     raise IndexError("[{}, {}] is out of range of screen size: {}".format(x, y, self.size))

        # Get appropriate index of byte in line
        byte = x//8
        # Get a bitmask for the bit we want to set
        bitmask = 1 << x%8
        # Get the line we want to edit in the y axis
        line_buf = self.lines[y]
        # Apply the bitmask to our byte in the x axis
        if v:
            # On
            line_buf[byte] &= ~bitmask
        else:
            # Off
            line_buf[byte] |= bitmask

        # Mark the line to be sync'd later
        if set_changed:
            self.changed.add(y)

    def set_line_horizontal(self, y:int, v:bool, set_changed:bool = True):
        steps = (self.size.x//8)
        # for i in self.lines[y]:
        #     i = SharpMemDisplay.SHARPMEM_PIXEL_BYTE_ON if v else SharpMemDisplay.SHARPMEM_PIXEL_BYTE_OFF
        if v:
            self.lines[y] = [SharpMemDisplay.SHARPMEM_PIXEL_BYTE_ON] * steps
        else:
            self.lines[y] = [SharpMemDisplay.SHARPMEM_PIXEL_BYTE_OFF] * steps

        # Mark the line to be sync'd later
        if set_changed:
            self.changed.add(y)

    def set_line_vertical(self, x:int, v:bool, set_changed:bool = True):
        # Get appropriate index of byte in line
        byte = x//8
        # Get a bitmask for the bit we want to set
        bitmask = 1 << x%8

        for y, line_buf in enumerate(self.lines):
            # Apply the bitmask to our byte in the x axis
            if v:
                # On
                line_buf[byte] &= ~bitmask
            else:
                # Off
                line_buf[byte] |= bitmask

            # Mark the line to be sync'd later
            if set_changed:
                self.changed.add(y)

    def _clear_sync_generator(self):
        yield SharpMemDisplay.BIT_FLIP[self._vcom | SharpMemDisplay.SHARPMEM_BIT_CLEAR]
        yield SharpMemDisplay.BIT_FLIP[SharpMemDisplay.SHARPMEM_DUMMY_BYTE]

    def _sync_generator(self):
        yield SharpMemDisplay.BIT_FLIP[self._vcom | self.SHARPMEM_BIT_WRITECMD]

        while self.changed:
            ix = self.changed.pop()
            # print("Line: " + str(ix))

            # for i in bytes([ix+1]):
            #     yield i
            yield SharpMemDisplay.BIT_FLIP[ix + 1]

            for i in self.lines[ix]:
                yield SharpMemDisplay.BIT_FLIP[i]

            yield SharpMemDisplay.BIT_FLIP[SharpMemDisplay.SHARPMEM_DUMMY_BYTE]

        # Final byte
        # print("Tail")
        yield SharpMemDisplay.BIT_FLIP[SharpMemDisplay.SHARPMEM_DUMMY_BYTE]

    # Clears the entire screen with a hardware wipe
    def clear_sync(self):
        self.cs.value(1)

        # data = bytes([self._vcom | SharpMemDisplay.SHARPMEM_BIT_CLEAR, SharpMemDisplay.SHARPMEM_DUMMY_BYTE])
        data = self._clear_sync_generator()
        self._write(data)

        self.TOGGLE_VCOM()
        self.cs.value(0)

        self.clear(False)

    def sync(self):
        self.cs.value(1)
        sync_data = self._sync_generator()
        # print("Head")
        self._write(sync_data)

        # out = bytearray()

        # out += b'0x00'
        # self.write(out)
        self.TOGGLE_VCOM()
        self.cs.value(0)

    def demo_zebra(self):
        self.clear()

        state = True
        for iy in range(self.size.y):
            self.set_line_horizontal(iy, state)
            state = not state

        # self.sync()

    def demo_checker(self):
        self.clear()

        state = True
        for iy in range(self.size.y):
            self.set_line_horizontal(iy, state)
            state = not state

        state = True
        for ix in range(self.size.x):
            if(state):
                self.set_line_vertical(ix, state)
            state = not state

        # self.sync()

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
                self.set_pixel(x, y, on)
                # self.sync()

            step = 1000
