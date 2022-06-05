import time, gc
from machine import Pin, Timer

from .user_interface_base import UserInterfaceBase
from .user_interface_heartbeat import UserInterfaceHeartbeat
from .user_interface_display import UserInterfaceDisplay
from .user_interface_control import UserInterfaceControl
from .user_interface_encoder import UserInterfaceEncoder
from .user_interface_button import UserInterfaceButton

class Starscope(UserInterfaceBase):
    def __init__(self) -> None:
        self._stop = False

        self.led_hb = UserInterfaceHeartbeat(Pin(25, Pin.OUT))

        self.dial_one = UserInterfaceEncoder(Pin(10, Pin.IN, Pin.PULL_UP), Pin(11, Pin.IN, Pin.PULL_UP))
        self.dial_two = UserInterfaceEncoder(Pin(14, Pin.IN, Pin.PULL_UP), Pin(15, Pin.IN, Pin.PULL_UP))
        self.dial_three = UserInterfaceEncoder(Pin(12, Pin.IN, Pin.PULL_UP), Pin(13, Pin.IN, Pin.PULL_UP))

        self.button_one = UserInterfaceButton(Pin(18, Pin.IN, Pin.PULL_DOWN))
        self.button_two = UserInterfaceButton(Pin(19, Pin.IN, Pin.PULL_DOWN))
        self.button_three = UserInterfaceButton(Pin(20, Pin.IN, Pin.PULL_DOWN))

        self.screen = UserInterfaceDisplay()
        self.ui = UserInterfaceControl(self.screen)

        # List of internal objects to be initialized in this order
        # and deinit'd in reverse order
        self._controls:list[UserInterfaceBase] = [
            self.led_hb,
            self.dial_one,
            self.dial_two,
            self.dial_three,
            self.button_one,
            self.button_two,
            self.button_three,
            self.screen,
            self.ui
        ]

        self.timer = Timer()

        print("--== Starscope ==--")

    def init(self, update_period=100):
        for control in self._controls:
            control.init()

        # self.led_hb.init()

        # self.dial_one.init()
        # self.dial_two.init()
        # self.dial_three.init()

        # self.button_one.init()
        # self.button_two.init()
        # self.button_three.init()

        # self.screen.init()
        # self.ui.init()

        # Finally initialize the runtime timer
        self.timer.init(period=update_period, callback=self.update)

    def deinit(self):
        # First, de-initialize the runtime timer
        if self.timer:
            self.timer.deinit()

        for control in reversed(self._controls): #type: ignore
            if control:
                control.deinit()

        # self.led_hb.init()

        # if self.dial_one:
        #     self.dial_one.deinit()

        # if self.dial_two:
        #     self.dial_two.deinit()

        # if self.dial_three:
        #     self.dial_three.deinit()

        # if self.button_one:
        #     self.button_one.deinit()

        # if self.button_two:
        #     self.button_two.deinit()

        # if self.button_three:
        #     self.button_three.deinit()

        # if self.screen:
        #     self.screen.deinit()

        # if self.ui:
        #     self.ui.deinit()

    def stop(self):
        self._stop = True

    def run(self):
        while not self._stop:
            time.sleep(1)

    def update(self, timer):
        if self.dial_one.has_changed():
            self.ui.scroll_menu(self.dial_one.get_delta())

        if self.dial_two.has_changed():
            self.ui.scroll_data(self.dial_two.get_delta())

        if self.dial_three.has_changed():
            self.ui.scroll_zoom(self.dial_three.get_delta())

        if self.button_one.has_changed():
            self.button_one.get_delta()
            self.ui.click_menu()

        if self.button_two.has_changed():
            self.button_two.get_delta()
            self.ui.click_data()

        if self.button_three.has_changed():
            self.button_three.get_delta()
            self.ui.click_zoom()

        # Force a garbage collection here if needed
        gc.collect()
