from machine import Pin

from .user_interface_base import UserInterfaceBase
from .user_interface_display import UserInterfaceDisplay

class UserInterfaceControl(UserInterfaceBase):
    def __init__(self, display:UserInterfaceDisplay) -> None:
        self.screen = display
        self.menu_position = 0
        self.data_position = 0
        self.zoom_position = 0

    def init(self):
        pass

    def deinit(self):
        pass

    def click_menu(self):
        print('Menu click: {}'.format(self.menu_position))

        print("Fill screen!")
        self.screen.fill()

    def click_data(self):
        print('Data click: {}'.format(self.data_position))

        print("Clear screen!")
        self.screen.clear()

    def click_zoom(self):
        print('Zoom click: {}'.format(self.zoom_position))

        print("Run screen demo!")
        self.screen.run_demo()

    def scroll_menu(self, delta:int):
        self.menu_position += delta

    def scroll_data(self, delta:int):
        self.data_position += delta

    def scroll_zoom(self, delta:int):
        self.zoom_position += delta

    def update(self):
        # self.dial_one.show_inputs()
        print('Menu item: {}'.format(self.menu_position))
        print('Data item: {}'.format(self.data_position))
        print('Zoom level: {}'.format(self.zoom_position))

        self.draw_screen()

    def draw_screen(self):
        # self.screen ...
        pass