from machine import Pin
from .tools_stateful_system import StatefulSystem
from .drivers.mpu6050 import MPU6050

class Sensors(StatefulSystem):
    def __init__(self, mpu_sda:Pin, mpu_scl:Pin) -> None:
        self.mpu = MPU6050(0, mpu_scl, mpu_sda)

    def init(self):
        self.mpu.init()

    def deinit(self):
        self.mpu.deinit()

    def get_motion_data(self):
        return self.mpu.get_values()
