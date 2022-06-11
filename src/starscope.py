import time, gc
from machine import Pin, Timer

from src.drivers.utils import Size

from .tools_stateful_system import StatefulSystem
from .user_interface_heartbeat import UserInterfaceHeartbeat
from .user_interface_display import UserInterfaceDisplay
from .user_interface_control import UserInterfaceControl
from .user_interface_encoder import UserInterfaceEncoder
from .user_interface_button import UserInterfaceButton
from .sensors import Sensors

class Starscope(StatefulSystem):
    def __init__(self) -> None:
        self._stop = False
        self.line_feed = Size()
        self.last_pos = 0
        self.printed_static = False

        self.led_hb = UserInterfaceHeartbeat(Pin(25, Pin.OUT))

        self.dial_one = UserInterfaceEncoder(Pin(10, Pin.IN, Pin.PULL_UP), Pin(11, Pin.IN, Pin.PULL_UP))
        self.dial_two = UserInterfaceEncoder(Pin(14, Pin.IN, Pin.PULL_UP), Pin(15, Pin.IN, Pin.PULL_UP))
        self.dial_three = UserInterfaceEncoder(Pin(12, Pin.IN, Pin.PULL_UP), Pin(13, Pin.IN, Pin.PULL_UP))

        self.button_one = UserInterfaceButton(Pin(18, Pin.IN, Pin.PULL_DOWN))
        self.button_two = UserInterfaceButton(Pin(19, Pin.IN, Pin.PULL_DOWN))
        self.button_three = UserInterfaceButton(Pin(20, Pin.IN, Pin.PULL_DOWN))

        self.screen = UserInterfaceDisplay()
        self.ui = UserInterfaceControl(self.screen)

        self.sensors = Sensors(Pin(8), Pin(9))

        # List of internal objects to be initialized in this order
        # and deinit'd in reverse order
        self._controls:list[StatefulSystem] = [
            self.led_hb,
            self.dial_one,
            self.dial_two,
            self.dial_three,
            self.button_one,
            self.button_two,
            self.button_three,
            self.screen,
            self.ui,
            self.sensors,
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

        # print(motion.acceleration)
        # print(motion.rates)
        # print(motion.temperature)

        if self.last_pos != self.ui.data_position:
            self.screen.clear()
            self.last_pos = self.ui.data_position
            self.printed_static = False

        if self.ui.data_position < 0:
            self.line_feed = self.screen.print('See you space cowboy...', self.line_feed)
        elif self.ui.data_position > 0:
            if not self.printed_static:
                loc = Size(self.screen.px_from_column(0), self.screen.px_from_line(-4))
                loc = self.screen.print("Time:", loc)
                loc = self.screen.print("Accel:", loc)
                loc = self.screen.print("Gyro:", loc)
                loc = self.screen.print("T.Int:", loc)

            motion = self.sensors.get_motion_data()
            loc = Size(self.screen.px_from_column(7), self.screen.px_from_line(-4))
            loc = self.screen.print(time.ticks_cpu(), loc)
            loc = self.screen.print(motion.acceleration, loc)
            loc = self.screen.print(motion.rates, loc)
            loc = self.screen.print(motion.temperature, loc)
        else:
            if not self.printed_static:
                loc = Size(44, 84)
                loc = self.screen.print("▛▀▀▀▀▀▀▀▀▀▀▀▜", loc, scaling=3)
                loc = self.screen.print("▌ Starscope ▐", loc, scaling=3)
                loc = self.screen.print("▙▄▄▄▄▄▄▄▄▄▄▄▟", loc, scaling=3)
            self.printed_static = True

        # Force a garbage collection here if needed
        gc.collect()
