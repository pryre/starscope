from machine import Pin
import time
import micropython

micropython.alloc_emergency_exception_buf(100)

class UserInterfaceEncoder():
    def __init__(self, monitor:Pin, direction:Pin, monitor_is_positive:bool = True, debounce_min_us:int = 3000) -> None:
        self._delta = 0
        self._direction_adjust = -1 if monitor_is_positive else 1
        self._debounce = time.ticks_cpu()
        self._debounce_min_us = debounce_min_us

        self._monitor = monitor
        self._direction = direction

        self._monitor.irq(self._irq, Pin.IRQ_FALLING, hard=True)

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
            self._delta = self._direction_adjust * (1 if self._direction.value()  == self._monitor.value() else -1)
