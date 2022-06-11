from machine import Pin, I2C
import math
from .utils import Vector3


#https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
REG_PWR_MGMT_1 = 0x6B
VAL_PWR_MGMT_1_RUN = 0x00

REG_CONFIG = 0x1A
VAL_CONFIG_EXT_SYNC_SET_NONE = 0x00
VAL_CONFIG_EXT_SYNC_SET_T = 0x01 << 3
VAL_CONFIG_EXT_SYNC_SET_GX = 0x02 << 3
VAL_CONFIG_EXT_SYNC_SET_GY = 0x03 << 3
VAL_CONFIG_EXT_SYNC_SET_GZ = 0x04 << 3
VAL_CONFIG_EXT_SYNC_SET_AX = 0x05 << 3
VAL_CONFIG_EXT_SYNC_SET_AY = 0x06 << 3
VAL_CONFIG_EXT_SYNC_SET_AZ = 0x07 << 3
VAL_CONFIG_DLPF_CFG_A260HZ_G256HZ = 0x00
VAL_CONFIG_DLPF_CFG_A184HZ_G188HZ = 0x01
VAL_CONFIG_DLPF_CFG_A94HZ_G98HZ = 0x02
VAL_CONFIG_DLPF_CFG_A44HZ_G42HZ = 0x03
VAL_CONFIG_DLPF_CFG_A21HZ_G20HZ = 0x04
VAL_CONFIG_DLPF_CFG_A10HZ_G10HZ = 0x05
VAL_CONFIG_DLPF_CFG_A5HZ_G5HZ = 0x06

REG_RAW_VALUES = 0x3B
LEN_RAW_VALUES = 14

REG_GYRO_CONFIG = 0x1B
VAL_GYRO_CONFIG_RANGE_250_DPS = 0x00 << 3
VAL_GYRO_CONFIG_RANGE_500_DPS = 0x01 << 3
VAL_GYRO_CONFIG_RANGE_1000_DPS = 0x02 << 3
VAL_GYRO_CONFIG_RANGE_2000_DPS = 0x03 << 3
SCALE_GYRO_CONFIG_RANGE_250_DPS = 131
SCALE_GYRO_CONFIG_RANGE_500_DPS = 65.5
SCALE_GYRO_CONFIG_RANGE_1000_DPS = 32.8
SCALE_GYRO_CONFIG_RANGE_2000_DPS = 16.4
REG_ACCEL_CONFIG = 0x1C
VAL_ACCEL_CONFIG_RANGE_2_G= 0x00 << 3
VAL_ACCEL_CONFIG_RANGE_4_G= 0x01 << 3
VAL_ACCEL_CONFIG_RANGE_8_G= 0x02 << 3
VAL_ACCEL_CONFIG_RANGE_16_G= 0x03 << 3
SCALE_ACCEL_CONFIG_RANGE_2_G= 16384
SCALE_ACCEL_CONFIG_RANGE_4_G= 8192
SCALE_ACCEL_CONFIG_RANGE_8_G= 4096
SCALE_ACCEL_CONFIG_RANGE_16_G= 2048

SCALE_TEMP_MUL = 1/340
SCALE_TEMP_OFFSET = 36.53

class MPU6050Data():
    def __init__(self, data:bytes, gyro_scaling:float=1.0, accel_scaling:float=1.0) -> None:
        self.acceleration = Vector3()
        self.rates = Vector3()
        self.temperature = 0
        self.gyro_scaling = gyro_scaling
        self.accel_scaling = accel_scaling

        if len(data) == LEN_RAW_VALUES:
            self.from_raw_data(data)

    def _bytes_to_int(self, first_byte, second_byte):
        if not first_byte & 0x80:
            return first_byte << 8 | second_byte
        return - (((first_byte ^ 255) << 8) | (second_byte ^ 255) + 1)

    def _data_decoder(self, data:bytes):
        count = 0
        while count < LEN_RAW_VALUES:
            yield self._bytes_to_int(data[count], data[count + 1])
            count += 2

    def from_raw_data(self, data:bytes):
        decoder = self._data_decoder(data)
        self.acceleration = Vector3(
            next(decoder) * self.accel_scaling,
            next(decoder) * self.accel_scaling,
            next(decoder) * self.accel_scaling
        )
        self.temperature = next(decoder) * SCALE_TEMP_MUL + SCALE_TEMP_OFFSET
        self.rates = Vector3(
            next(decoder) * self.gyro_scaling,
            next(decoder) * self.gyro_scaling,
            next(decoder) * self.gyro_scaling
        )

class MPU6050():
    def __init__(self, id:int, scl:Pin, sda:Pin, addr=0x68):
        self.i2c = I2C(id, scl=scl, sda=sda, freq=400000)
        self.addr = addr
        self.scale_accel = 1.0
        self.scale_gyro = 1.0
        self._scale_deg_to_rad = math.pi / 180
        self._scale_g_to_ms2 = 9.80665

    def init(self):
        self._write([REG_PWR_MGMT_1, VAL_PWR_MGMT_1_RUN])
        self._write([REG_CONFIG, VAL_CONFIG_EXT_SYNC_SET_NONE | VAL_CONFIG_DLPF_CFG_A10HZ_G10HZ])
        self._write([REG_GYRO_CONFIG, VAL_GYRO_CONFIG_RANGE_500_DPS])
        self._write([REG_ACCEL_CONFIG, VAL_ACCEL_CONFIG_RANGE_4_G])

        self.scale_gyro = self._scale_deg_to_rad / SCALE_GYRO_CONFIG_RANGE_500_DPS
        self.scale_accel = self._scale_g_to_ms2 / SCALE_ACCEL_CONFIG_RANGE_4_G

    def deinit(self):
        # self.i2c.deinit()
        pass

    def _write(self, data:list[int]):
        # self.i2c.start()
        self.i2c.writeto(self.addr, bytes(data))
        # self.i2c.stop()

    def _read(self, mem_addr:int, count:int):
        # self.i2c.start()
        a = self.i2c.readfrom_mem(self.addr, mem_addr, count)
        # self.i2c.stop()
        return a

    def get_raw_values(self):
        return self._read(REG_RAW_VALUES, LEN_RAW_VALUES)

    # def get_ints(self):
    #     b = self.get_raw_values()
    #     c = []
    #     for i in b:
    #         c.append(i)
    #     return c

    def get_values(self):
        return MPU6050Data(self.get_raw_values(), self.scale_gyro, self.scale_accel)
