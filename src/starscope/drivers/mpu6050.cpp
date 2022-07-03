#include "starscope/drivers/mpu6050.hpp"
#include <bitset>

namespace Starscope::Drivers::MPU6050 {
Data::Data(float gyro_scaling = 1.0, float accel_scaling = 1.0) :
    acceleration(Utils::Vector3()),
    rates(Utils::Vector3()),
    temperature(0),
    gyro_scaling(gyro_scaling),
    accel_scaling(accel_scaling) {
}


//Updates the internal data from a data packet
bool Data::from_raw_data(const std::span<const std::byte, LEN_RAW_VALUES> &data) {
    std::span<const std::byte> raw = data.subspan(0, data.size() - 2);

    acceleration = Utils::Vector3(
        data_decoder(raw) * accel_scaling,
        data_decoder(raw) * accel_scaling,
        data_decoder(raw) * accel_scaling
    );

    temperature = data_decoder(raw) * SCALE_TEMP_MUL + SCALE_TEMP_OFFSET;

    rates = Utils::Vector3(
        data_decoder(raw) * gyro_scaling,
        data_decoder(raw) * gyro_scaling,
        data_decoder(raw) * gyro_scaling
    );

    return true;
}

uint16_t Data::bytes_to_value(const std::byte first_byte, const std::byte second_byte) const {
    return  !(bool)(first_byte & VAL_INVERT_RESPONSE) ?
        ((uint8_t)first_byte << 8) | (uint8_t)second_byte :
        -(((uint8_t)flip(first_byte) << 8) | ((uint8_t)flip(second_byte)) + 1);
}

uint16_t Data::data_decoder(std::span<const std::byte> &data) const {
        uint16_t value = 0;

        if(data.size() >= 2) {
            value = bytes_to_value(data[0], data[1]);
            //Reduce the span size (consume)
            data = data.subspan(2, data.size() - 2);
        } else {
            data = std::span<const std::byte>{};
        }

        return value;
}

Driver::Driver(i2c_inst_t* i2c, std::byte addr) :
    _i2c(i2c),
    _addr(addr),
    _scale_accel(1.0),
    _scale_gyro(1.0) {
}

std::array<std::byte, LEN_RAW_VALUES> Driver::get_raw_values() const {
    std::array<std::byte, LEN_RAW_VALUES> data;
    int ret;
    ret = i2c_read_blocking(_i2c, (uint8_t)_addr, static_cast<uint8_t*>(data.data()), LEN_RAW_VALUES, false);

    return data;
}

Data Driver::get_values() const {
    //TODO: Stuff
}

bool Driver::_init() {
    write(REG_PWR_MGMT_1, VAL_PWR_MGMT_1_RUN);
    write(REG_CONFIG, VAL_CONFIG_EXT_SYNC_SET_NONE | VAL_CONFIG_DLPF_CFG_A10HZ_G10HZ);
    set_scaling(); //Set to default scaling
}

void Driver::set_scaling(const ACCEL_RANGE accel_scaling = ACCEL_RANGE::A_4G, const GYRO_RANGE gyro_scaling = GYRO_RANGE::G_500_DPS) {
    switch(accel_scaling) {
        case ACCEL_RANGE::A_2G: {
            write(REG_ACCEL_CONFIG, VAL_ACCEL_CONFIG_RANGE_2_G);
            _scale_accel = SCALE_G_TO_MS2 / SCALE_ACCEL_CONFIG_RANGE_2_G;
            break;
        }
        case ACCEL_RANGE::A_4G: {
            write(REG_ACCEL_CONFIG, VAL_ACCEL_CONFIG_RANGE_4_G);
            _scale_accel = SCALE_G_TO_MS2 / SCALE_ACCEL_CONFIG_RANGE_4_G;
            break;
        }
        case ACCEL_RANGE::A_8G: {
            write(REG_ACCEL_CONFIG, VAL_ACCEL_CONFIG_RANGE_8_G);
            _scale_accel = SCALE_G_TO_MS2 / SCALE_ACCEL_CONFIG_RANGE_8_G;
            break;
        }
        case ACCEL_RANGE::A_16G: {
            write(REG_ACCEL_CONFIG, VAL_ACCEL_CONFIG_RANGE_16_G);
            _scale_accel = SCALE_G_TO_MS2 / SCALE_ACCEL_CONFIG_RANGE_16_G;
            break;
        }
    }

    switch(gyro_scaling) {
        case GYRO_RANGE::G_250_DPS : {
            write(REG_GYRO_CONFIG, VAL_GYRO_CONFIG_RANGE_250_DPS);
            _scale_gyro = SCALE_DEG_TO_RAD / SCALE_GYRO_CONFIG_RANGE_250_DPS;
            break;
        }
        case GYRO_RANGE::G_500_DPS: {
            write(REG_GYRO_CONFIG, VAL_GYRO_CONFIG_RANGE_500_DPS);
            _scale_gyro = SCALE_DEG_TO_RAD / SCALE_GYRO_CONFIG_RANGE_500_DPS;
            break;
        }
        case GYRO_RANGE::G_1000_DPS: {
            write(REG_GYRO_CONFIG, VAL_GYRO_CONFIG_RANGE_1000_DPS);
            _scale_gyro = SCALE_DEG_TO_RAD / SCALE_GYRO_CONFIG_RANGE_1000_DPS;
            break;
        }
        case GYRO_RANGE::G_2000_DPS: {
            write(REG_GYRO_CONFIG, VAL_GYRO_CONFIG_RANGE_2000_DPS);
            _scale_gyro = SCALE_DEG_TO_RAD / SCALE_GYRO_CONFIG_RANGE_2000_DPS;
            break;
        }
    }
}

void Driver::_deinit() {
}

void Driver::write(const std::byte mem_addr, const std::byte data) const {
    const uint8_t src[2] = {(uint8_t)mem_addr, (uint8_t)data};
    i2c_write_blocking(_i2c, (uint8_t)_addr, src, 2, false);
}

}

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
