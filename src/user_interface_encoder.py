from machine import Pin
import time
import micropython

from .user_interface_base import UserInterfaceBase

micropython.alloc_emergency_exception_buf(100)

class UserInterfaceEncoder(UserInterfaceBase):
    def __init__(self, monitor:Pin, direction:Pin, monitor_is_positive:bool = True, debounce_min_us:int = 500) -> None:
        self._delta = 0
        self._rotary_state = 0
        self._direction_adjust = -1 if monitor_is_positive else 1
        self._debounce = time.ticks_cpu()
        self._debounce_min_us = debounce_min_us

        self._monitor = monitor
        self._direction = direction

    def init(self):
        self._monitor.irq(self._irq, Pin.IRQ_FALLING | Pin.IRQ_RISING, hard=True)

    def deinit(self):
        self._monitor.irq(trigger=0,handler=self._irq)

    def show_inputs(self):
        print("mon: {}; dir: {}".format(self._monitor.value(), self._direction.value()))

    def has_changed(self):
        return self._delta != 0

    def get_delta(self):
        delta = self._delta
        self._delta = 0

        return delta

    def _irq(self, pin:Pin):
        if time.ticks_diff(time.ticks_cpu(), self._debounce) > self._debounce_min_us:
            self._debounce = time.ticks_cpu()
            # self._delta = self._direction_adjust * (1 if self._direction.value()  == self._monitor.value() else -1)


            """
            # https://github.com/sparkfun/Rotary_Encoder_Breakout-Illuminated/blob/main/Firmware/RGB_Rotary_Encoder/RGB_Rotary_Encoder.ino
            // Process input from the rotary encoder.
            // The rotary "position" is held in rotary_counter, increasing for CW rotation (changes by one per detent).
            // If the position changes, rotary_change will be set true. (You may manually set this to false after handling the change).

            // This function will automatically run when rotary encoder input A transitions in either direction (low to high or high to low)
            // By saving the state of the A and B pins through two interrupts, we'll determine the direction of rotation
            // int rotary_counter will be updated with the new value, and boolean rotary_change will be true if there was a value change
            // Based on concepts from Oleg at circuits@home (http://www.circuitsathome.com/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros)
            // Unlike Oleg's original code, this code uses only one interrupt and has only two transition states;
            // it has less resolution but needs only one interrupt, is very smooth, and handles switchbounce well.
            """

            self._rotary_state <<= 2    # remember previous state
            self._rotary_state |= self._direction.value() | (self._monitor.value() << 1)  # mask in current state
            self._rotary_state &= 0x0F  # zero upper nybble

            # print(rotary_state,HEX);

            if self._rotary_state == 0x09: # from 10 to 01, increment counter. Also try 0x06 if unreliable
                self._delta += self._direction_adjust
                # rotary_change = True
            elif self._rotary_state == 0x03: # from 00 to 11, decrement counter. Also try 0x0C if unreliable
                self._delta += -self._direction_adjust
                # rotary_change = True
