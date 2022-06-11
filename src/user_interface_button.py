from machine import Pin
import time

from .tools_stateful_system import StatefulSystem

class UserInterfaceButton(StatefulSystem):
    def __init__(self, monitor:Pin, edge=Pin.IRQ_RISING, debounce_min_us:int = 500) -> None:
        self._debounce = time.ticks_cpu()
        self._debounce_min_us = debounce_min_us
        self._delta = False
        self._edge = edge

        self._monitor = monitor

    def init(self):
        self._monitor.irq(self._irq, self._edge)

    def deinit(self):
        self._monitor.irq(trigger=0,handler=self._irq)

    def show_inputs(self):
        print("mon: {}".format(self._monitor.value()))

    def has_changed(self):
        return self._delta

    def get_delta(self):
        delta = self._delta
        self._delta = False

        return delta

    def _irq(self, pin:Pin):
        if time.ticks_diff(time.ticks_cpu(), self._debounce) > self._debounce_min_us:
            self._debounce = time.ticks_cpu()
            self._delta = True
