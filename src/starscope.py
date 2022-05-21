import machine
from .user_interface_control import UserInterfaceControl

class Starscope():
    def __init__(self) -> None:
        self.ui = UserInterfaceControl()
        self.timer = machine.Timer()
        self.timer.init(period=100, callback=self.update)

        print("--== Starscope ==--")

    def update(self, timer):
        self.ui.update()

    def shutdown(self):
        if self.timer:
            self.timer.deinit()

        if self.ui:
            self.ui.shutdown()