from .tools_stateful_system import StatefulSystem
from .user_interface_display import UserInterfaceDisplay

class UserInterfaceControl(StatefulSystem):
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
        print(f'Menu click: {self.menu_position}')

        print("Fill screen!")
        self.screen.fill()

    def click_data(self):
        print(f'Data click: {self.data_position}')

        print("Clear screen!")
        self.screen.clear()

    def click_zoom(self):
        print(f'Zoom click: {self.zoom_position}')

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
        print(f'Menu item: {self.menu_position}')
        print(f'Data item: {self.data_position}')
        print(f'Zoom level: {self.zoom_position}')

        self.draw_screen()

    def draw_screen(self):
        # self.screen ...
        pass
