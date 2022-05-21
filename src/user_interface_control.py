from machine import Pin
from .user_interface_encoder import UserInterfaceEncoder
from .user_interface_button import UserInterfaceButton

class UserInterfaceControl():
    def __init__(self) -> None:
        self.dial_one = UserInterfaceEncoder(Pin(10, Pin.IN, Pin.PULL_UP), Pin(11, Pin.IN, Pin.PULL_UP))
        self.dial_two = UserInterfaceEncoder(Pin(12, Pin.IN, Pin.PULL_UP), Pin(13, Pin.IN, Pin.PULL_UP))
        self.dial_three = UserInterfaceEncoder(Pin(14, Pin.IN, Pin.PULL_UP), Pin(15, Pin.IN, Pin.PULL_UP))

        self.button_one = UserInterfaceButton(Pin(18, Pin.IN, Pin.PULL_DOWN))
        self.button_two = UserInterfaceButton(Pin(19, Pin.IN, Pin.PULL_DOWN))
        self.button_three = UserInterfaceButton(Pin(20, Pin.IN, Pin.PULL_DOWN))

        self.menu_position = 0
        self.data_position = 0
        self.zoom_position = 0

    def shutdown(self):
        pass

    def update(self):
        # self.dial_one.show_inputs()

        if self.dial_one.has_changed():
            self.menu_position += self.dial_one.get_delta()
            print('Menu item: {}'.format(self.menu_position))

        if self.dial_two.has_changed():
            self.data_position += self.dial_two.get_delta()
            print('Data item: {}'.format(self.data_position))

        if self.dial_three.has_changed():
            self.zoom_position += self.dial_three.get_delta()
            print('Zoom level: {}'.format(self.zoom_position))

        if self.button_one.has_changed():
            self.button_one.get_delta()
            print('Menu click: {}'.format(self.menu_position))

        if self.button_two.has_changed():
            self.button_two.get_delta()
            print('Data click: {}'.format(self.data_position))

        if self.button_three.has_changed():
            self.button_three.get_delta()
            print('Zoom click: {}'.format(self.zoom_position))