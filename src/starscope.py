import machine

from .user_interface_display import UserInterfaceDisplay
from .user_interface_control import UserInterfaceControl

class Starscope():
    def __init__(self) -> None:
        self.ui = UserInterfaceControl()
        self.timer = machine.Timer()
        self.screen = UserInterfaceDisplay()

        print("--== Starscope ==--")

    def begin(self, update_period=100):
        self.ui.begin()
        self.screen.begin()
        self.timer.init(period=update_period, callback=self.update)

    def update(self, timer):
        self.ui.update()

    def shutdown(self):
        if self.timer:
            self.timer.deinit()

        if self.screen:
            self.screen.shutdown()

        if self.ui:
            self.ui.shutdown()