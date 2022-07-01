
from machine import Pin, Timer

from .tools_stateful_system import StatefulSystem

class UserInterfaceHeartbeat(StatefulSystem):
    def __init__(self, led:Pin, interval_ms:int = 500) -> None:
        self.timer = Timer()
        self.led = led
        self.interval_ms = interval_ms

    def init(self):
        self.timer.init(period=self.interval_ms, callback=self.update)
        self.led.value(1)

    def deinit(self):
        self.timer.deinit()
        self.led.value(0)

    def update(self, timer):
        self.led.toggle()
